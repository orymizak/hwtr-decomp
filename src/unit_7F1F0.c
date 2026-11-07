#include "common.h"
#include "include_asm.h"

s32 __osSpSetStatus(s32);

void osSpTaskYield(void) {
    __osSpSetStatus(0x400);
}
