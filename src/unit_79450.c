#include "common.h"
#include "include_asm.h"



s32 osAiGetLength(void) {
    return *(volatile s32 *)0xA4500004;
}

s32 osAiGetStatus(void) {
    return *(volatile s32 *)0xA450000C;
}

INCLUDE_ASM("asm/nonmatchings/unit_79450", osAiSetFrequency);
