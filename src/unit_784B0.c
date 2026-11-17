#include "common.h"
#include "include_asm.h"

s32 osSetIntMask(s32);
extern s32 D_801380AC;

void func_800D78B0(void) {
    s32 mask;

    mask = osSetIntMask(1);
    D_801380AC = 1;
    osSetIntMask(mask);
}

void func_800D78E0(void) {
    s32 mask;

    mask = osSetIntMask(1);
    D_801380AC = 0;
    osSetIntMask(mask);
}
