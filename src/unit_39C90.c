#include "common.h"
#include "include_asm.h"
extern s32 D_8011B330;
extern s32 D_8011C820;
extern s32 D_8011C824;
extern s32 D_8011C828;
extern u8 D_8011C7A0[];
extern void (*D_801416F8)(void *);
s32 func_8008182C(void);
void func_800A3CD8(void);
u8 func_80099090(u8 *);
void func_800C9AF0(s32 n, s32 b);
extern s32 D_8011B33C;
extern s32 D_8011B344;
extern s32 D_8011B348;
extern s32 D_8011B34C;
void func_800C9B40(void);
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } V16_;
typedef struct { u8 pad[0x18]; V12_ v[6]; V16_ q[1]; } Cam41_;
typedef struct Nd4_ { s32 a; struct Nd4_ *prev; struct Nd4_ *next; u16 cnt; } Nd4_;
typedef struct { u8 pad[0x4C]; void *a; void *b; void *c; u8 pad2[0xC]; s32 id; } Obj95_;
typedef struct Lnk95_ { Obj95_ *obj; s32 x; struct Lnk95_ *next; } Lnk95_;
extern Lnk95_ *D_8011B338;
s32 func_8009A770(s32 *);
typedef struct { u8 pad[0x110]; u8 b110; } Obj60_;
typedef struct { u8 pad[8]; s32 w8; } Obj70_;
typedef struct { u8 pad[0x60]; Obj60_ *p60; u8 pad2[0xC]; Obj70_ *p70; } ObjA2_;
typedef struct LnkA2_ { ObjA2_ *obj; s32 x; struct LnkA2_ *next; } LnkA2_;
extern LnkA2_ *D_8011B340;
void func_800A25EC(ObjA2_ *);
typedef struct { u8 pad[0x40]; void *a; void *b; void *c; } Res40_;
void Memory_Free(void *);
typedef struct Nd3_ { s32 a; struct Nd3_ *prev; struct Nd3_ *next; } Nd3_;
typedef struct { f32 x; f32 y; f32 z; } Vec3f_;
typedef struct { u8 pad0[0xA]; u8 n; u8 padB; u16 first; u8 padE[2]; u8 list[4]; } Wp14_;
typedef struct { u8 pad[8]; u16 to; u8 flag; u8 padB; } Lk0C_;
typedef struct { u16 flags; u8 pad2[3]; s8 x; s8 y; s8 z; u8 pad8[2]; u8 n; u8 padB; u16 first; u8 padE[6]; } WpP_;
typedef struct { s16 nx; s16 ny; s16 nz; s16 d; u16 to; u8 flag; u8 padB; } PlP_;
typedef struct { f32 a; f32 b; f32 c; f32 d; s16 e; u16 f; } PlF_;
extern u32 D_8011C830[];
void func_800A780C(u8, u8, s32);
void func_800979F4(u8 *);
void func_8009751C(u8 *, u16, u8);
void func_800C9B84(u16);
void func_800A3978(u8 *o, u16 idx, s32 arg2);
u16 func_8009A9A0(Vec3f_ pos);
void func_8009A4E4(u8 *o);
void func_80099C94(u8 *n, u8 *o);
void *Memory_Alloc(s32 size);
void func_800996B8(u8 *, u8 *);

extern void *D_8011E860;
extern void *D_8011B334;

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099090);

s32 func_8009930C(u8 *o) {
    s32 v = *(s32 *)(o + 0x1C);

    D_8011B330 = 100;
    D_8011C820 = v;
    D_8011C824 = func_8008182C();
    D_8011C828 = o[0x1A];
    func_800A3CD8();
    if (func_80099090(o) == 0) {
        return 0;
    }
    D_801416F8(D_8011B334);
    func_800C9AF0(*(s32 *)((u8 *)D_8011B334 + 0x10), *(s32 *)((u8 *)D_8011B334 + 0xD0));
    memset(D_8011C7A0, 0, 0x80);
    return 1;
}


