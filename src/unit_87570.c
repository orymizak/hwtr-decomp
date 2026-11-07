#include "common.h"
#include "include_asm.h"



s32 __osSiDeviceBusy(void) {
    return (*(volatile s32 *)0xA4800018 & 3) != 0;
}
