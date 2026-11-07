#include "common.h"
#include "include_asm.h"
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;
extern Thread_ *__osRunningThread;
extern Thread_ *__osRunQueue;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(Thread_ **);
void __osDequeueThread(Thread_ **, Thread_ *);
void __osEnqueueThread(Thread_ **, Thread_ *);



void osSetThreadPri(Thread_ *t, s32 pri) {
    register u32 saveMask = __osDisableInt();

    if (t == NULL) {
        t = __osRunningThread;
    }
    if (t->priority != pri) {
        t->priority = pri;
        if (t != __osRunningThread && t->state != 1) {
            __osDequeueThread(t->queue, t);
            __osEnqueueThread(t->queue, t);
        }
        if (__osRunningThread->priority < __osRunQueue->priority) {
            __osRunningThread->state = 2;
            __osEnqueueAndYield(&__osRunQueue);
        }
    }
    __osRestoreInt(saveMask);
}
