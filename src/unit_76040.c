#include "common.h"
#include "n_audio.h"
#include "include_asm.h"


/* Main-bus pull, then interleave the two output channels and save them to outBuf. */
Acmd *n_alSavePull(s32 sampleOffset, Acmd *cmdList) {
    Acmd *ptr;

    ptr = n_alMainBusPull(sampleOffset, cmdList);
    n_aInterleave(ptr++);
    n_aSaveBuffer(ptr++, 0x2E0, 0, n_syn->outBuf);
    return ptr;
}
