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


s32 strlen(const char *s) {
    const char *sc = s;

    while (*sc != 0) {
        sc++;
    }
    return sc - s;
}

void *memcpy(void *s1, const void *s2, u32 n) {
    char *su1 = (char *)s1;
    const char *su2 = (const char *)s2;

    while (n > 0) {
        *su1 = *su2;
        su1++;
        su2++;
        n--;
    }
    return s1;
}
