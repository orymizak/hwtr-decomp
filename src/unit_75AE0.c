#include "common.h"
#include "include_asm.h"
typedef struct LinkM_ { struct LinkM_ *next; struct LinkM_ *prev; } LinkM_;
typedef struct ParamM_ { struct ParamM_ *next; u8 p4[0x18]; } ParamM_;
typedef struct { LinkM_ node; void *vvoice; u8 pC[0x80]; } PVoiceM_;
typedef struct { u8 p0[0x14]; s32 sourceCount; s32 maxSources; PVoiceM_ **sources; void *fx; u8 p24[0x20]; } AuxBusM_;
typedef struct { void *source; void *handler; u8 p8[0xC]; } MainBusM_;
typedef struct { void *head; LinkM_ pFreeList; LinkM_ pAllocList; LinkM_ pLameList; s32 paramSamples; s32 curSamples; void *dma; void *heap; ParamM_ *paramList; MainBusM_ *mainBus; AuxBusM_ *auxBus; s32 numPVoices; s32 pad3C; s32 outputRate; s32 maxOutSamples; s32 unk48; s32 maxAuxBusses; } SynthM_;
typedef struct { s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses; void *dmaproc; void *heap; s32 outputRate; u8 fxType; } SynConfigM_;
typedef struct { u32 w0; u32 w1; } AcmdL_;
typedef struct PlayerL_ { struct PlayerL_ *next; void *clientData; s32 (*handler)(void *); s32 callTime; s32 samplesLeft; } PlayerL_;
typedef struct { PlayerL_ *head; u8 p4[0x18]; s32 paramSamples; s32 curSamples; u8 p24[0x20]; s32 maxOutSamples; s16 *outBuf; } SynthL_;
typedef struct ALParamB_ { struct ALParamB_ *next; } ALParamB_;
typedef struct { u8 pad[0x2C]; ALParamB_ *paramList; } SynthB_;
s32 alLink(s32, void *);
s32 alUnlink(s32);

extern void *D_800FB174;
extern f64 D_801017B0;
extern f64 D_801017B8;
extern f64 D_801017C0;
extern f64 D_801017C8;

void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
void *func_800D5490(s32 a, SynConfigM_ *c, void *hp);
void func_800D2AAC(PVoiceM_ *pv, void *dma, void *hp);
void func_800D3EB0();
void func_800D2620();
void func_800D4EE0(SynConfigM_ *c) {
    s32 i;
    PVoiceM_ *pv;
    PVoiceM_ *pvoices;
    void *hp = c->heap;
    ParamM_ *params;
    ParamM_ *paramPtr;

    ((SynthM_ *)D_800FB174)->head = 0;
    ((SynthM_ *)D_800FB174)->numPVoices = c->maxPVoices;
    ((SynthM_ *)D_800FB174)->curSamples = 0;
    ((SynthM_ *)D_800FB174)->paramSamples = 0;
    ((SynthM_ *)D_800FB174)->outputRate = c->outputRate;
    ((SynthM_ *)D_800FB174)->maxOutSamples = 184;
    ((SynthM_ *)D_800FB174)->dma = c->dmaproc;
    ((SynthM_ *)D_800FB174)->maxAuxBusses = 1;
    ((SynthM_ *)D_800FB174)->unk48 = 0;
    ((SynthM_ *)D_800FB174)->auxBus = alHeapDBAlloc(0, 0, hp, 1, sizeof(AuxBusM_));
    ((SynthM_ *)D_800FB174)->auxBus->sourceCount = 0;
    ((SynthM_ *)D_800FB174)->auxBus->maxSources = c->maxPVoices;
    ((SynthM_ *)D_800FB174)->auxBus->sources = alHeapDBAlloc(0, 0, hp, c->maxPVoices, 4);
    ((SynthM_ *)D_800FB174)->mainBus = alHeapDBAlloc(0, 0, hp, 1, sizeof(MainBusM_));
    if (c->fxType != 0) {
        ((SynthM_ *)D_800FB174)->auxBus->fx = func_800D5490(0, c, hp);
        ((SynthM_ *)D_800FB174)->mainBus->handler = func_800D3EB0;
    } else {
        ((SynthM_ *)D_800FB174)->mainBus->handler = func_800D2620;
    }
    ((SynthM_ *)D_800FB174)->pFreeList.next = 0;
    ((SynthM_ *)D_800FB174)->pFreeList.prev = 0;
    ((SynthM_ *)D_800FB174)->pLameList.next = 0;
    ((SynthM_ *)D_800FB174)->pLameList.prev = 0;
    ((SynthM_ *)D_800FB174)->pAllocList.next = 0;
    ((SynthM_ *)D_800FB174)->pAllocList.prev = 0;
    pvoices = alHeapDBAlloc(0, 0, hp, c->maxPVoices, sizeof(PVoiceM_));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        alLink(&pv->node, &((SynthM_ *)D_800FB174)->pFreeList);
        pv->vvoice = 0;
        func_800D2AAC(pv, ((SynthM_ *)D_800FB174)->dma, hp);
        ((SynthM_ *)D_800FB174)->auxBus->sources[((SynthM_ *)D_800FB174)->auxBus->sourceCount++] = pv;
    }
    params = alHeapDBAlloc(0, 0, hp, c->maxUpdates, sizeof(ParamM_));
    ((SynthM_ *)D_800FB174)->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = ((SynthM_ *)D_800FB174)->paramList;
        ((SynthM_ *)D_800FB174)->paramList = paramPtr;
    }
    ((SynthM_ *)D_800FB174)->heap = hp;
}

