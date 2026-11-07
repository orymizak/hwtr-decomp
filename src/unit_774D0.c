#include "common.h"
#include "include_asm.h"
typedef struct { void *a; s32 b; s32 c; s8 d; s32 e; } Evt_;
s32 func_800D7208(s32, Evt_ *);



void func_800D68D0(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Evt_ evt;

    evt.b = arg1;
    evt.a = arg0;
    evt.c = arg2;
    evt.e = arg3;
    evt.d = arg4;
    *(s32 *)(arg0 + 8) = func_800D7208(0x204, &evt);
}
