#include "common.h"
#include "include_asm.h"
#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/piint.h"
#include "PR/rdb.h"

extern OSThread piThread;


extern STACK(piThreadStack, OS_PIM_STACKSIZE);


extern OSMesgQueue piEventQueue;


extern OSMesg piEventBuf[1];


extern OSDevMgr __osPiDevMgr;


extern OSPiHandle* __osPiTable;


extern OSPiHandle __Dom1SpeedParam;


extern OSPiHandle __Dom2SpeedParam;


extern OSPiHandle* __osCurrentHandle[2];

#include "PRinternal/piint.h"
#include "PR/ultraerror.h"

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

INCLUDE_ASM("asm/nonmatchings/unit_788B0", osSetIntMask);



void osCreatePiManager(OSPri pri, OSMesgQueue* cmdQ, OSMesg* cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;


    if (__osPiDevMgr.active) {
        return;
    }
    osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
    osCreateMesgQueue(&piEventQueue, (OSMesg*)piEventBuf, 1);

    if (!__osPiAccessQueueEnabled) {
        __osPiCreateAccessQueue();
    }

    osSetEventMesg(OS_EVENT_PI, &piEventQueue, (OSMesg)0x22222222);
    oldPri = -1;
    myPri = osGetThreadPri(NULL);

    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(NULL, pri);
    }

    savedMask = __osDisableInt();
    __osPiDevMgr.active = 1;
    __osPiDevMgr.thread = &piThread;
    __osPiDevMgr.cmdQueue = cmdQ;
    __osPiDevMgr.evtQueue = &piEventQueue;
    __osPiDevMgr.acsQueue = &__osPiAccessQueue;
    __osPiDevMgr.dma = __osPiRawStartDma;
    __osPiDevMgr.edma = __osEPiRawStartDma;
    osCreateThread(&piThread, 0, __osDevMgrMain, &__osPiDevMgr, STACK_START(piThreadStack), pri);
    osStartThread(&piThread);

    __osRestoreInt(savedMask);

    if (oldPri != -1) {
        osSetThreadPri(NULL, oldPri);
    }
}


s32 __osEPiRawStartDma(OSPiHandle* pihandle, s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    u32 stat;
    u32 domain;


    EPI_SYNC(pihandle, stat, domain);
    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS(pihandle->baseAddress | devAddr));

    switch (direction) {
        case OS_READ:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;
        case OS_WRITE:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;
        default:
            return -1;
    }
    return 0;
}
