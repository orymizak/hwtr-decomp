#include "common.h"
#include "include_asm.h"
f32 sqrtf(f32);
    /* TODO (Clean version): Unify duplicate physics structs across unit files into a shared header (e.g. physics.h):
    - Replace V3a_, V3e_, V3f_ with Vec3f.
    - Replace BodyA_, BodyE_, BodyF_ with a unified RigidBody struct (offsets 0x6C pos, 0x78 mom, 0x84 vel, 0x94 rot, 0xB8 am, 0xC4 iw, 0xE8 w, 0xF8 f, 0x104 t).
    - Replace OwnA_, OwnF_ with Collider (offset 0x5C kind, 0x60 body, 0x84 mass/weight).
    - Replace CtA_, CtF_ with Contact / CollisionPair.
    */
typedef struct { f32 x; f32 y; f32 z; } V3a_;
typedef struct { u8 pad0[0x4C]; f32 im; u8 pad50[0x1C]; V3a_ pos; V3a_ mom; V3a_ vel; u8 pad90[0x28]; V3a_ am; f32 m[9]; V3a_ w; } BodyA_;
typedef struct { u8 pad0[0x5C]; s32 kind; BodyA_ *b; } OwnA_;
typedef struct { OwnA_ *o; V3a_ p; V3a_ n; } CtA_;
typedef struct { f32 x; f32 y; f32 z; } V3e_;
typedef struct { f32 m[3][3]; } M3e_;
typedef struct { u8 pad0[0x24]; M3e_ ib; f32 f48; f32 im; f32 f50; V3e_ g; V3e_ f60; V3e_ pos; V3e_ mom; V3e_ vel; f32 speed; M3e_ r; V3e_ am; M3e_ iw; V3e_ w; f32 wlen; V3e_ f; V3e_ t; u8 frozen; u8 pad111[3]; s32 x114; } BodyE_;
typedef struct { f32 x; f32 y; f32 z; } V3f_;
typedef struct { u8 pad0[0x4C]; f32 im; u8 pad50[0x1C]; V3f_ pos; V3f_ mom; V3f_ vel; u8 pad90[0x28]; V3f_ am; f32 m[9]; V3f_ w; } BodyF_;
typedef struct { u8 pad0[4]; s32 flags; u8 pad8[0x54]; s32 kind; BodyF_ *b; u8 pad64[0x20]; u32 u84; } OwnF_;
typedef struct { OwnF_ *o0; OwnF_ *o1; V3f_ p; V3f_ n; } CtF_;
typedef struct { f32 i[9]; f32 ib[9]; f32 m; } BodyI_;
extern u8 D_80138D38[];



void func_800AE8F4(BodyI_ *o, f32 x, f32 y, f32 z);
/* Resets a rigid body: mass, identity orientation, zero motion, gravity of 386 units/s^2 along -Z,
 * box inertia from x/y/z, and the derived world inverse inertia and angular velocity. */
