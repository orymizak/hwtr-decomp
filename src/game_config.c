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
            /* TODO (Clean version):
            Magic dimensions (4 and 3) used for the 2D array lookup should be defined 
            using constants or macros (e.g., `#define MENU_ROWS 4`, `#define MENU_COLS 3`) 
            to prevent hardcoding issues if the grid size changes.
            */
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
    /* TODO (Clean version):
    Both `func_find` and `func_find2` perform table lookups for cheat codes but modify 
    different configuration fields. They could be unified into a single lookup function 
    that iterates over a unified table of cheat definitions, or handled via a generalized 
    flag-setting routine:

    static inline u8 Config_MatchCode(const char *code, const char **table, u32 size, s32 *out_index) {
        for (u32 i = 0; i < size; i++) {
            if (strcmp(code, table[i]) == 0) {
                *out_index = i;
                return 1;
            }
        }
        return 0;
    }
    */
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
        /* TODO (Clean version):
        The loop `for (j = 0; j < 1; j++)` iterates only once and is redundant. 
        It can be refactored into a direct condition check or structured to handle 
        multiple entries cleanly if the array size expands.
        */
        if (strcmp(code, D_800F3008[j]) == 0) {
            /* TODO (Clean version):
            The bitfield distribution between `unk14` and `unk18` handles more than 32 active flags.
            This can be refactored into a unified bitset array or an explicit enum-based flags struct:
            typedef struct {
                u32 flags[2]; // Replaces unk14 and unk18
            } CleanCheatFlags;
            */
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
