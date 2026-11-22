#include "common.h"
#include "include_asm.h"
s32 __osDpDeviceBusy(void);
u32 osVirtualToPhysical(void *);



s32 bcmp(const void *s1, const void *s2, u32 n) {
    asm volatile(
        ".set noreorder\n\t"
        ".set noat\n\t"
        "xor $v0, $a0, $a1\n\t"
        "slti $at, $a2, 0x10\n\t"
        "bnez $at, .L800E41AC\n\t"
        "nop\n\t"
        "andi $v0, $v0, 0x3\n\t"
        "bnez $v0, .L800E4148\n\t"
        "negu $t8, $a0\n\t"
        "andi $t8, $t8, 0x3\n\t"
        "beqz $t8, .L800E4110\n\t"
        "subu $a2, $a2, $t8\n\t"
        "addu $v0, $v1, $zero\n\t"
        "lwl $v0, 0x0($a0)\n\t"
        "lwl $v1, 0x0($a1)\n\t"
        "addu $a0, $a0, $t8\n\t"
        "bne $v0, $v1, .L800E41D8\n\t"
        "addu $a1, $a1, $t8\n\t"
        ".L800E4110:\n\t"
        "addiu $at, $zero, -0x4\n\t"
        "and $a3, $a2, $at\n\t"
        "beqz $a3, .L800E41AC\n\t"
        "subu $a2, $a2, $a3\n\t"
        "addu $a3, $a3, $a0\n\t"
        ".L800E4124:\n\t"
        "lw $v0, 0x0($a0)\n\t"
        "lw $v1, 0x0($a1)\n\t"
        "addiu $a0, $a0, 0x4\n\t"
        "bne $v0, $v1, .L800E41D8\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "bne $a0, $a3, .L800E4124\n\t"
        "nop\n\t"
        "b .L800E41AC\n\t"
        "nop\n\t"
        ".L800E4148:\n\t"
        "negu $a3, $a1\n\t"
        "andi $a3, $a3, 0x3\n\t"
        "beqz $a3, .L800E4178\n\t"
        "subu $a2, $a2, $a3\n\t"
        "addu $a3, $a3, $a0\n\t"
        ".L800E415C:\n\t"
        "lbu $v0, 0x0($a0)\n\t"
        "lbu $v1, 0x0($a1)\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "bne $v0, $v1, .L800E41D8\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "bne $a0, $a3, .L800E415C\n\t"
        "nop\n\t"
        ".L800E4178:\n\t"
        "addiu $at, $zero, -0x4\n\t"
        "and $a3, $a2, $at\n\t"
        "beqz $a3, .L800E41AC\n\t"
        "subu $a2, $a2, $a3\n\t"
        "addu $a3, $a3, $a0\n\t"
        ".L800E418C:\n\t"
        "lwl $v0, 0x0($a0)\n\t"
        "lwr $v0, 0x3($a0)\n\t"
        "lw $v1, 0x0($a1)\n\t"
        "addiu $a0, $a0, 0x4\n\t"
        "bne $v0, $v1, .L800E41D8\n\t"
        "addiu $a1, $a1, 0x4\n\t"
        "bne $a0, $a3, .L800E418C\n\t"
        "nop\n\t"
        ".L800E41AC:\n\t"
        "blez $a2, .L800E41D0\n\t"
        "addu $a3, $a2, $a0\n\t"
        ".L800E41B4:\n\t"
        "lbu $v0, 0x0($a0)\n\t"
        "lbu $v1, 0x0($a1)\n\t"
        "addiu $a0, $a0, 0x1\n\t"
        "bne $v0, $v1, .L800E41D8\n\t"
        "addiu $a1, $a1, 0x1\n\t"
        "bne $a0, $a3, .L800E41B4\n\t"
        "nop\n\t"
        ".L800E41D0:\n\t"
        "jr $ra\n\t"
        "addu $v0, $zero, $zero\n\t"
        ".L800E41D8:\n\t"
        ".set at\n\t"
        ".set reorder"
        :
        :
        : "v0", "v1", "a0", "a1", "a2", "a3", "t8"
    );
    return 1;
}

