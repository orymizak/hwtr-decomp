#include "common.h"
#include "include_asm.h"
extern u8 D_800F0403;
extern u8 D_800F0390;
s32 func_8006C0EC(s32,s32);
extern u8 D_800F03EB;
extern s32 D_80145568;
extern s32 D_8014556C;
extern s32 D_80145570;
extern s32 D_80145574;
extern s32 D_80145578;
extern s32 D_8014557C;
extern s32 D_80145580;
extern s32 D_80145584;
extern s32 D_80145588;
extern s32 D_8014558C;
extern s32 D_80145590;
extern s32 D_80145594;
extern s32 D_801455A4;
extern s32 D_801455B0;

s32 Memory_Free(s32);
extern s32 D_80102208;
extern s32 D_800F0414;
extern u8 D_800F03F4;
s32 func_800D7130();

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006A540);

s32 func_8006AA34(void) {
    s32 *var_s0;
    s32 var_s1;

    var_s1 = 0;
    var_s0 = &D_80102208;
    do {
        var_s1 += 1;
        Memory_Free(*var_s0);
        *var_s0 = 0;
        var_s0 += 1;
    } while (var_s1 < 3);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006AA84);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B1AC);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B2A0);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B4F8);

s32 func_8006B630(void) {
    D_80145568 = 0;
    D_8014556C = 0;
    D_80145570 = 0;
    D_801455A4 = 0;
    D_80145574 = 0;
    D_80145578 = 0;
    D_8014557C = 0;
    D_80145580 = 0;
    D_80145584 = 0;
    D_80145588 = 0;
    D_801455B0 = 0;
    D_8014558C = 0;
    D_80145590 = 0;
    D_80145594 = 0;
    D_80145584 = 0;
    D_80145588 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B6B8);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B838);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B894);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BA98);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BD78);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BDA8);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BE34);

u8 func_8006C0A0(void) {
    return D_800F03EB;
}

s32 func_8006C0AC(s32 arg0) {
    return *(s32 *)(&D_800F0390 + ((arg0 & 0xFF) * 4));
}

u8 func_8006C0C4(s32 arg0) {
    return *(u8 *)(&D_800F0403 + ((arg0 & 0xFF) * 0x54));
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C0EC);

s32 func_8006C218(s32 arg0) {
    func_8006C0EC(arg0 & 0xFF, 0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C23C);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C274);

s32 func_8006C334(void) {
    return 0;
}

s32 func_8006C33C(void) {
    return D_800F03F4 & 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C34C);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C3D4);

s32 func_8006C468(void) {
    func_800D7130();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C488);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C4BC);

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C520);
