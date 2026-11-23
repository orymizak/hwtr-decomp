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
void WorldData_FreeNodeCache(ItemListB_ *l);
typedef struct { u32 pad0 : 30; u32 b1 : 1; u32 b0 : 1; u8 pad[0x14]; } Slot18_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[0x20]; } Item54_;
typedef struct { u16 count; u8 pad[0x2A]; Item54_ *items; } ItemList_;
typedef struct { union { u32 flags; struct { u32 pad:30; u32 b1:1; u32 b0:1; } b; } u; f32 f[4]; u8 r; u8 g; u8 b; u8 p17; } SlotM_;
s32 Mtx_New(void *);
s32 Frustum_TestSphere(s32, s32, s32);
void SceneNode_DrawMesh(u8 *, void *, s32, s32, s32);
void Mtx34_Mul(s32, s32, void *);
void Image_DrawRGBA16Scaled(s32, s32, s32, s32, s32, f32, f32);
extern u16 gScreenWidth;
extern u16 gScreenHeight;
extern u8 D_800F2090;
typedef struct { u8 pad[8]; u8 b8; u8 pad2[0xB]; s32 *p14; u8 pad3[0x18]; s32 w30; } Obj74_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[8]; Obj74_ *p3C; u8 pad3[0x14]; } Item74_;
typedef struct { u16 count; u8 pad[0x2A]; Item74_ *items; u8 pad2[4]; s32 w34; } List74_;
typedef struct { u8 d[0x200]; } B200_;
typedef struct Node72_ { u8 pad[0x30]; s32 flags; s32 id; s32 p38; s32 p3C; s32 p40; s32 p44; s32 p48; struct Node72_ *next; struct Node72_ *child; } Node72_;
typedef struct { u8 p0[0x2C]; Item74_ *inst; } Model13_;
extern u8 D_800F1FA8[];
void Mtx34_Orthonormalize(f32 *);
extern s32 D_800F1EA0;
extern s32 gBgPalette;
extern s32 D_800F1EA4;
void Mesh_BuildMaterialDL(s32 *, s32, u8);
extern u8 D_800F2060;
typedef struct { f32 x; f32 y; f32 z; u8 r; u8 g; u8 b; u8 on; } VtxOut75_;
typedef struct { u8 p0[6]; u16 nVtx; } MeshHdr75_;
typedef struct { f32 m[3][4]; s32 flags; s32 id; f32 rad; MeshHdr75_ *hdr; Vtx *in; VtxOut75_ *out; } MeshXf75_;
extern u8 D_8014D184;
extern u8 D_8014D188;
extern u8 D_8014D18C;
extern u8 D_8014D190;
extern u8 D_8014D194;
extern u8 D_8014D195;
extern u8 D_8014D196;
extern s32 gPointLightCount;
extern SlotM_ gPointLights[];
s32 Mesh_BuildDisplayList(void *, s32 *, s32,s32, s32);
s32 Mesh_FillVtx(void *, s32);
s32 Mesh_AllocDisplayList(void *);
s32 Mesh_AllocVtx(void *);
extern s32 D_3000000;

s32 File_GetSize();
s32 File_Load(s32, s32);
s32 Memory_Alloc(s32);
s32 Memory_Free(s32);
s32 WorldData_FreeMaterialCache();
s32 ModelData_FreeCaches();
extern s32 gFrameParity;
extern s8 D_800F1FA0;
extern s8 D_800F1FA1;
extern s8 D_800F1FA2;

