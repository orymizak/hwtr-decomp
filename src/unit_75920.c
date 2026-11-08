#include "common.h"
#include "include_asm.h"
typedef struct ALParamH_ { struct ALParamH_ *next; s32 delta; s16 type; s32 data; s32 moredata; } ALParamH_;
typedef struct { u8 p0[0x88]; s32 unk88; } SndH_;
typedef struct { u8 p0[8]; SndH_ *unk8; } ObjH_;
typedef struct { u8 p0[0x1C]; s32 unk1C; } DrvH_;
extern DrvH_ *D_800FB174;
ALParamH_ *func_800D52B8(void);
s32 func_800D53E4(s32 t);
void func_800D3158(SndH_ *s, s32 a, ALParamH_ *p);



void func_800D4D20(ObjH_ *o, s16 vol, s32 t) {
    ALParamH_ *p;

    if (o->unk8 != 0) {
        p = func_800D52B8();
        if (p != 0) {
            p->delta = D_800FB174->unk1C + o->unk8->unk88;
            p->type = 11;
            p->data = vol;
            p->moredata = func_800D53E4(t);
            p->next = 0;
            func_800D3158(o->unk8, 3, p);
        }
    }
}
