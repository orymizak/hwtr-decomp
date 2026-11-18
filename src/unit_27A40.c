#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { u8 ch; s8 k; } Kern2_;
typedef struct { Kern2_ *list; u8 n; u8 ch; u8 has; u8 pad; } Kern1_;
typedef struct { Kern1_ *tbl; u16 n; u8 pad[2]; } KernFont_;
typedef struct { u8 flag; u8 p1[3]; char *str; } Ln74_;
typedef struct { Ln74_ *lines; s32 a4; s32 a8; s32 top; s32 bottom; s32 start; f32 pos; s32 count; } Scr74_;
void func_80087260(char *, s32, s32, s32, s32);
f32 Menu_DeltaSeconds(u8);
extern u8 D_80116C70[];
extern s32 (*sysFileLoad)(char *, void *);
extern u32 (*sysFileGetSize)(char *);
extern u8 *gLangText[];
extern char D_800FFAC0[];
extern KernFont_ D_800F2800[];
extern s32 D_800F2808;
extern u8 D_80140F90;
extern u8 D_80145568;
extern u8 D_8014C0A8;
extern u8 D_80141698;
u8 Sys_InitTable(void);
u8 Input_InitTable(void);
u8 func_800610DC(void);
u8 func_80060458(void);
s32 func_80060840();
s32 func_80061334();
s32 Input_ClearTable();
s32 Sys_ClearTable();



u8 Lang_Load(char *name) {
    char path[0x100];
    u32 size;
    u32 n;
    u8 *buf;
    u8 *p;
    u32 i;

    sprintf(path, D_800FFAC0, name);
    size = sysFileGetSize(path);
    if (size == 0) {
        return 0;
    }
    if (size > 0x1800) {
        return 0;
    }
    buf = D_80116C70;
    if (buf == NULL) {
        return 0;
    }
    n = sysFileLoad(path, buf);
    if (n != size) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            buf[i] = 0;
        }
    }
    p = D_80116C70;
    for (i = 0; i < 300; i++) {
        gLangText[i] = p;
        while (*p != 0) {
            p++;
        }
        if (i < 299) {
            while (*p == 0) {
                p++;
            }
        }
    }
    return 1;
}

void func_80086F78(void) {

}

s8 func_80086F80(u8 font, u8 a, u8 b) {
    s16 lo;
    s16 hi;
    s16 mid;
    s16 f1;
    s16 f2;
    Kern1_ *t;
    s8 r;

    lo = 0;
    f1 = 0xFF;
    f2 = 0xFF;
    r = 0;
    if (D_800F2808 < font) {
        return -0x80;
    }
    hi = D_800F2800[font].n;
    t = D_800F2800[font].tbl;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (a == t[mid].ch) {
            f1 = mid;
            break;
        }
        if (t[mid].ch < a) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    if (f1 != 0xFF) {
        if (t[f1].has == 0) {
            return r;
        }
        lo = 0;
        hi = t[f1].n;
        while (lo <= hi) {
            mid = (lo + hi) / 2;
            if (b == t[f1].list[mid].ch) {
                f2 = mid;
                break;
            }
            if (t[f1].list[mid].ch < b) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (f2 != 0xFF) {
            return t[f1].list[f2].k;
        }
    }
    return r;
}

s32 func_8008711C(u8 font, u8 a, u8 pct) {
    s16 lo;
    s16 hi;
    s16 mid;
    s16 f1;
    Kern1_ *t;
    Kern2_ *k;
    u16 i;

    lo = 0;
    f1 = 0xFF;
    if (D_800F2808 < font) {
        return 0;
    }
    hi = D_800F2800[font].n;
    t = D_800F2800[font].tbl;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (a == t[mid].ch) {
            f1 = mid;
            break;
        }
        if (t[mid].ch < a) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    if (f1 != 0xFF) {
        t[f1].has = 1;
        k = t[f1].list;
        for (i = 0; i < t[f1].n; i++, k++) {
            k->k = k->k * pct / 100;
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_27A40", func_80087260);

void func_8008744C(Scr74_ *s) {
    s->pos = s->start;
}

void func_8008745C(Scr74_ *s, u8 fast) {
    s32 i;
    s32 y;
    s32 drawn;

    drawn = 0;
    y = 0;
    for (i = 0; i < s->count; i++) {
        y = (s32)s->pos + i * 20;
        if (s->top < y && y < s->bottom) {
            func_80087260(s->lines[i].str, s->a4, s->a8, y, s->lines[i].flag == 0);
            drawn++;
        }
    }
    if (drawn == 0 && y < 0) {
        s->pos = s->start;
    } else if (fast) {
        s->pos -= Menu_DeltaSeconds(5) * 120.0f;
    } else {
        s->pos -= Menu_DeltaSeconds(5) * 30.0f;
    }
}

void func_800876D4(void **tbl, u32 size);
s32 func_800875A0(void) {
    func_800876D4((void **)&D_80140F90, 0xFC);
    if (Sys_InitTable() == 0) {
        return 0;
    }
    func_800876D4((void **)&D_80145568, 0x4C);
    if (Input_InitTable() != 0) {
        func_800876D4((void **)&D_8014C0A8, 0xAC);
        if (func_800610DC() != 0) {
            func_800876D4((void **)&D_80141698, 0x124);
            if (func_80060458() != 0) {
                return 1;
            }
            func_80061334();
        }
        Input_ClearTable();
    }
    Sys_ClearTable();
    return 0;
}


s32 func_80087660(void) {
    register s32 failed;
    s8 n;

    failed = (u8)func_80060840() == 0;
    n = failed;
    if (!(u8)func_80061334()) {
        n = failed + 1;
    }
    if (!(u8)Input_ClearTable()) {
        n++;
    }
    if (!(u8)Sys_ClearTable()) {
        n++;
    }
    return (u8)n == 0;
}

s32 func_8008770C(void);
void func_800876D4(void **tbl, u32 size) {
    s32 i;
    s32 n = size >> 2;

    for (i = 0; i < n; i++) {
        *tbl++ = func_8008770C;
    }
}


s32 func_8008770C(void) {
    return 0;
}
