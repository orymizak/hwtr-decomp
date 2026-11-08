#include "common.h"
#include "include_asm.h"

s32 __osSpSetPc(s32);

void osAfterPreNMI(void) {
    __osSpSetPc(0);
}
