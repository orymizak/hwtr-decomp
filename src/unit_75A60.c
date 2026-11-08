#include "common.h"
#include "include_asm.h"
typedef struct ALParamC_ { struct ALParamC_ *next; s32 delta; s16 type; } ALParamC_;
typedef struct { u8 p0[0x88]; s32 unk88; } SndC_;
typedef struct { u8 p0[8]; SndC_ *unk8; } ObjC_;
typedef struct { u8 p0[0x1C]; s32 unk1C; } DrvC_;
extern DrvC_ *D_800FB174;
ALParamC_ *func_800D52B8(void);
void func_800D3158(SndC_ *s, s32 a, ALParamC_ *p);



void func_800D4E60(ObjC_ *o) {
    ALParamC_ *p;

    if (o->unk8 != 0) {
        p = func_800D52B8();
        if (p != 0) {
            p->delta = D_800FB174->unk1C + o->unk8->unk88;
            p->type = 15;
            p->next = 0;
            func_800D3158(o->unk8, 3, p);
        }
    }
}

