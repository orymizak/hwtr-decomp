#include "common.h"
#include "ultra64.h"
#include "gfx.h"
#include "math.h"
#include "include_asm.h"
/* Particle template (D_800F2330 / D_801380C8): texture kind, animation frames and size. */
typedef struct { u8 kind; u8 frames; u8 p2[2]; f32 scale; } ParticleDef;
/* With flag 2 the first 0x40 bytes hold the particle's own quad (Vtx[4]), copied as is. */
typedef struct {
    /* 0x00 */ Quat q;
    /* 0x10 */ Vec3f axis;   /* spin axis */
    /* 0x1C */ f32 angle;
    /* 0x20 */ f32 spin;     /* added to angle over time */
    /* 0x24 */ u8 pad24[0x1C];
    /* 0x40 */ Vec3f pos;
    /* 0x4C */ f32 vx;
    /* 0x50 */ f32 vy;
    /* 0x54 */ f32 vz;
    /* 0x58 */ f32 f58;
    /* 0x5C */ f32 f5C;
    /* 0x60 */ ParticleDef *def;
    /* 0x64 */ u16 life;
    /* 0x66 */ u8 flags;
    /* 0x67 */ u8 r;
    /* 0x68 */ u8 g;
    /* 0x69 */ u8 b;
    /* 0x6A */ u8 kind;
    /* 0x6B */ u8 frames;
    /* 0x6C */ u16 alpha;    /* 8.8 fixed point */
    /* 0x6E */ s16 alphaVel; /* fade speed */
} Particle; /* size = 0x70 */
/* Ring buffer of particles: head is the next free slot, tail the oldest live one. */
typedef struct { u32 cap; s32 tail; s32 head; Particle *buf; } ParticlePool;

s32 Memory_Free(s32);
extern s32 D_800F2630;
extern s32 D_801345B4;
extern s32 D_80138B10;
extern s32 D_8014C6FC;

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", Particles_Init);

void Particles_FreeAll(void) {
    if (D_8014C6FC != 0) {
        Memory_Free(D_8014C6FC);
    }
    if (D_80138B10 != 0) {
        Memory_Free(D_80138B10);
    }
    if (D_801345B4 != 0) {
        Memory_Free(D_801345B4);
    }
    if (D_800F2630 != 0) {
        Memory_Free(D_800F2630);
    }
    D_8014C6FC = 0;
    D_80138B10 = 0;
    D_801345B4 = 0;
    D_800F2630 = 0;
}

void ParticlePool_Init(ParticlePool *pool, Particle *buf, u32 cap) {
    pool->tail = 0;
    pool->head = 0;
    pool->buf = buf;
    pool->cap = cap;
}

inline void ParticlePool_FadeOldest(ParticlePool *pool) {
    u32 i;
    Particle *p;

    if (pool != NULL) {
        i = pool->head + 15;
        if (i >= pool->cap) {
            i -= pool->cap;
        }
        p = &pool->buf[i];
        if (p->flags & 4) {
            p->alphaVel = -0xE00;
        }
    }
}

extern Vtx *gVtxHead;
extern ParticleDef *D_801380C8[];
extern Vtx D_800F23C0[3]; /* template triangle */
extern u8 D_800F23A8[];
extern u8 D_800F2400[];
extern u8 D_800F2418[];
extern u8 D_800F2480[];
extern u8 D_800F2490[];
extern u8 D_800F24F8[];
extern u8 D_800F2508[];
extern u8 D_800F25B0[];
extern u8 D_800F25C8[];
void GfxPools_ReserveVtx(s32);
s32 Mtx_New(f32 (*)[4]);
u8 Frustum_TestSphere(f32 (*)[4], f32 (*)[4], f32);
void Mtx34_Mul(f32 (*)[4], f32 (*)[4], f32 (*)[4]);
Gfx *Gfx_LoadTexture(Gfx *, ParticleDef *);
void Quat_FromAxisAngle(Quat *, f32, Vec3f);
void Quat_ToMtx3(Quat, f32 (*)[3]);
extern ParticleDef *D_800F2330[];

inline void ParticlePool_Push(ParticlePool *pool, Particle *p) {
    if (pool != NULL) {
        if (!(p->flags & 0x80)) {
            p->def = D_800F2330[p->kind];
            p->kind = p->def->kind;
            p->frames = p->def->frames;
        } else {
            p->frames = 0;
        }
        pool->buf[pool->head] = *p;
        if (++pool->head == pool->cap) {
            pool->head = 0;
        }
        if (pool->head == pool->tail) {
            if (++pool->tail == pool->cap) {
                pool->tail = 0;
            }
        }
        ParticlePool_FadeOldest(pool);
    }
}

