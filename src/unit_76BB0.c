#include "common.h"
#include "include_asm.h"

s32 osSetIntMask(s32);
extern s32 D_800F1CB4;

void func_800D5FB0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    D_800F1CB4 = arg0;
    osSetIntMask(temp_v0);
}
