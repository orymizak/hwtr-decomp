#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
extern char D_800FFAB0[];
extern s32 (*sysFileGetSize)(char *);
extern u32 (*sysFileLoad)(char *, void *);
extern u8 D_80115860[];
typedef struct { char s[5][20]; } CdtEntry_;
extern CdtEntry_ gCarNames[41];



/* Loads "<name>.cdt": 41 entries of 5 strings (up to 20 chars each, '|'-terminated). */
s32 CarNames_Load(char *name) {
    char buf[0x100];
    s32 size;
    u8 *src;
    u8 i;
    u8 j;
    u8 k;
    u8 done;
    u8 c;

    sprintf(buf, D_800FFAB0, name);
    size = sysFileGetSize(buf);
    if (size == 0) {
        return 0;
    }
    if (D_80115860 == NULL || sysFileLoad(buf, D_80115860) != size) {
        return 0;
    }
    src = D_80115860;
    for (i = 0; i < 41; i++) {
        for (j = 0; j < 5; j++) {
            k = 0;
            done = 0;
            do {
                c = *src;
                if (c == '|') {
                    gCarNames[i].s[j][k] = '\0';
                    done = 1;
                    k++;
                } else if (c != '\r' && c != '\n') {
                    gCarNames[i].s[j][k] = c;
                    k++;
                }
                src++;
            } while (k < 20 && !done);
        }
    }
    return 1;
}

void CarNames_Free(void) {

}
