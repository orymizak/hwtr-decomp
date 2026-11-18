#include "common.h"
#include "include_asm.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/osint.h"
#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/osint.h"

extern __OSEventState __osEventStateTab[OS_NUM_EVENTS];


extern u32 __osPreNMI;



s32 osSendMesg(OSMesgQueue* mq, OSMesg msg, s32 flags) {
    register u32 saveMask;
    register s32 last;


    saveMask = __osDisableInt();

    while (MQ_IS_FULL(mq)) {
        if (flags == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
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



void osSetEventMesg(OSEvent event, OSMesgQueue* mq, OSMesg msg) {
    register u32 saveMask;
    __OSEventState* es;


    saveMask = __osDisableInt();

    es = &__osEventStateTab[event];

    es->messageQueue = mq;
    es->message = msg;

    if (event == OS_EVENT_PRENMI) {
        if (__osShutdown && !__osPreNMI) {
            osSendMesg(mq, msg, OS_MESG_NOBLOCK);
        }
        __osPreNMI = TRUE;
    }

    __osRestoreInt(saveMask);
}
