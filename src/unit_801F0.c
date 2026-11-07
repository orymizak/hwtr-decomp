#include "common.h"
#include "include_asm.h"
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;
extern Thread_ *__osRunningThread;
extern Thread_ *__osRunQueue;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(Thread_ **);



void osYieldThread(void) {
    register u32 saveMask = __osDisableInt();

    __osRunningThread->state = 2;
    __osEnqueueAndYield(&__osRunQueue);
    __osRestoreInt(saveMask);
}
