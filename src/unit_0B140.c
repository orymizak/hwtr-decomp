#include "common.h"
#include "gfx.h"
#include "include_asm.h"
typedef struct { s32 state; u8 pad[0x50]; } SlotC_;
extern SlotC_ gRumbleState[];
extern u8 D_800F0548[];
void nuContRmbForceStopEnd(void);
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errnum; u8 pad; } Pad_;
typedef struct { u8 buf[8]; s32 st; s32 mode; } PakSt_;
typedef struct { s32 keys[6]; u8 pad[0x54 - 0x18]; } KeySet_;
typedef struct { u8 c[0x54]; } Row6B_;
typedef struct { f32 x; f32 y; f32 z; f32 vx; f32 vy; u8 type; } EntAA_;
typedef struct { s32 keys[21]; } KeySetB_;
typedef struct { s32 mode; s32 k[15]; u8 pad[0x54 - 0x40]; } Bind6C_;
s32 Input_Update();
s32 Input_Rumble();
extern void *sysInputIsKeyHeld;
extern void *sysInputRumbleStopAll;
extern void *sysInputRumbleResume;
extern void *sysInputLoadBindings;
extern void *sysInputSaveBindings;
extern s32 gInputLayoutCount;
extern Bind6C_ gPadBindings[];
s32 Input_SetLayout(u8 pad, u8 mode);
extern s32 D_800F0394[];
extern s32 gCloudCount;
extern u8 D_80101E94;
extern u8 D_80101E95;
extern u8 D_80101E96;
extern EntAA_ gClouds[];
extern f32 D_800F02D0;
extern u8 D_800F0318[];
extern u8 D_800F0338[];
void Clouds_Update(void);
void Mtx34_Mul(s32, f32 (*)[4], f32 (*)[4]);
s32 Mtx_New(f32 (*)[4]);
extern Row6B_ gPadControls[];
extern KeySet_ D_800F0404[];
void nuContDataGet(Pad_ *, s32);
s32 nuContRmbCheck(s32);
void nuContRmbModeSet(s32, s32);
void nuContPakOpen(PakSt_ *, s32);
extern void nuContDataGetAll(Pad_ *);
extern u8 D_800F0403;
extern s32 D_800F0390[];
extern u8 D_800F03EB;
extern void *sysInputIsKeyHeldAny;
extern void *sysInputUpdate;
extern void *sysInputGetControl;
extern void *sysInputGetKeyName;
extern void *sysInputBindKey;
extern void *sysInputGetLayoutCount;
extern void *sysInputGetLayoutName;
extern void *sysInputGetLayout;
extern void *sysInputSetLayout;
extern void *sysInputHasRumble;
extern void *sysInputRumble;
extern void *sysInputStub;
extern void *sysInputIsPad1Connected;
extern void *sysInputResetLayout;

s32 Memory_Free(void *);
extern void *gCloudTextures[3];
extern s32 D_800F0414;
extern u8 gContPattern;
s32 nuContRmbForceStop();

INCLUDE_ASM("asm/nonmatchings/unit_0B140", Clouds_Init);

s32 Clouds_Free(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Memory_Free(gCloudTextures[i]);
        gCloudTextures[i] = NULL;
    }
    return 1;
}

