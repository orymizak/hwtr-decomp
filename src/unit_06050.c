#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
#include "gfx.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } V16_;
typedef struct { V16_ q; V12_ v; } E1C_;
typedef struct { s32 a; u16 node; u16 h6; s32 b; s32 c; E1C_ *p; } E14_;
typedef struct { s32 idx; s32 a; s32 b; s32 c; } Rec16_;
typedef struct { Vec3f v; f32 s; u16 node; char name[0xE]; } Ent20_;
extern void *gTrackSky;
void WorldData_Free(void *);
void Clouds_Free(void);
void Explosions_Clear(void);
extern char D_800FE9F0[];
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
s32 File_Load(char *, s32);
extern void *D_80101E84;
void Memory_Free(void *);
typedef struct { s32 a; s32 b; s32 c; s32 d; } Vec4L_;
u8 Model_Load(u8 id, char *name, s32 unused);
typedef struct { u16 unk0; s16 x; s16 y; s16 z; s32 unk8; s32 unkC; s32 unk10; s32 unk14; } Node_;
typedef struct { Vec3i p; f32 m[9]; s32 d; f32 e; } Cam_;
typedef struct { f32 m[21]; } Node54_;
typedef struct {
    /* 0x00 */ u8 p0[4];
    /* 0x04 */ u16 n4;
    /* 0x06 */ u16 n6;
    /* 0x08 */ u8 p8[3];
    /* 0x0B */ u8 bB;
    /* 0x0C */ u16 nC;
    /* 0x0E */ u16 nE;
    /* 0x10 */ u16 n10;
    /* 0x12 */ u8 p12[2];
    /* 0x14 */ Vec3f v14;
    /* 0x20 */ Vec3f v20;
    /* 0x2C */ Node54_ *nodes;
    /* 0x30 */ u8 p30[4];
    /* 0x34 */ void *verts;
    /* 0x38 */ Node_ *links;
    /* 0x3C */ u8 p3C[4];
    /* 0x40 */ Cam_ *cams;
    /* 0x44 */ E14_ *paths;
    /* 0x48 */ Ent20_ *ents;
    /* 0x4C */ Rec16_ *recs;
} World_;
typedef struct { s32 d[0x114 / 4]; } Big114_;
typedef struct { s32 d[0x40 / 4]; } Big40_;
typedef struct { u8 p0[9]; u8 b9; } MdlMat_;
typedef struct { u8 p0[0x30]; s32 flags; u8 p34[8]; MdlMat_ *mat; u8 p40[0x44]; s32 flags84; } MdlInst_;
typedef struct {
    /* 0x000 */ u8 p0[0x2C];
    /* 0x02C */ MdlInst_ *inst;
    /* 0x030 */ u8 p30[0xCC];
    /* 0x0FC */ u8 nParts;
    /* 0x0FD */ u8 pFD;
    /* 0x0FE */ s8 id;
    /* 0x0FF */ u8 pFF;
    /* 0x100 */ u8 cur;
    /* 0x101 */ u8 tgt;
    /* 0x102 */ u8 r;
    /* 0x103 */ u8 g;
    /* 0x104 */ u8 b;
    /* 0x105 */ u8 b105;
    /* 0x106 */ u8 p106[7];
    /* 0x10D */ u8 b10D;
    /* 0x10E */ u8 b10E;
    /* 0x10F */ u8 p10F[0x2D];
    /* 0x13C */ Vec4L_ v;
    /* 0x14C */ u8 b14C;
    /* 0x14D */ u8 c[3];
    /* 0x150 */ u8 p150[0x10];
    /* 0x160 */ f32 mtx[7][3][4]; /* 0: model, then one per part */
} Model_;
typedef struct { s32 cur[6]; s32 tgt[6]; s32 dt; } Fade65_;
typedef struct { f32 f0; s32 i4; f32 f8; f32 fC; u8 p10[0x30]; Vec3f pos; Vec3f vel; f32 f58; f32 f5C; u8 p60[4]; s16 h64; u8 b66; u8 c[3]; u8 b6A; u8 p6B; u16 s6C; s16 s6E; } Pt94_;
typedef struct { u8 p0[0xC]; f32 x; u8 p10[0xC]; f32 y; u8 p20[0xC]; f32 z; u8 p30[8]; f32 r; } O_9A4;
void Particle_SpawnJitteredVel(Vec3f, Vec3f);
void Particle_SpawnWithVel(Vec3f, Vec3f);
/* One block from D_80138190: per-slot part flags, wreck flag, palette and fade state that splat split into
 * D_80138A6C, D_80138A72 and D_80138A78. */
