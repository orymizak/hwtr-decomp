#include "common.h"
#include "include_asm.h"
extern s32 D_800FB2A0;
extern void *D_800FB2A8;



INCLUDE_ASM("asm/nonmatchings/unit_847E0", __osPiRawStartDma);

void *osPiGetCmdQueue(void) {
    if (!D_800FB2A0) {
        return NULL;
    }
    return D_800FB2A8;
}

INCLUDE_ASM("asm/nonmatchings/unit_847E0", func_800E3CD0);
