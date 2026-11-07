#include "common.h"
#include "include_asm.h"

extern s32 D_80125580;
extern s8 D_80124D70;
extern s8 D_80124D71;
extern s32 (*D_801416A8)();
extern s32 (*D_801416B0)();

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA290);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA478);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA660);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA7A8);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA8F0);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CA9B8);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAB30);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CACA8);

void func_800CAE10(void) {
    D_80125580 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAE1C);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CAF28);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB034);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB0DC);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB168);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB268);

void func_800CB280(void) {
    D_80124D70 = 0;
}

void func_800CB28C(void) {
    D_801416B0();
    D_801416A8();
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB2C0);

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", func_800CB4D4);
