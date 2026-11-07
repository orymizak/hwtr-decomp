#include "common.h"
#include "include_asm.h"

extern void *D_800FB174;
extern f64 D_801017B0;
extern f64 D_801017B8;
extern f64 D_801017C0;
extern f64 D_801017C8;

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D4EE0);

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D5150);

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D52B8);

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D52E4);

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D52FC);

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D535C);

s32 func_800D5394(s32 arg0) {
    return (s32) (f32) (((f64) ((f32) arg0 * (f32) *(s32 *)((u8 *)D_800FB174 + 0x40)) / D_801017B0) + D_801017B8);
}

s32 func_800D53E4(s32 arg0) {
    return (s32) (f32) (((f64) ((f32) arg0 * (f32) *(s32 *)((u8 *)D_800FB174 + 0x40)) / D_801017C0) + D_801017C8) & ~0xF;
}
