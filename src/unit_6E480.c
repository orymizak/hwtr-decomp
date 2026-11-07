#include "common.h"
#include "include_asm.h"

s32 func_800CD9A0();
extern s32 D_800FABC0;

void func_800CD880(void) {
    D_800FABC0 = func_800CD9A0();
}
