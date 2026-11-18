#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 gAudioTaskCallback;

    /* TODO (Clean version):
    Atomic variable update using libultra interrupt mask constants:
    OSIntMask mask = osSetIntMask(OS_IM_RCP); // 1 = OS_IM_RCP
    gAudioTaskCallback = arg0;
    osSetIntMask(mask);
    */
void AudioTask_SetCallback(s32 arg0) {
    s32 mask;

    mask = osSetIntMask(1);
    gAudioTaskCallback = arg0;
    osSetIntMask(mask);
}