void Particles_Draw(ParticlePool *pool, f32 (*m)[4]) {
    f32 a[3][4];
    f32 b[3][4];
    f32 r[3][3];
    f32 ma[3][3];
    f32 mb[3][3];
    Quat q;
    u8 *last;
    s32 i;
    Particle *p;
    ParticleDef *cur;
    ParticleDef *tex;
    s32 k;
    s32 changed;
    s32 mtx;
    u8 *dl;
    u8 *dl2;
    f32 c;
    f32 s;
    f32 u;
    f32 v;
    f32 sc;

    if (pool != NULL) {
        GFX_RESERVE(1);
        cur = NULL;
        changed = 0;
        last = NULL;
        gDPPipeSync(gDisplayListHead++);
        for (i = pool->tail; ; i++) {
            if (i == pool->cap) {
                i = 0;
            }
            if (i == pool->head) {
                break;
            }
            p = &pool->buf[i];
            if (p->life == 0) {
                continue;
            }
            r[0][0] = r[0][1] = r[0][2] = r[1][0] = r[1][1] = r[1][2] = r[2][0] = r[2][1] = r[2][2] = 0.0f;
            r[0][0] = r[1][1] = r[2][2] = 1.0f;
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;
            a[0][2] = 0.0f;
            a[0][3] = p->pos.x;
            a[1][0] = 0.0f;
            a[1][1] = 1.0f;
            a[1][2] = 0.0f;
            a[1][3] = p->pos.y;
            a[2][0] = 0.0f;
            a[2][1] = 0.0f;
            a[2][2] = 1.0f;
            a[2][3] = p->pos.z;
            if (Frustum_TestSphere(m, a, p->f5C) != 1) {
                continue;
            }
            if (p->flags & 1) {
                b[0][1] = b[0][2] = b[1][0] = b[1][2] = b[2][0] = b[2][1] = 0.0f;
                b[0][0] = b[1][1] = b[2][2] = p->q.x / 256.0f * p->def->scale;
                b[0][3] = m[0][0] * a[0][3] + m[0][1] * a[1][3] + m[0][2] * a[2][3] + m[0][3];
                b[1][3] = m[1][0] * a[0][3] + m[1][1] * a[1][3] + m[1][2] * a[2][3] + m[1][3];
                b[2][3] = m[2][0] * a[0][3] + m[2][1] * a[1][3] + m[2][2] * a[2][3] + m[2][3];
                c = cosf(p->q.z);
                s = sinf(p->q.z);
                u = b[0][0] * c + b[0][1] * s; v = b[0][1] * c - b[0][0] * s; b[0][0] = u; b[0][1] = v;
                u = b[1][0] * c + b[1][1] * s; v = b[1][1] * c - b[1][0] * s; b[1][0] = u; b[1][1] = v;
                u = b[2][0] * c + b[2][1] * s; v = b[2][1] * c - b[2][0] * s; b[2][0] = u; b[2][1] = v;
            } else if (p->flags & 8) {
                Quat_FromAxisAngle(&q, p->angle, p->axis);
                Quat_ToMtx3(q, mb);
                Quat_ToMtx3(p->q, ma);
                r[0][0] = mb[0][0] * ma[0][0] + mb[0][1] * ma[1][0] + mb[0][2] * ma[2][0];
                r[0][1] = mb[0][0] * ma[0][1] + mb[0][1] * ma[1][1] + mb[0][2] * ma[2][1];
                r[0][2] = mb[0][0] * ma[0][2] + mb[0][1] * ma[1][2] + mb[0][2] * ma[2][2];
                r[1][0] = mb[1][0] * ma[0][0] + mb[1][1] * ma[1][0] + mb[1][2] * ma[2][0];
                r[1][1] = mb[1][0] * ma[0][1] + mb[1][1] * ma[1][1] + mb[1][2] * ma[2][1];
                r[1][2] = mb[1][0] * ma[0][2] + mb[1][1] * ma[1][2] + mb[1][2] * ma[2][2];
                r[2][0] = mb[2][0] * ma[0][0] + mb[2][1] * ma[1][0] + mb[2][2] * ma[2][0];
                r[2][1] = mb[2][0] * ma[0][1] + mb[2][1] * ma[1][1] + mb[2][2] * ma[2][1];
                r[2][2] = mb[2][0] * ma[0][2] + mb[2][1] * ma[1][2] + mb[2][2] * ma[2][2];
                a[0][0] = r[0][0];
                a[0][1] = r[0][1];
                a[0][2] = r[0][2];
                a[0][3] = p->pos.x;
                a[1][0] = r[1][0];
                a[1][1] = r[1][1];
                a[1][2] = r[1][2];
                a[1][3] = p->pos.y;
                a[2][0] = r[2][0];
                a[2][1] = r[2][1];
                a[2][2] = r[2][2];
                a[2][3] = p->pos.z;
                sc = p->f5C / 256.0f;
                a[0][0] *= sc;
                a[0][1] *= sc;
                a[0][2] *= sc;
                a[1][0] *= sc;
                a[1][1] *= sc;
                a[1][2] *= sc;
                a[2][0] *= sc;
                a[2][1] *= sc;
                a[2][2] *= sc;
                Mtx34_Mul(m, a, b);
            } else {
                b[0][0] = m[0][0];
                b[0][1] = m[0][1];
                b[0][2] = m[0][2];
                b[1][0] = m[1][0];
                b[1][1] = m[1][1];
                b[1][2] = m[1][2];
                b[2][0] = m[2][0];
                b[2][1] = m[2][1];
                b[2][2] = m[2][2];
                b[0][3] = m[0][0] * a[0][3] + m[0][1] * a[1][3] + m[0][2] * a[2][3] + m[0][3];
                b[1][3] = m[1][0] * a[0][3] + m[1][1] * a[1][3] + m[1][2] * a[2][3] + m[1][3];
                b[2][3] = m[2][0] * a[0][3] + m[2][1] * a[1][3] + m[2][2] * a[2][3] + m[2][3];
            }
            mtx = Mtx_New(b);
            GFX_RESERVE(14);
            gSPMatrix(gDisplayListHead++, mtx - 0x80000000, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gDPPipeSync(gDisplayListHead++);
            if (!(p->flags & 0x80)) {
                k = p->kind;
                tex = D_801380C8[k];
                if (cur != tex && k != 0) {
                    cur = tex;
                    changed = 1;
                }
            } else if (cur != p->def) {
                cur = p->def;
                changed = 1;
            }
            if (changed == 1) {
                gDisplayListHead = Gfx_LoadTexture(gDisplayListHead, cur);
                changed = 0;
            }
            if (p->flags & 2) {
                dl = D_800F2490;
                dl2 = D_800F2480;
            } else if (p->flags & 0x20) {
                dl = D_800F2508;
                dl2 = D_800F24F8;
            } else if (p->flags & 0x40) {
                dl = D_800F25C8;
                dl2 = D_800F25B0;
            } else {
                dl = D_800F2418;
                dl2 = D_800F2400;
                /* TODO (Bug potential):
                Si en la primera partícula activa flags no tiene el bit 0x80 y kind es 0, cur no se inicializa (permanece NULL), provocando una desreferencia de puntero nulo al evaluar cur->kind.
                */
                if (cur->kind == 0x10) {
                    dl2 = D_800F23A8;
                }
            }
            if (last != dl) {
                last = dl;
                gSPDisplayList(gDisplayListHead++, (u32) last - 0x80000000);
            }
            gDPPipeSync(gDisplayListHead++);
            gDPSetPrimColor(gDisplayListHead++, 0, 0, p->r, p->g, p->b, p->alpha >> 8);
            if (p->flags & 2) {
                GfxPools_ReserveVtx(4);
                memcpy(gVtxHead, p, 4 * sizeof(Vtx));
                gSPVertex(gDisplayListHead++, (u32) gVtxHead - 0x80000000, 4, 0);
                gVtxHead += 4;
            } else if (p->flags & 0x20) {
                GfxPools_ReserveVtx(3);
                memcpy(gVtxHead, D_800F23C0, 3 * sizeof(Vtx));
                gVtxHead[1].v.ob[0] = 1;
                gVtxHead[0].v.ob[0] = 0;
                gVtxHead[0].v.ob[1] = 0;
                gVtxHead[0].v.ob[2] = 0;
                gVtxHead[1].v.ob[1] = 1;
                gVtxHead[1].v.ob[2] = 1;
                gVtxHead[2].v.ob[0] = p->vx * p->q.x;
                gVtxHead[2].v.ob[1] = p->vy * p->q.x;
                gVtxHead[2].v.ob[2] = p->vz * p->q.x;
                gVtxHead[1].v.cn[0] = gVtxHead[0].v.cn[0] = p->r;
                gVtxHead[1].v.cn[1] = gVtxHead[0].v.cn[1] = p->g;
                gVtxHead[1].v.cn[2] = gVtxHead[0].v.cn[2] = p->b;
                gVtxHead[1].v.cn[3] = gVtxHead[0].v.cn[3] = p->alpha >> 8;
                gVtxHead[2].v.cn[0] = p->r;
                gVtxHead[2].v.cn[1] = 0;
                gVtxHead[2].v.cn[2] = 0;
                gVtxHead[2].v.cn[3] = p->alpha >> 8;
                gSPVertex(gDisplayListHead++, (u32) gVtxHead - 0x80000000, 3, 0);
                gVtxHead += 3;
            }
            gSPDisplayList(gDisplayListHead++, (u32) dl2 - 0x80000000);
        }
        GFX_RESERVE(1);
        gDPPipeSync(gDisplayListHead++);
    }
}

extern ParticlePool gParticles;
s32 rand(void);
void Quat_FromMtx3(Quat *, f32 (*)[3]);
/* Rotate the 3x3 part of m by angle a about the matrix's own Z, Y and X axes (post-multiply). */
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

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", ParticlePool_Update);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", SkidMark_AddQuad);

