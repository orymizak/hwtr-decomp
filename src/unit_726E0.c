#include "common.h"
#include "include_asm.h"

extern s32 func_800D1D30;

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1AE0);

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1C54);

s32 *func_800D1D24(void) {
    return &func_800D1D30;
}

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1D30);

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1D9C);
