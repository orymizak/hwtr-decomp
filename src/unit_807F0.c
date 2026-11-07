#include "common.h"
#include "include_asm.h"

extern s32 D_A4400000;

s32 osViGetStatus(void) {
    return D_A4400000;
}