void func_800AC880(BodyE_ *b, f32 mass, f32 x, f32 y, f32 z, V3e_ p) {
    M3e_ rt;
    M3e_ ri;
    V3e_ wv;

    b->f48 = mass;
    b->im = 1.0f / mass;
    b->pos.x = 0.0f;
    b->pos.y = 0.0f;
    b->pos.z = 0.0f;
    b->mom.x = 0.0f;
    b->mom.y = 0.0f;
    b->mom.z = 0.0f;
    b->vel.x = 0.0f;
    b->vel.y = 0.0f;
    b->vel.z = 0.0f;
    b->f60 = p;
    b->r.m[0][0] = b->r.m[1][1] = b->r.m[2][2] = 0.0f;
    b->r.m[0][0] = b->r.m[1][1] = b->r.m[2][2] = 1.0f;
    b->r.m[0][1] = b->r.m[0][2] = b->r.m[1][0] = b->r.m[1][2] = b->r.m[2][0] = b->r.m[2][1] = 0.0f;
    b->am.z = b->am.y = b->am.x = 0.0f;
    b->t.z = b->t.y = b->t.x = 0.0f;
    b->f.z = b->f.y = b->f.x = 0.0f;
    b->g.x = 0.0f;
    b->g.y = 0.0f;
    b->f50 = 386.0f;
    b->g.z = -1.0f;
    func_800AE8F4((BodyI_ *)b, x, y, z);
    rt.m[0][0] = b->r.m[0][0];
    rt.m[0][1] = b->r.m[1][0];
    rt.m[0][2] = b->r.m[2][0];
    rt.m[1][0] = b->r.m[0][1];
    rt.m[1][1] = b->r.m[1][1];
    rt.m[1][2] = b->r.m[2][1];
    rt.m[2][0] = b->r.m[0][2];
    rt.m[2][1] = b->r.m[1][2];
    rt.m[2][2] = b->r.m[2][2];
    ri.m[0][0] = b->r.m[0][0] * b->ib.m[0][0] + b->r.m[0][1] * b->ib.m[1][0] + b->r.m[0][2] * b->ib.m[2][0];
    ri.m[0][1] = b->r.m[0][0] * b->ib.m[0][1] + b->r.m[0][1] * b->ib.m[1][1] + b->r.m[0][2] * b->ib.m[2][1];
    ri.m[0][2] = b->r.m[0][0] * b->ib.m[0][2] + b->r.m[0][1] * b->ib.m[1][2] + b->r.m[0][2] * b->ib.m[2][2];
    ri.m[1][0] = b->r.m[1][0] * b->ib.m[0][0] + b->r.m[1][1] * b->ib.m[1][0] + b->r.m[1][2] * b->ib.m[2][0];
    ri.m[1][1] = b->r.m[1][0] * b->ib.m[0][1] + b->r.m[1][1] * b->ib.m[1][1] + b->r.m[1][2] * b->ib.m[2][1];
    ri.m[1][2] = b->r.m[1][0] * b->ib.m[0][2] + b->r.m[1][1] * b->ib.m[1][2] + b->r.m[1][2] * b->ib.m[2][2];
    ri.m[2][0] = b->r.m[2][0] * b->ib.m[0][0] + b->r.m[2][1] * b->ib.m[1][0] + b->r.m[2][2] * b->ib.m[2][0];
    ri.m[2][1] = b->r.m[2][0] * b->ib.m[0][1] + b->r.m[2][1] * b->ib.m[1][1] + b->r.m[2][2] * b->ib.m[2][1];
    ri.m[2][2] = b->r.m[2][0] * b->ib.m[0][2] + b->r.m[2][1] * b->ib.m[1][2] + b->r.m[2][2] * b->ib.m[2][2];
    b->iw.m[0][0] = ri.m[0][0] * rt.m[0][0] + ri.m[0][1] * rt.m[1][0] + ri.m[0][2] * rt.m[2][0];
    b->iw.m[0][1] = ri.m[0][0] * rt.m[0][1] + ri.m[0][1] * rt.m[1][1] + ri.m[0][2] * rt.m[2][1];
    b->iw.m[0][2] = ri.m[0][0] * rt.m[0][2] + ri.m[0][1] * rt.m[1][2] + ri.m[0][2] * rt.m[2][2];
    b->iw.m[1][0] = ri.m[1][0] * rt.m[0][0] + ri.m[1][1] * rt.m[1][0] + ri.m[1][2] * rt.m[2][0];
    b->iw.m[1][1] = ri.m[1][0] * rt.m[0][1] + ri.m[1][1] * rt.m[1][1] + ri.m[1][2] * rt.m[2][1];
    b->iw.m[1][2] = ri.m[1][0] * rt.m[0][2] + ri.m[1][1] * rt.m[1][2] + ri.m[1][2] * rt.m[2][2];
    b->iw.m[2][0] = ri.m[2][0] * rt.m[0][0] + ri.m[2][1] * rt.m[1][0] + ri.m[2][2] * rt.m[2][0];
    b->iw.m[2][1] = ri.m[2][0] * rt.m[0][1] + ri.m[2][1] * rt.m[1][1] + ri.m[2][2] * rt.m[2][1];
    b->iw.m[2][2] = ri.m[2][0] * rt.m[0][2] + ri.m[2][1] * rt.m[1][2] + ri.m[2][2] * rt.m[2][2];
    wv.x = b->iw.m[0][0] * b->am.x + b->iw.m[0][1] * b->am.y + b->iw.m[0][2] * b->am.z;
    wv.y = b->iw.m[1][0] * b->am.x + b->iw.m[1][1] * b->am.y + b->iw.m[1][2] * b->am.z;
    wv.z = b->iw.m[2][0] * b->am.x + b->iw.m[2][1] * b->am.y + b->iw.m[2][2] * b->am.z;
    b->w = wv;
    b->frozen = 0;
    b->x114 = 0;
}

    /* TODO (Clean version): Rigid body 6-DOF time-step integrator (Semi-implicit Euler):
    - Force & Linear integration:
        b->f.x += b->g.x * (b->mass * b->gravity_scale);
        b->mom += b->f * dt;
        b->vel = b->mom * b->im;
        if (b->speed > 2304.0f) { b->mom *= 2304.0f / b->speed; b->vel *= 2304.0f / b->speed; b->speed = 2304.0f; }
        b->pos += b->vel * dt;
        b->f = (Vec3f){0, 0, 0};
    - Orientation & Angular integration:
        b->am += b->t * dt;
        b->iw = b->r * b->ib * transpose(b->r); // World inverse inertia tensor
        b->w = b->iw * b->am;                   // Angular velocity
        if (b->wlen > 12.56636f) { // Clamp to 4*pi rad/s (2 rev/s)
            f32 k = 12.56636f / b->wlen;
            b->am *= k; b->w *= k; b->wlen = 12.56636f;
        }
        Mat3f S = skew_symmetric(b->w * dt);
        b->r += S * b->r;                       // dR = [w*dt]x * R
        b->t = (Vec3f){0, 0, 0};
    */
