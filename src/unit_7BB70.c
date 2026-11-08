#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd6_;
typedef struct { u8 filter[0x14]; void *state; f32 ratio; s32 upitch; f32 delta; s32 first; } Resampler6_;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; void *lp; Resampler6_ *rs; } Delay6_;
typedef struct { u8 filter[0x14]; s16 *base; s16 *input; u32 length; } Fx6_;
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct { u8 p0[0x14]; s16 *base; u8 p18[4]; s32 length; } Fx_;
u32 osVirtualToPhysical(void *);
typedef struct { u16 fc; } LP_;
typedef struct { s16 fc; s16 fgain; s32 pad4; s16 fccoef[16]; void *fstate; s32 first; } LowPass_;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; s16 pad; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; LP_ *lp; s32 pad24; } ALDelay_;
typedef struct { u8 pad0[0x20]; ALDelay_ *delay; } ALFx_;
extern void _init_lpfilter(LP_ *);
extern u8 *D_800FB5B0;



typedef struct FilterP_ { struct FilterP_ *source; Acmd_ *(*handler)(void *, s16 *, s32, s32, Acmd_ *); s32 (*setParam)(void *, s32, void *); u8 pad[8]; } FilterP_;
typedef struct { FilterP_ filter; s16 *base; s16 *input; u32 length; ALDelay_ *delay; u8 section_count; void *paramHdl; } FxP_;
Acmd_ *_saveBuffer(Fx_ *r, s16 *curr_ptr, s32 buff, s32 count, Acmd_ *p);
Acmd_ *_filterBuffer(LowPass_ *lp, s32 buff, s32 count, Acmd_ *p);
Acmd_ *_loadBuffer(Fx_ *r, s16 *curr_ptr, s32 buff, s32 count, Acmd_ *p);
Acmd6_ *_loadOutputBuffer(Fx6_ *r, Delay6_ *d, s32 buff, s32 incount, Acmd6_ *p);
Acmd_ *alFxPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd_ *p) {
    Acmd_ *ptr = p;
    FxP_ *r = (FxP_ *)filter;
    FilterP_ *source = r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
    ALDelay_ *d, *pd;

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);
    input = 1728;
    output = 2048;
    buff1 = 0;
    buff2 = 320;
    { Acmd_ *_a = ptr++; _a->w0 = 0x08000000; _a->w1 = (outCount << 1) & 0xFFFF; }
    { Acmd_ *_a = ptr++; _a->w0 = 0x0C00DA83; _a->w1 = (1728 << 16) | (input & 0xFFFF); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x0C005A82; _a->w1 = (2048 << 16) | (input & 0xFFFF); }
    ptr = _saveBuffer((Fx_ *)r, r->input, input, outCount, ptr);
    { Acmd_ *_a = ptr++; _a->w0 = 0x02000000 | (output & 0xFFFFFF); _a->w1 = (u32)(outCount << 1); }
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];
        if (in_ptr == prev_out_ptr) {
            s16 t = buff2;
            buff2 = buff1;
            buff1 = t;
        } else {
            ptr = _loadBuffer((Fx_ *)r, in_ptr, buff1, outCount, ptr);
        }
        ptr = (Acmd_ *)_loadOutputBuffer((Fx6_ *)r, (Delay6_ *)d, buff2, outCount, (Acmd6_ *)ptr);
        if (d->ffcoef) {
            { Acmd_ *_a = ptr++; _a->w0 = 0x0C000000 | ((u16)d->ffcoef & 0xFFFF); _a->w1 = ((buff1 & 0xFFFF) << 16) | (buff2 & 0xFFFF); }
            if (!d->pad24 && !d->lp) {
                ptr = _saveBuffer((Fx_ *)r, out_ptr, buff2, outCount, ptr);
            }
        }
        if (d->fbcoef) {
            { Acmd_ *_a = ptr++; _a->w0 = 0x0C000000 | ((u16)d->fbcoef & 0xFFFF); _a->w1 = ((buff2 & 0xFFFF) << 16) | (buff1 & 0xFFFF); }
            ptr = _saveBuffer((Fx_ *)r, in_ptr, buff1, outCount, ptr);
        }
        if (d->lp)
            ptr = _filterBuffer((LowPass_ *)d->lp, buff2, outCount, ptr);
        if (!d->pad24)
            ptr = _saveBuffer((Fx_ *)r, out_ptr, buff2, outCount, ptr);
        if (d->gain)
            { Acmd_ *_a = ptr++; _a->w0 = 0x0C000000 | ((u16)d->gain & 0xFFFF); _a->w1 = ((buff2 & 0xFFFF) << 16) | (output & 0xFFFF); }
        prev_out_ptr = &r->input[d->output];
    }
    r->input += outCount;
    if (r->input > &r->base[r->length])
        r->input -= r->length;
    { Acmd_ *_a = ptr++; _a->w0 = 0x0A000000 | (output & 0xFFFFFF); _a->w1 = (1728 << 16) | ((outCount << 1) & 0xFFFF); }
    return ptr;
}

s32 alFxParam(s32 *arg0, s32 arg1, s32 arg2) {
    if (arg1 == 1) {
        *arg0 = arg2;
    }
    return 0;
}

