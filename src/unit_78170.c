#include "common.h"
#include "include_asm.h"
s32 func_800D76B4(void *,s32);
extern s32 (*D_800FB230)(s16);
extern u8 D_801347A0;
extern s32 D_801380AC;
extern u8 D_80141090;
s32 osRecvMesg(void *,s32,s32);
s32 osSendMesg(void *,s32,s32);
extern u8 D_8014ECB0;

s32 func_800D74F0(s32 *);
extern s32 D_800FB248;

INCLUDE_ASM("asm/nonmatchings/unit_78170", func_800D7570);

void func_800D7644(void) {
    func_800D74F0(&D_800FB248);
}

void func_800D7664(void) {
    osSendMesg(&D_8014ECB0, 0, 1);
}

void func_800D768C(void) {
    osRecvMesg(&D_8014ECB0, 0, 1);
}

INCLUDE_ASM("asm/nonmatchings/unit_78170", func_800D76B4);

INCLUDE_ASM("asm/nonmatchings/unit_78170", func_800D773C);

s32 func_800D778C(s16 *arg0) {
    if (D_801380AC == 0) {
        osRecvMesg(&D_801347A0, 0, 0);
        func_800D76B4(&D_80141090, 1);
        if (D_800FB230 != NULL) {
            D_800FB230(*arg0);
        }
        osSendMesg(&D_801347A0, 0, 0);
    }
    return 0;
}

void func_800D7818(void *arg0) {
    ((s32 (*)(s32,s32))func_800D76B4)((*(s32 *)((u8 *)(arg0) + 0xC)), 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_78170", func_800D7838);
