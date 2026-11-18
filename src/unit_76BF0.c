#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
/* Graphics task: the frame buffer pointer lives at 0xC (as in OSScTask). */
typedef struct { u8 p0[0xC]; s32 framebuffer; } GfxTask7_;


void func_800D5FF0(GfxTask7_ *task) {
    osViSwapBuffer(task->framebuffer);
}
