#include "common.h"
#include "include_asm.h"
s32 alLink(s32, void *);
s32 alUnlink(s32);

extern void *D_800FB174;
extern f64 D_801017B0;
extern f64 D_801017B8;
extern f64 D_801017C0;
extern f64 D_801017C8;

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D4EE0);

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D5150);

s32 *func_800D52B8(void) {
    s32 *temp_v0;
    s32 *var_v1;

    temp_v0 = (*(s32 *)((u8 *)(D_800FB174) + 0x2C));
    var_v1 = NULL;
    if (temp_v0 != NULL) {
        var_v1 = temp_v0;
        (*(s32 *)((u8 *)(D_800FB174) + 0x2C)) = (s32 *) *var_v1;
        *var_v1 = NULL;
    }
    return var_v1;
}

INCLUDE_ASM("asm/nonmatchings/unit_75AE0", func_800D52E4);

void func_800D52FC(void) {
    s32 var_s0;

    var_s0 = (*(s32 *)((u8 *)(D_800FB174) + 0x14));
    if (var_s0 != 0) {
        do {
            alUnlink(var_s0);
            alLink(var_s0, D_800FB174 + 4);
            var_s0 = (*(s32 *)((u8 *)(D_800FB174) + 0x14));
        } while (var_s0 != 0);
    }
}

void func_800D535C(s32 arg0) {
    ((s32 (*)())alUnlink)();
    ((s32 (*)(s32, s32))alLink)(arg0, (*(s32 *)&D_800FB174) + 0x14);
}

s32 func_800D5394(s32 arg0) {
    return (s32) (f32) (((f64) ((f32) arg0 * (f32) *(s32 *)((u8 *)D_800FB174 + 0x40)) / D_801017B0) + D_801017B8);
}

s32 func_800D53E4(s32 arg0) {
    return (s32) (f32) (((f64) ((f32) arg0 * (f32) *(s32 *)((u8 *)D_800FB174 + 0x40)) / D_801017C0) + D_801017C8) & ~0xF;
}
