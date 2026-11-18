#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void n_alSynStartVoice(N_ALVoice *v, ALWaveTable *w) {
    ALStartParam *update;

    if (v->pvoice != NULL) {
        update = (ALStartParam *)__n_allocParam();
        if (update != NULL) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_START_VOICE;
            update->wave = w;
            update->next = NULL;
            update->unity = v->unityPitch;
            n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
