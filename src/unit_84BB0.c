#include "common.h"
#include "include_asm.h"



s32 __osAiDeviceBusy(void) {
    register s32 status = *(volatile s32 *)0xA450000C;

    if (status & 0x80000000) {
        return 1;
    } else {
        return 0;
    }
}
