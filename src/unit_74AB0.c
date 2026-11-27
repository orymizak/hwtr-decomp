#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

#define INPUT_PARAM         0
#define OUTPUT_PARAM        1
#define FBCOEF_PARAM        2
#define FFCOEF_PARAM        3
#define GAIN_PARAM          4
#define CHORUSRATE_PARAM    5
#define CHORUSDEPTH_PARAM   6
#define LPFILT_PARAM        7

#define RANGE   2.0
#define CONVERT 173123.404906676

/* TODO(match-debt): 195/196 words (784/784 B exact, s54). Residual is 1 instruction position in the final aDMEMMove:
 * `addiu $v0, $t0, 0x8` lands at 0x2D4 instead of 0x2C8 (before `addu $a0, $t0, $zero`). */
#ifdef NON_MATCHING
Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p);
Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p);
Acmd *_n_filterBuffer(ALLowPass *lp, s32 flags, Acmd *ptr);

Acmd *n_alFxPull(s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALFx *r = n_syn->auxBus->fx;
    s16 i;
    s16 input;
    s16 output;
    s16 *in_ptr;
    s16 *out_ptr;
    s16 *prev_outptr = NULL;
    ALDelay *d;

    ptr = n_alAuxBusPull(sampleOffset, p);

    aMix(ptr++, 0, 0xDA83, 0x7C0, 0x7C0);
    aMix(ptr++, 0, 0x5A82, 0x930, 0x7C0);
    ptr = _n_saveBuffer(r, r->input, 0x7C0, ptr);
    aClearBuffer(ptr++, 0x930, 184 << 1);

    input = 0;
    output = 184 << 1;

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];

        if (in_ptr == prev_outptr) {
            s16 t = output;
            output = input;
            input = t;
        } else {
            ptr = _n_loadBuffer(r, in_ptr, input, 184, ptr);
        }
        ptr = _n_loadOutputBuffer(r, d, output, ptr);

        if (d->ffcoef) {
            aMix(ptr++, 0, (u16)d->ffcoef, input, output);
            if (!d->rs && !d->lp) {
                ptr = _n_saveBuffer(r, out_ptr, output, ptr);
            }
        }

        if (d->fbcoef) {
            aMix(ptr++, 0, (u16)d->fbcoef, output, input);
            ptr = _n_saveBuffer(r, in_ptr, input, ptr);
        }

        if (d->lp) {
            ptr = _n_filterBuffer(d->lp, output, ptr);
        }

        if (!d->rs) {
            ptr = _n_saveBuffer(r, out_ptr, output, ptr);
        }

        if (d->gain) {
            aMix(ptr++, 0, (u16)d->gain, output, 0x930);
        }

        prev_outptr = &r->input[d->output];
    }

    r->input += 184;
    if (r->input > &r->base[r->length]) {
        r->input -= r->length;
    }

    {
        u32 w1 = _SHIFTL(0x7C0, 16, 16) | _SHIFTL(184 << 1, 0, 16);
        Acmd *_a = (Acmd *)(ptr++);
        do { } while (0);
        _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(0x930, 0, 24);
        _a->words.w1 = w1;
    }

    return ptr;
}
#else
INCLUDE_ASM("asm/nonmatchings/unit_74AB0", n_alFxPull);
#endif

