#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void func_800D4DD0(N_ALVoice *v, ALWaveTable *w) {
    ALStartParam *update;

    if (v->pvoice != NULL) {
        update = (ALStartParam *)func_800D52B8();
        if (update != NULL) {
            update->delta = D_800FB174->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_START_VOICE;
            update->wave = w;
            update->next = NULL;
            update->unity = v->unityPitch;
            func_800D3158(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
