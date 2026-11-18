#include "common.h"
#include "include_asm.h"

extern s32 D_800FB170;
extern s32 n_syn;

void func_800D48A0(s32 arg0, s32 arg1) {
    if (D_800FB170 == 0) {
        D_800FB170 = arg0;
        if (n_syn == 0) {
            n_syn = arg0;
            n_alSynNew(arg1);
        }
    }
}

void func_800D48E4(void) {
    if (D_800FB170 != 0) {
        n_alSynDelete();
        D_800FB170 = 0;
        n_syn = 0;
    }
}
