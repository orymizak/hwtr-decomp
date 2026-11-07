#include "common.h"
#include "include_asm.h"

s32 func_800D6010();
s32 osSetIntMask(s32);
extern s32 D_800FB190;

void func_800D5F30(s32 arg0) {
    s32 temp_v0;

    func_800D6010();
    temp_v0 = osSetIntMask(1);
    D_800FB190 = arg0;
    osSetIntMask(temp_v0);
}
