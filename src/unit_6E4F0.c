#include "common.h"
#include "include_asm.h"

s32 osSetIntMask(s32);
extern s32 D_800FABA4;

void func_800CD8F0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    D_800FABA4 = arg0;
    osSetIntMask(temp_v0);
}
