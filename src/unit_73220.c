#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D2620);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D26C0);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D2AAC);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D2BE0);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D3158);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D31FC);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D341C);

s32 func_800D355C(s32 arg0, s32 arg1, s16 arg2, s32 arg3) {
    s32 temp_a1;
    s32 var_v0;

    temp_a1 = arg1 >> 3;
    if (temp_a1 != 0) {
        var_v0 = (arg0 + (((s32) ((arg3 & 0xFFFF) * temp_a1) >> 0x10) + (arg2 * temp_a1))) << 0x10;
    } else {
        var_v0 = arg0 << 0x10;
    }
    return var_v0 >> 0x10;
}
