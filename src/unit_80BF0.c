#include "common.h"
#include "include_asm.h"
typedef struct { u8 type; u8 pad[3]; u32 ctrl; } ViModeF_;
typedef struct { u16 state; u16 retraceCount; void *framep; ViModeF_ *modep; u32 control; } ViCtxF_;
extern ViCtxF_ *__osViNext;
u32 __osDisableInt(void);
void __osRestoreInt(u32);



void osViSetSpecialFeatures(u32 func) {
    register u32 saveMask = __osDisableInt();

    if ((func & 1) != 0) {
        __osViNext->control |= 8;
    }
    if ((func & 2) != 0) {
        __osViNext->control &= ~8;
    }
    if ((func & 4) != 0) {
        __osViNext->control |= 4;
    }
    if ((func & 8) != 0) {
        __osViNext->control &= ~4;
    }
    if ((func & 0x10) != 0) {
        __osViNext->control |= 0x10;
    }
    if ((func & 0x20) != 0) {
        __osViNext->control &= ~0x10;
    }
    if ((func & 0x40) != 0) {
        __osViNext->control |= 0x10000;
        __osViNext->control &= ~0x300;
    }
    if ((func & 0x80) != 0) {
        __osViNext->control &= ~0x10000;
        __osViNext->control |= __osViNext->modep->ctrl & 0x300;
    }
    __osViNext->state |= 8;
    __osRestoreInt(saveMask);
}
