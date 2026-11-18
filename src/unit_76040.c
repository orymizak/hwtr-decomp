#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

Acmd *func_800D54E0(s32 sampleOffset, Acmd *cmdList);

/* Main-bus pull, then interleave the two output channels and save them to outBuf. */
Acmd *func_800D5440(s32 sampleOffset, Acmd *cmdList) {
    Acmd *ptr;

    ptr = func_800D54E0(sampleOffset, cmdList);
    n_aInterleave(ptr++);
    n_aSaveBuffer(ptr++, 0x2E0, 0, D_800FB174->outBuf);
    return ptr;
}
