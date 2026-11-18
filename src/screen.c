#include "common.h"
#include "gfx.h"
#include "math.h"
#include "include_asm.h"

#define SCREEN_BG_BUFFER_SIZE 0x4B200


extern void *gBgBuffer;
extern u8 gLightsEnabled;
extern u8 gLowRes;
extern u8 gColorShift;
extern u8 gLightsDirty;
extern u16 gScreenWidth;
extern u16 gScreenHeight;
extern u8 D_800F20FE;
extern u8 D_800F20FF;
extern u8 D_800F2100;
extern u8 D_800F2102;
extern void *gBgImage;
extern s16 D_800F2108;
extern s16 D_800F210A;
extern s16 D_800F210C;
extern s16 D_800F210E;
extern f32 D_800F2110;
extern f32 D_800F2114;
extern f32 D_800F2118;
extern f32 D_800F211C;
extern f32 D_800F2120;
extern f32 D_800F2124;
extern s32 D_800F2168;
extern s32 D_800F216C[];
extern s32 D_800F2170;
extern s32 D_800F2174;
extern s32 D_800F2178;
extern s32 D_800F217C;
extern s32 D_800F2180;
extern s32 D_800F2184;

void *Memory_Alloc(u32 size);
void Memory_Free(void *ptr);
s32 File_Load(char *name, void *dst);
char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
void Gfx_Init(void);
void Light_SetCount(void);
void func_8006FE2C(u8 arg0, u8 arg1, u8 arg2);
void Light_SetDir(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
void Light_SetColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
void Light_Reset(void);
void Camera_CalcFrustum(void);
void func_80070F38(void);
void func_80071530(void);
void Video_SetHighRes(void);
void Gfx_ClearFrame(u8 arg0, u8 arg1, u8 arg2, u16 arg3, u16 arg4);
void Bg_SetPalette(void *arg0);
void Bg_FadePalette(f32 arg0, void *arg1);
void Bg_DrawImage(void *arg0, s32 arg1, s32 arg2);
void SortQueue_Init(void);
void SortQueue_Shutdown(void);
u8 Screen_BeginFrame(s32 arg0);
void Camera_SetProjection(f32 arg0, f32 arg1, f32 arg2, f32 arg3);

s32 Screen_InitGfx(void) {
    Gfx_Init();
    return 1;
}

s32 Screen_Init(void) {
    gLightsDirty = 1;
    gLowRes = 1;
    D_800F2102 = 0xFF;
    D_800F210C = 320;
    gLightsEnabled = 0;
    gColorShift = 0;
    D_800F20FE = 0;
    D_800F20FF = 0;
    D_800F2100 = 0;
    gBgImage = NULL;
    D_800F2108 = 0;
    D_800F210A = 0;
    D_800F210E = 240;
    D_800F2168 = 0;
    D_800F216C[0] = 0;
    D_800F2170 = 0;
    D_800F2110 = 1.0f;
    D_800F211C = 1000.0f;
    D_800F2118 = 1.0f;
    D_800F2114 = 1e7f;
    D_800F2120 = 1.09f;
    D_800F2124 = 1.33333302f;
    Camera_CalcFrustum();
    Light_Reset();
    SortQueue_Init();
    D_800F2174 = 1;
    D_800F2178 = 1;
    D_800F217C = 0;
    D_800F2180 = 0;
    D_800F2184 = 0;
    Video_SetHighRes();
    return 1;
}

s32 Screen_Shutdown(void) {
    SortQueue_Shutdown();
    return 1;
}

void Screen_SetClearColor(u8 arg0, u8 arg1, u8 arg2) {
    D_800F20FE = arg0;
    D_800F20FF = arg1;
    D_800F2100 = arg2;
}

    /* TODO (Clean version):
    The alpha scaling calculation `alpha / 255.0` uses a double-precision float literal by default. 
    It should be explicitly typed as a single-precision float (`alpha / 255.0f`) to avoid 
    unnecessary type-promotion overhead on MIPS hardware.
    */
void Screen_SetBgAlpha(u8 alpha) {
    u8 *current = &D_800F2102;

    if (alpha != *current && gBgImage != NULL) {
        *current = alpha;
        Bg_FadePalette(alpha / 255.0, gBgImage);
    }
}

    /* TODO (Clean version):
    Allocating a large fixed-size background buffer (`SCREEN_BG_BUFFER_SIZE` / ~300 KB) 
    dynamically on demand can lead to heap fragmentation if done repeatedly. 
    It should ideally be allocated once during system startup or managed via a dedicated 
    static texture/framebuffer pool.
    */
s32 Screen_AllocBg(void) {
    if (gBgBuffer == NULL) {
        gBgBuffer = Memory_Alloc(SCREEN_BG_BUFFER_SIZE);
    }
    return gBgBuffer != NULL;
}

s32 Screen_FreeBg(void) {
    if (gBgBuffer != NULL) {
        Memory_Free(gBgBuffer);
        gBgBuffer = NULL;
        gBgImage = NULL;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/screen", Screen_LoadBg);

s32 Screen_BeginFrameDefault(void) {
    return Screen_BeginFrame(0);
}

u8 Screen_BeginFrame(s32 arg0) {
    Gfx_ClearFrame(D_800F20FE, D_800F20FF, D_800F2100, gScreenWidth, gScreenHeight);
    if (gBgImage != NULL) {
        Bg_DrawImage(gBgImage, 640, 480);
    }
    func_80071530();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/screen", Screen_EndFrame);

void SortQueue_Flush(void);
void SortQueue_Swap(void);
void GfxPools_Swap(s32);
s32 Screen_FlushFrame(void) {
    SortQueue_Flush();
    SortQueue_Swap();

    /* TODO (Clean version):
    Manually injecting raw display list commands and bit-shifting macros (`0xE9`, `0xDF`) 
    inline should be wrapped in clean SDK macro wrappers like `gDPNoOp` or `gSPEndDisplayList()` 
    for readability and platform maintenance.
    */
    GFX_RESERVE(2);
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
    GfxPools_Swap(0);
    gLightsDirty = 1;
    gLightsEnabled = 0;
    D_800F2168 = 0;
    D_800F216C[0] = 0;
    D_800F2170 = 0;
    D_800F217C = 0;
    D_800F2180 = 0;
    return 1;
}

u16 Screen_GetWidth(void) {
    return gScreenWidth;
}

u16 Screen_GetHeight(void) {
    return gScreenHeight;
}

void Screen_SetViewport(s16 x, s16 y, s16 width, s16 height) {
    D_800F2108 = x;
    D_800F210C = width;
    D_800F210E = height;
    D_800F210A = (gScreenHeight - y) - height;
    func_80070F38();
}

extern Mtx34f D_800F2138;
s32 Mtx_New(Mtx34f *);
    /* TODO (Clean version):
    Passing model-view matrices through custom transformation pipelines (`Mtx_New`) 
    and manually writing low-level display list commands with hardware offsets (`0xDA380003`) 
    should be encapsulated into a clean matrix stack helper (e.g., `Matrix_LoadModelView(...)`).
    */
void Camera_Apply(Mtx34f *m) {
    Mtx34f t;
    D_800F2138 = *m;
    t = *m;
    D_800F216C[0] = Mtx_New(&t);
    GFX_RESERVE(1);
    gSPMatrix(gDisplayListHead++, D_800F216C[0] - 0x80000000, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

    /* TODO (Clean version):
    Magic aspect ratio or viewport scaling multipliers (`1.04`) 
    should be defined using descriptive configuration constants:
    #define VIEWPORT_SCALE_X_FACTOR 1.04f
    */
void Camera_SetPerspective(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    Camera_SetProjection(arg0, arg1, arg2 * 1.04, arg3);
}

    /* TODO (Clean version):
    Magic aspect ratio or viewport scaling multipliers (`1.2`, `1.15`) 
    should be defined using descriptive configuration constants:
    #define VIEWPORT_SCALE_Y_FACTOR 1.2f
    */
void Camera_SetPerspectiveRace(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    Camera_SetProjection(arg0, arg1 * 1.2, arg2 * 1.15, arg3);
}

void Camera_SetProjection(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 changedA;
    s32 changedB;

    D_800F2110 = arg0;
    D_800F2124 = arg3;
    changedA = D_800F2120 != arg2;
    changedB = D_800F211C != arg1;
    if (changedA | changedB) {
        D_800F211C = arg1;
        D_800F2120 = arg2;
        Camera_CalcFrustum();
    }
}

void Screen_SetAmbient(u8 arg0, u8 arg1, u8 arg2) {
    func_8006FE2C(arg0, arg1, arg2);
}

void Screen_SetLightDir(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Light_SetDir(arg0, arg1, arg2, arg3);
}

void Screen_SetLightColor(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Light_SetColor(arg0, arg1, arg2, arg3);
}

void Screen_ApplyLights(void) {
    Light_SetCount();
}

void Screen_SetColorShift(u8 arg0) {
    if (arg0 == 1) {
        gColorShift = 2;
    } else {
        gColorShift = 0;
    }
}
