#include "common.h"
#include "include_asm.h"
typedef struct { void *next; void *prev; u64 interval; u64 value; void *mq; void *msg; } Timer_;
extern Timer_ **D_800FB620;
u64 __osInsertTimer(Timer_ *);
void __osSetTimerIntr(u64);



s32 osSetTimer(Timer_ *t, u64 value, u64 interval, void *mq, void *msg) {
    u64 time;

    t->next = NULL;
    t->prev = NULL;
    t->value = value;
    t->interval = interval;
    if (value == 0) {
        t->value = interval;
    }
    t->mq = mq;
    t->msg = msg;
    time = __osInsertTimer(t);
    if (*D_800FB620 == t) {
        __osSetTimerIntr(time);
    }
    return 0;
}
