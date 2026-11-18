#include "common.h"
#include "ultra64.h"
#include "gfx.h"
#include "math.h"
#include "include_asm.h"
typedef struct { u8 pad[0x10]; s32 p10; } Sub14_;
typedef struct { u8 pad[0x14]; Sub14_ *sub; u8 pad2[0x18]; s32 p30; } Item34_;
typedef struct { u16 a; u16 count; u8 pad[0x2C]; Item34_ *items; } ItemList34_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[0xC]; s32 p40; u8 pad3[0x10]; } Item54b_;
typedef struct { u16 count; u8 pad[0x2A]; Item54b_ *items; } ItemListB_;
void func_80073740(ItemListB_ *l);
typedef struct { u32 pad0 : 30; u32 b1 : 1; u32 b0 : 1; u8 pad[0x14]; } Slot18_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[0x20]; } Item54_;
typedef struct { u16 count; u8 pad[0x2A]; Item54_ *items; } ItemList_;
typedef struct { union { u32 flags; struct { u32 pad:30; u32 b1:1; u32 b0:1; } b; } u; f32 f[4]; u8 r; u8 g; u8 b; u8 p17; } SlotM_;
s32 func_80070BA4(void *);
s32 func_800717C8(s32, s32, s32);
void func_80072EE0(u8 *, void *, s32, s32, s32);
void func_8007AFD4(s32, s32, void *);
void func_80076D6C(s32, s32, s32, s32, s32, f32, f32);
extern u16 D_800F20FA;
extern u16 D_800F20FC;
extern u8 D_800F2090;
typedef struct { u8 pad[8]; u8 b8; u8 pad2[0xB]; s32 *p14; u8 pad3[0x18]; s32 w30; } Obj74_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[8]; Obj74_ *p3C; u8 pad3[0x14]; } Item74_;
typedef struct { u16 count; u8 pad[0x2A]; Item74_ *items; u8 pad2[4]; s32 w34; } List74_;
typedef struct { u8 d[0x200]; } B200_;
typedef struct Node72_ { u8 pad[0x30]; s32 flags; s32 id; s32 p38; s32 p3C; s32 p40; s32 p44; s32 p48; struct Node72_ *next; struct Node72_ *child; } Node72_;
typedef struct { u8 p0[0x2C]; Item74_ *inst; } Model13_;
extern u8 D_800F1FA8[];
void func_80072928(f32 *);
extern s32 D_800F1EA0;
extern s32 D_800F2028;
extern s32 D_800F1EA4;
void func_8007A004(s32 *, s32, u8);
extern u8 D_800F2060;
typedef struct { s16 x; s16 y; s16 z; u16 flag; s16 s; s16 t; u8 r; u8 g; u8 b; u8 a; } VtxIn75_;
typedef struct { f32 x; f32 y; f32 z; u8 r; u8 g; u8 b; u8 on; } VtxOut75_;
typedef struct { u8 p0[6]; u16 nVtx; } MeshHdr75_;
typedef struct { f32 m[3][4]; s32 flags; u8 p34[8]; MeshHdr75_ *hdr; VtxIn75_ *in; VtxOut75_ *out; } MeshXf75_;
extern u8 D_8014D184;
extern u8 D_8014D188;
extern u8 D_8014D18C;
extern u8 D_8014D190;
extern u8 D_8014D194;
extern u8 D_8014D195;
extern u8 D_8014D196;
extern s32 D_8014C64C;
extern SlotM_ D_8014D180[];
s32 func_80079140(void *, s32 *, s32,s32, s32);
s32 func_80079BD4(void *, s32);
s32 func_80079E48(void *);
s32 func_80079E68(void *);
extern s32 D_3000000;

s32 File_GetSize();
s32 File_Load(s32, s32);
s32 Memory_Alloc(s32);
s32 Memory_Free(s32);
s32 func_80073530();
s32 func_800741D8();
extern s32 D_80134798;
extern s8 D_800F1FA0;
extern s8 D_800F1FA1;
extern s8 D_800F1FA2;

