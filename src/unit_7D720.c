#include "common.h"

f32 sqrtf(f32 x) {
    f32 res;
    asm("sqrt.s %0, %1" : "=f"(res) : "f"(x));
    return res;
}
