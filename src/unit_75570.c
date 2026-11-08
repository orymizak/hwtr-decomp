#include "common.h"
#include "include_asm.h"
typedef struct ALParamK_ { struct ALParamK_ *next; s32 delta; s16 type; s32 data; s32 moredata; } ALParamK_;
typedef struct VoiceK_ { u8 node[8]; struct PVoiceK_ *pvoice; void *table; void *clientPrivate; s16 state; s16 priority; s16 fxBus; s16 unityPitch; } VoiceK_;
typedef struct PVoiceK_ { u8 node[8]; VoiceK_ *vvoice; u8 pC[0x7C]; s32 offset; } PVoiceK_;
typedef struct { s16 priority; s16 fxBus; u8 unityPitch; } VoiceConfigK_;
typedef struct { u8 p0[0x1C]; s32 paramSamples; } SynthK_;
typedef struct LinkJ_ { struct LinkJ_ *next; struct LinkJ_ *prev; } LinkJ_;
typedef struct { u8 p0[0x16]; s16 priority; } VVoiceJ_;
typedef struct { LinkJ_ node; VVoiceJ_ *vvoice; u8 pC[0x7C]; s32 offset; } PVoiceJ_;
typedef struct { void *head; LinkJ_ pFreeList; LinkJ_ pAllocList; LinkJ_ pLameList; s32 paramSamples; } SynthJ_;
extern SynthJ_ *D_800FB174;
void alUnlink(LinkJ_ *ln);
void alLink(LinkJ_ *ln, LinkJ_ *to);



ALParamK_ *func_800D52B8(void);
void func_800D3158(PVoiceK_ *pv, s32 id, ALParamK_ *p);
s32 func_800D4A88(PVoiceJ_ **pvoice, s16 priority);
s32 func_800D4970(VoiceK_ *voice, VoiceConfigK_ *vc) {
    PVoiceK_ *pvoice = 0;
    ALParamK_ *update;
    s32 stolen;

    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = 0;
    voice->fxBus = vc->fxBus;
    voice->state = 0;
    voice->pvoice = 0;
    stolen = func_800D4A88((PVoiceJ_ **)&pvoice, vc->priority);
    if (pvoice) {
        if (stolen) {
            pvoice->offset = 552;
            pvoice->vvoice->pvoice = 0;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;
            update = func_800D52B8();
            update->delta = D_800FB174->paramSamples;
            update->type = 11;
            update->data = 0;
            update->moredata = 552 - 184;
            func_800D3158(voice->pvoice, 3, update);
            update = func_800D52B8();
            if (update) {
                update->delta = D_800FB174->paramSamples + pvoice->offset;
                update->type = 15;
                update->next = 0;
                func_800D3158(voice->pvoice, 3, update);
            }
        } else {
            pvoice->offset = 0;
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;
        }
    }
    return (pvoice != 0);
}

s32 func_800D4A88(PVoiceJ_ **pvoice, s16 priority) {
    LinkJ_ *dl;
    PVoiceJ_ *pv;
    s32 stolen = 0;

    if ((dl = D_800FB174->pLameList.next) != 0) {
        *pvoice = (PVoiceJ_ *)dl;
        alUnlink(dl);
        alLink(dl, &D_800FB174->pAllocList);
    } else if ((dl = D_800FB174->pFreeList.next) != 0) {
        *pvoice = (PVoiceJ_ *)dl;
        alUnlink(dl);
        alLink(dl, &D_800FB174->pAllocList);
    } else {
        for (dl = D_800FB174->pAllocList.next; dl != 0; dl = dl->next) {
            pv = (PVoiceJ_ *)dl;
            if ((pv->vvoice->priority <= priority) && (pv->offset == 0)) {
                *pvoice = pv;
                priority = pv->vvoice->priority;
                stolen = 1;
            }
        }
    }
    return stolen;
}
