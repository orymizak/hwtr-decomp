#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

extern f64 D_801017B0;
extern f64 D_801017B8;
extern f64 D_801017C0;
extern f64 D_801017C8;


/* Builds the synthesizer from c: buses, physical voices and the parameter pool. */
void n_alSynNew(ALSynConfig *c) {
    s32 i;
    N_PVoice *pv;
    N_PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALParam *params;
    ALParam *paramPtr;

    n_syn->head = NULL;
    n_syn->numPVoices = c->maxPVoices;
    n_syn->curSamples = 0;
    n_syn->paramSamples = 0;
    n_syn->outputRate = c->outputRate;
    n_syn->maxOutSamples = 184;
    n_syn->dma = (ALDMANew)c->dmaproc;
    n_syn->maxAuxBusses = 1;
    n_syn->outBuf = NULL;
    n_syn->auxBus = alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));
    n_syn->auxBus->sourceCount = 0;
    n_syn->auxBus->maxSources = c->maxPVoices;
    n_syn->auxBus->sources = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));
    n_syn->mainBus = alHeapAlloc(hp, 1, sizeof(N_ALMainBus));
    if (c->fxType != AL_FX_NONE) {
        n_syn->auxBus->fx = n_alSynAllocFX(0, c, hp);
        n_syn->mainBus->pull = n_alFxPull;
    } else {
        n_syn->mainBus->pull = n_alAuxBusPull;
    }
    n_syn->pFreeList.next = NULL;
    n_syn->pFreeList.prev = NULL;
    n_syn->pLameList.next = NULL;
    n_syn->pLameList.prev = NULL;
    n_syn->pAllocList.next = NULL;
    n_syn->pAllocList.prev = NULL;
    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        alLink(&pv->node, &n_syn->pFreeList);
        pv->vvoice = NULL;
        _n_initPVoice(pv, n_syn->dma, hp);
        n_syn->auxBus->sources[n_syn->auxBus->sourceCount++] = pv;
    }
    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    n_syn->paramList = NULL;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = n_syn->paramList;
        n_syn->paramList = paramPtr;
    }
    n_syn->heap = hp;
}

/* Runs the players and builds the command list for outLen samples into outBuf. */
Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    ALPlayer *client;
    Acmd *cmdlEnd = cmdList;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (n_syn->head == NULL) {
        *cmdLen = 0;
        return cmdList;
    }
    client = n_syn->head;
    while (client->samplesLeft - n_syn->curSamples < outLen) {
        n_syn->paramSamples = client->samplesLeft & ~0xf;
        client->samplesLeft += _n_timeToSamplesNoRound((*client->handler)(client));
    }
    n_syn->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = n_syn->maxOutSamples;
        if (outLen < nOut) {
            nOut = outLen;
        }
        n_syn->outBuf = lOutBuf;
        cmdlEnd = n_alSavePull(n_syn->curSamples, cmdlEnd);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        n_syn->curSamples += nOut;
    }
    *cmdLen = cmdlEnd - cmdList;
    _n_collectPVoices();
    return cmdlEnd;
}

ALParam *__n_allocParam(void) {
    ALParam *update = NULL;
    N_ALSynth *drvr = n_syn;

    if (drvr->paramList != NULL) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = NULL;
    }
    return update;
}

void __n_freeParam(ALParam *param) {
    N_ALSynth *drvr = n_syn;

    param->next = drvr->paramList;
    drvr->paramList = param;
}

/* Returns the voices that finished this frame to the free list. */
void _n_collectPVoices(void) {
    ALLink *dl;

    while ((dl = n_syn->pLameList.next) != NULL) {
        alUnlink(dl);
        alLink(dl, &n_syn->pFreeList);
    }
}

void _n_freePVoice(N_PVoice *pv) {
    alUnlink(&pv->node);
    alLink(&pv->node, &n_syn->pLameList);
}

s32 _n_timeToSamplesNoRound(s32 micros) {
    f32 tmp = (f32)micros * n_syn->outputRate / D_801017B0 + D_801017B8;

    return (s32)tmp;
}

s32 _n_timeToSamples(s32 micros) {
    f32 tmp = (f32)micros * n_syn->outputRate / D_801017C0 + D_801017C8;

    return (s32)tmp & ~0xF;
}
