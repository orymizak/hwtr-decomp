#include "common.h"
#include "include_asm.h"
f32 func_8007AD24(f32);
f32 cosf(f32);
f32 sinf(f32);
typedef struct { f32 x; f32 y; f32 z; f32 w; } Quat_;
typedef struct { f32 x; f32 y; f32 z; } Vec3_;



extern s32 D_800F3920[3];
f32 sqrtf(f32);
/* Converts a rotation matrix to a quaternion (Shoemake's method). */
Quat_ func_800A83A0(f32 m[3][3]) {
    Quat_ q;
    f32 *p[3] = { &q.x, &q.y, &q.z };
    f32 tr;
    f32 s;
    s32 i;
    s32 j;
    s32 k;

    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f) {
        s = sqrtf(tr + 1.0f);
        q.w = s * 0.5f;
        s = 0.5f / s;
        q.x = (m[2][1] - m[1][2]) * s;
        q.y = (m[0][2] - m[2][0]) * s;
        q.z = (m[1][0] - m[0][1]) * s;
    } else {
        i = 0;
        if (m[1][1] > m[0][0]) {
            i = 1;
        }
        if (m[2][2] > m[i][i]) {
            i = 2;
        }
        j = D_800F3920[i];
        k = D_800F3920[j];
        s = sqrtf(m[i][i] - m[j][j] - m[k][k] + 1.0f);
        *p[i] = s * 0.5f;
        s = 0.5f / s;
        q.w = (m[k][j] - m[j][k]) * s;
        *p[j] = (m[j][i] + m[i][j]) * s;
        *p[k] = (m[k][i] + m[i][k]) * s;
    }
    return q;
}

Quat_ func_800A8670(f32 angle, Vec3_ axis) {
    Quat_ t;
    f32 half = angle / 2.0f;
    f32 s = sinf(half);

    t.w = cosf(half);
    t.x = axis.x * s;
    t.y = axis.y * s;
    t.z = axis.z * s;
    return t;
}

static inline void Vec3_Normalize_(Vec3_ *v) {
    f32 inv = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x *= inv;
    v->y *= inv;
    v->z *= inv;
}

/* Builds the rotation that points the forward (Y) axis along dir: makes a basis from dir, a
 * horizontal side vector and their cross product, then converts it as func_800A83A0 does. */
Quat_ func_800A8710(Vec3_ *dir) {
    Quat_ q = { 1.0f, 0.0f, 0.0f, 0.0f };
    Vec3_ f;
    Vec3_ side;
    Vec3_ up;
    f32 m[3][3];
    f32 *p[3] = { &q.x, &q.y, &q.z };
    f32 tr;
    f32 s;
    s32 i;
    s32 j;
    s32 k;

    f = *dir;
    Vec3_Normalize_(&f);
    side.x = f.y;
    side.y = 0.0f - f.x;
    side.z = 0.0f;
    up.x = 0.0f - f.x * f.z;
    up.y = 0.0f - f.y * f.z;
    up.z = f.y * f.y + f.x * f.x;
    Vec3_Normalize_(&side);
    Vec3_Normalize_(&up);
    m[0][0] = side.x;
    m[1][0] = side.y;
    m[2][0] = side.z;
    m[0][1] = f.x;
    m[1][1] = f.y;
    m[2][1] = f.z;
    m[0][2] = up.x;
    m[1][2] = up.y;
    m[2][2] = up.z;
    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f) {
        s = sqrtf(tr + 1.0f);
        q.w = s * 0.5f;
        s = 0.5f / s;
        q.x = (m[2][1] - m[1][2]) * s;
        /* The original sets y to 0 here instead of (m[0][2] - m[2][0]) * s. */
        q.y = 0.0f;
        q.z = (m[1][0] - m[0][1]) * s;
    } else {
        i = 0;
        if (m[1][1] > m[0][0]) {
            i = 1;
        }
        if (m[2][2] > m[i][i]) {
            i = 2;
        }
        j = D_800F3920[i];
        k = D_800F3920[j];
        s = sqrtf(m[i][i] - m[j][j] - m[k][k] + 1.0f);
        *p[i] = s * 0.5f;
        s = 0.5f / s;
        q.w = (m[k][j] - m[j][k]) * s;
        *p[j] = (m[j][i] + m[i][j]) * s;
        *p[k] = (m[k][i] + m[i][k]) * s;
    }
    return q;
}

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8BA4);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8C94);

void func_800A8E14(Quat_ *q) {
    f32 w = q->w;
    f32 x = q->x;
    f32 y = q->y;
    f32 z = q->z;
    f32 inv = 1.0f / sqrtf(w * w + x * x + (y * y + z * z));

    q->w *= inv;
    q->x *= inv;
    q->y *= inv;
    q->z *= inv;
}

Quat_ func_800A8EBC(f32 t, Quat_ a, Quat_ b) {
    Quat_ r;
    f32 c = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    f32 s0, s1, om, sinom;

    if (c < 0.0f) {
        c = -c;
        a.x = -a.x;
        a.y = -a.y;
        a.z = -a.z;
        a.w = -a.w;
    }
    if ((1.0f - c) > 0.001f) {
        om = func_8007AD24(c);
        sinom = sinf(om);
        s0 = sinf((1.0f - t) * om) / sinom;
        s1 = sinf(t * om) / sinom;
    } else {
        s0 = 1.0f - t;
        s1 = t;
    }
    r.x = s0 * a.x + s1 * b.x;
    r.y = s0 * a.y + s1 * b.y;
    r.z = s0 * a.z + s1 * b.z;
    r.w = s0 * a.w + s1 * b.w;
    return r;
}

Vec3_ func_800A9068(f32 t, Vec3_ a, Vec3_ b) {
    f32 s = 1.0f - t;

    a.x = a.x * s + b.x * t;
    a.y = a.y * s + b.y * t;
    a.z = a.z * s + b.z * t;
    return a;
}
