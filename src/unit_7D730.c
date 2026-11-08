#include "common.h"
#include "include_asm.h"
void guLookAtReflectF(f32 (*)[4], f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void guMtxF2L(f32 (*)[4], s32);



INCLUDE_ASM("asm/nonmatchings/unit_7D730", cosf);

INCLUDE_ASM("asm/nonmatchings/unit_7D730", guLookAtReflectF);

void guLookAtReflect(s32 m, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, f32 i, f32 j) {
    f32 mf[4][4];

    guLookAtReflectF(mf, a, b, c, d, e, f, g, h, i, j);
    guMtxF2L(mf, m);
}
