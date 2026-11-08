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

typedef struct { u8 p0[0x14]; u32 unk14; u32 unk18; u8 p1C[0x20]; u32 unk3C; } CheatCfg_;
extern char *gCheatButtonCodes[];
extern char *D_800F3008[];
extern char *D_800F300C[];
s8 func_800B513C(char *s);
static inline u8 func_find(char *code, CheatCfg_ *cfg) {
    u32 i;

    for (i = 0; i < 10; i++) {
        if (strcmp(code, gCheatButtonCodes[i]) == 0) {
            cfg->unk3C = 1 << i;
            return 1;
        }
    }
    return 0;
}
static inline u8 func_find2(char *code, CheatCfg_ *cfg) {
    s32 j;
    s8 bit;

    for (j = 0; j < 1; j++) {
        if (strcmp(code, D_800F3008[j]) == 0) {
            bit = func_800B513C(D_800F300C[j]);
            if (bit < 32) {
                cfg->unk14 |= 1 << bit;
            } else {
                cfg->unk18 |= 1 << bit;
            }
            return 1;
        }
    }
    return 0;
}
/* Trucos de botones del menú de opciones: cada código son 8 dígitos ASCII, uno por botón
 * ('1' C-arriba, '2' C-abajo, '3' Z, '4' R, '5' C-izquierda, '6' C-derecha). El código coincidente
 * pone su bit en cfg->unk3C. Las entradas con "77777777" no se pueden teclear (truco desactivado). */
s32 Config_CheckCheatCode(char *code, CheatCfg_ *cfg) {
    if (func_find(code, cfg)) {
        return 1;
    }
    if (func_find2(code, cfg)) {
        return 1;
    }
    return 0;
}
