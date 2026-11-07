#include "common.h"
#include "include_asm.h"
s32 Memory_Free(void *);



INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A7F00);

void func_800A7F5C(void *arg0) {
    void *temp_a0;

    temp_a0 = (*(s32 *)((u8 *)(arg0) + 0x8));
    if (temp_a0 != NULL) {
        Memory_Free(temp_a0);
    }
    Memory_Free(arg0);
}

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A7F98);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A8050);

s32 func_800A81B4(void *arg0, s32 arg1) {
    return *(s32 *)((u8 *)arg0 + 0x8) + (*(u16 *)((u8 *)arg0 + 0x0) * (arg1 & 0xFFFF));
}

u16 func_800A81D4(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 0x4);
}

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A81DC);

void func_800A8218(s32 *arg0) {
    s32 *var_a0;
    s32 *var_s0;

    var_s0 = *arg0;
    var_a0 = var_s0;
    if (var_s0 != NULL) {
        do {
            var_s0 = (*(s32 *)((u8 *)(var_s0) + 0x8));
            ((s32 (*)(s32 *))Memory_Free)(var_a0);
            var_a0 = var_s0;
        } while (var_s0 != NULL);
    }
    ((s32 (*)(s32 *))Memory_Free)(arg0);
}

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A8268);

INCLUDE_ASM("asm/nonmatchings/unit_48B00", func_800A82F8);
