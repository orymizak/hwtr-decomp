#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "physics.h"
#include "include_asm.h"
typedef struct { Collider *o; Vec3f p; Vec3f n; } Contact;        /* body against the world */
typedef struct { Collider *o0; Collider *o1; Vec3f p; Vec3f n; } ContactPair;
extern u8 D_80138D38[];



void RigidBody_SetBoxInertia(RigidBody *o, f32 x, f32 y, f32 z);
/* Resets a rigid body: mass, identity orientation, zero motion, gravity of 386 units/s^2 along -Z,
 * box inertia from x/y/z, and the derived world inverse inertia and angular velocity. */
void RigidBody_Init(RigidBody *b, f32 mass, f32 x, f32 y, f32 z, Vec3f p) {
    Mtx3f rt;
    Mtx3f ri;
    Vec3f wv;

    b->mass = mass;
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
    b->f50 = GRAVITY;
    b->g.z = -1.0f;
    RigidBody_SetBoxInertia(b, x, y, z);
    rt.m[0][0] = b->r.m[0][0];
    rt.m[0][1] = b->r.m[1][0];
    rt.m[0][2] = b->r.m[2][0];
    rt.m[1][0] = b->r.m[0][1];
    rt.m[1][1] = b->r.m[1][1];
    rt.m[1][2] = b->r.m[2][1];
    rt.m[2][0] = b->r.m[0][2];
    rt.m[2][1] = b->r.m[1][2];
    rt.m[2][2] = b->r.m[2][2];
    ri.m[0][0] = b->r.m[0][0] * b->invInertia.m[0][0] + b->r.m[0][1] * b->invInertia.m[1][0] + b->r.m[0][2] * b->invInertia.m[2][0];
    ri.m[0][1] = b->r.m[0][0] * b->invInertia.m[0][1] + b->r.m[0][1] * b->invInertia.m[1][1] + b->r.m[0][2] * b->invInertia.m[2][1];
    ri.m[0][2] = b->r.m[0][0] * b->invInertia.m[0][2] + b->r.m[0][1] * b->invInertia.m[1][2] + b->r.m[0][2] * b->invInertia.m[2][2];
    ri.m[1][0] = b->r.m[1][0] * b->invInertia.m[0][0] + b->r.m[1][1] * b->invInertia.m[1][0] + b->r.m[1][2] * b->invInertia.m[2][0];
    ri.m[1][1] = b->r.m[1][0] * b->invInertia.m[0][1] + b->r.m[1][1] * b->invInertia.m[1][1] + b->r.m[1][2] * b->invInertia.m[2][1];
    ri.m[1][2] = b->r.m[1][0] * b->invInertia.m[0][2] + b->r.m[1][1] * b->invInertia.m[1][2] + b->r.m[1][2] * b->invInertia.m[2][2];
    ri.m[2][0] = b->r.m[2][0] * b->invInertia.m[0][0] + b->r.m[2][1] * b->invInertia.m[1][0] + b->r.m[2][2] * b->invInertia.m[2][0];
    ri.m[2][1] = b->r.m[2][0] * b->invInertia.m[0][1] + b->r.m[2][1] * b->invInertia.m[1][1] + b->r.m[2][2] * b->invInertia.m[2][1];
    ri.m[2][2] = b->r.m[2][0] * b->invInertia.m[0][2] + b->r.m[2][1] * b->invInertia.m[1][2] + b->r.m[2][2] * b->invInertia.m[2][2];
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

    /* TODO (Clean version): [evaluated 2026-11-18: kept as documentation; the code now uses
    the VEC3_* macros and RigidBody fields] Rigid body 6-DOF time-step integrator (Semi-implicit Euler):
    - Force & Linear integration:
        b->f.x += b->g.x * (b->mass * b->gravity_scale);
        b->mom += b->f * dt;
        b->vel = b->mom * b->im;
        if (b->speed > 2304.0f) { b->mom *= 2304.0f / b->speed; b->vel *= 2304.0f / b->speed; b->speed = 2304.0f; }
        b->pos += b->vel * dt;
        b->f = (Vec3f){0, 0, 0};
    - Orientation & Angular integration:
        b->am += b->t * dt;
        b->iw = b->r * b->invInertia * transpose(b->r); // World inverse inertia tensor
        b->w = b->iw * b->am;                   // Angular velocity
        if (b->wlen > 12.56636f) { // Clamp to 4*pi rad/s (2 rev/s)
            f32 k = 12.56636f / b->wlen;
            b->am *= k; b->w *= k; b->wlen = 12.56636f;
        }
        Mat3f S = skew_symmetric(b->w * dt);
        b->r += S * b->r;                       // dR = [w*dt]x * R
        b->t = (Vec3f){0, 0, 0};
    */
void RigidBody_Integrate(RigidBody *b, f32 dt) {
    Vec3f tmp;
    Vec3f v;
    Mtx3f dr;
    Vec3f wd;
    Mtx3f s;
    Mtx3f rt;
    Mtx3f ri;
    Vec3f wv;
    f32 k1;
    f32 k;

    if (b->frozen == 0) {
        tmp.x = b->g.x * (b->f50 * b->mass);
        tmp.y = b->g.y * (b->f50 * b->mass);
        tmp.z = b->g.z * (b->f50 * b->mass);
        VEC3_ADD(b->f, b->f, tmp);
        VEC3_SCALE(tmp, b->f, dt);
        VEC3_ADD(b->mom, tmp, b->mom);
        VEC3_SCALE(b->vel, b->mom, b->im);
        b->speed = sqrtf(b->vel.x * b->vel.x + b->vel.y * b->vel.y + b->vel.z * b->vel.z);
        if (b->speed > 2304.0f) {
            k1 = 2304.0f / b->speed;
            b->speed = 2304.0f;
            VEC3_SCALE(b->mom, b->mom, k1);
            VEC3_SCALE(b->vel, b->vel, k1);
        }
        VEC3_SCALE(v, b->vel, dt);
        VEC3_ADD(b->pos, v, b->pos);
        b->f.x = 0.0f;
        b->f.y = 0.0f;
        b->f.z = 0.0f;
        VEC3_SCALE(tmp, b->t, dt);
        VEC3_ADD(b->am, tmp, b->am);
        rt.m[0][0] = b->r.m[0][0];
        rt.m[0][1] = b->r.m[1][0];
        rt.m[0][2] = b->r.m[2][0];
        rt.m[1][0] = b->r.m[0][1];
        rt.m[1][1] = b->r.m[1][1];
        rt.m[1][2] = b->r.m[2][1];
        rt.m[2][0] = b->r.m[0][2];
        rt.m[2][1] = b->r.m[1][2];
        rt.m[2][2] = b->r.m[2][2];
        ri.m[0][0] = b->r.m[0][0] * b->invInertia.m[0][0] + b->r.m[0][1] * b->invInertia.m[1][0] + b->r.m[0][2] * b->invInertia.m[2][0];
        ri.m[0][1] = b->r.m[0][0] * b->invInertia.m[0][1] + b->r.m[0][1] * b->invInertia.m[1][1] + b->r.m[0][2] * b->invInertia.m[2][1];
        ri.m[0][2] = b->r.m[0][0] * b->invInertia.m[0][2] + b->r.m[0][1] * b->invInertia.m[1][2] + b->r.m[0][2] * b->invInertia.m[2][2];
        ri.m[1][0] = b->r.m[1][0] * b->invInertia.m[0][0] + b->r.m[1][1] * b->invInertia.m[1][0] + b->r.m[1][2] * b->invInertia.m[2][0];
        ri.m[1][1] = b->r.m[1][0] * b->invInertia.m[0][1] + b->r.m[1][1] * b->invInertia.m[1][1] + b->r.m[1][2] * b->invInertia.m[2][1];
        ri.m[1][2] = b->r.m[1][0] * b->invInertia.m[0][2] + b->r.m[1][1] * b->invInertia.m[1][2] + b->r.m[1][2] * b->invInertia.m[2][2];
        ri.m[2][0] = b->r.m[2][0] * b->invInertia.m[0][0] + b->r.m[2][1] * b->invInertia.m[1][0] + b->r.m[2][2] * b->invInertia.m[2][0];
        ri.m[2][1] = b->r.m[2][0] * b->invInertia.m[0][1] + b->r.m[2][1] * b->invInertia.m[1][1] + b->r.m[2][2] * b->invInertia.m[2][1];
        ri.m[2][2] = b->r.m[2][0] * b->invInertia.m[0][2] + b->r.m[2][1] * b->invInertia.m[1][2] + b->r.m[2][2] * b->invInertia.m[2][2];
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
            VEC3_SCALE(b->am, b->am, k);
            VEC3_SCALE(b->w, b->w, k);
        }
        VEC3_SCALE(wd, b->w, dt);
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

    /* TODO (Clean version): [evaluated 2026-11-18: kept as documentation; the code now uses
    the VEC3_* macros and RigidBody fields] Rigid body collision impulse against a static surface with Coulomb friction:
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
f32 RigidBody_ResolveWorldContact(Contact *c, f32 fr) {
    Vec3f p;
    Vec3f n;
    Vec3f v;
    Vec3f r;
    Vec3f x;
    Vec3f a;
    Vec3f j;
    Vec3f t;
    Vec3f tot;
    Vec3f m;
    Vec3f wr;
    RigidBody *b;
    f32 vn;
    f32 e;
    f32 imp;
    f32 d;
    f32 inv;
    f32 s;

    b = c->o->body;
    p = c->p;
    n = c->n;
    VEC3_SUB(m, p, b->pos);
    VEC3_CROSS(wr, b->w, m);
    VEC3_ADD(v, b->vel, wr);
    vn = n.x * v.x + n.y * v.y + n.z * v.z;
    if (vn > 0.0f) {
        return 0.0f;
    }
    e = 0.5f;
    if (c->o->kind == 6) {
        e = 1.3f;
    }
    VEC3_SUB(r, p, b->pos);
    VEC3_CROSS(x, r, n);
    m.x = b->iw.m[0][0] * x.x + b->iw.m[0][1] * x.y + b->iw.m[0][2] * x.z;
    m.y = b->iw.m[1][0] * x.x + b->iw.m[1][1] * x.y + b->iw.m[1][2] * x.z;
    m.z = b->iw.m[2][0] * x.x + b->iw.m[2][1] * x.y + b->iw.m[2][2] * x.z;
    a = m;
    VEC3_CROSS(x, a, r);
    imp = (-e * vn) / (b->im + (n.x * x.x + n.y * x.y + n.z * x.z));
    VEC3_SCALE(j, n, imp);
    d = n.x * v.x + n.y * v.y + n.z * v.z;
    VEC3_SCALE(t, n, d);
    VEC3_SUB(t, v, t);
    if (((t.x < 0.0f) ? -t.x : t.x) > 1.0f || ((t.y < 0.0f) ? -t.y : t.y) > 1.0f || ((t.z < 0.0f) ? -t.z : t.z) > 1.0f) {
        inv = 1.0f / sqrtf(t.x * t.x + t.y * t.y + t.z * t.z);
        s = -imp * fr;
        VEC3_SCALE(t, t, inv);
        VEC3_SCALE(t, t, s);
    } else {
        t.x = 0.0f;
        t.y = 0.0f;
        t.z = 0.0f;
    }
    VEC3_ADD(tot, j, t);
    VEC3_ADD(b->mom, tot, b->mom);
    VEC3_CROSS(x, r, tot);
    VEC3_ADD(b->am, x, b->am);
    VEC3_SCALE(b->vel, b->mom, b->im);
    m.x = b->iw.m[0][0] * b->am.x + b->iw.m[0][1] * b->am.y + b->iw.m[0][2] * b->am.z;
    m.y = b->iw.m[1][0] * b->am.x + b->iw.m[1][1] * b->am.y + b->iw.m[1][2] * b->am.z;
    m.z = b->iw.m[2][0] * b->am.x + b->iw.m[2][1] * b->am.y + b->iw.m[2][2] * b->am.z;
    b->w = m;
    return imp;
}

    /* TODO (Clean version): [evaluated 2026-11-18: kept as documentation; the code now uses
    the VEC3_* macros and RigidBody fields] Two-body collision impulse resolution:
    - Coefficient of restitution table lookup based on collider kinds:
        f32 e = D_80138D38[restitution_index(o0->kind, o1->kind)] / 10.0f;
    - Immovable body mass fallback:
        if (body == NULL && (collider->flags & 0x6))
            denom_term = GRAVITY / (f32)collider->weight;
    - Flags:
        o->flags & 0x40: Attacker boost (magnifies impulse to opponent by x10 if opponent kind == 4, else x4).
        o->flags & 0x60: Immovable/kinematic mask (skips impulse application to this body).
    - Impulses:
        b1->mom += j1; b1->am += vec3_cross(r1, j1);
        b2->mom -= j2; b2->am -= vec3_cross(r2, j2);
    */
f32 RigidBody_ResolvePairContact(ContactPair *c) {
    Vec3f p;
    Vec3f n;
    Vec3f v1;
    Vec3f v2;
    Vec3f dv;
    Vec3f r1;
    Vec3f r2;
    Vec3f x;
    Vec3f a;
    Vec3f j1;
    Vec3f j2;
    Vec3f m;
    Vec3f wr1;
    Vec3f wr2;
    Collider *o0;
    Collider *o1;
    RigidBody *b1;
    RigidBody *b2;
    f32 vn;
    f32 num;
    f32 d1;
    f32 d2;
    f32 f;
    f32 s;
    s32 idx;

    o0 = c->o0;
    o1 = c->o1;
    b1 = o0->body;
    b2 = o1->body;
    p = c->p;
    n = c->n;
    if (b1 != NULL) {
        VEC3_SUB(m, p, b1->pos);
        VEC3_CROSS(wr1, b1->w, m);
        VEC3_ADD(v1, b1->vel, wr1);
    } else {
        v1.x = 0.0f;
        v1.y = 0.0f;
        v1.z = 0.0f;
    }
    if (b2 != NULL) {
        VEC3_SUB(m, p, b2->pos);
        VEC3_CROSS(wr2, b2->w, m);
        VEC3_ADD(v2, b2->vel, wr2);
    } else {
        v2.x = 0.0f;
        v2.y = 0.0f;
        v2.z = 0.0f;
    }
    VEC3_SUB(dv, v1, v2);
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
        VEC3_SUB(r1, p, b1->pos);
    }
    if (b2 != NULL) {
        VEC3_SUB(r2, p, b2->pos);
    }
    if (b1 != NULL) {
        VEC3_CROSS(x, r1, n);
        m.x = b1->iw.m[0][0] * x.x + b1->iw.m[0][1] * x.y + b1->iw.m[0][2] * x.z;
        m.y = b1->iw.m[1][0] * x.x + b1->iw.m[1][1] * x.y + b1->iw.m[1][2] * x.z;
        m.z = b1->iw.m[2][0] * x.x + b1->iw.m[2][1] * x.y + b1->iw.m[2][2] * x.z;
        a = m;
        VEC3_CROSS(x, a, r1);
        d1 = b1->im + (n.x * x.x + n.y * x.y + n.z * x.z);
    } else if (c->o0->flags & 6) {
        if (c->o0->weight != 0) {
            d1 = GRAVITY / (f32)c->o0->weight;
        } else {
            return 0.0f;
        }
    } else {
        d1 = 0.0f;
    }
    if (b2 != NULL) {
        VEC3_CROSS(x, r2, n);
        m.x = b2->iw.m[0][0] * x.x + b2->iw.m[0][1] * x.y + b2->iw.m[0][2] * x.z;
        m.y = b2->iw.m[1][0] * x.x + b2->iw.m[1][1] * x.y + b2->iw.m[1][2] * x.z;
        m.z = b2->iw.m[2][0] * x.x + b2->iw.m[2][1] * x.y + b2->iw.m[2][2] * x.z;
        a = m;
        VEC3_CROSS(x, a, r2);
        d2 = b2->im + (n.x * x.x + n.y * x.y + n.z * x.z);
    } else if (c->o1->flags & 6) {
        if (c->o1->weight != 0) {
            d2 = GRAVITY / (f32)c->o1->weight;
        } else {
            return 0.0f;
        }
    } else {
        d2 = 0.0f;
    }
    f = num / (d1 + d2);
    VEC3_SCALE(j1, n, f);
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
        VEC3_CROSS(x, r1, j1);
        b1->am.x += x.x;
        b1->am.y += x.y;
        b1->am.z += x.z;
    }
    if (b2 != NULL && !(o1->flags & 0x60)) {
        b2->mom.x -= j2.x;
        b2->mom.y -= j2.y;
        b2->mom.z -= j2.z;
        VEC3_CROSS(x, r2, j2);
        b2->am.x -= x.x;
        b2->am.y -= x.y;
        b2->am.z -= x.z;
    }
    if (b1 != NULL) {
        VEC3_SCALE(b1->vel, b1->mom, b1->im);
        m.x = b1->iw.m[0][0] * b1->am.x + b1->iw.m[0][1] * b1->am.y + b1->iw.m[0][2] * b1->am.z;
        m.y = b1->iw.m[1][0] * b1->am.x + b1->iw.m[1][1] * b1->am.y + b1->iw.m[1][2] * b1->am.z;
        m.z = b1->iw.m[2][0] * b1->am.x + b1->iw.m[2][1] * b1->am.y + b1->iw.m[2][2] * b1->am.z;
        b1->w = m;
    }
    if (b2 != NULL) {
        VEC3_SCALE(b2->vel, b2->mom, b2->im);
        m.x = b2->iw.m[0][0] * b2->am.x + b2->iw.m[0][1] * b2->am.y + b2->iw.m[0][2] * b2->am.z;
        m.y = b2->iw.m[1][0] * b2->am.x + b2->iw.m[1][1] * b2->am.y + b2->iw.m[1][2] * b2->am.z;
        m.z = b2->iw.m[2][0] * b2->am.x + b2->iw.m[2][1] * b2->am.y + b2->iw.m[2][2] * b2->am.z;
        b2->w = m;
    }
    return f;
}

    /* TODO (Clean version): [evaluated 2026-11-18: the memset/1-over-I form changes the ROM;
    the chained zeroing and a/12, 12/a stay for matching]
    Box moment of inertia tensor and its inverse (dimensions x, y, z; mass m):
    void rigid_body_set_box_inertia(RigidBody *o, f32 x, f32 y, f32 z) {
        f32 c = (1.0f / 12.0f) * o->mass;
        memset(o->i, 0, sizeof(o->i));
        memset(o->ib, 0, sizeof(o->ib));
        o->inertia.m[0][0]  = c * (y * y + z * z); o->invInertia.m[0][0] = 1.0f / o->inertia.m[0][0];
        o->inertia.m[1][1]  = c * (x * x + z * z); o->invInertia.m[1][1] = 1.0f / o->inertia.m[1][1];
        o->inertia.m[2][2]  = c * (x * x + y * y); o->invInertia.m[2][2] = 1.0f / o->inertia.m[2][2];
    }
    Note: chained assignment in original code redundantly zeroes i[4] and ib[4] before setting them.
    */
