#include "common.h"
#include "include_asm.h"
typedef struct TimerI_ { struct TimerI_ *next; struct TimerI_ *prev; u64 interval; u64 remaining; void *mq; void *msg; } TimerI_;
extern u32 __osTimerCounter;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
u32 osGetCount(void);
void __osSetCompare(u32);
typedef struct Timer_ { struct Timer_ *next; struct Timer_ *prev; u64 interval; u64 remaining; void *mq; void *msg; } Timer_;
extern u64 __osCurrentTime;
extern u32 __osBaseCounter;
extern u32 __osViIntrCount;
extern Timer_ *__osTimerList;
s32 osSendMesg(void *, void *, s32);
void __osSetTimerIntr(u64 tim);
u64 __osInsertTimer(TimerI_ *t);



void __osTimerServicesInit(void) {
    __osCurrentTime = 0;
    __osBaseCounter = 0;
    __osViIntrCount = 0;
    __osTimerList->prev = __osTimerList;
    __osTimerList->next = __osTimerList->prev;
    __osTimerList->remaining = 0;
    __osTimerList->interval = __osTimerList->remaining;
    __osTimerList->mq = NULL;
    __osTimerList->msg = 0;
}

void __osTimerInterrupt(void) {
    Timer_ *t;
    u32 count;
    u32 elapsed_cycles;

    if (__osTimerList->next == __osTimerList) {
        return;
    }
    while (1) {
        t = __osTimerList->next;
        if (t == __osTimerList) {
            __osSetCompare(0);
            __osTimerCounter = 0;
            break;
        }
        count = osGetCount();
        elapsed_cycles = count - __osTimerCounter;
        __osTimerCounter = count;
        if (elapsed_cycles < t->remaining) {
            t->remaining -= elapsed_cycles;
            __osSetTimerIntr(t->remaining);
            return;
        } else {
            t->prev->next = t->next;
            t->next->prev = t->prev;
            t->next = NULL;
            t->prev = NULL;
            if (t->mq != NULL) {
                osSendMesg(t->mq, t->msg, 0);
            }
            if (t->interval != 0) {
                t->remaining = t->interval;
                __osInsertTimer((TimerI_ *)t);
            }
        }
    }
}


void __osSetTimerIntr(u64 tim) {
    u64 NewTime;
    u32 savedMask;

    savedMask = __osDisableInt();
    __osTimerCounter = osGetCount();
    NewTime = __osTimerCounter + tim;
    __osSetCompare(NewTime);
    __osRestoreInt(savedMask);
}

u64 __osInsertTimer(TimerI_ *t) {
    TimerI_ *timep;
    u64 tim;
    u32 savedMask;

    savedMask = __osDisableInt();
    for (timep = (*(TimerI_ * *)&__osTimerList)->next, tim = t->remaining; timep != (*(TimerI_ * *)&__osTimerList) && tim > timep->remaining;
         tim -= timep->remaining, timep = timep->next) {
    }
    t->remaining = tim;
    if (timep != (*(TimerI_ * *)&__osTimerList)) {
        timep->remaining -= tim;
    }
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    __osRestoreInt(savedMask);
    return tim;
}
