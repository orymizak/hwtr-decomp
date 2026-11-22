#include "common.h"

u32 __osGetCause(void) {
    register u32 cause asm("$2");
    asm("mfc0 %0, $13" : "=r"(cause));
    asm("nop");
    return cause;
}
