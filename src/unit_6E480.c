#include "common.h"
#include "include_asm.h"

s32 Snd_Alloc();
extern s32 gSndGlobalBuf;

void Snd_AllocGlobal(void) {
    gSndGlobalBuf = Snd_Alloc();
}
