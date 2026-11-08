#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_4D480", func_800AC880);

INCLUDE_ASM("asm/nonmatchings/unit_4D480", func_800ACDC8);

INCLUDE_ASM("asm/nonmatchings/unit_4D480", func_800AD774);

INCLUDE_ASM("asm/nonmatchings/unit_4D480", func_800ADE1C);

INCLUDE_ASM("asm/nonmatchings/unit_4D480", func_800AE8F4);

void func_800AE9C4(u8 *t, u8 i) {
    f32 v[3];
    f32 d;
    f32 *a = (f32 *)(i * 4 + (u32)t);

    v[0] = *(f32 *)((u8 *)a + 0x94);
    v[1] = *(f32 *)((u8 *)a + 0xA0);
    v[2] = *(f32 *)((u8 *)a + 0xAC);
    d = v[0] * *(f32 *)(t + 0xB8) + v[1] * *(f32 *)(t + 0xBC) + v[2] * *(f32 *)(t + 0xC0);
    *(f32 *)(t + 0xB8) = v[0] * d;
    *(f32 *)(t + 0xBC) = v[1] * d;
    *(f32 *)(t + 0xC0) = v[2] * d;
}

