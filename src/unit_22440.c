#include "common.h"
#include "include_asm.h"
#include "car.h"
#include "race.h"
typedef struct { u8 id; u8 p1[3]; u8 *car; u8 p8[0x1C]; f32 f24; u8 p28[0x1E0]; u8 braking; u8 p209[3]; f32 f20C; u8 p210[8]; f32 f218; u8 p21C[0xC]; Vec3f v228; u8 p234[0xC]; Vec3f v240; s32 ctlTimer; f32 throttle; f32 steer; u8 p258[0x50]; } Slot85_;
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
u8 Bld_InitRacer(BlkG_ *, u8);
u32 RaceClock_Get(void);
extern Slot85_ gAi[];
extern s32 D_80115824;
extern f32 D_80115820;
void Ai_ApplyImpulse(Car *);
extern u8 D_80114838;

s32 Ai_Step();

INCLUDE_ASM("asm/nonmatchings/unit_22440", Ai_Step);

void Ai_Update(void) {
    Ai_Step();
}

extern u8 D_80138D4A;
extern u8 D_80138D4B;
extern u8 D_80138D4C;
extern u8 D_80138D4D;
extern u8 D_80138D4E;
extern u8 D_80138D4F;
extern u8 D_80138D50;

void Ai_ApplyImpulse(Car *car) {
    u8 *o = (u8 *)car;
    f32 m[3][4];
    f32 x;
    f32 y;
    f32 z;

    m[0][0] = *(f32 *)(o + 0xC0);
    m[0][1] = *(f32 *)(o + 0xCC);
    m[0][2] = *(f32 *)(o + 0xD8);
    m[1][0] = *(f32 *)(o + 0xC4);
    m[1][1] = *(f32 *)(o + 0xD0);
    m[1][2] = *(f32 *)(o + 0xDC);
    m[2][0] = *(f32 *)(o + 0xC8);
    m[2][1] = *(f32 *)(o + 0xD4);
    m[2][2] = *(f32 *)(o + 0xE0);
    x = *(f32 *)(o + 0xA4) * m[0][0] + *(f32 *)(o + 0xA8) * m[0][1] + *(f32 *)(o + 0xAC) * m[0][2];
    y = *(f32 *)(o + 0xA4) * m[1][0] + *(f32 *)(o + 0xA8) * m[1][1] + *(f32 *)(o + 0xAC) * m[1][2];
    z = *(f32 *)(o + 0xA4) * m[2][0] + *(f32 *)(o + 0xA8) * m[2][1] + *(f32 *)(o + 0xAC) * m[2][2];
    if (0.0f < x) {
        gAi[o[0]].steer = (gAi[o[0]].steer + D_80138D4A < 80.0f) ? gAi[o[0]].steer + D_80138D4A : 80.0f;
        gAi[o[0]].ctlTimer = 2;
    } else {
        gAi[o[0]].steer = (-80.0f < gAi[o[0]].steer - D_80138D4A) ? gAi[o[0]].steer - D_80138D4A : -80.0f;
        gAi[o[0]].ctlTimer = 2;
    }
    if (0.0f < y) {
        gAi[o[0]].throttle = (gAi[o[0]].throttle + D_80138D4D < (f32)D_80138D4B) ? gAi[o[0]].throttle + D_80138D4D : D_80138D4B;
        gAi[o[0]].ctlTimer = 2;
    } else {
        gAi[o[0]].throttle = (gAi[o[0]].throttle - D_80138D4E > (f32)(D_80138D4C - 100)) ? gAi[o[0]].throttle - D_80138D4E : D_80138D4C - 100;
        gAi[o[0]].ctlTimer = 2;
        gAi[o[0]].braking = 1;
    }
    if (0.0f < z) {
        gAi[o[0]].throttle = (gAi[o[0]].throttle - D_80138D50 > (f32)(D_80138D4C - 100)) ? gAi[o[0]].throttle - D_80138D50 : D_80138D4C - 100;
        gAi[o[0]].ctlTimer = 2;
    } else {
        gAi[o[0]].throttle = (gAi[o[0]].throttle + D_80138D4F < (f32)D_80138D4B) ? gAi[o[0]].throttle + D_80138D4F : D_80138D4B;
        gAi[o[0]].ctlTimer = 2;
    }
}

void Ai_SyncCars(void) {
    u32 i;
    Car *c1;
    Car *car;
    Vec3f t;
    s32 n;

    for (i = 0; i < 6; i++) {
        c1 = Car_Get(i);
        if (c1->state != 1) {
            gAi[i].f24 = c1->unk474 / 1000.0f;
        }
        n = D_80115824 - 1;
        gAi[i].f218 = (u32)(n - c1->lap) * D_80115820 + gAi[i].f24;
    }
    for (i = 0; i < 6; i++) {
        car = Car_Get(gAi[i].id);
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
            gAi[i].v228.x = (car->impulse.x) + gAi[i].v228.x;
            gAi[i].v228.y = (car->impulse.y) + gAi[i].v228.y;
            gAi[i].v228.z = (car->impulse.z) + gAi[i].v228.z;
            Ai_ApplyImpulse(car);
            (car->impulse.x) = 0.0f;
            (car->impulse.y) = 0.0f;
            (car->impulse.z) = 0.0f;
        }
        if ((car->vE4.x) < 0.0f || 0.0f < (car->vE4.x) || (car->vE4.y) < 0.0f || 0.0f < (car->vE4.y) || (car->vE4.z) < 0.0f || 0.0f < (car->vE4.z)) {
            gAi[i].v240.x = (car->vE4.x) + gAi[i].v240.x;
            gAi[i].v240.y = (car->vE4.y) + gAi[i].v240.y;
            gAi[i].v240.z = (car->vE4.z) + gAi[i].v240.z;
            (car->vE4.x) = 0.0f;
            (car->vE4.y) = 0.0f;
            (car->vE4.z) = 0.0f;
        }
    }
}

