#include "common.h"
#include "include_asm.h"
typedef struct { s32 quot; s32 rem; } ldiv_t_;
extern const f64 D_80101D70[];
extern char D_80101DB8[];
extern char D_80101DBC[];
ldiv_t_ ldiv(s32 num, s32 denom);
extern u8 D_80101DD0[];
typedef struct { union { s64 ll; f64 ld; } v; u8 *s; s32 n0; s32 nz0; s32 n1; s32 nz1; s32 n2; s32 nz2; s32 prec; s32 width; u32 nchar; u32 flags; char qual; } Pft_;
typedef struct { s64 quot; s64 rem; } lldiv_t_;
extern char D_800FC9F0[];
extern char D_800FCA04[];
lldiv_t_ lldiv(s64 num, s64 denom);
void *memcpy(void *dst, const void *src, u32 n);



void _Litob(Pft_ *px, char code) {
    char buff[0x18];
    const char *digs;
    s32 base;
    s32 i;
    u64 ullval;

    digs = (code == 'X') ? D_800FCA04 : D_800FC9F0;
    base = (code == 'o') ? 8 : ((code != 'x' && code != 'X') ? 10 : 16);
    i = 0x18;
    ullval = px->v.ll;
    if ((code == 'd' || code == 'i') && px->v.ll < 0) {
        ullval = -ullval;
    }
    if (ullval != 0 || px->prec != 0) {
        buff[--i] = digs[ullval % base];
    }
    px->v.ll = ullval / base;
    while (px->v.ll > 0 && i > 0) {
        lldiv_t_ qr = lldiv(px->v.ll, base);

        px->v.ll = qr.quot;
        buff[--i] = digs[qr.rem];
    }
    px->n1 = 0x18 - i;
    memcpy(px->s, buff + i, px->n1);
    if (px->n1 < px->prec) {
        px->nz0 = px->prec - px->n1;
    }
    if (px->prec < 0 && (px->flags & (16 | 4)) == 16) {
        if ((i = px->width - px->n0 - px->nz0 - px->n1) > 0) {
            px->nz0 += i;
        }
    }
}

s16 _Ldunscale(s16 *pex, f64 *px);
void _Genld(Pft_ *px, char code, u8 *p, s16 nsig, s16 xexp);
void _Ldtob(Pft_ *px, char code) {
    char buff[0x20];
    char *p;
    f64 ldval;
    s16 err;
    s16 nsig;
    s16 xexp;

    p = buff;
    ldval = px->v.ld;
    if (px->prec < 0) {
        px->prec = 6;
    } else if (px->prec == 0 && (code == 'g' || code == 'G')) {
        px->prec = 1;
    }
    err = _Ldunscale(&xexp, &px->v.ld);
    if (err > 0) {
        memcpy(px->s, err == 2 ? D_80101DB8 : D_80101DBC, px->n1 = 3);
        return;
    } else if (err == 0) {
        nsig = 0;
        xexp = 0;
    } else {
        {
            s32 i;
            s32 n;

            if (ldval < 0) {
                ldval = -ldval;
            }
            if ((xexp = xexp * 30103 / 100000 - 4) < 0) {
                n = (((u32)(-xexp) + 3) & ~3), xexp = -n;
                for (i = 0; n > 0; n >>= 1, i++) {
                    if (n & 1) {
                        ldval *= D_80101D70[i];
                    }
                }
            } else if (xexp > 0) {
                f64 factor = 1;

                xexp &= ~3;
                for (n = xexp, i = 0; n > 0; n >>= 1, i++) {
                    if (n & 1) {
                        factor *= D_80101D70[i];
                    }
                }
                ldval /= factor;
            }
        }
        {
            s32 gen = px->prec + ((code == 'f') ? 10 + xexp : 6);

            if (gen > 0x13) {
                gen = 0x13;
            }
            for (*p++ = '0'; gen > 0 && 0 < ldval; p += 8) {
                s32 j;
                s32 lo = ldval;

                if ((gen -= 8) > 0) {
                    ldval = (ldval - lo) * 1e8;
                }
                for (p += 8, j = 8; lo > 0 && --j >= 0;) {
                    ldiv_t_ qr;
                    qr = ldiv(lo, 10);
                    *--p = qr.rem + '0', lo = qr.quot;
                }
                while (--j >= 0) {
                    *--p = '0';
                }
            }
            gen = p - &buff[1];
            for (p = &buff[1], xexp += 7; *p == '0'; p++) {
                --gen, --xexp;
            }
            nsig = px->prec + ((code == 'f') ? xexp + 1 : ((code == 'e' || code == 'E') ? 1 : 0));
            if (gen < nsig) {
                nsig = gen;
            }
            if (nsig > 0) {
                const char drop = nsig < gen && '5' <= p[nsig] ? '9' : '0';
                s32 n;

                for (n = nsig; p[--n] == drop;) {
                    --nsig;
                }
                if (drop == '9') {
                    ++p[n];
                }
                if (n < 0) {
                    --p, ++nsig, ++xexp;
                }
            }
        }
    }
    _Genld(px, code, (u8 *)p, nsig, xexp);
}

