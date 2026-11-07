#include "common.h"
#include "include_asm.h"

s32 __osDisableInt();
s32 __osRestoreInt(s32);
extern void *__osViNext;

void osViSetYScale(f32 arg0) {
    s32 temp_v0;

    temp_v0 = __osDisableInt();
    *(f32 *)((u8 *)__osViNext + 0x24) = arg0;
    *(u16 *)((u8 *)__osViNext + 0x0) = (u16) (*(u16 *)((u8 *)__osViNext + 0x0) | 4);
    __osRestoreInt(temp_v0);
}
