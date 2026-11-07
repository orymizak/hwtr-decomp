#include "common.h"
#include "include_asm.h"



s32 __osSpGetStatus(void) {
    return *(volatile s32 *)0xA4040010;
}

void __osSpSetStatus(s32 arg0) {
    *(volatile s32 *)0xA4040010 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/unit_85950", __osSpSetPc);

INCLUDE_ASM("asm/nonmatchings/unit_85950", __osSpRawStartDma);
