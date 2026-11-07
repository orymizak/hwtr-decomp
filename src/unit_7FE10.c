#include "common.h"
#include "include_asm.h"

extern void *__osRunningThread;

INCLUDE_ASM("asm/nonmatchings/unit_7FE10", osCreateThread);

s32 osGetThreadPri(void *arg0) {
    void *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = __osRunningThread;
    }
    return *(s32 *)((u8 *)var_a0 + 0x4);
}
