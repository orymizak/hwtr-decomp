#include "common.h"
#include "include_asm.h"
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;
extern Thread_ *__osRunningThread;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(Thread_ **);
void __osDequeueThread(Thread_ **, Thread_ *);



void osStopThread(Thread_ *t) {
    register u32 saveMask = __osDisableInt();
    register u16 state;

    state = (t == NULL) ? 4 : t->state;
    switch (state) {
    case 4:
        __osRunningThread->state = 1;
        __osEnqueueAndYield(NULL);
        break;
    case 2:
    case 8:
        t->state = 1;
        __osDequeueThread(t->queue, t);
        break;
    }
    __osRestoreInt(saveMask);
}
