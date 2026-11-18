#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

extern s32 gAudioTaskCallback;

/* Sets the callback with interrupts masked (OS_IM_NONE). */
void AudioTask_SetCallback(s32 arg0) {
    s32 mask;

    mask = osSetIntMask(OS_IM_NONE);
    gAudioTaskCallback = arg0;
    osSetIntMask(mask);
}
