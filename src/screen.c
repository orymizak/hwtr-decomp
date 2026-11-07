#include "common.h"
#include "include_asm.h"

#define SCREEN_BG_BUFFER_SIZE 0x4B200

extern void *D_800F20F0;
extern u8 D_800F20F4;
extern u8 D_800F20F5;
extern u8 D_800F20F6;
extern u8 D_800F20F8;
extern u16 D_800F20FA;
extern u16 D_800F20FC;
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
extern s32 D_800F216C;
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
void func_8006FBF0(void);
void func_8006FDB8(void);
void func_8006FE2C(u8 arg0, u8 arg1, u8 arg2);
void func_8006FF00(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
void func_8006FF90(s32 arg0, u8 arg1, u8 arg2, u8 arg3);
void func_80070148(void);
void func_80070E58(void);
void func_80070F38(void);
void func_80071530(void);
void func_80071A40(void);
void func_800761D4(u8 arg0, u8 arg1, u8 arg2, u16 arg3, u16 arg4);
void func_80076470(void *arg0);
void func_80076578(f32 arg0, void *arg1);
void func_800766FC(void *arg0, s32 arg1, s32 arg2);
void func_800802C0(void);
void func_80080360(void);
u8 func_80078320(s32 arg0);
void func_80078878(f32 arg0, f32 arg1, f32 arg2, f32 arg3);

s32 func_80077FB0(void) {
    func_8006FBF0();
    return 1;
}

s32 func_80077FD0(void) {
    D_800F20F8 = 1;
    D_800F20F5 = 1;
    D_800F2102 = 0xFF;
    D_800F210C = 320;
    D_800F20F4 = 0;
    D_800F20F6 = 0;
    D_800F20FE = 0;
    D_800F20FF = 0;
    D_800F2100 = 0;
    D_800F2104 = NULL;
    D_800F2108 = 0;
    D_800F210A = 0;
    D_800F210E = 240;
    D_800F2168 = 0;
    D_800F216C = 0;
    D_800F2170 = 0;
    D_800F2110 = 1.0f;
    D_800F211C = 1000.0f;
    D_800F2118 = 1.0f;
    D_800F2114 = 1e7f;
    D_800F2120 = 1.09f;
    D_800F2124 = 1.33333302f;
    func_80070E58();
    func_80070148();
    func_800802C0();
    D_800F2174 = 1;
    D_800F2178 = 1;
    D_800F217C = 0;
    D_800F2180 = 0;
    D_800F2184 = 0;
    func_80071A40();
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

void func_8007815C(u8 alpha) {
    u8 *current = &D_800F2102;

    if (alpha != *current && D_800F2104 != NULL) {
        *current = alpha;
        func_80076578(alpha / 255.0, D_800F2104);
    }
}

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
    func_800761D4(D_800F20FE, D_800F20FF, D_800F2100, D_800F20FA, D_800F20FC);
    if (D_800F2104 != NULL) {
        func_800766FC(D_800F2104, 640, 480);
    }
    func_80071530();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/screen", func_80078388);

INCLUDE_ASM("asm/nonmatchings/screen", func_800784F0);

u16 func_80078624(void) {
    return D_800F20FA;
}

u16 func_80078630(void) {
    return D_800F20FC;
}

void func_8007863C(s16 x, s16 y, s16 width, s16 height) {
    D_800F2108 = x;
    D_800F210C = width;
    D_800F210E = height;
    D_800F210A = (D_800F20FC - y) - height;
    func_80070F38();
}

INCLUDE_ASM("asm/nonmatchings/screen", func_80078684);

void func_800787EC(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    func_80078878(arg0, arg1, arg2 * 1.04, arg3);
}

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
        func_80070E58();
    }
}

void func_800788F8(u8 arg0, u8 arg1, u8 arg2) {
    func_8006FE2C(arg0, arg1, arg2);
}

void func_8007891C(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    func_8006FF00(arg0, arg1, arg2, arg3);
}

void func_80078940(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    func_8006FF90(arg0, arg1, arg2, arg3);
}

void func_80078964(void) {
    func_8006FDB8();
}

void func_80078980(u8 arg0) {
    if (arg0 == 1) {
        D_800F20F6 = 2;
    } else {
        D_800F20F6 = 0;
    }
}
