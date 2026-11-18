#include "common.h"
#include "n_audio.h"
#include "include_asm.h"

void func_800D4B50(void) {
    D_800FB174->head = NULL;
}

void func_800D4B60(N_ALVoice *v, s32 fxmix) {
    register ALParam *update;
    register u8 mix = fxmix;

    if (v->pvoice != NULL) {
        update = func_800D52B8();
        if (update != NULL) {
            update->delta = D_800FB174->paramSamples + v->pvoice->offset;
            update->type = AL_FILTER_SET_FXAMT;
            if (mix >= 0x80) {
                mix = 0x7F;
            }
            update->data.i = mix;
            update->next = NULL;
            func_800D3158(v->pvoice, AL_FILTER_ADD_UPDATE, update);
        }
    }
}
