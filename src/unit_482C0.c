#include "common.h"
#include "include_asm.h"

extern s32 D_800F3900;
extern s32 D_800F3904;

void func_800A76C0(s32 arg0) {
    D_800F3900 += arg0;
}

s32 func_800A76D8(void) {
    return D_800F3900;
}

s32 func_800A76E4(void) {
    return D_800F3904;
}

void func_800A76F0(void) {
    D_800F3900 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A76FC);

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A780C);

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A7B88);

s32 func_800A7C2C(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A7C34);

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A7C8C);
