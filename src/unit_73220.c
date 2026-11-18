#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

Acmd *func_800D2BE0(void *filter, s32 sampleOffset, Acmd *p);
Acmd *func_800D31FC(void *filter, s16 *inp, s16 *outp, s32 outCount, Acmd *p);
Acmd *func_800D3CD0(N_PVoice *f, s16 *outp, Acmd *p);
void func_800D39F8(N_PVoice *a, s32 paramID, void *param);
void func_800D535C(N_PVoice *pv);
void func_800D52E4(ALParam *p);
s16 func_800D341C(f64 vol, f64 tgt, s32 count, u16 *ratel);
s32 func_800D355C(s32 ivol, s32 samples, s16 ratem, s32 ratel);
extern s32 D_800FAED0[]; /* reverb settings: small room */
extern s32 D_800FAF38[]; /* big room */
extern s32 D_800FAFC0[]; /* echo */
extern s32 D_800FAFE8[]; /* chorus */
extern s32 D_800FB010[]; /* flange */
extern s32 D_800FB038[]; /* none */
extern s16 D_800FB060[]; /* equal-power pan/mix table */


Acmd *func_800D2620(s32 a, Acmd *p) {
    N_ALAuxBus *m = D_800FB174->auxBus;
    N_PVoice **sources = m->sources;
    s32 i;

    aClearBuffer(p++, 0x7C0, 0x2E0);
    for (i = 0; i < m->sourceCount; i++) {
        p = func_800D2BE0(sources[i], a, p);
    }
    return p;
}


