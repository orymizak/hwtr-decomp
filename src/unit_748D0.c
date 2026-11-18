#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

Acmd *func_800D35B0(N_PVoice *f, s16 *outp, s32 byteCount, Acmd *p);
s32 func_800D39F8();

/* Resampler pull: decodes enough input for 184 output samples and resamples it (n_alResamplePull). */
Acmd *func_800D3CD0(N_PVoice *f, s16 *outp, Acmd *p) {
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    s32 incr;
    f32 finCount;

    inp = 0x170;
    if (f->rs_upitch) {
        ptr = func_800D35B0(f, &inp, 184, p);
        aDMEMMove(ptr++, inp, *outp, 184 << 1);
    } else {
        if (f->rs_ratio > 1.99996) {
            f->rs_ratio = 1.99996;
        }
        f->rs_ratio = (s32)(f->rs_ratio * 32768.0f);
        f->rs_ratio = f->rs_ratio / 32768.0f;
        finCount = f->rs_delta + (f->rs_ratio * 184.0f);
        inCount = (s32)finCount;
        f->rs_delta = finCount - (f32)inCount;
        ptr = func_800D35B0(f, &inp, inCount, p);
        incr = (s32)(f->rs_ratio * 32768.0f);
        n_aResample(ptr++, osVirtualToPhysical(f->rs_state), f->rs_first, incr, inp, 0);
        f->rs_first = 0;
    }
    return ptr;
}

s32 func_800D3E8C(void) {
    func_800D39F8();
    return 0;
}
