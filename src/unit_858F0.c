#include "common.h"

void __osSetCompare(u32 val) {
    asm("mtc0 %0, $11" : : "r"(val));
    asm("nop");
}
