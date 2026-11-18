#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void func_800D4D20(N_ALVoice *v, s16 vol, s32 t) {
    ALParam *update;

    if (v->pvoice != NULL) {
        update = (ALParam *)func_800D52B8();
        if (update != NULL) {
            update->delta = D_800FB174->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_VOLUME;
            update->data.i = vol;
            update->moredata.i = func_800D53E4(t);
            update->next = NULL;
            func_800D3158(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
