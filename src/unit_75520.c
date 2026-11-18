#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

/* Adds client to the synthesizer's player list. */
void func_800D4920(ALPlayer *client) {
    OSIntMask mask = osSetIntMask(OS_IM_NONE);
    N_ALSynth *drvr = D_800FB174;

    client->samplesLeft = drvr->curSamples;
    client->next = drvr->head;
    drvr->head = client;
    osSetIntMask(mask);
}
