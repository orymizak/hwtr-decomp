#include "common.h"
#include "math.h"
#include "input.h"
#include "gfx.h"
#include "include_asm.h"
/* Per-controller state (0x54). */
typedef struct { s32 state; u32 time; s32 mode; u32 k[14]; u8 keys[14]; u8 p52[2]; } SlotC_;
extern SlotC_ gRumbleState[];
extern u8 D_800F0548[];
void nuContRmbForceStopEnd(void);
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errnum; u8 pad; } Pad_;
typedef struct { u8 buf[8]; s32 st; s32 mode; } PakSt_;
typedef struct { u8 c[0x54]; } Row6B_;
typedef struct { f32 x; f32 y; f32 z; f32 vx; f32 vy; u8 type; } EntAA_;
typedef struct { s32 keys[21]; } KeySetB_;
typedef struct { s32 mode; s32 k[15]; u8 pad[0x54 - 0x40]; } Bind6C_;
void Input_Update(u8 pad);
s32 Input_Rumble();
extern void *sysInputIsKeyHeld;
extern void *sysInputRumbleStopAll;
extern void *sysInputRumbleResume;
extern void *sysInputLoadBindings;
extern void *sysInputSaveBindings;
/* Controller block: layout count, then gContPattern (+0xC) and the four gRumbleState slots (+0x10).
   Input_Init addresses all of it from this symbol. */
typedef struct { s32 layouts; u8 p4[8]; u8 pattern; u8 pD[3]; SlotC_ slot[4]; } InputCtl_;
extern InputCtl_ gInputLayoutCount;
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
void nuContDataGet(Pad_ *, s32);
s32 nuContRmbCheck(s32);
void nuContRmbModeSet(s32, s32);
void nuContPakOpen(PakSt_ *, s32);
extern void nuContDataGetAll(Pad_ *);
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

typedef struct { f32 x; f32 y; } Vec2f_;
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    Vec2f_ vel;
    u8 tex;
    u8 pad15[3];
} Cloud_;

extern f32 D_80101E98[2];
extern f32 D_80101EA0[2];
extern char D_800FEA00[];
u32 Rand_Range(s32);
s32 File_GetSize(char *);
void *Memory_Alloc(s32);
void File_Load(char *, void *);

s32 Clouds_Init(Vec3f *min, Vec3f *max, u8 r, u8 g, u8 b) {
    char name[16];
    Vec2f_ vel;
    f32 z;
    f32 dx;
    f32 dy;
    s32 nx;
    s32 ny;
    s32 i;
    s32 j;
    s32 k;
    f32 x;
    f32 y;
    Cloud_ *c;

    D_80101E94 = r;
    D_80101E95 = g;
    D_80101E96 = b;
    {
        s32 rv = Rand_Range(100);
        vel.x = (f64)(rv % 7 - 3);
        rv = Rand_Range(100);
        vel.y = (f64)(rv % 7 - 3);
    }

    dx = (max->x - min->x) / 6.0f;
    nx = 6;
    ny = 6;
    if (dx < 14000.0f) {
        nx = (s32)(((f64)(max->x - min->x) + 21000.0) / 14000.0);
        if (nx > 6) {
            nx = 6;
        }
        dx = 14000.0f;
    }

    dy = (max->y - min->y) / 6.0f;
    if (dy < 14000.0f) {
        ny = (s32)(((f64)(max->y - min->y) + 21000.0) / 14000.0);
        if (ny > 6) {
            ny = 6;
        }
        dy = 14000.0f;
    }

    max->x += dx;
    max->y += dy;
    min->x -= dx;
    min->y -= dy;

    if (max->z + 1200.0f > 10000.0f) {
        z = max->z + 1200.0f;
    } else {
        z = 10000.0f;
    }

    c = (Cloud_ *)gClouds;
    x = min->x;
    for (i = 0; i < nx; i++, x += dx) {
        y = min->y;
        for (j = 0; j < ny; c++, j++, y += dy) {
            c->x = x + (f32)Rand_Range((s32)(dx / 2.0f)) + dx / 2.0f;
            c->y = y + (f32)Rand_Range((s32)(dy / 2.0f)) + dy / 2.0f;
            c->z = z + (f32)Rand_Range(1000);
            c->vel = vel;
            c->tex = Rand_Range(1000) % 3;
        }
    }

    for (k = nx * ny; k < 36; c++, k++) {
        c->x = 0.0f;
        c->y = 0.0f;
        c->z = 0.0f;
        c->vel = vel;
        c->tex = 3;
    }

    for (i = 0; i < 3; i++) {
        void *buf;
        sprintf(name, D_800FEA00, i);
        buf = Memory_Alloc(File_GetSize(name));
        File_Load(name, buf);
        gCloudTextures[i] = buf;
    }

    D_80101E98[0] = max->x;
    D_80101E98[1] = max->y;
    D_80101EA0[0] = min->x;
    gCloudCount = nx * ny;
    D_80101EA0[1] = min->y;
    return 1;
}

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

