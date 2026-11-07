#include "common.h"
#include "include_asm.h"
void func_800DCC80(f32 (*)[4], f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_800DD270(f32 (*)[4], s32);



INCLUDE_ASM("asm/nonmatchings/unit_7D730", cosf);

INCLUDE_ASM("asm/nonmatchings/unit_7D730", func_800DCC80);

void func_800DD1F8(s32 m, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, f32 i, f32 j) {
    f32 mf[4][4];

    func_800DCC80(mf, a, b, c, d, e, f, g, h, i, j);
    func_800DD270(mf, m);
}
