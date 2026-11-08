#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_7E930", bzero);

const char *strchr(const char *s, s32 c) {
    const char ch = c;

    while (*s != ch) {
        if (*s++ == 0) {
            return 0;
        }
    }
    return s;
}


s32 strlen(s8 *arg0) {
    s8 *var_v1;

    var_v1 = arg0;
    if (*arg0 != 0) {
        do {
            var_v1 += 1;
        } while (*var_v1 != 0);
    }
    return var_v1 - arg0;
}

u8 *memcpy(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 var_a2;
    u8 *var_a1;
    u8 *var_v1;
    u8 temp_v0;

    var_a1 = arg1;
    var_a2 = arg2;
    var_v1 = arg0;
    if (var_a2 != 0) {
        do {
            temp_v0 = *var_a1;
            var_a1 += 1;
            var_a2 -= 1;
            *var_v1 = temp_v0;
            var_v1 += 1;
        } while (var_a2 != 0);
    }
    return arg0;
}
