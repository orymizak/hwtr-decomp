#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 nuContDataLockKey;

void nuContDataClose(void) {
    s32 mask;

    mask = osSetIntMask(1);
    nuContDataLockKey = 1;
    osSetIntMask(mask);
}

void nuContDataOpen(void) {
    s32 mask;

    mask = osSetIntMask(1);
    nuContDataLockKey = 0;
    osSetIntMask(mask);
}
