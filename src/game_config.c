#include "common.h"
#include "profile.h"
#include "include_asm.h"

extern u8 gMenuFsm[];
extern char *gTrackNames[4][3];

s32 strcmp(const char *a, const char *b);
u8 Tables_InstallAll(void);
u8 Tables_UninstallAll(void);
void Fsm_Reset(void *arg0);
s32 Fsm_Update(void *arg0);

s32 Game_InitMenus(void) {
    if (Tables_InstallAll()) {
        Fsm_Reset(gMenuFsm);
        return 1;
    }
    return 0;
}

s32 Game_ShutdownMenus(void) {
    return Tables_UninstallAll() != 0;
}

s32 Game_MenuFrame(void) {
    return Fsm_Update(gMenuFsm) == 0;
}

/* Tracks are grouped in four cups of three (gTrackNames[cup][track]). */
#define NUM_CUPS 4
#define TRACKS_PER_CUP 3

s32 Track_FindByName(char *name) {
    u8 i;
    u8 j;

    for (i = 0; i < NUM_CUPS; i++) {
        for (j = 0; j < TRACKS_PER_CUP; j++) {
            if (strcmp(name, gTrackNames[i][j]) == 0) {
                return i * TRACKS_PER_CUP + j;
            }
        }
    }
    return -1;
}

extern char *gCheatButtonCodes[];
extern char *D_800F3008[];
extern char *D_800F300C[];
s8 CarModel_FindByName(char *s);
    /* TODO (Clean version): [evaluated 2026-11-18: kept as two inline helpers; a shared table
    walker changes the ROM]
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
static inline u8 func_find(char *code, Profile *cfg) {
    u32 i;

    for (i = 0; i < 10; i++) {
        if (strcmp(code, gCheatButtonCodes[i]) == 0) {
            cfg->cheats = 1 << i;
            return 1;
        }
    }
    return 0;
}
static inline u8 func_find2(char *code, Profile *cfg) {
    s32 j;
    s8 bit;

    for (j = 0; j < 1; j++) {
        /* TODO (Clean version): [evaluated 2026-11-18: a direct check without the loop changes
        the ROM]
        The loop `for (j = 0; j < 1; j++)` iterates only once and is redundant. 
        It can be refactored into a direct condition check or structured to handle 
        multiple entries cleanly if the array size expands.
        */
        if (strcmp(code, D_800F3008[j]) == 0) {
            /* For bit >= 32, 1 << bit is undefined in C, but MIPS sllv only uses the low five bits,
               so this sets bit (bit - 32) as intended. */
            bit = CarModel_FindByName(D_800F300C[j]);
            if (bit < 32) {
                cfg->carUnlocks[0] |= 1 << bit;
            } else {
                cfg->carUnlocks[1] |= 1 << bit;
            }
            return 1;
        }
    }
    return 0;
}

/* Trucos de botones del menú de opciones: cada código son 8 dígitos ASCII, uno por botón
 * ('1' C-arriba, '2' C-abajo, '3' Z, '4' R, '5' C-izquierda, '6' C-derecha). El código coincidente
 * pone su bit en cfg->cheats. Las entradas con "77777777" no se pueden teclear (truco desactivado). */
s32 Config_CheckCheatCode(char *code, Profile *cfg) {
    if (func_find(code, cfg)) {
        return 1;
    }
    if (func_find2(code, cfg)) {
        return 1;
    }
    return 0;
}