void func_800A3D64(Nd3_ **list);
s32 func_800993B8(void) {
    func_800C9B40();
    func_800A3D64((Nd3_ **)&(*(s32 *)&D_8011B338));
    func_800A3D64((Nd3_ **)&D_8011B33C);
    func_800A3D64((Nd3_ **)&(*(s32 *)&D_8011B340));
    func_800A3D64((Nd3_ **)&D_8011B344);
    func_800A3D64((Nd3_ **)&D_8011B348);
    func_800A3D64((Nd3_ **)&D_8011B34C);
    Memory_Free(D_8011B334);
    D_8011B334 = NULL;
    return 1;
}


void func_80099434(u8 *o) {
    u8 *n = Memory_Alloc(0x88);
    u8 i;

    memset(n, 0, 0x88);
    if (*(s32 *)(o + 0x6D8) == 2) {
        func_800996B8(n + 0xC, o);
        *(s32 *)(n + 0x5C) = 3;
    } else if (*(s32 *)(o + 0x6D8) == 1) {
        func_80099C94(n + 0xC, o);
        *(s32 *)(n + 0x5C) = 4;
    }
    *(u8 **)(n + 0x60) = o + 0x2C;
    *(u8 **)(n + 0x64) = o;
    func_8009A4E4(n);
    for (i = 0; i < n[0x48]; i++) {
        func_800A3978(n, (*(u16 **)(n + 0x54))[i], 1);
    }
    *(u8 **)(o + 0x758) = n;
}

void func_8009951C(s32 id) {
    Lnk95_ *n = D_8011B338;
    Obj95_ *o;

    while (n != NULL) {
        o = n->obj;
        if (o->id == id) {
            break;
        }
        n = n->next;
    }
    if (n != NULL) {
        func_8009A770((s32 *)o);
        Memory_Free(o->a);
        Memory_Free(o->b);
        Memory_Free(o->c);
        Memory_Free(o);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099594);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800996B8);

void func_80099C94(u8 *n, u8 *o) {
    s32 zero[3];
    Vec3f_ v;
    Vec3f_ p;
    s16 id;
    u8 i;

    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    v = *(Vec3f_ *)(o + 0x5E8);
    v.x *= 0.5f;
    v.y *= 0.5f;
    v.z *= 0.5f;
    *(Vec3f_ *)(n + 0xC) = v;
    n[0x3C] = 1;
    *(void **)(n + 0x40) = Memory_Alloc(0xC);
    *(void **)(n + 0x44) = Memory_Alloc(n[0x3C] * 0xC);
    memset(*(void **)(n + 0x40), 0, n[0x3C] * 0xC);
    memset(*(void **)(n + 0x44), 0, n[0x3C] * 0xC);
    (*(s32 **)(n + 0x40))[0] = 0;
    (*(s32 **)(n + 0x40))[1] = 0;
    (*(s32 **)(n + 0x40))[2] = 0;
    *(void **)(n + 0x48) = Memory_Alloc(n[0x3C] * 2);
    p.x = *(f32 *)(o + 0x98) + *(f32 *)(o + 0x8C);
    p.y = *(f32 *)(o + 0x9C) + *(f32 *)(o + 0x90);
    p.z = *(f32 *)(o + 0xA0) + *(f32 *)(o + 0x94);
    id = func_8009A9A0(p);
    for (i = 0; i < n[0x3C]; i++) {
        (*(s16 **)(n + 0x48))[i] = id;
    }
    *(f32 *)(n + 0x4C) = sqrtf(*(f32 *)(n + 0xC) * *(f32 *)(n + 0xC) + *(f32 *)(n + 0x10) * *(f32 *)(n + 0x10) + *(f32 *)(n + 0x14) * *(f32 *)(n + 0x14));
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099E78);

void func_8009A488(Res40_ *r) {
    if (r->a != NULL) {
        Memory_Free(r->a);
    }
    if (r->b != NULL) {
        Memory_Free(r->b);
    }
    if (r->c != NULL) {
        Memory_Free(r->c);
    }
}


