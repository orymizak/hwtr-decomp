#include "common.h"
#include "include_asm.h"
typedef struct { s32 m[4][4]; } Mtx_;
void guMtxIdentF(f32 mf[4][4]);
f32 cosf(f32);
f32 sinf(f32);



void guMtxF2L(f32 mf[4][4], Mtx_ *m) {
    s32 i, j;
    s32 e1, e2;
    s32 *ai, *af;

    ai = (s32 *)&m->m[0][0];
    af = (s32 *)&m->m[2][0];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 2; j++) {
            e1 = (s32)(mf[i][j * 2] * 65536.0f);
            e2 = (s32)(mf[i][j * 2 + 1] * 65536.0f);
            *(ai++) = (e1 & 0xffff0000) | ((e2 >> 16) & 0xffff);
            *(af++) = ((e1 << 16) & 0xffff0000) | (e2 & 0xffff);
        }
}

void guMtxL2F(f32 mf[4][4], Mtx_ *m) {
    s32 i, j;
    u32 e1, e2;
    u32 *ai, *af;
    s32 q1, q2;

    ai = (u32 *)&m->m[0][0];
    af = (u32 *)&m->m[2][0];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 2; j++) {
            e1 = (*ai & 0xffff0000) | ((*af >> 16) & 0xffff);
            e2 = ((*(ai++) << 16) & 0xffff0000) | (*(af++) & 0xffff);
            q1 = *((s32 *)&e1);
            q2 = *((s32 *)&e2);
            mf[i][j * 2] = ((f32)q1 * (1.0f / 65536.0f));
            mf[i][j * 2 + 1] = ((f32)q2 * (1.0f / 65536.0f));
        }
}

void guMtxIdentF(f32 mf[4][4]) {
    s32 i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j) {
                mf[i][j] = 1.0;
            } else {
                mf[i][j] = 0.0;
            }
        }
    }
}

void guMtxIdent(Mtx_ *m) {
    f32 mf[4][4];

    guMtxIdentF(mf);
    guMtxF2L(mf, m);
}
