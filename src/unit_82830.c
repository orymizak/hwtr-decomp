#include "common.h"
#include "include_asm.h"
typedef struct { u8 txsize; u8 rxsize; u8 cmd; u8 typeh; u8 typel; u8 status; } ReqShort_;
typedef struct { u32 ramarray[15]; u32 pifstatus; } PifRam_;
extern u8 D_80145498;
void func_800E1D20(s32 channel, s32 cmd);
typedef struct { u16 type; u8 status; u8 errnum; } ContStatus_;
extern u8 D_800FC7D4;
extern PifRam_ D_8014C700;
s32 __osSiRawStartDma(s32, void *);
s32 osRecvMesg(void *, void **, s32);
void func_800E1DB4(s32, ContStatus_ *);



s32 func_800E1C30(void *queue, s32 channel) {
    s32 ret = 0;
    void *dummy;
    ContStatus_ data;

    D_800FC7D4 = 250;
    func_800E1D20(channel, 0);
    ret = __osSiRawStartDma(1, &D_8014C700);
    osRecvMesg(queue, &dummy, 1);
    ret = __osSiRawStartDma(0, &D_8014C700);
    osRecvMesg(queue, &dummy, 1);
    func_800E1DB4(channel, &data);
    if (((data.status & 1) != 0) && ((data.status & 2) != 0)) {
        return 2;
    } else if ((data.errnum != 0) || ((data.status & 1) == 0)) {
        return 1;
    } else if ((data.status & 4) != 0) {
        return 4;
    }
    return ret;
}

void func_800E1D20(s32 channel, s32 cmd) {
    u8 *ptr;
    ReqShort_ requestformat;
    s32 i;

    D_80145498 = 0xFE;
    D_8014C700.pifstatus = 1;
    ptr = (u8 *)&D_8014C700;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    for (i = 0; i < channel; i++) {
        *ptr++ = 0;
    }
    *(ReqShort_ *)ptr = requestformat;
    ptr += sizeof(ReqShort_);
    *ptr = 0xFE;
}

void func_800E1DB4(s32 channel, ContStatus_ *data) {
    u8 *ptr;
    ReqShort_ requestformat;
    s32 i;

    ptr = (u8 *)&D_8014C700;
    for (i = 0; i < channel; i++) {
        ptr++;
    }
    requestformat = *(ReqShort_ *)ptr;
    data->errnum = (requestformat.rxsize & 0xC0) >> 4;
    if (data->errnum == 0) {
        data->type = (requestformat.typel << 8) | requestformat.typeh;
        data->status = requestformat.status;
    }
}

