#include "common.h"
#include "include_asm.h"
s32 func_800D7208(s32, void *);
extern u8 D_8014E998;



void func_800D66D0(void *arg0, s32 arg1) {
    void *temp_v0;

    temp_v0 = (arg1 * 0x68) + &D_8014E998;
    (*(s32 *)((u8 *)(arg0) + 0x0)) = temp_v0;
    (*(s32 *)((u8 *)(temp_v0) + 0x8)) = arg1;
    (*(s32 *)((u8 *)(arg0) + 0x8)) = func_800D7208(0x201, arg0);
}
