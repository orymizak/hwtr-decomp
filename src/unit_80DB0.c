#include "common.h"
#include "include_asm.h"
#include "os_vi.h"

void osViSwapBuffer(void *frameBufPtr) {
    u32 saveMask;

    saveMask = __osDisableInt();
    __osViNext->framep = frameBufPtr;
    __osViNext->state |= VI_STATE_BUFFER_UPDATED;
    __osRestoreInt(saveMask);
}
