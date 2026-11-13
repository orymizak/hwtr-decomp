#include "common.h"
#include "include_asm.h"
typedef struct { u8 b; u8 pad[3]; s32 w[6]; } E1C8_;
extern E1C8_ D_800F316C[];
extern u8 (*D_8014169C)(u8);
extern s32 D_801410E0[];
extern char D_8010080C[];
s32 sprintf(char *, const char *, ...);
typedef struct Lnk89_ { s32 *obj; s32 x; struct Lnk89_ *next; } Lnk89_;
typedef struct { u8 *def; s32 t; u8 pad8; u8 hold; } Tmr89_;
typedef struct { s32 pad0; u8 id; u8 pad5[3]; s32 on; s32 t; u8 fired; u8 pad11[7]; } Evt18_;
typedef struct { f32 x, y, z; } V3_8010;
typedef struct { u8 id; u8 p1[0x3]; s32 flags; u8 p8[0x6C]; f32 scale; f32 inv; u8 p7C[0x28]; V3_8010 vel; u8 pB0[0x10]; f32 rot[9]; u8 pE4[0x31C]; f32 k400; f32 k404; u8 p408[0x24]; f32 k42C; u8 p430[0x98]; u8 col[4]; f32 k4CC; f32 k[0x18]; f32 k530; u8 p534[0x8]; f32 k53C; u8 p540[0x17C]; u8 b[5]; u8 p6C1[0xB]; u8 n6CC; u8 p6CD[0xB]; s32 state; } C_8010;
typedef struct { u8 p0[0x34]; s32 flags; f32 f[0x18]; V3_8010 v; f32 fA4; u8 col[4]; } P_8010;
extern u16 D_800F302A;
extern u16 D_800F302C;
extern u16 D_800F302E;
extern u16 D_800F3030;
extern u16 D_800F3032;
extern u16 D_800F3034;
void func_800AC6EC(u8, s32);
void func_800B0614(u8, s32);
void func_80088010(u8 *arg0, u8 *arg1);
extern void (*D_8014C0A8)(s32, s32, f32, f32);
void func_800A8268(Lnk89_ **, s32 *);
void *Memory_Alloc(u32 size);
u8 *func_80089B10(u8 *);
s32 func_80089FC4(u8 *);
extern Evt18_ *D_800F3020;
extern u32 D_800F3024;
extern void (*D_80141714)(u8, s32, f32);
u8 *func_800960D8(u8);
s32 func_800A76D8(void);
extern Lnk89_ **D_80118470[];
void func_800A82F8(Lnk89_ **, s32 *);
void Memory_Free(void *);
s32 func_80095970(void *);
extern s32 (*D_80141768)(u8,s32);



