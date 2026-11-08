#include "common.h"
#include "include_asm.h"
typedef struct { u32 errStatus; void *dramAddr; void *C2Addr; u32 sectorSize; u32 C1ErrNum; u32 C1ErrSector[4]; } BlockInfo_;
typedef struct { u32 cmdType; u16 transferMode; u16 blockNum; s32 sectorNum; u32 devAddr; u32 bmCtlShadow; u32 seqCtlShadow; BlockInfo_ block[2]; } TranxInfo_;
typedef struct PiHandle3_ { struct PiHandle3_ *next; u8 type; u8 latency; u8 pageSize; u8 relDuration; u8 pulse; u8 domain; u32 baseAddress; u32 speed; TranxInfo_ transferInfo; } PiHandle3_;
typedef struct { u16 type; u8 pri; u8 status; void *retQueue; } IoMesgHdr_;
typedef struct { IoMesgHdr_ hdr; void *dramAddr; u32 devAddr; u32 size; PiHandle3_ *piHandle; } IoMesg_;
typedef struct { s32 active; void *thread; void *cmdQueue; void *evtQueue; void *acsQueue; s32 (*dma)(s32, u32, void *, u32); s32 (*edma)(PiHandle3_ *, s32, u32, void *, u32); } DevMgr3_;
s32 osRecvMesg(void *mq, void **msg, s32 flag);
s32 osSendMesg(void *mq, void *msg, s32 flag);
void __osResetGlobalIntMask(u32 mask);
void __osSetGlobalIntMask(u32 mask);
s32 __osEPiRawWriteIo(PiHandle3_ *pihandle, u32 devAddr, u32 data);
s32 __osEPiRawReadIo(PiHandle3_ *pihandle, u32 devAddr, u32 *data);
void osYieldThread(void);



void __osDevMgrMain(void *args) {
    IoMesg_ *mb;
    void *em;
    void *dummy;
    s32 ret;
    DevMgr3_ *dm;
    s32 messageSend = 0;

    dm = (DevMgr3_ *)args;
    mb = 0;
    ret = 0;
    while (1) {
        osRecvMesg(dm->cmdQueue, (void **)&mb, 1);
        if (mb->piHandle != 0 && mb->piHandle->type == 2 &&
            (mb->piHandle->transferInfo.cmdType == 0 || mb->piHandle->transferInfo.cmdType == 1)) {
            BlockInfo_ *blockInfo;
            TranxInfo_ *info;
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            info->sectorNum = -1;
            if (info->transferMode != 3) {
                blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr - blockInfo->sectorSize);
            }
            if (info->transferMode == 2 && mb->piHandle->transferInfo.cmdType == 0) {
                messageSend = 1;
            } else {
                messageSend = 0;
            }
            osRecvMesg(dm->acsQueue, &dummy, 1);
            __osResetGlobalIntMask(0x00100401);
            __osEPiRawWriteIo(mb->piHandle, 0x05000510, (info->bmCtlShadow | 0x80000000));
        readblock1:
            osRecvMesg(dm->evtQueue, &em, 1);
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            if (blockInfo->errStatus == 29) {
                u32 stat;
                __osEPiRawWriteIo(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x10000000);
                __osEPiRawWriteIo(mb->piHandle, 0x05000510, info->bmCtlShadow);
                __osEPiRawReadIo(mb->piHandle, 0x05000508, &stat);
                if (stat & 0x02000000) {
                    __osEPiRawWriteIo(mb->piHandle, 0x05000510, info->bmCtlShadow | 0x01000000);
                }
                blockInfo->errStatus = 4;
                *(volatile u32 *)0xA4600010 = 2;
                __osSetGlobalIntMask(0x00100401 | 0x800);
            }
            osSendMesg(mb->hdr.retQueue, mb, 0);
            if (messageSend == 1 && mb->piHandle->transferInfo.block[0].errStatus == 0) {
                messageSend = 0;
                goto readblock1;
            }
            osSendMesg(dm->acsQueue, 0, 0);
            if (mb->piHandle->transferInfo.blockNum == 1) {
                osYieldThread();
            }
        } else {
            switch (mb->hdr.type) {
                case 11:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->dma(0, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 12:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->dma(1, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 15:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->edma(mb->piHandle, 0, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 16:
                    osRecvMesg(dm->acsQueue, &dummy, 1);
                    ret = dm->edma(mb->piHandle, 1, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 10:
                    osSendMesg(mb->hdr.retQueue, mb, 0);
                    ret = -1;
                    break;
                default:
                    ret = -1;
                    break;
            }
            if (ret == 0) {
                osRecvMesg(dm->evtQueue, &em, 1);
                osSendMesg(mb->hdr.retQueue, mb, 0);
                osSendMesg(dm->acsQueue, 0, 0);
            }
        }
    }
}
