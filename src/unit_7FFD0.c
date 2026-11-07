#include "common.h"
#include "include_asm.h"
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;
extern Thread_ *__osRunningThread;
extern Thread_ *__osRunQueue;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(Thread_ **);
void __osEnqueueThread(Thread_ **, Thread_ *);
Thread_ *__osPopThread(Thread_ **);
void __osDispatchThread(void);



void osStartThread(Thread_ *t) {
    register u32 saveMask = __osDisableInt();

    switch (t->state) {
    case 8:
        t->state = 2;
        __osEnqueueThread(&__osRunQueue, t);
        break;
    case 1:
        if (t->queue == NULL || t->queue == &__osRunQueue) {
            t->state = 2;
            __osEnqueueThread(&__osRunQueue, t);
        } else {
            t->state = 8;
            __osEnqueueThread(t->queue, t);
            __osEnqueueThread(&__osRunQueue, __osPopThread(t->queue));
        }
        break;
    }
    if (__osRunningThread == NULL) {
        __osDispatchThread();
    } else if (__osRunningThread->priority < __osRunQueue->priority) {
        __osRunningThread->state = 2;
        __osEnqueueAndYield(&__osRunQueue);
    }
    __osRestoreInt(saveMask);
}
