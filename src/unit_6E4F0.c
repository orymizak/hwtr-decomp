#include "common.h"
#include "include_asm.h"

s32 osSetIntMask(s32);
extern s32 D_800FABA4;

/* TODO (Clean version):
   Atomic variable update using libultra interrupt mask constants:
   OSIntMask mask = osSetIntMask(OS_IM_RCP); // 1 = OS_IM_RCP
   D_800FABA4 = arg0;
   osSetIntMask(mask);
*/
void func_800CD8F0(s32 arg0) {
    s32 mask;

    mask = osSetIntMask(1);
    D_800FABA4 = arg0;
    osSetIntMask(mask);
}