void func_800D26C0(ALFx **rp, ALSynConfig *c, void *hp) {
    u16 i, j, k;
    s32 *param = 0;
    ALDelay *d;
    ALFx *r;

    r = (ALFx *)alHeapDBAlloc(0, 0, hp, 1, sizeof(ALFx));
    *rp = r;
    switch (c->fxType) {
        case AL_FX_SMALLROOM: param = D_800FAED0; break;
        case AL_FX_BIGROOM: param = D_800FAF38; break;
        case AL_FX_ECHO: param = D_800FAFC0; break;
        case AL_FX_CHORUS: param = D_800FAFE8; break;
        case AL_FX_FLANGE: param = D_800FB010; break;
        case AL_FX_CUSTOM: param = c->params; break;
        default: param = D_800FB038; break;
    }
    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = (ALDelay *)alHeapDBAlloc(0, 0, hp, r->section_count, sizeof(ALDelay));
    r->base = (s16 *)alHeapDBAlloc(0, 0, hp, r->length, sizeof(s16));
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
            d->rs = (ALResampler *)alHeapDBAlloc(0, 0, hp, 1, sizeof(ALResampler));
            d->rs->state = (void *)alHeapDBAlloc(0, 0, hp, 1, 0x20);
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }
        if (param[j]) {
            d->lp = (ALLowPass *)alHeapDBAlloc(0, 0, hp, 1, sizeof(ALLowPass));
            d->lp->fstate = (void *)alHeapDBAlloc(0, 0, hp, 1, 8);
            d->lp->fc = param[j++];
            _init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void func_800D2AAC(N_PVoice *pv, s32 (*dmaNew)(s32 *), void *hp) {
    /* load filter */
    pv->dc_state = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    pv->dc_lstate = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    pv->dc_dma = dmaNew(&pv->dc_dmaState);
    pv->dc_lastsam = 0;
    pv->dc_first = 1;
    pv->dc_memin = 0;
    /* resampler */
    pv->rs_state = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    pv->rs_delta = 0;
    pv->rs_first = 1;
    pv->rs_upitch = 0;
    pv->rs_ratio = 1.0f;
    /* envelope mixer */
    pv->em_state = alHeapDBAlloc(0, 0, hp, 1, 0x50);
    pv->em_first = 1;
    pv->em_motion = 0;
    pv->em_volume = 1;
    pv->em_ltgt = 1;
    pv->em_rtgt = 1;
    pv->em_cvolL = 1;
    pv->em_cvolR = 1;
    pv->em_dryamt = 0;
    pv->em_wetamt = 0;
    pv->em_lratm = 1;
    pv->em_lratl = 0;
    /* Repeated in the original code. */
    pv->em_lratm = 1;
    pv->em_lratl = 0;
    pv->em_delta = 0;
    pv->em_segEnd = 0;
    pv->em_pan = 0;
    pv->em_ctrlList = 0;
    pv->em_ctrlTail = 0;
}

Acmd *func_800D2BE0(void *filter, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    N_PVoice *e = (N_PVoice *)filter;
    s16 inp;
    s32 lastOffset;
    s32 thisOffset = sampleOffset;
    s32 samples;
    s16 loutp = 0;
    s32 fVol;
    s32 outCount = 184;
    ALParam *thisParam;

    inp = 0;

    while (e->em_ctrlList != 0) {
        lastOffset = thisOffset;
        thisOffset = e->em_ctrlList->delta;
        samples = ((thisOffset - lastOffset + 92) / 184) * 184;
        if (samples == 0)
            thisOffset = lastOffset;
        if (samples > outCount)
            break;

        switch (e->em_ctrlList->type) {
        case AL_FILTER_START_VOICE_ALT: {
            ALStartParamAlt *param = (ALStartParamAlt *)e->em_ctrlList;
            s32 tmp;

            if (param->unity) {
                e->rs_upitch = 1;
            }
            func_800D39F8(e, AL_FILTER_SET_WAVETABLE, param->wave);
            e->em_motion = 1;
            e->em_first = 1;
            e->em_delta = 0;
            e->em_segEnd = ((param->samples + 92) / 184) * 184;
            tmp = ((s32)param->volume * (s32)param->volume) >> 15;
            e->em_volume = (s16)tmp;
            e->em_pan = param->pan;
            e->em_dryamt = D_800FB060[param->fxMix];
            e->em_wetamt = D_800FB060[128 - param->fxMix - 1];
            if (param->samples) {
                e->em_cvolL = 1;
                e->em_cvolR = 1;
            } else {
                e->em_cvolL = (e->em_volume * D_800FB060[e->em_pan]) >> 15;
                e->em_cvolR = (e->em_volume * D_800FB060[128 - e->em_pan - 1]) >> 15;
            }
            e->rs_ratio = param->pitch;
        } break;

        case AL_FILTER_SET_FXAMT:
        case AL_FILTER_SET_PAN:
        case AL_FILTER_SET_VOLUME:
            ptr = func_800D31FC(e, &inp, &loutp, samples, ptr);
            if (e->em_delta >= e->em_segEnd) {
                e->em_ltgt = (e->em_volume * D_800FB060[e->em_pan]) >> 15;
                e->em_rtgt = (e->em_volume * D_800FB060[128 - e->em_pan - 1]) >> 15;
                e->em_delta = e->em_segEnd;
                e->em_cvolL = e->em_ltgt;
                e->em_cvolR = e->em_rtgt;
            } else {
                e->em_cvolL = func_800D355C(e->em_cvolL, e->em_delta, e->em_lratm, e->em_lratl);
                e->em_cvolR = func_800D355C(e->em_cvolR, e->em_delta, e->em_rratm, e->em_rratl);
            }
            if (e->em_cvolL == 0) e->em_cvolL = 1;
            if (e->em_cvolR == 0) e->em_cvolR = 1;
            if (e->em_ctrlList->type == AL_FILTER_SET_PAN)
                e->em_pan = (s16)e->em_ctrlList->data.i;
            if (e->em_ctrlList->type == AL_FILTER_SET_VOLUME) {
                e->em_delta = 0;
                fVol = (e->em_ctrlList->data.i);
                fVol = (fVol * fVol) >> 15;
                e->em_volume = (s16)fVol;
                e->em_segEnd = ((e->em_ctrlList->moredata.i + 92) / 184) * 184;
            }
            if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT) {
                e->em_dryamt = D_800FB060[e->em_ctrlList->data.i];
                e->em_wetamt = D_800FB060[128 - e->em_ctrlList->data.i - 1];
            }
            e->em_first = 1;
            break;

        case AL_FILTER_START_VOICE: {
            ALStartParam *sp = (ALStartParam *)e->em_ctrlList;
            if (sp->unity) {
                e->rs_upitch = 1;
            }
            func_800D39F8(e, AL_FILTER_SET_WAVETABLE, sp->wave);
            e->em_motion = 1;
        } break;

        case AL_FILTER_STOP_VOICE:
            ptr = func_800D31FC(e, &inp, &loutp, samples, ptr);
            func_800D3158(e, AL_FILTER_RESET, NULL);
            break;

        case AL_FILTER_FREE_VOICE: {
            N_ALFreeParam *param = (N_ALFreeParam *)e->em_ctrlList;
            param->pvoice->offset = 0;
            func_800D535C(param->pvoice);
        } break;

        case AL_FILTER_SET_PITCH:
            ptr = func_800D31FC(e, &inp, &loutp, samples, ptr);
            e->rs_ratio = e->em_ctrlList->data.f;
            break;

        case AL_FILTER_SET_UNITY_PITCH:
            ptr = func_800D31FC(e, &inp, &loutp, samples, ptr);
            e->rs_upitch = 1;
            break;

        case AL_FILTER_SET_WAVETABLE:
            ptr = func_800D31FC(e, &inp, &loutp, samples, ptr);
            func_800D39F8(e, AL_FILTER_SET_WAVETABLE, (void *)e->em_ctrlList->data.i);
            break;

        default:
            ptr = func_800D31FC(e, &inp, &loutp, samples, ptr);
            func_800D3158(e, e->em_ctrlList->type, (void *)e->em_ctrlList->data.i);
            break;
        }
        loutp += (samples << 1);
        outCount -= samples;

        thisParam = e->em_ctrlList;
        e->em_ctrlList = e->em_ctrlList->next;
        if (e->em_ctrlList == 0)
            e->em_ctrlTail = 0;
        func_800D52E4(thisParam);
    }

    ptr = func_800D31FC(e, &inp, &loutp, outCount, ptr);
    if (e->em_delta > e->em_segEnd)
        e->em_delta = e->em_segEnd;
    return ptr;
}

