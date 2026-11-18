#include "common.h"
#include "ultra64.h"
#include "nusys.h"
#include "gfx.h"
#include "include_asm.h"
typedef struct { s8 a; s8 b; s8 c; u8 pad[0xD]; } Ent6F_;
extern Ent6F_ gLightDirs[];
extern u8 gLightsDirty;
extern u8 gLightsEnabled;
void Gfx_UpdateLights(void);
typedef struct { s32 f0; s32 f4; s32 f8; s32 fC; u8 p10[0x78]; s32 f88; s32 f8C; } S_D_8014EB58;
extern S_D_8014EB58 gMtxPools[];
extern s32 gMtxHead;
typedef struct { u8 r; u8 g; u8 b; u8 pad3; u8 r2; u8 g2; u8 b2; u8 pad[9]; } L16_;
typedef struct { u8 p[0x54]; s16 *type; } Msg6F_;
typedef struct { f32 r; f32 g; f32 b; } ColF_;
extern ColF_ D_800F1E58[6];
extern u8 gColorShift;
void Screen_SetAmbient(u8 arg0, u8 arg1, u8 arg2);
void Screen_SetLightDir(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
void Screen_SetLightColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
void Screen_ApplyLights();
extern f32 D_800F2110;
extern void (*D_800F1CB4)(Msg6F_ *);
extern void (*D_800F1CB8)(Msg6F_ *);
extern s32 nuGfxDisplay;
void Gfx_EventThread(void *arg);
void EnvMap_LoadStatic(void *);
extern u8 D_800FF7B0;
extern OSViMode D_800FB6F0[];
void nuGfxRetraceWait(s32);
extern u8 gLowRes;
void Screen_SetViewport(s32, s32, s32, s32);
void Gfx_ClearFrame(s32, s32, s32, s32, s32);
void EnvMap_CreateDynamic(void);
void World_SetupLod(s32, s32);
void nuGfxDisplayOn(void);
s32 GfxPools_Shutdown();
extern NUScTask D_8014C1D0[];
extern s16 D_8010A226;
extern u8 D_8010A228[];
extern u8 D_8010C3E0[];
extern u8 D_8014EC80[];
extern u8 D_800E7AA0[];
extern u8 D_800E7B70[];
extern u8 D_8014CD80[];
extern u8 D_80138E40[];
extern u8 D_80140E40[];
extern u8 D_80143850[];
void nuGfxDisplayOff(void);
void Gfx_StartEventThread(void);
extern L16_ gLightColors[];
void MtxPool_Grow(void *, u32);
extern NUScTask *D_8010A220;
extern s32 *D_80141690;
extern u16 D_800F1CBC;
extern s32 D_8014C648;
extern volatile s32 nuGfxTaskSpool;
extern s32 gViModeIndex;
extern s32 D_801380A8;
extern u8 D_800FB9C0[];
extern u16 gScreenWidth;
extern u16 gScreenHeight;
extern u8 D_800F1CC0;
extern s32 D_800F1CC8;
extern s16 D_8010A224;
extern u8 D_8014D33C;
extern u8 D_F1CF8;
void nuGfxThreadStart(void);
void nuGfxSetCfb(void *, s32);
void nuGfxSwapCfbFuncSet(void *);
void nuGfxSwapCfb(void);
extern u8 gNumLights;
extern f32 D_800F2114;
extern f32 D_800F2118;
extern f32 D_800F211C;
extern f32 D_800F2120;
extern f32 D_800F2124;
extern f32 D_800F2128;
extern f32 D_800F212C;
extern f32 D_800F2130;
extern f32 D_800F2134;

s32 nuGfxTaskAllEndWait();

void Gfx_EventThread(void *arg) {
    Msg6F_ *msg;
    u32 mask;

    osCreateMesgQueue(D_8014EC80, D_8010C3E0, 8);
    while (1) {
        osRecvMesg(D_8014EC80, &msg, 1);
        switch (*msg->type) {
        case 4:
            if (D_800F1CB4 != NULL) {
                D_800F1CB4(msg);
            }
            if (nuGfxDisplay & 0x80) {
                osViBlack(0);
                nuGfxDisplay = 1;
            }
            break;
        case 8:
            if (D_800F1CB8 != NULL) {
                D_800F1CB8(msg);
            }
            break;
        default:
            continue;
        }
        mask = osSetIntMask(1);
        nuGfxTaskSpool--;
        osSetIntMask(mask);
    }
}

void Gfx_StartEventThread(void) {
    u32 i;

    D_8010A224 = 8;
    D_8010A226 = 4;
    nuGfxTaskSpool = 0;
    nuGfxDisplayOff();
    osCreateThread(D_8010A228, 5, Gfx_EventThread, 0, D_8010C3E0, 0x3C);
    osStartThread(D_8010A228);
    for (i = 0; i < 10; i++) {
        D_8014C1D0[i].next = &D_8014C1D0[i + 1];
        D_8014C1D0[i].msgQ = (OSMesgQueue *)D_8014EC80;
        D_8014C1D0[i].list.t.type = M_GFXTASK;
        D_8014C1D0[i].list.t.flags = 0;
        D_8014C1D0[i].list.t.ucode_boot = (u64 *)D_800E7AA0;
        D_8014C1D0[i].list.t.ucode_boot_size = D_800E7B70 - D_800E7AA0;
        D_8014C1D0[i].list.t.ucode_size = SP_UCODE_SIZE;
        D_8014C1D0[i].list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
        D_8014C1D0[i].list.t.dram_stack = (u64 *)D_8014CD80;
        D_8014C1D0[i].list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
        D_8014C1D0[i].list.t.output_buff = (u64 *)D_80138E40;
        D_8014C1D0[i].list.t.output_buff_size = (u64 *)D_80140E40; /* end of the buffer */
        D_8014C1D0[i].list.t.yield_data_ptr = (u64 *)D_80143850;
        D_8014C1D0[i].list.t.yield_data_size = OS_YIELD_DATA_SIZE;
        D_8014C1D0[i].msgQ = (OSMesgQueue *)D_8014EC80;
    }
    D_8014C1D0[9].next = &D_8014C1D0[0];
    D_8010A220 = &D_8014C1D0[0];
}

extern u32 D_80138D30;
extern u32 D_800F1CB0;
extern s32 *D_8014C6C8;

/* Queues an RSP graphics task for dl (ucode pair ucode); flag bit 0 also flips to the next frame buffer. */
void Gfx_SendTask(Gfx *dl, s32 size, s32 ucode, u32 flags) {
    NUScTask *t;
    u16 f;
    u32 mask;
    u32 next;

    t = D_8010A220;
    t->list.t.data_ptr = (u64 *)dl;
    t->list.t.data_size = size;
    t->list.t.flags = flags >> 16;
    t->list.t.ucode = (u64 *)D_80141690[ucode * 2];
    t->list.t.ucode_data = (u64 *)D_80141690[ucode * 2 + 1];
    t->flags = flags & 0xFFFF;
    t->framebuffer = (void *)D_8014C648;
    f = D_800F1CBC;
    if (f & 4) {
        D_800F1CBC = f ^ 4;
        t->list.t.flags |= OS_TASK_DP_WAIT;
    }
    D_800F1CBC = flags;
    if (flags & 1) {
        next = (D_80138D30 + 1) % D_800F1CB0;
        D_8010A220->msg = &D_8010A226;
        D_80138D30 = next;
        D_8014C648 = D_8014C6C8[next];
    } else {
        D_8010A220->msg = &D_8010A224;
    }
    mask = osSetIntMask(1);
    nuGfxTaskSpool++;
    osSetIntMask(mask);
    osWritebackDCacheAll();
    osSendMesg(&D_8014D33C, D_8010A220, 1);
    D_8010A220 = D_8010A220->next;
}

s32 Gfx_Init(void) {
    Gfx dl[0x101];
    NUScTask *t;
    u16 f;
    u32 mask;

    osViBlack(1);
    osViSetMode(&D_800FB9C0[gViModeIndex * 0x50]);
    osViSetSpecialFeatures(0x5A);
    osViBlack(0);
    gScreenWidth = 640;
    gScreenHeight = 480;
    nuGfxThreadStart();
    nuGfxSetCfb(&D_800F1CC0, 2);
    D_801380A8 = 0x8023E000;
    nuGfxSwapCfbFuncSet(nuGfxSwapCfb);
    D_80141690 = &D_800F1CC8;
    Gfx_StartEventThread();
    t = D_8010A220;
    f = D_800F1CBC;
    gSPDisplayList(&dl[0], &D_F1CF8);
    gDPFullSync(&dl[1]);
    gSPEndDisplayList(&dl[2]);
    t->list.t.data_ptr = (u64 *)dl;
    t->list.t.data_size = 0x18;
    t->list.t.flags = 0;
    t->list.t.ucode = D_80141690[0];
    t->list.t.ucode_data = D_80141690[1];
    t->flags = 0;
    t->framebuffer = (void *)D_8014C648;
    if (f & 4) {
        D_800F1CBC = f ^ 4;
        t->list.t.flags |= OS_TASK_DP_WAIT;
    }
    D_800F1CBC = 0;
    D_8010A220->msg = &D_8010A224;
    mask = osSetIntMask(1);
    nuGfxTaskSpool++;
    osSetIntMask(mask);
    osWritebackDCacheAll();
    osSendMesg(&D_8014D33C, D_8010A220, 1);
    D_8010A220 = D_8010A220->next;
    nuGfxTaskAllEndWait();
    return 1;
}

void Light_SetCount(s32 a) {
    u8 *p;

    if (a == 0) {
        a = 1;
    }
    p = &gNumLights;
    if ((*p != a) | (gLightsDirty == 1)) {
        *p = a;
        gLightsDirty = 1;
        if (gLightsEnabled == 1) {
            Gfx_UpdateLights();
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8006FE2C);

void Light_SetDir(s32 i, u8 a, u8 b, u8 c) {
    if (gLightDirs[i].a != a) {
        gLightDirs[i].a = a;
        gLightsDirty = 1;
    }
    if (gLightDirs[i].b != b) {
        gLightDirs[i].b = b;
        gLightsDirty = 1;
    }
    if (gLightDirs[i].c != c) {
        gLightDirs[i].c = c;
        gLightsDirty = 1;
    }
}


void Gfx_SetLightColor(s32 arg0);
void Light_SetColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((gLightsEnabled == 1) & (gLightsDirty == 0)) {
        gLightColors[arg0].r = arg1;
        gLightColors[arg0].g = arg2;
        gLightColors[arg0].b = arg3;
        gLightColors[arg0].r2 = arg1;
        gLightColors[arg0].g2 = arg2;
        gLightColors[arg0].b2 = arg3;
        Gfx_SetLightColor(arg0);
    }
    if (gLightColors[arg0].r != (u8) arg1) {
        gLightColors[arg0].r = arg1;
        gLightsDirty = 1;
    }
    if (gLightColors[arg0].g != (u8) arg2) {
        gLightColors[arg0].g = arg2;
        gLightsDirty = 1;
    }
    if (gLightColors[arg0].b != (u8) arg3) {
        gLightColors[arg0].b = arg3;
        gLightsDirty = 1;
    }
    if (gLightColors[arg0].r2 != (u8) arg1) {
        gLightColors[arg0].r2 = arg1;
        gLightsDirty = 1;
    }
    if (gLightColors[arg0].g2 != (u8) arg2) {
        gLightColors[arg0].g2 = arg2;
        gLightsDirty = 1;
    }
    if (gLightColors[arg0].b2 != (u8) arg3) {
        gLightColors[arg0].b2 = arg3;
        gLightsDirty = 1;
    }
}

/* Resets the six color slots from D_800F1E58 (level 100 >> gColorShift) after setting 20/20/20. */
void Light_Reset(void) {
    s32 i;

    Screen_ApplyLights(6);
    Screen_SetAmbient(0x14, 0x14, 0x14);
    (*(s8 *)&gLightsDirty) = 1;
    for (i = 0; i < 6; i++) {
        Screen_SetLightDir(i, D_800F1E58[i].r, D_800F1E58[i].g, D_800F1E58[i].b);
        Screen_SetLightColor(i, 100 >> gColorShift, 100 >> gColorShift, 100 >> gColorShift);
    }
}

void Gfx_SetAllLightColors(s32 a, s32 b, s32 c) {
    s32 color;

    color = (a << 24) + ((b & 0xFF) << 16) + ((c & 0xFF) << 8);
    GFX_RESERVE(12);
    gSPLightColor(gDisplayListHead++, LIGHT_1, color);
    gSPLightColor(gDisplayListHead++, LIGHT_2, color);
    gSPLightColor(gDisplayListHead++, LIGHT_3, color);
    gSPLightColor(gDisplayListHead++, LIGHT_4, color);
    gSPLightColor(gDisplayListHead++, LIGHT_5, color);
    gSPLightColor(gDisplayListHead++, LIGHT_6, color);
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", Gfx_UpdateLights);

void Gfx_SetNumLights(void) {
    u8 n = gNumLights;

    GFX_RESERVE(1);
    gSPNumLights(gDisplayListHead++, n);
}

void Gfx_SetLightColor(s32 arg0) {
    GFX_RESERVE(2);
    gMoveWd(gDisplayListHead++, G_MW_LIGHTCOL, arg0,
            (gLightColors[arg0].r << 24) + (gLightColors[arg0].g << 16) + (gLightColors[arg0].b << 8));
    gMoveWd(gDisplayListHead++, G_MW_LIGHTCOL, arg0 + 4,
            (gLightColors[arg0].r << 24) + (gLightColors[arg0].g << 16) + (gLightColors[arg0].b << 8));
}

void Mtx_New(s32 arg0);
void Gfx_SetLookAt(void) {
    s32 m;
    s32 l;

    m = ((s32 (*)(void)) Mtx_New)();
    if ((u32) (gMtxHead + 0x40) >= (u32) gMtxPools[gGfxPoolIndex].f4) {
        gMtxPools[gGfxPoolIndex].f0 = gMtxHead;
        MtxPool_Grow(&gMtxPools[gGfxPoolIndex], 1U);
        gMtxHead = gMtxPools[gGfxPoolIndex].f0;
    }
    GFX_RESERVE(1);
    guLookAtReflect((void *) m, (void *) gMtxHead, 50.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    l = gMtxHead;
    gSPLookAt(gDisplayListHead++, l);
    gMtxHead = l + 0x40;
}

s32 *Mtx_FromF34(f32 src[3][4], s32 *dst);
void Mtx_New(s32 src) {
    s32 mtx;

    if ((u32)(gMtxHead + 0x40) >= (u32)gMtxPools[gGfxPoolIndex].f4) {
        gMtxPools[gGfxPoolIndex].f0 = gMtxHead;
        MtxPool_Grow(&gMtxPools[gGfxPoolIndex], 1);
        gMtxHead = gMtxPools[gGfxPoolIndex].f0;
    }
    mtx = gMtxHead;
    gMtxHead = mtx + 0x40;
    Mtx_FromF34((void *)src, (void *)mtx);
}

/* Converts a 3x4 float transform (rows = axes, translation in column 3) to a fixed-point Mtx, transposed,
 * scaled by 1/4 and with the translation clamped to the s16 range. */
s32 *Mtx_FromF34(f32 src[3][4], s32 *dst) {
    f32 mf[4][4];
    s32 i;
    s32 j;
    s32 e1;
    s32 e2;
    s32 *ai;
    s32 *af;

    mf[0][0] = src[0][0] * 0.25f;
    mf[0][1] = src[1][0] * 0.25f;
    mf[0][2] = src[2][0] * 0.25f;
    mf[1][0] = src[0][1] * 0.25f;
    mf[1][1] = src[1][1] * 0.25f;
    mf[1][2] = src[2][1] * 0.25f;
    mf[2][0] = src[0][2] * 0.25f;
    mf[2][1] = src[1][2] * 0.25f;
    mf[2][2] = src[2][2] * 0.25f;
    mf[3][0] = src[0][3] * 0.25f;
    mf[3][1] = src[1][3] * 0.25f;
    mf[3][2] = src[2][3] * 0.25f;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
    mf[3][0] = (mf[3][0] < -32767.0f) ? -32767.0f : mf[3][0];
    mf[3][1] = (mf[3][1] < -32767.0f) ? -32767.0f : mf[3][1];
    mf[3][2] = (mf[3][2] < -32767.0f) ? -32767.0f : mf[3][2];
    mf[3][0] = (mf[3][0] > 32767.0f) ? 32767.0f : mf[3][0];
    mf[3][1] = (mf[3][1] > 32767.0f) ? 32767.0f : mf[3][1];
    mf[3][2] = (mf[3][2] > 32767.0f) ? 32767.0f : mf[3][2];
    ai = dst;
    af = dst + 8;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            e1 = mf[i][j * 2] * 65536.0f;
            e2 = mf[i][j * 2 + 1] * 65536.0f;
            *(ai++) = (e1 & 0xFFFF0000) | ((u32)e2 >> 16);
            *(af++) = ((e1 << 16) & 0xFFFF0000) | (e2 & 0xFFFF);
        }
    }
    return dst;
}

void Camera_CalcFrustum(void) {
    register f32 vh;
    register f32 hh;
    register f32 t;
    register f32 half;

    half = D_800F2120 * 0.5f;
    D_800F2128 = 1.0f / cosf(half);
    D_800F212C = -(sinf(half) / cosf(half));
    vh = half / D_800F2124;
    hh = sinf(vh) / cosf(vh);
    t = D_800F211C * D_800F2118;
    D_800F2130 = -hh;
    D_800F2114 = t;
    D_800F2134 = D_800F212C * t;
}

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80070F38);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_8007126C);

