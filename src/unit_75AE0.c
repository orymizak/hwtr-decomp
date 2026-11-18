#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

extern f64 D_801017B0;
extern f64 D_801017B8;
extern f64 D_801017C0;
extern f64 D_801017C8;

ALFx *func_800D5490(s32 bus, ALSynConfig *c, ALHeap *hp);
void func_800D2AAC(N_PVoice *pv, ALDMANew dmaNew, ALHeap *hp);
Acmd *func_800D3EB0(s32 sampleOffset, Acmd *p);
Acmd *func_800D2620(s32 sampleOffset, Acmd *p);
s32 func_800D5394(s32 micros);
Acmd *func_800D5440(s32 sampleOffset, Acmd *cmdList);
void func_800D52FC(void);

/* Builds the synthesizer from c (n_alSynNew): buses, physical voices and the parameter pool. */
void func_800D4EE0(ALSynConfig *c) {
    s32 i;
    N_PVoice *pv;
    N_PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALParam *params;
    ALParam *paramPtr;

    D_800FB174->head = NULL;
    D_800FB174->numPVoices = c->maxPVoices;
    D_800FB174->curSamples = 0;
    D_800FB174->paramSamples = 0;
    D_800FB174->outputRate = c->outputRate;
    D_800FB174->maxOutSamples = 184;
    D_800FB174->dma = (ALDMANew)c->dmaproc;
    D_800FB174->maxAuxBusses = 1;
    D_800FB174->outBuf = NULL;
    D_800FB174->auxBus = alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));
    D_800FB174->auxBus->sourceCount = 0;
    D_800FB174->auxBus->maxSources = c->maxPVoices;
    D_800FB174->auxBus->sources = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));
    D_800FB174->mainBus = alHeapAlloc(hp, 1, sizeof(N_ALMainBus));
    if (c->fxType != AL_FX_NONE) {
        D_800FB174->auxBus->fx = func_800D5490(0, c, hp);
        D_800FB174->mainBus->pull = func_800D3EB0;
    } else {
        D_800FB174->mainBus->pull = func_800D2620;
    }
    D_800FB174->pFreeList.next = NULL;
    D_800FB174->pFreeList.prev = NULL;
    D_800FB174->pLameList.next = NULL;
    D_800FB174->pLameList.prev = NULL;
    D_800FB174->pAllocList.next = NULL;
    D_800FB174->pAllocList.prev = NULL;
    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        alLink(&pv->node, &D_800FB174->pFreeList);
        pv->vvoice = NULL;
        func_800D2AAC(pv, D_800FB174->dma, hp);
        D_800FB174->auxBus->sources[D_800FB174->auxBus->sourceCount++] = pv;
    }
    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    D_800FB174->paramList = NULL;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = D_800FB174->paramList;
        D_800FB174->paramList = paramPtr;
    }
    D_800FB174->heap = hp;
}

/* Runs the players and builds the command list for outLen samples into outBuf (n_alAudioFrame). */
Acmd *func_800D5150(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    ALPlayer *client;
    Acmd *cmdlEnd = cmdList;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (D_800FB174->head == NULL) {
        *cmdLen = 0;
        return cmdList;
    }
    client = D_800FB174->head;
    while (client->samplesLeft - D_800FB174->curSamples < outLen) {
        D_800FB174->paramSamples = client->samplesLeft & ~0xf;
        client->samplesLeft += func_800D5394((*client->handler)(client));
    }
    D_800FB174->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = D_800FB174->maxOutSamples;
        if (outLen < nOut) {
            nOut = outLen;
        }
        D_800FB174->outBuf = lOutBuf;
        cmdlEnd = func_800D5440(D_800FB174->curSamples, cmdlEnd);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        D_800FB174->curSamples += nOut;
    }
    *cmdLen = cmdlEnd - cmdList;
    func_800D52FC();
    return cmdlEnd;
}

ALParam *func_800D52B8(void) {
    ALParam *update = NULL;
    N_ALSynth *drvr = D_800FB174;

    if (drvr->paramList != NULL) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = NULL;
    }
    return update;
}

void func_800D52E4(ALParam *param) {
    N_ALSynth *drvr = D_800FB174;

    param->next = drvr->paramList;
    drvr->paramList = param;
}

/* Returns the voices that finished this frame to the free list. */
void func_800D52FC(void) {
    ALLink *dl;

    while ((dl = D_800FB174->pLameList.next) != NULL) {
        alUnlink(dl);
        alLink(dl, &D_800FB174->pFreeList);
    }
}

void func_800D535C(N_PVoice *pv) {
    alUnlink(&pv->node);
    alLink(&pv->node, &D_800FB174->pLameList);
}

s32 func_800D5394(s32 micros) {
    f32 tmp = (f32)micros * D_800FB174->outputRate / D_801017B0 + D_801017B8;

    return (s32)tmp;
}

s32 func_800D53E4(s32 micros) {
    f32 tmp = (f32)micros * D_800FB174->outputRate / D_801017C0 + D_801017C8;

    return (s32)tmp & ~0xF;
}
