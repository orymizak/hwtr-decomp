#include "common.h"
#include "include_asm.h"
typedef struct ALParam8_ { struct ALParam8_ *next; s32 delta; s16 type; union { f32 f; s32 i; } data; union { f32 f; s32 i; } moredata; } ALParam8_;
typedef struct { void *next; s32 delta; s16 type; s16 unity; f32 pitch; s16 volume; u8 pan; u8 fxMix; s32 samples; void *wave; } StartParamAlt8_;
typedef struct { void *next; s32 delta; s16 type; s16 unity; void *wave; } StartParam8_;
typedef struct { u8 pad[0xD8]; s32 offset; } PVoice8_;
typedef struct { void *next; s32 delta; s16 type; PVoice8_ *pvoice; } FreeParam8_;
typedef struct { u32 w0; u32 w1; } Acmd8_;
typedef struct ALFilt8_ { struct ALFilt8_ *source; Acmd8_ *(*handler)(void *, s16 *, s32, s32, Acmd8_ *); s32 (*setParam)(void *, s32, void *); u8 pad[8]; } ALFilt8_;
typedef struct { ALFilt8_ filter; void *state; s16 pan; s16 volume; s16 cvolL; s16 cvolR; s16 dryamt; s16 wetamt; u16 lratl; s16 lratm; s16 ltgt; u16 rratl; s16 rratm; s16 rtgt; s32 delta; s32 segEnd; s32 first; struct ALParam8_ *ctrlList; struct ALParam8_ *ctrlTail; void **sources; s32 motion; } EnvMix8_;
typedef struct ALFilt3_ { struct ALFilt3_ *source; void *handler; s32 (*setParam)(void *, s32, void *); } ALFilt3_;
typedef struct ALParam3_ { struct ALParam3_ *next; } ALParam3_;
typedef struct { ALFilt3_ filter; u8 padC[8]; s32 state; s16 pan; s16 volume; u8 pad1C[0x1C]; s32 first; ALParam3_ *ctrlList; ALParam3_ *ctrlTail; void **sources; s32 motion; } ALEnvMix3_;



