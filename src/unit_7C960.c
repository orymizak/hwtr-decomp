#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd5_;
typedef struct Player5_ { struct Player5_ *next; void *clientData; s32 (*handler)(void *); s32 callTime; s32 samplesLeft; } Player5_;
typedef struct Filter5_ { struct Filter5_ *source; Acmd5_ *(*handler)(void *, s16 *, s32, s32, Acmd5_ *); s32 (*setParam)(void *, s32, void *); } Filter5_;
typedef struct { Player5_ *head; u32 lists[6]; s32 paramSamples; s32 curSamples; void *dma; void *heap; void *paramList; void *mainBus; void *auxBus; Filter5_ *outputFilter; s32 numPVoices; s32 maxAuxBusses; s32 outputRate; s32 maxOutSamples; } Synth5b_;
typedef struct ALLink5_ { struct ALLink5_ *next; struct ALLink5_ *prev; } ALLink5_;
typedef struct ALParam5_ { struct ALParam5_ *next; s32 delta; s16 type; s16 pad; s32 data; s32 moredata; s32 stillmoredata; s32 yetstillmoredata; } ALParam5_;
typedef struct { void *head; ALLink5_ pFreeList; ALLink5_ pAllocList; ALLink5_ pLameList; s32 paramSamples; s32 curSamples; void *dma; void *heap; ALParam5_ *paramList; void *mainBus; void *auxBus; void *outputFilter; s32 numPVoices; s32 maxAuxBusses; s32 outputRate; s32 maxOutSamples; } ALSynth5_;
typedef struct { s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses; void *dmaproc; void *heap; s32 outputRate; u8 fxType; s32 *params; } SynConfig5_;
typedef struct { ALLink5_ node; void *vvoice; void *channelKnob; u8 decoder[0x48]; u8 resampler[0x34]; u8 envmixer[0x4C]; s32 offset; } PVoice5_;
void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
void alSaveNew(void *f);
void alAuxBusNew(void *m, void *sources, s32 maxSources);
void alMainBusNew(void *m, void *sources, s32 maxSources);
void alSynAllocFX(ALSynth5_ *s, s16 bus, SynConfig5_ *c, void *hp);
s32 func_800DAC90(void *filter, s32 paramID, void *param);
void alLoadNew(void *f, void *dmaNew, void *hp);
s32 alLoadParam(void *filter, s32 paramID, void *param);
void alResampleNew(void *r, void *hp);
s32 alResampleParam(void *filter, s32 paramID, void *param);
void alEnvmixerNew(void *e, void *hp);
s32 alEnvmixerParam(void *filter, s32 paramID, void *param);
s32 alAuxBusParam(void *filter, s32 paramID, void *param);
s32 alSaveParam(void *filter, s32 paramID, void *param);
typedef struct ALParam_ { struct ALParam_ *next; } ALParam_;
typedef struct { u8 pad[0x2C]; ALParam_ *paramList; } ALSynth_;
s32 alLink(s32, void *);
s32 alUnlink(s32);

extern void *D_800FB5B0;
extern f64 D_80101A30;
extern f64 D_80101A38;
extern f64 D_80101A40;
extern f64 D_80101A48;

void alSynNew(ALSynth5_ *drvr, SynConfig5_ *c) {
    s32 i;
    PVoice5_ *pv;
    PVoice5_ *pvoices;
    void *hp = c->heap;
    void *save;
    void *sources;
    ALParam5_ *params;
    ALParam5_ *paramPtr;

    drvr->head = 0;
    drvr->numPVoices = c->maxPVoices;
    drvr->curSamples = 0;
    drvr->paramSamples = 0;
    drvr->outputRate = c->outputRate;
    drvr->maxOutSamples = 160;
    drvr->dma = c->dmaproc;
    save = alHeapDBAlloc(0, 0, hp, 1, 0x1C);
    alSaveNew(save);
    drvr->outputFilter = save;
    drvr->auxBus = alHeapDBAlloc(0, 0, hp, 1, 0x4C);
    drvr->maxAuxBusses = 1;
    sources = alHeapDBAlloc(0, 0, hp, c->maxPVoices, 4);
    alAuxBusNew(drvr->auxBus, sources, c->maxPVoices);
    drvr->mainBus = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    sources = alHeapDBAlloc(0, 0, hp, c->maxPVoices, 4);
    alMainBusNew(drvr->mainBus, sources, c->maxPVoices);
    if (c->fxType != 0) {
        alSynAllocFX(drvr, 0, c, hp);
    } else
        func_800DAC90(drvr->mainBus, 2, drvr->auxBus);
    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;
    pvoices = alHeapDBAlloc(0, 0, hp, c->maxPVoices, sizeof(PVoice5_));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        ((void (*)(ALLink5_ *ln, ALLink5_ *to))alLink)((ALLink5_ *)pv, &drvr->pFreeList);
        pv->vvoice = 0;
        alLoadNew(&pv->decoder, drvr->dma, hp);
        alLoadParam(&pv->decoder, 1, 0);
        alResampleNew(&pv->resampler, hp);
        alResampleParam(&pv->resampler, 1, &pv->decoder);
        alEnvmixerNew(&pv->envmixer, hp);
        alEnvmixerParam(&pv->envmixer, 1, &pv->resampler);
        alAuxBusParam(drvr->auxBus, 2, &pv->envmixer);
        pv->channelKnob = &pv->envmixer;
    }
    alSaveParam(save, 1, drvr->mainBus);
    params = alHeapDBAlloc(0, 0, hp, c->maxUpdates, sizeof(ALParam5_));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }
    drvr->heap = hp;
}

