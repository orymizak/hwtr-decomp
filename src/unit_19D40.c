#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "gfx.h"
#include "include_asm.h"
typedef struct { u16 magic; u8 pad[0xA]; s32 c; s32 w10; u32 p14; u32 p18; u32 p1C; u32 p20; u32 p24; u32 p28; u32 p2C; s32 w30; } Hdr79_;
extern s32 D_80138E38;
void Debug_FormatNumber(u16, char *);
void Debug_DrawString(char *, s32, s32, s32);
typedef struct { u8 pad[0xC]; u8 r; u8 g; u8 b; s8 on; } Col16_;
typedef struct { u8 pad[0xC]; u8 r; u8 g; u8 b; u8 flags; } Col16b_;
typedef struct { u16 n; u8 pad[2]; Col16b_ *c; u32 p8; u32 pC; s32 w10; } Hdr7E_;
extern s32 gWorldSlots;
void GfxPools_Shutdown(void);
void WorldData_Free(s32);
typedef struct { u8 shift; } ColorShift_;
extern ColorShift_ gColorShift;
void MeshColors_Relocate(Hdr7E_ *h);
extern f64 D_800FF8E0;

s32 Memory_Alloc(s32);
f32 Math_Atan2(f32, f32);
extern s32 D_8014551C;

INCLUDE_ASM("asm/nonmatchings/unit_19D40", Mesh_BuildDisplayList);

typedef struct { u16 end; u16 posOff; u16 uvOff; u16 colOff; } MeshChunk_;
typedef struct { s16 x; s16 y; s16 z; } Pos3s_;
typedef struct { s16 u; s16 v; } Uv2s_;
typedef struct { u8 a; u8 b; } Col2b_;
typedef struct {
    /* 0x00 */ u8 p0[6];
    /* 0x06 */ u16 vtxCount;
    /* 0x08 */ u8 p8[0x14];
    /* 0x1C */ Pos3s_ *pos;
    /* 0x20 */ Uv2s_ *uv;
    /* 0x24 */ Col2b_ *col;
    /* 0x28 */ u8 *idx;
    /* 0x2C */ MeshChunk_ *chunks;
} Mesh_;

void Mesh_FillVtx(Mesh_ *m, Vtx *v) {
    MeshChunk_ *ch;
    s32 i;
    Pos3s_ *p;
    Uv2s_ *t;
    Col2b_ *c;
    u16 col;

    ch = m->chunks;
    for (i = 0; i < m->vtxCount; ch++) {
        while (i < ch->end) {
            p = &m->pos[m->idx[i * 3] + ch->posOff];
            t = &m->uv[m->idx[i * 3 + 1] + ch->uvOff];
            c = &m->col[m->idx[i * 3 + 2] + ch->colOff];

            v->v.ob[0] = p->x;
            v->v.ob[1] = p->y;
            v->v.ob[2] = p->z;
            v->v.flag = 0;
            v->v.tc[0] = t->u;
            v->v.tc[1] = t->v;

            col = (c->a << 8) + c->b;
            if ((col & 1) == 0) {
                v->v.cn[0] = (((s16)col >> 8) & 0xF8) + 4;
                v->v.cn[1] = (((s16)col >> 3) & 0xF8) + 4;
                v->v.cn[2] = ((col << 2) & 0xF8) + 4;
                v->v.cn[3] = 0xFF;
            } else {
                v->v.cn[0] = (((s16)col >> 8) & 0xF0) + 8;
                v->v.cn[1] = (((s16)col >> 4) & 0xF0) + 8;
                v->v.cn[2] = (((s8)col & 0xF0) + 8);
                v->v.cn[3] = 1;
                v->v.cn[0] >>= gColorShift.shift;
                v->v.cn[1] >>= gColorShift.shift;
                v->v.cn[2] >>= gColorShift.shift;
            }
            v++;
            i++;
        }
    }
}

void Mesh_Relocate(Hdr79_ *h) {
    if (h->p14 != 0) {
        h->p14 += (u32)h;
        MeshColors_Relocate(h->p14);
    }
    h->c = 0;
    h->magic = 0xF6B1;
    h->w30 = 0;
    h->p18 += (u32)h;
    h->p1C += (u32)h;
    h->p20 += (u32)h;
    h->p24 += (u32)h;
    h->p28 += (u32)h;
    h->p2C += (u32)h;
}


