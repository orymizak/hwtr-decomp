#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 unk8; } ObjD2_;
typedef struct { ObjD2_ *obj; s32 a; s32 b; s32 pad[3]; } MsgD_;
s32 nuSiSendMesg(s32 cmd, void *args);



void nuContPakFileDelete(ObjD2_ *o, s32 a, s32 b) {
    MsgD_ m;

    m.obj = o;
    m.a = a;
    m.b = b;
    o->unk8 = nuSiSendMesg(0x205, &m);
}
