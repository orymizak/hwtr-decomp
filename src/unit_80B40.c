#include "common.h"
#include "include_asm.h"

s32 __osDisableInt();
s32 __osRestoreInt(s32);
extern void *__osViNext;

void osViSetEvent(s32 arg0, s32 arg1, s16 arg2) {
    s32 temp_a0;

    temp_a0 = __osDisableInt();
    *(s32 *)((u8 *)__osViNext + 0x10) = arg0;
    *(s32 *)((u8 *)__osViNext + 0x14) = arg1;
    *(s16 *)((u8 *)__osViNext + 0x2) = arg2;
    __osRestoreInt(temp_a0);
}
