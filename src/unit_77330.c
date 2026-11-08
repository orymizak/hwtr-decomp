#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 unk8; } ObjC2_;
typedef struct { ObjC2_ *obj; s32 res; } MsgC_;
s32 func_800D7208(s32 cmd, void *args);



s32 func_800D6730(ObjC2_ *o) {
    MsgC_ m;

    m.obj = o;
    o->unk8 = func_800D7208(0x202, &m);
    return m.res;
}

