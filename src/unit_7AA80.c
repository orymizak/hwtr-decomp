#include "common.h"
#include "include_asm.h"



INCLUDE_ASM("asm/nonmatchings/unit_7AA80", alEnvmixerPull);

INCLUDE_ASM("asm/nonmatchings/unit_7AA80", alEnvmixerParam);

INCLUDE_ASM("asm/nonmatchings/unit_7AA80", _pullSubFrame);

INCLUDE_ASM("asm/nonmatchings/unit_7AA80", _frexpf);

f64 _ldexpf(f64 arg0, s32 arg2) {
    f64 var_fa0;

    var_fa0 = arg0;
    if (arg2 != 0) {
        var_fa0 *= (f64) (1 << arg2);
    }
    return var_fa0;
}

INCLUDE_ASM("asm/nonmatchings/unit_7AA80", _getRate);

f32 _getVol(f32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 var_ft0;
    f32 var_fv1;
    s32 var_a1;
    s32 var_v1;

    var_a1 = arg1 >> 3;
    if (var_a1 == 0) {
        return arg0;
    }
    var_ft0 = 1.0f;
    var_v1 = 0;
    var_fv1 = ((f32) (arg2 << 0x10) + (f32) (arg3 & 0xFFFF)) / 65536.0f;
loop_3:
    if (var_a1 & 1) {
        var_ft0 *= var_fv1;
    }
    var_a1 = var_a1 >> 1;
    if (var_a1 != 0) {
        var_v1 += 1;
        var_fv1 *= var_fv1;
        if (var_v1 >= 0x20) {

        } else {
            goto loop_3;
        }
    }
    return arg0 * var_ft0;
}

void alFilterNew(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)((u8 *)arg0 + 0x0) = 0;
    *(s32 *)((u8 *)arg0 + 0x4) = arg1;
    *(s32 *)((u8 *)arg0 + 0x8) = arg2;
    *(s16 *)((u8 *)arg0 + 0xC) = 0;
    *(s16 *)((u8 *)arg0 + 0xE) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = arg3;
}
