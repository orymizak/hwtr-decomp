#include "common.h"
#include "include_asm.h"

s32 osViSwapBuffer(s32);

void func_800D5FF0(void *arg0) {
    osViSwapBuffer(*(s32 *)((u8 *)arg0 + 0xC));
}