INCLUDE_ASM("asm/nonmatchings/unit_103B0", func_80071530);

/* Sphere-in-frustum test: o's translation transformed by view matrix m, radius r. */
u8 Frustum_TestSphere(f32 (*m)[4], f32 (*o)[4], f32 r) {
    f32 vz;
    f32 depth;
    f32 rs;
    f32 v;
    f32 lim;

    vz = m[2][0] * o[0][3] + m[2][1] * o[1][3] + m[2][2] * o[2][3] + m[2][3];
    depth = -vz;
    if (D_800F2114 < depth - r) {
        return 0;
    }
    if (depth + r < D_800F2110) {
        return 0;
    }
    rs = r * D_800F2128;
    v = m[0][0] * o[0][3] + m[0][1] * o[1][3] + m[0][2] * o[2][3] + m[0][3];
    lim = vz * D_800F212C;
    if (v + rs < -lim) {
        return 0;
    }
    if (lim < v - rs) {
        return 0;
    }
    v = m[1][0] * o[0][3] + m[1][1] * o[1][3] + m[1][2] * o[2][3] + m[1][3];
    lim = vz * D_800F2130;
    if (v + rs < -lim) {
        return 0;
    }
    if (lim < v - rs) {
        return 0;
    }
    return 1;
}

void Gfx_WaitTasks(void) {
    nuGfxTaskAllEndWait();
}