extern s16 D_800FB490[];
extern void *D_800FB5B0;
Acmd8_ *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd8_ *p);
f32 _getVol(f32 arg0, s32 arg1, s32 arg2, s32 arg3);
void _freePVoice(s32 arg0, s32 arg1);
void __freeParam(void *param);
Acmd8_ *alEnvmixerPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd8_ *p) {
    Acmd8_ *ptr = p;
    EnvMix8_ *e = (EnvMix8_ *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    ALParam8_ *thisParam;

    inp = 0;
    while (e->ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->ctrlList->delta;
        samples = thisOffset - lastOffset;
        if (samples > outCount)
            break;
        switch (e->ctrlList->type) {
            case 13:
                {
                    StartParamAlt8_ *param = (StartParamAlt8_ *)e->ctrlList;
                    ALFilt8_ *f = (ALFilt8_ *)e;
                    s32 tmp;

                    if (param->unity) {
                        (*e->filter.setParam)(&e->filter, 8, 0);
                    }
                    (*e->filter.setParam)(&e->filter, 5, param->wave);
                    (*e->filter.setParam)(&e->filter, 9, 0);
                    e->first = 1;
                    e->delta = 0;
                    e->segEnd = param->samples;
                    tmp = ((s32)param->volume * (s32)param->volume) >> 15;
                    e->volume = (s16)tmp;
                    e->pan = param->pan;
                    e->dryamt = D_800FB490[param->fxMix];
                    e->wetamt = D_800FB490[128 - param->fxMix - 1];
                    if (param->samples) {
                        e->cvolL = 1;
                        e->cvolR = 1;
                    } else {
                        e->cvolL = (e->volume * D_800FB490[e->pan]) >> 15;
                        e->cvolR = (e->volume * D_800FB490[128 - e->pan - 1]) >> 15;
                    }
                    if (f->source) {
                        union {
                            f32 f;
                            s32 i;
                        } data;
                        data.f = param->pitch;
                        (*f->source->setParam)(f->source, 7, (void *)data.i);
                    }
                }
                break;
            case 16:
            case 12:
            case 11:
                ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * D_800FB490[e->pan]) >> 15;
                    e->rtgt = (e->volume * D_800FB490[128 - e->pan - 1]) >> 15;
                    e->delta = e->segEnd;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                } else {
                    e->cvolL = _getVol(e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = _getVol(e->cvolR, e->delta, e->rratm, e->rratl);
                }
                if (e->cvolL == 0) e->cvolL = 1;
                if (e->cvolR == 0) e->cvolR = 1;
                if (e->ctrlList->type == 12)
                    e->pan = (s16)e->ctrlList->data.i;
                if (e->ctrlList->type == 11) {
                    e->delta = 0;
                    fVol = (e->ctrlList->data.i);
                    fVol = (fVol * fVol) >> 15;
                    e->volume = (s16)fVol;
                    e->segEnd = e->ctrlList->moredata.i;
                }
                if (e->ctrlList->type == 16) {
                    e->dryamt = D_800FB490[e->ctrlList->data.i];
                    e->wetamt = D_800FB490[128 - e->ctrlList->data.i - 1];
                }
                e->first = 1;
                break;
            case 14:
                {
                    StartParam8_ *p = (StartParam8_ *)e->ctrlList;

                    if (p->unity) {
                        (*e->filter.setParam)(&e->filter, 8, 0);
                    }
                    (*e->filter.setParam)(&e->filter, 5, p->wave);
                    (*e->filter.setParam)(&e->filter, 9, 0);
                }
                break;
            case 15:
                {
                    ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
                    (*e->filter.setParam)(&e->filter, 4, 0);
                }
                break;
            case 0:
                {
                    void *drvr = D_800FB5B0;
                    FreeParam8_ *param = (FreeParam8_ *)e->ctrlList;
                    param->pvoice->offset = 0;
                    _freePVoice((s32)drvr, (s32)param->pvoice);
                }
                break;
            default:
                ptr = _pullSubFrame(e, &inp, &loutp, samples, sampleOffset, ptr);
                (*e->filter.setParam)(&e->filter, e->ctrlList->type, (void *)e->ctrlList->data.i);
                break;
        }
        loutp += (samples << 1);
        outCount -= samples;
        thisParam = e->ctrlList;
        e->ctrlList = e->ctrlList->next;
        if (e->ctrlList == 0)
            e->ctrlTail = 0;
        __freeParam(thisParam);
    }
    ptr = _pullSubFrame(e, &inp, &loutp, outCount, sampleOffset, ptr);
    if (e->delta > e->segEnd)
        e->delta = e->segEnd;
    return ptr;
}

s32 alEnvmixerParam(void *filter, s32 paramID, void *param) {
    ALFilt3_ *f = (ALFilt3_ *)filter;
    ALEnvMix3_ *e = (ALEnvMix3_ *)filter;

    switch (paramID) {
        case 3:
            if (e->ctrlTail) {
                e->ctrlTail->next = (ALParam3_ *)param;
            } else {
                e->ctrlList = (ALParam3_ *)param;
            }
            e->ctrlTail = (ALParam3_ *)param;
            break;
        case 4:
            e->first = 1;
            e->motion = 0;
            e->volume = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 4, param);
            break;
        case 9:
            e->motion = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 9, param);
            break;
        case 1:
            f->source = (ALFilt3_ *)param;
            break;
        default:
            if (f->source)
                (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}


extern s16 D_800FB490[];
s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel);
u32 osVirtualToPhysical(void *);
Acmd8_ *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd8_ *p) {
    Acmd8_ *ptr = p;
    EnvMix8_ *e = (EnvMix8_ *)filter;
    ALFilt8_ *source = e->filter.source;

    if (e->motion != 1 || !outCount)
        return ptr;
    ptr = (*source->handler)(source, inp, outCount, sampleOffset, p);
    { Acmd8_ *_a = ptr++; _a->w0 = (8 << 24) | (0 << 16) | (*inp & 0xFFFF); _a->w1 = (((1088 + *outp) & 0xFFFF) << 16) | ((outCount << 1) & 0xFFFF); }
    { Acmd8_ *_a = ptr++; _a->w0 = (8 << 24) | (8 << 16) | ((1408 + *outp) & 0xFFFF); _a->w1 = (((1728 + *outp) & 0xFFFF) << 16) | ((2048 + *outp) & 0xFFFF); }
    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * D_800FB490[e->pan]) >> 15;
        e->lratm = _getRate((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &(e->lratl));
        e->rtgt = (e->volume * D_800FB490[128 - e->pan - 1]) >> 15;
        e->rratm = _getRate((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &(e->rratl));
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((2 | 4) << 16) | (e->cvolL & 0xFFFF); _a->w1 = 0; }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((0 | 4) << 16) | (e->cvolR & 0xFFFF); _a->w1 = 0; }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((2 | 0) << 16) | (e->ltgt & 0xFFFF); _a->w1 = ((e->lratm & 0xFFFF) << 16) | (e->lratl & 0xFFFF); }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | ((0 | 0) << 16) | (e->rtgt & 0xFFFF); _a->w1 = ((e->rratm & 0xFFFF) << 16) | (e->rratl & 0xFFFF); }
        { Acmd8_ *_a = ptr++; _a->w0 = (9 << 24) | (8 << 16) | (e->dryamt & 0xFFFF); _a->w1 = (0 << 16) | (e->wetamt & 0xFFFF); }
        { Acmd8_ *_a = ptr++; _a->w0 = (3 << 24) | (((1 | 8) & 0xFF) << 16); _a->w1 = osVirtualToPhysical(e->state); }
    } else
        { Acmd8_ *_a = ptr++; _a->w0 = (3 << 24) | (((0 | 8) & 0xFF) << 16); _a->w1 = osVirtualToPhysical(e->state); }
    *inp += (outCount << 1);
    e->delta += outCount;
    return ptr;
}

