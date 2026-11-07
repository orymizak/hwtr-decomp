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



s32 osJamMesg(MesgQueue_ *mq, void *msg, s32 flags) {
    register u32 saveMask;

    saveMask = __osDisableInt();
    while (mq->validCount >= mq->msgCount) {
        if (flags == 1) {
            __osRunningThread->state = 8;
            __osEnqueueAndYield(&mq->fullqueue);
        } else {
            __osRestoreInt(saveMask);
            return -1;
        }
    }
    mq->first = (mq->first + mq->msgCount - 1) % mq->msgCount;
    mq->msg[mq->first] = msg;
    mq->validCount++;
    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }
    __osRestoreInt(saveMask);
    return 0;
}
