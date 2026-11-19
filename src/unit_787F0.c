#include "common.h"
#include "include_asm.h"



typedef struct { u8 p0[8]; s32 unk8; s32 unkC; } PakFileB_;
typedef struct { PakFileB_ *file; u8 *name; u8 *ext; s32 a; s32 b; } PakArgsB_;
void bcopy(void *, void *, s32);
void bzero(void *, s32);
s32 nuSiSendMesg(u16 cmd, void *args);
void nuContPakFileOpen(PakFileB_ *file, u8 *name, u8 *ext, s32 b, s32 a) {
    PakArgsB_ args;
    u8 nameBuf[16];
    u8 extBuf[4];
    u8 *ep;
    PakArgsB_ *msg;
    u8 *np;

    if (file->unkC != 0) {
        msg = &args;
        np = nameBuf;
        ep = extBuf;
        bcopy(name, np, 16);
        bzero(ep, 4);
        ep[0] = *ext;
        msg->file = file;
        msg->name = np;
        msg->ext = ep;
        msg->a = a;
        msg->b = b;
        file->unk8 = nuSiSendMesg(0x203, msg);
    } else {
        file->unk8 = 1;
    }
}
