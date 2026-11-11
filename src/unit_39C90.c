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
s16 func_8009A9A0(Vec3f_);
void func_80099C94(u8 *n, u8 *o);
void *Memory_Alloc(s32 size);
void func_800996B8(u8 *, u8 *);
void func_8009A4E4(u8 *);
void func_800A3978(u8 *, u16, s32);

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


INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A4E4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A770);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A9A0);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009ABC8);

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

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3978);

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