void RigidBody_SetBoxInertia(RigidBody *o, f32 x, f32 y, f32 z) {
    f32 yy;
    f32 zz;
    f32 xx;
    f32 a;
    f32 b;
    f32 c;

    yy = y * y;
    zz = z * z;
    a = o->mass * (yy + zz);
    xx = x * x;
    b = o->mass * (xx + zz);
    c = o->mass * (xx + yy);
    o->inertia.m[0][1] = o->inertia.m[0][2] = o->inertia.m[1][0] = o->inertia.m[1][1] = o->inertia.m[1][2] = o->inertia.m[2][0] = o->inertia.m[2][1] = o->inertia.m[2][2] = 0.0f;
    o->invInertia.m[0][1] = o->invInertia.m[0][2] = o->invInertia.m[1][0] = o->invInertia.m[1][1] = o->invInertia.m[1][2] = o->invInertia.m[2][0] = o->invInertia.m[2][1] = o->invInertia.m[2][2] = 0.0f;
    o->inertia.m[0][0] = a / 12.0f;
    o->invInertia.m[0][0] = 12.0f / a;
    o->inertia.m[1][1] = b / 12.0f;
    o->inertia.m[2][2] = c / 12.0f;
    o->invInertia.m[1][1] = 12.0f / b;
    o->invInertia.m[2][2] = 12.0f / c;
}

/* Keeps only the component of the angular momentum along local axis i (column i of r). */
void RigidBody_ConstrainSpin(RigidBody *b, u8 i) {
    f32 v[3];
    f32 d;

    v[0] = b->r.m[0][i];
    v[1] = b->r.m[1][i];
    v[2] = b->r.m[2][i];
    d = v[0] * b->am.x + v[1] * b->am.y + v[2] * b->am.z;
    b->am.x = v[0] * d;
    b->am.y = v[1] * d;
    b->am.z = v[2] * d;
}

