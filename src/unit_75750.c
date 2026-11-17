#include "common.h"
#include "include_asm.h"
/* libaudio-like layouts for alSynSetFXMix. */
typedef struct ParamFx_ { struct ParamFx_ *next; s32 delta; u16 type; u8 pA[2]; u32 data; } ParamFx_;
typedef struct { u8 p0[0x88]; s32 offset; } PVoiceFx_;
typedef struct { u8 p0[8]; PVoiceFx_ *pvoice; } VoiceFx_;
typedef struct { u8 p0[0x1C]; s32 paramSamples; } SynthFx_;
s32 func_800D3158(void *, u16, s32);
void *func_800D52B8();

extern s32 *D_800FB174;

void func_800D4B50(void) {
    *D_800FB174 = 0;
}

void func_800D4B60(VoiceFx_ *v, s32 fxmix) {
    register ParamFx_ *update;
    register u8 mix = fxmix;

    if (v->pvoice != NULL) {
        update = (ParamFx_ *)func_800D52B8();
        if (update != NULL) {
            update->delta = ((SynthFx_ *)D_800FB174)->paramSamples + v->pvoice->offset;
            update->type = 0x10;
            if (mix >= 0x80) {
                mix = 0x7F;
            }
            update->data = mix;
            update->next = NULL;
            func_800D3158(v->pvoice, 3, update);
        }
    }
}
