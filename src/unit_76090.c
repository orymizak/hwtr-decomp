#include "common.h"
#include "n_audio.h"
#include "include_asm.h"


ALFx *n_alSynAllocFX(s16 bus, ALSynConfig *c, ALHeap *hp) {
    n_alFxNew(&n_syn->auxBus->fxSlots[bus], c, hp);
    return n_syn->auxBus->fxSlots[bus];
}

/* Clears the main-bus buffer, runs the main-bus filter and mixes the result into both outputs. */
Acmd *n_alMainBusPull(s32 sampleOffset, Acmd *cmdList) {
    Acmd *ptr = cmdList;

    aClearBuffer(ptr++, 0x4E0, 0x2E0);
    ptr = n_syn->mainBus->pull(sampleOffset, ptr);
    aMix(ptr++, 0, 0x7FFF, 0x7C0, 0x4E0);
    aMix(ptr++, 0, 0x7FFF, 0x930, 0x650);
    return ptr;
}
