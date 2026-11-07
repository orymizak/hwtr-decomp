#include "common.h"
#include "include_asm.h"

extern s32 *D_800FB174;

void func_800D4B50(void) {
    *D_800FB174 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_75750", func_800D4B60);
