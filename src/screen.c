#include "common.h"
#include "gfx.h"
#include "math.h"
#include "include_asm.h"

#define SCREEN_BG_BUFFER_SIZE 0x4B200


extern void *D_800F20F0;
extern u8 gLightsEnabled;
extern u8 gLowRes;
extern u8 D_800F20F6;
extern u8 gLightsDirty;
extern u16 gScreenWidth;
extern u16 gScreenHeight;
extern u8 D_800F20FE;
extern u8 D_800F20FF;
extern u8 D_800F2100;
extern u8 D_800F2102;
extern void *D_800F2104;
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
void func_800761D4(u8 arg0, u8 arg1, u8 arg2, u16 arg3, u16 arg4);
void func_80076470(void *arg0);
void func_80076578(f32 arg0, void *arg1);
void func_800766FC(void *arg0, s32 arg1, s32 arg2);
void func_800802C0(void);
void func_80080360(void);
u8 func_80078320(s32 arg0);
void func_80078878(f32 arg0, f32 arg1, f32 arg2, f32 arg3);

s32 func_80077FB0(void) {
    Gfx_Init();
    return 1;
}

s32 func_80077FD0(void) {
    gLightsDirty = 1;
    gLowRes = 1;
    D_800F2102 = 0xFF;
    D_800F210C = 320;
    gLightsEnabled = 0;
    D_800F20F6 = 0;
    D_800F20FE = 0;
    D_800F20FF = 0;
    D_800F2100 = 0;
    D_800F2104 = NULL;
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
    func_800802C0();
    D_800F2174 = 1;
    D_800F2178 = 1;
    D_800F217C = 0;
    D_800F2180 = 0;
    D_800F2184 = 0;
    Video_SetHighRes();
    return 1;
}

s32 func_80078120(void) {
    func_80080360();
    return 1;
}

void func_80078140(u8 arg0, u8 arg1, u8 arg2) {
    D_800F20FE = arg0;
    D_800F20FF = arg1;
    D_800F2100 = arg2;
}

    /* TODO (Clean version):
    The alpha scaling calculation `alpha / 255.0` uses a double-precision float literal by default. 
    It should be explicitly typed as a single-precision float (`alpha / 255.0f`) to avoid 
    unnecessary type-promotion overhead on MIPS hardware.
    */
void func_8007815C(u8 alpha) {
    u8 *current = &D_800F2102;

    if (alpha != *current && D_800F2104 != NULL) {
        *current = alpha;
        func_80076578(alpha / 255.0, D_800F2104);
    }
}

    /* TODO (Clean version):
    Allocating a large fixed-size background buffer (`SCREEN_BG_BUFFER_SIZE` / ~300 KB) 
    dynamically on demand can lead to heap fragmentation if done repeatedly. 
    It should ideally be allocated once during system startup or managed via a dedicated 
    static texture/framebuffer pool.
    */
s32 func_800781BC(void) {
    if (D_800F20F0 == NULL) {
        D_800F20F0 = Memory_Alloc(SCREEN_BG_BUFFER_SIZE);
    }
    return D_800F20F0 != NULL;
}

s32 func_800781FC(void) {
    if (D_800F20F0 != NULL) {
        Memory_Free(D_800F20F0);
        D_800F20F0 = NULL;
        D_800F2104 = NULL;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/screen", func_80078238);

s32 func_80078300(void) {
    return func_80078320(0);
}

u8 func_80078320(s32 arg0) {
    func_800761D4(D_800F20FE, D_800F20FF, D_800F2100, gScreenWidth, gScreenHeight);
    if (D_800F2104 != NULL) {
        func_800766FC(D_800F2104, 640, 480);
    }
    func_80071530();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/screen", func_80078388);

void func_800804B4(void);
void func_80080500(void);
void GfxPools_Swap(s32);
s32 func_800784F0(void) {
    func_800804B4();
    func_80080500();

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

u16 func_80078624(void) {
    return gScreenWidth;
}

u16 func_80078630(void) {
    return gScreenHeight;
}

void func_8007863C(s16 x, s16 y, s16 width, s16 height) {
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
void func_80078684(Mtx34f *m) {
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
void func_800787EC(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    func_80078878(arg0, arg1, arg2 * 1.04, arg3);
}

    /* TODO (Clean version):
    Magic aspect ratio or viewport scaling multipliers (`1.2`, `1.15`) 
    should be defined using descriptive configuration constants:
    #define VIEWPORT_SCALE_Y_FACTOR 1.2f
    */
void func_80078828(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    func_80078878(arg0, arg1 * 1.2, arg2 * 1.15, arg3);
}

void func_80078878(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
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

void func_800788F8(u8 arg0, u8 arg1, u8 arg2) {
    func_8006FE2C(arg0, arg1, arg2);
}

void func_8007891C(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Light_SetDir(arg0, arg1, arg2, arg3);
}

void func_80078940(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Light_SetColor(arg0, arg1, arg2, arg3);
}

void func_80078964(void) {
    Light_SetCount();
}

void func_80078980(u8 arg0) {
    if (arg0 == 1) {
        D_800F20F6 = 2;
    } else {
        D_800F20F6 = 0;
    }
}