void func_800ACDC8(BodyE_ *b, f32 dt) {
    V3e_ tmp;
    V3e_ v;
    M3e_ dr;
    V3e_ wd;
    M3e_ s;
    M3e_ rt;
    M3e_ ri;
    V3e_ wv;
    f32 k1;
    f32 k;

    if (b->frozen == 0) {
        tmp.x = b->g.x * (b->f50 * b->f48);
        tmp.y = b->g.y * (b->f50 * b->f48);
        tmp.z = b->g.z * (b->f50 * b->f48);
        b->f.x = b->f.x + tmp.x;
        b->f.y = b->f.y + tmp.y;
        b->f.z = b->f.z + tmp.z;
        tmp.x = b->f.x * dt;
        tmp.y = b->f.y * dt;
        tmp.z = b->f.z * dt;
        b->mom.x = tmp.x + b->mom.x;
        b->mom.y = tmp.y + b->mom.y;
        b->mom.z = tmp.z + b->mom.z;
        b->vel.x = b->mom.x * b->im;
        b->vel.y = b->mom.y * b->im;
        b->vel.z = b->mom.z * b->im;
        b->speed = sqrtf(b->vel.x * b->vel.x + b->vel.y * b->vel.y + b->vel.z * b->vel.z);
        if (b->speed > 2304.0f) {
            k1 = 2304.0f / b->speed;
            b->speed = 2304.0f;
            b->mom.x = b->mom.x * k1;
            b->mom.y = b->mom.y * k1;
            b->mom.z = b->mom.z * k1;
            b->vel.x = b->vel.x * k1;
            b->vel.y = b->vel.y * k1;
            b->vel.z = b->vel.z * k1;
        }
        v.x = b->vel.x * dt;
        v.y = b->vel.y * dt;
        v.z = b->vel.z * dt;
        b->pos.x = v.x + b->pos.x;
        b->pos.y = v.y + b->pos.y;
        b->pos.z = v.z + b->pos.z;
        b->f.x = 0.0f;
        b->f.y = 0.0f;
        b->f.z = 0.0f;
        tmp.x = b->t.x * dt;
        tmp.y = b->t.y * dt;
        tmp.z = b->t.z * dt;
        b->am.x = tmp.x + b->am.x;
        b->am.y = tmp.y + b->am.y;
        b->am.z = tmp.z + b->am.z;
        rt.m[0][0] = b->r.m[0][0];
        rt.m[0][1] = b->r.m[1][0];
        rt.m[0][2] = b->r.m[2][0];
        rt.m[1][0] = b->r.m[0][1];
        rt.m[1][1] = b->r.m[1][1];
        rt.m[1][2] = b->r.m[2][1];
        rt.m[2][0] = b->r.m[0][2];
        rt.m[2][1] = b->r.m[1][2];
        rt.m[2][2] = b->r.m[2][2];
        ri.m[0][0] = b->r.m[0][0] * b->ib.m[0][0] + b->r.m[0][1] * b->ib.m[1][0] + b->r.m[0][2] * b->ib.m[2][0];
        ri.m[0][1] = b->r.m[0][0] * b->ib.m[0][1] + b->r.m[0][1] * b->ib.m[1][1] + b->r.m[0][2] * b->ib.m[2][1];
        ri.m[0][2] = b->r.m[0][0] * b->ib.m[0][2] + b->r.m[0][1] * b->ib.m[1][2] + b->r.m[0][2] * b->ib.m[2][2];
        ri.m[1][0] = b->r.m[1][0] * b->ib.m[0][0] + b->r.m[1][1] * b->ib.m[1][0] + b->r.m[1][2] * b->ib.m[2][0];
        ri.m[1][1] = b->r.m[1][0] * b->ib.m[0][1] + b->r.m[1][1] * b->ib.m[1][1] + b->r.m[1][2] * b->ib.m[2][1];
        ri.m[1][2] = b->r.m[1][0] * b->ib.m[0][2] + b->r.m[1][1] * b->ib.m[1][2] + b->r.m[1][2] * b->ib.m[2][2];
        ri.m[2][0] = b->r.m[2][0] * b->ib.m[0][0] + b->r.m[2][1] * b->ib.m[1][0] + b->r.m[2][2] * b->ib.m[2][0];
        ri.m[2][1] = b->r.m[2][0] * b->ib.m[0][1] + b->r.m[2][1] * b->ib.m[1][1] + b->r.m[2][2] * b->ib.m[2][1];
        ri.m[2][2] = b->r.m[2][0] * b->ib.m[0][2] + b->r.m[2][1] * b->ib.m[1][2] + b->r.m[2][2] * b->ib.m[2][2];
        b->iw.m[0][0] = ri.m[0][0] * rt.m[0][0] + ri.m[0][1] * rt.m[1][0] + ri.m[0][2] * rt.m[2][0];
        b->iw.m[0][1] = ri.m[0][0] * rt.m[0][1] + ri.m[0][1] * rt.m[1][1] + ri.m[0][2] * rt.m[2][1];
        b->iw.m[0][2] = ri.m[0][0] * rt.m[0][2] + ri.m[0][1] * rt.m[1][2] + ri.m[0][2] * rt.m[2][2];
        b->iw.m[1][0] = ri.m[1][0] * rt.m[0][0] + ri.m[1][1] * rt.m[1][0] + ri.m[1][2] * rt.m[2][0];
        b->iw.m[1][1] = ri.m[1][0] * rt.m[0][1] + ri.m[1][1] * rt.m[1][1] + ri.m[1][2] * rt.m[2][1];
        b->iw.m[1][2] = ri.m[1][0] * rt.m[0][2] + ri.m[1][1] * rt.m[1][2] + ri.m[1][2] * rt.m[2][2];
        b->iw.m[2][0] = ri.m[2][0] * rt.m[0][0] + ri.m[2][1] * rt.m[1][0] + ri.m[2][2] * rt.m[2][0];
        b->iw.m[2][1] = ri.m[2][0] * rt.m[0][1] + ri.m[2][1] * rt.m[1][1] + ri.m[2][2] * rt.m[2][1];
        b->iw.m[2][2] = ri.m[2][0] * rt.m[0][2] + ri.m[2][1] * rt.m[1][2] + ri.m[2][2] * rt.m[2][2];
        wv.x = b->iw.m[0][0] * b->am.x + b->iw.m[0][1] * b->am.y + b->iw.m[0][2] * b->am.z;
        wv.y = b->iw.m[1][0] * b->am.x + b->iw.m[1][1] * b->am.y + b->iw.m[1][2] * b->am.z;
        wv.z = b->iw.m[2][0] * b->am.x + b->iw.m[2][1] * b->am.y + b->iw.m[2][2] * b->am.z;
        b->w = wv;
        b->wlen = sqrtf(b->w.x * b->w.x + b->w.y * b->w.y + b->w.z * b->w.z);
        if (b->wlen > 12.56636f) {
            k = 12.56636f / b->wlen;
            b->wlen = 12.56636f;
            b->am.x = b->am.x * k;
            b->am.y = b->am.y * k;
            b->am.z = b->am.z * k;
            b->w.x = b->w.x * k;
            b->w.y = b->w.y * k;
            b->w.z = b->w.z * k;
        }
        wd.x = b->w.x * dt;
        wd.y = b->w.y * dt;
        wd.z = b->w.z * dt;
        s.m[0][0] = 0.0f;
        s.m[0][1] = -wd.z;
        s.m[0][2] = wd.y;
        s.m[1][0] = wd.z;
        s.m[1][1] = 0.0f;
        s.m[1][2] = -wd.x;
        s.m[2][0] = -wd.y;
        s.m[2][1] = wd.x;
        s.m[2][2] = 0.0f;
        dr.m[0][0] = s.m[0][0] * b->r.m[0][0] + s.m[0][1] * b->r.m[1][0] + s.m[0][2] * b->r.m[2][0];
        dr.m[0][1] = s.m[0][0] * b->r.m[0][1] + s.m[0][1] * b->r.m[1][1] + s.m[0][2] * b->r.m[2][1];
        dr.m[0][2] = s.m[0][0] * b->r.m[0][2] + s.m[0][1] * b->r.m[1][2] + s.m[0][2] * b->r.m[2][2];
        dr.m[1][0] = s.m[1][0] * b->r.m[0][0] + s.m[1][1] * b->r.m[1][0] + s.m[1][2] * b->r.m[2][0];
        dr.m[1][1] = s.m[1][0] * b->r.m[0][1] + s.m[1][1] * b->r.m[1][1] + s.m[1][2] * b->r.m[2][1];
        dr.m[1][2] = s.m[1][0] * b->r.m[0][2] + s.m[1][1] * b->r.m[1][2] + s.m[1][2] * b->r.m[2][2];
        dr.m[2][0] = s.m[2][0] * b->r.m[0][0] + s.m[2][1] * b->r.m[1][0] + s.m[2][2] * b->r.m[2][0];
        dr.m[2][1] = s.m[2][0] * b->r.m[0][1] + s.m[2][1] * b->r.m[1][1] + s.m[2][2] * b->r.m[2][1];
        dr.m[2][2] = s.m[2][0] * b->r.m[0][2] + s.m[2][1] * b->r.m[1][2] + s.m[2][2] * b->r.m[2][2];
        b->r.m[0][0] = dr.m[0][0] + b->r.m[0][0];
        b->r.m[0][1] = dr.m[0][1] + b->r.m[0][1];
        b->r.m[0][2] = dr.m[0][2] + b->r.m[0][2];
        b->r.m[1][0] = dr.m[1][0] + b->r.m[1][0];
        b->r.m[1][1] = dr.m[1][1] + b->r.m[1][1];
        b->r.m[1][2] = dr.m[1][2] + b->r.m[1][2];
        b->r.m[2][0] = dr.m[2][0] + b->r.m[2][0];
        b->r.m[2][1] = dr.m[2][1] + b->r.m[2][1];
        b->r.m[2][2] = dr.m[2][2] + b->r.m[2][2];
        b->t.x = 0.0f;
        b->t.y = 0.0f;
        b->t.z = 0.0f;
    }
}

    /* TODO (Clean version): Rigid body collision impulse against a static surface with Coulomb friction:
    - Relative contact velocity:
        Vec3f r = vec3_sub(c->p, b->pos);
        Vec3f v = vec3_add(b->vel, vec3_cross(b->w, r));
        f32 vn = vec3_dot(c->n, v);
        if (vn > 0.0f) return 0.0f; // Separating
    - Normal impulse:
        f32 e = (c->o->kind == 6) ? 1.3f : 0.5f; // Kind 6 has super-elastic restitution
        f32 denom = b->im + vec3_dot(c->n, vec3_cross(mat3_mul_vec3(b->inv_inertia, vec3_cross(r, c->n)), r));
        f32 imp = (-e * vn) / denom;
        Vec3f J = vec3_scale(c->n, imp);
    - Dynamic friction impulse (clamped if tangential displacement exceeds threshold):
        Vec3f vt = vec3_sub(v, vec3_scale(c->n, vn));
        if (fabsf(vt.x) > 1.0f || fabsf(vt.y) > 1.0f || fabsf(vt.z) > 1.0f)
            J = vec3_add(J, vec3_scale(vec3_normalize(vt), -imp * fr));
    - State update:
        b->mom += J; b->vel = b->mom * b->im;
        b->am += vec3_cross(r, J); b->w = mat3_mul_vec3(b->inv_inertia, b->am);
    */
