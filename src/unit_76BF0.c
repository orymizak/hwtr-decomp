#include "common.h"
#include "nusys.h"
#include "ultra64.h"
#include "include_asm.h"
/* Graphics task: the frame buffer pointer lives at 0xC (as in OSScTask). */


void nuGfxSwapCfb(NUScTask *task) {
    osViSwapBuffer(task->framebuffer);
}
