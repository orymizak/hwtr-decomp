#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern char D_800FFAB8[];
extern s32 (*D_80140FE4)(char *);
extern u32 (*D_80140FE0)(char *, void *);
extern u8 D_801410A8[];
extern u8 D_80116870[];



u8 func_80086D50(char *name) {
    char buf[0x100];
    s32 size;
    u32 n;
    u32 i;
    u8 k;
    u8 c;

    sprintf(buf, D_800FFAB8, name);
    size = D_80140FE4(buf);
    if (size == 0) {
        return 0;
    }
    if (D_801410A8 == NULL) {
        return 0;
    }
    n = D_80140FE0(buf, D_80116870);
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