typedef struct {
    s8 partA[6][6];    /* wheel contact: 0 none, 1 first frame, 2 held */
    s8 partB[6][6];    /* frames in contact, 0-80 */
    u8 partC[6][6];    /* surface under the wheel */
    struct { Vec3f pos; Vec3f n; Vec3f lastPos; Vec3f lastN; } contact[6][6];
    Vec3f touchPos[6][6]; /* where the current contact started */
    u8 wrecked[6];
    u8 palette[6];
    s32 fadeCur[6];
    s32 fadeTgt[6];
    s32 contactOn;     /* D_80138AA8: wheel contacts are tracked */
    u8 b91C[5];        /* D_80138AAC..AB0 */
    u8 partScaleOn;    /* D_80138AB1: scale the part matrices by partScale */
    u8 p922[2];
    f32 partScale;     /* D_80138AB4 */
} ModelSlotState_;
extern ModelSlotState_ D_80138190;
extern s32 D_80138A90[];
extern u8 gColorShift;
void ParticlePool_Push(void *, Pt94_ *);
extern Fade65_ D_80138A78;
extern u8 D_80138AAC;
extern u8 gParticles;
extern u8 gParticlesAlt;
extern u8 D_80145488;
void ParticlePool_Update(void *);
void Explosions_Update(s32);
void ModelData_Update(void *, s32);
void PointLight_SetColor(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void PointLight_Enable(s32);
void PointLight_Disable(s32);
void Model_UpdateWheelEffects(void);
s32 rand(void);
void Mesh_GetMaterialList(u8 *, u8 *, s32 *, s32);
void Particle_SpawnBurst(Vec3f, u32, u8 *, Vec3f);
s32 File_GetSize(char *);
extern char D_800FE964[];
s32 CarInfo_LoadEx(s32 unused, char *name, Big114_ *a, Big40_ *b);
extern u8 D_80138150[];
extern u8 D_80138156[];
extern f32 gLodDist0;
extern f32 gLodDist1;
extern f32 gLodDist2;
extern f32 gLodDist3;
extern s32 gModelLoadHandles[];
extern s32 gModelLoading[];
u8 File_PollAsync(s32);
s32 ModelData_Load(s32);
void Model_SetPartRotation(u8, u8, f32, f32, f32);
s32 Memory_Alloc();
extern u8 gModelBuffers;

extern World_ *gTrackWorld;
extern s32 D_8013818C;
s32 GfxPools_Shutdown();
s32 ModelData_FreeCaches(s32);
s32 ModelData_Free(s32);
extern Model_ *gModels[6];
s32 Model_Draw(u8);
/* Camera block from screen.c: view matrix followed by draw state; 0x4C is passed to ModelData_Draw. */
typedef struct {
    Mtx34f view;
    u8 pad30[0x1C];
    s32 f4C;
} CameraState_;
extern CameraState_ D_800F2138;
s32 Gfx_SetLookAt(CameraState_ *);

extern u8 D_80138AAD;
extern u8 D_80138AAE;
extern u8 D_80138AAF;
extern u8 D_80138AB0;
extern u8 D_80138AB1;
extern f32 D_80138AB4;
void Model_ResetSlot(u8);
void World_SetupLod(s32 a, s32 b);
void PointLights_Reset(void);
void Gfx_FlipFrameParity(void);
s32 World_Init(void) {
    void **p = (void **)&gTrackWorld;
    void **q = p + 2;
    s32 i;

    *p = NULL;
    gTrackSky = NULL;
    D_8013818C = 0;
    D_80138AAC = 0;
    for (i = 0; i < 6; i++) {
        q[i] = NULL;
        Model_ResetSlot(i);
    }
    D_80138AAD = 0;
    D_80138AAE = 0;
    D_80138AAF = 0;
    D_80138AB0 = 0;
    D_80138AB1 = 0;
    D_80138AB4 = 2.0f;
    World_SetupLod(1, 1);
    PointLights_Reset();
    Gfx_FlipFrameParity();
    return 1;
}

s32 Model_AllocBuffers(void) {
    if ((*(s32 *)((u8 *)&gModelBuffers + 0x0)) == 0) {
        (*(s32 *)((u8 *)&gModelBuffers + 0x0)) = Memory_Alloc(0x12C00);
    }
    if ((*(s32 *)((u8 *)&gModelBuffers + 0x4)) == 0) {
        (*(s32 *)((u8 *)&gModelBuffers + 0x4)) = Memory_Alloc(0x12C00);
    }
    return ((*(s32 *)((u8 *)&gModelBuffers + 0x0)) == 0) + ((*(s32 *)((u8 *)&gModelBuffers + 0x4)) == 0);
}

s32 Model_FreeBuffers(void) {
    void **p = (void **)&gModelBuffers;

    Memory_Free(*p);
    Memory_Free(D_80101E84);
    *p = NULL;
    D_80101E84 = NULL;
    return 0;
}

typedef struct { u8 p0[4]; char name[0x13]; u8 variant; } TrackName_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } Vec4LX_;
typedef struct { u8 p0[0x2C]; void *inst; u8 p30[0xCC]; u8 nParts; u8 pFD; s8 id; u8 pFF; u8 cur; u8 tgt; u8 r; u8 g; u8 b; } ModelX_;
void Model_SetHeadlights(u8 idx, u8 on, u8 fade);
extern char D_800FE944[];
extern char *gTrackColorNames[8];
extern u8 gTrackColors[8][3];
s32 strcmp(const char *a, const char *b);
char *strlwr(char *);
s32 Track_Unload(void);
s32 File_LoadAlloc(char *);
World_ *WorldData_Relocate(s32);
void WorldData_SetMirrored(World_ *);
void EnvMap_Apply(World_ *);
void Track_LoadSky(char *, u8);
void Clouds_Init(Vec3f *, Vec3f *, u8, u8, u8);

/* Loads "<name><variant>.WLD" and applies the matching per-track RGB color (white if none). */
s32 Track_Load(TrackName_ *t) {
    char buf[64];
    World_ **wp = &gTrackWorld;
    u8 r;
    u8 g;
    u8 b;
    u32 i;

    sprintf(buf, D_800FE944, t->name, t->variant);
    if (*wp != NULL) {
        Track_Unload();
    }
    *wp = WorldData_Relocate(File_LoadAlloc(buf));
    if (D_80138AB0 == 1) {
        WorldData_SetMirrored(*wp);
    }
    EnvMap_Apply(*wp);
    b = 255;
    g = 255;
    r = 255;
    Track_LoadSky(t->name, t->variant);
    for (i = 0; i < 8; i++) {
        strlwr(buf);
        if (strcmp(buf, gTrackColorNames[i]) == 0) {
            r = gTrackColors[i][0];
            g = gTrackColors[i][1];
            b = gTrackColors[i][2];
            break;
        }
    }
    Clouds_Init(&gTrackWorld->v14, &gTrackWorld->v20, r, g, b);
    return 1;
}

extern char D_800FE950[];
void Screen_SetClearColor(u8, u8, u8);

void Track_LoadSky(char *name, u8 variant) {
    char buf[64];
    u16 *pal = NULL;
    World_ **wp;
    s32 c;
    World_ *sky;
    s8 r;
    s8 g;
    s8 b;

    sprintf(buf, D_800FE950, name, variant);
    sky = WorldData_Relocate(File_LoadAlloc(buf));
    wp = (World_ **)&gTrackSky;
    *wp = sky;
    if (D_80138AB0 == 1) {
        WorldData_SetMirrored(*wp);
    }
    if (((World_ *)gTrackSky)->n4 != 0) {
        pal = ((u16 **)((World_ *)gTrackSky)->verts)[3];
    }
    c = *pal;
    /* TODO(match-debt): the f64 round-trip and the masked u32 copy reproduce an inlined
     * RGBA5551 -> RGB888 unpack helper; a plain shift does not match. */
    {
        u32 cc = c & 0xFFFF;

        r = (f64)(s32)((cc >> 8) & 0xF8);
        g = (f64)(s32)((cc >> 3) & 0xF8);
        b = (f64)((c << 2) & 0xF8);
    }
    Screen_SetClearColor(r >> gColorShift, g >> gColorShift, b >> gColorShift);
    EnvMap_Apply(*wp);
}

s32 Track_Unload(void) {
    void **p = (void **)&gTrackWorld;

    WorldData_Free(*p);
    WorldData_Free(gTrackSky);
    Clouds_Free();
    Explosions_Clear();
    *p = NULL;
    gTrackSky = NULL;
    return 1;
}

u16 Track_GetMarkerCount(void) {
    return gTrackWorld->n10;
}

void Track_GetMarker(s32 i, Vec3f *pos, s32 *a, s32 *b, s32 *c) {
    Rec16_ *r = &gTrackWorld->recs[i];
    f32 *m = gTrackWorld->nodes[r->idx].m;

    pos->x = m[3];
    pos->y = m[7];
    pos->z = m[11];
    *a = r->a;
    *b = r->b;
    *c = r->c;
}


