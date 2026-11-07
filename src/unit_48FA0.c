#include "common.h"
#include "include_asm.h"
f32 func_8007AD24(f32);
f32 cosf(f32);
f32 sinf(f32);
typedef struct { f32 x; f32 y; f32 z; f32 w; } Quat_;
typedef struct { f32 x; f32 y; f32 z; } Vec3_;



INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A83A0);

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

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8710);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8BA4);

INCLUDE_ASM("asm/nonmatchings/unit_48FA0", func_800A8C94);

void func_800A8E14(void *arg0) {
    f32 temp_ft0;
    f32 temp_ft1;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp_fv1_2;

    temp_ft1 = *(f32 *)((u8 *)arg0 + 0xC);
    temp_ft0 = *(f32 *)((u8 *)arg0 + 0x0);
    temp_fv1 = *(f32 *)((u8 *)arg0 + 0x4);
    temp_fv0 = *(f32 *)((u8 *)arg0 + 0x8);
    temp_fv1_2 = 1.0f / sqrtf((temp_ft1 * temp_ft1) + (temp_ft0 * temp_ft0) + ((temp_fv1 * temp_fv1) + (temp_fv0 * temp_fv0)));
    *(f32 *)((u8 *)arg0 + 0xC) = (f32) (*(f32 *)((u8 *)arg0 + 0xC) * temp_fv1_2);
    *(f32 *)((u8 *)arg0 + 0x0) = (f32) (*(f32 *)((u8 *)arg0 + 0x0) * temp_fv1_2);
    *(f32 *)((u8 *)arg0 + 0x4) = (f32) (*(f32 *)((u8 *)arg0 + 0x4) * temp_fv1_2);
    *(f32 *)((u8 *)arg0 + 0x8) = (f32) (*(f32 *)((u8 *)arg0 + 0x8) * temp_fv1_2);
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