/* Spawns a coloured particle of the given kind at pos, sized by size. */
void Particle_SpawnColored(Vec3f pos, u32 size, s32 kind, s32 r, s32 g, s32 b) {
    Particle p;

    p.pos = pos;
    p.vx = (rand() & 3) + 3;
    p.vy = (rand() & 3) + 5;
    p.vz = rand() & 1;
    p.f58 = 0.0f;
    p.flags = 1;
    p.q.x = (rand() & 7) + 12 + size / 5;
    p.q.y = -1.0f;
    p.q.z = rand();
    p.q.w = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    p.life = 1000;
    p.kind = kind;
    p.alpha = ((rand() & 0xF) + (size / 2 + 100)) << 8;
    p.alphaVel = -((rand() & 3) + 6) << 8;
    p.r = r;
    p.g = g;
    p.b = b;
    p.f5C = 100.0f;
    ParticlePool_Push(&gParticles, &p);
}

extern ParticlePool gParticlesAlt;
/* Spawns a particle of the given kind near pos (jittered by up to 64 units), sized by size. */
void Particle_SpawnJittered(Vec3f pos, u32 size, s32 kind) {
    Particle p;

    p.pos = pos;
    p.pos.x += (rand() & 0x7F) - 0x3F;
    p.pos.y += (rand() & 0x7F) - 0x3F;
    p.pos.z += (rand() & 0x7F) - 0x3F;
    p.vx = 0.0f;
    p.vy = 0.0f;
    p.vz = rand() & 1;
    p.f58 = 0.0f;
    p.flags = 1;
    p.q.x = (rand() & 0xF) + 42 + size / 5;
    p.q.y = -1.0f;
    p.q.z = rand();
    p.q.w = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    p.life = 1200;
    p.kind = kind;
    p.alpha = ((rand() & 0xF) + (size / 2 + 120)) << 8;
    p.alphaVel = -((rand() & 3) + 5) << 8;
    p.r = (rand() & 0xF) + 90;
    p.g = (rand() & 0xF) + 90;
    p.b = (rand() & 0x1F) + 100;
    p.f5C = 100.0f;
    ParticlePool_Push(&gParticlesAlt, &p);
}

