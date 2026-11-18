#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 nuGfxPreNMIFunc;

void nuGfxPreNMIFuncSet(s32 arg0) {
    s32 mask;

    mask = osSetIntMask(1);
    nuGfxPreNMIFunc = arg0;
    osSetIntMask(mask);
}
