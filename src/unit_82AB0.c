#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E1EB0);

s32 func_800E1EE4(void *arg0, void *arg1, void *arg2) {
    u16 temp_v1;
    void *temp_a3;
    void *var_a0;

    var_a0 = arg0;
    temp_a3 = var_a0 + 0x1C;
    (*(s16 *)arg2) = 0U;
    (*(s16 *)arg1) = 0U;
    do {
        temp_v1 = (*(s16 *)var_a0);
        (*(s16 *)arg1) = (u16) ((*(s16 *)arg1) + temp_v1);
        var_a0 += 2;
        (*(s16 *)arg2) = (u16) ((*(s16 *)arg2) + ~temp_v1);
    } while ((u32) var_a0 < (u32) temp_a3);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E1F28);

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E2214);

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E2378);

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E2544);

INCLUDE_ASM("asm/nonmatchings/unit_82AB0", func_800E25FC);
