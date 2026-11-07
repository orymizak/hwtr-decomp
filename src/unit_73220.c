#include "common.h"
#include "include_asm.h"
s32 alHeapDBAlloc(s32, s32, void *, s32, s32);



INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D2620);

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D26C0);

void func_800D2AAC(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((u8 *)(arg0) + 0xC)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x10)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x28)) = ((s32 (*)(s32)) arg1)(arg0 + 0x2C);
    (*(s32 *)((u8 *)(arg0) + 0x34)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x38)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x3C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x40)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x20);
    (*(s32 *)((u8 *)(arg0) + 0x4C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x50)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x48)) = 0;
    (*(f32 *)((u8 *)(arg0) + 0x44)) = 1.0f;
    (*(s32 *)((u8 *)(arg0) + 0x54)) = alHeapDBAlloc(0, 0, (void *) arg2, 1, 0x50);
    (*(s32 *)((u8 *)(arg0) + 0x78)) = 1;
    (*(s32 *)((u8 *)(arg0) + 0x84)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x5A)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x68)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x6E)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x5C)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x5E)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x60)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x62)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x66)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x64)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x66)) = 1;
    (*(s16 *)((u8 *)(arg0) + 0x64)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x70)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x74)) = 0;
    (*(s16 *)((u8 *)(arg0) + 0x58)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x7C)) = 0;
    (*(s32 *)((u8 *)(arg0) + 0x80)) = 0;
}

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
