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

INCLUDE_ASM("asm/nonmatchings/unit_74AB0", n_alFxPull);

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

INCLUDE_ASM("asm/nonmatchings/unit_74AB0", _n_loadOutputBuffer);

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