f32 func_800AD774(CtA_ *c, f32 fr) {
    V3a_ p;
    V3a_ n;
    V3a_ v;
    V3a_ r;
    V3a_ x;
    V3a_ a;
    V3a_ j;
    V3a_ t;
    V3a_ tot;
    V3a_ m;
    V3a_ wr;
    BodyA_ *b;
    f32 vn;
    f32 e;
    f32 imp;
    f32 d;
    f32 inv;
    f32 s;

    b = c->o->b;
    p = c->p;
    n = c->n;
    m.x = p.x - b->pos.x;
    m.y = p.y - b->pos.y;
    m.z = p.z - b->pos.z;
    wr.x = b->w.y * m.z - m.y * b->w.z;
    wr.y = m.x * b->w.z - b->w.x * m.z;
    wr.z = b->w.x * m.y - m.x * b->w.y;
    v.x = b->vel.x + wr.x;
    v.y = b->vel.y + wr.y;
    v.z = b->vel.z + wr.z;
    vn = n.x * v.x + n.y * v.y + n.z * v.z;
    if (vn > 0.0f) {
        return 0.0f;
    }
    e = 0.5f;
    if (c->o->kind == 6) {
        e = 1.3f;
    }
    r.x = p.x - b->pos.x;
    r.y = p.y - b->pos.y;
    r.z = p.z - b->pos.z;
    x.x = r.y * n.z - n.y * r.z;
    x.y = n.x * r.z - r.x * n.z;
    x.z = r.x * n.y - n.x * r.y;
    m.x = b->m[0] * x.x + b->m[1] * x.y + b->m[2] * x.z;
    m.y = b->m[3] * x.x + b->m[4] * x.y + b->m[5] * x.z;
    m.z = b->m[6] * x.x + b->m[7] * x.y + b->m[8] * x.z;
    a = m;
    x.x = a.y * r.z - r.y * a.z;
    x.y = r.x * a.z - a.x * r.z;
    x.z = a.x * r.y - r.x * a.y;
    imp = (-e * vn) / (b->im + (n.x * x.x + n.y * x.y + n.z * x.z));
    j.x = n.x * imp;
    j.y = n.y * imp;
    j.z = n.z * imp;
    d = n.x * v.x + n.y * v.y + n.z * v.z;
    t.x = n.x * d;
    t.y = n.y * d;
    t.z = n.z * d;
    t.x = v.x - t.x;
    t.y = v.y - t.y;
    t.z = v.z - t.z;
    if (((t.x < 0.0f) ? -t.x : t.x) > 1.0f || ((t.y < 0.0f) ? -t.y : t.y) > 1.0f || ((t.z < 0.0f) ? -t.z : t.z) > 1.0f) {
        inv = 1.0f / sqrtf(t.x * t.x + t.y * t.y + t.z * t.z);
        s = -imp * fr;
        t.x = t.x * inv;
        t.y = t.y * inv;
        t.z = t.z * inv;
        t.x = t.x * s;
        t.y = t.y * s;
        t.z = t.z * s;
    } else {
        t.x = 0.0f;
        t.y = 0.0f;
        t.z = 0.0f;
    }
    tot.x = j.x + t.x;
    tot.y = j.y + t.y;
    tot.z = j.z + t.z;
    b->mom.x = tot.x + b->mom.x;
    b->mom.y = tot.y + b->mom.y;
    b->mom.z = tot.z + b->mom.z;
    x.x = r.y * tot.z - tot.y * r.z;
    x.y = tot.x * r.z - r.x * tot.z;
    x.z = r.x * tot.y - tot.x * r.y;
    b->am.x = x.x + b->am.x;
    b->am.y = x.y + b->am.y;
    b->am.z = x.z + b->am.z;
    b->vel.x = b->mom.x * b->im;
    b->vel.y = b->mom.y * b->im;
    b->vel.z = b->mom.z * b->im;
    m.x = b->m[0] * b->am.x + b->m[1] * b->am.y + b->m[2] * b->am.z;
    m.y = b->m[3] * b->am.x + b->m[4] * b->am.y + b->m[5] * b->am.z;
    m.z = b->m[6] * b->am.x + b->m[7] * b->am.y + b->m[8] * b->am.z;
    b->w = m;
    return imp;
}

    /* TODO (Clean version): Two-body collision impulse resolution:
    - Coefficient of restitution table lookup based on collider kinds:
        f32 e = D_80138D38[restitution_index(o0->kind, o1->kind)] / 10.0f;
    - Immovable body mass fallback:
        if (body == NULL && (collider->flags & 0x6))
            denom_term = 386.0f / (f32)collider->weight; // Standard gravity g (in/s^2) / weight W = 1/mass
    - Flags:
        o->flags & 0x40: Attacker boost (magnifies impulse to opponent by x10 if opponent kind == 4, else x4).
        o->flags & 0x60: Immovable/kinematic mask (skips impulse application to this body).
    - Impulses:
        b1->mom += j1; b1->am += vec3_cross(r1, j1);
        b2->mom -= j2; b2->am -= vec3_cross(r2, j2);
    */
