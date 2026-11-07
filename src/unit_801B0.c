#include "common.h"
#include "include_asm.h"
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;



void __osDequeueThread(Thread_ **queue, Thread_ *t) {
    register Thread_ *pred;
    register Thread_ *succ;

    pred = (Thread_ *)queue;
    succ = pred->next;
    while (succ != NULL) {
        if (succ == t) {
            pred->next = t->next;
            return;
        }
        pred = succ;
        succ = pred->next;
    }
}