f32 Ai_GetRaceDistance(u8 i) {
    if (i < 6) {
        return gAi[i].f218;
    }
    return 0.0f;
}

extern s32 (*svcRandRange)(s32);
extern u8 D_80115850;
extern f32 D_80115834;
extern f32 D_80115838;
extern f32 D_8011583C;
extern f32 D_80115840;
extern u8 D_80115844;
extern char D_800FFA90[];
extern u8 D_80138D38, D_80138D39, D_80138D3A, D_80138D3B, D_80138D3C, D_80138D3D, D_80138D44, D_80138D45, D_80138D66, D_80138D67, D_80138D6C;
s32 strcmp(const char *, const char *);
u32 Bld_GetLapLength(void);
static inline f32 Lerp85(f32 t, f32 lo, f32 hi) {
    return t * (hi - lo) + lo;
}
s32 Ai_Init(RaceSetup *cfg) {
    u8 f;
    s32 i;

    D_80115854 = svcRandRange(0x40000000);
    D_80115850 = (cfg->flags & 0x100) && strcmp(cfg->sub, D_800FFA90) == 0;
    if (D_80115850) {
        D_80115840 = 0.0f;
        D_80115828 = 1.0f;
        D_8011582C = 1.0f;
        D_80115830 = 1.04f;
        D_80115834 = 0.4f;
        D_80115838 = 1.0f;
        D_8011583C = 2.0f;
    } else {
        D_80115828 = Lerp85(cfg->f9C / 255.0f, D_80138D38, D_80138D39) / 100.0f;
        D_8011582C = Lerp85(cfg->f9C / 255.0f, D_80138D3A, D_80138D3B) / 100.0f;
        D_80115830 = Lerp85(cfg->f9C / 255.0f, D_80138D44, D_80138D45) / 100.0f;
        D_80115834 = Lerp85(cfg->f9C / 255.0f, D_80138D3C, D_80138D3D) / 10.0f;
        D_80115838 = cfg->f9C / 255.0f;
        D_8011583C = Lerp85(cfg->f9C / 255.0f, D_80138D66, D_80138D67) / 10.0f;
        D_80115840 = Lerp85(cfg->f9C / 255.0f, D_80138D6C, 0.0f) / 10.0f;
    }
    D_80115844 = cfg->f19;
    for (i = 0; i < 6; i++) {
        gAi[i].car = Car_Get(i);
        gAi[i].id = gAi[i].car[0];
    }
    D_80115820 = Bld_GetLapLength() / 1000.0f;
    return 1;
}

extern f32 D_80115838;
extern s8 gAiPhysicsCarCount;
extern Car *gAiPhysicsCar0;
extern Car *gAiPhysicsCar1;
extern u8 D_80115850;
u32 Cars_GetCount(void);

/* Scales each slot's f20C (offset by 100) by car 0's k538, then records the first two cars whose state is not 1. */
void Ai_StartRace(void) {
    Car *car;
    Car *c;
    u32 i;
    f32 k;

    car = Car_Get(0);
    if (D_80115850 == 0 && car->state != 1) {
        k = 1.0f - D_80115838 * (1.0f - car->k538);
        for (i = 0; i < Cars_GetCount(); i++) {
            gAi[i].f20C = (gAi[i].f20C + 100.0f) * k - 100.0f;
        }
    }
    gAiPhysicsCar1 = NULL;
    gAiPhysicsCar0 = NULL;
    for (i = 0; i < 6; i++) {
        c = Car_Get(i);
        if (c->state == 1) {
            continue;
        }
        if (gAiPhysicsCar0 == NULL) {
            gAiPhysicsCar0 = c;
        } else if (gAiPhysicsCar1 == NULL) {
            gAiPhysicsCar1 = c;
            break;
        }
    }
    if (gAiPhysicsCar0 == NULL) {
        gAiPhysicsCarCount = 0;
        return;
    }
    if (gAiPhysicsCar1 != NULL) {
        gAiPhysicsCarCount = 2;
    } else {
        gAiPhysicsCarCount = 1;
    }
}

s32 Ai_Shutdown(void) {
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", Ai_InitRacer);

void *Ai_GetSlot(u8 *arg0) {
    return (*arg0 * 0x2A8) + &D_80114838;
}

INCLUDE_ASM("asm/nonmatchings/unit_22440", Ai_RestoreSlot);
