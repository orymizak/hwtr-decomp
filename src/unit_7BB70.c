#include "common.h"
#include "include_asm.h"
typedef struct { u16 fc; } LP_;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; s16 pad; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; LP_ *lp; s32 pad24; } ALDelay_;
typedef struct { u8 pad0[0x20]; ALDelay_ *delay; } ALFx_;
extern void func_800D8A30(LP_ *);
extern u8 *D_800FB5B0;



INCLUDE_ASM("asm/nonmatchings/unit_7BB70", func_800DAF70);

s32 func_800DB2E4(s32 *arg0, s32 arg1, s32 arg2) {
    if (arg1 == 1) {
        *arg0 = arg2;
    }
    return 0;
}

s32 func_800DB2F8(void *filter, s32 paramID, void *param) {
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
            func_800D8A30(f->delay[s].lp);
        }
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_7BB70", func_800DB4E8);

INCLUDE_ASM("asm/nonmatchings/unit_7BB70", func_800DB700);

INCLUDE_ASM("asm/nonmatchings/unit_7BB70", func_800DB870);

INCLUDE_ASM("asm/nonmatchings/unit_7BB70", func_800DB9E0);

INCLUDE_ASM("asm/nonmatchings/unit_7BB70", func_800DBA7C);