void func_800A3DB8(Nd4_ **list, s32 key);
void func_8009A4E4(u8 *o) {
    u32 i;
    u16 k;
    Wp14_ *w;
    Lk0C_ *l;
    u32 lo;
    u32 hi;

    for (i = 0; i < o[0x48]; i++) {
        k = (*(u16 **) (o + 0x54))[i];
        func_800A3DB8((Nd4_ **) ((u8 *) &(*(Wp14_ **) ((u8 *) D_8011B334 + 0xC0))[k] + 0x10), (s32) o);
        func_800A3DB8((Nd4_ **) (o + 0x70), k);
    }
    if (*(s32 *) (o + 0x5C) == 4) {
        w = &(*(Wp14_ **) ((u8 *) D_8011B334 + 0xC0))[**(u16 **) (o + 0x54)];
        l = &(*(Lk0C_ **) ((u8 *) D_8011B334 + 0xC8))[w->first];
        for (i = 0; i < w->n; i++, l++) {
            if (l->flag == 0) {
                ((void (*)(void *, void *, u16)) func_800A3DB8)((u8 *) &(*(Wp14_ **) ((u8 *) D_8011B334 + 0xC0))[l->to] + 0x10, o, l->to);
            }
        }
    }
    func_800A3DB8((Nd4_ **) &D_8011B338, (s32) o);
    if ((*(s32 *) (o + 0x5C) != 0) & (*(s32 *) (o + 0x5C) != 5)) {
        func_800A3DB8((Nd4_ **) &D_8011B33C, (s32) o);
    }
    if (*(s32 *) (o + 0x5C) == 3) {
        func_800A3DB8((Nd4_ **) &D_8011B340, (s32) o);
    }
    if (*(s32 *) (o + 0x5C) == 3 || *(s32 *) (o + 0x5C) == 4) {
        func_800A3DB8((Nd4_ **) &D_8011B344, (s32) o);
    }
    if (((*(s32 *) (o + 0x5C) == 1) | (*(s32 *) (o + 0x5C) == 3)) || *(s32 *) (o + 0x5C) == 6) {
        func_800A3DB8((Nd4_ **) &D_8011B348, (s32) o);
    }
    if (*(s32 *) (o + 0x5C) == 4) {
        func_800A3DB8((Nd4_ **) &D_8011B34C, (s32) o);
    }
    if ((*(s32 *) (o + 0x5C) == 0) | (*(s32 *) (o + 0x5C) == 5)) {
        lo = 0x20;
        hi = 0x80;
    } else {
        lo = 0;
        hi = 0x20;
    }
    for (i = lo; i < hi; i++) {
        if (D_8011C7A0[i] == 0) {
            *(s32 *) o = i;
            D_8011C7A0[i] = 1;
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A770);

u16 func_8009A9A0(Vec3f_ pos) {
    Vec3f_ p;
    Vec3f_ d;
    PlF_ pl;
    f32 best;
    f32 m;
    f32 dist;
    WpP_ *w;
    PlP_ *l;
    u16 i;
    u16 j;
    u16 bi;

    best = -60000.0f;
    bi = 0;
    m = 0.0f;
    w = *(WpP_ **)((u8 *)D_8011B334 + 0xC0);
    for (i = 0; i < *(u32 *)D_8011B334; w++, i++) {
        if (!(w->flags & 0x800)) {
            p.x = w->x << 8;
            p.y = w->y << 8;
            p.z = w->z << 8;
            d.x = pos.x - p.x;
            d.y = pos.y - p.y;
            d.z = pos.z - p.z;
            l = &(*(PlP_ **)((u8 *)D_8011B334 + 0xC8))[w->first];
            for (j = 0; j < w->n; l++, j++) {
                pl.a = l->nx / 4096.0f;
                pl.b = l->ny / 4096.0f;
                pl.c = l->nz / 4096.0f;
                pl.d = l->d;
                pl.e = l->flag;
                pl.f = l->to;
                dist = d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d;
                if ((j == 0) | (dist < m)) {
                    m = dist;
                }
            }
            if (0.0f < m) {
                return i;
            }
            if (best < m) {
                best = m;
                bi = i;
            }
        }
    }
    return bi;
}

typedef struct { f32 x; f32 y; f32 z; } V3AB_;
typedef struct {
    u8 pad0[4]; u32 flags; u8 pad8[0x4B0]; u8 b4B8; u8 pad4B9[0x205]; u8 b6BE; u8 b6BF; u8 pad6C0[0x50]; u8 b710; u8 pad711[0xF];
    V3AB_ n720; u8 pad72C[0x44]; s32 w770;
} PlAB_;
typedef struct { u8 pad0[4]; u32 flags; u8 pad8[0x54]; s32 type; u8 pad60[4]; PlAB_ *pl; u8 pad68[0xC]; s32 frame; V3AB_ pos; } ObjAB_;
typedef struct { ObjAB_ *obj; u8 pad4[0xC]; V3AB_ pos; u8 idx; u8 pad1D[3]; } EntAB_;
typedef struct { ObjAB_ *a; ObjAB_ *b; u8 pad8[0x18]; } PairAB_;
extern u16 D_8011B350;
extern EntAB_ D_8011B358[];
extern u16 D_8011B558;
extern PairAB_ D_8011B560[];
extern u8 D_800F38E4[];
extern void (*D_8014177C)(EntAB_ *, f32);
void func_8009B0AC(void);
void func_8009B56C(void);
void func_8009D574(void);
void func_8008DF18(EntAB_ *);
void func_80091DC4(PlAB_ *);
void func_80098D84(EntAB_ *);
void func_800A66C0(EntAB_ *);
void func_80096954(PlAB_ *, EntAB_ *);
s32 func_800A76D8(void);
f32 func_800AD774(EntAB_ *, f32);
void func_8008E0BC(PairAB_ *);
void func_80098E84(PairAB_ *, s32);
void func_800A6850(PairAB_ *, s32);
void func_800ADE1C(PairAB_ *);
void func_800AEFF8(void);
void func_8009ABC8(void) {
    u32 i;
    EntAB_ *e;
    PairAB_ *pr;
    PlAB_ *pl;
    PlAB_ *pa;
    PlAB_ *pb;
    u8 changed;
    u8 isRacer;
    u8 isCar;
    u8 flag;
    f32 f;
    f32 r;

    D_8011B558 = 0;
    D_8011B350 = 0;
    D_8011B330++;
    func_8009B0AC();
    func_8009B56C();
    func_8009D574();
    for (i = 0, e = D_8011B358; i < D_8011B350; e++, i++) {
        changed = e->obj->frame != D_8011B330;
        if (changed) {
            func_8008DF18(e);
        }
        isRacer = e->obj->type == 3;
        isCar = 0;
        if (isRacer || e->obj->type == 4) {
            isCar = 1;
        }
        flag = 0;
        if (isCar) {
            flag = e->obj->pl->b4B8;
        }
        pl = e->obj->pl;
        if (isRacer && !flag) {
            if (changed) {
                func_80091DC4(pl);
                if (pl->flags & 1) {
                    func_80098D84(e);
                }
            }
            func_800A66C0(e);
            if (pl->b710 && e->pos.x * pl->n720.x + e->pos.y * pl->n720.y + e->pos.z * pl->n720.z > 0.866f) {
                func_80096954(pl, e);
            }
            pl->w770 = func_800A76D8();
        }
        e->obj->frame = D_8011B330;
        e->obj->pos = e->pos;
        f = D_800F38E4[e->idx] / 100.0f;
        if (isCar && !isRacer && !flag) {
            f *= 4.0f;
        }
        r = func_800AD774(e, f);
        if (isRacer & changed) {
            D_8014177C(e, r);
        }
    }
    for (i = 0, pr = D_8011B560; i < D_8011B558; pr++, i++) {
        pa = NULL;
        if (pr->a->type == 3 || pr->a->type == 4) {
            pa = pr->a->pl;
        }
        pb = NULL;
        if (pr->b->type == 3 || pr->b->type == 4) {
            pb = pr->b->pl;
        }
        func_8008E0BC(pr);
        if (pa != NULL) {
            if (pa->flags & 1) {
                func_80098E84(pr, 0);
            }
            func_800A6850(pr, 0);
        }
        if (pb != NULL) {
            if (pb->flags & 1) {
                func_80098E84(pr, 1);
            }
            func_800A6850(pr, 1);
        }
        if (pa != NULL) {
            if (pa->b6BE) {
                pr->a->flags |= 0x20;
            }
            if (pa->b6BF) {
                pr->a->flags |= 0x40;
            }
        }
        if (pb != NULL) {
            if (pb->b6BE) {
                pr->b->flags |= 0x20;
            }
            if (pb->b6BF) {
                pr->b->flags |= 0x40;
            }
        }
        if ((pr->a->flags & 0x20) && (pr->b->flags & 0x20)) {
            pr->a->flags &= ~0x20;
            pr->b->flags &= ~0x20;
        }
        if ((pr->a->flags & 0x40) && (pr->b->flags & 0x40)) {
            pr->a->flags &= ~0x40;
            pr->b->flags &= ~0x40;
        }
        func_800ADE1C(pr);
        if (pa != NULL) {
            pr->a->flags &= ~0x60;
        }
        if (pb != NULL) {
            pr->b->flags &= ~0x60;
        }
        if (pa != NULL && pb != NULL && (pa->flags & 1) && (pb->flags & 1)) {
            func_800AEFF8();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009B0AC);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009B56C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009BAF0);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009C758);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009D574);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009D67C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009DBF8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009EC8C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009F7D4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A0C44);

