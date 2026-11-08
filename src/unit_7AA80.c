#include "common.h"
#include "include_asm.h"
typedef struct ALFilt3_ { struct ALFilt3_ *source; void *handler; s32 (*setParam)(void *, s32, void *); } ALFilt3_;
typedef struct ALParam3_ { struct ALParam3_ *next; } ALParam3_;
typedef struct { ALFilt3_ filter; u8 padC[8]; s32 state; s16 pan; s16 volume; u8 pad1C[0x1C]; s32 first; ALParam3_ *ctrlList; ALParam3_ *ctrlTail; void **sources; s32 motion; } ALEnvMix3_;



INCLUDE_ASM("asm/nonmatchings/unit_7AA80", alEnvmixerPull);

s32 alEnvmixerParam(void *filter, s32 paramID, void *param) {
    ALFilt3_ *f = (ALFilt3_ *)filter;
    ALEnvMix3_ *e = (ALEnvMix3_ *)filter;

    switch (paramID) {
        case 3:
            if (e->ctrlTail) {
                e->ctrlTail->next = (ALParam3_ *)param;
            } else {
                e->ctrlList = (ALParam3_ *)param;
            }
            e->ctrlTail = (ALParam3_ *)param;
            break;
        case 4:
            e->first = 1;
            e->motion = 0;
            e->volume = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 4, param);
            break;
        case 9:
            e->motion = 1;
            if (f->source)
                (*f->source->setParam)(f->source, 9, param);
            break;
        case 1:
            f->source = (ALFilt3_ *)param;
            break;
        default:
            if (f->source)
                (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_7AA80", _pullSubFrame);

f64 _frexpf(f64 value, s32 *eptr) {
    f64 absvalue;

    *eptr = 0;
    if (value == 0.0)
        return (value);
    absvalue = (value > 0.0) ? value : -value;
    for (; absvalue >= 1.0; absvalue *= 0.5)
        ++*eptr;
    for (; absvalue < 0.5; absvalue += absvalue)
        --*eptr;
    return (value > 0.0 ? absvalue : -absvalue);
}

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
