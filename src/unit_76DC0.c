#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 nuGfxDisplay;

void nuGfxDisplayOff(void) {
    nuGfxDisplay = 0;
    osViBlack(1);
}
