#include "common.h"
#include "include_asm.h"
typedef struct ALParamE_ { struct ALParamE_ *next; s32 delta; s16 type; s16 unity; void *wave; } ALParamE_;
typedef struct { u8 p0[0x88]; s32 unk88; } SndE_;
typedef struct { u8 p0[8]; SndE_ *unk8; u8 pC[0xE]; s16 unk1A; } ObjE_;
typedef struct { u8 p0[0x1C]; s32 unk1C; } DrvF_;
extern DrvF_ *D_800FB174;
ALParamE_ *func_800D52B8(void);
void func_800D3158(SndE_ *s, s32 a, ALParamE_ *p);



void func_800D4DD0(ObjE_ *o, void *wave) {
    ALParamE_ *p;

    if (o->unk8 != 0) {
        p = func_800D52B8();
        if (p != 0) {
            p->delta = D_800FB174->unk1C + o->unk8->unk88;
            p->type = 14;
            p->wave = wave;
            p->next = 0;
            p->unity = o->unk1A;
            func_800D3158(o->unk8, 3, p);
        }
    }
}

