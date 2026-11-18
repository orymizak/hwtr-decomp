#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

/* Adds client to the synthesizer's player list. */
void n_alSynAddPlayer(ALPlayer *client) {
    OSIntMask mask = osSetIntMask(OS_IM_NONE);
    N_ALSynth *drvr = n_syn;

    client->samplesLeft = drvr->curSamples;
    client->next = drvr->head;
    drvr->head = client;
    osSetIntMask(mask);
}
