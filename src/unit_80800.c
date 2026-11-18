#include "common.h"
#include "include_asm.h"
typedef struct { u16 type; u8 pri; void *retQueue; } MesgHdrC_;
typedef struct { u8 b[0x18]; } MesgQueueV_;
typedef struct { u16 state; u16 retraceCount; void *framep; void *modep; u32 control; void *msgq; void *msg; } ViCtxM_;
typedef struct { s32 active; void *thread; void *cmdQueue; void *evtQueue; void *acsQueue; void *dma; void *edma; } DevMgrV_;
typedef struct { u16 type; } MesgHdrV_;
extern u16 D_80133280;
extern u32 __osViIntrCount;
extern u64 __osCurrentTime;
extern u32 __osBaseCounter;
ViCtxM_ *__osViGetCurrentContext(void);
s32 osRecvMesg(void *, void **, s32);
s32 osSendMesg(void *, void *, s32);
void __osViSwapContext(void);
void __osTimerInterrupt(void);
u32 osGetCount(void);



extern DevMgrV_ __osViDevMgr;
extern s32 __additional_scanline;
extern MesgQueueV_ viEventQueue;
extern void *viEventBuf[5];
extern MesgHdrC_ viRetraceMsg;
extern MesgHdrC_ viCounterMsg;
extern u8 viThread[];
extern u8 viThreadStack[];
void __osTimerServicesInit(void);
void osCreateMesgQueue(void *mq, void **msg, s32 count);
void osSetEventMesg(s32 e, void *mq, void *msg);
s32 osGetThreadPri(void *t);
void osSetThreadPri(void *t, s32 pri);
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void osCreateThread(void *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
void osStartThread(void *t);
void __osViInit(void);
void viMgrMain(void *arg);
void osCreateViManager(s32 pri) {
    u32 savedMask;
    s32 oldPri;
    s32 myPri;

    if (__osViDevMgr.active) {
        return;
    }
    __osTimerServicesInit();
    __additional_scanline = 0;
    osCreateMesgQueue(&viEventQueue, viEventBuf, 5);
    viRetraceMsg.type = 13;
    viRetraceMsg.pri = 0;
    viRetraceMsg.retQueue = 0;
    viCounterMsg.type = 14;
    viCounterMsg.pri = 0;
    viCounterMsg.retQueue = 0;
    osSetEventMesg(7, &viEventQueue, &viRetraceMsg);
    osSetEventMesg(3, &viEventQueue, &viCounterMsg);
    oldPri = -1;
    myPri = osGetThreadPri(0);
    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(0, pri);
    }
    savedMask = __osDisableInt();
    __osViDevMgr.active = 1;
    __osViDevMgr.thread = viThread;
    __osViDevMgr.cmdQueue = &viEventQueue;
    __osViDevMgr.evtQueue = &viEventQueue;
    __osViDevMgr.acsQueue = 0;
    __osViDevMgr.dma = 0;
    __osViDevMgr.edma = 0;
    osCreateThread(viThread, 0, viMgrMain, &__osViDevMgr, viThreadStack + 0x1000, pri);
    __osViInit();
    osStartThread(viThread);
    __osRestoreInt(savedMask);
    if (oldPri != -1) {
        osSetThreadPri(0, oldPri);
    }
}

void viMgrMain(void *arg) {
    ViCtxM_ *vc;
    DevMgrV_ *dm;
    MesgHdrV_ *mb;
    s32 first;
    u32 count;

    mb = NULL;
    first = 0;
    vc = __osViGetCurrentContext();
    D_80133280 = vc->retraceCount;
    if (D_80133280 == 0) {
        D_80133280 = 1;
    }
    dm = (DevMgrV_ *)arg;
    while (1) {
        osRecvMesg(dm->evtQueue, (void **)&mb, 1);
        switch (mb->type) {
        case 13:
            __osViSwapContext();
            D_80133280--;
            if (D_80133280 == 0) {
                vc = __osViGetCurrentContext();
                if (vc->msgq != NULL) {
                    osSendMesg(vc->msgq, vc->msg, 0);
                }
                D_80133280 = vc->retraceCount;
            }
            __osViIntrCount++;
            if (first) {
                count = osGetCount();
                __osCurrentTime = count;
                first = 0;
            }
            count = __osBaseCounter;
            __osBaseCounter = osGetCount();
            count = __osBaseCounter - count;
            __osCurrentTime = __osCurrentTime + count;
            break;
        case 14:
            __osTimerInterrupt();
            break;
        }
    }
}
