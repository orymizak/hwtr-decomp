#include "common.h"
#include "include_asm.h"
typedef struct { u8 type; } ViMode_;
typedef struct { u16 state; u16 retraceCount; void *framep; ViMode_ *modep; } ViCtx_;
extern ViCtx_ *__osViCurr;
u32 __osDisableInt(void);
void __osRestoreInt(u32);



u32 osViGetCurrentMode(void) {
    register u32 saveMask = __osDisableInt();
    u32 modep = __osViCurr->modep->type;

    __osRestoreInt(saveMask);
    return modep;
}