void Particle_SpawnBurst(Vec3f pos, u32 n, ParticleDef *def, Vec3f vel) {
    Particle p;
    Mtx34f m;
    Vec3f t;
    f32 r[3][3];
    Quat q;
    s32 bits;
    f32 a;

    p.pos = pos;
    p.vx = vel.x;
    p.vy = vel.y;
    p.vz = vel.z;
    p.f58 = -5.0f;
    p.flags = 0x98;
    p.f5C = n + (rand() & 0xF);
    p.kind = 0;
    bits = rand();
    m.m[0][0] = m.m[0][1] = m.m[0][2] = m.m[0][3] = m.m[1][0] = m.m[1][1] = m.m[1][2] = m.m[1][3] = m.m[2][0] = m.m[2][1] = m.m[2][2] = m.m[2][3] = 0.0f;
    m.m[0][0] = m.m[1][1] = m.m[2][2] = 1.0f;
    a = (f32) (rand() & 0x3F) / 32.0f;
    if (bits & 1) {
        a = -a;
    }
    Mtx34_RotateZ(&m, a);
    a = (f32) (rand() & 0x3F) / 32.0f;
    if (bits & 2) {
        a = -a;
    }
    Mtx34_RotateY(&m, a);
    a = (f32) (rand() & 0x3F) / 32.0f;
    if (bits & 4) {
        a = -a;
    }
    Mtx34_RotateX(&m, a);
    r[0][0] = m.m[0][0];
    r[0][1] = m.m[0][1];
    r[0][2] = m.m[0][2];
    r[1][0] = m.m[1][0];
    r[1][1] = m.m[1][1];
    r[1][2] = m.m[1][2];
    r[2][0] = m.m[2][0];
    r[2][1] = m.m[2][1];
    r[2][2] = m.m[2][2];
    /* TODO (Clean version): [evaluated 2026-11-18: removing it changes the ROM]
    La variable local 't' almacena componentes de traslación de la matriz pero su valor nunca se vuelve a leer (código muerto original). Se mantiene para preservar la distribución de registros y stack frame para el match.
    */
    t.x = m.m[0][3];
    t.y = m.m[1][3];
    t.z = m.m[2][3];
    Quat_FromMtx3(&q, r);
    p.q = q;
    a = (f32) (rand() & 0x3F) / 16.0f;
    if (bits & 0x10) {
        a = -a;
    }
    Mtx34_RotateZ(&m, a);
    a = (f32) (rand() & 0x3F) / 16.0f;
    if (bits & 0x20) {
        a = -a;
    }
    Mtx34_RotateY(&m, a);
    a = (f32) (rand() & 0x3F) / 16.0f;
    if (bits & 0x40) {
        a = -a;
    }
    Mtx34_RotateX(&m, a);
    p.axis.x = m.m[0][0];
    p.axis.y = m.m[0][1];
    p.axis.z = m.m[0][2];
    a = (f32) (rand() & 0x3F) / 128.0f;
    if (bits & 8) {
        a = -a;
    }
    p.angle = a;
    a = (f32) (rand() & 0x3F) / 128.0f;
    if (bits & 0x10) {
        a = -a;
    }
    p.spin = a;
    p.life = 1000;
    p.alpha = 0xFFFF;
    /* TODO (Clean version): [evaluated 2026-11-18: -(x << 8) changes the ROM]
    Operación con desplazamiento a la izquierda de un número negativo (-((rand() & 3) + 8) << 8). En C limpio se escribiría explícitamente como -(((rand() & 3) + 8) << 8) para evitar depender de precedencia de operadores y comportamiento de shifts negativos. Se mantiene para el match.
    */
    p.alphaVel = -((rand() & 3) + 8) << 8;
    p.r = 0xDC;
    p.g = 0xDC;
    p.b = 0xDC;
    p.def = def;
    ParticlePool_Push(&gParticles, &p);
}