void World_Update(s32 dt) {
    u32 i;
    u32 k;
    Model_ *o;
    f32 c;
    f32 t;

    D_80138A78.dt = dt;
    if (dt != 0) {
        ParticlePool_Update(&gParticles);
        ParticlePool_Update(&gParticlesAlt);
        ParticlePool_Update(&D_80145488);
        Explosions_Update(dt);
        ModelData_Update(gTrackWorld, dt);
        for (i = 0; i < 6; i++) {
            o = gModels[i];
            if (o == NULL) {
                continue;
            }
            ModelData_Update(o, dt);
            if (o->b105 < 5) {
                o->b105 = 0;
            } else {
                o->b105 -= 5;
            }
            for (k = 0; k < 12; k++) {
                if (o->b10D < o->b10E) {
                    o->b10D++;
                } else if (o->b10E < o->b10D) {
                    o->b10D--;
                } else {
                    break;
                }
            }
            if (D_80138A78.cur[i] < D_80138A78.tgt[i]) {
                D_80138A78.cur[i] += 15;
                if (D_80138A78.cur[i] > D_80138A78.tgt[i]) {
                    D_80138A78.cur[i] = D_80138A78.tgt[i];
                }
            }
            if (D_80138A78.cur[i] > D_80138A78.tgt[i]) {
                D_80138A78.cur[i] -= 15;
                if (D_80138A78.cur[i] < D_80138A78.tgt[i]) {
                    D_80138A78.cur[i] = D_80138A78.tgt[i];
                }
            }
            if (o->cur != o->tgt) {
                c = o->cur / 255.0f;
                t = o->tgt / 255.0f;
                if (c < t) {
                    c += 0.05f;
                    if (t < c) {
                        c = t;
                    }
                } else {
                    c -= 0.05f;
                    if (c < t) {
                        c = t;
                    }
                }
                o->cur = c * 255.0f;
                if (c != 0.0f) {
                    PointLight_SetColor(o->id, c * o->r, c * o->g, c * o->b);
                    PointLight_Enable(o->id);
                } else {
                    PointLight_Disable(o->id);
                }
            }
        }
        Model_UpdateWheelEffects();
        D_80138AAC = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", World_Draw);

void World_SetCollLevel(s32 arg0) {
    D_8013818C = arg0;
}

inline void Models_FreeCaches(void) {
    u32 i;

    GfxPools_Shutdown();
    for (i = 0; i < 6; i++) {
        if (gModels[i] != NULL) {
            ModelData_FreeCaches((s32)gModels[i]);
        }
    }
}

inline void Model_ResetSlot(u8 i) {
    s32 j;

    D_80138190.wrecked[i] = 0;
    D_80138190.fadeCur[i] = 100;
    D_80138190.fadeTgt[i] = 100;
    D_80138190.palette[i] = 1;
    for (j = 0; j < 6; j++) {
        D_80138190.partA[i][j] = 0;
        D_80138190.partC[i][j] = 0;
        D_80138190.partB[i][j] = 0;
    }
}

s32 Color_PackRGBA5551(u32 r, u32 g, u32 b, s32 a) {
    /* TODO(match-debt): the mixed u32/s32 temporaries only fix the instruction order */
    u32 hi;
    s32 mid;

    a &= 0xFF;
    r <<= 8;
    g <<= 3;
    b >>= 2;
    hi = r & 0xF800;
    mid = g & 0x7C0;
    return a | ((b & 0x3E) | (hi | mid));
}

void Color_Unpack5551(u32 color, void *r, void *g, void *b) {
    u32 c = color & 0xFFFF;
    u8 cr = c >> 11;
    u8 cg = (c >> 6) & 0x1F;
    u8 cb = (c >> 1) & 0x1F;

    *(u8 *)r = cr;
    *(u8 *)g = cg;
    *(u8 *)b = cb;
}

__asm__(
    ".section .text\n"
    ".globl Model_SwapPalette\n"
    ".type Model_SwapPalette, @function\n"
    ".ent Model_SwapPalette\n"
    "Model_SwapPalette:\n"
    ".set noreorder\n"
    ".set noat\n"
    "andi $a0, $a0, 0xFF\n"
    "lui $v1, %hi(gModels)\n"
    "addiu $v1, $v1, %lo(gModels)\n"
    "sll $v0, $a0, 2\n"
    "addu $a2, $v0, $v1\n"
    "lw $v0, 0x0($a2)\n"
    "beqz $v0, .L80066730\n"
    "addiu $sp, $sp, -0x8\n"
    "lbu $v0, 0x150($v0)\n"
    "beqz $v0, .L80066730\n"
    "andi $a3, $a1, 0xFF\n"
    "lui $v0, %hi(D_80138A72)\n"
    "addu $v0, $v0, $a0\n"
    "lbu $v0, %lo(D_80138A72)($v0)\n"
    "beq $v0, $a3, .L80066730\n"
    "nop\n"
    "lui $at, %hi(D_80138A72)\n"
    "addu $at, $at, $a0\n"
    "sb $a1, %lo(D_80138A72)($at)\n"
    "lw $v0, 0x0($a2)\n"
    "lbu $t3, 0x150($v0)\n"
    "lw $v0, 0x34($v0)\n"
    "sll $v1, $t3, 3\n"
    "addu $v0, $v0, $v1\n"
    "lw $a0, 0x4($v0)\n"
    "addiu $v0, $zero, 0x1\n"
    "bne $a3, $v0, .L8006669C\n"
    "addiu $a0, $a0, 0x8\n"
    "addu $t3, $zero, $zero\n"
    "addu $a1, $a2, $zero\n"
    ".L80066670:\n"
    "lw $v1, 0x0($a1)\n"
    "sll $v0, $t3, 1\n"
    "addu $v0, $v0, $v1\n"
    "lhu $v0, 0x152($v0)\n"
    "addiu $t3, $t3, 0x1\n"
    "sh $v0, 0x0($a0)\n"
    "sltiu $v0, $t3, 0x7\n"
    "bnez $v0, .L80066670\n"
    "addiu $a0, $a0, 0x2\n"
    "j .L80066730\n"
    "nop\n"
    ".L8006669C:\n"
    "addu $t3, $zero, $zero\n"
    "addu $t5, $a2, $zero\n"
    "addu $t4, $a0, $zero\n"
    ".L800666A8:\n"
    "lw $v1, 0x0($t5)\n"
    "lhu $a0, 0x0($t4)\n"
    "sll $v0, $t3, 1\n"
    "addu $v0, $v0, $v1\n"
    "sh $a0, 0x152($v0)\n"
    "lhu $a1, 0x0($t4)\n"
    "addiu $t3, $t3, 0x1\n"
    "srl $t2, $a1, 11\n"
    "srl $a2, $a1, 6\n"
    "andi $a2, $a2, 0x1F\n"
    "srl $a1, $a1, 1\n"
    "andi $a1, $a1, 0x1F\n"
    "srl $a3, $t2, 1\n"
    "srl $t0, $a2, 1\n"
    "srl $t1, $a1, 1\n"
    "srl $v1, $a3, 3\n"
    "srl $a0, $t0, 3\n"
    "srl $v0, $t1, 3\n"
    "sll $v1, $v1, 11\n"
    "sll $a0, $a0, 6\n"
    "sll $v0, $v0, 1\n"
    "or $v1, $v1, $a0\n"
    "or $v0, $v0, $v1\n"
    "ori $v0, $v0, 0x1\n"
    "sh $v0, 0x0($t4)\n"
    "addiu $t4, $t4, 0x2\n"
    "sltiu $v0, $t3, 0x7\n"
    "sb $t2, 0x0($sp)\n"
    "sb $a2, 0x1($sp)\n"
    "sb $a1, 0x2($sp)\n"
    "sb $a3, 0x0($sp)\n"
    "sb $t0, 0x1($sp)\n"
    "bnez $v0, .L800666A8\n"
    "sb $t1, 0x2($sp)\n"
    ".L80066730:\n"
    "addiu $sp, $sp, 0x8\n"
    "jr $ra\n"
    "nop\n"
    ".set at\n"
    ".set reorder\n"
    ".size Model_SwapPalette, . - Model_SwapPalette\n"
    ".end Model_SwapPalette\n"
);

void Model_FlashOff(u8 i) {
    gModels[i]->b105 = 0;
}

void Model_FlashOn(u8 i) {
    gModels[i]->b105 = 0xFF;
}

void Model_SetAlpha(u8 i, s8 val) {
    Model_ *m = gModels[i];

    if (m != NULL) {
        m->inst->mat->b9 = val;
    }
}

void Model_SetVisible(u8 i, u8 on) {
    Model_ *m = gModels[i];

    if (m != NULL) {
        if (on) {
            m->inst->flags |= 0x40;
            return;
        }
        m->inst->flags &= ~0x40;
    }
}

u8 Model_LoadDefault(u8 a, s32 b) {
    return Model_Load(a, b, 0);
}

extern char D_800FE95C[];
s32 File_LoadAsync(char *name, void *dst);
s32 Model_PollLoad(u8 i);
s32 Model_Free(u8 i);

/* Starts loading a model file asynchronously, waiting for any previous load in that slot. */
s32 Model_LoadAsync(u8 id, char *name, u8 mode) {
    char path[64];
    u32 i;
    s32 h;

    if (gModelLoading[id] != 0) {
        while (!(u8)Model_PollLoad(id)) {
        }
    }
    D_80138150[id] = mode ^ 1;
    D_80138156[id] = mode ^ 1;
    strcpy(path, name);
    strcat(path, D_800FE95C);
    if (gModels[id] != NULL) {
        Model_Free(id);
    }
    GfxPools_Shutdown();
    for (i = 0; i < 6; i++) {
        if (gModels[i] != NULL) {
            ModelData_FreeCaches((s32)gModels[i]);
        }
    }
    h = File_LoadAsync(path, ((void **)&gModelBuffers)[id]);
    gModelLoadHandles[id] = h;
    if (h != -1) {
        gModelLoading[id] = 1;
        return 1;
    }
    return 0;
}

s32 Model_PollLoad(u8 i) {
    s32 j;

    if (File_PollAsync(gModelLoadHandles[i]) == 0) {
        return 0;
    }
    gModelLoading[i] = 0;
    gModels[i] = (Model_ *)ModelData_Load(((s32 *)&gModelBuffers)[i]);
    for (j = 0; j < gModels[i]->nParts; j++) {
        Model_SetPartRotation(i, j, 0.0f, 0.0f, 0.0f);
    }
    return 1;
}


s32 Model_Unload(u8 id);
s32 File_LoadAllocCopy(char *);

u8 Model_Load(u8 id, char *name, s32 unused) {
    char path[64];
    s32 j;

    strcpy(path, name);
    strcat(path, D_800FE95C);
    if (gModels[id] != NULL) {
        Model_Unload(id);
    }
    gModels[id] = (Model_ *)ModelData_Load(File_LoadAllocCopy(path));
    for (j = 0; j < gModels[id]->nParts; j++) {
        Model_SetPartRotation(id, j, 0.0f, 0.0f, 0.0f);
    }
    Model_ResetSlot(id);
    return 1;
}

s32 Model_Free(u8 i) {
    ModelData_FreeCaches((s32)gModels[i]);
    gModels[i] = NULL;
    return 1;
}

s32 Model_Unload(u8 id) {
    Models_FreeCaches();
    if (gModels[id] != NULL) {
        ModelData_Free((s32)gModels[id]);
        gModels[id] = NULL;
        return 1;
    }
    return 0;
}

void Model_DrawWithLookAt(s32 arg0) {
    Gfx_SetLookAt(&D_800F2138);
    Model_Draw(arg0 & 0xFF);
}

void World_SetupLod(s32 a, s32 b) {
    u8 y = 1;
    u8 x = 24;
    f32 *p;
    s32 i;

    if ((a == 0) & (b == 0)) {
        x = 0;
        y = 0;
    }
    for (i = 0; i < 6; i++) {
        D_80138150[i] = x;
        D_80138156[i] = y;
    }
    p = &gLodDist0;
    *p = 700.0f;
    gLodDist1 = 1400.0f;
    gLodDist2 = 2800.0f;
    gLodDist3 = 4800.0f;
    if (a != 1) {
        *p = 490.0f;
        gLodDist1 = 980.0f;
        gLodDist2 = 1960.0f;
        gLodDist3 = 3360.0f;
    } else {
        D_80138156[0] = 0;
    }
}

void Light_Reset(void);
void Gfx_UpdateLights(void);
void ModelData_Draw(Model_ *, CameraState_ *, s32, u8, u8, s32, s32);

s32 Model_Draw(u8 idx) {
    if (gModels[idx] != NULL) {
        Light_Reset();
        Gfx_UpdateLights();
        GFX_RESERVE(1);
        gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
        ModelData_Draw(gModels[idx], &D_800F2138, D_800F2138.f4C, D_80138150[idx], D_80138156[idx], 0, 0);
    }
    return 0;
}

void Model_GetColor(u8 i, u8 *r, u8 *g, u8 *b) {
    Model_ *m = gModels[i];

    *r = m->c[0];
    *g = m->c[1];
    *b = m->c[2];
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", Model_SetTransform);

/* Sets wheel contact state and updates wheel part rotation */
__asm__(
    ".section .text\n"
    ".globl Model_SetWheel\n"
    ".type Model_SetWheel, @function\n"
    ".ent Model_SetWheel\n"
    "Model_SetWheel:\n"
    ".set noreorder\n"
    ".set noat\n"
    "addiu $sp, $sp, -0x40\n"
    "sdc1 $fs0, 0x28($sp)\n"
    "lwc1 $fs0, 0x50($sp)\n"
    "sdc1 $fs2, 0x38($sp)\n"
    "mtc1 $a3, $fs2\n"
    "lw $a3, 0x60($sp)\n"
    "sdc1 $fs1, 0x30($sp)\n"
    "mtc1 $a2, $fs1\n"
    "lbu $a2, 0x5F($sp)\n"
    "lw $v0, 0x64($sp)\n"
    "lbu $v1, 0x5B($sp)\n"
    "sw $s0, 0x18($sp)\n"
    "andi $s0, $a0, 0xFF\n"
    "addu $a0, $s0, $zero\n"
    "sw $s1, 0x1C($sp)\n"
    "andi $s1, $a1, 0xFF\n"
    "addu $a1, $s1, $zero\n"
    "sw $ra, 0x20($sp)\n"
    "sw $v0, 0x10($sp)\n"
    "jal Model_SetWheelContact\n"
    "sw $v1, 0x14($sp)\n"
    "addu $a0, $s0, $zero\n"
    "mfc1 $a2, $fs1\n"
    "mfc1 $a3, $fs2\n"
    "addu $a1, $s1, $zero\n"
    "jal Model_SetPartRotation\n"
    "swc1 $fs0, 0x10($sp)\n"
    "lw $ra, 0x20($sp)\n"
    "lw $s1, 0x1C($sp)\n"
    "lw $s0, 0x18($sp)\n"
    "ldc1 $fs2, 0x38($sp)\n"
    "ldc1 $fs1, 0x30($sp)\n"
    "ldc1 $fs0, 0x28($sp)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 0x40\n"
    ".set at\n"
    ".set reorder\n"
    ".size Model_SetWheel, . - Model_SetWheel\n"
    ".end Model_SetWheel\n"
);


/* Wheel w of model slot touched (contact 1) or left the ground; keeps the contact points and counters
 * used by the skid and dust effects. */
void Model_SetWheelContact(u8 slot, u8 w, u8 contact, Vec3f *pos, Vec3f *n, u8 surface) {
    if (D_80138190.contactOn == 0) {
        contact = 0;
    }
    D_80138190.partC[slot][w] = surface;
    if (contact != 1) {
        D_80138190.partA[slot][w] = 0;
        if (D_80138190.partB[slot][w] != 0) {
            D_80138190.partB[slot][w]--;
        }
    } else {
        if (D_80138190.partB[slot][w] != 80) {
            D_80138190.partB[slot][w]++;
        }
        if (D_80138190.partA[slot][w] == 0) {
            D_80138190.contact[slot][w].pos = *pos;
            D_80138190.contact[slot][w].n = *n;
            D_80138190.touchPos[slot][w] = *pos;
            D_80138190.partA[slot][w] = 1;
        } else {
            D_80138190.contact[slot][w].lastPos = *pos;
            D_80138190.contact[slot][w].lastN = *n;
            D_80138190.partA[slot][w] = 2;
        }
        D_80138AAC = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", Model_UpdateWheelEffects);

void Model_RotateAllParts(u8 i, f32 v) {
    u32 j;

    for (j = 0; j < gModels[i]->nParts; j++) {
        Model_SetPartRotation(i, j, 0.0f, 0.0f, v);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", Model_SetPartRotation);

void Model_SetShadowPlane(u8 i, u8 val, Vec4L_ *src) {
    gModels[i]->b14C = val;
    gModels[i]->v = *src;
}


void Model_SetEngineShake(u8 i, f32 v) {
    gModels[i]->b10E = v * 240.0f;
}


inline void Model_SetLightIntensity(u8 i, f32 v, u8 flag) {
    s32 n;

    if (v == 0.0f) {
        v = 0.2f;
    }
    D_80138A90[i] = v * 100.0f;
    if (!flag) {
        D_80138A90[i - 6] = v * 100.0f;
    }
}

void Model_SwapPalette(u8, u8);
void Model_ApplyFlags(u8 id, u32 flags) {
    Model_SetHeadlights(id, (flags >> 1) & 1, ((flags >> 3) ^ 1) & 1);
    Model_SetLightIntensity(id, !(flags & 1) ? 1.0f : 0.0f, ((flags >> 3) ^ 1) & 1);
    Model_SwapPalette(id, (flags >> 2) & 1);
}

void Model_SetHeadlights(u8 idx, u8 on, u8 fade) {
    ModelX_ *o = ((ModelX_ * *)&gModels)[idx];
    f32 c;

    c = 0.0f;
    /* TODO(match-debt): the do/while(0) block and the duplicated Disable branch below only
       change register allocation (`on` must not stay in a1); no natural form found yet. */
    do {
        if (on == 1) {
            c = 0.5f;
        }
    } while (0);

    o->tgt = c * 255.0f;
    if (fade == 0) {
        o->cur = o->tgt;
        if (c != 0.0f) {
            PointLight_SetColor(o->id, c * o->r, c * o->g, c * o->b);
            PointLight_Enable(o->id);
        } else if (on || fade) {
            PointLight_Disable(o->id);
        } else {
            PointLight_Disable(o->id);
        }
    }
}

s32 CarInfo_Load(s32 arg0, s32 arg1, s32 arg2) {
    return CarInfo_LoadEx(0, arg0, arg1, arg2) & 0xFF;
}

s32 CarInfo_LoadEx(s32 unused, char *name, Big114_ *a, Big40_ *b) {
    char path[32];
    u8 *buf;

    strcpy(path, name);
    strcat(path, D_800FE964);
    buf = (u8 *)Memory_Alloc(File_GetSize(path));
    File_Load(path, (s32)buf);
    *a = *(Big114_ *)(buf + 4);
    *b = *(Big40_ *)(buf + 0x118);
    Memory_Free(buf);
    return 1;
}


typedef struct { s32 a; s32 b; } Pair68_;
void Explosion_Spawn(s32 a, f32 b, u8 c);

/* Unless mode is 2, passes the model matrix moved 96 units along its local Y axis to Explosion_Spawn. */
void Model_Wreck(u8 slot, u8 mode, Pair68_ unused) {
    f32 m[3][4];

    D_80138190.wrecked[slot] = 1;
    gModels[slot]->inst->flags84 &= ~0x40;
    if (mode != 2) {
        m[0][0] = gModels[slot]->mtx[0][0][0];
        m[0][1] = gModels[slot]->mtx[0][0][1];
        m[0][2] = gModels[slot]->mtx[0][0][2];
        m[0][3] = gModels[slot]->mtx[0][0][3];
        m[1][0] = gModels[slot]->mtx[0][1][0];
        m[1][1] = gModels[slot]->mtx[0][1][1];
        m[1][2] = gModels[slot]->mtx[0][1][2];
        m[1][3] = gModels[slot]->mtx[0][1][3];
        m[2][0] = gModels[slot]->mtx[0][2][0];
        m[2][1] = gModels[slot]->mtx[0][2][1];
        m[2][2] = gModels[slot]->mtx[0][2][2];
        m[2][3] = gModels[slot]->mtx[0][2][3];
        m[0][3] += m[0][0] * 0.0f + m[0][1] * 96.0f + m[0][2] * 0.0f;
        m[1][3] += m[1][0] * 0.0f + m[1][1] * 96.0f + m[1][2] * 0.0f;
        m[2][3] += m[2][0] * 0.0f + m[2][1] * 96.0f + m[2][2] * 0.0f;
        Explosion_Spawn((s32)m, 1.0f, 1);
    }
    gModels[slot]->b105 = 0;
}

void Model_Repair(u8 slot) {
    u32 i;

    D_80138190.wrecked[slot] = 0;
    for (i = 0; i < gModels[slot]->nParts; i++) {
        Model_SetPartRotation(slot, i, 0.0f, 0.0f, 0.0f);
    }
    gModels[slot]->inst->flags84 |= 0x40;
}

/* Sets part's matrix from rotation rot and position pos; scaled by partScale when partScaleOn is
 * set. Only for a wrecked model, whose parts are then drawn separately (flag 0x40). */
void Model_SetPartTransform(u8 slot, u8 part, Vec3f pos, f32 (*rot)[3]) {
    f32 m[3][4];
    Model_ *mdl;

    if (D_80138190.wrecked[slot]) {
        mdl = gModels[slot];
        mdl->inst->flags84 |= 0x40;
        if (mdl != NULL && part < mdl->nParts) {
            m[0][0] = rot[0][0];
            m[0][1] = rot[0][1];
            m[0][2] = rot[0][2];
            m[0][3] = pos.x;
            m[1][0] = rot[1][0];
            m[1][1] = rot[1][1];
            m[1][2] = rot[1][2];
            m[1][3] = pos.y;
            m[2][0] = rot[2][0];
            m[2][1] = rot[2][1];
            m[2][2] = rot[2][2];
            m[2][3] = pos.z;
            mdl->mtx[part + 1][0][0] = m[0][0];
            mdl->mtx[part + 1][0][1] = m[0][1];
            mdl->mtx[part + 1][0][2] = m[0][2];
            mdl->mtx[part + 1][0][3] = m[0][3];
            mdl->mtx[part + 1][1][0] = m[1][0];
            mdl->mtx[part + 1][1][1] = m[1][1];
            mdl->mtx[part + 1][1][2] = m[1][2];
            mdl->mtx[part + 1][1][3] = m[1][3];
            mdl->mtx[part + 1][2][0] = m[2][0];
            mdl->mtx[part + 1][2][1] = m[2][1];
            mdl->mtx[part + 1][2][2] = m[2][2];
            mdl->mtx[part + 1][2][3] = m[2][3];
            if (D_80138190.partScaleOn == 1) {
                mdl->mtx[part + 1][0][0] *= D_80138190.partScale;
                mdl->mtx[part + 1][0][1] *= D_80138190.partScale;
                mdl->mtx[part + 1][0][2] *= D_80138190.partScale;
                mdl->mtx[part + 1][1][0] *= D_80138190.partScale;
                mdl->mtx[part + 1][1][1] *= D_80138190.partScale;
                mdl->mtx[part + 1][1][2] *= D_80138190.partScale;
                mdl->mtx[part + 1][2][0] *= D_80138190.partScale;
                mdl->mtx[part + 1][2][1] *= D_80138190.partScale;
                mdl->mtx[part + 1][2][2] *= D_80138190.partScale;
            }
        }
    }
}

u16 Track_GetLinkCount(void) {
    return gTrackWorld->n6;
}

void Track_GetLink(s32 i, f32 *pos, f32 *rot, f32 *a3, s32 *a4, s32 *a5, s32 *a6, s32 *a7) {
    Node_ *n = &gTrackWorld->links[i];
    f32 *m = gTrackWorld->nodes[n->unk0].m;

    rot[0] = m[0];
    rot[1] = m[1];
    rot[2] = m[2];
    rot[3] = m[4];
    rot[4] = m[5];
    rot[5] = m[6];
    rot[6] = m[8];
    rot[7] = m[9];
    rot[8] = m[10];
    pos[0] = m[3];
    pos[1] = m[7];
    pos[2] = m[11];
    a3[0] = n->x;
    a3[1] = n->y;
    a3[2] = n->z;
    *a4 = n->unkC;
    *a5 = n->unk8;
    *a6 = n->unk10;
    *a7 = n->unk14;
}

void Track_SetLinkTransform(s32 idx, Vec3f pos, f32 *m) {
    f32 *q = gTrackWorld->nodes[gTrackWorld->links[idx].unk0].m;

    q[0] = m[0];
    q[1] = m[1];
    q[2] = m[2];
    q[4] = m[3];
    q[5] = m[4];
    q[6] = m[5];
    q[8] = m[6];
    q[9] = m[7];
    q[10] = m[8];
    q[3] = pos.x;
    q[7] = pos.y;
    q[11] = pos.z;
}

void Fx_BurstAt(O_9A4 *o, Vec3f v) {
    Vec3f d;
    Vec3f p;
    u32 s;
    f32 len;
    u32 i, n;

    if (o == NULL) {
        return;
    }
    s = o->r / 2.5f;
    v.x /= 14.0f;
    v.y /= 14.0f;
    v.z /= 14.0f;
    len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    n = (rand() & 7) + 12;
    for (i = 0; i < n; i++) {
        p.x = o->x;
        p.y = o->y;
        p.z = o->z;
        p.x += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.y += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.z += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        d.x = v.x + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        d.y = v.y + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        d.z = v.z + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        Particle_SpawnJitteredVel(p, d);
    }
    n = (rand() & 7) + 12;
    v.x /= 3.0f;
    v.y /= 3.0f;
    v.z /= 3.0f;
    for (i = 0; i < n; i++) {
        p.x = o->x;
        p.y = o->y;
        p.z = o->z;
        p.x += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.y += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.z += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        d.x = v.x + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        d.y = v.y + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        d.z = v.z + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        Particle_SpawnWithVel(p, d);
    }
}

void Fx_ShatterMesh(u8 *o, Vec3f vel, u8 *verts) {
    Vec3f v;
    Vec3f p;
    s32 idx[16];
    u8 *m;
    u32 n;
    u32 sz;
    u32 i;
    f32 sp;

    if (o == NULL) {
        return;
    }
    m = *(u8 **)(o + 0x3C);
    if (m != NULL) {
        n = *(u16 *)(m + 2) >> 1;
        if (n > 16) {
            n = 16;
        }
        if (n < 6) {
            n = 6;
        }
    } else {
        n = (rand() & 7) + 7;
    }
    Mesh_GetMaterialList(m, verts, idx, 16);
    sz = *(f32 *)(o + 0x38) / 2.5f;
    vel.x /= 14.0f;
    vel.y /= 14.0f;
    vel.z /= 14.0f;
    sp = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    for (i = 0; i < n; i++) {
        p.x = *(f32 *)(o + 0xC);
        p.y = *(f32 *)(o + 0x1C);
        p.z = *(f32 *)(o + 0x2C);
        p.x += ((rand() & 0x7F) - 64) / 128.0f * sz;
        p.y += ((rand() & 0x7F) - 64) / 128.0f * sz;
        p.z += ((rand() & 0x7F) - 64) / 128.0f * sz;
        v.x = vel.x + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        v.y = vel.y + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        v.z = vel.z + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        Particle_SpawnBurst(p, sz, verts + idx[rand() & 0xF] * 8, v);
    }
}

/* Scene node as seen by the smash effects: flag bits 12-13 and the radius at 0x38. */
typedef struct { u8 p0[0x30]; u32 flags; u8 p34[4]; f32 radius; } SmashNode_;
void SceneNode_Hide(s32);

/* Hides a breakable track object and plays its effects at pos (link flag 0x20: burst instead of
 * explosion and shatter). */
void Track_SmashObject(s32 i, Vec3f pos) {
    SmashNode_ *node = (SmashNode_ *)&gTrackWorld->nodes[gTrackWorld->links[i].unk0];

    SceneNode_Hide((s32)node);
    if (((node->flags >> 13) & 1) & !(((u32)gTrackWorld->links[i].unk8 >> 5) & 1)) {
        Explosion_Spawn((s32)node, node->radius / 900.0f, 0);
    }
    if (gTrackWorld->links[i].unk8 & 0x20) {
        Fx_BurstAt((O_9A4 *)node, pos);
    }
    if ((node->flags & 0x3000) != 0 & !(((u32)gTrackWorld->links[i].unk8 >> 5) & 1)) {
        Fx_ShatterMesh((u8 *)node, pos, gTrackWorld->verts);
    }
}

void Fx_SpawnSpark(Vec3f pos, Vec3f vel) {
    Pt94_ pt;

    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= -15.0f;
    pt.vel.y /= -15.0f;
    pt.vel.z = pt.vel.z / -15.0f + 1.0f;
    pt.b66 = 0x20;
    pt.h64 = 1000;
    pt.b6A = 4;
    pt.s6C = 0x8C00;
    pt.i4 = 0;
    pt.f8 = 0.0f;
    pt.fC = 0.0f;
    pt.s6E = -0x200;
    pt.f58 = -3.0f;
    pt.f0 = 4.0f;
    pt.c[2] = (rand() & 0xF) - 0x65;
    pt.c[1] = (rand() & 0xF) - 0x65;
    pt.c[0] = (rand() & 0xF) - 0x65;
    pt.f5C = 100.0f;
    ParticlePool_Push(&D_80145488, &pt);
}


void Fx_SpawnPaintChips(u8 **car, f32 speed, Vec3f pos, Vec3f vel) {
    Pt94_ pt;
    Model_ *o;
    f32 r;

    if (car == NULL || *car == NULL || *(s32 *)(*car + 0x5C) != 3) {
        return;
    }
    o = gModels[**(u8 **)(*car + 0x64)];
    if (o == NULL) {
        return;
    }
    if (speed < 200.0f) {
        return;
    }
    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= 15.0f;
    pt.vel.y /= 15.0f;
    pt.vel.z /= 15.0f;
    pt.vel.x += rand() & 3;
    pt.vel.y += rand() & 3;
    pt.vel.z += (rand() & 3) + 20;
    pt.b66 = 0x41;
    pt.i4 = 0;
    pt.f58 = -2.0f;
    pt.f0 = 20.0f;
    pt.f8 = rand();
    pt.fC = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    pt.h64 = 1000;
    pt.b6A = 0;
    pt.s6C = 0xFF00;
    pt.s6E = 0;
    r = (rand() % 21) / 100.0;
    pt.c[2] = (u8)((0.8 - r) * o->c[2]) >> gColorShift;
    pt.c[1] = (u8)((0.8 - r) * o->c[1]) >> gColorShift;
    pt.c[0] = (u8)((0.8 - r) * o->c[0]) >> gColorShift;
    if (D_80138190.wrecked[**(u8 **)(*car + 0x64)] == 1) {
        pt.c[0] >>= 4;
        pt.c[2] >>= 4;
        pt.c[1] >>= 4;
    }
    if (D_80138A90[**(u8 **)(*car + 0x64)] != 100) {
        pt.c[2] >>= 2;
        pt.c[1] >>= 2;
        pt.c[0] >>= 2;
    }
    pt.f5C = 100.0f;
    ParticlePool_Push(&D_80145488, &pt);
}

typedef struct { u8 p0[0x6C]; Vec3f pos; u8 p78[0xC]; Vec3f vel; f32 speed; f32 r[3][3]; } Bds9_;
typedef struct { u8 p0[0x18]; f32 w; f32 h; f32 l; u8 p24[0x3C]; Bds9_ *body; } Cars9_;
typedef struct { Cars9_ *car; Vec3f pos; Vec3f n; u8 flag; } Hits9_;
extern u32 (*svcRandRange)(s32);

extern inline void Sparks9_(Vec3f pos, Vec3f vel) {
    f32 pad[2];
    Pt94_ pt;

    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= -15.0f;
    pt.vel.y /= -15.0f;
    pt.vel.z = pt.vel.z / -15.0f + 1.0f;
    pt.b66 = 0x20;
    pt.h64 = 1000;
    pt.b6A = 4;
    pt.s6C = 0x8C00;
    pt.i4 = 0;
    pt.f8 = 0.0f;
    pt.fC = 0.0f;
    pt.s6E = -0x200;
    pt.f58 = -3.0f;
    pt.f0 = 4.0f;
    pt.c[2] = (rand() & 0xF) - 0x65;
    pt.c[1] = (rand() & 0xF) - 0x65;
    pt.c[0] = (rand() & 0xF) - 0x65;
    pt.f5C = 100.0f;
    ParticlePool_Push(&D_80145488, &pt);
}

void Fx_CarScrape(Hits9_ *h, f32 speed) {
    Vec3f t;
    Vec3f v;
    Vec3f ax;
    Vec3f ay;
    Vec3f az;
    Vec3f q;
    Vec3f p;
    Bds9_ *b;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 d;
    f32 k;

    if (h->flag == 1) {
        return;
    }
    b = h->car->body;
    v = b->vel;
    if (v.x * v.x + v.y * v.y + v.z * v.z > 100.0f) {
        d = v.x * h->n.x + v.y * h->n.y + v.z * h->n.z;
        t.x = h->n.x * d;
        t.y = h->n.y * d;
        t.z = h->n.z * d;
        v.x = v.x - t.x;
        v.y = v.y - t.y;
        v.z = v.z - t.z;
        t.x = h->n.x * 10.0f;
        t.y = h->n.y * 10.0f;
        t.z = h->n.z * 60.0f;
        v.x = v.x + t.x;
        v.y = v.y + t.y;
        v.z = v.z + t.z;
        ax.x = b->r[0][0];
        ax.y = b->r[1][0];
        ax.z = b->r[2][0];
        ay.x = b->r[0][1];
        ay.y = b->r[1][1];
        ay.z = b->r[2][1];
        az.x = b->r[0][2];
        az.y = b->r[1][2];
        az.z = b->r[2][2];
        dx = ax.x * h->n.x + ax.y * h->n.y + ax.z * h->n.z;
        dy = ay.x * h->n.x + ay.y * h->n.y + ay.z * h->n.z;
        dz = az.x * h->n.x + az.y * h->n.y + az.z * h->n.z;
        if ((dx < 0.0f ? -dx : dx) > 0.9f) {
            if (dx > 0.0f) {
                t.x = -h->car->w;
                t.y = 0.0f;
                t.z = 0.0f;
            } else {
                t.x = h->car->w;
                t.y = 0.0f;
                t.z = 0.0f;
            }
            k = svcRandRange(2000) * 0.001f + -1.0f;
            t.y += k * h->car->h;
            k = svcRandRange(2000) * 0.001f + -1.0f;
            t.z += k * h->car->l;
            p.x = b->r[0][0] * t.x + b->r[0][1] * t.y + b->r[0][2] * t.z;
            p.y = b->r[1][0] * t.x + b->r[1][1] * t.y + b->r[1][2] * t.z;
            p.z = b->r[2][0] * t.x + b->r[2][1] * t.y + b->r[2][2] * t.z;
            q = p;
            q.x += b->pos.x;
            q.y += b->pos.y;
            q.z += b->pos.z;
        } else if ((dy < 0.0f ? -dy : dy) > 0.9f) {
            if (dy > 0.0f) {
                t.x = 0.0f;
                t.y = -h->car->h;
                t.z = 0.0f;
            } else {
                t.x = 0.0f;
                t.y = h->car->h;
                t.z = 0.0f;
            }
            k = svcRandRange(2000) * 0.001f + -1.0f;
            t.x += k * h->car->w;
            k = svcRandRange(2000) * 0.001f + -1.0f;
            t.z += k * h->car->l;
            p.x = b->r[0][0] * t.x + b->r[0][1] * t.y + b->r[0][2] * t.z;
            p.y = b->r[1][0] * t.x + b->r[1][1] * t.y + b->r[1][2] * t.z;
            p.z = b->r[2][0] * t.x + b->r[2][1] * t.y + b->r[2][2] * t.z;
            q = p;
            q.x += b->pos.x;
            q.y += b->pos.y;
            q.z += b->pos.z;
        } else if ((dz < 0.0f ? -dz : dz) > 0.9f) {
            if (dz > 0.0f) {
                t.x = 0.0f;
                t.y = 0.0f;
                t.z = -h->car->l;
            } else {
                t.x = 0.0f;
                t.y = 0.0f;
                t.z = h->car->l;
            }
            k = svcRandRange(2000) * 0.001f + -1.0f;
            t.x += k * h->car->w;
            k = svcRandRange(2000) * 0.001f + -1.0f;
            t.y += k * h->car->h;
            p.x = b->r[0][0] * t.x + b->r[0][1] * t.y + b->r[0][2] * t.z;
            p.y = b->r[1][0] * t.x + b->r[1][1] * t.y + b->r[1][2] * t.z;
            p.z = b->r[2][0] * t.x + b->r[2][1] * t.y + b->r[2][2] * t.z;
            q = p;
            q.x += b->pos.x;
            q.y += b->pos.y;
            q.z += b->pos.z;
        } else {
            q = h->pos;
        }
        Sparks9_(q, v);
        Fx_SpawnPaintChips((u8 **)h, speed, q, v);
    }
}

u16 Track_GetPathCount(void) {
    return gTrackWorld->nC;
}

void Track_GetPath(s32 i, s32 *a, s32 *h, s32 *b, s32 *c) {
    E14_ *e = &gTrackWorld->paths[i];

    *a = e->a;
    *h = e->h6;
    *b = e->b;
    *c = e->c;
}

void Track_GetPathKey(s32 a, s32 b, V12_ *v, V16_ *q) {
    *v = gTrackWorld->paths[a].p[b].v;
    *q = gTrackWorld->paths[a].p[b].q;
}

void Track_SetPathTransform(s32 i, Vec3f *t, f32 *m) {
    f32 *d = gTrackWorld->nodes[gTrackWorld->paths[i].node].m;

    d[0] = m[0];
    d[1] = m[1];
    d[2] = m[2];
    d[3] = t->x;
    d[4] = m[3];
    d[5] = m[4];
    d[6] = m[5];
    d[7] = t->y;
    d[8] = m[6];
    d[9] = m[7];
    d[10] = m[8];
    d[11] = t->z;
}

u8 Track_GetCameraCount(void) {
    return gTrackWorld->bB;
}

void Track_GetCamera(u8 i, Vec3i *a1, f32 *a2, s32 *a3, f32 *a4) {
    Cam_ *c = &gTrackWorld->cams[i];

    *a1 = c->p;
    *a3 = c->d;
    *a4 = c->e;
    a2[0] = c->m[0];
    a2[1] = c->m[1];
    a2[2] = c->m[2];
    a2[3] = c->m[3];
    a2[4] = c->m[4];
    a2[5] = c->m[5];
    a2[6] = c->m[6];
    a2[7] = c->m[7];
    a2[8] = c->m[8];
}

s32 Track_LoadDup(char *name, s32 dst) {
    char buf[64];

    strcpy(buf, name);
    strcat(buf, D_800FE9F0);
    File_Load(buf, dst);
    return 1;
}

u16 Track_GetEntityCount(void) {
    return gTrackWorld->nE;
}

void Track_GetEntity(s32 i, Vec3f *v, f32 *s, char *name) {
    *v = gTrackWorld->ents[i].v;
    *s = gTrackWorld->ents[i].s;
    strcpy(name, gTrackWorld->ents[i].name);
}

void Track_SetEntityVisible(s32 i, s8 on) {
    s32 node = (s32)&gTrackWorld->nodes[gTrackWorld->ents[i].node];

    if (!(on & 0xFF)) {
        SceneNode_Hide(node);
        return;
    }
    SceneNode_Show(node);
}
