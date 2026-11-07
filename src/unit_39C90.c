#include "common.h"
#include "include_asm.h"

extern void *D_8011E860;
extern void *D_8011B334;

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099090);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009930C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800993B8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099434);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009951C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099594);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800996B8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099C94);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099E78);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A488);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A4E4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A770);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A9A0);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009ABC8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009B0AC);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009B56C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009BAF0);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009C758);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009D574);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009D67C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009DBF8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009EC8C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009F7D4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A0C44);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A258C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A25EC);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A34B4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3978);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3CD8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3D64);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3DB8);

void func_800A3E30(void **arg0, s32 arg1) {
    u16 temp_v0;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_v0_2;
    void *var_v1;

    var_v1 = *arg0;
    if (var_v1 != NULL) {
loop_1:
        if (*(s32 *)((u8 *)var_v1 + 0x0) != arg1) {
            var_v1 = *(s32 *)((u8 *)var_v1 + 0x8);
            if (var_v1 != NULL) {
                goto loop_1;
            }
        }
        if (var_v1 != NULL) {
            temp_v0 = *(u16 *)((u8 *)var_v1 + 0xC) - 1;
            *(u16 *)((u8 *)var_v1 + 0xC) = temp_v0;
            if (!(temp_v0 & 0xFFFF)) {
                temp_a1 = *(s32 *)((u8 *)var_v1 + 0x8);
                if (temp_a1 != NULL) {
                    *(s32 *)((u8 *)temp_a1 + 0x4) = (void *) *(s32 *)((u8 *)var_v1 + 0x4);
                }
                temp_a1_2 = *(s32 *)((u8 *)var_v1 + 0x4);
                if (temp_a1_2 != NULL) {
                    *(s32 *)((u8 *)temp_a1_2 + 0x8) = (void *) *(s32 *)((u8 *)var_v1 + 0x8);
                }
                if (var_v1 == *arg0) {
                    *arg0 = *(s32 *)((u8 *)var_v1 + 0x8);
                }
                temp_v0_2 = D_8011E860;
                *(s32 *)((u8 *)var_v1 + 0x4) = NULL;
                D_8011E860 = var_v1;
                *(s32 *)((u8 *)var_v1 + 0x8) = temp_v0_2;
                *(s32 *)((u8 *)temp_v0_2 + 0x4) = var_v1;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3ED4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4120);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4198);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4EE8);

s32 func_800A5894(void) {
    return *(s32 *)((u8 *)D_8011B334 + 0x14);
}

s32 func_800A58A4(s32 arg0) {
    return *(s32 *)((u8 *)D_8011B334 + 0xD4) + (arg0 * 0x18);
}
