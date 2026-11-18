#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

s32 func_800D4A88(N_PVoice **pvoice, s16 priority);

/* Allocates a physical voice for voice; a stolen one is faded out over 552 - 184 samples first. */
s32 func_800D4970(N_ALVoice *voice, ALVoiceConfig *vc) {
    N_PVoice *pvoice = NULL;
    ALParam *update;
    s32 stolen;

    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = NULL;
    voice->fxBus = vc->fxBus;
    voice->state = AL_STOPPED;
    voice->pvoice = NULL;
    stolen = func_800D4A88(&pvoice, vc->priority);
    if (pvoice) {
        if (stolen) {
            pvoice->offset = 552;
            pvoice->vvoice->pvoice = NULL;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;
            update = func_800D52B8();
            update->delta = D_800FB174->paramSamples;
            update->type = AL_FILTER_SET_VOLUME;
            update->data.i = 0;
            update->moredata.i = 552 - 184;
            func_800D3158(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
            update = func_800D52B8();
            if (update) {
                update->delta = D_800FB174->paramSamples + pvoice->offset;
                update->type = AL_FILTER_STOP_VOICE;
                update->next = NULL;
                func_800D3158(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
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
s32 func_800D4A88(N_PVoice **pvoice, s16 priority) {
    ALLink *dl;
    N_PVoice *pv;
    s32 stolen = 0;

    if ((dl = D_800FB174->pLameList.next) != NULL) {
        *pvoice = (N_PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &D_800FB174->pAllocList);
    } else if ((dl = D_800FB174->pFreeList.next) != NULL) {
        *pvoice = (N_PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &D_800FB174->pAllocList);
    } else {
        for (dl = D_800FB174->pAllocList.next; dl != NULL; dl = dl->next) {
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
