#include "common.h"
#include "include_asm.h"
void guMtxF2L(f32 (*)[4], s32);



extern const f64 D_80101A58;
extern const f64 D_80101A60;
extern const f64 D_80101A68;
extern const f64 D_80101A70;
extern const f64 D_80101A78;
extern const f64 D_80101A80;
extern const f64 D_80101A88;
extern const f32 D_80101A90;
extern const f32 D_80101BE0;
f32 cosf(f32 x) {
    f32 absx;
    f64 dx, xsq, poly;
    f64 dn;
    s32 n;
    f64 result;
    s32 ix, xpt;

    ix = *(s32 *)&x;
    xpt = (ix >> 22);
    xpt &= 0x1ff;
    if (xpt < 0x136) {
        absx = (x > 0) ? x : -x;
        dx = absx;
        dn = dx * D_80101A78 + 0.5;
        n = (s32)((dn >= 0.0) ? (dn + 0.5) : (dn - 0.5));
        dn = n;
        dn -= 0.5;
        dx = dx - dn * D_80101A80;
        dx = dx - dn * D_80101A88;
        xsq = dx * dx;
        poly = ((D_80101A70 * xsq + D_80101A68) * xsq + D_80101A60) * xsq + D_80101A58;
        result = dx + (dx * xsq) * poly;
        if ((n & 1) == 0)
            return ((f32)result);
        return (-(f32)result);
    }
    if (x != x) {
        return (D_80101BE0);
    }
    return (D_80101A90);
}

typedef struct { u8 col[3]; char pad1; u8 colc[3]; char pad2; s8 dir[3]; char pad3; s32 padC; } LightT_;
typedef struct { LightT_ l[2]; } LookAt_;
void guMtxIdentF(f32 mf[4][4]);
f32 sqrtf(f32);
#define FTOFRAC8(x) ((s32)(((x) * (128.0f)) < 127.0f ? ((x) * (128.0f)) : 127.0f) & 0xff)
void guLookAtReflectF(f32 mf[4][4], LookAt_ *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 len, xLook, yLook, zLook, xRight, yRight, zRight;

    guMtxIdentF(mf);
    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = -1.0 / sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;
    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = 1.0 / sqrtf(xRight * xRight + yRight * yRight + zRight * zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;
    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = 1.0 / sqrtf(xUp * xUp + yUp * yUp + zUp * zUp);
    xUp *= len;
    yUp *= len;
    zUp *= len;
    l->l[0].dir[0] = FTOFRAC8(xRight);
    l->l[0].dir[1] = FTOFRAC8(yRight);
    l->l[0].dir[2] = FTOFRAC8(zRight);
    l->l[1].dir[0] = FTOFRAC8(xUp);
    l->l[1].dir[1] = FTOFRAC8(yUp);
    l->l[1].dir[2] = FTOFRAC8(zUp);
    l->l[0].col[0] = 0x00;
    l->l[0].col[1] = 0x00;
    l->l[0].col[2] = 0x00;
    l->l[0].pad1 = 0x00;
    l->l[0].colc[0] = 0x00;
    l->l[0].colc[1] = 0x00;
    l->l[0].colc[2] = 0x00;
    l->l[0].pad2 = 0x00;
    l->l[1].col[0] = 0x00;
    l->l[1].col[1] = 0x80;
    l->l[1].col[2] = 0x00;
    l->l[1].pad1 = 0x00;
    l->l[1].colc[0] = 0x00;
    l->l[1].colc[1] = 0x80;
    l->l[1].colc[2] = 0x00;
    l->l[1].pad2 = 0x00;
    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);
    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);
    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);
    mf[0][3] = 0;
    mf[1][3] = 0;
    mf[2][3] = 0;
    mf[3][3] = 1;
}

void guLookAtReflect(s32 m, LookAt_ *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 mf[4][4];

    guLookAtReflectF(mf, l, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    guMtxF2L(mf, m);
}
