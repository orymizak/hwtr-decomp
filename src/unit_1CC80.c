#include "common.h"
#include "ultra64.h"
#include "gfx.h"
#include "math.h"
#include "include_asm.h"
typedef struct { u8 kind; u8 frames; } PDef_;
typedef struct {
    u8 pad0[0x60];
    PDef_ *def;
    s16 life; u8 flags; u8 r; u8 g; u8 b; u8 kind; u8 frames; s16 h6C; s16 h6E;
} Prt70_;
typedef struct { u32 cap; s32 tail; s32 head; Prt70_ *buf; } PPool_;

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

void ParticlePool_Init(PPool_ *pool, Prt70_ *buf, u32 cap) {
    pool->tail = 0;
    pool->head = 0;
    pool->buf = buf;
    pool->cap = cap;
}

void ParticlePool_FadeOldest(PPool_ *pool) {
    u32 i;
    Prt70_ *p;

    if (pool != NULL) {
        i = pool->head + 15;
        if (i >= pool->cap) {
            i -= pool->cap;
        }
        p = &pool->buf[i];
        if (p->flags & 4) {
            p->h6E = -0xE00;
        }
    }
}

    /* TODO (Clean version):
    Estructuras locales y duplicadas creadas para Particles_Draw. En C limpio deben sustituirse por los tipos estándar de libultra (Gfx, Vtx) y definiciones unificadas de partículas (Particle, ParticlePool, Vec3f, Vec4f), evitando tipos artificiales como Vtx4C4_ y Vtx3C4_ usados para copiar memoria. Se mantienen para el match.
    */
typedef struct { s32 w[16]; } Vtx4C4_;
typedef struct { s32 w[12]; } Vtx3C4_;
typedef struct { u8 kind; u8 frames; u8 p2[2]; f32 scale; } PDefC4_;
typedef struct { Quat q; Vec3f axis; f32 angle; f32 f20; u8 pad24[0x1C]; Vec3f pos; f32 vx; f32 vy; f32 vz; f32 f58; f32 f5C; PDefC4_ *def; u16 life; u8 flags; u8 r; u8 g; u8 b; u8 kind; u8 frames; u16 h6C; s16 h6E; } PrtC4_;
typedef struct { u32 cap; s32 tail; s32 head; PrtC4_ *buf; } PoolPC4_;
extern Vtx *gVtxHead;
extern PDefC4_ *D_801380C8[];
extern Vtx3C4_ D_800F23C0;
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
Gfx *Gfx_LoadTexture(Gfx *, PDefC4_ *);
void Quat_FromAxisAngle(Quat *, f32, Vec3f);
void Quat_ToMtx3(Quat, f32 (*)[3]);
extern PDef_ *D_800F2330[];
    /* TODO (Clean version):
    Función estática inline que duplica exactamente a ParticlePool_FadeOldest para forzar el inlining en ParticlePool_Push. En C limpio se llamaría directamente a ParticlePool_FadeOldest o se compartiría una única función inline. Se mantiene para el match.
    */
static inline void Pool_Fix(PPool_ *pool) {
    u32 i;
    Prt70_ *e;

    if (pool != NULL) {
        i = pool->head + 15;
        if (i >= pool->cap) {
            i -= pool->cap;
        }
        e = &pool->buf[i];
        if (e->flags & 4) {
            e->h6E = -0xE00;
        }
    }
}
void ParticlePool_Push(PPool_ *pool, Prt70_ *p) {
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
        Pool_Fix(pool);
    }
}

