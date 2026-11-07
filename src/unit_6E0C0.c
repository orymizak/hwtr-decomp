#include "common.h"
#include "include_asm.h"
s32 osCreateThread(void *,s32, void *,s32, void *, s32);
s32 osInitialize();
s32 osStartThread(void *);
extern u8 D_801255A0;
extern u8 D_80127900;
extern u8 func_800CD524;



void main(void) {
    osInitialize();
    osCreateThread(&D_801255A0, 1, &func_800CD524, 0, &D_80127900, 0xA);
    osStartThread(&D_801255A0);
}

INCLUDE_ASM("asm/nonmatchings/unit_6E0C0", func_800CD524);
