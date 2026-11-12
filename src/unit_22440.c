#include "common.h"
#include "include_asm.h"
typedef struct { f32 x; f32 y; f32 z; } Vec3f_;
typedef struct { u8 id; u8 p1[3]; u8 *car; u8 p8[0x1C]; f32 f24; u8 p28[0x1F0]; f32 f218; u8 p21C[0xC]; Vec3f_ v228; u8 p234[0xC]; Vec3f_ v240; u8 p24C[0x5C]; } Slot85_;
extern Slot85_ D_80114830[];
extern s32 D_80115824;
extern f32 D_80115820;
u8 *func_800960D8(u8);
void func_80084B9C(u8 *);
extern u8 D_80114838;

s32 func_80081840();

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80081840);

void func_80084B80(void) {
    func_80081840();
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80084B9C);

void func_80084FDC(void) {
    u32 i;
    u8 *c1;
    u8 *car;
    Vec3f_ t;
    s32 n;

    for (i = 0; i < 6; i++) {
        c1 = func_800960D8(i);
        if (*(s32 *)(c1 + 0x6D8) != 1) {
            D_80114830[i].f24 = *(u32 *)(c1 + 0x474) / 1000.0f;
        }
        n = D_80115824 - 1;
        D_80114830[i].f218 = (u32)(n - c1[0x478]) * D_80115820 + D_80114830[i].f24;
    }
    for (i = 0; i < 6; i++) {
        car = func_800960D8(D_80114830[i].id);
        if (*(s32 *)(car + 0x6D8) != 1 || car[0x4B8] != 0 || car[0x47B] != 0) {
            continue;
        }
        if ((*(f32 *)(car + 0xA4)) < 0.0f || 0.0f < (*(f32 *)(car + 0xA4)) || (*(f32 *)(car + 0xA8)) < 0.0f || 0.0f < (*(f32 *)(car + 0xA8)) || (*(f32 *)(car + 0xAC)) < 0.0f || 0.0f < (*(f32 *)(car + 0xAC))) {
            t.x = (*(f32 *)(car + 0x748)) * ((*(f32 *)(car + 0x748)) * (*(f32 *)(car + 0xA4)) + (*(f32 *)(car + 0x74C)) * (*(f32 *)(car + 0xA8)) + (*(f32 *)(car + 0x750)) * (*(f32 *)(car + 0xAC)));
            t.y = (*(f32 *)(car + 0x74C)) * ((*(f32 *)(car + 0x748)) * (*(f32 *)(car + 0xA4)) + (*(f32 *)(car + 0x74C)) * (*(f32 *)(car + 0xA8)) + (*(f32 *)(car + 0x750)) * (*(f32 *)(car + 0xAC)));
            t.z = (*(f32 *)(car + 0x750)) * ((*(f32 *)(car + 0x748)) * (*(f32 *)(car + 0xA4)) + (*(f32 *)(car + 0x74C)) * (*(f32 *)(car + 0xA8)) + (*(f32 *)(car + 0x750)) * (*(f32 *)(car + 0xAC)));
            (*(f32 *)(car + 0xA4)) -= t.x;
            (*(f32 *)(car + 0xA8)) -= t.y;
            (*(f32 *)(car + 0xAC)) -= t.z;
            D_80114830[i].v228.x = (*(f32 *)(car + 0xA4)) + D_80114830[i].v228.x;
            D_80114830[i].v228.y = (*(f32 *)(car + 0xA8)) + D_80114830[i].v228.y;
            D_80114830[i].v228.z = (*(f32 *)(car + 0xAC)) + D_80114830[i].v228.z;
            func_80084B9C(car);
            (*(f32 *)(car + 0xA4)) = 0.0f;
            (*(f32 *)(car + 0xA8)) = 0.0f;
            (*(f32 *)(car + 0xAC)) = 0.0f;
        }
        if ((*(f32 *)(car + 0xE4)) < 0.0f || 0.0f < (*(f32 *)(car + 0xE4)) || (*(f32 *)(car + 0xE8)) < 0.0f || 0.0f < (*(f32 *)(car + 0xE8)) || (*(f32 *)(car + 0xEC)) < 0.0f || 0.0f < (*(f32 *)(car + 0xEC))) {
            D_80114830[i].v240.x = (*(f32 *)(car + 0xE4)) + D_80114830[i].v240.x;
            D_80114830[i].v240.y = (*(f32 *)(car + 0xE8)) + D_80114830[i].v240.y;
            D_80114830[i].v240.z = (*(f32 *)(car + 0xEC)) + D_80114830[i].v240.z;
            (*(f32 *)(car + 0xE4)) = 0.0f;
            (*(f32 *)(car + 0xE8)) = 0.0f;
            (*(f32 *)(car + 0xEC)) = 0.0f;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800853B4);

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800853F4);

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_80085780);

s32 func_800858E8(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800858F0);

void *func_8008637C(u8 *arg0) {
    return (*arg0 * 0x2A8) + &D_80114838;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800863A4);
