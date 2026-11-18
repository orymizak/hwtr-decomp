#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void n_alSynSetPitch(N_ALVoice *v, f32 pitch) {
    ALParam *update;

    if (v->pvoice != NULL) {
        update = (ALParam *)__n_allocParam();
        if (update != NULL) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_PITCH;
            update->data.f = pitch;
            update->next = NULL;
            n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
