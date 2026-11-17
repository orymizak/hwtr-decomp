#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[0x24]; s32 unk24[8]; } SynMB5_;
typedef struct { u8 p0[0x34]; SynMB5_ *unk34; } SynD5_;
typedef struct { u32 w0; u32 w1; } Acmd5_;
typedef struct { u8 p0[4]; Acmd5_ *(*fn)(s32, Acmd5_ *); } Fx5_;
s32 func_800D26C0(s32);
extern void *D_800FB174;



s32 func_800D5490(s32 arg0) {
    s32 i = (s16)arg0;

    func_800D26C0(&((SynD5_ *)D_800FB174)->unk34->unk24[i]);
    return ((SynD5_ *)D_800FB174)->unk34->unk24[i];
}

Acmd5_ *func_800D54E0(s32 a, Acmd5_ *cmd) {
    Acmd5_ *ptr = cmd;

    {
        Acmd5_ *_a = (Acmd5_ *)ptr++;
        _a->w0 = 0x020004E0;
        _a->w1 = 0x2E0;
    }
    ptr = (*(Fx5_ **)((u8 *)D_800FB174 + 0x30))->fn(a, ptr);
    {
        Acmd5_ *_a = (Acmd5_ *)ptr++;
        _a->w0 = 0x0C007FFF;
        _a->w1 = 0x07C004E0;
    }
    {
        Acmd5_ *_a = (Acmd5_ *)ptr++;
        _a->w0 = 0x0C007FFF;
        _a->w1 = 0x09300650;
    }
    return ptr;
}