/* Effect parameter handler: paramID 2 + 8 * section + parameter. */
s32 n_alFxParamHdl(void *filter, s32 paramID, void *param) {
    ALFx *f = filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;

    switch (p) {
    case INPUT_PARAM:
        f->delay[s].input = (u32)val & 0xFFFFFFF8;
        break;
    case OUTPUT_PARAM:
        f->delay[s].output = (u32)val & 0xFFFFFFF8;
        break;
    case FFCOEF_PARAM:
        f->delay[s].ffcoef = (s16)val;
        break;
    case FBCOEF_PARAM:
        f->delay[s].fbcoef = (s16)val;
        break;
    case GAIN_PARAM:
        f->delay[s].gain = (s16)val;
        break;
    case CHORUSRATE_PARAM:
        f->delay[s].rsinc = ((((f32)val) / 1000) * RANGE) / n_syn->outputRate;
        break;
    case CHORUSDEPTH_PARAM:
        f->delay[s].rsgain = (((f32)val) / CONVERT) * (f->delay[s].output - f->delay[s].input);
        break;
    case LPFILT_PARAM:
        if (f->delay[s].lp) {
            f->delay[s].lp->fc = (s16)val;
            _init_lpfilter(f->delay[s].lp);
        }
        break;
    }
    return 0;
}

f32 _doModFunc(ALDelay *d, s32 count);
Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);

/* Loads the delayed output of section d into buff (flags >> 8 select the DMEM buffer); with a chorus
 * resampler, pulls a modulated number of samples and resamples them to 184. */
Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    s32 ratio;
    s32 count;
    s32 rbuff = 0x2E0;
    s16 *out_ptr;
    f32 fincount;
    f32 fratio;
    f32 delta;
    s32 ramalign = 0;
    s32 length;
    s32 incount = 184;

    if (d->rs) {
        length = d->output - d->input;
        delta = _doModFunc(d, 184);
        delta /= length;
        delta = (s32)(delta * UNITY_PITCH);
        delta = delta / UNITY_PITCH;
        fratio = 1.0 - delta;
        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;
        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 0x7) >> 1;
        ptr = _n_loadBuffer(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
        ratio = (s32)(fratio * UNITY_PITCH);
        {
            s32 t = buff >> 8;
            n_aResample(ptr++, osVirtualToPhysical(d->rs->state), d->rs->first, ratio, rbuff + (ramalign << 1), t);
        }
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = _n_loadBuffer(r, out_ptr, buff, incount, ptr);
    }
    return ptr;
}

/* Loads count samples of the circular delay line at curr_ptr into DMEM buff. */
Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p) {
    Acmd *ptr = p;
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
        n_aLoadBuffer(ptr++, before_end << 1, buff, osVirtualToPhysical(curr_ptr));
        n_aLoadBuffer(ptr++, after_end << 1, buff + (before_end << 1), osVirtualToPhysical(r->base));
    } else {
        n_aLoadBuffer(ptr++, count << 1, buff, osVirtualToPhysical(curr_ptr));
    }
    return ptr;
}

/* Saves 184 samples from DMEM buff into the circular delay line at curr_ptr. */
Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base) {
        curr_ptr += r->length;
    }
    updated_ptr = curr_ptr + 184;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        n_aSaveBuffer(ptr++, before_end << 1, buff, osVirtualToPhysical(curr_ptr));
        n_aSaveBuffer(ptr++, after_end << 1, buff + (before_end << 1), osVirtualToPhysical(r->base));
    } else {
        n_aSaveBuffer(ptr++, 184 << 1, buff, osVirtualToPhysical(curr_ptr));
    }
    return ptr;
}

/* Low-pass filters DMEM buff (buff is passed as flags >> 8).
 * TODO(match-debt): the pole-filter command is written in two halves around r = ptr + 2;
 * aLoadADPCM(ptr++) + n_aPoleFilter(ptr++) schedules differently. */
Acmd *_n_filterBuffer(ALLowPass *lp, s32 flags, Acmd *ptr) {
    s32 t;
    Acmd *r;

    t = flags >> 8;
    aLoadADPCM(&ptr[0], 32, osVirtualToPhysical(lp->fcvec.fccoef));
    ptr[1].words.w0 = _SHIFTL(A_POLEF, 24, 8) | ((u8)lp->first << 16) | (u16)lp->fgain;
    r = ptr + 2;
    ptr[1].words.w1 = (t << 24) | (osVirtualToPhysical(lp->fstate) & 0xFFFFFF);
    lp->first = 0;
    return r;
}
