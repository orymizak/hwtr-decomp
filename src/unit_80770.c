#include "common.h"
#include "include_asm.h"
typedef struct { u16 state; u16 retraceCount; void *framep; void *modep; } ViCtx_;
extern ViCtx_ *__osViNext;
u32 __osDisableInt(void);
void __osRestoreInt(u32);



void *osViGetNextFramebuffer(void) {
    register u32 saveMask = __osDisableInt();
    void *framep = __osViNext->framep;

    __osRestoreInt(saveMask);
    return framep;
}