s32 alFxParamHdl(void *filter, s32 paramID, void *param) {
    ALFx_ *f = filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;

    switch (p) {
    case 0:
        f->delay[s].input = (u32)val & 0xFFFFFFF8;
        break;
    case 1:
        f->delay[s].output = (u32)val & 0xFFFFFFF8;
        break;
    case 3:
        f->delay[s].ffcoef = (s16)val;
        break;
    case 2:
        f->delay[s].fbcoef = (s16)val;
        break;
    case 4:
        f->delay[s].gain = (s16)val;
        break;
    case 5:
        f->delay[s].rsinc = ((((f32)val) / 1000) * 2.0) / *(s32 *)(D_800FB5B0 + 0x44);
        break;
    case 6:
        f->delay[s].rsgain = (((f32)val) / 173123.404906676) * (f->delay[s].output - f->delay[s].input);
        break;
    case 7:
        if (f->delay[s].lp) {
            f->delay[s].lp->fc = (s16)val;
            _init_lpfilter(f->delay[s].lp);
        }
        break;
    }
    return 0;
}

f32 _doModFunc(ALDelay_ *d, s32 count);
Acmd_ *_loadBuffer(Fx_ *r, s16 *curr_ptr, s32 buff, s32 count, Acmd_ *p);
Acmd6_ *_loadOutputBuffer(Fx6_ *r, Delay6_ *d, s32 buff, s32 incount, Acmd6_ *p) {
    Acmd6_ *ptr = p;
    s32 ratio, count, rbuff = 640;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;

    if (d->rs) {
        length = d->output - d->input;
        delta = _doModFunc((ALDelay_ *)d, incount);
        delta /= length;
        delta = (s32)(delta * 0x8000);
        delta = delta / 0x8000;
        fratio = 1.0 - delta;
        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;
        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 0x7) >> 1;
        ptr = (Acmd6_ *)_loadBuffer((Fx_ *)r, out_ptr - ramalign, rbuff, count + ramalign, (Acmd_ *)ptr);
        ratio = (s32)(fratio * 0x8000);
        { Acmd6_ *_a = ptr++; _a->w0 = (8 << 24) | (((rbuff + (ramalign << 1))) & 0xFFFF); _a->w1 = ((buff & 0xFFFF) << 16) | ((incount << 1) & 0xFFFF); }
        { Acmd6_ *_a = ptr++; _a->w0 = (5 << 24) | ((d->rs->first & 0xFF) << 16) | (ratio & 0xFFFF); _a->w1 = osVirtualToPhysical(d->rs->state); }
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = (Acmd6_ *)_loadBuffer((Fx_ *)r, out_ptr, buff, incount, (Acmd_ *)ptr);
    }
    return ptr;
}

Acmd_ *_loadBuffer(Fx_ *r, s16 *curr_ptr, s32 buff, s32 count, Acmd_ *p) {
    Acmd_ *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        { Acmd_ *a = ptr++; a->w0 = (8 << 24) | (buff & 0xFFFF); a->w1 = (before_end << 1) & 0xFFFF; }
        { Acmd_ *a = ptr++; a->w0 = 4 << 24; a->w1 = osVirtualToPhysical(curr_ptr); }
        { Acmd_ *a = ptr++; a->w0 = (8 << 24) | ((buff + (before_end << 1)) & 0xFFFF); a->w1 = (after_end << 1) & 0xFFFF; }
        { Acmd_ *a = ptr++; a->w0 = 4 << 24; a->w1 = osVirtualToPhysical(r->base); }
    } else {
        { Acmd_ *a = ptr++; a->w0 = (8 << 24) | (buff & 0xFFFF); a->w1 = (count << 1) & 0xFFFF; }
        { Acmd_ *a = ptr++; a->w0 = 4 << 24; a->w1 = osVirtualToPhysical(curr_ptr); }
    }
    { Acmd_ *a = ptr++; a->w0 = 8 << 24; a->w1 = (count << 1) & 0xFFFF; }
    return ptr;
}

Acmd_ *_saveBuffer(Fx_ *r, s16 *curr_ptr, s32 buff, s32 count, Acmd_ *p) {
    Acmd_ *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        { Acmd_ *a = ptr++; a->w0 = 8 << 24; a->w1 = (buff << 16) | ((before_end << 1) & 0xFFFF); }
        { Acmd_ *a = ptr++; a->w0 = 6 << 24; a->w1 = osVirtualToPhysical(curr_ptr); }
        { Acmd_ *a = ptr++; a->w0 = 8 << 24; a->w1 = ((buff + (before_end << 1)) << 16) | ((after_end << 1) & 0xFFFF); }
        { Acmd_ *a = ptr++; a->w0 = 6 << 24; a->w1 = osVirtualToPhysical(r->base); }
        { Acmd_ *a = ptr++; a->w0 = 8 << 24; a->w1 = (count << 1) & 0xFFFF; }
    } else {
        { Acmd_ *a = ptr++; a->w0 = 8 << 24; a->w1 = (buff << 16) | ((count << 1) & 0xFFFF); }
        { Acmd_ *a = ptr++; a->w0 = 6 << 24; a->w1 = osVirtualToPhysical(curr_ptr); }
    }
    return ptr;
}

Acmd_ *_filterBuffer(LowPass_ *lp, s32 buff, s32 count, Acmd_ *p) {
    Acmd_ *ptr = p;

    { Acmd_ *_a = ptr++; _a->w0 = 0x08000000 | (0 << 16) | (buff & 0xFFFF); _a->w1 = ((buff & 0xFFFF) << 16) | ((count << 1) & 0xFFFF); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x0B000020; _a->w1 = osVirtualToPhysical(lp->fccoef); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x0E000000 | ((lp->first & 0xFF) << 16) | (lp->fgain & 0xFFFF); _a->w1 = osVirtualToPhysical(lp->fstate); }
    lp->first = 0;
    return ptr;
}

f32 _doModFunc(ALDelay_ *d, s32 count) {
    f32 val;

    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > 2.0) ? d->rsval - (2.0 * 2) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= 2.0 / 2;
    return (d->rsgain * val);
}
