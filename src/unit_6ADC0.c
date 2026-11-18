#include "common.h"
#include "include_asm.h"
s32 func_800C9F78(void *);

s32 func_800C9ED0(s32 *);
extern s32 D_800FAB88;
extern s8 D_80124D60;
extern s8 D_80124D61;
extern s8 D_80124D62;

void func_800CA1C0(void) {
    D_80124D60 = 0;
    D_80124D61 = 0;
    D_80124D62 = 0;
    func_800C9ED0(&D_800FAB88);
}

void func_800CA1F8(void) {

}

void func_800CA200(void) {
    D_80124D60 = 1;
}

void func_800CA210(void) {
    D_80124D61 = 1;
}

    /* TODO (Bug check): D_80124D62 is set here and cleared in func_800CA1C0, but is never checked in func_800CA230 or anywhere else in this unit. Check if there was a missing state condition (e.g. returning 4). */
void func_800CA220(void) {
    D_80124D62 = 1;
}

s32 func_800CA230(void) {
    if (func_800C9F78(&D_800FAB88) == 0) {
        return 0;
    }
    /* TODO (Clean version): Declare D_80124D60..62 as u8 (or a state flags struct) to eliminate pointer-cast dereferences:
       if (D_80124D60) {
           return 3;
       }
       if (D_80124D61) {
           return 2;
       }
    */
    if (*(u8 *)&D_80124D60 != 0) {
        return 3;
    }
    if (*(u8 *)&D_80124D61 != 0) {
        return 2;
    }
    return 1;
}

