#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern u8 D_801347A0;
s32 func_800D7208(s32, s32);



void func_800D6340(s32 arg0) {
    func_800D7208(0x101, arg0);
}

void func_800D6360(void) {
    osRecvMesg(&D_801347A0, 0, 1);
}
