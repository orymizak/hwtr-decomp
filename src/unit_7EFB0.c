#include "common.h"
#include "include_asm.h"

s32 __osSpDeviceBusy();
s32 __osSpSetStatus(s32);

INCLUDE_ASM("asm/nonmatchings/unit_7EFB0", osSpTaskLoad);

void osSpTaskStartGo(void) {
    do {

    } while (__osSpDeviceBusy() != 0);
    __osSpSetStatus(0x125);
}
