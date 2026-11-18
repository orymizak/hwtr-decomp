#include "common.h"
#include "include_asm.h"
s32 Snd_StartSound(s32, s32, s32, s32, s32);

s32 Snd_PlaySfxDefault();

void Snd_PlaySfxSimple(void) {
    Snd_PlaySfxDefault();
}

s32 Snd_Play(s32 a, s32 b, s32 c, s32 d, s32 e) {
    return Snd_StartSound(a, b, c, d, e);
}
