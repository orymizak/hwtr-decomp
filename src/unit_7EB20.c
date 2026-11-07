#include "common.h"
#include "include_asm.h"

extern s32 __osThreadTail;

void osCreateMesgQueue(void *arg0, s32 arg1, s32 arg2) {
    *(s32 *)((u8 *)arg0 + 0x0) = &__osThreadTail;
    *(s32 *)((u8 *)arg0 + 0x4) = &__osThreadTail;
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = arg2;
    *(s32 *)((u8 *)arg0 + 0x14) = arg1;
}