s32 func_800D3158(N_PVoice *e, s32 id, void *param) {
    switch (id) {
        case AL_FILTER_ADD_UPDATE:
            if (e->em_ctrlTail != 0) {
                e->em_ctrlTail->next = param;
            } else {
                e->em_ctrlList = param;
            }
            e->em_ctrlTail = param;
            break;
        case AL_FILTER_RESET:
            e->em_first = 1;
            e->em_motion = 0;
            e->em_volume = 1;
            e->em_segEnd = 0;
            e->rs_delta = 0;
            e->rs_first = 1;
            e->rs_upitch = 0;
            func_800D39F8(e, AL_FILTER_RESET, param);
            break;
        case AL_FILTER_START:
            e->em_motion = 1;
            break;
        default:
            func_800D39F8(e, id, param);
            break;
    }
    return 0;
}


Acmd *func_800D31FC(void *filter, s16 *inp, s16 *outp, s32 outCount, Acmd *p) {
    Acmd *ptr = p;
    N_PVoice *e = (N_PVoice *)filter;

    if (e->em_motion != 1 || !outCount) {
        return ptr;
    }
    ptr = func_800D3CD0(e, inp, p);
    if (e->em_first) {
        e->em_first = 0;
        e->em_ltgt = (e->em_volume * D_800FB060[e->em_pan]) >> 15;
        e->em_lratm = func_800D341C((f64)e->em_cvolL, (f64)e->em_ltgt, e->em_segEnd, &e->em_lratl);
        e->em_rtgt = (e->em_volume * D_800FB060[127 - e->em_pan]) >> 15;
        e->em_rratm = func_800D341C((f64)e->em_cvolR, (f64)e->em_rtgt, e->em_segEnd, &e->em_rratl);
        aSetVolume(ptr++, A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);
        aSetVolume(ptr++, A_LEFT | A_VOL, e->em_cvolL, e->em_dryamt, e->em_wetamt);
        aSetVolume(ptr++, A_RIGHT | A_VOL, e->em_rtgt, e->em_rratm, e->em_rratl);
        n_aEnvMixer(ptr++, A_INIT, e->em_cvolR, osVirtualToPhysical(e->em_state));
    } else {
        n_aEnvMixer(ptr++, A_CONTINUE, 0, osVirtualToPhysical(e->em_state));
    }
    *inp += 0x170;
    e->em_delta += 184;
    return ptr;
}

INCLUDE_ASM("asm/nonmatchings/unit_73220", func_800D341C);

s32 func_800D355C(s32 ivol, s32 samples, s16 ratem, s32 ratel) {
    s32 n;
    s32 v;

    n = samples >> 3;
    if (n != 0) {
        v = (ivol + (((s32) ((ratel & 0xFFFF) * n) >> 0x10) + (ratem * n))) << 0x10;
    } else {
        v = ivol << 0x10;
    }
    return v >> 0x10;
}
