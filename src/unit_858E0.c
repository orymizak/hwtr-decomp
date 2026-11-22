#include "common.h"

u32 __osGetSR(void) {
    register u32 sr asm("$2");
    asm("mfc0 %0, $12" : "=r"(sr));
    asm("nop");
    return sr;
}