f32 func_800ADE1C(CtF_ *c) {
    V3f_ p;
    V3f_ n;
    V3f_ v1;
    V3f_ v2;
    V3f_ dv;
    V3f_ r1;
    V3f_ r2;
    V3f_ x;
    V3f_ a;
    V3f_ j1;
    V3f_ j2;
    V3f_ m;
    V3f_ wr1;
    V3f_ wr2;
    OwnF_ *o0;
    OwnF_ *o1;
    BodyF_ *b1;
    BodyF_ *b2;
    f32 vn;
    f32 num;
    f32 d1;
    f32 d2;
    f32 f;
    f32 s;
    s32 idx;

    o0 = c->o0;
    o1 = c->o1;
    b1 = o0->b;
    b2 = o1->b;
    p = c->p;
    n = c->n;
    if (b1 != NULL) {
        m.x = p.x - b1->pos.x;
        m.y = p.y - b1->pos.y;
        m.z = p.z - b1->pos.z;
        wr1.x = b1->w.y * m.z - m.y * b1->w.z;
        wr1.y = m.x * b1->w.z - b1->w.x * m.z;
        wr1.z = b1->w.x * m.y - m.x * b1->w.y;
        v1.x = b1->vel.x + wr1.x;
        v1.y = b1->vel.y + wr1.y;
        v1.z = b1->vel.z + wr1.z;
    } else {
        v1.x = 0.0f;
        v1.y = 0.0f;
        v1.z = 0.0f;
    }
    if (b2 != NULL) {
        m.x = p.x - b2->pos.x;
        m.y = p.y - b2->pos.y;
        m.z = p.z - b2->pos.z;
        wr2.x = b2->w.y * m.z - m.y * b2->w.z;
        wr2.y = m.x * b2->w.z - b2->w.x * m.z;
        wr2.z = b2->w.x * m.y - m.x * b2->w.y;
        v2.x = b2->vel.x + wr2.x;
        v2.y = b2->vel.y + wr2.y;
        v2.z = b2->vel.z + wr2.z;
    } else {
        v2.x = 0.0f;
        v2.y = 0.0f;
        v2.z = 0.0f;
    }
    dv.x = v1.x - v2.x;
    dv.y = v1.y - v2.y;
    dv.z = v1.z - v2.z;
    vn = n.x * dv.x + n.y * dv.y + n.z * dv.z;
    if (vn > 0.0f) {
        return 0.0f;
    }
    if (o0->kind == 3 && o1->kind == 3) {
        idx = 0x2A;
    } else if (o0->kind == 4 && o1->kind == 4) {
        idx = 0x2B;
    } else if (o0->kind == 3 && o1->kind == 4) {
        idx = 0x2C;
    } else if (o0->kind == 4 && o1->kind == 3) {
        idx = 0x2C;
    } else {
        idx = 0x2D;
    }
    num = -(D_80138D38[idx] / 10.0f) * vn;
    if (b1 != NULL) {
        r1.x = p.x - b1->pos.x;
        r1.y = p.y - b1->pos.y;
        r1.z = p.z - b1->pos.z;
    }
    if (b2 != NULL) {
        r2.x = p.x - b2->pos.x;
        r2.y = p.y - b2->pos.y;
        r2.z = p.z - b2->pos.z;
    }
    if (b1 != NULL) {
        x.x = r1.y * n.z - n.y * r1.z;
        x.y = n.x * r1.z - r1.x * n.z;
        x.z = r1.x * n.y - n.x * r1.y;
        m.x = b1->m[0] * x.x + b1->m[1] * x.y + b1->m[2] * x.z;
        m.y = b1->m[3] * x.x + b1->m[4] * x.y + b1->m[5] * x.z;
        m.z = b1->m[6] * x.x + b1->m[7] * x.y + b1->m[8] * x.z;
        a = m;
        x.x = a.y * r1.z - r1.y * a.z;
        x.y = r1.x * a.z - a.x * r1.z;
        x.z = a.x * r1.y - r1.x * a.y;
        d1 = b1->im + (n.x * x.x + n.y * x.y + n.z * x.z);
    } else if (c->o0->flags & 6) {
        if (c->o0->u84 != 0) {
            d1 = 386.0f / (f32)c->o0->u84;
        } else {
            return 0.0f;
        }
    } else {
        d1 = 0.0f;
    }
    if (b2 != NULL) {
        x.x = r2.y * n.z - n.y * r2.z;
        x.y = n.x * r2.z - r2.x * n.z;
        x.z = r2.x * n.y - n.x * r2.y;
        m.x = b2->m[0] * x.x + b2->m[1] * x.y + b2->m[2] * x.z;
        m.y = b2->m[3] * x.x + b2->m[4] * x.y + b2->m[5] * x.z;
        m.z = b2->m[6] * x.x + b2->m[7] * x.y + b2->m[8] * x.z;
        a = m;
        x.x = a.y * r2.z - r2.y * a.z;
        x.y = r2.x * a.z - a.x * r2.z;
        x.z = a.x * r2.y - r2.x * a.y;
        d2 = b2->im + (n.x * x.x + n.y * x.y + n.z * x.z);
    } else if (c->o1->flags & 6) {
        if (c->o1->u84 != 0) {
            d2 = 386.0f / (f32)c->o1->u84;
        } else {
            return 0.0f;
        }
    } else {
        d2 = 0.0f;
    }
    f = num / (d1 + d2);
    j1.x = n.x * f;
    j1.y = n.y * f;
    j1.z = n.z * f;
    j2 = j1;
    if (o0->flags & 0x40) {
        if (o1->kind == 4) {
            j2.x *= 10.0f;
            j2.y *= 10.0f;
            j2.z *= 10.0f;
        } else {
            j2.x *= 4.0f;
            j2.y *= 4.0f;
            j2.z *= 4.0f;
        }
    }
    if (o1->flags & 0x40) {
        if (o0->kind == 4) {
            j1.x *= 10.0f;
            j1.y *= 10.0f;
            j1.z *= 10.0f;
        } else {
            j1.x *= 4.0f;
            j1.y *= 4.0f;
            j1.z *= 4.0f;
        }
    }
    if (b1 != NULL && !(o0->flags & 0x60)) {
        b1->mom.x += j1.x;
        b1->mom.y += j1.y;
        b1->mom.z += j1.z;
        x.x = r1.y * j1.z - j1.y * r1.z;
        x.y = j1.x * r1.z - r1.x * j1.z;
        x.z = r1.x * j1.y - j1.x * r1.y;
        b1->am.x += x.x;
        b1->am.y += x.y;
        b1->am.z += x.z;
    }
    if (b2 != NULL && !(o1->flags & 0x60)) {
        b2->mom.x -= j2.x;
        b2->mom.y -= j2.y;
        b2->mom.z -= j2.z;
        x.x = r2.y * j2.z - j2.y * r2.z;
        x.y = j2.x * r2.z - r2.x * j2.z;
        x.z = r2.x * j2.y - j2.x * r2.y;
        b2->am.x -= x.x;
        b2->am.y -= x.y;
        b2->am.z -= x.z;
    }
    if (b1 != NULL) {
        b1->vel.x = b1->mom.x * b1->im;
        b1->vel.y = b1->mom.y * b1->im;
        b1->vel.z = b1->mom.z * b1->im;
        m.x = b1->m[0] * b1->am.x + b1->m[1] * b1->am.y + b1->m[2] * b1->am.z;
        m.y = b1->m[3] * b1->am.x + b1->m[4] * b1->am.y + b1->m[5] * b1->am.z;
        m.z = b1->m[6] * b1->am.x + b1->m[7] * b1->am.y + b1->m[8] * b1->am.z;
        b1->w = m;
    }
    if (b2 != NULL) {
        b2->vel.x = b2->mom.x * b2->im;
        b2->vel.y = b2->mom.y * b2->im;
        b2->vel.z = b2->mom.z * b2->im;
        m.x = b2->m[0] * b2->am.x + b2->m[1] * b2->am.y + b2->m[2] * b2->am.z;
        m.y = b2->m[3] * b2->am.x + b2->m[4] * b2->am.y + b2->m[5] * b2->am.z;
        m.z = b2->m[6] * b2->am.x + b2->m[7] * b2->am.y + b2->m[8] * b2->am.z;
        b2->w = m;
    }
    return f;
}

    /* TODO (Clean version): Box moment of inertia tensor and its inverse (dimensions x, y, z; mass m):
    void rigid_body_set_box_inertia(BodyI_ *o, f32 x, f32 y, f32 z) {
        f32 c = (1.0f / 12.0f) * o->m;
        memset(o->i, 0, sizeof(o->i));
        memset(o->ib, 0, sizeof(o->ib));
        o->i[0]  = c * (y * y + z * z); o->ib[0] = 1.0f / o->i[0];
        o->i[4]  = c * (x * x + z * z); o->ib[4] = 1.0f / o->i[4];
        o->i[8]  = c * (x * x + y * y); o->ib[8] = 1.0f / o->i[8];
    }
    Note: chained assignment in original code redundantly zeroes i[4] and ib[4] before setting them.
    */
