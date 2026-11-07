#include "common.h"
#include "include_asm.h"
typedef struct TimerI_ { struct TimerI_ *next; struct TimerI_ *prev; u64 interval; u64 remaining; void *mq; void *msg; } TimerI_;
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

u64 __osInsertTimer(TimerI_ *t) {
    TimerI_ *timep;
    u64 tim;
    u32 savedMask;

    savedMask = __osDisableInt();
    for (timep = (*(TimerI_ * *)&D_800FB620)->next, tim = t->remaining; timep != (*(TimerI_ * *)&D_800FB620) && tim > timep->remaining;
         tim -= timep->remaining, timep = timep->next) {
    }
    t->remaining = tim;
    if (timep != (*(TimerI_ * *)&D_800FB620)) {
        timep->remaining -= tim;
    }
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    __osRestoreInt(savedMask);
    return tim;
}
