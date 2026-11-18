#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 unk8; } ObjE2_;
typedef struct { ObjE2_ *obj; s32 a; } MsgE_;
s32 nuSiSendMesg(s32 cmd, void *args);



void nuContPakFileState(ObjE2_ *o, s32 a) {
    MsgE_ m;

    m.obj = o;
    m.a = a;
    o->unk8 = nuSiSendMesg(0x206, &m);
}

