#include "common.h"
#include "n_audio.h"
#include "include_asm.h"


/* Allocates a physical voice for voice; a stolen one is faded out over 552 - 184 samples first. */
s32 n_alSynAllocVoice(N_ALVoice *voice, ALVoiceConfig *vc) {
    N_PVoice *pvoice = NULL;
    ALParam *update;
    s32 stolen;

    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = NULL;
    voice->fxBus = vc->fxBus;
    voice->state = AL_STOPPED;
    voice->pvoice = NULL;
    stolen = _n_allocatePVoice(&pvoice, vc->priority);
    if (pvoice) {
        if (stolen) {
            pvoice->offset = 552;
            pvoice->vvoice->pvoice = NULL;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;
            update = __n_allocParam();
            update->delta = n_syn->paramSamples;
            update->type = AL_FILTER_SET_VOLUME;
            update->data.i = 0;
            update->moredata.i = 552 - 184;
            n_alEnvmixerParam(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
            update = __n_allocParam();
            if (update) {
                update->delta = n_syn->paramSamples + pvoice->offset;
                update->type = AL_FILTER_STOP_VOICE;
                update->next = NULL;
                n_alEnvmixerParam(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
            }
        } else {
            pvoice->offset = 0;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;
        }
    }
    return (pvoice != NULL);
}

/* Takes a voice from the lame list, then the free list, else steals the lowest-priority one. */
s32 _n_allocatePVoice(N_PVoice **pvoice, s16 priority) {
    ALLink *dl;
    N_PVoice *pv;
    s32 stolen = 0;

    if ((dl = n_syn->pLameList.next) != NULL) {
        *pvoice = (N_PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &n_syn->pAllocList);
    } else if ((dl = n_syn->pFreeList.next) != NULL) {
        *pvoice = (N_PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &n_syn->pAllocList);
    } else {
        for (dl = n_syn->pAllocList.next; dl != NULL; dl = dl->next) {
            pv = (N_PVoice *)dl;
            if ((pv->vvoice->priority <= priority) && (pv->offset == 0)) {
                *pvoice = pv;
                priority = pv->vvoice->priority;
                stolen = 1;
            }
        }
    }
    return stolen;
}