/* OSContPad as filled by nuContDataRead: Pad_ without the explicit tail byte (with it GCC copies the
   local initialiser in a different order). */
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errnum; } ContPadII_;
u8 nuContInit(void);
void nuContDataRead(ContPadII_ *);
void nuContPakCodeSet(char *, char *);
void PakMenu_Run(s32);
u32 osGetCount(void);
extern char D_800FEB38[];
extern char D_800FEB3C[];

/* Initialises the controller slots, then picks the controlling pad: the only connected one, or the
 * first connected pad holding START. */
s32 Input_Init(void) {
    s32 n = 0;
    ContPadII_ pads[4] = { { 0 }, { 0 }, { 0 }, { 0 } };
    s32 i;
    s32 j;

    gInputLayoutCount.layouts = 1;
    gInputLayoutCount.pattern = nuContInit();
    for (i = 0; i < 4; i++) {
        Input_SetLayout(i, 0);
        gInputLayoutCount.slot[i].time = osGetCount();
        for (j = 13; j >= 0; j--) {
            gInputLayoutCount.slot[i].keys[j] = 0;
        }
    }
    nuContPakCodeSet(D_800FEB38, D_800FEB3C);
    Rumble_ProbeAll();
    nuContDataRead(pads);
    for (i = 0; i < 4; i++) {
        n += gInputLayoutCount.slot[i].state == 1;
    }
    if (n != 0) {
        if (n == 1) {
            i = 0;
            while (!(pads[i].button & 0x1000)) {
                if (++i >= 4) {
                    break;
                }
            }
            if (i < 4) {
                for (i = 0; i < 4; i++) {
                    if (gInputLayoutCount.slot[i].state == 1) {
                        break;
                    }
                }
                PakMenu_Run(i);
            }
        } else {
            for (i = 0; i < 4; i++) {
                if ((pads[i].button & 0x1000) && gInputLayoutCount.slot[i].state == 1) {
                    break;
                }
            }
            if (i < 4) {
                PakMenu_Run(i);
            }
        }
    }
    nuContPakCodeSet(D_800FEB38, D_800FEB3C);
    return 1;
}

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

    /* TODO (Clean version): [evaluated 2026-11-18: original behaviour, in the ROM]
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
    case KEY_UP:
        if (pad[ch].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case KEY_DOWN:
        if (pad[ch].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case KEY_LEFT:
        if (pad[ch].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case KEY_RIGHT:
        if (pad[ch].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case KEY_A:
        mask = 0x8000;
        break;
    case KEY_B:
        mask = 0x4000;
        break;
    case KEY_C_UP:
        mask = 8;
        break;
    case KEY_C_DOWN:
        mask = 4;
        break;
    case KEY_Z:
        mask = 0x2000;
        break;
    case KEY_R:
        mask = 0x10;
        break;
    case KEY_C_LEFT:
        mask = 2;
        break;
    case KEY_C_RIGHT:
        mask = 1;
        break;
    case KEY_START:
        mask = 0x1000;
        break;
    case KEY_A_OR_START:
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
    case KEY_UP:
        if (pad[0].stick_y > 0x28 || pad[1].stick_y > 0x28 || pad[2].stick_y > 0x28 || pad[3].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case KEY_DOWN:
        if (pad[0].stick_y < -0x28 || pad[1].stick_y < -0x28 || pad[2].stick_y < -0x28 || pad[3].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case KEY_LEFT:
        if (pad[0].stick_x < -0x28 || pad[1].stick_x < -0x28 || pad[2].stick_x < -0x28 || pad[3].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case KEY_RIGHT:
        if (pad[0].stick_x > 0x28 || pad[1].stick_x > 0x28 || pad[2].stick_x > 0x28 || pad[3].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case KEY_A:
        mask = 0x8000;
        break;
    case KEY_B:
        mask = 0x4000;
        break;
    case KEY_C_UP:
        mask = 8;
        break;
    case KEY_C_DOWN:
        mask = 4;
        break;
    case KEY_Z:
        mask = 0x2000;
        break;
    case KEY_R:
        mask = 0x10;
        break;
    case KEY_C_LEFT:
        mask = 2;
        break;
    case KEY_C_RIGHT:
        mask = 1;
        break;
    case KEY_START:
        mask = 0x1000;
        break;
    case KEY_A_OR_START:
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

u32 osGetCount(void);
void Input_Update(u8 pad) {
    Pad_ cont;
    u32 t;
    s32 i;
    u32 bind;
    s32 val;

    t = osGetCount();
    {
        f32 dt2 = (f64)(u32)(t - gRumbleState[pad].time);
        if (dt2 < 0.0f) {
            dt2 = 0.0f;
        }
    }
    nuContDataGet(&cont, pad);

    for (i = 0; i < 14; i++) {
        val = gRumbleState[pad].keys[i];
        bind = gRumbleState[pad].k[i];
        switch (i) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                switch (bind) {
                    case 0x80000:
                        if ((f64)cont.stick_y > 7.0) {
                            val = (s32)(((f64)cont.stick_y - 7.0) * 255.0 / 63.0);
                        } else {
                            val = 0;
                        }
                        break;
                    case 0x40000:
                        if ((f64)cont.stick_y < -7.0) {
                            val = (s32)(((f64)cont.stick_y + 7.0) * -255.0 / 63.0);
                        } else {
                            val = 0;
                        }
                        break;
                    case 0x20000:
                        if ((f64)cont.stick_x < -7.0) {
                            val = (s32)(((f64)cont.stick_x + 7.0) * -255.0 / 63.0);
                        } else {
                            val = 0;
                        }
                        break;
                    case 0x10000:
                        if ((f64)cont.stick_x > 7.0) {
                            val = (s32)(((f64)cont.stick_x - 7.0) * 255.0 / 63.0);
                        } else {
                            val = 0;
                        }
                        break;
                    case 0:
                        break;
                    default:
                        val = (bind & cont.button) ? 255 : 0;
                        break;
                }
                break;
            default:
                switch (bind) {
                    case 0x80000:
                        val = cont.stick_y > 40;
                        break;
                    case 0x40000:
                        val = cont.stick_y < -40;
                        break;
                    case 0x20000:
                        val = cont.stick_x < -40;
                        break;
                    case 0x10000:
                        val = cont.stick_x > 40;
                        break;
                    case 0:
                        break;
                    default:
                        if (bind & cont.button) {
                            val = 1;
                        } else {
                            val = 0;
                        }
                        break;
                }
                break;
        }
        if (val > 255) {
            val = 255;
        }
        if (val < 0) {
            val = 0;
        }
        gRumbleState[pad].keys[i] = val;
    }
    gRumbleState[pad].time = t;
}

u8 Input_GetControl(u8 a, s32 b) {
    return gPadControls[a].c[b];
}

s32 Input_GetKeyName(u8 ch, s32 slot) {
    s32 v;
    s32 i;
    s32 m;

    v = gPadBindings[ch].k[slot];
    for (i = 1, m = 1; m != v && i < 21; i++) {
        m <<= 1;
    }
    if (i < 21) {
        return D_800F0394[i];
    }
    return D_800F0394[0];
}

    /* TODO (Clean version): [evaluated 2026-11-18: the else-if chain changes the ROM]
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
            gPadBindings[ch].k[0] = mask;
            gPadBindings[ch].k[5] = mask;
        } else if (slot == 1 || slot == 4) {
            gPadBindings[ch].k[1] = mask;
            gPadBindings[ch].k[4] = mask;
        } else {
            gPadBindings[ch].k[slot] = mask;
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

u8 Input_GetLayout(s32 arg0) {
    return gPadBindings[arg0 & 0xFF].mode;
}

s32 Input_SetLayout(u8 pad, u8 mode) {
    if (mode >= gInputLayoutCount.layouts) {
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

    /* TODO (Clean version): [evaluated 2026-11-18: without it the stack frame shrinks; kept]
    Arreglo local "unused[16]" no utilizado, forzado artificialmente para ajustar el tamaño del frame de pila y lograr el match binario. En C limpio debe eliminarse.
    */
