#include "common.h"
#include "include_asm.h"
s32 func_800D8AD4(s32,s32,s32);
s32 func_800DAC90(s32,s32, s32);
s32 func_800DB2E4(s32,s32, s32);



s32 func_800DC3B0(void *arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp_a2;
    s32 temp_s0;

    temp_s0 = arg1 * 0x4C;
    func_800D8AD4((*(s32 *)((u8 *)(arg0) + 0x34)) + temp_s0 + 0x20, arg2, arg3);
    temp_a2 = (*(s32 *)((u8 *)(arg0) + 0x34)) + temp_s0;
    func_800DB2E4(temp_a2 + 0x20, 1, temp_a2);
    func_800DAC90((*(s32 *)((u8 *)(arg0) + 0x30)), 2, (*(s32 *)((u8 *)(arg0) + 0x34)) + temp_s0 + 0x20);
    return (*(s32 *)((u8 *)(arg0) + 0x34)) + temp_s0 + 0x20;
}
