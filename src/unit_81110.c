#include "common.h"
#include "include_asm.h"
typedef struct { u16 state; u16 retraceCount; void *framep; void *modep; u32 control; } ViCtxB_;
extern ViCtxB_ *__osViNext;
u32 __osDisableInt(void);
void __osRestoreInt(u32);



void osViBlack(u8 active) {
    register u32 saveMask = __osDisableInt();

    if (active) {
        __osViNext->state |= 0x20;
    } else {
        __osViNext->state &= ~0x20;
    }
    __osRestoreInt(saveMask);
}
