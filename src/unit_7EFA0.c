#include "common.h"

u32 osGetCount(void) {
    register u32 count asm("$2");
    asm("mfc0 %0, $9" : "=r"(count));
    asm("nop");
    return count;
}
