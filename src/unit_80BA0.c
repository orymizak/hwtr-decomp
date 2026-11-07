#include "common.h"
#include "include_asm.h"
typedef struct { u8 type; u8 pad[3]; u32 ctrl; } ViModeS_;
typedef struct { u16 state; u16 retraceCount; void *framep; ViModeS_ *modep; u32 control; } ViCtxS_;
extern ViCtxS_ *__osViNext;
u32 __osDisableInt(void);
void __osRestoreInt(u32);



void osViSetMode(ViModeS_ *modep) {
    register u32 saveMask = __osDisableInt();

    __osViNext->modep = modep;
    __osViNext->state = 1;
    __osViNext->control = __osViNext->modep->ctrl;
    __osRestoreInt(saveMask);
}
