#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void n_alSynSetPan(N_ALVoice *v, u8 pan) {
    ALParam *update;

    if (v->pvoice != NULL) {
        update = (ALParam *)__n_allocParam();
        if (update != NULL) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_PAN;
            update->data.i = pan;
            update->next = NULL;
            n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
