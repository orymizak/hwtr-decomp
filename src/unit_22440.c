#include "common.h"
#include "include_asm.h"
#include "car.h"
typedef struct { u8 id; u8 p1[3]; u8 *car; u8 p8[0x1C]; f32 f24; u8 p28[0x1E4]; f32 f20C; u8 p210[8]; f32 f218; u8 p21C[0xC]; Vec3f v228; u8 p234[0xC]; Vec3f v240; u8 p24C[0x5C]; } Slot85_;
typedef struct { Vec3f p; u8 pad[0x7C]; } BlkD_;
typedef struct { u8 id; u8 p1[0xBF]; f32 m[3][3]; } CarD_;
typedef struct { u8 id; u8 p1[7]; BlkD_ blk; u8 p90[4]; Vec3f v94[4]; f32 m[3][3]; Vec3f h1[10]; s32 n160; Vec3f h2[10]; s32 n1DC; Vec3f v1E0; Vec3f v1EC; s32 z1F8; u8 p1FC[4]; s32 z200; s32 z204; u8 p208[0xC]; s32 z214; f32 f218; Vec3f v21C; Vec3f v228; Vec3f v234; Vec3f v240; Vec3f v24C; u8 p258[0x10]; s32 z268; s32 z26C; u8 p270[0x38]; } SlotD_;
typedef struct { Vec3f p; u8 pad[0x7C]; } BlkG_;
typedef struct { u8 id; u8 p1[0x7B]; Vec3f v7C; f32 f88; u8 p8C[0xC]; Vec3f pos; Vec3f vA4; u8 pB0[0xC]; s32 zBC; f32 m[3][3]; Vec3f vE4; u8 pF0[0x444]; f32 f534; } CarG_;
typedef struct { u8 id; u8 p1[7]; BlkG_ blk; u8 ok; u8 p91[3]; Vec3f v94[4]; f32 m[3][3]; Vec3f h1[10]; s32 n160; Vec3f h2[10]; s32 n1DC; Vec3f v1E0; Vec3f v1EC; s32 z1F8; f32 f1FC; s32 z200; s32 z204; u8 b208; u8 p209[3]; f32 f20C; u8 p210[4]; s32 z214; f32 f218; Vec3f v21C; Vec3f v228; Vec3f v234; Vec3f v240; Vec3f v24C; f32 f258; f32 f25C; f32 f260; u32 u264; s32 z268; s32 z26C; u8 p270[0x30]; s32 s2A0; u8 b2A4; u8 b2A5; u8 p2A6[2]; } SlotG_;
extern u8 D_80138D3E;
extern u8 D_80138D3F;
extern f32 D_80115828;
extern f32 D_8011582C;
extern f32 D_80115830;
extern u32 D_80115854;
u8 func_80080AD0(BlkG_ *, u8);
u32 func_800A76D8(void);
extern Slot85_ D_80114830[];
extern s32 D_80115824;
extern f32 D_80115820;
Car *func_800960D8(u8);
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
    Car *c1;
    Car *car;
    Vec3f t;
    s32 n;

    for (i = 0; i < 6; i++) {
        c1 = func_800960D8(i);
        if (c1->state != 1) {
            D_80114830[i].f24 = c1->unk474 / 1000.0f;
        }
        n = D_80115824 - 1;
        D_80114830[i].f218 = (u32)(n - c1->lap) * D_80115820 + D_80114830[i].f24;
    }
    for (i = 0; i < 6; i++) {
        car = func_800960D8(D_80114830[i].id);
        if (car->state != 1 || car->b4B8 != 0 || car->finished != 0) {
            continue;
        }
        if ((car->impulse.x) < 0.0f || 0.0f < (car->impulse.x) || (car->impulse.y) < 0.0f || 0.0f < (car->impulse.y) || (car->impulse.z) < 0.0f || 0.0f < (car->impulse.z)) {
            t.x = (car->unk748) * ((car->unk748) * (car->impulse.x) + (car->unk74C) * (car->impulse.y) + (car->unk750) * (car->impulse.z));
            t.y = (car->unk74C) * ((car->unk748) * (car->impulse.x) + (car->unk74C) * (car->impulse.y) + (car->unk750) * (car->impulse.z));
            t.z = (car->unk750) * ((car->unk748) * (car->impulse.x) + (car->unk74C) * (car->impulse.y) + (car->unk750) * (car->impulse.z));
            (car->impulse.x) -= t.x;
            (car->impulse.y) -= t.y;
            (car->impulse.z) -= t.z;
            D_80114830[i].v228.x = (car->impulse.x) + D_80114830[i].v228.x;
            D_80114830[i].v228.y = (car->impulse.y) + D_80114830[i].v228.y;
            D_80114830[i].v228.z = (car->impulse.z) + D_80114830[i].v228.z;
            func_80084B9C(car);
            (car->impulse.x) = 0.0f;
            (car->impulse.y) = 0.0f;
            (car->impulse.z) = 0.0f;
        }
        if ((car->vE4.x) < 0.0f || 0.0f < (car->vE4.x) || (car->vE4.y) < 0.0f || 0.0f < (car->vE4.y) || (car->vE4.z) < 0.0f || 0.0f < (car->vE4.z)) {
            D_80114830[i].v240.x = (car->vE4.x) + D_80114830[i].v240.x;
            D_80114830[i].v240.y = (car->vE4.y) + D_80114830[i].v240.y;
            D_80114830[i].v240.z = (car->vE4.z) + D_80114830[i].v240.z;
            (car->vE4.x) = 0.0f;
            (car->vE4.y) = 0.0f;
            (car->vE4.z) = 0.0f;
        }
    }
}

f32 func_800853B4(u8 i) {
    if (i < 6) {
        return D_80114830[i].f218;
    }
    return 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800853F4);

extern f32 D_80115838;
extern s8 D_80115845;
extern Car *D_80115848;
extern Car *D_8011584C;
extern u8 D_80115850;
u32 func_80096100(void);

/* Scales each slot's f20C (offset by 100) by car 0's k538, then records the first two cars whose state is not 1. */
void func_80085780(void) {
    Car *car;
    Car *c;
    u32 i;
    f32 k;

    car = func_800960D8(0);
    if (D_80115850 == 0 && car->state != 1) {
        k = 1.0f - D_80115838 * (1.0f - car->k538);
        for (i = 0; i < func_80096100(); i++) {
            D_80114830[i].f20C = (D_80114830[i].f20C + 100.0f) * k - 100.0f;
        }
    }
    D_8011584C = NULL;
    D_80115848 = NULL;
    for (i = 0; i < 6; i++) {
        c = func_800960D8(i);
        if (c->state == 1) {
            continue;
        }
        if (D_80115848 == NULL) {
            D_80115848 = c;
        } else if (D_8011584C == NULL) {
            D_8011584C = c;
            break;
        }
    }
    if (D_80115848 == NULL) {
        D_80115845 = 0;
        return;
    }
    if (D_8011584C != NULL) {
        D_80115845 = 2;
    } else {
        D_80115845 = 1;
    }
}

s32 func_800858E8(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800858F0);

void *func_8008637C(u8 *arg0) {
    return (*arg0 * 0x2A8) + &D_80114838;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", func_800863A4);
