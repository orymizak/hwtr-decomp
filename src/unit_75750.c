#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void n_alSynDelete(void) {
    n_syn->head = NULL;
}

void n_alSynSetFXMix(N_ALVoice *v, s32 fxmix) {
    register ALParam *update;
    register u8 mix = fxmix;

    if (v->pvoice != NULL) {
        update = __n_allocParam();
        if (update != NULL) {
            update->delta = n_syn->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_FXAMT;
            if (mix >= 0x80) {
                mix = 0x7F;
            }
            update->data.i = mix;
            update->next = NULL;
            n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