void _collectPVoices(ALSynth5_ *drvr);
s32 _timeToSamplesNoRound(ALSynth5_ *synth, s32 micros);
s32 __nextSampleTime(ALSynth5_ *drvr, Player5_ **client);
Acmd5_ *alAudioFrame(Acmd5_ *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    Player5_ *client;
    Filter5_ *output;
    Synth5b_ *drvr = (Synth5b_ *)D_800FB5B0;
    s16 tmp = 0;
    Acmd5_ *cmdlEnd = cmdList;
    Acmd5_ *cmdPtr;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }
    for (drvr->paramSamples = __nextSampleTime(drvr, (void **)&client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = __nextSampleTime(drvr, (void **)&client)) {
        drvr->paramSamples &= ~0xf;
        client->samplesLeft += ((s32 (*)(void *arg0, s32 arg1))_timeToSamplesNoRound)(drvr, (*client->handler)(client));
    }
    drvr->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = (drvr->maxOutSamples < outLen) ? drvr->maxOutSamples : outLen;
        cmdPtr = cmdlEnd;
        { Acmd5_ *_a = cmdPtr++; _a->w0 = 0x07000000; _a->w1 = 0; }
        output = drvr->outputFilter;
        (*output->setParam)(output, 6, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples, cmdPtr);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        drvr->curSamples += nOut;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);
    _collectPVoices(drvr);
    return cmdlEnd;
}

ALParam5_ *__allocParam(void) {
    ALParam5_ *update = NULL;
    ALSynth5_ *drvr = (ALSynth5_ *)D_800FB5B0;

    if (drvr->paramList != NULL) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = NULL;
    }
    return update;
}

void __freeParam(ALParam_ *param) {
    ALSynth_ *drvr = (*(ALSynth_ * *)&D_800FB5B0);
    param->next = drvr->paramList;
    drvr->paramList = param;
}


void _collectPVoices(ALSynth5_ *drvr) {
    ALLink5_ *dl;

    while ((dl = drvr->pLameList.next) != NULL) {
        alUnlink(dl);
        alLink(dl, &drvr->pFreeList);
    }
}

void _freePVoice(s32 arg0, s32 arg1) {
    ((s32 (*)())alUnlink)(arg1);
    ((s32 (*)(s32, s32))alLink)(arg1, arg0 + 0x14);
}

s32 _timeToSamplesNoRound(ALSynth5_ *synth, s32 micros) {
    f32 tmp = (f32)micros * synth->outputRate / D_80101A30 + D_80101A38;

    return (s32)tmp;
}

s32 _timeToSamples(ALSynth5_ *synth, s32 micros) {
    /* _timeToSamplesNoRound inlined, with its own copies of the constants. */
    f32 tmp = (f32)micros * synth->outputRate / D_80101A40 + D_80101A48;

    return (s32)tmp & ~0xF;
}

s32 __nextSampleTime(ALSynth5_ *drvr, Player5_ **client) {
    s32 delta = 0x7FFFFFFF;
    Player5_ *cl;

    *client = NULL;
    for (cl = drvr->head; cl != NULL; cl = cl->next) {
        if (cl->samplesLeft - drvr->curSamples < delta) {
            *client = cl;
            delta = cl->samplesLeft - drvr->curSamples;
        }
    }
    return (*client)->samplesLeft;
}
