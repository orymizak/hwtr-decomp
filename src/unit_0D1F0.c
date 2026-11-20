#include "common.h"
#include "gfx.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
typedef struct { u8 p0[4]; u16 *unk4; } BufD_;
typedef struct { u8 p0[0x34]; Gfx *unk34; } ObjA2_;
extern s32 D_80138AA8;
extern s32 gGameState;
void PointLight_SetColor(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void PointLight_SetPos(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
typedef struct { Vtx v[32]; Mtx34f m; f32 r; f32 a; u8 p238; u8 mode; } Ring6D_;
typedef struct { u32 t; Vec3f pos; Vec3f vel; f32 m[9]; } MvD6_;
/* Explosion (gExplosions): shock ring, point light and 12 debris pieces. */
typedef struct {
    /* 0x000 */ s32 timer;     /* ms left; 0 = free slot */
    /* 0x004 */ s32 light;     /* point light handle, -1 = none */
    /* 0x008 */ u8 pad8[8];
    /* 0x010 */ Ring6D_ ring;
    /* 0x250 */ MvD6_ debris[12];
} Explosion; /* size = 0x550 */
extern Explosion gExplosions[];
void ExplosionDebris_Update(MvD6_ *o, u32 dt);
f64 sin(f64);
f64 cos(f64);
void ExplosionRing_Update(Ring6D_ *s, u32 t);
void Explosion_Update(Explosion *p, s32 dt);
void Explosion_Start(Explosion *, s32, f32, u8);
BufD_ *Texture_Create(s32, s32);
void EnvMap_BuildIndex(s32 *, s32, s32);
extern s32 gEnvMapIndex;

s32 PointLight_Free(s32);
s32 Explosion_Draw(s32 *, s32, s32);
s32 Memory_Free(void *);
extern BufD_ *gEnvMapTex;
BufD_ *Texture_Load(s32);
extern s32 gEnvMapDynamic;

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", Explosion_InitDebris);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", Explosion_Start);

void Explosion_Update(Explosion *p, s32 dt) {
    u8 v;
    f32 t;
    u32 i;

    if (dt >= p->timer) {
        p->timer = 0;
        if (p->light != -1) {
            PointLight_Free(p->light);
            p->light = -1;
        }
        return;
    }
    if (p->light != -1) {
        v = p->timer * 255.0f / 1000.0f;
        PointLight_SetColor(p->light, v, v, v);
        PointLight_SetPos(p->light, sinf(p->timer / 1000.0f * 3.141592f) * 1800.0f, p->ring.m.m[0][3], p->ring.m.m[1][3], p->ring.m.m[2][3]);
    }
    p->timer -= dt;
    if (p->timer < 900) {
        t = (u32)dt / ((930 - p->timer) / 30);
    } else {
        t = (u32)dt;
    }
    ExplosionRing_Update(&p->ring, t);
    for (i = 0; i < 12; i++) {
        ExplosionDebris_Update(&p->debris[i], dt);
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
            s->v[i].v.ob[0] = sin(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[1] = cos(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[2] = -((i & 1) * 65.0 / 3.0);
        }
    } else if (s->mode != 2) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].v.ob[0] = sin(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[1] = cos(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].v.ob[2] = 0;
        }
    } else {
        return;
    }
    s->r = s->r + (f32) t * 3.5;
    s->a = s->a + (f32) t * 0.001;
}

void Explosion_Stop(Explosion *e) {
    if (e->light != -1) {
        PointLight_Free(e->light);
    }
    e->timer = 0;
    e->light = -1;
}

void Explosions_Init(void) {
    u32 i;

    for (i = 0; i < 8; i++) {
        gExplosions[i].timer = 0;
        gExplosions[i].light = -1;
    }
}

void Explosions_Update(s32 dt) {
    u32 i;

    for (i = 0; i < 8; i++) {
        Explosion_Update(&gExplosions[i], dt);
    }
}

void Explosions_Draw(s32 a, s32 b) {
    u32 i;

    for (i = 0; i < 8; i++) {
        Explosion_Draw((s32 *)&gExplosions[i], a, b & 0xFF);
    }
}

void Explosion_Spawn(s32 a, f32 b, u8 c) {
    s32 min = gExplosions[0].timer;
    u32 best = 0;
    u32 i;

    for (i = 1; i < 8; i++) {
        s32 v = gExplosions[i].timer;

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
    Explosion *p;

    for (i = 0; i < 8; i++) {
        p = &gExplosions[i];
        if (p->light != -1) {
            PointLight_Free(p->light);
        }
        p->timer = 0;
        p->light = -1;
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


void EnvMap_BuildIndex(s32 *out, s32 w, s32 h) {
    s32 i;
    s32 j;
    s32 x0;
    s32 y0;
    s32 dx;
    s32 dy;
    f32 f;

    x0 = w / 4;
    y0 = h / 4;
    dx = w / 64;
    dy = h / 64;
    for (i = 0; i < 32; i++) {
        f = (f32) i / 32.0f;
        for (j = 0; j < 32; j++) {
            *out++ = x0 + i * dx + (y0 + (s32) (j * dy * ((1.0 - sin(f * 3.14159265)) * 0.7 + 0.3))) * w;
        }
    }
}

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
