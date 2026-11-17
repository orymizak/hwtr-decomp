#include "common.h"
#include "include_asm.h"

s32 osSetIntMask(s32);
extern s32 D_800F1CB4;

void func_800D5FB0(s32 arg0) {
    s32 mask;

    mask = osSetIntMask(1);
    D_800F1CB4 = arg0;
    osSetIntMask(mask);
}
