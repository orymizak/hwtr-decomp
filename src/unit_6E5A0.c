#include "common.h"
#include "include_asm.h"
s32 SndHeap_GetUsed();
s32 Snd_GetUsedMem(void);
void *SndHeap_Get();

s32 SndHeap_Alloc();

void Snd_Alloc(void) {
    SndHeap_Alloc();
}

    /* TODO (Clean version):
    Replace raw pointer byte offset 0x8 with a structured type:
    typedef struct {
        u8 pad00[8];
        s32 unk08;
    } ContextHeader; // Adapt type name according to SndHeap_Get definition
    ContextHeader *s = SndHeap_Get();
    return s->unk08 - Snd_GetUsedMem();
    */
s32 Snd_GetFreeMem(void) {
    u8 *s = SndHeap_Get();

    return *(s32 *)(s + 8) - Snd_GetUsedMem();
}

s32 Snd_GetUsedMem(void) {
    return SndHeap_GetUsed();
}
