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

extern u32 __OSGlobalIntMask;
extern u32 D_A430000C;
extern u16 D_801017D0[];

OSIntMask osSetIntMask(OSIntMask mask) {
    register OSIntMask ret asm("$2");
    asm volatile(
        ".set noreorder\n\t"
        ".set noat\n\t"
        "mfc0 $t4, $12\n\t"
        "andi $2, $t4, 0xFF01\n\t"
        "lui $t0, %%hi(__OSGlobalIntMask)\n\t"
        "addiu $t0, $t0, %%lo(__OSGlobalIntMask)\n\t"
        "lw $t3, 0($t0)\n\t"
        "addiu $at, $zero, -1\n\t"
        "xor $t0, $t3, $at\n\t"
        "andi $t0, $t0, 0xFF00\n\t"
        "or $2, $2, $t0\n\t"
        "lui $t2, %%hi(D_A430000C)\n\t"
        "lw $t2, %%lo(D_A430000C)($t2)\n\t"
        "beqz $t2, 1f\n\t"
        "srl $t1, $t3, 16\n\t"
        "addiu $at, $zero, -1\n\t"
        "xor $t1, $t1, $at\n\t"
        "andi $t1, $t1, 0x3F\n\t"
        "or $t2, $t2, $t1\n"
        "1:\n\t"
        "sll $t2, $t2, 16\n\t"
        "or $2, $2, $t2\n\t"
        "lui $at, 0x3F\n\t"
        "and $t0, %1, $at\n\t"
        "and $t0, $t0, $t3\n\t"
        "srl $t0, $t0, 15\n\t"
        "lui $t2, %%hi(D_801017D0)\n\t"
        "addu $t2, $t2, $t0\n\t"
        "lhu $t2, %%lo(D_801017D0)($t2)\n\t"
        "lui $at, %%hi(D_A430000C)\n\t"
        "sw $t2, %%lo(D_A430000C)($at)\n\t"
        "andi $t0, %1, 0xFF01\n\t"
        "andi $t1, $t3, 0xFF00\n\t"
        "and $t0, $t0, $t1\n\t"
        "lui $at, 0xFFFF\n\t"
        "ori $at, $at, 0x00FF\n\t"
        "and $t4, $t4, $at\n\t"
        "or $t4, $t4, $t0\n\t"
        "mtc0 $t4, $12\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set at\n\t"
        ".set reorder"
        : "=r"(ret)
        : "r"(mask)
        : "t0", "t1", "t2", "t3", "t4"
    );
    return ret;
}



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
