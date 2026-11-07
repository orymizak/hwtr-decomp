#include "common.h"
#include "include_asm.h"
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;
typedef struct { Thread_ *mtqueue; Thread_ *fullqueue; s32 validCount; s32 first; s32 msgCount; void **msg; } MesgQueue_;
extern Thread_ *__osRunningThread;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(Thread_ **);
Thread_ *__osPopThread(Thread_ **);
void osStartThread(Thread_ *);



s32 osRecvMesg(MesgQueue_ *mq, void **msg, s32 flags) {
    register u32 saveMask;

    saveMask = __osDisableInt();
    while (mq->validCount == 0) {
        if (flags == 0) {
            __osRestoreInt(saveMask);
            return -1;
        }
        __osRunningThread->state = 8;
        __osEnqueueAndYield(&mq->mtqueue);
    }
    if (msg != NULL) {
        *msg = mq->msg[mq->first];
    }
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;
    if (mq->fullqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->fullqueue));
    }
    __osRestoreInt(saveMask);
    return 0;
}
