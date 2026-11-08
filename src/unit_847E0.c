#include "common.h"
#include "include_asm.h"
extern u32 D_80000308;
u32 osVirtualToPhysical(void *);
extern s32 D_800FB2A0;
extern void *D_800FB2A8;



s32 __osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    register u32 stat;

    while (stat = *(volatile u32 *)0xA4600010, stat & 3) {
    }
    *(volatile u32 *)0xA4600000 = osVirtualToPhysical(dramAddr);
    *(volatile u32 *)0xA4600004 = (D_80000308 | devAddr) & 0x1FFFFFFF;
    switch (direction) {
    case 0:
        *(volatile u32 *)0xA460000C = size - 1;
        break;
    case 1:
        *(volatile u32 *)0xA4600008 = size - 1;
        break;
    default:
        return -1;
    }
    return 0;
}

void *osPiGetCmdQueue(void) {
    if (!D_800FB2A0) {
        return NULL;
    }
    return D_800FB2A8;
}

INCLUDE_ASM("asm/nonmatchings/unit_847E0", __osEPiRawReadIo);
