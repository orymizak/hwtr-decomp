#include "common.h"
#include "include_asm.h"
extern u32 D_80145560;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
u32 osGetCount(void);
void __osSetCompare(u32);
typedef struct Timer_ { struct Timer_ *next; struct Timer_ *prev; u64 interval; u64 remaining; void *mq; void *msg; } Timer_;
extern u64 D_801415B0;
extern u32 D_801345C4;
extern u32 D_80138B14;
extern Timer_ *D_800FB620;



void __osTimerServicesInit(void) {
    D_801415B0 = 0;
    D_801345C4 = 0;
    D_80138B14 = 0;
    D_800FB620->prev = D_800FB620;
    D_800FB620->next = D_800FB620->prev;
    D_800FB620->remaining = 0;
    D_800FB620->interval = D_800FB620->remaining;
    D_800FB620->mq = NULL;
    D_800FB620->msg = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_80370", __osTimerInterrupt);

void __osSetTimerIntr(u64 tim) {
    u64 NewTime;
    u32 savedMask;

    savedMask = __osDisableInt();
    D_80145560 = osGetCount();
    NewTime = D_80145560 + tim;
    __osSetCompare(NewTime);
    __osRestoreInt(savedMask);
}

INCLUDE_ASM("asm/nonmatchings/unit_80370", __osInsertTimer);