s16 _Ldunscale(s16 *pex, f64 *px) {
    u16 *ps = (u16 *)px;
    s16 xchar = (ps[0] & 0x7FF0) >> 4;

    if (xchar == 0x7FF) {
        *pex = 0;
        return (ps[0] & 0xF) || ps[1] || ps[2] || ps[3] ? 2 : 1;
    } else if (xchar > 0) {
        ps[0] = (ps[0] & ~0x7FF0) | 0x3FF0;
        *pex = xchar - 0x3FE;
        return -1;
    } else if (xchar < 0) {
        return 2;
    } else {
        *pex = 0;
        return 0;
    }
}


void _Genld(Pft_ *px, char code, u8 *p, s16 nsig, s16 xexp) {
    const u8 point = '.';

    if (nsig <= 0) {
        nsig = 1, p = D_80101DD0;
    }
    if (code == 'f' || (code == 'g' || code == 'G') && xexp >= -4 && xexp < px->prec) {
        xexp++;
        if (code != 'f') {
            if (((px->flags & 8) == 0) && nsig < px->prec) {
                px->prec = nsig;
            }
            if ((px->prec -= xexp) < 0) {
                px->prec = 0;
            }
        }
        if (xexp <= 0) {
            px->s[px->n1++] = '0';
            if (px->prec > 0 || (px->flags & 8)) {
                px->s[px->n1++] = point;
            }
            if (px->prec < -xexp) {
                xexp = -px->prec;
            }
            px->nz1 = -xexp;
            px->prec += xexp;
            if (px->prec < nsig) {
                nsig = px->prec;
            }
            memcpy(&px->s[px->n1], p, px->n2 = nsig);
            px->nz2 = px->prec - nsig;
        } else if (nsig < xexp) {
            memcpy(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = xexp - nsig;
            if (px->prec > 0 || (px->flags & 8)) {
                px->s[px->n1] = point;
                px->n2++;
            }
            px->nz2 = px->prec;
        } else {
            memcpy(&px->s[px->n1], p, xexp);
            px->n1 += xexp;
            nsig -= xexp;
            if (px->prec > 0 || (px->flags & 8)) {
                px->s[px->n1++] = point;
            }
            if (px->prec < nsig) {
                nsig = px->prec;
            }
            memcpy(&px->s[px->n1], &p[xexp], nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
    } else {
        if (code == 'g' || code == 'G') {
            if (nsig < px->prec) {
                px->prec = nsig;
            }
            if (--px->prec < 0) {
                px->prec = 0;
            }
            code = (code == 'g') ? 'e' : 'E';
        }
        px->s[px->n1++] = *p++;
        if (px->prec > 0 || (px->flags & 8)) {
            px->s[px->n1++] = point;
        }
        if (px->prec > 0) {
            if (px->prec < --nsig) {
                nsig = px->prec;
            }
            memcpy(&px->s[px->n1], p, nsig);
            px->n1 += nsig;
            px->nz1 = px->prec - nsig;
        }
        p = &px->s[px->n1];
        *p++ = code;
        if (xexp >= 0) {
            *p++ = '+';
        } else {
            *p++ = '-';
            xexp = -xexp;
        }
        if (xexp >= 100) {
            if (xexp >= 1000) {
                *p++ = (xexp / 1000) + '0', xexp %= 1000;
            }
            *p++ = (xexp / 100) + '0', xexp %= 100;
        }
        *p++ = (xexp / 10) + '0', xexp %= 10;
        *p++ = xexp + '0';
        px->n2 = (u32)p - ((u32)px->s + px->n1);
    }
    if ((px->flags & 0x14) == 0x10) {
        s32 n = px->n0 + px->n1 + px->nz1 + px->n2 + px->nz2;

        if (n < px->width) {
            px->nz0 = px->width - n;
        }
    }
}
