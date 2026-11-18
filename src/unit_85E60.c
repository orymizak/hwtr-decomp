#include "common.h"
#include "include_asm.h"

extern s32 __osViCurr;

s32 __osViGetCurrentContext(void) {
    return __osViCurr;
}
