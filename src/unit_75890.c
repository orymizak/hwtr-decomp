#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void func_800D4C90(N_ALVoice *v, f32 pitch) {
    ALParam *update;

    if (v->pvoice != NULL) {
        update = (ALParam *)func_800D52B8();
        if (update != NULL) {
            update->delta = D_800FB174->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_PITCH;
            update->data.f = pitch;
            update->next = NULL;
            func_800D3158(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