void func_80072890(Node72_ *n, Node72_ *nodes, s32 mats) {
    if (n->p3C != -1) {
        n->p3C = mats + n->p3C * 0x34;
    } else {
        n->p3C = 0;
    }
    if ((s32)n->next != -1) {
        n->next = &nodes[(s32)n->next];
    } else {
        n->next = NULL;
    }
    if ((s32)n->child != -1) {
        n->child = &nodes[(s32)n->child];
        return;
    }
    n->child = NULL;
}

static inline void V3o_Normalize(Vec3f *v) {
    f32 inv = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x *= inv;
    v->y *= inv;
    v->z *= inv;
}
static inline void V3o_Cross(Vec3f *r, Vec3f *a, Vec3f *b) {
    r->x = a->y * b->z - b->y * a->z;
    r->y = b->x * a->z - a->x * b->z;
    r->z = a->x * b->y - b->x * a->y;
}
/* Re-orthonormalises the rotation part of a 3x4 matrix, keeping the direction of its first column. */
void func_80072928(f32 *m) {
    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f t;
    Vec3f u;

    a.x = m[0];
    a.y = m[4];
    a.z = m[8];
    b.x = m[1];
    b.y = m[5];
    b.z = m[9];
    c.x = 0.0f;
    c.y = 0.0f;
    c.z = 0.0f;
    V3o_Normalize(&a);
    V3o_Cross(&t, &a, &b);
    V3o_Normalize(&t);
    c = t;
    V3o_Cross(&u, &c, &a);
    V3o_Normalize(&u);
    b = u;
    m[0] = a.x;
    m[1] = b.x;
    m[2] = c.x;
    m[4] = a.y;
    m[5] = b.y;
    m[6] = c.y;
    m[8] = a.z;
    m[9] = b.z;
    m[10] = c.z;
}

void func_80072BAC(Node72_ *n, s32 arg1, s32 arg2, s32 arg3) {
    f32 m[12];
    f32 one;
    f32 zero;

    if (n != NULL) {
        one = 1.0f;
        zero = 0.0f;
        do {
            if (n->flags & 0x40) {
                if ((func_800717C8(arg1, (s32) n, n->p38) & 0xFF) == 1) {
                    if (n->child != NULL) {
                        func_80072BAC(n->child, arg1, arg2, arg3);
                    }
                    if (n->p3C != 0) {
                        func_8007AFD4(arg1, (s32) n, m);
                        if (n->flags & 1) {
                            m[9] = zero;
                            m[8] = zero;
                            m[6] = zero;
                            m[4] = zero;
                            m[2] = zero;
                            m[1] = zero;
                            m[10] = one;
                            m[5] = one;
                            m[0] = one;
                        }
                        if (n->flags & 2) {
                            ((s32 *) m)[2] = 0;
                            ((s32 *) m)[6] = 0;
                            ((s32 *) m)[8] = 0;
                            ((s32 *) m)[9] = 0;
                            m[10] = one;
                            func_80072928(m);
                        }
                        if (n->flags & 4) {
                            m[9] = zero;
                            m[8] = zero;
                            m[6] = zero;
                            m[4] = zero;
                            m[2] = zero;
                            m[1] = zero;
                            m[10] = one;
                            m[5] = one;
                            m[0] = one;
                        }
                        func_80072EE0((u8 *) n, m, func_80070BA4(m), arg2, arg3);
                    }
                } else if (n->id != arg2) {
                    n->flags &= ~0x80;
                }
            }
            n = n->next;
        } while (n != NULL);
    }
}

void func_80072D48(Node72_ *n) {
    n->flags &= ~0x40;
}

void func_80072D5C(Node72_ *n) {
    n->flags |= 0x40;
}

