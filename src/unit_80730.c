#include "common.h"
#include "include_asm.h"
typedef struct { u16 state; u16 retraceCount; void *framep; void *modep; } ViCtx_;
extern ViCtx_ *__osViCurr;
u32 __osDisableInt(void);
void __osRestoreInt(u32);



void *osViGetCurrentFramebuffer(void) {
    register u32 saveMask = __osDisableInt();
    void *framep = __osViCurr->framep;

    __osRestoreInt(saveMask);
    return framep;
}
