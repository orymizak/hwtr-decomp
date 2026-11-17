#include "common.h"
#include "include_asm.h"
#include "os_vi.h"

void osViSetEvent(void *mq, void *m, u32 retraceCount) {
    register u32 saveMask;

    saveMask = __osDisableInt();
    __osViNext->msgq = mq;
    __osViNext->msg = m;
    __osViNext->retraceCount = retraceCount;
    __osRestoreInt(saveMask);
}