s32 func_80072D6C(u8 *a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    u8 buf[0x30];

    if ((func_800717C8(a1, a2, *(s32 *)(a0 + 0x38)) & 0xFF) != 1) {
        if (*(s32 *)(a0 + 0x34) != a3) {
            *(s32 *)(a0 + 0x30) &= ~0x80;
        }
        return 0;
    }
    if (*(s32 *)(a0 + 0x3C) != 0) {
        func_8007AFD4(a1, a2, buf);
        func_80072EE0(a0, buf, func_80070BA4(buf), a3, a4);
    }
    return 1;
}

void func_80072E38(Node72_ *n, s32 arg1) {
    Node72_ *mat;

    if (n->p40 == 0) {
        n->p40 = func_80079E68((void *)n->p3C);
        if (n->p40 == 0) {
            return;
        }
        func_80079BD4((void *)n->p3C, n->p40);
    }
    if (((Node72_ *)n->p3C)->flags == 0) {
        ((Node72_ *)n->p3C)->flags = func_80079E48((void *)n->p3C);
        mat = (Node72_ *)n->p3C;
        if (mat->flags != 0) {
            func_80079140((void *)n->p3C, &D_3000000, mat->flags, arg1, n->flags & 0x4000);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80072EE0);

s32 func_8007319C(s32 name) {
    s32 buf;

    buf = Memory_Alloc(File_GetSize());
    File_Load(name, buf);
    return buf;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800731DC);

ItemList_ *func_800734AC(ItemList_ *l) {
    u32 i;

    for (i = 0; i < l->count; i++) {
        l->items[i].flags |= 0x4000;
    }
    return l;
}


void func_800734F4(s32 arg0) {
    if (arg0 != 0) {
        func_80073530();
        func_80073740(arg0);
        Memory_Free(arg0);
    }
}

s32 func_80073530(ItemList34_ *l) {
    s32 i;

    for (i = 0; i < l->count; i++) {
        if (l->items[i].p30 != 0) {
            Memory_Free(l->items[i].p30);
            l->items[i].p30 = 0;
        }
        if (l->items[i].sub != NULL && l->items[i].sub->p10 != 0) {
            Memory_Free(l->items[i].sub->p10);
            l->items[i].sub->p10 = 0;
        }
    }
}

void func_800735E4(ItemList34_ *l, u32 t) {
    u32 n;

    if (t < 41) {
        return;
    }
    for (n = 0; n < 10; n++) {
        if (++D_800F1EA0 >= l->count) {
            D_800F1EA0 = 0;
        }
        if (t - *(u32 *)((u8 *)&l->items[D_800F1EA0] + 0xC) > 40) {
            if (l->items[D_800F1EA0].p30 != 0) {
                Memory_Free(l->items[D_800F1EA0].p30);
                l->items[D_800F1EA0].p30 = 0;
            }
            if (l->items[D_800F1EA0].sub != NULL && l->items[D_800F1EA0].sub->p10 != 0) {
                Memory_Free(l->items[D_800F1EA0].sub->p10);
                l->items[D_800F1EA0].sub->p10 = 0;
            }
        }
    }
}

void func_800759F0(Node72_ *n);
void func_80073740(ItemListB_ *l) {
    s32 i;

    for (i = 0; i < l->count; i++) {
        if (l->items[i].p40 != 0) {
            Memory_Free(l->items[i].p40);
            l->items[i].p40 = 0;
            func_800759F0(&l->items[i]);
        }
    }
}


void func_800737C8(ItemListB_ *l, u32 t) {
    u32 n;

    if (t < 41) {
        return;
    }
    for (n = 0; n < 10; n++) {
        if (++D_800F1EA4 >= l->count) {
            D_800F1EA4 = 0;
        }
        if (t - *(u32 *)((u8 *)&l->items[D_800F1EA4] + 0x34) > 40 && l->items[D_800F1EA4].p40 != 0) {
            Memory_Free(l->items[D_800F1EA4].p40);
            l->items[D_800F1EA4].p40 = 0;
            func_800759F0(&l->items[D_800F1EA4]);
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800738C0);

s32 func_80073A48(s32 name) {
    s32 buf;

    buf = Memory_Alloc(0x12C00);
    File_Load(name, buf);
    return buf;
}

s32 func_80073A88(s32 name) {
    s32 buf;

    buf = Memory_Alloc(File_GetSize());
    File_Load(name, buf);
    return buf;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80073AC8);

void func_800740DC(List74_ *l, u8 v) {
    u32 i;
    Obj74_ *o;

    if (l != NULL) {
        for (i = 0; i < l->count; i++) {
            o = l->items[i].p3C;
            o->b8 = v;
            if (o->w30 != 0) {
                func_80079140(o, &D_3000000, o->w30, l->w34, l->items->flags & 0x4000);
            }
            if (o->p14 != NULL && o->p14[4] != 0) {
                func_8007A004(o->p14, l->w34, o->b8);
            }
        }
    }
}

void func_800741AC(s32 arg0) {
    func_800741D8();
    Memory_Free(arg0);
}

void func_800757E8(s32);
s32 func_800741D8(ItemListB_ *l) {
    s32 i;

    if (l != NULL) {
        func_800757E8(((s8 *)l)[0xFE]);
        func_80073530(l);
        for (i = 0; i < l->count; i++) {
            if (l->items[i].p40 != 0) {
                Memory_Free(l->items[i].p40);
                l->items[i].p40 = 0;
                func_800759F0(&l->items[i]);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80074274);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075084);

u8 func_80075564(Model13_ *m) {
    u8 id;

    id = 0xFF;
    if (m != NULL) {
        id = m->inst->p3C->b8;
    }
    return id;
}

void func_80075580(void) {
    s32 i;

    D_80134798 = 0;
    D_8014C64C = 0;
    for (i = 0; i < 16; i++) {
        D_8014D180[i].u.flags = 0;
    }
}

void func_800755B4(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 16; i++) {
        if (D_8014D180[i].u.b.b0 & D_8014D180[i].u.b.b1) {
            n = i + 1;
        }
    }
    D_8014C64C = n;
}

void func_80075600(s32 i, s8 r, s8 g, s8 b) {
    if (i != -1) {
        D_8014D180[i].r = r;
        D_8014D180[i].g = g;
        D_8014D180[i].b = b;
    }
}

void func_80075640(s32 i, f32 a, f32 b, f32 c, f32 d) {
    if (i != -1) {
        D_8014D180[i].f[0] = a;
        D_8014D180[i].f[1] = b;
        D_8014D180[i].f[2] = c;
        D_8014D180[i].f[3] = d;
    }
}

void func_800756A0(s32 id) {
    s32 n;
    s32 i;

    if (id != -1) {
        n = 0;
        D_8014D180[id].u.flags |= 1;
        for (i = 0; i < 16; i++) {
            if (D_8014D180[i].u.b.b0 & D_8014D180[i].u.b.b1) {
                n = i + 1;
            }
        }
        D_8014C64C = n;
    }
}

void func_80075720(s32 id) {
    s32 n;
    s32 i;

    if (id != -1) {
        n = 0;
        D_8014D180[id].u.flags &= ~1;
        for (i = 0; i < 16; i++) {
            if (D_8014D180[i].u.b.b0 & D_8014D180[i].u.b.b1) {
                n = i + 1;
            }
        }
        D_8014C64C = n;
    }
}

s32 func_800757A4(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(D_8014D180[i].u.flags & 2)) {
            D_8014D180[i].u.flags = 2;
            return i;
        }
    }
    return -1;
}


void func_800757E8(s32 id) {
    s32 n;
    s32 i;

    if (id != -1) {
        D_8014D180[id].u.flags = 0;
        n = 0;
        for (i = 0; i < 16; i++) {
            if (D_8014D180[i].u.b.b0 & D_8014D180[i].u.b.b1) {
                n = i + 1;
            }
        }
        D_8014C64C = n;
    }
}


void func_80075858(void) {
    if (++D_80134798 >= 2) {
        D_80134798 = 0;
    }
}

/* Transforms the mesh vertices by m; vertices whose alpha bit 0 is clear are marked off. */
void func_80075888(MeshXf75_ *o) {
    VtxIn75_ *in;
    VtxOut75_ *out;
    s32 i;
    f32 x;
    f32 y;
    f32 z;

    if (o->out == NULL) {
        o->out = ((void * (*)(s32 size))Memory_Alloc)(o->hdr->nVtx * sizeof(VtxOut75_));
    }
    out = o->out;
    in = o->in;
    for (i = 0; i < o->hdr->nVtx; out++, in++, i++) {
        if (in->a & 1) {
            x = in->x;
            y = in->y;
            z = in->z;
            out->x = o->m[0][0] * x + o->m[0][1] * y + o->m[0][2] * z + o->m[0][3];
            out->y = o->m[1][0] * x + o->m[1][1] * y + o->m[1][2] * z + o->m[1][3];
            out->z = o->m[2][0] * x + o->m[2][1] * y + o->m[2][2] * z + o->m[2][3];
            out->r = in->r;
            out->g = in->g;
            out->b = in->b;
            out->on = 1;
        } else {
            out->on = 0;
        }
    }
}

void func_800759F0(Node72_ *n) {
    if (n->p44 != 0) {
        Memory_Free(n->p44);
        n->p44 = 0;
        n->flags &= ~0x100;
    }
}

/* Copies the colors of the visible transformed vertices back to the input vertices. */
void func_80075A38(MeshXf75_ *o) {
    s32 i;
    VtxOut75_ *out;
    VtxIn75_ *in;
    s32 n = o->hdr->nVtx;

    in = o->in;
    out = o->out;
    for (i = 0; i < n; i++) {
        if (o->out[i].on & 1) {
            in->r = out->r;
            in->g = out->g;
            in->b = out->b;
        }
        out++;
        in++;
    }
    o->flags &= ~0x100;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075ABC);

void func_80076020(s32 w, s32 h) {
    GFX_RESERVE(6);
    gSPSegment(D_8014EC78++, 0, 0);
    gSPLoadGeometryMode(D_8014EC78++, 0);
    gSPTexture(D_8014EC78++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);
    gDPPipeSync(D_8014EC78++);
    gDPSetScissor(D_8014EC78++, G_SC_NON_INTERLACE, 0, 0, w - 1, h - 1);
    gSPDisplayList(D_8014EC78++, D_800F1FA8);
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800761D4);

void func_80076470(B200_ *src) {
    if (src != NULL) {
        if (D_800F2028 == 0) {
            D_800F2028 = Memory_Alloc(0x200);
        }
        *(B200_ *)D_800F2028 = *src;
    } else if (D_800F2028 != 0) {
        Memory_Free(D_800F2028);
        D_800F2028 = 0;
    }
}


void func_80076578(f32 k, u16 *src) {
    u16 *dst;
    s32 i;
    u16 c;
    f32 r;
    f32 g;
    f32 b;
    u16 rr;
    u16 gg;
    u16 bb;

    dst = (u16 *)D_800F2028;
    for (i = 0; i < 256; i++) {
        c = *src;
        r = (c >> 11) / 31.0;
        g = ((c & 0x7C0) >> 6) / 31.0;
        b = ((c & 0x3E) >> 1) / 31.0;
        rr = (u16)(r * k * 255.0f) >> 3;
        gg = (u16)(g * k * 255.0f) >> 3;
        bb = (u16)(b * k * 255.0f) >> 3;
        *dst = (rr << 11) | (gg << 6) | (bb << 1);
        src++;
        dst++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800766FC);

void func_80076BB0(s8 arg0, s8 arg1, s8 arg2) {
    D_800F1FA0 = arg0;
    D_800F1FA1 = arg1;
    D_800F1FA2 = arg2;
}

extern u8 D_800F2030[];
void func_80076BCC(void) {
    GFX_RESERVE(3);
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xDE) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (8)) - 1)) << (16))) | ((u32) (((u32) (0) & ((0x01 << (16)) - 1)) << (0))); _g->words.w1 = (u32) ( D_800F2030); };
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ( 0xE7) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; };
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) (0xFA) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (8)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (8)) - 1)) << (0)))); _g->words.w1 = (((u32) (((u32) ( (u8) D_800F1FA0) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( (u8) D_800F1FA1) & ((0x01 << (8)) - 1)) << (16))) | ((u32) (((u32) ( (u8) D_800F1FA2) & ((0x01 << (8)) - 1)) << (8))) | ((u32) (((u32) ( 0xFF) & ((0x01 << (8)) - 1)) << (0)))); };
}

