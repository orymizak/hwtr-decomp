#include "common.h"
#include "include_asm.h"
s32 func_800DC6F4();
s32 __osSiGetAccess();
s32 __osSiRawStartDma(s32, void *);
s32 __osSiRelAccess();
s32 func_800DCA00();
s32 osRecvMesg(s32,s32,s32);
extern u8 D_80137FE0;
extern u8 D_80145498;

s32 func_800DC94C(s32 *, s32);

s32 func_800DC530(s32 arg0) {
    s32 temp_v0;

    __osSiGetAccess();
    if (D_80145498 != 0) {
        func_800DCA00(0);
        __osSiRawStartDma(1, &D_80137FE0);
        osRecvMesg(arg0, 0, 1);
    }
    temp_v0 = __osSiRawStartDma(0, &D_80137FE0);
    D_80145498 = 0;
    __osSiRelAccess();
    return temp_v0;
}

void func_800DC5B0(s32 arg0) {
    s32 sp10;

    func_800DC94C(&sp10, arg0);
}

s32 func_800DC5D0(s32 arg0) {
    s32 temp_v0;

    __osSiGetAccess();
    if (D_80145498 != 1) {
        func_800DC6F4();
        __osSiRawStartDma(1, &D_80137FE0);
        osRecvMesg(arg0, 0, 1);
    }
    temp_v0 = __osSiRawStartDma(0, &D_80137FE0);
    D_80145498 = 1;
    __osSiRelAccess();
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_7D130", func_800DC658);

INCLUDE_ASM("asm/nonmatchings/unit_7D130", func_800DC6F4);
