#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } AcmdR_;
typedef struct { u8 p0[0x40]; void *state; f32 ratio; s32 upitch; f32 delta; s32 first; } ResR_;
AcmdR_ *func_800D35B0(void *, s16 *, s32, AcmdR_ *);
u32 osVirtualToPhysical(void *);

s32 func_800D39F8();

AcmdR_ *func_800D3CD0(void *filter, s16 *outp, AcmdR_ *p) {
    ResR_ *f = (ResR_ *)filter;
    AcmdR_ *ptr = p;
    s16 inp;
    s32 inCount;
    s32 incr;
    f32 finCount;

    inp = 0x170;
    if (f->upitch) {
        ptr = func_800D35B0(f, &inp, 184, p);
        {
            AcmdR_ *_a = ptr++;
            _a->w0 = (10 << 24) | (inp & 0xFFFFFF);
            _a->w1 = ((*outp & 0xFFFF) << 16) | (0x170 & 0xFFFF);
        }
    } else {
        if (f->ratio > 1.99996) {
            f->ratio = 1.99996;
        }
        f->ratio = (s32)(f->ratio * 32768.0f);
        f->ratio = f->ratio / 32768.0f;
        finCount = f->delta + (f->ratio * 184.0f);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;
        ptr = func_800D35B0(f, &inp, inCount, p);
        incr = (s32)(f->ratio * 32768.0f);
        {
            AcmdR_ *_a = ptr++;
            _a->w0 = (5 << 24) | (osVirtualToPhysical(f->state) & 0xFFFFFF);
            _a->w1 = (f->first << 30) | ((incr & 0xFFFF) << 14) | ((inp & 0xFFF) << 2);
        }
        f->first = 0;
    }
    return ptr;
}

s32 func_800D3E8C(void) {
    func_800D39F8();
    return 0;
}
