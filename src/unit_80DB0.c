#include "common.h"
#include "include_asm.h"

s32 __osDisableInt();
s32 __osRestoreInt(s32);
extern void *__osViNext;

void osViSwapBuffer(s32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    *(s32 *)((u8 *)__osViNext + 0x4) = arg0;
    *(u16 *)((u8 *)__osViNext + 0x0) = (u16) (*(u16 *)((u8 *)__osViNext + 0x0) | 0x10);
    __osRestoreInt(temp_v0);
}