s32 Input_HasRumble(u8 i) {
    u8 unused[16];

    return gRumbleState[i].state == 2;
}

extern u16 D_80140F6A[];
void nuContRmbStart(s32, s32, s32);
/* Old-style definition: callers use the unprototyped sysInputRumble hook. Small strengths
   (< 0x40) are dropped; a new rumble only overrides a weaker one still running. */
s32 Input_Rumble(ch, v)
    u8 ch;
    u8 v;
{
    u16 t;

    if (gRumbleState[ch].state != 2) {
        return 0;
    }
    if (v < 0x40) {
        v = 0;
    }
    t = D_80140F6A[ch * 5];
    if (t == 0xFFFF || t == 0 || v >= D_800F0548[ch]) {
        D_800F0548[ch] = v;
        nuContRmbStart(ch, v, 20);
    }
    return 1;
}

s32 Input_Stub(void) {
    return 0;
}

s32 Input_IsPad1Connected(void) {
    return gContPattern & 1;
}

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
        gPadBindings[id].k[i] = 1 << buf[i];
    }
    return 1;
}

s32 Input_SaveBindings(u8 *out, u8 ch) {
    s32 j;
    s32 n;
    s32 m;
    u8 sum;

    sum = 0;
    for (j = 0; j < 13; j++) {
        n = 0;
        m = 1;
        while (m != gPadBindings[ch].k[j]) {
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

