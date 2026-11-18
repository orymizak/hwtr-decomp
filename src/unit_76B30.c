#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

s32 nuGfxTaskAllEndWait();
extern s32 nuGfxFunc;

void nuGfxFuncSet(s32 arg0) {
    s32 mask;

    nuGfxTaskAllEndWait();
    mask = osSetIntMask(1);
    nuGfxFunc = arg0;
    osSetIntMask(mask);
}
