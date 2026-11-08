#include "common.h"
#include "include_asm.h"
s32 __osDpDeviceBusy(void);
u32 osVirtualToPhysical(void *);



INCLUDE_ASM("asm/nonmatchings/unit_84CD0", func_800E40D0);

INCLUDE_ASM("asm/nonmatchings/unit_84CD0", _Printf);

INCLUDE_ASM("asm/nonmatchings/unit_84CD0", _Putfld);

s32 osDpSetNextBuffer(void *bufPtr, u64 size) {
    if (__osDpDeviceBusy()) {
        return -1;
    }
    *(volatile u32 *)0xA410000C = 1;
    do {
    } while (*(volatile u32 *)0xA410000C & 1);
    *(volatile u32 *)0xA4100000 = osVirtualToPhysical(bufPtr);
    *(volatile u32 *)0xA4100004 = osVirtualToPhysical(bufPtr) + (u32)size;
    return 0;
}
