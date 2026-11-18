#include "common.h"
#include "ultra64.h"
#include "include_asm.h"

s32 func_800D6010();
extern s32 D_800FB190;

void func_800D5F30(s32 arg0) {
    s32 mask;

    func_800D6010();
    mask = osSetIntMask(1);
    D_800FB190 = arg0;
    osSetIntMask(mask);
}
