#include "common.h"
#include "include_asm.h"
void *memcpy(void *dst, const void *src, u32 n);
void *proutSprintf(void *s, const char *buf, u32 n);
s32 _Printf(void *(*prout)(void *, const char *, u32), void *arg, const char *fmt, char *ap);



s32 sprintf(char *s, const char *fmt, ...) {
    s32 ans;
    char *ap;

    ap = (char *)__builtin_next_arg(fmt);
    ans = _Printf(proutSprintf, s, fmt, ap);
    if (ans >= 0) {
        s[ans] = 0;
    }
    return ans;
}


void *proutSprintf(void *s, const char *buf, u32 n) {
    return (char *)memcpy(s, buf, n) + n;
}

