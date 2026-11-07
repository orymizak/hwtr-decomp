#include "common.h"
#include "include_asm.h"

extern s32 D_801279D0;

void func_800D1F40(s32 arg0) {
    D_801279D0 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/unit_72B40", func_800D1F4C);

INCLUDE_ASM("asm/nonmatchings/unit_72B40", func_800D1FB0);

INCLUDE_ASM("asm/nonmatchings/unit_72B40", func_800D2010);