void func_80088010(u8 *arg0, u8 *arg1) {
    P_8010 *p = (P_8010 *)arg0;
    C_8010 *c = (C_8010 *)arg1;
    V3_8010 v;
    V3_8010 d;
    V3_8010 t;

    if (c->state != 2) {
        return;
    }
    v.x = p->v.x * c->scale;
    v.y = p->v.y * c->scale;
    v.z = p->v.z * c->scale;
    t.x = c->rot[0] * v.x + c->rot[1] * v.y + c->rot[2] * v.z;
    t.y = c->rot[3] * v.x + c->rot[4] * v.y + c->rot[5] * v.z;
    t.z = c->rot[6] * v.x + c->rot[7] * v.y + c->rot[8] * v.z;
    d = t;
    c->vel.x = d.x + c->vel.x;
    c->vel.y = d.y + c->vel.y;
    c->vel.z = d.z + c->vel.z;
    c->k4CC *= p->fA4;
    c->col[0] += p->col[0];
    c->col[1] += p->col[1];
    c->col[2] += p->col[2];
    c->col[3] += p->col[3];
    c->k[0] *= p->f[0];
    c->k[1] *= p->f[1];
    c->k[2] *= p->f[2];
    c->k[3] *= p->f[3];
    c->k[4] *= p->f[4];
    c->k[5] *= p->f[5];
    c->k[6] *= p->f[6];
    c->k[7] *= p->f[7];
    c->k[8] *= p->f[8];
    c->k[9] *= p->f[9];
    c->k[10] *= p->f[13];
    c->k[11] *= p->f[14];
    c->k[13] *= p->f[15];
    c->k[14] *= p->f[16];
    c->k[16] *= p->f[17];
    c->k[17] *= p->f[18];
    c->k[19] *= p->f[19];
    c->k[20] *= p->f[20];
    c->k[22] *= p->f[11];
    c->k530 *= p->f[10];
    c->k[23] *= p->f[12];
    c->k400 *= p->f[21];
    c->k404 *= p->f[22];
    c->k42C *= p->f[23];
    if (p->flags & 8) {
        c->b[1] = 1;
    }
    if (p->flags & 0x10) {
        c->b[2] = 1;
        c->k53C *= 10.0f;
    }
    if (p->flags & 0x20) {
        c->b[3] = 1;
    }
    if (p->flags & 0x40) {
        c->b[4] = 1;
    }
    if (p->flags & 1) {
        c->b[0] = 1;
    }
    if (p->flags & 2) {
        D_80141768(c->id, 1);
    }
    if ((p->flags & 4) && c->n6CC < 10) {
        func_800AC6EC(c->id, 1);
        c->n6CC++;
    }
    if (c->flags & 1) {
        if (p->flags & 0x01000000) {
            if (c->id == 0) {
                D_800F302E = D_800F302A;
            } else if (c->id == 1) {
                D_800F3030 = D_800F302A;
            }
        }
        if (p->flags & 0x02000000) {
            if (c->id == 0) {
                D_800F3032 = D_800F302C;
            } else if (c->id == 1) {
                D_800F3034 = D_800F302C;
            }
        }
        if (p->flags & 0x04000000) {
            func_800B0614(c->id, 3);
        }
        if (p->flags & 0x08000000) {
            func_800B0614(c->id, 4);
        }
        if (p->flags & 0x10000000) {
            func_800B0614(c->id, 5);
        }
        if (p->flags & 0x20000000) {
            func_800B0614(c->id, 6);
        }
        if (p->flags & 0x40000000) {
            func_800B0614(c->id, 7);
        }
        if (p->flags & 0x80000000) {
            func_800B0614(c->id, 8);
        }
    }
    func_80095970(c);
    c->scale = c->k[0];
    c->inv = 1.0f / c->k[0];
}