s32 Mesh_AllocDisplayList(u8 *m) {
    return Memory_Alloc(*(u16 *)(m + 4) * 8);
}

s32 Mesh_AllocVtx(u8 *m) {
    s32 buf;
    u16 n = *(u16 *)(m + 6);

    buf = 0;
    if (n != 0) {
        buf = Memory_Alloc(n * 0x10);
    }
    return buf;
}

void Mesh_CopyColors(Col16_ *src, Col16_ *dst, u32 n) {
    u32 i;

    for (i = 0; i < n; i++, src++) {
        if (src->on != 0) {
            dst->r = src->r << 3;
            dst->g = src->g << 3;
            dst->b = src->b << 3;
        }
        dst++;
    }
}


void MeshColors_Relocate(Hdr7E_ *h) {
    u8 *sh;
    s32 i;

    h->w10 = 0;
    h->c = (Col16b_ *)((u32)h->c + (u32)h);
    h->p8 += (u32)h;
    h->pC += (u32)h;
    for (i = 0; i < h->n * 3; i++) {
        sh = &gColorShift.shift;
        if (h->c[i].flags & 1) {
            h->c[i].r >>= *sh;
            h->c[i].g >>= *sh;
            h->c[i].b >>= *sh;
        }
    }
}


void *Mesh_Alloc(u16 *n) {
    return ((void * (*)(s32 size))Memory_Alloc)(*n * 12 + *n * 160 + *n * 48 + 16);
}


typedef struct {
    u32 key;
    Gfx *dlStart;
    Gfx *dlEnd;
} MatEntry_;

typedef struct {
    union {
        struct {
            s8 hi : 4;
            s8 lo : 4;
        } f;
        s8 raw;
    } u;
    u8 b1;
    u8 b2;
} Tri3_;

typedef struct {
    u16 count;
    u16 pad2;
    Vtx *vtx;
    void *p8;
    Tri3_ *triData;
    MatEntry_ *entries;
} SubMesh_;

typedef struct {
    u8 w;
    u8 h;
    u8 pad[6];
} Tex8_;

typedef struct { u8 v; } LowRes_;
extern LowRes_ gLowRes;
Gfx *Gfx_LoadTexture(Gfx *dl, Tex8_ *tex);