void func_800AE8F4(BodyI_ *o, f32 x, f32 y, f32 z) {
    f32 yy;
    f32 zz;
    f32 xx;
    f32 a;
    f32 b;
    f32 c;

    yy = y * y;
    zz = z * z;
    a = o->m * (yy + zz);
    xx = x * x;
    b = o->m * (xx + zz);
    c = o->m * (xx + yy);
    o->i[1] = o->i[2] = o->i[3] = o->i[4] = o->i[5] = o->i[6] = o->i[7] = o->i[8] = 0.0f;
    o->ib[1] = o->ib[2] = o->ib[3] = o->ib[4] = o->ib[5] = o->ib[6] = o->ib[7] = o->ib[8] = 0.0f;
    o->i[0] = a / 12.0f;
    o->ib[0] = 12.0f / a;
    o->i[4] = b / 12.0f;
    o->i[8] = c / 12.0f;
    o->ib[4] = 12.0f / b;
    o->ib[8] = 12.0f / c;
}

    /* TODO (Clean version): Project angular momentum vector onto body local orientation axis i (0=X, 1=Y, 2=Z):
    void body_project_angular_momentum_axis(RigidBody *b, s32 axis) {
        // Column 'axis' of orientation matrix r (0x94) is the world-space unit vector of local axis
        Vec3f local_axis = { b->r.m[0][axis], b->r.m[1][axis], b->r.m[2][axis] };
        f32 d = vec3_dot(local_axis, b->am); // Offset 0xB8 is angular momentum (am)
        b->am = vec3_scale(local_axis, d);
    }
    */
void func_800AE9C4(u8 *t, u8 i) {
    f32 v[3];
    f32 d;
    f32 *a = (f32 *)(i * 4 + (u32)t);

    v[0] = *(f32 *)((u8 *)a + 0x94);
    v[1] = *(f32 *)((u8 *)a + 0xA0);
    v[2] = *(f32 *)((u8 *)a + 0xAC);
    d = v[0] * *(f32 *)(t + 0xB8) + v[1] * *(f32 *)(t + 0xBC) + v[2] * *(f32 *)(t + 0xC0);
    *(f32 *)(t + 0xB8) = v[0] * d;
    *(f32 *)(t + 0xBC) = v[1] * d;
    *(f32 *)(t + 0xC0) = v[2] * d;
}

