#include "common.h"
#include "include_asm.h"

#define OS_IM_RCP 0x401

s32 __osDisableInt();
s32 __osRestoreInt(s32);
extern s32 __OSGlobalIntMask;

void __osSetGlobalIntMask(s32 mask) {
    register u32 saveMask = __osDisableInt();

    __OSGlobalIntMask |= mask;
    __osRestoreInt(saveMask);
}

void __osResetGlobalIntMask(s32 mask) {
    register u32 saveMask = __osDisableInt();

    __OSGlobalIntMask &= ~(mask & ~OS_IM_RCP);
    __osRestoreInt(saveMask);
}
