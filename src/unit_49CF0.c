#include "common.h"
#include "include_asm.h"
s32 (*D_801416D4)(s32);
s32 func_800A76D8(s32);
extern u8 D_800F3938;
extern u8 D_800F393C;
extern u8 D_800F3A1E;
extern u8 D_800F3A1F;
extern u8 D_8011F68F;
extern u8 D_800F3B10;
extern u8 D_800F3B40;

extern s8 D_800F3B45;

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A90F0);

s32 func_800A9370(void) {
    s32 (**fp)(s32) = &D_801416D4;
    s32 a = (*fp)(0) & 1;

    return a & (*fp)(1);
}


INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A93B4);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A9604);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A978C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A99D0);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A9C00);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800A9EDC);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA054);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA214);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AA80C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AAA20);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AABE4);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AAD9C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AB38C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AB700);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AB8AC);

void func_800ABC80(s32 arg0, s32 arg1, s8 arg2) {
    s32 temp_a0;

    temp_a0 = arg0 & 0xFF;
    *(s32 *)(&D_800F3B10 + (temp_a0 * 4)) = arg1;
    *(u8 *)(&D_800F3B40 + temp_a0) = arg2;
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800ABCA4);

void func_800ABEA8(s8 arg0, s32 arg1) {
    *(u8 *)(&D_8011F68F + (arg1 & 0xFF)) = arg0;
}

void func_800ABEBC(s8 arg0) {
    D_800F3B45 = arg0;
}

void func_800ABEC8(s32 arg0, s32 arg1, s8 arg2) {
    s32 temp_a0;
    s32 temp_s0;

    temp_a0 = arg0 & 0xFF;
    temp_s0 = temp_a0 * 0xF0;
    *(u8 *)(&D_800F3A1F + temp_s0) = arg2;
    *(u8 *)(&D_800F3A1E + temp_s0) = 1;
    *(s32 *)(&D_800F3938 + temp_s0) = arg1;
    *(s32 *)(&D_800F393C + temp_s0) = func_800A76D8(temp_a0);
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800ABF2C);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AC6EC);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", func_800AC7C8);
