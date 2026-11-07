#include "common.h"
#include "include_asm.h"

extern void *D_800FB5B0;
extern f64 D_80101A30;
extern f64 D_80101A38;
extern f64 D_80101A40;
extern f64 D_80101A48;

INCLUDE_ASM("asm/nonmatchings/unit_7C960", func_800DBD60);

INCLUDE_ASM("asm/nonmatchings/unit_7C960", func_800DC02C);

s32 *func_800DC1DC(void) {
    s32 *temp_v0;
    s32 *var_v1;

    temp_v0 = *(s32 *)((u8 *)D_800FB5B0 + 0x2C);
    var_v1 = NULL;
    if (temp_v0 != NULL) {
        var_v1 = temp_v0;
        *(s32 *)((u8 *)D_800FB5B0 + 0x2C) = (s32 *) *var_v1;
        *var_v1 = NULL;
    }
    return var_v1;
}

INCLUDE_ASM("asm/nonmatchings/unit_7C960", func_800DC208);

INCLUDE_ASM("asm/nonmatchings/unit_7C960", func_800DC220);

INCLUDE_ASM("asm/nonmatchings/unit_7C960", func_800DC274);

s32 func_800DC2B4(void *arg0, s32 arg1) {
    return (s32) (f32) (((f64) ((f32) arg1 * (f32) *(s32 *)((u8 *)arg0 + 0x44)) / D_80101A30) + D_80101A38);
}

s32 func_800DC2FC(void *arg0, s32 arg1) {
    return (s32) (f32) (((f64) ((f32) arg1 * (f32) *(s32 *)((u8 *)arg0 + 0x44)) / D_80101A40) + D_80101A48) & ~0xF;
}

s32 func_800DC348(void *arg0, void **arg1) {
    s32 var_a3;
    void *var_a2;

    *arg1 = NULL;
    var_a2 = *(s32 *)((u8 *)arg0 + 0x0);
    var_a3 = 0x7FFFFFFF;
    if (var_a2 != NULL) {
        do {
            if ((*(s32 *)((u8 *)var_a2 + 0x10) - *(s32 *)((u8 *)arg0 + 0x20)) < var_a3) {
                *arg1 = var_a2;
                var_a3 = *(s32 *)((u8 *)var_a2 + 0x10) - *(s32 *)((u8 *)arg0 + 0x20);
            }
            var_a2 = *(s32 *)((u8 *)var_a2 + 0x0);
        } while (var_a2 != NULL);
    }
    return *(s32 *)((u8 *)(*arg1) + 0x10);
}
