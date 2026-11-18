#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 unk8; } ObjC2_;
typedef struct { ObjC2_ *obj; s32 res; } MsgC_;
s32 nuSiSendMesg(s32 cmd, void *args);



s32 nuContPakGetFree(ObjC2_ *o) {
    MsgC_ m;

    m.obj = o;
    o->unk8 = nuSiSendMesg(0x202, &m);
    return m.res;
}

