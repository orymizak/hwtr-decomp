#include "common.h"
#include "include_asm.h"
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u8 typeh; u8 typel; u8 status; u8 dummy1; } ReqFormat_;
extern u32 D_8014C73C;
extern u8 D_80145498;
void __osPfsRequestData(u8 cmd);
typedef struct { u16 type; u8 status; u8 errnum; } ContStatus_;
extern u8 D_8014C700;
extern u8 D_80141590;
void __osSiGetAccess(void);
void __osSiRelAccess(void);
s32 __osSiRawStartDma(s32 direction, void *dramAddr);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
void func_800E5BB0(u8 *pattern, ContStatus_ *data);



INCLUDE_ASM("asm/nonmatchings/unit_85E70", osPfsChecker);

INCLUDE_ASM("asm/nonmatchings/unit_85E70", corrupted_init);

INCLUDE_ASM("asm/nonmatchings/unit_85E70", corrupted);

s32 osPfsIsPlug(void *mq, u8 *pattern) {
    s32 ret = 0;
    void *msg;
    u8 bitpattern;
    ContStatus_ contData[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    __osSiGetAccess();
    do {
        __osPfsRequestData(0);
        ret = __osSiRawStartDma(1, &D_8014C700);
        osRecvMesg(mq, &msg, 1);
        ret = __osSiRawStartDma(0, &D_8014C700);
        osRecvMesg(mq, &msg, 1);
        func_800E5BB0(&bitpattern, &contData[0]);
        for (channel = 0; channel < D_80141590; channel++) {
            if ((contData[channel].status & 4) == 0) {
                crcErrorCount--;
                break;
            }
        }
        if (channel == D_80141590) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);
    for (channel = 0; channel < D_80141590; channel++) {
        if ((contData[channel].errnum == 0) && ((contData[channel].status & 1) != 0)) {
            bits |= (1 << channel);
        }
    }
    __osSiRelAccess();
    *pattern = bits;
    return ret;
}


void __osPfsRequestData(u8 cmd) {
    u8 *ptr = &D_8014C700;
    ReqFormat_ requestformat;
    s32 i;

    D_80145498 = cmd;
    D_8014C73C = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    for (i = 0; i < D_80141590; i++) {
        *((ReqFormat_ *)ptr) = requestformat;
        ptr += sizeof(ReqFormat_);
    }
    *ptr = 0xFE;
}


void func_800E5BB0(u8 *pattern, ContStatus_ *data) {
    u8 *ptr;
    ReqFormat_ requestformat;
    s32 i;
    u8 bits = 0;

    ptr = &D_8014C700;
    for (i = 0; i < D_80141590; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *((ReqFormat_ *)ptr);
        data->errnum = (requestformat.rxsize & 0xC0) >> 4;
        if (data->errnum != 0) {
            continue;
        }
        data->type = ((requestformat.typel << 8) | requestformat.typeh);
        data->status = requestformat.status;
        bits |= (1 << i);
    }
    *pattern = bits;
}
