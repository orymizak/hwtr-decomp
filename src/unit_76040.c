#include "common.h"
#include "include_asm.h"
typedef struct { u32 w0; u32 w1; } Acmd4_;
extern void *D_800FB174;
Acmd4_ *func_800D54E0(s32, Acmd4_ *);



Acmd4_ *func_800D5440(s32 a, Acmd4_ *cmd) {
    Acmd4_ *ptr;

    ptr = func_800D54E0(a, cmd);
    {
        Acmd4_ *_a = (Acmd4_ *)ptr++;
        _a->w0 = 0x0D000000;
    }
    {
        Acmd4_ *_a = (Acmd4_ *)ptr++;
        _a->w0 = 0x062E0000;
        _a->w1 = *(s32 *)((u8 *)D_800FB174 + 0x48);
    }
    return ptr;
}
