#include "common.h"
#include "include_asm.h"
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u8 addrh; u8 addrl; u8 data[32]; u8 datacrc; } RamRead_;
typedef struct { u32 ramarray[15]; u32 pifstatus; } PifRam_;
extern PifRam_ D_8014C700;
extern u8 D_80145498;
extern s32 D_800FC7E0;
void __osSiGetAccess(void);
void __osSiRelAccess(void);
s32 __osSiRawStartDma(s32, void *);
s32 osRecvMesg(void *, void **, s32);
u8 __osContAddressCrc(u16);
u8 __osContDataCrc(u8 *);
s32 __osPfsGetStatus(void *, s32);
void bcopy(void *, void *, s32);



s32 __osContRamRead(void *mq, s32 channel, u16 address, u8 *buffer) {
    s32 ret;
    s32 i;
    u8 *ptr;
    s32 retry;

    ret = 0;
    retry = 2;
    __osSiGetAccess();
    do {
        ptr = (u8 *)&D_8014C700;
        if (D_80145498 != 2 || D_800FC7E0 != channel) {
            D_80145498 = 2;
            D_800FC7E0 = channel;
            for (i = 0; i < channel; i++) {
                *ptr++ = 0;
            }
            D_8014C700.pifstatus = 1;
            ((RamRead_ *)ptr)->dummy = 0xFF;
            ((RamRead_ *)ptr)->txsize = 3;
            ((RamRead_ *)ptr)->rxsize = 0x21;
            ((RamRead_ *)ptr)->cmd = 2;
            ((RamRead_ *)ptr)->datacrc = 0xFF;
            ptr[sizeof(RamRead_)] = 0xFE;
        } else {
            ptr += channel;
        }
        ((RamRead_ *)ptr)->addrh = address >> 3;
        ((RamRead_ *)ptr)->addrl = __osContAddressCrc(address) | (address << 5);
        ret = __osSiRawStartDma(1, &D_8014C700);
        osRecvMesg(mq, NULL, 1);
        ret = __osSiRawStartDma(0, &D_8014C700);
        osRecvMesg(mq, NULL, 1);
        ret = (((RamRead_ *)ptr)->rxsize & 0xC0) >> 4;
        if (ret == 0) {
            if (__osContDataCrc(((RamRead_ *)ptr)->data) != ((RamRead_ *)ptr)->datacrc) {
                ret = __osPfsGetStatus(mq, channel);
                if (ret != 0) {
                    break;
                }
                ret = 4;
            } else {
                bcopy(((RamRead_ *)ptr)->data, buffer, 32);
            }
        } else {
            ret = 1;
        }
    } while ((ret == 4) && (retry-- >= 0));
    __osSiRelAccess();
    return ret;
}