void SceneNode_Relocate(Node72_ *n, Node72_ *nodes, s32 mats) {
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
void Mtx34_Orthonormalize(f32 *m) {
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

void SceneNode_Draw(Node72_ *n, s32 arg1, s32 arg2, s32 arg3) {
    f32 m[12];
    f32 one;
    f32 zero;

    if (n != NULL) {
        one = 1.0f;
        zero = 0.0f;
        do {
            if (n->flags & 0x40) {
                if ((Frustum_TestSphere(arg1, (s32) n, n->p38) & 0xFF) == 1) {
                    if (n->child != NULL) {
                        SceneNode_Draw(n->child, arg1, arg2, arg3);
                    }
                    if (n->p3C != 0) {
                        Mtx34_Mul(arg1, (s32) n, m);
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
                            Mtx34_Orthonormalize(m);
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
                        SceneNode_DrawMesh((u8 *) n, m, Mtx_New(m), arg2, arg3);
                    }
                } else if (n->id != arg2) {
                    n->flags &= ~0x80;
                }
            }
            n = n->next;
        } while (n != NULL);
    }
}

void SceneNode_Hide(Node72_ *n) {
    n->flags &= ~0x40;
}

void SceneNode_Show(Node72_ *n) {
    n->flags |= 0x40;
}

s32 SceneNode_DrawSingle(u8 *a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    u8 buf[0x30];

    if ((Frustum_TestSphere(a1, a2, *(s32 *)(a0 + 0x38)) & 0xFF) != 1) {
        if (*(s32 *)(a0 + 0x34) != a3) {
            *(s32 *)(a0 + 0x30) &= ~0x80;
        }
        return 0;
    }
    if (*(s32 *)(a0 + 0x3C) != 0) {
        Mtx34_Mul(a1, a2, buf);
        SceneNode_DrawMesh(a0, buf, Mtx_New(buf), a3, a4);
    }
    return 1;
}

void SceneNode_PrepareMaterial(Node72_ *n, s32 arg1) {
    Node72_ *mat;

    if (n->p40 == 0) {
        n->p40 = Mesh_AllocVtx((void *)n->p3C);
        if (n->p40 == 0) {
            return;
        }
        Mesh_FillVtx((void *)n->p3C, n->p40);
    }
    if (((Node72_ *)n->p3C)->flags == 0) {
        ((Node72_ *)n->p3C)->flags = Mesh_AllocDisplayList((void *)n->p3C);
        mat = (Node72_ *)n->p3C;
        if (mat->flags != 0) {
            Mesh_BuildDisplayList((void *)n->p3C, &D_3000000, mat->flags, arg1, n->flags & 0x4000);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", SceneNode_DrawMesh);

s32 File_LoadAlloc(s32 name) {
    s32 buf;

    buf = Memory_Alloc(File_GetSize());
    File_Load(name, buf);
    return buf;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", WorldData_Relocate);

ItemList_ *WorldData_SetMirrored(ItemList_ *l) {
    u32 i;

    for (i = 0; i < l->count; i++) {
        l->items[i].flags |= 0x4000;
    }
    return l;
}


void WorldData_Free(s32 arg0) {
    if (arg0 != 0) {
        WorldData_FreeMaterialCache();
        WorldData_FreeNodeCache(arg0);
        Memory_Free(arg0);
    }
}

s32 WorldData_FreeMaterialCache(ItemList34_ *l) {
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

void WorldData_TrimMaterialCache(ItemList34_ *l, u32 t) {
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

void SceneNode_FreeCache(Node72_ *n);
void WorldData_FreeNodeCache(ItemListB_ *l) {
    s32 i;

    for (i = 0; i < l->count; i++) {
        if (l->items[i].p40 != 0) {
            Memory_Free(l->items[i].p40);
            l->items[i].p40 = 0;
            SceneNode_FreeCache(&l->items[i]);
        }
    }
}


void WorldData_TrimNodeCache(ItemListB_ *l, u32 t) {
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
            SceneNode_FreeCache(&l->items[D_800F1EA4]);
        }
    }
}


typedef struct { u16 pal; u8 first; u8 last; s16 speed; u16 time; } PalAnim38_;
typedef struct { u8 p0[8]; u16 col[1]; } PalData38_;
typedef struct { u8 p0[4]; PalData38_ *data; } PalRef38_;
typedef struct { u8 p0[0x12]; u16 nAnims; u8 p14[0x20]; PalRef38_ *pals; u8 p38[0x18]; PalAnim38_ *anims; } PalObj38_;
void *memcpy(void *, const void *, u32);
static inline void PalAnim_Rotate(PalAnim38_ *a, u16 *col, u16 old) {
    u16 tmp[256];
    u32 j;
    u32 n;
    s32 steps;
    s16 d;

    d = a->time - old;
    n = a->last - a->first + 1;
    memcpy(tmp, col, n * 2);
    steps = (s32)((f32)d * (f32)a->speed) >> 10;
    a->time = steps << 10;
    steps = (n < 2) ? 0 : steps % n;
    for (j = 0; j < n; j++) {
        col[j] = tmp[steps];
        steps++;
        if (steps == n) {
            steps = 0;
        }
    }
}

void ModelData_Update(PalObj38_ *o, s32 dt) {
    PalAnim38_ *a;
    u32 i;
    u16 old;
    PalRef38_ *ref;
    s32 ofs;

    if (o == NULL) {
        return;
    }
    for (i = 0; i < o->nAnims; i++) {
        a = &o->anims[i];
        old = a->time;
        a->time += dt;
        ref = &o->pals[a->pal];
        /* TODO(match-debt): &ref->data->col[a->first] adds the base after the index; the byte offset
           has to be its own value. */
        ofs = a->first * 2;
        PalAnim_Rotate(a, (u16 *)((u8 *)ref->data + ofs) + 4, old);
    }
}

s32 File_LoadAllocFixed(s32 name) {
    s32 buf;

    buf = Memory_Alloc(0x12C00);
    File_Load(name, buf);
    return buf;
}

s32 File_LoadAllocCopy(s32 name) {
    s32 buf;

    buf = Memory_Alloc(File_GetSize());
    File_Load(name, buf);
    return buf;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", ModelData_Load);

void ModelData_SetVariant(List74_ *l, u8 v) {
    u32 i;
    Obj74_ *o;

    if (l != NULL) {
        for (i = 0; i < l->count; i++) {
            o = l->items[i].p3C;
            o->b8 = v;
            if (o->w30 != 0) {
                Mesh_BuildDisplayList(o, &D_3000000, o->w30, l->w34, l->items->flags & 0x4000);
            }
            if (o->p14 != NULL && o->p14[4] != 0) {
                Mesh_BuildMaterialDL(o->p14, l->w34, o->b8);
            }
        }
    }
}

void ModelData_Free(s32 arg0) {
    ModelData_FreeCaches();
    Memory_Free(arg0);
}

void PointLight_Free(s32);
s32 ModelData_FreeCaches(ItemListB_ *l) {
    s32 i;

    if (l != NULL) {
        PointLight_Free(((s8 *)l)[0xFE]);
        WorldData_FreeMaterialCache(l);
        for (i = 0; i < l->count; i++) {
            if (l->items[i].p40 != 0) {
                Memory_Free(l->items[i].p40);
                l->items[i].p40 = 0;
                SceneNode_FreeCache(&l->items[i]);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", ModelData_Draw);

typedef struct { f32 x, y, z; } V3_75;
typedef struct { f32 a, b, c, d; } Pl_75;
typedef struct { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } Vx_75;
typedef struct { u8 pad[8]; u8 a; } C3C_75;
typedef struct { u8 pad[0x3C]; C3C_75 *p; } C2C_75;
typedef struct {
    u8 pad0[0x14]; f32 f14, f18, f1C, f20, f24; u8 pad28[4]; C2C_75 *p2C;
    u8 pad30[0xE8]; Vx_75 *vtx; u8 pad11C[0x18]; s32 h134; u8 p138[4]; Pl_75 pl; u8 on; u8 pad14D[0x13]; f32 m[3][4];
} O_75;
extern V3_75 D_800FF7C0;
extern V3_75 D_800F1F64[4];
void SortQueue_Add(s32, s32, s32);
/* Projects the shadow quad of model o onto its ground plane and queues it (view matrix arg1). */
void ModelData_DrawShadow(O_75 *o, s32 arg1) {
    V3_75 pts[4];
    V3_75 out[4];
    V3_75 tmp;
    V3_75 dir;
    Pl_75 pl;
    f32 m[3][4];
    f32 r[3][4];
    u32 i;
    u32 k;
    f32 dot, t;
    f32 zero;
    Vx_75 *v;
    u8 a;
    s32 h;

    dir = D_800FF7C0;
    if (o == NULL || !o->on) {
        return;
    }
    pl = o->pl;
    D_800F1F64[0].x = o->f14;
    D_800F1F64[0].y = o->f18;
    D_800F1F64[1].x = o->f20;
    D_800F1F64[1].y = o->f18;
    D_800F1F64[2].x = o->f20;
    D_800F1F64[2].y = o->f24;
    D_800F1F64[3].x = o->f14;
    D_800F1F64[3].y = o->f24;
    for (i = 0; i < 4; i++) {
        f32 x = D_800F1F64[i].x;
        f32 y = D_800F1F64[i].y;
        f32 z = D_800F1F64[i].z;
        pts[i].x = o->m[0][0] * x + o->m[0][1] * y + o->m[0][2] * z + o->m[0][3];
        pts[i].y = o->m[1][0] * x + o->m[1][1] * y + o->m[1][2] * z + o->m[1][3];
        pts[i].z = o->m[2][0] * x + o->m[2][1] * y + o->m[2][2] * z + o->m[2][3];
    }
    dot = pl.a * dir.x + pl.b * dir.y + pl.c * dir.z;
    if ((dot < 0.5f) & (-0.5f < dot)) {
        return;
    }
    for (k = 0; k < 4; k++) {
        t = (pts[k].x * pl.a + pts[k].y * pl.b + pts[k].z * pl.c + pl.d - 4.0f) / -dot;
        tmp.x = dir.x * t;
        tmp.y = dir.y * t;
        tmp.z = dir.z * t;
        out[k].x = pts[k].x + tmp.x;
        out[k].y = pts[k].y + tmp.y;
        out[k].z = pts[k].z + tmp.z;
    }
    for (k = 1; k < 4; k++) {
        out[k].x -= out[0].x;
        out[k].y -= out[0].y;
        out[k].z -= out[0].z;
    }
    if ((fabsf(out[0].x) > 32767.0f) | (fabsf(out[0].y) > 32767.0f) | (fabsf(out[0].z) > 32767.0f)) {
        return;
    }
    zero = 0.0f;
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][1] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][2] = m[2][3] = zero;
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    m[0][3] = out[0].x + out[0].y * zero + out[0].z * zero + zero;
    m[1][3] = out[0].x * zero + out[0].y + out[0].z * zero + zero;
    m[2][3] = out[0].x * zero + out[0].y * zero + out[0].z + zero;
    Mtx34_Mul(arg1, m[0], r[0]);
    out[0].x = zero;
    out[0].y = zero;
    out[0].z = zero;
    h = Mtx_New(r[0]);
    v = o->vtx;
    a = o->p2C->p->a;
    for (k = 0; k < 4; k++, v++) {
        v->ob[0] = out[k].x;
        v->ob[1] = out[k].y;
        v->ob[2] = out[k].z;
        v->cn[0] = 0;
        v->cn[1] = 0;
        v->cn[2] = 0;
        v->cn[3] = a;
    }
    SortQueue_Add(o->h134, h, 0);
}

u8 Model_GetVariant(Model13_ *m) {
    u8 id;

    id = 0xFF;
    if (m != NULL) {
        id = m->inst->p3C->b8;
    }
    return id;
}

void PointLights_Reset(void) {
    s32 i;

    gFrameParity = 0;
    gPointLightCount = 0;
    for (i = 0; i < 16; i++) {
        gPointLights[i].u.flags = 0;
    }
}

void PointLights_Recount(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 16; i++) {
        if (gPointLights[i].u.b.b0 & gPointLights[i].u.b.b1) {
            n = i + 1;
        }
    }
    gPointLightCount = n;
}

void PointLight_SetColor(s32 i, s8 r, s8 g, s8 b) {
    if (i != -1) {
        gPointLights[i].r = r;
        gPointLights[i].g = g;
        gPointLights[i].b = b;
    }
}

void PointLight_SetPos(s32 i, f32 a, f32 b, f32 c, f32 d) {
    if (i != -1) {
        gPointLights[i].f[0] = a;
        gPointLights[i].f[1] = b;
        gPointLights[i].f[2] = c;
        gPointLights[i].f[3] = d;
    }
}

void PointLight_Enable(s32 id) {
    s32 n;
    s32 i;

    if (id != -1) {
        n = 0;
        gPointLights[id].u.flags |= 1;
        for (i = 0; i < 16; i++) {
            if (gPointLights[i].u.b.b0 & gPointLights[i].u.b.b1) {
                n = i + 1;
            }
        }
        gPointLightCount = n;
    }
}

void PointLight_Disable(s32 id) {
    s32 n;
    s32 i;

    if (id != -1) {
        n = 0;
        gPointLights[id].u.flags &= ~1;
        for (i = 0; i < 16; i++) {
            if (gPointLights[i].u.b.b0 & gPointLights[i].u.b.b1) {
                n = i + 1;
            }
        }
        gPointLightCount = n;
    }
}

s32 PointLight_Alloc(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(gPointLights[i].u.flags & 2)) {
            gPointLights[i].u.flags = 2;
            return i;
        }
    }
    return -1;
}


void PointLight_Free(s32 id) {
    s32 n;
    s32 i;

    if (id != -1) {
        gPointLights[id].u.flags = 0;
        n = 0;
        for (i = 0; i < 16; i++) {
            if (gPointLights[i].u.b.b0 & gPointLights[i].u.b.b1) {
                n = i + 1;
            }
        }
        gPointLightCount = n;
    }
}


void Gfx_FlipFrameParity(void) {
    if (++gFrameParity >= 2) {
        gFrameParity = 0;
    }
}

/* Transforms the mesh vertices by m; vertices whose alpha bit 0 is clear are marked off. */
void Mesh_TransformVerts(MeshXf75_ *o) {
    Vtx *in;
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
        if (in->v.cn[3] & 1) {
            x = in->v.ob[0];
            y = in->v.ob[1];
            z = in->v.ob[2];
            out->x = o->m[0][0] * x + o->m[0][1] * y + o->m[0][2] * z + o->m[0][3];
            out->y = o->m[1][0] * x + o->m[1][1] * y + o->m[1][2] * z + o->m[1][3];
            out->z = o->m[2][0] * x + o->m[2][1] * y + o->m[2][2] * z + o->m[2][3];
            out->r = in->v.cn[0];
            out->g = in->v.cn[1];
            out->b = in->v.cn[2];
            out->on = 1;
        } else {
            out->on = 0;
        }
    }
}

void SceneNode_FreeCache(Node72_ *n) {
    if (n->p44 != 0) {
        Memory_Free(n->p44);
        n->p44 = 0;
        n->flags &= ~0x100;
    }
}

/* Copies the colors of the visible transformed vertices back to the input vertices. */
void Mesh_StoreColors(MeshXf75_ *o) {
    s32 i;
    VtxOut75_ *out;
    Vtx *in;
    s32 n = o->hdr->nVtx;

    in = o->in;
    out = o->out;
    for (i = 0; i < n; i++) {
        if (o->out[i].on & 1) {
            in->v.cn[0] = out->r;
            in->v.cn[1] = out->g;
            in->v.cn[2] = out->b;
        }
        out++;
        in++;
    }
    o->flags &= ~0x100;
}

/* Adds the colored light of nearby point lights to the mesh vertices (alloca keeps the base colors). */
void Mesh_ApplyPointLights(MeshXf75_ *o, s32 id) {
    u8 *r;
    u8 *g;
    u8 *b;
    Vtx *dst;
    VtxOut75_ *src;
    s32 lit;
    u32 i, j;
    s32 k, n;
    f32 rad, rr, lx, ly, lz, dx, dy, dz, d;
    u32 c;
    f32 r2;
    s32 prev;
    Vtx *d0;
    VtxOut75_ *s0;

    r = __builtin_alloca(o->hdr->nVtx + 1);
    g = __builtin_alloca(o->hdr->nVtx + 1);
    b = __builtin_alloca(o->hdr->nVtx + 1);
    /* TODO(match-debt): lit is cleared inside the condition; before the if it changes codegen. */
    if (!(o->flags & 0x200) && (lit = 0, o->id != id)) {
        src = o->out;
        dst = o->in;
        for (i = 0; i < (u32)gPointLightCount; i++) {
            rad = gPointLights[i].f[0];
            rr = rad + o->rad;
            rad *= rad;
            rr *= rr;
            lx = gPointLights[i].f[1];
            ly = gPointLights[i].f[2];
            lz = gPointLights[i].f[3];
            if (gPointLights[i].u.flags & 1) {
                dx = o->m[0][3] - lx;
                dy = o->m[1][3] - ly;
                dz = o->m[2][3] - lz;
                if (dx * dx + dy * dy + dz * dz <= rr) {
                    if (src == NULL) {
                        Mesh_TransformVerts(o);
                        src = o->out;
                    }
                    prev = lit;
                    o->flags |= 0x100;
                    lit = 1;
                    for (j = 0; j < o->hdr->nVtx; j++) {
                        if (src[j].on & 1) {
                            dx = src[j].x - lx;
                            dy = src[j].y - ly;
                            dz = src[j].z - lz;
                            d = (dx * dx + dy * dy + dz * dz) / rad;
                            if (!prev & (((u32)o->flags >> 8) & 1)) {
                                r[j] = src[j].r;
                                g[j] = src[j].g;
                                b[j] = src[j].b;
                            }
                            if (d <= 1.0) {
                                c = (f32)r[j] + (f32)gPointLights[i].r * (1.0 - d);
                                if (c > 255) {
                                    c = 255;
                                }
                                r[j] = c;
                                c = (f32)g[j] + (f32)gPointLights[i].g * (1.0 - d);
                                if (c > 255) {
                                    c = 255;
                                }
                                g[j] = c;
                                c = (f32)b[j] + (f32)gPointLights[i].b * (1.0 - d);
                                if (c > 255) {
                                    c = 255;
                                }
                                b[j] = c;
                            }
                        }
                    }
                }
            }
        }
        if (!lit & (((u32)o->flags >> 8) & 1)) {
            n = o->hdr->nVtx;
            d0 = o->in;
            s0 = o->out;
            for (k = 0; k < n; k++) {
                if (o->out[k].on & 1) {
                    d0->v.cn[0] = s0->r;
                    d0->v.cn[1] = s0->g;
                    d0->v.cn[2] = s0->b;
                }
                s0++;
                d0++;
            }
            o->flags &= ~0x100;
        } else if (lit) {
            for (i = 0; i < o->hdr->nVtx; i++) {
                if (src[i].on & 1) {
                    dst[i].v.cn[0] = r[i];
                    dst[i].v.cn[1] = g[i];
                    dst[i].v.cn[2] = b[i];
                }
            }
        }
    }
}

void Gfx_SetupRDP(s32 w, s32 h) {
    GFX_RESERVE(6);
    gSPSegment(gDisplayListHead++, 0, 0);
    gSPLoadGeometryMode(gDisplayListHead++, 0);
    gSPTexture(gDisplayListHead++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, w - 1, h - 1);
    gSPDisplayList(gDisplayListHead++, D_800F1FA8);
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", Gfx_ClearFrame);

void Bg_SetPalette(B200_ *src) {
    if (src != NULL) {
        if (gBgPalette == 0) {
            gBgPalette = Memory_Alloc(0x200);
        }
        *(B200_ *)gBgPalette = *src;
    } else if (gBgPalette != 0) {
        Memory_Free(gBgPalette);
        gBgPalette = 0;
    }
}


void Bg_FadePalette(f32 k, u16 *src) {
    u16 *dst;
    s32 i;
    u16 c;
    f32 r;
    f32 g;
    f32 b;
    u16 rr;
    u16 gg;
    u16 bb;

    dst = (u16 *)gBgPalette;
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

INCLUDE_ASM("asm/nonmatchings/unit_13490", Bg_DrawImage);

void Draw2D_SetColor(s8 arg0, s8 arg1, s8 arg2) {
    D_800F1FA0 = arg0;
    D_800F1FA1 = arg1;
    D_800F1FA2 = arg2;
}

extern u8 D_800F2030[];
void Draw2D_Begin(void) {
    GFX_RESERVE(3);
    gSPDisplayList(gDisplayListHead++, D_800F2030);
    gDPPipeSync(gDisplayListHead++);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, (u8)D_800F1FA0, (u8)D_800F1FA1, (u8)D_800F1FA2, 0xFF);
}

void Image_DrawRGBA16(s32 a, s32 b, s32 c, s32 d, s32 e) {
    if (d >= -a && e >= -b && a < gScreenWidth && b < gScreenHeight) {
        Image_DrawRGBA16Scaled(a, b, c, d, e, 1.0f, 1.0f);
    }
}

void Image_DrawRGBA16Scaled(s32 px, s32 py, s32 timg, s32 w, s32 h, f32 sx, f32 sy) {
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
            gDPLoadTextureTile(gDisplayListHead++, timg, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, h, x, y, x + tw - 1, y + th - 1, 0,
                               G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK,
                               G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(gDisplayListHead++, (px + (s32)(sx * x)) << 2, (py + (s32)(sy * y)) << 2,
                                (px + (s32)(sx * x) + (s32)(sx * tw)) << 2, (py + (s32)(sy * y) + (s32)(sy * th)) << 2,
                                G_TX_RENDERTILE, x << 5, y << 5, (u32)dsdx, (u32)dtdy);
        }
    }
}

void Image_BeginCI8(void) {
    GFX_RESERVE(1);
    gSPDisplayList(gDisplayListHead++, &D_800F2060);
}

void Image_DrawCI8Scaled(s32 px, s32 py, u32 timg, s32 w, s32 h, f32 sx, f32 sy) {
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
    gDPLoadTLUT_pal256(gDisplayListHead++, timg);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
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
            gDPLoadTextureTile(gDisplayListHead++, timg, G_IM_FMT_CI, G_IM_SIZ_8b, w, h, x, y, x + tw - 1, y + th - 1, 0,
                               G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK,
                               G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(gDisplayListHead++, (px + (s32)(sx * x)) << 2, (py + (s32)(sy * y)) << 2,
                                (px + (s32)(sx * x) + (s32)(sx * tw)) << 2, (py + (s32)(sy * y) + (s32)(sy * th)) << 2,
                                G_TX_RENDERTILE, x << 5, y << 5, (u32)dsdx, (u32)dtdy);
        }
    }
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
}

void Image_BeginIA4(void) {
    GFX_RESERVE(1);
    gSPDisplayList(gDisplayListHead++, &D_800F2090);
}

void Image_DrawIA4Scaled(s32 px, s32 py, u32 timg, s32 w, s32 h, f32 sx, f32 sy) {
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
            gDPLoadTextureTile_4b(gDisplayListHead++, timg, G_IM_FMT_IA, w, h, x, y, x + tw - 1, y + th - 1, 0,
                                  G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK,
                                  G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(gDisplayListHead++, (px + (s32)(sx * x)) << 2, (py + (s32)(sy * y)) << 2,
                                (px + (s32)(sx * x) + (s32)(sx * tw)) << 2, (py + (s32)(sy * y) + (s32)(sy * th)) << 2,
                                G_TX_RENDERTILE, x << 5, y << 5, (u32)dsdx, (u32)dtdy);
        }
    }
}

extern u8 D_800F20C0[];
void Draw2D_BeginPrim(s32 r, s32 g, s32 b, s32 a) {
    GFX_RESERVE(3);
    gDPPipeSync(gDisplayListHead++);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, r, g, b, a);
    gSPDisplayList(gDisplayListHead++, D_800F20C0);
}