s32 func_800D5394(s32 t);
AcmdL_ *func_800D5440(s32 samples, AcmdL_ *p);
void func_800D52FC(void);
AcmdL_ *func_800D5150(AcmdL_ *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    PlayerL_ *client;
    AcmdL_ *cmdlEnd = cmdList;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (((SynthL_ *)D_800FB174)->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }
    client = ((SynthL_ *)D_800FB174)->head;
    while (client->samplesLeft - ((SynthL_ *)D_800FB174)->curSamples < outLen) {
        ((SynthL_ *)D_800FB174)->paramSamples = client->samplesLeft & ~0xf;
        client->samplesLeft += func_800D5394((*client->handler)(client));
    }
    ((SynthL_ *)D_800FB174)->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = ((SynthL_ *)D_800FB174)->maxOutSamples;
        if (outLen < nOut) {
            nOut = outLen;
        }
        ((SynthL_ *)D_800FB174)->outBuf = lOutBuf;
        cmdlEnd = func_800D5440(((SynthL_ *)D_800FB174)->curSamples, cmdlEnd);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        ((SynthL_ *)D_800FB174)->curSamples += nOut;
    }
    *cmdLen = cmdlEnd - cmdList;
    func_800D52FC();
    return cmdlEnd;
}

ParamM_ *func_800D52B8(void) {
    ParamM_ *update = NULL;
    SynthM_ *drvr = (SynthM_ *)D_800FB174;

    if (drvr->paramList != NULL) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = NULL;
    }
    return update;
}

void func_800D52E4(ALParamB_ *param) {
    SynthB_ *drvr = (*(SynthB_ * *)&D_800FB174);
    param->next = drvr->paramList;
    drvr->paramList = param;
}

void func_800D52FC(void) {
    LinkM_ *dl;

    while ((dl = ((SynthM_ *)D_800FB174)->pLameList.next) != NULL) {
        alUnlink((s32)dl);
        alLink((s32)dl, &((SynthM_ *)D_800FB174)->pFreeList);
    }
}

void func_800D535C(s32 arg0) {
    ((s32 (*)())alUnlink)();
    ((s32 (*)(s32, s32))alLink)(arg0, (*(s32 *)&D_800FB174) + 0x14);
}

s32 func_800D5394(s32 micros) {
    f32 tmp = (f32)micros * ((SynthM_ *)D_800FB174)->outputRate / D_801017B0 + D_801017B8;

    return (s32)tmp;
}

s32 func_800D53E4(s32 micros) {
    f32 tmp = (f32)micros * ((SynthM_ *)D_800FB174)->outputRate / D_801017C0 + D_801017C8;

    return (s32)tmp & ~0xF;
}
