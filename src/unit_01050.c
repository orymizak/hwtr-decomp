#include "common.h"
#include "include_asm.h"

s32 func_8007F514();
s32 func_8008C08C();
s32 func_8006495C();
s32 func_8006C468();
s32 func_800CA1C0();
s32 func_8006C488();
s32 func_800CA1F8(s32);
s32 func_800CA230();
s32 Font_DrawChar(s32, s32, s32, s32, s32);
s32 func_800CD600(s32 *);
extern s32 D_800F015C;
extern u8 D_800F0105;
extern f32 D_800EFC70;
extern f64 D_800FE898;
extern f32 D_800EFC74;
extern f64 D_800FE8A0;

s32 func_80060450(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060458);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060840);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060990);

s32 func_80060998(void) {
    return 1;
}

s32 func_800609A0(void) {
    return 1;
}

s32 func_800609A8(void) {
    func_8007F514();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800609C8);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060AFC);

s32 func_80060B6C(void) {
    return func_8008C08C() & 0xFF;
}

void func_80060B8C(void) {
    func_8006C468();
    func_8006495C();
    func_800CA1C0();
}

void func_80060BB8(s32 arg0) {
    func_8006C488();
    func_800CA1F8(arg0);
}

void func_80060BE4(void) {
    func_800CA230();
}

s32 func_80060C00(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return Font_DrawChar(arg0 & 0xFF, arg1 & 0xFF, arg2 & 0xFFFF, arg3 & 0xFFFF, 0) & 0xFFFF;
}

void func_80060C30(void) {
    func_800CD600(&D_800F015C);
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060C50);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800610DC);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061334);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061454);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800616A0);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006185C);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061A84);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061B40);

u8 func_80061B88(void) {
    return D_800F0105;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061B94);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061C70);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80062950);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063434);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063468);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800634C0);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800634F4);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063550);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800635AC);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006365C);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063938);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800639EC);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063A18);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063AC0);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063C18);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063C88);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063CB4);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063E0C);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063E7C);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063EA8);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063FE8);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064058);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064194);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064204);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064230);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064308);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064364);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064418);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800645BC);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064638);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064684);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006470C);

void func_8006476C(s32 arg0) {
    D_800EFC70 = (f32) ((f64) (arg0 & 0xFF) / D_800FE898);
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064798);

void func_800647F8(s32 arg0) {
    D_800EFC74 = (f32) ((f64) (arg0 & 0xFF) / D_800FE8A0);
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064824);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064884);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006495C);

s32 func_80064980(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064988);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800649F8);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064B58);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064D3C);

f32 func_80064E68(void *arg0, void *arg1) {
    f32 temp_ft0;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_fv1 = *(f32 *)((u8 *)arg0 + 0x0) - *(f32 *)((u8 *)arg1 + 0x0);
    temp_ft0 = *(f32 *)((u8 *)arg0 + 0x4) - *(f32 *)((u8 *)arg1 + 0x4);
    temp_fv0 = *(f32 *)((u8 *)arg0 + 0x8) - *(f32 *)((u8 *)arg1 + 0x8);
    return (temp_fv1 * temp_fv1) + (temp_ft0 * temp_ft0) + (temp_fv0 * temp_fv0);
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064EA4);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064EE8);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064F5C);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064F84);