s32 Clouds_Draw(s32 cam, f32 *pos) {
    f32 m[3][4];
    f32 o[3][4];
    f32 d[2];
    s32 i;
    s32 j;
    s32 n = 0;
    s32 mtx;
    EntAA_ *e;
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][1] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    Clouds_Update();
    GFX_RESERVE(9);
    gSPClipRatio(gDisplayListHead++, FRUSTRATIO_1);
    gSPDisplayList(gDisplayListHead++, D_800F0338);
    gDPPipeSync(gDisplayListHead++);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, D_80101E94, D_80101E95, D_80101E96, 0xC0);
    gDPSetDepthSource(gDisplayListHead++, G_ZS_PRIM);
    gDPSetPrimDepth(gDisplayListHead++, -4, 0);
    for (i = 0; i < 3; i++) {
        GFX_RESERVE(7);
        gDPLoadTextureBlock_4b(gDisplayListHead++, gCloudTextures[i], G_IM_FMT_I, 32, 32, 0, G_TX_WRAP | G_TX_NOMIRROR,
                               G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        for (j = 0, e = gClouds; j < gCloudCount; e++, j++) {
            if (e->type == i) {
                d[0] = pos[0] - e->x;
                d[1] = pos[1] - e->y;
                if (d[0] * d[0] + d[1] * d[1] < 1.296e9f) {
                    m[0][3] = e->x;
                    m[1][3] = e->y;
                    m[2][3] = e->z;
                    Mtx34_Mul(cam, m, o);
                    o[0][3] *= D_800F02D0;
                    o[1][3] *= D_800F02D0;
                    o[2][3] *= D_800F02D0;
                    mtx = Mtx_New(o);
                    GFX_RESERVE(2);
                    gSPMatrix(gDisplayListHead++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                    gSPDisplayList(gDisplayListHead++, D_800F0318);
                    n += 2;
                }
            }
        }
    }
    GFX_RESERVE(7);
    gSPClipRatio(gDisplayListHead++, FRUSTRATIO_3);
    gDPPipeSync(gDisplayListHead++);
    gDPSetColorDither(gDisplayListHead++, G_CD_MAGICSQ);
    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
    return n;
}

extern f32 D_80101E98[2];
extern f32 D_80101EA0[2];
void Clouds_Update(void) {
    s32 i;
    EntAA_ *p;

    for (i = 0, p = gClouds; i < gCloudCount; p++, i++) {
        p->x += p->vx;
        p->y += p->vy;
        if (p->x > D_80101E98[0]) {
            p->x = D_80101EA0[0];
        } else if (p->x < D_80101EA0[0]) {
            p->x = D_80101E98[0];
        }
        if (p->y > D_80101E98[1]) {
            p->y = D_80101EA0[1];
        } else if (p->y < D_80101EA0[1]) {
            p->y = D_80101E98[1];
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", Input_Init);

s32 Input_IsKeyHeld(u8 ch, s32 key);
s32 Input_IsKeyHeldAny(s32 key);
u8 Input_GetControl(u8 a, s32 b);
s32 Input_GetKeyName(u8 ch, s32 slot);
s32 Input_BindKey(u8 ch, s32 slot);
u8 Input_GetLayoutCount(void);
s32 Input_GetLayoutName(s32 arg0);
u8 Input_GetLayout(s32 arg0);
s32 Input_ResetLayout(s32 arg0);
s32 Input_HasRumble(u8 i);
s32 Input_Stub(void);
s32 Input_IsPad1Connected(void);
s32 Input_LoadBindings(u8 *buf, u8 id);
s32 Input_SaveBindings(u8 *out, u8 ch);
s32 Input_RumbleStopAll(void);
s32 Input_RumbleResume(void);
s32 Input_InitTable(void) {
    sysInputIsKeyHeldAny = Input_IsKeyHeldAny;
    sysInputIsKeyHeld = Input_IsKeyHeld;
    sysInputUpdate = Input_Update;
    sysInputGetControl = Input_GetControl;
    sysInputIsPad1Connected = Input_IsPad1Connected;
    sysInputGetKeyName = Input_GetKeyName;
    sysInputBindKey = Input_BindKey;
    sysInputGetLayoutCount = Input_GetLayoutCount;
    sysInputGetLayoutName = Input_GetLayoutName;
    sysInputGetLayout = Input_GetLayout;
    sysInputSetLayout = Input_SetLayout;
    sysInputResetLayout = Input_ResetLayout;
    sysInputRumbleStopAll = Input_RumbleStopAll;
    sysInputRumbleResume = Input_RumbleResume;
    sysInputHasRumble = Input_HasRumble;
    sysInputRumble = Input_Rumble;
    sysInputStub = Input_Stub;
    sysInputLoadBindings = Input_LoadBindings;
    sysInputSaveBindings = Input_SaveBindings;
    return 1;
}

    /* TODO (Clean version):
    sysInputGetLayout y sysInputSetLayout se asignan a 0 dos veces mientras que las variables void* no se limpian. (Original behavior.)
    */
s32 Input_ClearTable(void) {
    sysInputIsKeyHeldAny = NULL;
    sysInputUpdate = NULL;
    sysInputGetControl = NULL;
    sysInputIsPad1Connected = NULL;
    sysInputGetKeyName = NULL;
    sysInputBindKey = NULL;
    sysInputGetLayoutCount = NULL;
    sysInputGetLayoutName = NULL;
    sysInputGetLayout = NULL;
    sysInputSetLayout = NULL;
    sysInputResetLayout = NULL;
    sysInputHasRumble = NULL;
    sysInputRumble = NULL;
    sysInputStub = NULL;
    sysInputGetLayout = NULL;
    sysInputSetLayout = NULL;
    return 1;
}

s32 Input_IsKeyHeld(u8 ch, s32 key) {
    Pad_ pad[4];
    u16 mask;

    nuContDataGetAll(pad);
    switch (key) {
    case 14:
        if (pad[ch].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case 15:
        if (pad[ch].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case 16:
        if (pad[ch].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case 17:
        if (pad[ch].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case 18:
        mask = 0x8000;
        break;
    case 19:
        mask = 0x4000;
        break;
    case 20:
        mask = 8;
        break;
    case 21:
        mask = 4;
        break;
    case 22:
        mask = 0x2000;
        break;
    case 23:
        mask = 0x10;
        break;
    case 24:
        mask = 2;
        break;
    case 25:
        mask = 1;
        break;
    case 26:
        mask = 0x1000;
        break;
    case 27:
        mask = 0x9000;
        break;
    default:
        return 0;
    }
    return (pad[ch].button & mask) != 0;
}

void Input_MirrorSteering(void) {
    s32 i;
    s32 t;

    for (i = 0; i < 4; i++) {
        t = gPadBindings[i].k[0];
        gPadBindings[i].k[0] = gPadBindings[i].k[1];
        gPadBindings[i].k[1] = t;
        t = gPadBindings[i].k[5];
        gPadBindings[i].k[5] = gPadBindings[i].k[4];
        gPadBindings[i].k[4] = t;
    }
}

s32 Input_IsKeyHeldAny(s32 key) {
    Pad_ pad[4];
    u16 mask;

    nuContDataGetAll(pad);
    switch (key) {
    case 14:
        if (pad[0].stick_y > 0x28 || pad[1].stick_y > 0x28 || pad[2].stick_y > 0x28 || pad[3].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case 15:
        if (pad[0].stick_y < -0x28 || pad[1].stick_y < -0x28 || pad[2].stick_y < -0x28 || pad[3].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case 16:
        if (pad[0].stick_x < -0x28 || pad[1].stick_x < -0x28 || pad[2].stick_x < -0x28 || pad[3].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case 17:
        if (pad[0].stick_x > 0x28 || pad[1].stick_x > 0x28 || pad[2].stick_x > 0x28 || pad[3].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case 18:
        mask = 0x8000;
        break;
    case 19:
        mask = 0x4000;
        break;
    case 20:
        mask = 8;
        break;
    case 21:
        mask = 4;
        break;
    case 22:
        mask = 0x2000;
        break;
    case 23:
        mask = 0x10;
        break;
    case 24:
        mask = 2;
        break;
    case 25:
        mask = 1;
        break;
    case 26:
        mask = 0x1000;
        break;
    case 27:
        mask = 0x9000;
        break;
    default:
        return 0;
    }
    if ((pad[0].button & mask) || (pad[1].button & mask) || (pad[2].button & mask) || (pad[3].button & mask)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", Input_Update);

u8 Input_GetControl(u8 a, s32 b) {
    return gPadControls[a].c[b];
}

s32 Input_GetKeyName(u8 ch, s32 slot) {
    s32 v;
    s32 i;
    s32 m;

    v = D_800F0404[ch].keys[slot];
    for (i = 1, m = 1; m != v && i < 21; i++) {
        m <<= 1;
    }
    if (i < 21) {
        return D_800F0394[i];
    }
    return D_800F0394[0];
}

    /* TODO (Clean version):
    Cadena repetitiva de comprobaciones "if (mask == 0 && ...)" para emular selección de prioridad sin ramas else-if. En C limpio se escribiría con una cadena if / else if directa. Se mantiene para el match.
    */
s32 Input_BindKey(u8 ch, s32 slot) {
    Pad_ pad;
    s32 mask;

    nuContDataGet(&pad, ch);
    mask = 0;
    if (mask == 0 && pad.stick_x > 0x28) mask = 0x10000;
    if (mask == 0 && pad.stick_x < -0x28) mask = 0x20000;
    if (mask == 0 && pad.stick_y > 0x28) mask = 0x80000;
    if (mask == 0 && pad.stick_y < -0x28) mask = 0x40000;
    if (mask == 0 && (pad.button & 0x8000)) mask = 0x8000;
    if (mask == 0 && (pad.button & 0x4000)) mask = 0x4000;
    if (mask == 0 && (pad.button & 0x20)) mask = 0x20;
    if (mask == 0 && (pad.button & 0x10)) mask = 0x10;
    if (mask == 0 && (pad.button & 0x2000)) mask = 0x2000;
    if (mask == 0 && (pad.button & 0x800)) mask = 0x800;
    if (mask == 0 && (pad.button & 0x200)) mask = 0x200;
    if (mask == 0 && (pad.button & 0x100)) mask = 0x100;
    if (mask == 0 && (pad.button & 0x400)) mask = 0x400;
    if (mask == 0 && (pad.button & 8)) mask = 8;
    if (mask == 0 && (pad.button & 2)) mask = 2;
    if (mask == 0 && (pad.button & 1)) mask = 1;
    if (mask == 0 && (pad.button & 4)) mask = 4;
    if (mask != 0) {
        if (slot == 0 || slot == 5) {
            D_800F0404[ch].keys[0] = mask;
            D_800F0404[ch].keys[5] = mask;
        } else if (slot == 1 || slot == 4) {
            D_800F0404[ch].keys[1] = mask;
            D_800F0404[ch].keys[4] = mask;
        } else {
            D_800F0404[ch].keys[slot] = mask;
        }
        return 1;
    }
    return 0;
}

u8 Input_GetLayoutCount(void) {
    return D_800F03EB;
}

s32 Input_GetLayoutName(s32 arg0) {
    return D_800F0390[(u8)arg0];
}

    /* TODO (Clean version):
    Aritmética manual de punteros (*(u8 *)(&D_800F0403 + ((arg0 & 0xFF) * 0x54))) calculando el stride de 0x54 bytes manualmente sobre un u8. En C limpio debería accederse a través de un arreglo de estructuras (ej. gPadBindings[(u8)arg0].campo). Se mantiene para el match.
    */
u8 Input_GetLayout(s32 arg0) {
    return *(u8 *)(&D_800F0403 + ((arg0 & 0xFF) * 0x54));
}

s32 Input_SetLayout(u8 pad, u8 mode) {
    if (mode >= gInputLayoutCount) {
        return 0;
    }
    gPadBindings[pad].mode = mode;
    if (mode == 0) {
        gPadBindings[pad].k[0] = 0x20000;
        gPadBindings[pad].k[1] = 0x10000;
        gPadBindings[pad].k[2] = 0x8000;
        gPadBindings[pad].k[3] = 0x4000;
        gPadBindings[pad].k[4] = 0x10000;
        gPadBindings[pad].k[5] = 0x20000;
        gPadBindings[pad].k[6] = 0x40000;
        gPadBindings[pad].k[7] = 0x80000;
        gPadBindings[pad].k[8] = 0x10;
        gPadBindings[pad].k[9] = 1;
        gPadBindings[pad].k[10] = 0x2000;
        gPadBindings[pad].k[11] = 4;
        gPadBindings[pad].k[12] = 2;
        gPadBindings[pad].k[13] = 0x1000;
    }
    return 1;
}

s32 Input_ResetLayout(s32 arg0) {
    Input_SetLayout(arg0 & 0xFF, 0);
    return 1;
}

    /* TODO (Clean version):
    Arreglo local "unused[16]" no utilizado, forzado artificialmente para ajustar el tamaño del frame de pila y lograr el match binario. En C limpio debe eliminarse.
    */
s32 Input_HasRumble(u8 i) {
    u8 unused[16];

    return gRumbleState[i].state == 2;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", Input_Rumble);

s32 Input_Stub(void) {
    return 0;
}

s32 Input_IsPad1Connected(void) {
    return gContPattern & 1;
}

    /* TODO (Clean version):
    D_800F0404 fue declarado con KeySet_ donde keys tiene tamaño 6 (s32 keys[6]), pero el bucle itera hasta 13 escribiendo fuera de los límites del arreglo sobre el padding. En C limpio la estructura debería definir el arreglo con al menos 13 elementos (ej. s32 keys[13]). Se mantiene para el match.
    */
s32 Input_LoadBindings(u8 *buf, u8 id) {
    s32 sum;
    s32 i;

    sum = 0;
    for (i = 0; i < 13; i++) {
        sum += buf[i];
    }
    if ((u8)sum != buf[13]) {
        return 0;
    }
    for (i = 0; i < 13; i++) {
        D_800F0404[id].keys[i] = 1 << buf[i];
    }
    return 1;
}

    /* TODO (Clean version):
    D_800F0404[ch].keys[j] accede hasta el índice 12 en un struct declarado con keys[6], indexando fuera de los límites sobre el padding. En C limpio la estructura debería tener el tamaño de arreglo adecuado. Se mantiene para el match.
    */
s32 Input_SaveBindings(u8 *out, u8 ch) {
    s32 j;
    s32 n;
    s32 m;
    u8 sum;

    sum = 0;
    for (j = 0; j < 13; j++) {
        n = 0;
        m = 1;
        while (m != D_800F0404[ch].keys[j]) {
            m <<= 1;
            n++;
        }
        out[j] = n;
        sum += n;
    }
    out[13] = sum;
    return 1;
}

s32 Input_RumbleStopAll(void) {
    nuContRmbForceStop();
    return 1;
}

s32 Input_RumbleResume(void) {
    nuContRmbForceStopEnd();
    memset(D_800F0548, 0, 2);
    return 1;
}


s32 Rumble_Probe(u8 i) {
    if (nuContRmbCheck(i) == 0) {
        gRumbleState[i].state = 2;
        nuContRmbModeSet(i, 2);
        return 1;
    }
    return 0;
}

    /* TODO (Bug potential):
    Si nuContRmbCheck retorna un estado distinto de 0 y 11 (o si st == 11 pero st != 10 tras nuContPakOpen), s.mode queda sin inicializar en la primera iteración o retiene el valor de una iteración previa, asignando datos no inicializados o desactualizados a gRumbleState[i].state.
    */
s32 Rumble_ProbeAll(void) {
    PakSt_ s;
    s32 i;

    for (i = 0; i < 4; i++) {
        s.st = nuContRmbCheck(i);
        if (s.st == 0) {
            s.mode = 2;
            nuContRmbModeSet(i, 2);
        } else if (s.st == 11) {
            nuContPakOpen(&s, i);
            if (s.st == 10) {
                s.mode = 1;
            }
        }
        gRumbleState[i].state = s.mode;
    }
    return 1;
}

