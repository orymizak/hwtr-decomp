#include "common.h"
#include "include_asm.h"
typedef struct { void *mtqueue; void *fullqueue; s32 validCount; s32 first; s32 msgCount; void *msg; } OSMesgQueue_;

extern s32 __osThreadTail;

void osCreateMesgQueue(OSMesgQueue_ *mq, void *msg, s32 msgCount) {
    mq->mtqueue = &__osThreadTail;
    mq->fullqueue = &__osThreadTail;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = msgCount;
    mq->msg = msg;
}