void func_80076CF8(s32 a, s32 b, s32 c, s32 d, s32 e) {
    if (d >= -a && e >= -b && a < D_800F20FA && b < D_800F20FC) {
        func_80076D6C(a, b, c, d, e, 1.0f, 1.0f);
    }
}

void func_80076D6C(s32 px, s32 py, s32 timg, s32 w, s32 h, f32 sx, f32 sy) {
    s32 n;
    s32 x;
    s32 y;
    s32 tw;
    s32 th;
    f32 dsdx;
    f32 dtdy;

    tw = 64;
    th = 32;
    n = ((w + 63) / tw) * ((h + 31) / th) * 10;
    dsdx = (1.0f / sx) * 1024.0f;
    dtdy = (1.0f / sy) * 1024.0f;
    GFX_RESERVE(n);
    for (x = 0; x < w; x += tw) {
        if (w - x < tw) {
            tw = w - x;
        }
        th = 32;
        for (y = 0; y < h; y += th) {
            if (h - y < th) {
                th = h - y;
            }
            { { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xFD) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) ( 2) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (( w) - 1) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = (u32) ( timg); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) ( 2) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((((( x + tw - 1) - ( x) + 1) * 2) + 7) >> 3)) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE6) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF4) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (( x) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y) << 2) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (( x + tw - 1) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y + th - 1) << 2) & ((0x01 << (12)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE7) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) ( 2) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((((( x + tw - 1) - ( x) + 1) * 2) + 7) >> 3)) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF2) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (( x) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y) << 2) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (( x + tw - 1) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y + th - 1) << 2) & ((0x01 << (12)) - 1)) << (0))); } };
            { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) (0xE4) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( (px + (s32) (sx * x) + (s32) (sx * tw)) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) ( (py + (s32) (sy * y) + (s32) (sy * th)) << 2) & ((0x01 << (12)) - 1)) << (0)))); _g->words.w1 = (((u32) (((u32) ( 0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) ( (px + (s32) (sx * x)) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) ( (py + (s32) (sy * y)) << 2) & ((0x01 << (12)) - 1)) << (0)))); { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ((0xE1)) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = (u32) ((((u32) (((u32) ( x << 5) & ((0x01 << (16)) - 1)) << (16))) | ((u32) (((u32) ( y << 5) & ((0x01 << (16)) - 1)) << (0))))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ((0xF1)) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = (u32) ((((u32) (((u32) ( (u32) dsdx) & ((0x01 << (16)) - 1)) << (16))) | ((u32) (((u32) ( (u32) dtdy) & ((0x01 << (16)) - 1)) << (0))))); }; };
        }
    }
}

