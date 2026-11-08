#include "common.h"
#include "include_asm.h"
typedef struct { s32 m[4][4]; } Mtx_;
void guMtxIdentF(f32 mf[4][4]);
void guMtxF2L(f32 mf[4][4], Mtx_ *m);
f32 cosf(f32);
f32 sinf(f32);

void guPerspectiveF(f32 mf[4][4], u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    f32 cot;
    s32 i, j;

    guMtxIdentF(mf);
    fovy *= 3.1415926 / 180.0;
    cot = cosf(fovy / 2) / sinf(fovy / 2);
    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1;
    mf[3][2] = (2 * near * far) / (near - far);
    mf[3][3] = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }
    if (perspNorm != NULL) {
        if (near + far <= 2.0) {
            *perspNorm = (u16)0xFFFF;
        } else {
            *perspNorm = (u16)((2.0 * 65536.0) / (near + far));
            if (*perspNorm <= 0) {
                *perspNorm = (u16)0x0001;
            }
        }
    }
}

void guPerspective(Mtx_ *m, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    f32 mf[4][4];

    guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);
    guMtxF2L(mf, m);
}