f64 _frexpf(f64 value, s32 *eptr) {
    f64 absvalue;

    *eptr = 0;
    if (value == 0.0)
        return (value);
    absvalue = (value > 0.0) ? value : -value;
    for (; absvalue >= 1.0; absvalue *= 0.5)
        ++*eptr;
    for (; absvalue < 0.5; absvalue += absvalue)
        --*eptr;
    return (value > 0.0 ? absvalue : -absvalue);
}

f64 _ldexpf(f64 arg0, s32 arg2) {
    f64 var_fa0;

    var_fa0 = arg0;
    if (arg2 != 0) {
        var_fa0 *= (f64) (1 << arg2);
    }
    return var_fa0;
}

s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel) {
    s16 s;
    f64 invn = 1.0 / count, eps, a, fs, mant;
    s32 i_invn, ex, indx;

    if (count == 0) {
        if (tgt >= vol) {
            *ratel = 0xffff;
            return 0x7fff;
        } else {
            *ratel = 0;
            return 0;
        }
    }
    if (tgt < 1.0)
        tgt = 1.0;
    if (vol <= 0) vol = 1;
    {
        f64 logtab[] = { -0.912537, -0.752072, -0.607683, -0.476438,
                         -0.356144, -0.245112, -0.142019, -0.045804 };

        i_invn = (s32)_ldexpf(invn, 30);
        mant = _frexpf(tgt / vol, &ex);
        indx = (s32)(_ldexpf(mant, 3 + 1));
        eps = (logtab[indx - 8] + ex) * 0.69314718055994530942;
        eps /= _ldexpf(1, 30);
        fs = (1.0 + eps);
        a = 1.0;
        while (i_invn) {
            if (i_invn & 1)
                a = a * fs;
            fs *= fs;
            i_invn >>= 1;
        }
    }
    a *= (a *= (a *= a));
    s = (s16)a;
    *ratel = (s16)(0xffff * (a - (f32)s));
    return (s16)a;
}

f32 _getVol(f32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 var_ft0;
    f32 var_fv1;
    s32 var_a1;
    s32 var_v1;

    var_a1 = arg1 >> 3;
    if (var_a1 == 0) {
        return arg0;
    }
    var_ft0 = 1.0f;
    var_v1 = 0;
    var_fv1 = ((f32) (arg2 << 0x10) + (f32) (arg3 & 0xFFFF)) / 65536.0f;
loop_3:
    if (var_a1 & 1) {
        var_ft0 *= var_fv1;
    }
    var_a1 = var_a1 >> 1;
    if (var_a1 != 0) {
        var_v1 += 1;
        var_fv1 *= var_fv1;
        if (var_v1 >= 0x20) {

        } else {
            goto loop_3;
        }
    }
    return arg0 * var_ft0;
}

void alFilterNew(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)((u8 *)arg0 + 0x0) = 0;
    *(s32 *)((u8 *)arg0 + 0x4) = arg1;
    *(s32 *)((u8 *)arg0 + 0x8) = arg2;
    *(s16 *)((u8 *)arg0 + 0xC) = 0;
    *(s16 *)((u8 *)arg0 + 0xE) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = arg3;
}
