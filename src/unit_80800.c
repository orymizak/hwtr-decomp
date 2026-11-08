#include "common.h"
#include "include_asm.h"
typedef struct { u16 type; u8 pri; void *retQueue; } MesgHdrC_;
typedef struct { u8 b[0x18]; } MesgQueueV_;
typedef struct { u16 state; u16 retraceCount; void *framep; void *modep; u32 control; void *msgq; void *msg; } ViCtxM_;
typedef struct { s32 active; void *thread; void *cmdQueue; void *evtQueue; void *acsQueue; void *dma; void *edma; } DevMgrV_;
typedef struct { u16 type; } MesgHdrV_;
extern u16 D_80133280;
extern u32 D_80138B14;
extern u64 D_801415B0;
extern u32 D_801345C4;
ViCtxM_ *func_800E5260(void);
s32 osRecvMesg(void *, void **, s32);
s32 osSendMesg(void *, void *, s32);
void __osViSwapContext(void);
void __osTimerInterrupt(void);
u32 osGetCount(void);



extern DevMgrV_ D_800FB630;
extern s32 D_800FB64C;
extern MesgQueueV_ D_80134440;
extern void *D_80134458[5];
extern MesgHdrC_ D_80134470;
extern MesgHdrC_ D_80134488;
extern u8 D_80133288[];
extern u8 D_80133440[];
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

    if (D_800FB630.active) {
        return;
    }
    __osTimerServicesInit();
    D_800FB64C = 0;
    osCreateMesgQueue(&D_80134440, D_80134458, 5);
    D_80134470.type = 13;
    D_80134470.pri = 0;
    D_80134470.retQueue = 0;
    D_80134488.type = 14;
    D_80134488.pri = 0;
    D_80134488.retQueue = 0;
    osSetEventMesg(7, &D_80134440, &D_80134470);
    osSetEventMesg(3, &D_80134440, &D_80134488);
    oldPri = -1;
    myPri = osGetThreadPri(0);
    if (myPri < pri) {
        oldPri = myPri;
        osSetThreadPri(0, pri);
    }
    savedMask = __osDisableInt();
    D_800FB630.active = 1;
    D_800FB630.thread = D_80133288;
    D_800FB630.cmdQueue = &D_80134440;
    D_800FB630.evtQueue = &D_80134440;
    D_800FB630.acsQueue = 0;
    D_800FB630.dma = 0;
    D_800FB630.edma = 0;
    osCreateThread(D_80133288, 0, viMgrMain, &D_800FB630, D_80133440 + 0x1000, pri);
    __osViInit();
    osStartThread(D_80133288);
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
    vc = func_800E5260();
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
                vc = func_800E5260();
                if (vc->msgq != NULL) {
                    osSendMesg(vc->msgq, vc->msg, 0);
                }
                D_80133280 = vc->retraceCount;
            }
            D_80138B14++;
            if (first) {
                count = osGetCount();
                D_801415B0 = count;
                first = 0;
            }
            count = D_801345C4;
            D_801345C4 = osGetCount();
            count = D_801345C4 - count;
            D_801415B0 = D_801415B0 + count;
            break;
        case 14:
            __osTimerInterrupt();
            break;
        }
    }
}