void Mesh_BuildMaterialDL(SubMesh_ *sub, Tex8_ *tex, u8 alpha) {
    MatEntry_ *ent;
    Vtx *vtxBuf;
    Gfx *dl;
    s32 i;
    u8 a;
    u32 k;

    ent = sub->entries;
    if (ent != NULL) {
        vtxBuf = (Vtx *)(((u32)ent + sub->count * 12 + 7) & ~7);
        dl = (Gfx *)(((u32)vtxBuf + sub->count * 48 + 7) & ~7);
        for (i = 0; i < sub->count * 3; i++) {
            vtxBuf[i] = sub->vtx[i];
        }
        for (i = 0; i < sub->count; i++) {
            ent[i].key = (sub->triData[i].b2 << 8) | sub->triData[i].b1 |
                         ((u8)sub->triData[i].u.f.lo << 16) | ((u8)sub->triData[i].u.f.hi << 20);
            ent[i].dlStart = dl;
            a = (sub->triData[i].u.f.hi << 4) | 0xF;
            if (alpha != 0xFF) {
                if (alpha < a) {
                    a = alpha;
                }
            }
            switch (sub->triData[i].u.f.lo) {
                case 0:
                    k = (u16)ent[i].key;
                    gDPSetPrimColor(dl++, 0, 0, 0xFF, 0xFF, 0xFF, a);
                    gDPSetCombineLERP(dl++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                    if (gLowRes.v == 1) {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                    } else {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2);
                    }
                    dl = Gfx_LoadTexture(dl, &tex[k]);
                    gSPTexture(dl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
                    gSPClearGeometryMode(dl++, G_LIGHTING | G_TEXTURE_GEN);
                    gSPSetGeometryMode(dl++, G_FOG);
                    break;
                case 1:
                    k = (u16)ent[i].key;
                    gDPSetPrimColor(dl++, 0, 0, 0xFF, 0xFF, 0xFF, a);
                    gDPSetCombineLERP(dl++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                    if (gLowRes.v == 1) {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                    } else {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2);
                    }
                    dl = Gfx_LoadTexture(dl, &tex[k]);
                    gSPTexture(dl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
                    gSPClearGeometryMode(dl++, G_TEXTURE_GEN);
                    gSPSetGeometryMode(dl++, G_FOG | G_LIGHTING);
                    break;
                case 2:
                    k = ent[i].key;
                    gDPSetPrimColor(dl++, 0, 0, ((s32)k >> 7) & 0xF8, ((s32)k >> 2) & 0xF8, (k << 3) & 0xF8, a);
                    gDPSetCombineLERP(dl++, PRIMITIVE, 0, SHADE, 0, 0, 0, 0, PRIMITIVE, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                    if (gLowRes.v == 1) {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                    } else {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2);
                    }
                    gSPTexture(dl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
                    gSPClearGeometryMode(dl++, G_LIGHTING | G_TEXTURE_GEN);
                    gSPSetGeometryMode(dl++, G_FOG);
                    break;
                case 3:
                    k = ent[i].key;
                    gDPSetPrimColor(dl++, 0, 0, ((s32)k >> 7) & 0xF8, ((s32)k >> 2) & 0xF8, (k << 3) & 0xF8, a);
                    gDPSetCombineLERP(dl++, PRIMITIVE, 0, SHADE, 0, 0, 0, 0, PRIMITIVE, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                    if (gLowRes.v == 1) {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                    } else {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2);
                    }
                    gSPTexture(dl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
                    gSPClearGeometryMode(dl++, G_TEXTURE_GEN);
                    gSPSetGeometryMode(dl++, G_FOG | G_LIGHTING);
                    break;
                case 5:
                    k = (u16)ent[i].key;
                    gDPSetPrimColor(dl++, 0, 0, 0xFF, 0xFF, 0xFF, a);
                    gDPSetCombineLERP(dl++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                    if (gLowRes.v == 1) {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                    } else {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2);
                    }
                    dl = Gfx_LoadTexture(dl, &tex[k]);
                    gSPTexture(dl++, 0x10000 / tex[k].w, 0x10000 / tex[k].h, 0, G_TX_RENDERTILE, G_ON);
                    gSPSetGeometryMode(dl++, G_FOG | G_LIGHTING | G_TEXTURE_GEN);
                    break;
                case 4:
                    k = (u16)ent[i].key;
                    gDPSetPrimColor(dl++, 0, 0, 0xFF, 0xFF, 0xFF, a);
                    gDPSetCombineLERP(dl++, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                    if (gLowRes.v == 1) {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
                    } else {
                        gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2);
                    }
                    dl = Gfx_LoadTexture(dl, &tex[k]);
                    gSPTexture(dl++, 0x10000 / tex[k].w, 0x10000 / tex[k].h, 0, G_TX_RENDERTILE, G_ON);
                    gSPClearGeometryMode(dl++, G_LIGHTING);
                    gSPSetGeometryMode(dl++, G_FOG | G_TEXTURE_GEN);
                    break;
            }
            ent[i].dlEnd = dl;
            ent[i].key ^= (u32)tex;
            gSPVertex(dl++, &vtxBuf[i * 3], 3, 0);
            gSP1Triangle(dl++, 0, 1, 2, 0);
            gDPPipeSync(dl++);
            gSPEndDisplayList(dl++);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_19D40", Mesh_GetMaterialList);

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
/* Re-orthonormalises the rotation part of a 3x3 matrix, keeping the direction of its first column. */
void Mtx3_Orthonormalize(f32 *m) {
    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f t;
    Vec3f u;

    a.x = m[0];
    a.y = m[3];
    a.z = m[6];
    b.x = m[1];
    b.y = m[4];
    b.z = m[7];
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
    m[3] = a.y;
    m[4] = b.y;
    m[5] = c.y;
    m[6] = a.z;
    m[7] = b.z;
    m[8] = c.z;
}

f32 Math_Asin(f32 x) {
    f32 c;
    f32 r;

    if (fabsf(x) > 1.0f) {
        return 0.0f;
    }
    c = sqrtf(1.0f - x * x);
    if (c != 0.0f) {
        return Math_Atan2(x, c);
    }
    r = 1.5707964f;
    if (!(x > 0.0f)) {
        r = -1.5707964f;
    }
    return r;
}

f32 Math_Acos(f32 x) {
    f32 s;

    if (fabsf(x) > 1.0f) {
        return 0.0f;
    }
    s = sqrtf(1.0f - x * x);
    if (s != 0.0f) {
        return D_800FF8E0 - Math_Atan2(x, s);
    }
    if (x > 0.0f) {
        return 0.0f;
    }
    return 3.1415927f;
}

f32 Math_Atan2(f32 y, f32 x) {
    f32 t;
    f32 a;
    f32 s;
    f32 r;
    s32 i;

    if ((x == 0.0f) & (y == 0.0f)) {
        return 0.0f;
    }
    if (x == 0.0f) {
        if (y > 0.0f) {
            return 1.5707964f;
        }
        return -1.5707964f;
    }
    /* TODO(match-debt): both arms are identical (x is already known non-zero); the redundant
       test only changes FP register allocation. */
    if (x) {
        t = y / x;
        a = t;
    } else {
        t = y / x;
        a = t;
    }
    if (!(fabsf(t) <= 1.0f)) {
        a = 1.0f / t;
    }
    s = 0.0f;
    for (i = 14; i > 0; i--) {
        s = (f32) (i * i) * a * a / ((f32) (i * 2 + 1) + s);
    }
    r = a / (s + 1.0f);
    if (t > 1.0f) {
        r = 1.5707963267948966 - r;
    }
    if (t < -1.0f) {
        r = -1.5707963267948966 - r;
    }
    if (x > 0.0f) {
        return r;
    }
    if (y > 0.0f) {
        return r + 3.141592653589793;
    }
    return r - 3.141592653589793;
}

f32 Math_Abs(f32 x) {
    if (0.0f <= x) {
        return x;
    }
    return -x;
}


void Mtx34_Mul(f32 a[3][4], f32 b[3][4], f32 out[3][4]) {
    out[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0];
    out[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1];
    out[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2];
    out[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3];
    out[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0];
    out[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1];
    out[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2];
    out[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3];
    out[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0];
    out[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1];
    out[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2];
    out[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3];
}

/* One Controller Pak note (0x20): size in bytes, extension letter and name. */
typedef struct { u32 size; u32 gameCode; u16 compCode; s8 ext; u8 pB[3]; u8 name[16]; u8 p1E[2]; } PakNote_;
extern PakNote_ D_80138B20[16];
extern s32 D_80148048[16];
extern u8 *D_800F21B0[];
extern u16 D_800F21BC[];
extern s16 D_800F21C4[];
extern u8 D_800FF92C[];
void Debug_DrawChar(s32 c, u16 color, u16 shadow);
void Debug_DrawChars(u8 *s, s32 n, u16 color, u16 shadow);
#define PAK_FB ((u16 *)0x80300000)

static inline void PakMenu_SetCursor(s32 x, s32 y) {
    D_80138E38 = (s32)(PAK_FB + (y * 640 + x));
}

/* Draws row item of the Controller Pak menu (640-wide debug framebuffer): number, note name,
 * size in pages and the note type label; row 16 is the exit line. */
void PakMenu_Draw(s32 type, s32 item, u16 color, u16 typeColor, u16 shadow) {
    char buf[8];
    s32 y = item * 20 + 84;

    if (item < 16) {
        if (D_80148048[item] == 0) {
            PakMenu_SetCursor(126, y);
            Debug_DrawChars(D_80138B20[item].name, 16, color, shadow);
            if (D_80138B20[item].ext != 0) {
                Debug_DrawChar(0x3C, color, shadow);
                D_80138E38 = (s32)((u16 *)D_80138E38 - 8);
                Debug_DrawChar(D_80138B20[item].ext, color, shadow);
            }
            PakMenu_SetCursor(406, y);
            Debug_FormatNumber((D_80138B20[item].size + 255) >> 8, buf);
            Debug_DrawString(buf, 3, color, shadow);
            D_80138E38 = (s32)((u16 *)D_80138E38 + D_800F21C4[type]);
            Debug_DrawString(D_800F21B0[type], D_800F21BC[type], typeColor, shadow);
        }
        if (item < 9) {
            PakMenu_SetCursor(99, y);
            Debug_DrawChar(item + 0x11, color, shadow);
        } else {
            PakMenu_SetCursor(92, y);
            Debug_DrawChar(0x11, color, shadow);
            PakMenu_SetCursor(100, y);
            Debug_DrawChar(item + 7, color, shadow);
        }
        PakMenu_SetCursor(111, y);
        Debug_DrawChar(0x3E, color, shadow);
    } else {
        PakMenu_SetCursor(94, (item * 20 + 88));
        Debug_DrawString(D_800FF92C, 4, color, shadow);
    }
}

void Debug_ShowNumber(u16 n) {
    char buf[8];
    s32 y;
    s32 x;

    Debug_FormatNumber(n, buf);
    D_80138E38 = 0x8037FB2C;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 48; x++) {
            ((u16 *)0x8037FB2C)[y * 640 + x] = 0;
        }
    }
    Debug_DrawString(buf, 3, 0xFFFF, 1);
}


typedef struct { void *pfs; s32 file_no; s32 error; s32 type; } PakFile_;
extern s32 gViModeIndex;
extern OSViMode D_800FB920[];
extern OSViMode osViModeTable[];
extern char D_800FF934[];
extern char D_800FF94C[];
extern char D_800FF968[];
extern char D_800FF980[];
extern char D_800FF998[];
extern char D_800FF9B0[];
extern char D_800FF9BC[];
extern u16 D_800F21CC[];
void Debug_InitFont(void *);
void nuGfxRetraceWait(s32);
void nuContDataRead(OSContPad *);
void nuContDataReadStart(void);
void nuContDataReadWait(void);
void nuContDataGetAll(OSContPad *);
void nuContPakOpen(PakFile_ *, u32);
void nuContPakRepairId(PakFile_ *);
void nuContPakFileNum(PakFile_ *, s32 *, s32 *);
void nuContPakFileState(PakFile_ *, void *);
s32 nuContPakGetFree(PakFile_ *);
void nuContPakCodeSet(u8 *, u8 *);
void nuContPakFileDelete(PakFile_ *, u8 *, u8 *);

u32 PakMenu_Run(s32 ctrl) {
    OSContPad pads[4];
    PakFile_ file;
    s32 maxFiles;
    s32 usedFiles;
    s32 i;
    u32 status;
    u32 special;
    s32 savedMode;
    void *savedFb;
    s32 freePages;
    s32 screenMode;
    s32 item;
    s32 prevItem;
    s32 mode;
    s32 prevMode;
    s32 flash;
    u16 btn;
    u16 rep;
    u16 holdCount;
    u16 col;
    u16 typeCol;

    special = 0;
    nuContPakOpen(&file, ctrl);
    mode = file.error;
    if (mode != 0 && mode != 10) {
        return 0;
    }

    Debug_InitFont((void *)0x80200000);
    for (i = 0; i < 0x25800U; i++) {
        ((u32 *)0x80300000)[i] = 0;
    }

    status = osViGetStatus();
    if (status & 8) {
        special |= 1;
    } else {
        special |= 2;
    }
    if (status & 4) {
        special |= 4;
    } else {
        special |= 8;
    }
    if (status & 0x10) {
        special |= 0x10;
    } else {
        special |= 0x20;
    }
    if (status & 0x10000) {
        special |= 0x40;
    } else {
        special |= 0x80;
    }

    savedFb = osViGetCurrentFramebuffer();
    osViSwapBuffer((void *)0x80300000);
    savedMode = osViGetCurrentMode();
    osViSetMode(&D_800FB920[gViModeIndex]);
    osViBlack(0);

    if (mode == 10) {
        D_80138E38 = 0x8032BD40;
        Debug_DrawString(D_800FF934, 0x14, 0xF801, 0x52BF);
        D_80138E38 = 0x80351500;
        Debug_DrawString(D_800FF94C, 0x1B, 0x52BF, 0x52BF);
        D_80138E38 = 0x803578C0;
        Debug_DrawString(D_800FF968, 0x16, 0x52BF, 0x52BF);
        osWritebackDCacheAll();
        nuGfxRetraceWait(1);
        do {
            nuContDataRead(pads);
            if (pads[ctrl].button != 0) {
                if (pads[ctrl].button == 0x30) {
                    nuContPakRepairId(&file);
                }
                nuContPakOpen(&file, ctrl);
                mode = file.error;
            }
        } while (mode != 0);
    }

    nuContPakFileNum(&file, &maxFiles, &usedFiles);
    if (file.error != 0) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        file.file_no = i;
        nuContPakFileState(&file, &D_80138B20[i]);
        D_80148048[i] = file.error;
    }
    freePages = nuContPakGetFree(&file);
    if (file.error != 0) {
        return 0;
    }
    goto start_menu;

err_screen:
    screenMode = 1;
    goto redraw;

start_menu:
    screenMode = 0;
    freePages >>= 8;
redraw:
    for (i = 0; i < 0x25800U; i++) {
        ((u32 *)0x802D4000)[i] = 0;
    }
    switch (screenMode) {
    case 0:
        D_80138E38 = 0x8030B4C0;
        Debug_DrawString(D_800FF980, 0x14, 0x52BF, 1);
        D_80138E38 = 0x803140C0;
        Debug_DrawString(D_800FF998, 0x17, 0xFFFF, 1);
        D_80138E38 = 0x8037FA24;
        Debug_DrawString(D_800FF9B0, 9, 0xFFFF, 1);
        Debug_ShowNumber(freePages);
        for (i = 0; i < 17; i++) {
            PakMenu_Draw(0, i, 0xFFFF, 1, 1);
        }
        break;
    case 1:
        D_80138E38 = 0x8033E940;
        Debug_DrawString(D_800FF9BC, 0x10, 0x52BF, 0x52BF);
        usedFiles = 0;
        break;
    }

    osWritebackDCacheAll();
    holdCount = 0;
    nuGfxRetraceWait(1);
    rep = 0;
    item = 16;
    btn = 0x400;
    flash = 0;
    mode = 0;

    while (1) {
        prevItem = item;
        prevMode = mode;
        switch (mode) {
        case 0:
            if (btn & 0x8000) {
                if (item == 16) {
                    osViBlack(1);
                    osViSetMode(&osViModeTable[savedMode]);
                    osViSetSpecialFeatures(special);
                    osViSwapBuffer(savedFb);
                    osViBlack(0);
                    return (screenMode == 1) ? 1 : 3;
                }
                for (i = 0; i < 17; i++) {
                    PakMenu_Draw(0, i, 0x843, 1, 1);
                }
                mode = 2;
            } else if ((btn | rep) & 0x800) {
                item--;
                while (D_80148048[item] != 0) {
                    if (item < 0) {
                        break;
                    }
                    item--;
                }
                if (item == -1) {
                    item = 16;
                }
            } else if ((btn | rep) & 0x400) {
            next_item:
                if (item == 16) {
                    item = -1;
                }
                item++;
                while (D_80148048[item] != 0) {
                    if (item >= 16) {
                        break;
                    }
                    item++;
                    if (D_80148048[item] == 0) {
                        break;
                    }
                }
            } else if (btn & 0x4000) {
                item = 16;
            }
            break;
        case 1:
            if (btn & 0x400) {
                mode = 2;
            }
            if (btn & 0x4000) {
                mode = 0;
            }
            if (btn & 0x8000) {
                nuContPakCodeSet((u8 *)&D_80138B20[item].compCode, (u8 *)&D_80138B20[item].gameCode);
                nuContPakFileDelete(&file, D_80138B20[item].name, (u8 *)&D_80138B20[item].ext);
                if (file.error != 0) {
                    goto err_screen;
                }
                PakMenu_Draw(1, item, 1, 1, 1);
                PakMenu_Draw(2, item, 1, 1, 1);
                D_80148048[item] = -1;
                freePages += ((D_80138B20[item].size + 0xFF) >> 8) & 0xFF;
                Debug_ShowNumber(freePages);
                mode = 0;
                goto next_item;
            }
            break;
        case 2:
            if (btn & 0x800) {
                mode = 1;
            }
            if (btn & 0xC000) {
                mode = 0;
            }
            break;
        }

        if (usedFiles == 0) {
            item = 16;
        }
        if (mode != prevMode) {
            typeCol = 0xFFFF;
            if (mode == 0) {
                for (i = 0; i < 17; i++) {
                    PakMenu_Draw(0, i, 0xFFFF, 1, 1);
                }
                typeCol = 1;
            }
            PakMenu_Draw(1, prevItem, 0xFFFF, typeCol, 1);
            PakMenu_Draw(2, prevItem, 0xFFFF, typeCol, 1);
            flash = 0;
        } else if (item != prevItem) {
            PakMenu_Draw(0, prevItem, 0xFFFF, 1, 1);
            flash = 0;
        }

        col = D_800F21CC[flash / 3];
        PakMenu_Draw(mode, item, col, col, col);
        flash++;
        if (flash == 30) {
            flash = 0;
        }

        nuContDataReadStart();
        osWritebackDCacheAll();
        nuGfxRetraceWait(1);
        nuContDataReadWait();

        if (pads[ctrl].stick_y > 8) {
            pads[ctrl].button |= 0x800;
        }
        if (pads[ctrl].stick_y < -8) {
            pads[ctrl].button |= 0x400;
        }
        btn = pads[ctrl].button;
        nuContDataGetAll(pads);
        if (pads[ctrl].stick_y > 8) {
            pads[ctrl].button |= 0x800;
        }
        if (pads[ctrl].stick_y < -8) {
            pads[ctrl].button |= 0x400;
        }
        rep = pads[ctrl].button & btn;
        if (rep != 0) {
            holdCount++;
            if ((holdCount >> 5) == 0 || (holdCount & 7) != 0) {
                rep = 0;
            }
        } else {
            holdCount = 0;
        }
        btn = pads[ctrl].button & (pads[ctrl].button ^ btn);
    }
}

s32 World_InitSlots(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        (&gWorldSlots)[i] = 0;
    }
    return 1;
}

extern char D_800FF9D0[];
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
s32 File_LoadAlloc(char *);
s32 WorldData_Relocate(s32);

s32 World_Load(u8 i, char *name) {
    char buf[64];
    s32 *slots;
    s32 *p;

    if (i < 32) {
        slots = &gWorldSlots;
        p = &slots[i];
        strcpy(buf, name);
        strcat(buf, D_800FF9D0);
        *p = WorldData_Relocate(File_LoadAlloc(buf));
    }
    return 1;
}

s32 World_Free(u8 i) {
    s32 *p;

    if (i < 32) {
        if (((s32 *)&gWorldSlots)[i] == 0) {
            return 1;
        }
        GfxPools_Shutdown();
        WorldData_Free(((s32 *)&gWorldSlots)[i]);
        ((s32 *)&gWorldSlots)[i] = 0;
        return 1;
    }
    return 1;
}

typedef struct { u8 p0[0x2C]; u8 *root; u8 p30[4]; s32 f34; } WorldD_;
extern u8 D_800F2138;
extern s32 D_800F216C[];
void Gfx_SetLookAt(void *);
void Light_Reset(void);
void Gfx_UpdateLights(void);
void SceneNode_DrawMesh(u8 *, void *, s32, s32, s32);

s32 World_DrawSlot(u8 i) {
    WorldD_ **slots;
    WorldD_ **p;
    WorldD_ *world;
    u8 *root;
    void *cam;
    Gfx *gfx;

    if (i < 32) {
        slots = (WorldD_ **)&gWorldSlots;
        p = &slots[i];
        if (*p == NULL) {
            return 1;
        }
        root = (*p)->root;
        cam = &D_800F2138;
        Gfx_SetLookAt(cam);
        Light_Reset();
        Gfx_UpdateLights();
        GFX_RESERVE(1);
        gfx = gDisplayListHead;
        gDPSetTexturePersp(gfx, G_TP_PERSP);
        world = *p;
        gDisplayListHead = gfx + 1;
        SceneNode_DrawMesh(root, cam, D_800F216C[0], 0, world->f34);
    }
    return 1;
}
