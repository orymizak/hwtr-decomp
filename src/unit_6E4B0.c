#include "common.h"
#include "include_asm.h"
s32 func_800CE110(s32, s32, s32, s32, s32);

s32 func_800CE090();

void func_800CD8B0(void) {
    func_800CE090();
}

s32 func_800CD8CC(s32 a, s32 b, s32 c, s32 d, s32 e) {
    return func_800CE110(a, b, c, d, e);
}
