#include "common.h"
#include "include_asm.h"
s32 osCreateMesgQueue(void *, void *,s32);
s32 osSendMesg(void *,s32,s32);
extern s32 D_800FB5F0;
extern u8 D_80133270;
extern u8 D_8014EC98;



void __osSiCreateAccessQueue(void) {
    D_800FB5F0 = 1;
    osCreateMesgQueue(&D_8014EC98, &D_80133270, 1);
    osSendMesg(&D_8014EC98, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_7FD20", __osSiGetAccess);

void __osSiRelAccess(void) {
    osSendMesg(&D_8014EC98, 0, 0);
}
