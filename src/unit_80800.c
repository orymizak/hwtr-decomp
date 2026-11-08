#include "common.h"
#include "include_asm.h"
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



INCLUDE_ASM("asm/nonmatchings/unit_80800", osCreateViManager);

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