/* Spawns a kind-11 particle at pos with a velocity jittered around vel. */
void Particle_SpawnJitteredVel(Vec3f pos, Vec3f vel) {
    Particle p;

    p.pos = pos;
    p.vx = vel.x + (rand() & 3) - 1.0f;
    p.vy = vel.y + (rand() & 3) - 2.0f;
    p.vz = vel.z + (rand() & 3) - 1.0f;
    p.f58 = -3.0f;
    p.flags = 1;
    p.q.x = (rand() & 3) + 14;
    p.q.y = 1.0f;
    p.q.z = rand();
    p.q.w = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    p.life = 1000;
    p.kind = 11;
    p.alpha = ((rand() & 0xF) + 150) << 8;
    p.alphaVel = -((rand() & 3) + 3) << 8;
    p.r = 200;
    p.g = 200;
    p.b = 200;
    p.f5C = 100.0f;
    ParticlePool_Push(&gParticles, &p);
}

/* Spawns a kind-12 particle at pos moving with vel. */
void Particle_SpawnWithVel(Vec3f pos, Vec3f vel) {
    Particle p;

    p.pos = pos;
    p.vx = vel.x;
    p.vy = vel.y;
    p.vz = vel.z;
    p.f58 = -1.0f;
    p.flags = 1;
    p.q.x = (rand() & 0xF) + 24;
    p.q.y = 2.0f;
    p.q.z = rand();
    p.q.w = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    p.life = 1000;
    p.kind = 12;
    p.alpha = ((rand() & 0xF) + 150) << 8;
    p.alphaVel = -((rand() & 3) + 2) << 8;
    p.r = 200;
    p.g = 200;
    p.b = 200;
    p.f5C = 100.0f;
    ParticlePool_Push(&gParticles, &p);
}
