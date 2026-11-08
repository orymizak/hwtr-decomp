#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 unk8; } ObjE2_;
typedef struct { ObjE2_ *obj; s32 a; } MsgE_;
s32 func_800D7208(s32 cmd, void *args);



void func_800D6960(ObjE2_ *o, s32 a) {
    MsgE_ m;

    m.obj = o;
    m.a = a;
    o->unk8 = func_800D7208(0x206, &m);
}

