#include "common.h"
#include "include_asm.h"
s32 __osPackReadData();
s32 __osSiGetAccess();
s32 __osSiRawStartDma(s32, void *);
s32 __osSiRelAccess();
s32 __osPackRequestData();
s32 osRecvMesg(s32,s32,s32);
extern u8 D_80137FE0;
extern u8 D_80145498;

s32 __osContGetInitData(s32 *, s32);

s32 osContStartQuery(s32 arg0) {
    s32 temp_v0;

    __osSiGetAccess();
    if (D_80145498 != 0) {
        __osPackRequestData(0);
        __osSiRawStartDma(1, &D_80137FE0);
        osRecvMesg(arg0, 0, 1);
    }
    temp_v0 = __osSiRawStartDma(0, &D_80137FE0);
    D_80145498 = 0;
    __osSiRelAccess();
    return temp_v0;
}

void osContGetQuery(s32 arg0) {
    s32 sp10;

    __osContGetInitData(&sp10, arg0);
}

s32 osContStartReadData(s32 arg0) {
    s32 temp_v0;

    __osSiGetAccess();
    if (D_80145498 != 1) {
        __osPackReadData();
        __osSiRawStartDma(1, &D_80137FE0);
        osRecvMesg(arg0, 0, 1);
    }
    temp_v0 = __osSiRawStartDma(0, &D_80137FE0);
    D_80145498 = 1;
    __osSiRelAccess();
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_7D130", osContGetReadData);

INCLUDE_ASM("asm/nonmatchings/unit_7D130", __osPackReadData);
