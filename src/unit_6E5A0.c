#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
s32 SndHeap_GetUsed();
s32 Snd_GetUsedMem(void);
void *SndHeap_Get();

s32 SndHeap_Alloc();

void Snd_Alloc(void) {
    SndHeap_Alloc();
}

s32 Snd_GetFreeMem(void) {
    ALHeap *s = SndHeap_Get();

    return s->len - Snd_GetUsedMem();
}

s32 Snd_GetUsedMem(void) {
    return SndHeap_GetUsed();
}