void func_80088558(void *arg0, void *arg1) {
    if ((*(s32 *)((u8 *)(arg1) + 0x6D8)) == 2) {
        (*(f32 *)((u8 *)(arg1) + 0x4CC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4CC)) / (*(f32 *)((u8 *)(arg0) + 0xA4)));
        (*(u8 *)((u8 *)(arg1) + 0x4C8)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4C8)) - (*(u8 *)((u8 *)(arg0) + 0xA8)));
        (*(u8 *)((u8 *)(arg1) + 0x4C9)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4C9)) - (*(u8 *)((u8 *)(arg0) + 0xA9)));
        (*(u8 *)((u8 *)(arg1) + 0x4CA)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4CA)) - (*(u8 *)((u8 *)(arg0) + 0xAA)));
        (*(u8 *)((u8 *)(arg1) + 0x4CB)) = (u8) ((*(u8 *)((u8 *)(arg1) + 0x4CB)) - (*(u8 *)((u8 *)(arg0) + 0xAB)));
        (*(f32 *)((u8 *)(arg1) + 0x4D0)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4D0)) / (*(f32 *)((u8 *)(arg0) + 0x38)));
        (*(f32 *)((u8 *)(arg1) + 0x4D4)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4D4)) / (*(f32 *)((u8 *)(arg0) + 0x3C)));
        (*(f32 *)((u8 *)(arg1) + 0x4D8)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4D8)) / (*(f32 *)((u8 *)(arg0) + 0x40)));
        (*(f32 *)((u8 *)(arg1) + 0x4DC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4DC)) / (*(f32 *)((u8 *)(arg0) + 0x44)));
        (*(f32 *)((u8 *)(arg1) + 0x4E0)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4E0)) / (*(f32 *)((u8 *)(arg0) + 0x48)));
        (*(f32 *)((u8 *)(arg1) + 0x4E4)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4E4)) / (*(f32 *)((u8 *)(arg0) + 0x4C)));
        (*(f32 *)((u8 *)(arg1) + 0x4E8)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4E8)) / (*(f32 *)((u8 *)(arg0) + 0x50)));
        (*(f32 *)((u8 *)(arg1) + 0x4EC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4EC)) / (*(f32 *)((u8 *)(arg0) + 0x54)));
        (*(f32 *)((u8 *)(arg1) + 0x4F0)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4F0)) / (*(f32 *)((u8 *)(arg0) + 0x58)));
        (*(f32 *)((u8 *)(arg1) + 0x4F4)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4F4)) / (*(f32 *)((u8 *)(arg0) + 0x5C)));
        (*(f32 *)((u8 *)(arg1) + 0x4F8)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4F8)) / (*(f32 *)((u8 *)(arg0) + 0x6C)));
        (*(f32 *)((u8 *)(arg1) + 0x4FC)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x4FC)) / (*(f32 *)((u8 *)(arg0) + 0x70)));
        (*(f32 *)((u8 *)(arg1) + 0x504)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x504)) / (*(f32 *)((u8 *)(arg0) + 0x74)));
        (*(f32 *)((u8 *)(arg1) + 0x508)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x508)) / (*(f32 *)((u8 *)(arg0) + 0x78)));
        (*(f32 *)((u8 *)(arg1) + 0x510)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x510)) / (*(f32 *)((u8 *)(arg0) + 0x7C)));
        (*(f32 *)((u8 *)(arg1) + 0x514)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x514)) / (*(f32 *)((u8 *)(arg0) + 0x80)));
        (*(f32 *)((u8 *)(arg1) + 0x51C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x51C)) / (*(f32 *)((u8 *)(arg0) + 0x84)));
        (*(f32 *)((u8 *)(arg1) + 0x520)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x520)) / (*(f32 *)((u8 *)(arg0) + 0x88)));
        (*(f32 *)((u8 *)(arg1) + 0x528)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x528)) / (*(f32 *)((u8 *)(arg0) + 0x64)));
        (*(f32 *)((u8 *)(arg1) + 0x530)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x530)) / (*(f32 *)((u8 *)(arg0) + 0x60)));
        (*(f32 *)((u8 *)(arg1) + 0x52C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x52C)) / (*(f32 *)((u8 *)(arg0) + 0x68)));
        (*(f32 *)((u8 *)(arg1) + 0x400)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x400)) / (*(f32 *)((u8 *)(arg0) + 0x8C)));
        (*(f32 *)((u8 *)(arg1) + 0x404)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x404)) / (*(f32 *)((u8 *)(arg0) + 0x90)));
        (*(f32 *)((u8 *)(arg1) + 0x42C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x42C)) / (*(f32 *)((u8 *)(arg0) + 0x94)));
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 8) {
            (*(u8 *)((u8 *)(arg1) + 0x6BD)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 0x10) {
            (*(u8 *)((u8 *)(arg1) + 0x6BE)) = 0;
            (*(f32 *)((u8 *)(arg1) + 0x53C)) = (f32) ((*(f32 *)((u8 *)(arg1) + 0x53C)) / 10.0f);
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 0x20) {
            (*(u8 *)((u8 *)(arg1) + 0x6BF)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 0x40) {
            (*(u8 *)((u8 *)(arg1) + 0x6C0)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 1) {
            (*(u8 *)((u8 *)(arg1) + 0x6BC)) = 0;
        }
        if ((*(s32 *)((u8 *)(arg0) + 0x34)) & 2) {
            D_80141768((*(u8 *)((u8 *)(arg1) + 0x0)), 0);
        }
        func_80095970(arg1);
        (*(f32 *)((u8 *)(arg1) + 0x74)) = (f32) (*(f32 *)((u8 *)(arg1) + 0x4D0));
        (*(f32 *)((u8 *)(arg1) + 0x78)) = (f32) (1.0f / (*(f32 *)((u8 *)(arg1) + 0x4D0)));
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80088810);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_800888E0);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80088B50);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_8008955C);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80089848);

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80089B10);

