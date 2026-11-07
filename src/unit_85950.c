#include "common.h"
#include "include_asm.h"



s32 __osSpGetStatus(void) {
    return *(volatile s32 *)0xA4040010;
}

void __osSpSetStatus(s32 arg0) {
    *(volatile s32 *)0xA4040010 = arg0;
}

s32 __osSpSetPc(u32 pc) {
    register u32 stat = *(volatile u32 *)0xA4040010;

    if (!(stat & 1)) {
        return -1;
    }
    *(volatile u32 *)0xA4080000 = pc;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_85950", __osSpRawStartDma);