void Particles_Draw(PoolPC4_ *pool, f32 (*m)[4]) {
    f32 a[3][4];
    f32 b[3][4];
    f32 r[3][3];
    f32 ma[3][3];
    f32 mb[3][3];
    Quat q;
    u8 *last;
    s32 i;
    PrtC4_ *p;
    PDefC4_ *cur;
    PDefC4_ *tex;
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
        /* TODO (Clean version):
        Expansión manual de macros de gestión de buffer de gráficos y comandos GBI (gDPPipeSync, gSPMatrix, gSPDisplayList, gDPSetPrimColor, gSPVertex) escribiendo palabras directamente en memoria en vez de usar la API de libultra. Se mantiene para el match.
        */
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
            gDPSetPrimColor(gDisplayListHead++, 0, 0, p->r, p->g, p->b, p->h6C >> 8);
            if (p->flags & 2) {
                GfxPools_ReserveVtx(4);
                /* TODO (Clean version):
                Copia en bloque de vértices mediante casteo forzado a structs temporales (Vtx4C4_ y Vtx3C4_) de 64 y 48 bytes directamente desde la partícula o plantilla en lugar de usar un array de Vtx o memcpy. Se mantiene para el match.
                */
                *(Vtx4C4_ *) gVtxHead = *(Vtx4C4_ *) p;
                gSPVertex(gDisplayListHead++, (u32) gVtxHead - 0x80000000, 4, 0);
                gVtxHead += 4;
            } else if (p->flags & 0x20) {
                GfxPools_ReserveVtx(3);
                *(Vtx3C4_ *) gVtxHead = D_800F23C0;
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
                gVtxHead[1].v.cn[3] = gVtxHead[0].v.cn[3] = p->h6C >> 8;
                gVtxHead[2].v.cn[0] = p->r;
                gVtxHead[2].v.cn[1] = 0;
                gVtxHead[2].v.cn[2] = 0;
                gVtxHead[2].v.cn[3] = p->h6C >> 8;
                gSPVertex(gDisplayListHead++, (u32) gVtxHead - 0x80000000, 3, 0);
                gVtxHead += 3;
            }
            gSPDisplayList(gDisplayListHead++, (u32) dl2 - 0x80000000);
        }
        GFX_RESERVE(1);
        gDPPipeSync(gDisplayListHead++);
    }
}

    /* TODO (Clean version):
    Definición redundante de estructuras matemáticas y de partículas (Vec3f, Quat, PrtQ_, PPoolQ_, Mtx34f) locales para Particle_SpawnBurst. En C limpio deben sustituirse por tipos globales compartidos (Vec3f, Vec4f, Particle, ParticlePool, Mtx34f). Se mantienen para el match.
    */
typedef struct { Quat q; f32 f10; f32 f14; f32 f18; f32 f1C; f32 f20; u8 pad24[0x1C]; Vec3f pos; f32 vx; f32 vy; f32 vz; f32 f58; f32 f5C; PDef_ *def; s16 life; u8 flags; u8 r; u8 g; u8 b; u8 kind; u8 frames; u16 h6C; s16 h6E; } PrtQ_;
typedef struct { u32 cap; s32 tail; s32 head; PrtQ_ *buf; } PPoolQ_;
extern PPoolQ_ gParticles;
s32 rand(void);
void Quat_FromMtx3(Quat *, f32 (*)[3]);
    /* TODO (Clean version):
    Funciones inline locales (RotA_, RotB_, RotC_) que aplican rotaciones a matrices 3x4 en Z, Y y X. En C limpio deberían ser funciones matemáticas compartidas del motor con nombres semánticos. Se mantienen inline para el match.
    */
static inline void RotA_(Mtx34f *m, f32 a) {
    f32 c, s, u, v;

    c = cosf(a);
    s = sinf(a);
    u = m->m[0][0] * c + m->m[0][1] * s; v = m->m[0][1] * c - m->m[0][0] * s; m->m[0][0] = u; m->m[0][1] = v;
    u = m->m[1][0] * c + m->m[1][1] * s; v = m->m[1][1] * c - m->m[1][0] * s; m->m[1][0] = u; m->m[1][1] = v;
    u = m->m[2][0] * c + m->m[2][1] * s; v = m->m[2][1] * c - m->m[2][0] * s; m->m[2][0] = u; m->m[2][1] = v;
}
static inline void RotB_(Mtx34f *m, f32 a) {
    f32 c, s, u, v;

    c = cosf(a);
    s = sinf(a);
    u = m->m[0][0] * c - m->m[0][2] * s; v = m->m[0][2] * c + m->m[0][0] * s; m->m[0][0] = u; m->m[0][2] = v;
    u = m->m[1][0] * c - m->m[1][2] * s; v = m->m[1][2] * c + m->m[1][0] * s; m->m[1][0] = u; m->m[1][2] = v;
    u = m->m[2][0] * c - m->m[2][2] * s; v = m->m[2][2] * c + m->m[2][0] * s; m->m[2][0] = u; m->m[2][2] = v;
}
static inline void RotC_(Mtx34f *m, f32 a) {
    f32 c, s, u, v;

    c = cosf(a);
    s = sinf(a);
    u = m->m[0][1] * c + m->m[0][2] * s; v = m->m[0][2] * c - m->m[0][1] * s; m->m[0][1] = u; m->m[0][2] = v;
    u = m->m[1][1] * c + m->m[1][2] * s; v = m->m[1][2] * c - m->m[1][1] * s; m->m[1][1] = u; m->m[1][2] = v;
    u = m->m[2][1] * c + m->m[2][2] * s; v = m->m[2][2] * c - m->m[2][1] * s; m->m[2][1] = u; m->m[2][2] = v;
}
    /* TODO (Clean version):
    Función inline que replica la lógica de ParticlePool_Push para insertar partículas en la cola circular e incluye un condicional redundante (if (pool != NULL)) debido al inlining. En C limpio se reutilizaría una función unificada del pool. Se mantiene para el match.
    */