void func_80077228(void) {
    GFX_RESERVE(1);
    gSPDisplayList(D_8014EC78++, &D_800F2060);
}

void func_800772F4(s32 px, s32 py, u32 timg, s32 w, s32 h, f32 sx, f32 sy) {
    s32 n;
    s32 x;
    s32 y;
    s32 tw;
    s32 th;
    f32 dsdx;
    f32 dtdy;

    tw = 64;
    th = 32;
    dsdx = (1.0f / sx) * 1024.0f;
    dtdy = (1.0f / sy) * 1024.0f;
    n = ((w + 63) / tw) * ((h + 31) / th) * 10;
    GFX_RESERVE(n + 8);
    { { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xFD) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (2) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) ((1) - 1) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = (u32) ( timg); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE8) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (0) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (256) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) (0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) (0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE6) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF0) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (255) & ((0x01 << (10)) - 1)) << (14))); } { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE7) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; };
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) ( 0xE3) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (32 - ( 14) - ( 2)) & ((0x01 << (8)) - 1)) << (8))) | ((u32) (((u32) (( 2) - 1) & ((0x01 << (8)) - 1)) << (0)))); _g->words.w1 = (u32) ( 0x8000); };
    timg += 0x200;
    for (x = 0; x < w; x += tw) {
        if (w - x < tw) {
            tw = w - x;
        }
        th = 32;
        for (y = 0; y < h; y += th) {
            if (h - y < th) {
                th = h - y;
            }
            { { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xFD) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 2) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (1) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (( w) - 1) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = (u32) ( timg); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 2) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (1) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((((( x + tw - 1) - ( x) + 1) * 1) + 7) >> 3)) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE6) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF4) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (( x) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y) << 2) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (( x + tw - 1) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y + th - 1) << 2) & ((0x01 << (12)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE7) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 2) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (1) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((((( x + tw - 1) - ( x) + 1) * 1) + 7) >> 3)) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF2) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (( x) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y) << 2) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (( x + tw - 1) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y + th - 1) << 2) & ((0x01 << (12)) - 1)) << (0))); } };
            { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) (0xE4) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( (px + (s32) (sx * x) + (s32) (sx * tw)) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) ( (py + (s32) (sy * y) + (s32) (sy * th)) << 2) & ((0x01 << (12)) - 1)) << (0)))); _g->words.w1 = (((u32) (((u32) ( 0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) ( (px + (s32) (sx * x)) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) ( (py + (s32) (sy * y)) << 2) & ((0x01 << (12)) - 1)) << (0)))); { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ((0xE1)) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = (u32) ((((u32) (((u32) ( x << 5) & ((0x01 << (16)) - 1)) << (16))) | ((u32) (((u32) ( y << 5) & ((0x01 << (16)) - 1)) << (0))))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ((0xF1)) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = (u32) ((((u32) (((u32) ( (u32) dsdx) & ((0x01 << (16)) - 1)) << (16))) | ((u32) (((u32) ( (u32) dtdy) & ((0x01 << (16)) - 1)) << (0))))); }; };
        }
    }
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) ( 0xE3) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (32 - ( 14) - ( 2)) & ((0x01 << (8)) - 1)) << (8))) | ((u32) (((u32) (( 2) - 1) & ((0x01 << (8)) - 1)) << (0)))); _g->words.w1 = (u32) ( 0); };
}