void func_800A258C(void) {
    LnkA2_ *n;

    for (n = D_8011B340; n != NULL; n = n->next) {
        ObjA2_ *o = n->obj;

        if (o->p60->b110 == 0 && o->p70->w8 != 0) {
            func_800A25EC(o);
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A25EC);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A34B4);

void func_800A3978(u8 *o, u16 idx, s32 arg2) {
    u8 *w;
    u8 *c;
    u32 m;

    w = *(u8 **)((u8 *)D_8011B334 + 0xC0) + idx * 0x14;
    if (*(s32 *)(o + 0x5C) == 3 || *(s32 *)(o + 0x5C) == 4) {
        c = *(u8 **)(o + 0x64);
        if (c[0x4B8] == 0) {
            if (((*(u16 *)(w + 0)) & 1) && (u8)arg2 == 0) {
                func_800A780C(c[0], w[3], 1);
            }
            if ((*(u16 *)(w + 0)) & 0x10) {
                (*(s32 *)(c + 8)) |= 1;
            } else {
                (*(s32 *)(c + 8)) &= ~1;
            }
            if (((*(u16 *)(w + 0)) & 8) || (D_8011C820 & 0x80)) {
                (*(s32 *)(c + 8)) |= 2;
            } else {
                (*(s32 *)(c + 8)) &= ~2;
            }
            (*(s32 *)(c + 4)) &= ~0x1C;
            m = (*(u16 *)(w + 0)) & 0x9000;
            if (m == 0x9000) {
                (*(s32 *)(c + 4)) |= 0x10;
            } else if (m == 0x8000) {
                (*(s32 *)(c + 4)) |= 8;
            } else if (m == 0x1000) {
                (*(s32 *)(c + 4)) |= 4;
            }
            if ((*(u16 *)(w + 0)) & 0x40) {
                (*(s32 *)(c + 4)) |= 0x20;
            } else {
                (*(s32 *)(c + 4)) &= ~0x20;
            }
            if ((*(u16 *)(w + 0)) & 0x80) {
                (*(s32 *)(c + 4)) |= 0x40;
            } else {
                (*(s32 *)(c + 4)) &= ~0x40;
            }
            if ((*(u16 *)(w + 0)) & 4) {
                (*(s32 *)(c + 4)) |= 0x80;
            } else {
                (*(s32 *)(c + 4)) &= ~0x80;
            }
            if ((*(u16 *)(w + 0)) & 0x100) {
                (*(s32 *)(c + 4)) |= 0x100;
            } else {
                (*(s32 *)(c + 4)) &= ~0x100;
            }
            if ((*(u16 *)(w + 0)) & 0x200) {
                (*(s32 *)(c + 4)) |= 0x200;
            } else {
                (*(s32 *)(c + 4)) &= ~0x200;
            }
            if ((*(u16 *)(w + 0)) & 0x400) {
                (*(s32 *)(c + 4)) |= 0x4000;
            } else {
                (*(s32 *)(c + 4)) &= ~0x4000;
                (*(s32 *)(c + 0x474)) = (*(u16 *)(w + 8)) * 10;
                if ((u32) (*(s32 *)(c + 0x474)) < D_8011C830[c[0x479]]) {
                    (*(s32 *)(c + 0x474)) += D_8011C824;
                }
            }
            if ((*(u16 *)(w + 0)) & 0x2000) {
                (*(s32 *)(c + 4)) |= 0x400;
            } else {
                (*(f32 *)(c + 0x80)) = 0.0f;
                (*(f32 *)(c + 0x84)) = 0.0f;
                (*(s32 *)(c + 4)) &= ~0x400;
                (*(f32 *)(c + 0x88)) = -1.0f;
            }
            if ((*(u16 *)(w + 0)) & 0x20) {
                (*(f32 *)(c + 0x7C)) = (*(u16 *)(w + 2)) / 256.0f * 386.0f;
            } else {
                (*(f32 *)(c + 0x7C)) = 386.0f;
            }
            if (((*(u16 *)(w + 0)) & 0x4000) && (*(s32 *)(c + 0x6D8)) == 2) {
                if ((*(u16 *)(w + 0)) & 0x800) {
                    func_800979F4(c);
                } else {
                    func_8009751C(c, (*(u16 *)(w + 2)), w[4]);
                }
            }
            if ((*(u16 *)(w + 0)) & 2) {
                func_800C9B84((*(u16 *)(w + 2)));
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3CD8);

void func_800A3D64(Nd3_ **list) {
    Nd3_ *n = *list;

    while (n != NULL) {
        Nd3_ *c = n;
        Nd3_ *old;

        n = n->next;
        old = (Nd3_ *)D_8011E860;
        c->prev = NULL;
        D_8011E860 = c;
        c->next = old;
        old->prev = c;
    }
    memset(list, 0, 4);
}


void func_800A3DB8(Nd4_ **list, s32 key) {
    Nd4_ *n;
    Nd4_ *nx;

    for (n = *list; n != NULL; n = n->next) {
        if (n->a == key) {
            break;
        }
    }
    if (n == NULL) {
        n = (Nd4_ *)D_8011E860;
        nx = n->next;
        nx->prev = NULL;
        n->a = key;
        n->prev = NULL;
        n->next = *list;
        n->cnt = 0;
        *list = n;
        D_8011E860 = nx;
        if (n->next != NULL) {
            n->next->prev = n;
        }
    }
    n->cnt++;
}


void func_800A3E30(void **arg0, s32 arg1) {
    u16 temp_v0;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_v0_2;
    void *var_v1;

    var_v1 = *arg0;
    if (var_v1 != NULL) {
loop_1:
        if (*(s32 *)((u8 *)var_v1 + 0x0) != arg1) {
            var_v1 = *(s32 *)((u8 *)var_v1 + 0x8);
            if (var_v1 != NULL) {
                goto loop_1;
            }
        }
        if (var_v1 != NULL) {
            temp_v0 = *(u16 *)((u8 *)var_v1 + 0xC) - 1;
            *(u16 *)((u8 *)var_v1 + 0xC) = temp_v0;
            if (!(temp_v0 & 0xFFFF)) {
                temp_a1 = *(s32 *)((u8 *)var_v1 + 0x8);
                if (temp_a1 != NULL) {
                    *(s32 *)((u8 *)temp_a1 + 0x4) = (void *) *(s32 *)((u8 *)var_v1 + 0x4);
                }
                temp_a1_2 = *(s32 *)((u8 *)var_v1 + 0x4);
                if (temp_a1_2 != NULL) {
                    *(s32 *)((u8 *)temp_a1_2 + 0x8) = (void *) *(s32 *)((u8 *)var_v1 + 0x8);
                }
                if (var_v1 == *arg0) {
                    *arg0 = *(s32 *)((u8 *)var_v1 + 0x8);
                }
                temp_v0_2 = D_8011E860;
                *(s32 *)((u8 *)var_v1 + 0x4) = NULL;
                D_8011E860 = var_v1;
                *(s32 *)((u8 *)var_v1 + 0x8) = temp_v0_2;
                *(s32 *)((u8 *)temp_v0_2 + 0x4) = var_v1;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3ED4);

void func_800A4120(u8 i, V12_ *v, V16_ *q) {
    if (D_8011B334 != NULL) {
        *v = ((Cam41_ *)D_8011B334)->v[i];
        *q = ((Cam41_ *)D_8011B334)->q[i];
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4198);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4EE8);

s32 func_800A5894(void) {
    return *(s32 *)((u8 *)D_8011B334 + 0x14);
}

s32 func_800A58A4(s32 arg0) {
    return *(s32 *)((u8 *)D_8011B334 + 0xD4) + (arg0 * 0x18);
}
