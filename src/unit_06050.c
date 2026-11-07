#include "common.h"
#include "include_asm.h"

extern void *D_8013816C;
extern s32 D_8013818C;
s32 GfxPools_Shutdown();
s32 func_800741D8(s32);
extern s32 D_80138174;
s32 func_80066D80(s32);
s32 func_800709CC(s32 *);
extern s32 D_800F2138;

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065450);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065514);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065584);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800655CC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006572C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006586C);

u16 func_800658C4(void) {
    return *(u16 *)((u8 *)D_8013816C + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800658D4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065940);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065C5C);

void func_800664A4(s32 arg0) {
    D_8013818C = arg0;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800664B0);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066504);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665A4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665D0);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665F8);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006673C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066758);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066778);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800667A8);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800667F8);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006681C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006695C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066A2C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066B94);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066BD4);

void func_80066C64(s32 arg0) {
    func_800709CC(&D_800F2138);
    func_80066D80(arg0 & 0xFF);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066C9C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066D80);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066ECC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066EFC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067284);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067310);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800674DC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067950);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067A00);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067EFC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067F40);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067FA4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068020);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006810C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068270);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800682A4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068390);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068504);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800685E0);

u16 func_800687D8(void) {
    return *(u16 *)((u8 *)D_8013816C + 0x6);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800687E8);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800688F4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800689A4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068E98);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80069214);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006936C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80069494);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800698C0);

u16 func_8006A1E0(void) {
    return *(u16 *)((u8 *)D_8013816C + 0xC);
}

void func_8006A1F0(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    void *temp_v1;

    temp_v1 = *(s32 *)((u8 *)D_8013816C + 0x44) + (arg0 * 0x14);
    *arg1 = *(s32 *)((u8 *)temp_v1 + 0x0);
    *arg2 = (s32) *(u16 *)((u8 *)temp_v1 + 0x6);
    *arg3 = *(s32 *)((u8 *)temp_v1 + 0x8);
    *arg4 = *(s32 *)((u8 *)temp_v1 + 0xC);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006A234);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006A2B8);

u8 func_8006A358(void) {
    return *(u8 *)((u8 *)D_8013816C + 0xB);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006A368);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006A400);

u16 func_8006A450(void) {
    return *(u16 *)((u8 *)D_8013816C + 0xE);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006A460);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006A4D4);