void func_80089F28(u8 *o) {
    Lnk89_ *n = *D_80118470[*o];
    s32 *x;

    while (n != NULL) {
        x = n->obj;
        n = n->next;
        func_80088558((void *)*x, o);
        func_800A82F8(D_80118470[*o], x);
        Memory_Free(x);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_80089FC4);

s32 func_8008AA9C(u8 *o, u8 *q) {
    u8 *d;
    Tmr89_ *t;
    Lnk89_ *l;
    Lnk89_ *m;
    s32 n;
    Tmr89_ *nt;
    s32 r;

    if (q[0x10] == 0) {
        return 0;
    }
    d = func_80089B10(q);
    if (d == NULL) {
        return 0;
    }

    l = *D_80118470[*o];
    while (l != NULL) {
        t = (Tmr89_ *) l->obj;
        l = l->next;
        func_80088558(t->def, o);
        func_800A82F8(D_80118470[*o], (s32 *) t);
        Memory_Free(t);
    }

    m = *D_80118470[*o];
    n = 0;
    while (m != NULL) {
        n += (((Tmr89_ *) m->obj)->hold != 0);
        m = m->next;
    }

    if (n == 0) {
        nt = Memory_Alloc(0xC);
        nt->t = func_800A76D8();
        nt->def = d;
        nt->hold = d[0xAC];
        nt->pad8 = *(s32 *) (d + 0x30);
        if (d[0xAC] != 0) {
            m = *D_80118470[*o];
            while (m != NULL) {
                t = (Tmr89_ *) m->obj;
                m = m->next;
                if (t->hold != 0) {
                    func_80088558(t->def, o);
                    func_800A82F8(D_80118470[*o], (s32 *) t);
                    Memory_Free(t);
                    break;
                }
            }
        }
        func_800A8268(D_80118470[*o], (s32 *) nt);
        if (nt->hold == 0) {
            func_80088010(d, func_800960D8(*o));
        }
        D_80141714(q[4], 0, 0.0f);
        r = func_80089FC4(d);
        *(s32 *) (o + 0x6DC) = r;
        if (r == 1) {
            *(s32 *) (o + 0x6DC) = 0;
        }
        if (*(s32 *) (o + 4) & 3) {
            D_8014C0A8(0x18, 0, 1.0f, 1.0f);
        }
    }
    *(s32 *) (q + 0xC) = func_800A76D8();
    q[0x10] = 0;
    return 1;
}

void func_8008AD04(void) {
    s32 now;
    u32 i;
    s32 n;
    Lnk89_ *l;
    Lnk89_ *next;
    Tmr89_ *t;
    u8 *p;

    now = func_800A76D8();
    for (i = 0; i < 6; i++) {
        p = func_800960D8(i);
        l = *D_80118470[i];
        n = 0;
        while (l != NULL) {
            t = (Tmr89_ *) l->obj;
            next = l->next;
            n++;
            if (t->hold == 1) {
                t->t = func_800A76D8();
            } else if ((u32) (now - t->t) > (u32) (*(s32 *) (t->def + 0x2C) * 1000)) {
                func_80088558(t->def, p);
                func_800A82F8(D_80118470[i], (s32 *) t);
                Memory_Free(t);
            }
            l = next;
        }
        if (n == 0) {
            *(s32 *) (p + 0x6DC) = 0;
        }
    }
    for (i = 0; i < D_800F3024; i++) {
        if (D_800F3020[i].fired == 0 && (u32) (now - D_800F3020[i].t) > 10000 && D_800F3020[i].on != 0) {
            D_800F3020[i].fired = 1;
            D_80141714(D_800F3020[i].id, 1, 1.0f);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_28C10", func_8008AEC0);

void func_8008B9FC(u16 i, char *out) {
    if (i < 49) {
        sprintf(out, D_8010080C, D_801410E0[D_800F316C[i].w[D_8014169C(D_800F316C[i].b)]]);
    }
}

