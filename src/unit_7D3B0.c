#include "common.h"
#include "include_asm.h"
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u8 typeh; u8 typel; u8 status; u8 dummy1; } ContRequestFormat_;
void __osPackRequestData(u8 cmd);
typedef struct { u16 type; u8 status; u8 errnum; } OSContStatus_;
typedef struct { u8 b[0x18]; } OSMesgQueue_;
typedef struct { u8 b[0x20]; } OSTimer_;
extern s32 D_800FB5C0;
extern u8 D_80141590;
extern u8 D_80137FE0;
extern u8 D_80145498;
extern OSMesgQueue_ D_8014E980;
extern void *D_80145520;
u64 osGetTime(void);
void osCreateMesgQueue(OSMesgQueue_ *mq, void **msg, s32 count);
s32 osSetTimer(OSTimer_ *t, u64 countdown, u64 interval, OSMesgQueue_ *mq, void *msg);
s32 osRecvMesg(OSMesgQueue_ *mq, void **msg, s32 flag);
s32 __osSiRawStartDma(s32 direction, void *dramAddr);
void __osSiCreateAccessQueue(void);
void __osContGetInitData(u8 *pattern, OSContStatus_ *data);



s32 osContInit(OSMesgQueue_ *mq, u8 *bitpattern, OSContStatus_ *data) {
    void *dummy;
    s32 ret = 0;
    u64 t;
    OSTimer_ mytimer;
    OSMesgQueue_ timerMesgQueue;

    if (D_800FB5C0) {
        return 0;
    }
    D_800FB5C0 = 1;
    t = osGetTime();
    if (t < 23437500) {
        osCreateMesgQueue(&timerMesgQueue, &dummy, 1);
        osSetTimer(&mytimer, 23437500 - t, 0, &timerMesgQueue, &dummy);
        osRecvMesg(&timerMesgQueue, &dummy, 1);
    }
    D_80141590 = 4;
    __osPackRequestData(0);
    ret = __osSiRawStartDma(1, &D_80137FE0);
    osRecvMesg(mq, &dummy, 1);
    ret = __osSiRawStartDma(0, &D_80137FE0);
    osRecvMesg(mq, &dummy, 1);
    __osContGetInitData(bitpattern, data);
    D_80145498 = 0;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&D_8014E980, &D_80145520, 1);
    return ret;
}


void __osContGetInitData(u8 *pattern, OSContStatus_ *data) {
    u8 *ptr;
    ContRequestFormat_ requestHeader;
    s32 i;
    u8 bits = 0;

    ptr = &D_80137FE0;
    for (i = 0; i < D_80141590; i++, ptr += sizeof(requestHeader), data++) {
        requestHeader = *(ContRequestFormat_ *)ptr;
        data->errnum = (requestHeader.rxsize & 0xC0) >> 4;
        if (data->errnum != 0) {
            continue;
        }
        data->type = requestHeader.typel << 8 | requestHeader.typeh;
        data->status = requestHeader.status;
        bits |= 1 << i;
    }
    *pattern = bits;
}

void __osPackRequestData(u8 cmd) {
    u8 *ptr;
    ContRequestFormat_ requestHeader;
    s32 i;

    for (i = 0; i < 15; i++) {
        ((u32 *)&D_80137FE0)[i] = 0;
    }
    ((u32 *)&D_80137FE0)[15] = 1;
    ptr = &D_80137FE0;
    requestHeader.dummy = 0xFF;
    requestHeader.txsize = 1;
    requestHeader.rxsize = 3;
    requestHeader.cmd = cmd;
    requestHeader.typeh = 0xFF;
    requestHeader.typel = 0xFF;
    requestHeader.status = 0xFF;
    requestHeader.dummy1 = 0xFF;
    for (i = 0; i < D_80141590; i++) {
        *(ContRequestFormat_ *)ptr = requestHeader;
        ptr += sizeof(requestHeader);
    }
    *ptr = 0xFE;
}
