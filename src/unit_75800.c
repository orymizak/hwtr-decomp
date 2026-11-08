#include "common.h"
#include "include_asm.h"
typedef struct ALParamD_ { struct ALParamD_ *next; s32 delta; s16 type; union { f32 f; s32 i; } data; } ALParamD_;
typedef struct { u8 p0[0x88]; s32 unk88; } SndD_;
typedef struct { u8 p0[8]; SndD_ *unk8; } ObjD_;
typedef struct { u8 p0[0x1C]; s32 unk1C; } DrvE_;
extern DrvE_ *D_800FB174;
ALParamD_ *func_800D52B8(void);
void func_800D3158(SndD_ *s, s32 a, ALParamD_ *p);



void func_800D4C00(ObjD_ *o, u8 val) {
    ALParamD_ *p;

    if (o->unk8 != 0) {
        p = func_800D52B8();
        if (p != 0) {
            p->delta = D_800FB174->unk1C + o->unk8->unk88;
            p->type = 12;
            p->data.i = val;
            p->next = 0;
            func_800D3158(o->unk8, 3, p);
        }
    }
}

