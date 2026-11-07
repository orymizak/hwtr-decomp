#include "common.h"
#include "include_asm.h"

s32 osViBlack(s32);
extern s32 D_80140E44;

void func_800D61C0(void) {
    D_80140E44 = 0;
    osViBlack(1);
}
