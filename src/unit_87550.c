#include "common.h"
#include "include_asm.h"



s32 __osDpDeviceBusy(void) {
    register u32 stat = *(volatile u32 *)0xA410000C;

    if (stat & 0x100) {
        return 1;
    } else {
        return 0;
    }
}

