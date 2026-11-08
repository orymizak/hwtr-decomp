#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 unk8; } ObjF2_;
typedef struct { ObjF2_ *obj; s32 a; s32 b; s32 pad; } MsgF_;
s32 func_800D7208(s32 cmd, void *args);



void func_800D69A0(ObjF2_ *o, s32 a, s32 b) {
    MsgF_ m;

    m.obj = o;
    m.a = a;
    m.b = b;
    o->unk8 = func_800D7208(0x207, &m);
}

