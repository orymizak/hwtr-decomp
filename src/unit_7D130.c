#include "common.h"
#include "include_asm.h"
void __osPackReadData(void);
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u16 button; s8 stick_x; s8 stick_y; } ContReadFormat_;
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errnum; } OSContPad_;
extern u8 D_80141590;
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

void osContGetReadData(OSContPad_ *data) {
    u8 *ptr = &D_80137FE0;
    ContReadFormat_ readformat;
    s32 i;

    for (i = 0; i < D_80141590; i++, ptr += sizeof(ContReadFormat_), data++) {
        readformat = *(ContReadFormat_ *)ptr;
        data->errnum = (readformat.rxsize & 0xC0) >> 4;
        if (data->errnum != 0) {
            continue;
        }
        data->button = readformat.button;
        data->stick_x = readformat.stick_x;
        data->stick_y = readformat.stick_y;
    }
}


void __osPackReadData(void) {
    u8 *ptr = &D_80137FE0;
    ContReadFormat_ readformat;
    s32 i;

    for (i = 0; i < 15; i++) {
        ((u32 *)&D_80137FE0)[i] = 0;
    }
    ((u32 *)&D_80137FE0)[15] = 1;
    readformat.dummy = 0xFF;
    readformat.txsize = 1;
    readformat.rxsize = 4;
    readformat.cmd = 1;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;
    for (i = 0; i < D_80141590; i++) {
        *(ContReadFormat_ *)ptr = readformat;
        ptr += sizeof(ContReadFormat_);
    }
    *ptr = 0xFE;
}
