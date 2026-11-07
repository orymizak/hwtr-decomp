#include "common.h"
#include "include_asm.h"

extern s32 D_8014D304;
extern s32 D_8014D33C;
s32 osSetIntMask(s32);
extern s8 D_8014D97D;
extern u8 D_8014D97C;

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5560);

s32 *func_800D57AC(void) {
    return &D_8014D304;
}

s32 *func_800D57B8(void) {
    return &D_8014D33C;
}

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D57C4);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5910);

void func_800D59A0(void *arg0, s16 arg1) {
    s32 temp_a0;

    temp_a0 = osSetIntMask(1);
    *(s16 *)((u8 *)arg0 + 0x8) = arg1;
    osSetIntMask(temp_a0);
}

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D59E0);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5A5C);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5ABC);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5C18);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5D5C);

void func_800D5DE8(s8 arg0) {
    D_8014D97D = arg0;
}

u8 func_800D5DF4(void) {
    return D_8014D97C;
}

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5E00);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5ED0);
