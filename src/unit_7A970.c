#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct ALFilter_ { struct ALFilter_ *source; Acmd_ *(*handler)(void *, s16 *, s32, s32, Acmd_ *); s32 (*setParam)(void *, s32, void *); } ALFilter_;
typedef struct { ALFilter_ filter; u8 padC[8]; s32 sourceCount; s32 maxSources; ALFilter_ **sources; } ALBus_;



Acmd_ *alAuxBusPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd_ *p) {
    Acmd_ *ptr = p;
    ALBus_ *m = (ALBus_ *)filter;
    ALFilter_ **sources = m->sources;
    s32 i;

    { Acmd_ *_a = ptr++; _a->w0 = 0x020006C0; _a->w1 = (u32)(outCount << 1); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x02000800; _a->w1 = (u32)(outCount << 1); }
    for (i = 0; i < m->sourceCount; i++) {
        ptr = (sources[i]->handler)(sources[i], outp, outCount, sampleOffset, ptr);
    }
    return ptr;
}

s32 alAuxBusParam(void *filter, s32 paramID, void *param) {
    ALBus_ *m = (ALBus_ *)filter;
    ALFilter_ **sources = m->sources;

    switch (paramID) {
        case 2:
            sources[m->sourceCount++] = (ALFilter_ *)param;
            break;
        default:
            break;
    }
    return 0;
}