typedef struct { union { s64 ll; f64 ld; } v; u8 *s; s32 n0; s32 nz0; s32 n1; s32 nz1; s32 n2; s32 nz2; s32 prec; s32 width; u32 nchar; u32 flags; char qual; } Pft_;
#define __va_rounded_size(__TYPE) (((sizeof(__TYPE) + sizeof(s32) - 1) / sizeof(s32)) * sizeof(s32))
#define va_arg(__AP, __type) ((__AP = (char *)((__alignof__(__type) > 4 ? ((s32)__AP + 8 - 1) & -8 : ((s32)__AP + 4 - 1) & -4) + __va_rounded_size(__type))), *(__type *)(void *)(__AP - __va_rounded_size(__type)))
#define isdigit(x) ((x >= '0' && x <= '9'))
#define ATOI(dst, src) for (dst = 0; isdigit(*src); ++src) { if (dst < 999) dst = dst * 10 + *src - '0'; }
#define MAX_PAD ((sizeof(D_800FC840) - 1))
#define PAD(s, n) if (0 < (n)) { s32 i, j = (n); for (; 0 < j; j -= i) { i = MAX_PAD < (u32)j ? (s32)MAX_PAD : j; PUT(s, i); } }
#define PUT(s, n) if (0 < (n)) { if ((arg = (*pfn)(arg, s, n)) != 0) x.nchar += (n); else return x.nchar; }
extern char D_800FC840[33];
extern char D_800FC864[33];
extern const char D_80101BF0[];
extern const u32 D_80101BF8[];
extern const char D_80101C10[];
const char *strchr(const char *s, s32 c);
void _Putfld(Pft_ *px, char **pap, char code, char *ac);
void _Litob(Pft_ *px, char code);
void _Ldtob(Pft_ *px, char code);
u32 strlen(const char *s);
s32 _Printf(void *pfn(void *, const char *, u32), void *arg, const char *fmt, char *ap) {
    Pft_ x;

    x.nchar = 0;
    while (1) {
        const char *s;
        char c;
        const char *t;
        char ac[32];
        s = fmt;

        for (c = *s; c != 0 && c != '%';) {
            c = *++s;
        }
        PUT(fmt, s - fmt);
        if (c == 0) {
            return x.nchar;
        }
        fmt = ++s;
        for (x.flags = 0; (t = strchr(D_80101BF0, *s)) != 0; s++) {
            x.flags |= D_80101BF8[t - D_80101BF0];
        }
        if (*s == '*') {
            x.width = va_arg(ap, s32);
            if (x.width < 0) {
                x.width = -x.width;
                x.flags |= 4;
            }
            s++;
        } else
            ATOI(x.width, s);
        if (*s != '.') {
            x.prec = -1;
        } else if (*++s == '*') {
            x.prec = va_arg(ap, s32);
            ++s;
        } else
            for (x.prec = 0; isdigit(*s); s++) {
                if (x.prec < 999)
                    x.prec = x.prec * 10 + *s - '0';
            }
        x.qual = strchr(D_80101C10, *s) ? *s++ : '\0';
        if (x.qual == 'l' && *s == 'l') {
            x.qual = 'L';
            ++s;
        }
        _Putfld(&x, &ap, *s, ac);
        x.width -= x.n0 + x.nz0 + x.n1 + x.nz1 + x.n2 + x.nz2;
        {
            if (!(x.flags & 4)) {
                s32 i, j;
                if (0 < (x.width)) {
                    i, j = x.width;
                    for (; 0 < j; j -= i) {
                        i = MAX_PAD < (u32)j ? (s32)MAX_PAD : j;
                        PUT(D_800FC840, i);
                    }
                }
            }
            PUT(ac, x.n0);
            PAD(D_800FC864, x.nz0)
            PUT(x.s, x.n1);
            PAD(D_800FC864, x.nz1);
            PUT(x.s + x.n1, x.n2);
            PAD(D_800FC864, x.nz2);
            if (x.flags & 4) {
                PAD(D_800FC840, x.width);
            }
        }
        fmt = s + 1;
    }
    return 0;
}

