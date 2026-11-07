#include "common.h"
#include "include_asm.h"
extern s8 D_80138024;
extern s8 D_80138025;
extern s8 D_80138026;
extern s8 D_80138027;
extern s8 D_80138028;
extern s8 D_801417C4;
extern s8 D_801417C5;
extern s8 D_801417C6;
extern s8 D_801417C7;
extern s8 D_801417C8;



INCLUDE_ASM("asm/nonmatchings/unit_839C0", osGetMemSize);

INCLUDE_ASM("asm/nonmatchings/unit_839C0", osInitialize);

void func_800E3090(void) {
    D_801417C4 = 7;
    D_80138024 = 7;
    D_801417C5 = (s8) *(volatile s32 *)0xA4600014;
    D_801417C8 = (s8) *(volatile s32 *)0xA4600018;
    D_801417C6 = (s8) *(volatile s32 *)0xA460001C;
    D_801417C7 = (s8) *(volatile s32 *)0xA4600020;
    D_80138025 = (s8) *(volatile s32 *)0xA4600024;
    D_80138028 = (s8) *(volatile s32 *)0xA4600028;
    D_80138026 = (s8) *(volatile s32 *)0xA460002C;
    D_80138027 = (s8) *(volatile s32 *)0xA4600030;
}
