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
    /* 0x008 */ f32 scale;
    /* 0x00C */ u8 padC[4];
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

s32 PointLight_Alloc(void);
void PointLight_Enable(s32 id);
s32 rand(void);
f32 cosf(f32);
f32 sinf(f32);

static inline void Mtx34_RotateZ(Mtx34f *m, f32 a) {
    f32 c, s, u, v;

    c = cosf(a);
    s = sinf(a);
    u = m->m[0][0] * c + m->m[0][1] * s; v = m->m[0][1] * c - m->m[0][0] * s; m->m[0][0] = u; m->m[0][1] = v;
    u = m->m[1][0] * c + m->m[1][1] * s; v = m->m[1][1] * c - m->m[1][0] * s; m->m[1][0] = u; m->m[1][1] = v;
    u = m->m[2][0] * c + m->m[2][1] * s; v = m->m[2][1] * c - m->m[2][0] * s; m->m[2][0] = u; m->m[2][1] = v;
}
static inline void Mtx34_RotateY(Mtx34f *m, f32 a) {
    f32 c, s, u, v;

    c = cosf(a);
    s = sinf(a);
    u = m->m[0][0] * c - m->m[0][2] * s; v = m->m[0][2] * c + m->m[0][0] * s; m->m[0][0] = u; m->m[0][2] = v;
    u = m->m[1][0] * c - m->m[1][2] * s; v = m->m[1][2] * c + m->m[1][0] * s; m->m[1][0] = u; m->m[1][2] = v;
    u = m->m[2][0] * c - m->m[2][2] * s; v = m->m[2][2] * c + m->m[2][0] * s; m->m[2][0] = u; m->m[2][2] = v;
}
static inline void Mtx34_RotateX(Mtx34f *m, f32 a) {
    f32 c, s, u, v;

    c = cosf(a);
    s = sinf(a);
    u = m->m[0][1] * c + m->m[0][2] * s; v = m->m[0][2] * c - m->m[0][1] * s; m->m[0][1] = u; m->m[0][2] = v;
    u = m->m[1][1] * c + m->m[1][2] * s; v = m->m[1][2] * c - m->m[1][1] * s; m->m[1][1] = u; m->m[1][2] = v;
    u = m->m[2][1] * c + m->m[2][2] * s; v = m->m[2][2] * c - m->m[2][1] * s; m->m[2][1] = u; m->m[2][2] = v;
}

/* Debris piece o of an explosion at matrix m: random lifetime, an upward direction with random x/y
 * at a random speed (8-38 times scale) and a small random tilt. */
void Explosion_InitDebris(MvD6_ *o, Mtx34f *m, f32 scale) {
    Mtx34f r;
    f32 t[3];
    f32 len;
    f32 ang;
    s32 bits;

    o->t = 1000 - (u8)rand();
    o->pos.x = m->m[0][3];
    o->pos.y = m->m[1][3];
    o->pos.z = m->m[2][3];
    o->vel.x = (u8)rand() / 128.0f - 1.0f;
    o->vel.y = (u8)rand() / 128.0f - 1.0f;
    o->vel.z = 0.7f;
    len = sqrtf(o->vel.x * o->vel.x + o->vel.y * o->vel.y + 0.7f * 0.7f);
    len = 1.0f / len;
    o->vel.x *= len;
    o->vel.y *= len;
    o->vel.z *= len;
    ang = ((rand() & 0xF) * 2.0f + 8.0f) * scale;
    o->vel.x *= ang;
    o->vel.y *= ang;
    o->vel.z *= ang;
    bits = rand();
    r.m[0][0] = r.m[0][1] = r.m[0][2] = r.m[0][3] = r.m[1][0] = r.m[1][1] = r.m[1][2] = r.m[1][3] = r.m[2][0] =
        r.m[2][1] = r.m[2][2] = r.m[2][3] = 0.0f;
    r.m[0][0] = r.m[1][1] = r.m[2][2] = 1.0f;
    ang = (rand() & 0x3F) / 1024.0f;
    if (bits & 1) {
        ang = -ang;
    }
    Mtx34_RotateZ(&r, ang);
    ang = (rand() & 0x3F) / 1024.0f;
    if (bits & 2) {
        ang = -ang;
    }
    Mtx34_RotateY(&r, ang);
    ang = (rand() & 0x3F) / 1024.0f;
    if (bits & 4) {
        ang = -ang;
    }
    Mtx34_RotateX(&r, ang);
    o->m[0] = r.m[0][0];
    o->m[1] = r.m[0][1];
    o->m[2] = r.m[0][2];
    o->m[3] = r.m[1][0];
    o->m[4] = r.m[1][1];
    o->m[5] = r.m[1][2];
    o->m[6] = r.m[2][0];
    o->m[7] = r.m[2][1];
    o->m[8] = r.m[2][2];
    t[0] = r.m[0][3];
    t[1] = r.m[1][3];
    t[2] = r.m[2][3];
}

/* Starts explosion e at the position of matrix m: debris, shock ring (randomly tilted for big ones,
 * 1 in 4 of which use the other ring texture) and a white point light. */
