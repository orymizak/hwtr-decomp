#include "common.h"
#include "include_asm.h"

s32 __osDisableInt();
s32 __osRestoreInt(s32);
extern s32 __OSGlobalIntMask;

void __osSetGlobalIntMask(s32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    __OSGlobalIntMask |= arg0;
    __osRestoreInt(temp_v0);
}

void __osResetGlobalIntMask(s32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    __OSGlobalIntMask &= ~arg0 | 0x401;
    __osRestoreInt(temp_v0);
}
