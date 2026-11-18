#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

s32 func_800D26C0(s32);

s32 func_800D5490(s32 arg0) {
    s32 i = (s16)arg0;

    func_800D26C0(&D_800FB174->auxBus->unk24[i]);
    return D_800FB174->auxBus->unk24[i];
}

/* Clears the main-bus buffer, runs the main-bus filter and mixes the result into both outputs. */
Acmd *func_800D54E0(s32 sampleOffset, Acmd *cmdList) {
    Acmd *ptr = cmdList;

    aClearBuffer(ptr++, 0x4E0, 0x2E0);
    ptr = D_800FB174->mainBus->pull(sampleOffset, ptr);
    aMix(ptr++, 0, 0x7FFF, 0x7C0, 0x4E0);
    aMix(ptr++, 0, 0x7FFF, 0x930, 0x650);
    return ptr;
}
