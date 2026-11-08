#include "common.h"
#include "include_asm.h"
typedef struct { s32 active; void *thread; void *cmdQueue; void *evtQueue; void *acsQueue; void *dma; void *edma; } DevMgr_;
extern DevMgr_ D_800FB2A0;
extern u32 D_800FB2E0;
extern u8 D_80132050[];
extern u8 D_80133200[];
extern u8 D_80132200[];
extern void *D_80133218[];
extern u8 D_80141678[];
void osCreateMesgQueue(void *, void **, s32);
void __osPiCreateAccessQueue(void);
void osSetEventMesg(s32, void *, void *);
s32 osGetThreadPri(void *);
void osSetThreadPri(void *, s32);
u32 __osDisableInt(void);
void __osRestoreInt(u32);
s32 __osPiRawStartDma(s32, u32, void *, u32);
void __osDevMgrMain(void *);
void osCreateThread(void *, s32, void (*)(void *), void *, void *, s32);
void osStartThread(void *);
typedef struct PiHandle_ { struct PiHandle_ *next; u8 type; u8 latency; u8 pageSize; u8 relDuration; u8 pulse; u8 domain; u8 pad[2]; u32 baseAddress; } PiHandle_;
extern PiHandle_ *D_800FB2C0[];
u32 osVirtualToPhysical(void *);



INCLUDE_ASM("asm/nonmatchings/unit_788B0", osSetIntMask);

s32 __osEPiRawStartDma(PiHandle_ *pihandle, s32 direction, u32 devAddr, void *dramAddr, u32 size);
void osCreatePiManager(s32 pri, void *cmdQ, void **cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    s32 oldPri;
    s32 myPri;

    if (!D_800FB2A0.active) {
        osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
        osCreateMesgQueue(D_80133200, D_80133218, 1);
        if (!D_800FB2E0) {
            __osPiCreateAccessQueue();
        }
        osSetEventMesg(8, D_80133200, (void *)0x22222222);
        oldPri = -1;
        myPri = osGetThreadPri(NULL);
        if (myPri < pri) {
            oldPri = myPri;
            osSetThreadPri(NULL, pri);
        }
        savedMask = __osDisableInt();
        D_800FB2A0.active = 1;
        D_800FB2A0.thread = D_80132050;
        D_800FB2A0.cmdQueue = cmdQ;
        D_800FB2A0.evtQueue = D_80133200;
        D_800FB2A0.acsQueue = D_80141678;
        D_800FB2A0.dma = __osPiRawStartDma;
        D_800FB2A0.edma = __osEPiRawStartDma;
        osCreateThread(D_80132050, 0, __osDevMgrMain, &D_800FB2A0, &D_80132200[0x1000], pri);
        osStartThread(D_80132050);
        __osRestoreInt(savedMask);
        if (oldPri != -1) {
            osSetThreadPri(NULL, oldPri);
        }
    }
}

s32 __osEPiRawStartDma(PiHandle_ *pihandle, s32 direction, u32 devAddr, void *dramAddr, u32 size) {
    u32 stat;
    u32 domain;

    while (stat = *(volatile u32 *)0xA4600010, stat & 3) {
    }
    domain = pihandle->domain;
    if (D_800FB2C0[domain]->type != pihandle->type) {
        PiHandle_ *cHandle = D_800FB2C0[domain];

        if (domain == 0) {
            if (cHandle->latency != pihandle->latency) {
                *(volatile u32 *)0xA4600014 = pihandle->latency;
            }
            if (cHandle->pageSize != pihandle->pageSize) {
                *(volatile u32 *)0xA460001C = pihandle->pageSize;
            }
            if (cHandle->relDuration != pihandle->relDuration) {
                *(volatile u32 *)0xA4600020 = pihandle->relDuration;
            }
            if (cHandle->pulse != pihandle->pulse) {
                *(volatile u32 *)0xA4600018 = pihandle->pulse;
            }
        } else {
            if (cHandle->latency != pihandle->latency) {
                *(volatile u32 *)0xA4600024 = pihandle->latency;
            }
            if (cHandle->pageSize != pihandle->pageSize) {
                *(volatile u32 *)0xA460002C = pihandle->pageSize;
            }
            if (cHandle->relDuration != pihandle->relDuration) {
                *(volatile u32 *)0xA4600030 = pihandle->relDuration;
            }
            if (cHandle->pulse != pihandle->pulse) {
                *(volatile u32 *)0xA4600028 = pihandle->pulse;
            }
        }
        cHandle->type = pihandle->type;
        cHandle->latency = pihandle->latency;
        cHandle->pageSize = pihandle->pageSize;
        cHandle->relDuration = pihandle->relDuration;
        cHandle->pulse = pihandle->pulse;
    }
    *(volatile u32 *)0xA4600000 = osVirtualToPhysical(dramAddr);
    *(volatile u32 *)0xA4600004 = (pihandle->baseAddress | devAddr) & 0x1FFFFFFF;
    switch (direction) {
    case 0:
        *(volatile u32 *)0xA460000C = size - 1;
        break;
    case 1:
        *(volatile u32 *)0xA4600008 = size - 1;
        break;
    default:
        return -1;
    }
    return 0;
}
