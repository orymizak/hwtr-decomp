#include "common.h"
#include "include_asm.h"
s32 __osSpDeviceBusy(void);
u32 osVirtualToPhysical(void *);



s32 __osSpGetStatus(void) {
    return *(volatile s32 *)0xA4040010;
}

void __osSpSetStatus(s32 arg0) {
    *(volatile s32 *)0xA4040010 = arg0;
}

s32 __osSpSetPc(u32 pc) {
    register u32 stat = *(volatile u32 *)0xA4040010;

    if (!(stat & 1)) {
        return -1;
    }
    *(volatile u32 *)0xA4080000 = pc;
    return 0;
}

s32 __osSpRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    if (__osSpDeviceBusy()) {
        return -1;
    }
    *(volatile u32 *)0xA4040000 = devAddr;
    *(volatile u32 *)0xA4040004 = osVirtualToPhysical(dramAddr);
    if (direction == 0) {
        *(volatile u32 *)0xA404000C = size - 1;
    } else {
        *(volatile u32 *)0xA4040008 = size - 1;
    }
    return 0;
}
