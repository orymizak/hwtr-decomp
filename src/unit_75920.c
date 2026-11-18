#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void n_alSynSetVol(N_ALVoice *v, s16 vol, s32 t) {
    ALParam *update;

    if (v->pvoice != NULL) {
        update = (ALParam *)__n_allocParam();
        if (update != NULL) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_VOLUME;
            update->data.i = vol;
            update->moredata.i = _n_timeToSamples(t);
            update->next = NULL;
            n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
