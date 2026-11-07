#include "common.h"
#include "include_asm.h"

extern u8 D_800F9F70[];
extern char *D_800F9FA0[4][3];

s32 strcmp(const char *a, const char *b);
u8 func_800875A0(void);
u8 func_80087660(void);
void func_800C9ED0(void *arg0);
s32 func_800C9F78(void *arg0);

s32 func_80087720(void) {
    if (func_800875A0()) {
        func_800C9ED0(D_800F9F70);
        return 1;
    }
    return 0;
}

s32 func_80087758(void) {
    return func_80087660() != 0;
}

s32 func_8008777C(void) {
    return func_800C9F78(D_800F9F70) == 0;
}

s32 func_800877A0(char *name) {
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (strcmp(name, D_800F9FA0[i][j]) == 0) {
                return i * 3 + j;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/game_config", func_80087864);
