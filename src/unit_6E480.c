#include "common.h"
#include "include_asm.h"

s32 Snd_Alloc();
extern s32 D_800FABC0;

void func_800CD880(void) {
    D_800FABC0 = Snd_Alloc();
}
