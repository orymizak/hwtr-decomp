#include "common.h"
#include "include_asm.h"
/* Minimal libaudio layouts used here (sizes from ultralib). */
typedef struct { u8 p0[0x20]; u8 fx[0x2C]; } ALAuxBus_;
typedef struct { u8 p0[0x30]; s32 mainBus; ALAuxBus_ *auxBus; } ALSynthFX_;
s32 alFxNew(s32,s32,s32);
s32 func_800DAC90(s32,s32, s32);
s32 alFxParam(s32,s32, s32);



s32 alSynAllocFX(ALSynthFX_ *s, s16 bus, s32 c, s32 hp) {
    alFxNew((s32)s->auxBus[bus].fx, c, hp);
    alFxParam((s32)s->auxBus[bus].fx, 1, (s32)&s->auxBus[bus]);
    func_800DAC90(s->mainBus, 2, (s32)s->auxBus[bus].fx);
    return (s32)s->auxBus[bus].fx;
}
