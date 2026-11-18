#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern char D_800FFAB8[];
extern s32 (*sysFileGetSize)(char *);
extern u32 (*sysFileLoad)(char *, void *);
extern u8 D_801410A8[];
extern u8 D_80116870[];



u8 Keyboard_LoadCharset(char *name) {
    char buf[0x100];
    s32 size;
    u32 n;
    u32 i;
    u8 k;
    u8 c;

    sprintf(buf, D_800FFAB8, name);
    size = sysFileGetSize(buf);
    if (size == 0) {
        return 0;
    }
    if (D_801410A8 == NULL) {
        return 0;
    }
    n = sysFileLoad(buf, D_80116870);
    if (n != size) {
        return 0;
    }
    k = 0;
    for (i = 0; i < n; i++) {
        c = D_80116870[i];
        if (c != '\n' && c != '\r') {
            D_801410A8[k] = c;
            k++;
        }
    }
    return k;
}

void func_80086E2C(void) {

}
