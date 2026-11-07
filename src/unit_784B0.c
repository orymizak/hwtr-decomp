#include "common.h"
#include "include_asm.h"

s32 osSetIntMask(s32);
extern s32 D_801380AC;

void func_800D78B0(void) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    D_801380AC = 1;
    osSetIntMask(temp_v0);
}

void func_800D78E0(void) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    D_801380AC = 0;
    osSetIntMask(temp_v0);
}
