#include "common.h"
#include "include_asm.h"
s32 osSendMesg(void *, void *,s32);
extern u8 D_800FB1A0;
extern u8 D_80144460;



void func_800D6390(void) {
    osSendMesg(&D_80144460, &D_800FB1A0, 1);
}
