#include "common.h"
#include "include_asm.h"

extern s32 D_8011F6A0;

INCLUDE_ASM("asm/nonmatchings/unit_4FB80", func_800AEF80);

INCLUDE_ASM("asm/nonmatchings/unit_4FB80", func_800AEFBC);

INCLUDE_ASM("asm/nonmatchings/unit_4FB80", func_800AEFF8);

s32 func_800AF174(void) {
    return D_8011F6A0;
}

INCLUDE_ASM("asm/nonmatchings/unit_4FB80", func_800AF180);
