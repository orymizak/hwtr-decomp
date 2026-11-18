#include "common.h"
#include "gfx.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
typedef struct { u8 p0[4]; u16 *unk4; } BufD_;
typedef struct { u8 p0[0x34]; Gfx *unk34; } ObjA2_;
extern s32 D_80138AA8;
extern s32 gGameState;
typedef struct { s32 f0; s32 f4; u8 p8[0x548]; } S_D_801455C0;
void PointLight_SetColor(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void PointLight_SetPos(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
typedef struct { Vtx v[32]; u8 p200[0x30]; f32 r; f32 a; u8 p238; u8 mode; } Ring6D_;
typedef struct { u32 t; Vec3f pos; Vec3f vel; f32 m[9]; } MvD6_;
void ExplosionDebris_Update(MvD6_ *o, u32 dt);
f64 func_800E6E80(f64);
f64 func_800E6F60(f64);
void ExplosionRing_Update(Ring6D_ *s, u32 t);
void Explosion_Update(s32 *p, s32 dt);
extern S_D_801455C0 gExplosions[];
void Explosion_Start(S_D_801455C0 *, s32, f32, u8);
BufD_ *Texture_Create(s32, s32);
s32 EnvMap_BuildIndex(s32 *,s32,s32);
extern s32 gEnvMapIndex;

s32 PointLight_Free(s32);
s32 Explosion_Draw(s32 *, s32, s32);
s32 Memory_Free(void *);
extern BufD_ *gEnvMapTex;
BufD_ *Texture_Load(s32);
extern s32 gEnvMapDynamic;

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", Explosion_InitDebris);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", Explosion_Start);

    /* TODO (Clean version):
    Aritmética manual de punteros y accesos a campos reinterpretando un arreglo s32* (como *(f32 *)&p[135], (Ring6D_ *)(p + 4) y p + 148 + i * 16). En C limpio se debería definir y pasar un puntero a una estructura completa con sus miembros (Ring6D_, coordenadas float y arreglo de MvD6_); se mantiene para el match.
    */
void Explosion_Update(s32 *p, s32 dt) {
    u8 v;
    f32 t;
    u32 i;

    if (dt >= p[0]) {
        p[0] = 0;
        if (p[1] != -1) {
            PointLight_Free(p[1]);
            p[1] = -1;
        }
        return;
    }
    if (p[1] != -1) {
        v = p[0] * 255.0f / 1000.0f;
        PointLight_SetColor(p[1], v, v, v);
        PointLight_SetPos(p[1], sinf(p[0] / 1000.0f * 3.141592f) * 1800.0f, *(f32 *)&p[135], *(f32 *)&p[139], *(f32 *)&p[143]);
    }
    p[0] -= dt;
    if (p[0] < 900) {
        t = (u32)dt / ((930 - p[0]) / 30);
    } else {
        t = (u32)dt;
    }
    ExplosionRing_Update((Ring6D_ *)(p + 4), t);
    for (i = 0; i < 12; i++) {
        ExplosionDebris_Update(p + 148 + i * 16, dt);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", Explosion_Draw);

void ExplosionDebris_Update(MvD6_ *o, u32 dt) {
    Vec3f v;
    f32 x;
    f32 y;
    f32 z;

    if (dt >= o->t) {
        o->t = 0;
        return;
    }
    o->t -= dt;
    o->pos.x += o->vel.x;
    o->pos.y += o->vel.y;
    o->pos.z += o->vel.z;
    x = o->vel.x;
    y = o->vel.y;
    z = o->vel.z;
    v.x = o->m[0] * x + o->m[1] * y + o->m[2] * z;
    v.y = o->m[3] * x + o->m[4] * y + o->m[5] * z;
    v.z = o->m[6] * x + o->m[7] * y + o->m[8] * z;
    o->vel.x = v.x;
    o->vel.y = v.y;
    o->vel.z = v.z;
}

void ExplosionRing_Update(Ring6D_ *s, u32 t) {
    u32 i;
    f32 ang;

    if (s->mode == 0) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].v.ob[0] = func_800E6E80(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[1] = func_800E6F60(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[2] = -((i & 1) * 65.0 / 3.0);
        }
    } else if (s->mode != 2) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].v.ob[0] = func_800E6E80(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[1] = func_800E6F60(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[2] = 0;
        }
    } else {
        return;
    }
    s->r = s->r + (f32) t * 3.5;
    s->a = s->a + (f32) t * 0.001;
}

void Explosion_Stop(S_D_801455C0 *e) {
    if (e->f4 != -1) {
        PointLight_Free(e->f4);
    }
    e->f0 = 0;
    e->f4 = -1;
}

void Explosions_Init(void) {
    u32 i;

    for (i = 0; i < 8; i++) {
        gExplosions[i].f0 = 0;
        gExplosions[i].f4 = -1;
    }
}

void Explosions_Update(s32 dt) {
    u32 i;

    for (i = 0; i < 8; i++) {
        Explosion_Update((s32 *)&gExplosions[i], dt);
    }
}

void Explosions_Draw(s32 a, s32 b) {
    u32 i;

    for (i = 0; i < 8; i++) {
        Explosion_Draw((s32 *)&gExplosions[i], a, b & 0xFF);
    }
}

void Explosion_Spawn(s32 a, f32 b, u8 c) {
    s32 min = gExplosions[0].f0;
    u32 best = 0;
    u32 i;

    for (i = 1; i < 8; i++) {
        s32 v = gExplosions[i].f0;

        if (v == 0) {
            best = i;
            break;
        }
        if (v < min) {
            best = i;
            min = v;
        }
    }
    Explosion_Start(&gExplosions[best], a, b, c);
}


void Explosions_Clear(void) {
    u32 i;
    S_D_801455C0 *p;

    for (i = 0; i < 8; i++) {
        p = &gExplosions[i];
        if (p->f4 != -1) {
            PointLight_Free(p->f4);
        }
        p->f0 = 0;
        p->f4 = -1;
    }
}

void EnvMap_Init(void) {
    EnvMap_BuildIndex(&gEnvMapIndex, 0x140, 0xF0);
}

void EnvMap_Free(void) {
    if (gEnvMapTex != 0) {
        Memory_Free(gEnvMapTex);
        gEnvMapTex = NULL;
    }
}

void EnvMap_Load(s32 arg0) {
    if (gEnvMapTex != 0) {
        Memory_Free(gEnvMapTex);
        gEnvMapTex = NULL;
    }
    gEnvMapTex = Texture_Load(arg0);
}

void EnvMap_Apply(ObjA2_ *o) {
    if (gEnvMapTex != NULL) {
        *o->unk34 = *(Gfx *)gEnvMapTex;
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", EnvMap_BuildIndex);

void EnvMap_FromPalette(u16 *pal, s32 *idx) {
    s32 i;
    u16 *dst = gEnvMapTex->unk4;

    for (i = 0; i < 0x400; i++) {
        *dst++ = pal[*idx++];
    }
}

/* TODO(match-debt): 'register' locals, the branchless '&' in the condition and the hand-advanced
   do-while are needed to match; the plain for loop over dst[i] = samples[idx[i]] does not. */
void EnvMap_Capture(s16 *samples) {
    register s32 i;
    register s32 *idx;
    register u16 *dst;
    register s16 v;

    if (gEnvMapTex != NULL && gEnvMapDynamic != 0 && ((1 != gGameState) & (D_80138AA8 != 0))) {
        dst = gEnvMapTex->unk4;
        i = 0;
        idx = &gEnvMapIndex;
        do {
            v = samples[*idx];
            idx++;
            *dst = v;
            dst++;
            i++;
        } while (i < 0x400);
    }
}

void EnvMap_CreateDynamic(void) {
    if (gEnvMapTex != 0) {
        Memory_Free(gEnvMapTex);
        gEnvMapTex = NULL;
    }
    gEnvMapTex = Texture_Create(0x20, 0x20);
    gEnvMapDynamic = 1;
}

void EnvMap_LoadStatic(s32 arg0) {
    if (gEnvMapTex != 0) {
        Memory_Free(gEnvMapTex);
        gEnvMapTex = NULL;
    }
    gEnvMapTex = Texture_Load(arg0);
    gEnvMapDynamic = 0;
}
