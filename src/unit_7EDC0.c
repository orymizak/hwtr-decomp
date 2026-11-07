#include "common.h"
#include "include_asm.h"
typedef struct { void *messageQueue; void *message; } EventState_;
extern EventState_ __osEventStateTab[];
extern u32 D_800FB5D0;
extern u32 D_800FC7FC;
typedef struct Thread_ { struct Thread_ *next; s32 priority; struct Thread_ **queue; struct Thread_ *tlnext; u16 state; u16 flags; } Thread_;
typedef struct { Thread_ *mtqueue; Thread_ *fullqueue; s32 validCount; s32 first; s32 msgCount; void **msg; } MesgQueue_;
extern Thread_ *__osRunningThread;
u32 __osDisableInt(void);
void __osRestoreInt(u32);
void __osEnqueueAndYield(Thread_ **);
Thread_ *__osPopThread(Thread_ **);
void osStartThread(Thread_ *);



s32 osSendMesg(MesgQueue_ *mq, void *msg, s32 flags) {
    register u32 saveMask;
    register s32 last;

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
    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;
    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }
    __osRestoreInt(saveMask);
    return 0;
}

void osSetEventMesg(s32 event, void *mq, void *msg) {
    register u32 saveMask = __osDisableInt();
    EventState_ *es;

    es = &__osEventStateTab[event];
    es->messageQueue = mq;
    es->message = msg;
    if (event == 14) {
        if (D_800FC7FC && !D_800FB5D0) {
            ((s32 (*)(void *, void *, s32))osSendMesg)(mq, msg, 0);
        }
        D_800FB5D0 = 1;
    }
    __osRestoreInt(saveMask);
}
