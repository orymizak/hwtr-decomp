#include "common.h"
#include "include_asm.h"
u32 func_800DEE5C(s32, void *, void *, u32);

s32 osSetIntMask(s32);
s32 osViGetCurrentFramebuffer();
s32 osViGetNextFramebuffer();
s32 osSpTaskYield(void *);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DE660);

void func_800DE7A8(void *arg0, void *arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    *(s32 *)((u8 *)arg1 + 0x4) = arg2;
    *(s32 *)((u8 *)arg1 + 0x0) = (void *) *(s32 *)((u8 *)arg0 + 0x260);
    *(s32 *)((u8 *)arg0 + 0x260) = arg1;
    osSetIntMask(temp_v0);
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DE800);

s32 func_800DE890(s32 arg0) {
    return arg0 + 0x78;
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DE898);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DE99C);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DEA9C);

void func_800DEB8C(s32 arg0) {
    s32 sp10;
    s32 sp14;
    u32 var_s0;
    void *temp_a1;

    temp_a1 = (*(s32 *)((u8 *)(arg0) + 0x278));
    sp10 = 0;
    sp14 = 0;
    (*(s32 *)((u8 *)(arg0) + 0x278)) = NULL;
    (*(s32 *)((u8 *)(temp_a1) + 0x4)) = (s32) ((*(s32 *)((u8 *)(temp_a1) + 0x4)) & ~1);
    func_800DEC70(arg0, (s32) temp_a1);
    var_s0 = ((*(s32 *)((u8 *)(arg0) + 0x274)) == 0) * 2;
    if ((*(s32 *)((u8 *)(arg0) + 0x278)) == NULL) {
        var_s0 |= 1;
    }
    if (func_800DEE5C(arg0, &sp10, &sp14, var_s0) != var_s0) {
        func_800DED54(arg0, sp10, sp14);
    }
}

s32 func_800DEC1C(s32 arg0) {
    s32 temp_s0;

    if (arg0 != 0) {
        temp_s0 = osViGetCurrentFramebuffer();
        return arg0 & -(temp_s0 == osViGetNextFramebuffer());
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DEC70);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DECFC);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DED54);

void func_800DEE24(void *arg0) {
    void *temp_a0;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0x274);
    if (*(s32 *)((u8 *)temp_a0 + 0x10) == 1) {
        *(s32 *)((u8 *)temp_a0 + 0x4) = (s32) (*(s32 *)((u8 *)temp_a0 + 0x4) | 0x10);
        osSpTaskYield(temp_a0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_7F260", func_800DEE5C);

INCLUDE_ASM("asm/nonmatchings/unit_7F260", __osSiRawStartDma);