void _Putfld(Pft_ *px, char **pap, char code, char *ac) {
    px->n0 = px->nz0 = px->n1 = px->nz1 = px->n2 = px->nz2 = 0;
    switch (code) {
        case 'c':
            ac[px->n0++] = va_arg(*pap, s32);
            break;
        case 'd':
        case 'i':
            if (px->qual == 'l') {
                px->v.ll = va_arg(*pap, long);
            } else if (px->qual == 'L') {
                px->v.ll = va_arg(*pap, s64);
            } else {
                px->v.ll = va_arg(*pap, s32);
            }
            if (px->qual == 'h') {
                px->v.ll = (s16)px->v.ll;
            }
            if (px->v.ll < 0) {
                ac[px->n0++] = '-';
            } else if (px->flags & 2) {
                ac[px->n0++] = '+';
            } else if (px->flags & 1) {
                ac[px->n0++] = ' ';
            }
            px->s = (u8 *)&ac[px->n0];
            _Litob(px, code);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            if (px->qual == 'l') {
                px->v.ll = va_arg(*pap, long);
            } else if (px->qual == 'L') {
                px->v.ll = va_arg(*pap, s64);
            } else {
                px->v.ll = va_arg(*pap, s32);
            }
            if (px->qual == 'h') {
                px->v.ll = (u16)px->v.ll;
            } else if (px->qual == 0) {
                px->v.ll = (u32)px->v.ll;
            }
            if (px->flags & 8) {
                ac[px->n0++] = '0';
                if (code == 'x' || code == 'X') {
                    ac[px->n0++] = code;
                }
            }
            px->s = (u8 *)&ac[px->n0];
            _Litob(px, code);
            break;
        case 'e':
        case 'f':
        case 'g':
        case 'E':
        case 'G':
            px->v.ld = px->qual == 'L' ? va_arg(*pap, f64) : va_arg(*pap, f64);
            if (((u16 *)&(px->v.ld))[0] & 0x8000)
                ac[px->n0++] = '-';
            else if (px->flags & 2)
                ac[px->n0++] = '+';
            else if (px->flags & 1)
                ac[px->n0++] = ' ';
            px->s = (u8 *)&ac[px->n0];
            _Ldtob(px, code);
            break;
        case 'n':
            if (px->qual == 'h') {
                *va_arg(*pap, u16 *) = px->nchar;
            } else if (px->qual == 'l') {
                *va_arg(*pap, u32 *) = px->nchar;
            } else if (px->qual == 'L') {
                *va_arg(*pap, u64 *) = px->nchar;
            } else {
                *va_arg(*pap, u32 *) = px->nchar;
            }
            break;
        case 'p':
            px->v.ll = (long)va_arg(*pap, void *);
            px->s = (u8 *)&ac[px->n0];
            _Litob(px, 'x');
            break;
        case 's':
            px->s = va_arg(*pap, u8 *);
            px->n1 = strlen((char *)px->s);
            if (px->prec >= 0 && px->prec < px->n1) {
                px->n1 = px->prec;
            }
            break;
        case '%':
            ac[px->n0++] = '%';
            break;
        default:
            ac[px->n0++] = code;
            break;
    }
}

s32 osDpSetNextBuffer(void *bufPtr, u64 size) {
    if (__osDpDeviceBusy()) {
        return -1;
    }
    *(volatile u32 *)0xA410000C = 1;
    do {
    } while (*(volatile u32 *)0xA410000C & 1);
    *(volatile u32 *)0xA4100000 = osVirtualToPhysical(bufPtr);
    *(volatile u32 *)0xA4100004 = osVirtualToPhysical(bufPtr) + (u32)size;
    return 0;
}
