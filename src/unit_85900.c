#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_85900", __osSetFpcCsr);

INCLUDE_ASM("asm/nonmatchings/unit_85900", __osSetSR);

INCLUDE_ASM("asm/nonmatchings/unit_85900", __osSetWatchLo);

s32 __osSpDeviceBusy(void) {
    return (*(volatile s32 *)0xA4040010 & 0x1C) != 0;
}
