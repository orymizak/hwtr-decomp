#include "common.h"
#include "include_asm.h"
s32 bcopy(void *, s32,s32);
s32 func_800D7664();
s32 func_800D768C();
extern u8 D_80141090;



void func_800D63C0(s32 arg0, s32 arg1) {
    func_800D7664();
    bcopy((arg1 * 6) + &D_80141090, arg0, 6);
    func_800D768C();
}
