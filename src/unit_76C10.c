#include "common.h"
#include "include_asm.h"
extern volatile s32 nuGfxTaskSpool;



void nuGfxTaskAllEndWait(void) {
    while (nuGfxTaskSpool != 0) {
    }
}