void Explosion_Start(Explosion *e, s32 mtx, f32 scale, u8 big) {
    u32 i;
    u32 j;
    Vtx *v;
    f32 ang;

    e->timer = 1000;
    e->scale = scale;
    e->light = PointLight_Alloc();
    for (i = 0; i < 12; i++) {
        Explosion_InitDebris(&e->debris[i], (Mtx34f *)mtx, scale);
    }
    e->ring.m.m[0][0] = e->ring.m.m[0][3] = e->ring.m.m[1][3] = e->ring.m.m[2][3] = 0.0f;
    e->ring.m.m[2][2] = e->ring.m.m[2][1] = e->ring.m.m[2][0] = 0.0f;
    e->ring.m.m[1][2] = e->ring.m.m[1][1] = e->ring.m.m[1][0] = 0.0f;
    e->ring.m.m[0][2] = e->ring.m.m[0][1] = 0.0f;
    e->ring.m.m[0][0] = e->ring.m.m[1][1] = e->ring.m.m[2][2] = 1.0f;
    e->ring.m.m[0][0] *= scale;
    e->ring.m.m[0][1] *= scale;
    e->ring.m.m[0][2] *= scale;
    e->ring.m.m[1][0] *= scale;
    e->ring.m.m[1][1] *= scale;
    e->ring.m.m[1][2] *= scale;
    e->ring.m.m[2][0] *= scale;
    e->ring.m.m[2][1] *= scale;
    e->ring.m.m[2][2] *= scale;
    e->ring.m.m[0][3] = ((Mtx34f *)mtx)->m[0][3];
    e->ring.m.m[1][3] = ((Mtx34f *)mtx)->m[1][3];
    e->ring.m.m[2][3] = ((Mtx34f *)mtx)->m[2][3];
    e->ring.r = 1.0f;
    e->ring.a = 0.0f;
    v = e->ring.v;
    for (j = 0; j < 32; j++) {
        v[j].v.ob[0] = 0;
        v[j].v.ob[1] = 0;
        v[j].v.ob[2] = 0;
        v[j].v.flag = 0;
        v[j].v.tc[0] = (j & 2) << 11;
        v[j].v.tc[1] = ((j ^ 1) & 1) << 11;
        v[j].v.cn[3] = 0xFF;
        v[j].v.cn[2] = 0xFF;
        v[j].v.cn[1] = 0xFF;
        v[j].v.cn[0] = 0xFF;
    }
    if (big == 1) {
        if (rand() & 3) {
            e->ring.mode = 0;
            e->ring.p238 = 0x1A;
        } else {
            e->ring.mode = 1;
            e->ring.p238 = 0x1B;
            ang = (u8)rand() / 64.0f;
            Mtx34_RotateX(&e->ring.m, ang);
            ang = (u8)rand() / 64.0f;
            Mtx34_RotateY(&e->ring.m, ang);
            ang = (u8)rand() / 64.0f;
            Mtx34_RotateZ(&e->ring.m, ang);
        }
    } else {
        e->ring.mode = 2;
    }
    if (e->light != -1) {
        PointLight_SetColor(e->light, 0x7F, 0x7F, 0x7F);
        PointLight_SetPos(e->light, 0.0f, e->ring.m.m[0][3], e->ring.m.m[1][3], e->ring.m.m[2][3]);
        PointLight_Enable(e->light);
    }
}

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

extern void *D_801380C8[];
extern u8 D_800F2418[];
extern u8 D_800F0550[];
s32 Mtx_New(Mtx34f *);
void Mtx34_Mul(s32 a, Mtx34f *b, Mtx34f *out);
Gfx *Gfx_LoadTexture(Gfx *, void *);
void Particle_SpawnColored(Vec3f pos, u32 size, s32 kind, s32 r, s32 g, s32 b);
void Particle_SpawnJittered(Vec3f pos, u32 size, s32 kind);
f32 fabsf(f32);

/* Draws explosion e with camera matrix cam: on pass 1 sparks from the debris and the fireball, and
 * the shock ring while the explosion is young (fading out from 800 ms). */
s32 Explosion_Draw(s32 *ep, s32 cam, s32 pass) {
    Explosion *e = (Explosion *)ep;
    u32 i;
    Mtx34f m;
    Vec3f p;
    s32 mtx;
    u8 a;

    if (e->timer == 0) {
        return;
    }
    if ((u8)pass == 1) {
        for (i = 0; i < 12; i++) {
            if (e->debris[i].t != 0) {
                Particle_SpawnColored(e->debris[i].pos, (u32)(((rand() & 0x1F) + 40) * e->scale), 2, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    if (e->ring.mode != 2 && e->timer > 600) {
        GFX_RESERVE(13);
        gDisplayListHead = Gfx_LoadTexture(gDisplayListHead, D_801380C8[e->ring.p238]);
        Mtx34_Mul(cam, &e->ring.m, &m);
        if ((fabsf(m.m[0][3]) < 32767.0f) & (fabsf(m.m[1][3]) < 32767.0f) & (fabsf(m.m[2][3]) < 32767.0f)) {
            mtx = Mtx_New(&m);
            gSPMatrix(gDisplayListHead++, mtx - 0x80000000, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPVertex(gDisplayListHead++, (s32)e->ring.v - 0x80000000, 32, 0);
            gSPDisplayList(gDisplayListHead++, D_800F2418);
            if (e->timer > 800) {
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, 0xE0);
            } else {
                a = 0xE0 - (u32)((800.0f - e->timer) / 0.89285713f);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0xFF, a);
            }
            gSPDisplayList(gDisplayListHead++, D_800F0550);
        }
    }
    if ((u8)pass == 1) {
        p.x = e->ring.m.m[0][3];
        p.y = e->ring.m.m[1][3];
        p.z = e->ring.m.m[2][3];
        rand();
        if (e->timer > 800) {
            Particle_SpawnJittered(p, (u32)(e->scale * 70.0f), 3);
        }
    }
}

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
