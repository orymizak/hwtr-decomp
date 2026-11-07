#include "common.h"
#include "include_asm.h"

s32 func_800D4EE0(s32);
extern s32 D_800FB170;
extern s32 D_800FB174;
s32 func_800D4B50();

void func_800D48A0(s32 arg0, s32 arg1) {
    if (D_800FB170 == 0) {
        D_800FB170 = arg0;
        if (D_800FB174 == 0) {
            D_800FB174 = arg0;
            func_800D4EE0(arg1);
        }
    }
}

void func_800D48E4(void) {
    if (D_800FB170 != 0) {
        func_800D4B50();
        D_800FB170 = 0;
        D_800FB174 = 0;
    }
}
