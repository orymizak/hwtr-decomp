#include "common.h"
#include "include_asm.h"
extern u8 *D_800F3B90;
extern void *D_800F3B94;
extern u32 D_801206C0;
s32 func_8009A770(s32 *);
void func_8009A488(void *);
void Memory_Free(void *);



INCLUDE_ASM("asm/nonmatchings/unit_4FE30", func_800AF230);

s32 func_800AF5B4(void) {
    u8 *e;
    u32 i;

    if (D_800F3B90 != NULL) {
        e = D_800F3B90;
        for (i = 0; i < D_801206C0; i++) {
            func_8009A770((s32 *)e);
            if (*(s32 *)(e + 0x5C) == 1) {
                Memory_Free(*(void **)(e + 0x60));
            }
            func_8009A488(e + 0xC);
            e += 0x88;
        }
        Memory_Free(D_800F3B90);
        Memory_Free(D_800F3B94);
        D_800F3B90 = NULL;
        D_800F3B94 = NULL;
    }
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_4FE30", func_800AF67C);

INCLUDE_ASM("asm/nonmatchings/unit_4FE30", func_800AF820);

s32 func_800AF8EC(s32 arg0, u32 arg1) {
    return (arg1 >= 4U) * 4;
}

void func_800AF8FC(void) {

}
