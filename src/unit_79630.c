#include "common.h"
#include "include_asm.h"
typedef struct { u8 filter[0x14]; void *state; s16 pan; s16 volume; s16 cvolL; s16 cvolR; s16 dryamt; s16 wetamt; u16 lratl; s16 lratm; s16 ltgt; u16 rratl; s16 rratm; s16 rtgt; s32 delta; s32 segEnd; s32 first; void *ctrlList; void *ctrlTail; void **sources; s32 motion; } ALEnvMixer_;
s32 alEnvmixerPull();
s32 alEnvmixerParam();
typedef struct { s16 fc; s16 fgain; s32 pad4; s16 fccoef[16]; void *fstate; s32 first; } ALLowPass_;
typedef struct { u8 filter[0x14]; void *state; void *lstate; u8 pad1C[0x14]; void *(*dma)(void *); void *dmaState; s32 pad38; s32 lastsam; s32 first; s32 memin; } ALLoad_;
s32 alAdpcmPull();
s32 alLoadParam();
typedef struct { u8 filter[0x14]; void *state; f32 ratio; s32 upitch; f32 delta; s32 first; void *ctrlList; void *ctrlTail; s32 motion; } ALResampler_;
void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
s32 alResamplePull();
s32 alResampleParam();
typedef struct { u8 filter[0x14]; s32 dramout; s32 first; } ALSave_;
s32 alSavePull();
s32 alSaveParam();
s32 alMainBusPull();
s32 func_800DAC90();
typedef struct { u8 filter[0x14]; s32 sourceCount; s32 maxSources; void **sources; } ALBus_;
void alFilterNew(void *f, void *pull, void *param, s32 type);
s32 alAuxBusPull();
s32 alAuxBusParam();



typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; ALLowPass_ *lp; ALResampler_ *rs; } ALDelay_;
typedef struct { u8 filter[0x14]; s16 *base; s16 *input; u32 length; ALDelay_ *delay; u8 section_count; void *paramHdl; } ALFx_;
typedef struct { s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses; void *dmaproc; void *heap; s32 outputRate; u8 fxType; s32 *params; } ALSynConfig_;
s32 alFxPull();
s32 alFxParam();
s32 alFxParamHdl();
extern s32 D_800FB300[];
extern s32 D_800FB368[];
extern s32 D_800FB3F0[];
extern s32 D_800FB418[];
extern s32 D_800FB440[];
extern s32 D_800FB468[];
void _init_lpfilter(ALLowPass_ *lp) {
    s32 i, temp;
    s16 fc;
    f64 ffc, fcoef;

    temp = lp->fc * 16384;
    fc = temp >> 15;
    lp->fgain = 16384 - fc;
    lp->first = 1;
    for (i = 0; i < 8; i++) {
        lp->fccoef[i] = 0;
    }
    lp->fccoef[i++] = fc;
    fcoef = ffc = (f64)fc / 16384.0;
    for (; i < 16; i++) {
        fcoef *= ffc;
        lp->fccoef[i] = (s16)(fcoef * 16384.0);
    }
}

void alFxNew(ALFx_ *r, ALSynConfig_ *c, void *hp) {
    u16 i, j, k;
    s32 *param = 0;
    ALDelay_ *d;

    alFilterNew(r, 0, alFxParam, 5);
    *(void **)((u8 *)r + 4) = alFxPull;
    r->paramHdl = alFxParamHdl;
    switch (c->fxType) {
        case 1: param = D_800FB300; break;
        case 2: param = D_800FB368; break;
        case 5: param = D_800FB3F0; break;
        case 3: param = D_800FB418; break;
        case 4: param = D_800FB440; break;
        case 6: param = c->params; break;
        default: param = D_800FB468; break;
    }
    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = alHeapDBAlloc(0, 0, hp, r->section_count, sizeof(ALDelay_));
    r->base = alHeapDBAlloc(0, 0, hp, r->length, sizeof(s16));
    r->input = r->base;
    for (k = 0; k < r->length; k++) {
        r->base[k] = 0;
    }
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];
        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * 2.0) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / 173123.404906676) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = alHeapDBAlloc(0, 0, hp, 1, sizeof(ALResampler_));
            d->rs->state = alHeapDBAlloc(0, 0, hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }
        if (param[j]) {
            d->lp = alHeapDBAlloc(0, 0, hp, 1, sizeof(ALLowPass_));
            d->lp->fstate = alHeapDBAlloc(0, 0, hp, 1, 8);
            d->lp->fc = param[j++];
            _init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void alEnvmixerNew(ALEnvMixer_ *e, void *hp) {
    alFilterNew(e, alEnvmixerPull, alEnvmixerParam, 4);
    e->state = alHeapDBAlloc(0, 0, hp, 1, 0x50);
    e->first = 1;
    e->motion = 0;
    e->volume = 1;
    e->ltgt = 1;
    e->rtgt = 1;
    e->cvolL = 1;
    e->cvolR = 1;
    e->dryamt = 0;
    e->wetamt = 0;
    e->lratm = 1;
    e->lratl = 0;
    e->lratm = 1;
    e->lratl = 0;
    e->delta = 0;
    e->segEnd = 0;
    e->pan = 0;
    e->ctrlList = 0;
    e->ctrlTail = 0;
    e->sources = 0;
}


void alLoadNew(ALLoad_ *f, void *(*dmaNew)(void **), void *hp) {
    alFilterNew(f, alAdpcmPull, alLoadParam, 0);
    f->state = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    f->lstate = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    f->dma = dmaNew(&f->dmaState);
    f->lastsam = 0;
    f->first = 1;
    f->memin = 0;
}


void alResampleNew(ALResampler_ *r, void *hp) {
    alFilterNew(r, alResamplePull, alResampleParam, 1);
    r->state = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    r->delta = 0.0;
    r->first = 1;
    r->motion = 0;
    r->ratio = 1.0;
    r->upitch = 0;
    r->ctrlList = 0;
    r->ctrlTail = 0;
}


void alAuxBusNew(ALBus_ *m, void *sources, s32 maxSources) {
    alFilterNew(m, alAuxBusPull, alAuxBusParam, 6);
    m->sourceCount = 0;
    m->maxSources = maxSources;
    m->sources = sources;
}


void alMainBusNew(ALBus_ *m, void *sources, s32 maxSources) {
    alFilterNew(m, alMainBusPull, func_800DAC90, 7);
    m->sourceCount = 0;
    m->maxSources = maxSources;
    m->sources = sources;
}


void alSaveNew(ALSave_ *f) {
    alFilterNew(f, alSavePull, alSaveParam, 3);
    f->dramout = 0;
    f->first = 1;
}

