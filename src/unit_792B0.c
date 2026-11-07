#include "common.h"
#include "include_asm.h"
s32 osCreateMesgQueue(void *, void *,s32);
s32 osSendMesg(void *,s32,s32);
extern s32 D_800FB2E0;
extern u8 D_80133220;
extern u8 D_80141678;



void __osPiCreateAccessQueue(void) {
    D_800FB2E0 = 1;
    osCreateMesgQueue(&D_80141678, &D_80133220, 1);
    osSendMesg(&D_80141678, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_792B0", __osPiGetAccess);

void __osPiRelAccess(void) {
    osSendMesg(&D_80141678, 0, 0);
}
