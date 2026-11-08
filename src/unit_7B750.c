#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct ALFilt2_ { struct ALFilt2_ *source; Acmd_ *(*handler)(void *, s16 *, s32, s32, Acmd_ *); s32 (*setParam)(void *, s32, void *); } ALFilt2_;
typedef struct { ALFilt2_ filter; u8 padC[8]; s32 sourceCount; s32 maxSources; ALFilt2_ **sources; } ALBus2_;
typedef struct Filt_ { struct Filt_ *source; Acmd_ *(*handler)(struct Filt_ *, s16 *, s32, s32, Acmd_ *); } Filt_;
typedef struct { Filt_ *source; u8 p4[0x10]; u32 state; f32 ratio; s32 upitch; f32 delta; s32 first; } Resampler_;
u32 osVirtualToPhysical(u32);
typedef struct ALFilter_ { struct ALFilter_ *source; void *handler; s32 (*setParam)(void *, s32, void *); } ALFilter_;
typedef struct { u8 pad0[0x18]; void *ctrlList; s32 unk1C; s32 unk20; s32 unk24; u8 pad28[8]; s32 unk30; } ALMix_;



Acmd_ *alMainBusPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd_ *p) {
    Acmd_ *ptr = p;
    ALBus2_ *m = (ALBus2_ *)filter;
    ALFilt2_ **sources = m->sources;
    s32 i;

    { Acmd_ *_a = ptr++; _a->w0 = 0x02000440; _a->w1 = (u32)(outCount << 1); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x02000580; _a->w1 = (u32)(outCount << 1); }
    for (i = 0; i < m->sourceCount; i++) {
        ptr = (sources[i]->handler)(sources[i], outp, outCount, sampleOffset, ptr);
        { Acmd_ *_a = ptr++; _a->w0 = 0x08000000; _a->w1 = (u32)((outCount << 1) & 0xFFFF); }
        { Acmd_ *_a = ptr++; _a->w0 = 0x0C007FFF; _a->w1 = 0x06C00440; }
        { Acmd_ *_a = ptr++; _a->w0 = 0x0C007FFF; _a->w1 = 0x08000580; }
    }
    return ptr;
}

s32 func_800DAC90(void *filter, s32 paramID, void *param) {
    ALBus2_ *m = (ALBus2_ *)filter;
    ALFilt2_ **sources = m->sources;

    switch (paramID) {
        case 2:
            sources[m->sourceCount++] = (ALFilt2_ *)param;
            break;
        default:
            break;
    }
    return 0;
}

Acmd_ *alResamplePull(Resampler_ *f, u16 *outp, s32 outCount, s32 sampleOffset, Acmd_ *p) {
    Acmd_ *ptr = p;
    s16 inp;
    s32 inCount;
    Filt_ *source = f->source;
    s32 incr;
    f32 finCount;

    inp = 0x140;
    if (!outCount) {
        return ptr;
    }
    if (f->upitch) {
        ptr = source->handler(source, &inp, outCount, sampleOffset, p);
        {
            Acmd_ *a = ptr++;
            a->w0 = (10 << 24) | (inp & 0xFFFFFF);
            a->w1 = (*outp << 16) | ((outCount << 1) & 0xFFFF);
        }
    } else {
        if (f->ratio > 1.99996) {
            f->ratio = 1.99996;
        }
        f->ratio = (s32)(f->ratio * 32768.0f);
        f->ratio = f->ratio / 32768.0f;
        finCount = f->delta + (f->ratio * (f32)outCount);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;
        ptr = source->handler(source, &inp, inCount, sampleOffset, p);
        incr = (s32)(f->ratio * 32768.0f);
        {
            Acmd_ *a = ptr++;
            a->w0 = (8 << 24) | (inp & 0xFFFF);
            a->w1 = (*outp << 16) | ((outCount << 1) & 0xFFFF);
        }
        {
            Acmd_ *a = ptr++;
            a->w0 = (5 << 24) | ((f->first & 0xFF) << 16) | (incr & 0xFFFF);
            a->w1 = osVirtualToPhysical(f->state);
        }
        f->first = 0;
    }
    return ptr;
}

s32 alResampleParam(void *filter, s32 param, void *value) {
    ALFilter_ *f = filter;
    ALMix_ *e = filter;

    switch (param) {
    case 1:
        f->source = value;
        break;
    case 4:
        e->unk20 = 0;
        e->unk24 = 1;
        e->unk30 = 0;
        e->unk1C = 0;
        if (f->source != NULL) {
            f->source->setParam(f->source, 4, NULL);
        }
        break;
    case 9:
        e->unk30 = 1;
        if (f->source != NULL) {
            f->source->setParam(f->source, 9, NULL);
        }
        break;
    case 7:
        e->ctrlList = value;
        break;
    case 8:
        e->unk1C = 1;
        break;
    default:
        if (f->source != NULL) {
            f->source->setParam(f->source, param, value);
        }
        break;
    }
    return 0;
}