static inline void PushQ_(PPoolQ_ *pool, PrtQ_ *p) {
    u32 i;
    PrtQ_ *e;

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
        if (pool != NULL) {
            i = pool->head + 15;
            if (i >= pool->cap) {
                i -= pool->cap;
            }
            e = &pool->buf[i];
            if (e->flags & 4) {
                e->h6E = -0xE00;
            }
        }
    }
}
INCLUDE_ASM("asm/nonmatchings/unit_1CC80", ParticlePool_Update);

INCLUDE_ASM("asm/nonmatchings/unit_1CC80", SkidMark_AddQuad);

/* Spawns a coloured particle of the given kind at pos, sized by size. */
void Particle_SpawnColored(Vec3f pos, u32 size, s32 kind, s32 r, s32 g, s32 b) {
    PrtQ_ p;

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
    p.h6C = ((rand() & 0xF) + (size / 2 + 100)) << 8;
    p.h6E = -((rand() & 3) + 6) << 8;
    p.r = r;
    p.g = g;
    p.b = b;
    p.f5C = 100.0f;
    PushQ_(&gParticles, &p);
}

extern PPoolQ_ gParticlesAlt;
/* Spawns a particle of the given kind near pos (jittered by up to 64 units), sized by size. */
void Particle_SpawnJittered(Vec3f pos, u32 size, s32 kind) {
    PrtQ_ p;

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
    p.h6C = ((rand() & 0xF) + (size / 2 + 120)) << 8;
    p.h6E = -((rand() & 3) + 5) << 8;
    p.r = (rand() & 0xF) + 90;
    p.g = (rand() & 0xF) + 90;
    p.b = (rand() & 0x1F) + 100;
    p.f5C = 100.0f;
    PushQ_(&gParticlesAlt, &p);
}

void Particle_SpawnBurst(Vec3f pos, u32 n, PDef_ *def, Vec3f vel) {
    PrtQ_ p;
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
    RotA_(&m, a);
    a = (f32) (rand() & 0x3F) / 32.0f;
    if (bits & 2) {
        a = -a;
    }
    RotB_(&m, a);
    a = (f32) (rand() & 0x3F) / 32.0f;
    if (bits & 4) {
        a = -a;
    }
    RotC_(&m, a);
    r[0][0] = m.m[0][0];
    r[0][1] = m.m[0][1];
    r[0][2] = m.m[0][2];
    r[1][0] = m.m[1][0];
    r[1][1] = m.m[1][1];
    r[1][2] = m.m[1][2];
    r[2][0] = m.m[2][0];
    r[2][1] = m.m[2][1];
    r[2][2] = m.m[2][2];
    /* TODO (Clean version):
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
    RotA_(&m, a);
    a = (f32) (rand() & 0x3F) / 16.0f;
    if (bits & 0x20) {
        a = -a;
    }
    RotB_(&m, a);
    a = (f32) (rand() & 0x3F) / 16.0f;
    if (bits & 0x40) {
        a = -a;
    }
    RotC_(&m, a);
    p.f10 = m.m[0][0];
    p.f14 = m.m[0][1];
    p.f18 = m.m[0][2];
    a = (f32) (rand() & 0x3F) / 128.0f;
    if (bits & 8) {
        a = -a;
    }
    p.f1C = a;
    a = (f32) (rand() & 0x3F) / 128.0f;
    if (bits & 0x10) {
        a = -a;
    }
    p.f20 = a;
    p.life = 1000;
    p.h6C = 0xFFFF;
    /* TODO (Clean version):
    Operación con desplazamiento a la izquierda de un número negativo (-((rand() & 3) + 8) << 8). En C limpio se escribiría explícitamente como -(((rand() & 3) + 8) << 8) para evitar depender de precedencia de operadores y comportamiento de shifts negativos. Se mantiene para el match.
    */
    p.h6E = -((rand() & 3) + 8) << 8;
    p.r = 0xDC;
    p.g = 0xDC;
    p.b = 0xDC;
    p.def = def;
    PushQ_(&gParticles, &p);
}

/* Spawns a kind-11 particle at pos with a velocity jittered around vel. */
void Particle_SpawnJitteredVel(Vec3f pos, Vec3f vel) {
    PrtQ_ p;

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
    p.h6C = ((rand() & 0xF) + 150) << 8;
    p.h6E = -((rand() & 3) + 3) << 8;
    p.r = 200;
    p.g = 200;
    p.b = 200;
    p.f5C = 100.0f;
    PushQ_(&gParticles, &p);
}

/* Spawns a kind-12 particle at pos moving with vel. */
void Particle_SpawnWithVel(Vec3f pos, Vec3f vel) {
    PrtQ_ p;

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
    p.h6C = ((rand() & 0xF) + 150) << 8;
    p.h6E = -((rand() & 3) + 2) << 8;
    p.r = 200;
    p.g = 200;
    p.b = 200;
    p.f5C = 100.0f;
    PushQ_(&gParticles, &p);
}
