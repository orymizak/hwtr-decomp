#include "common.h"
#include "include_asm.h"
s32 func_800D7208(s32, void *);



void func_800D6770(void *arg0) {
    (*(s32 *)((u8 *)(arg0) + 0x8)) = func_800D7208(0x208, arg0);
}

INCLUDE_ASM("asm/nonmatchings/unit_77370", func_800D67A0);
