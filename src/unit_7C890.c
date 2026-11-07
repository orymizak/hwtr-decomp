#include "common.h"
#include "include_asm.h"



void func_800DBC90(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_v1;

    temp_v1 = 0x10 - (arg1 & 0xF);
    if (temp_v1 != 0x10) {
        *(s32 *)((u8 *)arg0 + 0x0) = (s32) (arg1 + temp_v1);
    } else {
        *(s32 *)((u8 *)arg0 + 0x0) = arg1;
    }
    *(s32 *)((u8 *)arg0 + 0x8) = arg2;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 0x4) = (s32) *(s32 *)((u8 *)arg0 + 0x0);
}
