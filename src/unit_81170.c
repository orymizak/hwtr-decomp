#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } OSPfs_;
typedef struct { u32 ramarray[15]; u32 pifstatus; } PifRam_;
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u8 addrh; u8 addrl; u8 data[32]; u8 datacrc; } RamReadFormat_;
extern PifRam_ D_801344A0[4];
extern u8 D_80145498;
void __osSiGetAccess(void);
void __osSiRelAccess(void);
s32 __osSiRawStartDma(s32 direction, void *dramAddr);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
u8 __osContAddressCrc(u16 addr);
s32 __osPfsSelectBank(OSPfs_ *pfs, u8 bank);
s32 __osContRamRead(void *mq, s32 channel, u16 address, u8 *buffer);



s32 __osMotorAccess(OSPfs_ *pfs, s32 flag) {
    s32 i;
    s32 ret;
    u8 *ptr = (u8 *)&D_801344A0[pfs->channel];

    if (!(pfs->status & 8)) {
        return 5;
    }
    __osSiGetAccess();
    D_801344A0[pfs->channel].pifstatus = 1;
    ptr += pfs->channel;
    for (i = 0; i < 32; i++) {
        ((RamReadFormat_ *)ptr)->data[i] = flag;
    }
    D_80145498 = 0xFE;
    __osSiRawStartDma(1, &D_801344A0[pfs->channel]);
    osRecvMesg(pfs->queue, 0, 1);
    ret = __osSiRawStartDma(0, &D_801344A0[pfs->channel]);
    osRecvMesg(pfs->queue, 0, 1);
    ret = ((RamReadFormat_ *)ptr)->rxsize & 0xC0;
    if (!ret) {
        if (!flag) {
            if (((RamReadFormat_ *)ptr)->datacrc != 0) {
                ret = 4;
            }
        } else {
            if (((RamReadFormat_ *)ptr)->datacrc != 0xEB) {
                ret = 4;
            }
        }
    }
    __osSiRelAccess();
    return ret;
}

static void __osMakeMotorData(s32 channel, PifRam_ *mdata) {
    u8 *ptr = (u8 *)mdata->ramarray;
    RamReadFormat_ ramreadformat;
    s32 i;

    ramreadformat.dummy = 0xFF;
    ramreadformat.txsize = 35;
    ramreadformat.rxsize = 1;
    ramreadformat.cmd = 3;
    ramreadformat.addrh = 0x600 >> 3;
    ramreadformat.addrl = (u8)(__osContAddressCrc(0x600) | (0x600 << 5));
    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *ptr++ = 0;
        }
    }
    *(RamReadFormat_ *)ptr = ramreadformat;
    ptr += sizeof(RamReadFormat_);
    ptr[0] = 0xFE;
}
s32 osMotorInit(void *mq, OSPfs_ *pfs, s32 channel) {
    s32 ret;
    u8 temp[32];

    pfs->queue = mq;
    pfs->channel = channel;
    pfs->activebank = 0xFF;
    pfs->status = 0;
    ret = __osPfsSelectBank(pfs, 0xFE);
    if (ret == 2) {
        ret = __osPfsSelectBank(pfs, 0x80);
    }
    if (ret != 0) {
        return ret;
    }
    ret = __osContRamRead(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    } else if (temp[31] == 254) {
        return 11;
    }
    ret = __osPfsSelectBank(pfs, 0x80);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }
    ret = __osContRamRead(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    } else if (temp[31] != 0x80) {
        return 11;
    }
    if (!(pfs->status & 8)) {
        __osMakeMotorData(channel, &D_801344A0[channel]);
    }
    pfs->status = 8;
    return 0;
}
