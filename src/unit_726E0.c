#include "common.h"
#include "include_asm.h"
s32 osRecvMesg(void *, void *,s32);
extern void **D_80127990;
extern void **D_80127994;
extern s32 D_801279A8;
extern u8 D_801279B0;

extern s32 func_800D1D30;

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1AE0);

void func_800D1C54(void) {
    s32 sp10;
    s32 temp_v0;
    s32 temp_v0_2;
    void **temp_a1;
    void **temp_v0_3;
    void **var_v1;
    void *temp_a0;

    if (D_801279A8 != 0) {
        do {
            osRecvMesg(&D_801279B0, &sp10, 0);
            temp_v0 = D_801279A8 - 1;
            D_801279A8 = temp_v0;
        } while (temp_v0 != 0);
    }
    var_v1 = D_80127990;
    if (var_v1 != NULL) {
        do {
            temp_v0_2 = (*(s32 *)((u8 *)(var_v1) + 0x8)) - 1;
            (*(s32 *)((u8 *)(var_v1) + 0x8)) = temp_v0_2;
            if (temp_v0_2 == 0) {
                temp_a1 = (*(s32 *)((u8 *)(var_v1) + 0x4));
                if (temp_a1 != NULL) {
                    *temp_a1 = (*(s32 *)((u8 *)(var_v1) + 0x0));
                }
                temp_a0 = (*(s32 *)((u8 *)(var_v1) + 0x0));
                if (temp_a0 != NULL) {
                    (*(s32 *)((u8 *)(temp_a0) + 0x4)) = (void **) (*(s32 *)((u8 *)(var_v1) + 0x4));
                } else {
                    D_80127990 = (*(s32 *)((u8 *)(var_v1) + 0x4));
                }
                temp_v0_3 = D_80127994;
                (*(s32 *)((u8 *)(var_v1) + 0x0)) = NULL;
                D_80127994 = var_v1;
                (*(s32 *)((u8 *)(var_v1) + 0x4)) = temp_v0_3;
                var_v1 = temp_a1;
            } else {
                var_v1 = (*(s32 *)((u8 *)(var_v1) + 0x4));
            }
        } while (var_v1 != NULL);
    }
}

s32 *func_800D1D24(void) {
    return &func_800D1D30;
}

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1D30);

INCLUDE_ASM("asm/nonmatchings/unit_726E0", func_800D1D9C);
