#include "common.h"
#include "include_asm.h"
s32 func_800D5910(void *, void *,s32);
s32 osCreateMesgQueue(void *, void *,s32);
extern u8 D_80127900;
extern u8 D_80127910;
extern u8 D_80134780;
extern u8 D_80144450;
extern u8 D_80145528;



INCLUDE_ASM("asm/nonmatchings/unit_6E200", func_800CD600);

void func_800CD6C4(void) {
    osCreateMesgQueue(&D_80145528, &D_80127900, 4);
    osCreateMesgQueue(&D_80134780, &D_80127910, 1);
    func_800D5910(&D_80144450, &D_80145528, 3);
}

INCLUDE_ASM("asm/nonmatchings/unit_6E200", func_800CD728);

INCLUDE_ASM("asm/nonmatchings/unit_6E200", func_800CD7B8);
