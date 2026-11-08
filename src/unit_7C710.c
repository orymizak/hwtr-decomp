#include "common.h"
#include "include_asm.h"
typedef struct ALLink_ { struct ALLink_ *next; struct ALLink_ *prev; } ALLink_;
typedef struct { u32 w0; u32 w1; } Acmd_;
typedef struct ALFilter_ { struct ALFilter_ *source; Acmd_ *(*handler)(void *, s16 *, s32, s32, Acmd_ *); s32 (*setParam)(void *, s32, void *); } ALFilter_;
typedef struct { ALFilter_ filter; u8 padC[8]; s32 sourceCount; s32 maxSources; ALFilter_ **sources; } ALBus_;
typedef struct { ALFilter_ filter; u8 padC[8]; s32 dramout; s32 first; } ALSave2_;

s32 alSynNew();
extern s32 D_800FB5B0;
s32 alSynDelete();

Acmd_ *alSavePull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd_ *p) {
    Acmd_ *ptr = p;
    ALSave2_ *f = (ALSave2_ *)filter;
    ALFilter_ *source = f->filter.source;

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, ptr);
    { Acmd_ *_a = ptr++; _a->w0 = 0x08000000; _a->w1 = (u32)((outCount << 1) & 0xFFFF); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x0D000000; _a->w1 = 0x04400580; }
    { Acmd_ *_a = ptr++; _a->w0 = 0x08000000; _a->w1 = (u32)((outCount << 2) & 0xFFFF); }
    { Acmd_ *_a = ptr++; _a->w0 = 0x06000000; _a->w1 = (u32)(f->dramout); }
    return ptr;
}

s32 alSaveParam(void *filter, s32 paramID, void *param) {
    ALSave2_ *a = (ALSave2_ *)filter;
    ALFilter_ *f = (ALFilter_ *)filter;
    s32 pp = (s32)param;

    switch (paramID) {
        case 1:
            f->source = (ALFilter_ *)param;
            break;
        case 6:
            a->dramout = pp;
            break;
        default:
            break;
    }
    return 0;
}


void alInit(s32 arg0) {
    if (D_800FB5B0 == 0) {
        D_800FB5B0 = arg0;
        alSynNew();
    }
}

void alClose(void) {
    if (D_800FB5B0 != 0) {
        alSynDelete();
        D_800FB5B0 = 0;
    }
}

void alLink(ALLink_ *ln, ALLink_ *to) {
    ln->next = to->next;
    ln->prev = to;
    if (to->next) {
        to->next->prev = ln;
    }
    to->next = ln;
}

void alUnlink(void *arg0) {
    void **temp_v1_2;
    void *temp_v1;

    temp_v1 = *(s32 *)((u8 *)arg0 + 0x0);
    if (temp_v1 != NULL) {
        *(s32 *)((u8 *)temp_v1 + 0x4) = (void **) *(s32 *)((u8 *)arg0 + 0x4);
    }
    temp_v1_2 = *(s32 *)((u8 *)arg0 + 0x4);
    if (temp_v1_2 != NULL) {
        *temp_v1_2 = *(s32 *)((u8 *)arg0 + 0x0);
    }
}
