#include "common.h"
#include "include_asm.h"
s32 osRecvMesg(s32, s16 **,s32);
s32 func_800D23FC();
s32 osScAddClient(s32, s32, s32);
s32 osCreateMesgQueue(s32, s32,s32);
extern s32 D_801279D4;

extern s32 D_801279D0;

void func_800D1F40(s32 arg0) {
    D_801279D0 = arg0;
}

void func_800D1F4C(void) {
    s32 temp_v0;

    temp_v0 = func_800D23FC(0x58);
    D_801279D4 = temp_v0;
    osCreateMesgQueue(temp_v0 + 8, temp_v0 + 0x20, 4);
    osCreateMesgQueue(D_801279D4 + 0x30, D_801279D4 + 0x48, 4);
    osScAddClient(D_801279D0, D_801279D4, D_801279D4 + 8);
}

void func_800D1FB0(void) {
    s16 *sp10;

    do {
        osRecvMesg(D_801279D4 + 8, &sp10, 1);
        osRecvMesg(D_801279D4 + 8, NULL, 0);
    } while (*sp10 != 1);
}

INCLUDE_ASM("asm/nonmatchings/unit_72B40", func_800D2010);