void func_800778A4(void) {
    GFX_RESERVE(1);
    gSPDisplayList(D_8014EC78++, &D_800F2090);
}

void func_80077970(s32 px, s32 py, u32 timg, s32 w, s32 h, f32 sx, f32 sy) {
    s32 n;
    s32 x;
    s32 y;
    s32 tw;
    s32 th;
    f32 dsdx;
    f32 dtdy;

    tw = 64;
    th = 32;
    n = ((w + 63) / tw) * ((h + 63) / tw) * 10;
    dsdx = (1.0f / sx) * 1024.0f;
    dtdy = (1.0f / sy) * 1024.0f;
    GFX_RESERVE(n);
    for (x = 0; x < w; x += tw) {
        if (w - x < tw) {
            tw = w - x;
        }
        th = 32;
        for (y = 0; y < h; y += th) {
            if (h - y < th) {
                th = h - y;
            }
            { { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xFD) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 3) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (1) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((( w) >> 1)) - 1) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = (u32) ( timg); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 3) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (1) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((((( x + tw - 1) - ( x) + 1) >> 1) + 7) >> 3)) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE6) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF4) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (( x) << 1) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y) << 2) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (7) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (( x + tw - 1) << 1) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y + th - 1) << 2) & ((0x01 << (12)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xE7) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF5) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 3) & ((0x01 << (3)) - 1)) << (21))) | ((u32) (((u32) (0) & ((0x01 << (2)) - 1)) << (19))) | ((u32) (((u32) (((((( x + tw - 1) - ( x) + 1) >> 1) + 7) >> 3)) & ((0x01 << (9)) - 1)) << (9))) | ((u32) (((u32) (0) & ((0x01 << (9)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (20))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (18))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (14))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (10))) | ((u32) (((u32) ( 0) & ((0x01 << (2)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (4))) | ((u32) (((u32) ( 0) & ((0x01 << (4)) - 1)) << (0))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xF2) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (( x) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y) << 2) & ((0x01 << (12)) - 1)) << (0))); _g->words.w1 = ((u32) (((u32) (0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) (( x + tw - 1) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) (( y + th - 1) << 2) & ((0x01 << (12)) - 1)) << (0))); } };
            { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) (0xE4) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( (px + (s32) (sx * x) + (s32) (sx * tw)) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) ( (py + (s32) (sy * y) + (s32) (sy * th)) << 2) & ((0x01 << (12)) - 1)) << (0)))); _g->words.w1 = (((u32) (((u32) ( 0) & ((0x01 << (3)) - 1)) << (24))) | ((u32) (((u32) ( (px + (s32) (sx * x)) << 2) & ((0x01 << (12)) - 1)) << (12))) | ((u32) (((u32) ( (py + (s32) (sy * y)) << 2) & ((0x01 << (12)) - 1)) << (0)))); { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ((0xE1)) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = (u32) ((((u32) (((u32) ( x << 5) & ((0x01 << (16)) - 1)) << (16))) | ((u32) (((u32) ( y << 5) & ((0x01 << (16)) - 1)) << (0))))); }; { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ((0xF1)) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = (u32) ((((u32) (((u32) ( (u32) dsdx) & ((0x01 << (16)) - 1)) << (16))) | ((u32) (((u32) ( (u32) dtdy) & ((0x01 << (16)) - 1)) << (0))))); }; };
        }
    }
}

extern u8 D_800F20C0[];
void func_80077E5C(s32 r, s32 g, s32 b, s32 a) {
    GFX_RESERVE(3);
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) ( 0xE7) & ((0x01 << (8)) - 1)) << (24))); _g->words.w1 = 0; };
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = (((u32) (((u32) (0xFA) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( 0) & ((0x01 << (8)) - 1)) << (8))) | ((u32) (((u32) ( 0) & ((0x01 << (8)) - 1)) << (0)))); _g->words.w1 = (((u32) (((u32) ( r) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) ( g) & ((0x01 << (8)) - 1)) << (16))) | ((u32) (((u32) ( b) & ((0x01 << (8)) - 1)) << (8))) | ((u32) (((u32) ( a) & ((0x01 << (8)) - 1)) << (0)))); };
    { Gfx *_g = (Gfx *) (D_8014EC78++); _g->words.w0 = ((u32) (((u32) (0xDE) & ((0x01 << (8)) - 1)) << (24))) | ((u32) (((u32) (0) & ((0x01 << (8)) - 1)) << (16))) | ((u32) (((u32) (0) & ((0x01 << (16)) - 1)) << (0))); _g->words.w1 = (u32) ( D_800F20C0); };
}