s32 Video_SetLowRes(void) {
    osViSetMode(&D_800FB6F0[gViModeIndex]);
    nuGfxDisplayOff();
    nuGfxRetraceWait(2);
    osViSetSpecialFeatures(0x5A);
    ((u32 *)&D_800F1CC0)[0] = 0x803B5000;
    ((u32 *)&D_800F1CC0)[1] = 0x803DA800;
    gLowRes = 1;
    nuGfxSetCfb(&D_800F1CC0, 2);
    gScreenWidth = 320;
    gScreenHeight = 240;
    Screen_SetViewport(0, 0, 320, 240);
    Gfx_ClearFrame(0, 0, 0, gScreenWidth, gScreenHeight);
    EnvMap_CreateDynamic();
    World_SetupLod(1, 1);
    nuGfxDisplayOn();
    GfxPools_Shutdown();
    return 1;
}

s32 Video_SetHighRes(void) {
    u16 *p;

    osViSetMode(&((OSViMode *)&D_800FB9C0)[gViModeIndex]);
    nuGfxDisplayOff();
    nuGfxRetraceWait(2);
    osViSetSpecialFeatures(0x5A);
    ((u32 *)&D_800F1CC0)[0] = 0x802D4000;
    ((u32 *)&D_800F1CC0)[1] = 0x8036A000;
    nuGfxSetCfb(&D_800F1CC0, 2);
    p = &gScreenWidth;
    *p = 640;
    gScreenHeight = 480;
    Screen_SetViewport(0, 0, 640, 480);
    Gfx_ClearFrame(0, 0, 0, *p, gScreenHeight);
    gLowRes = 0;
    World_SetupLod(0, 0);
    EnvMap_LoadStatic(&D_800FF7B0);
    GfxPools_Shutdown();
    nuGfxDisplayOn();
    return 1;
}
