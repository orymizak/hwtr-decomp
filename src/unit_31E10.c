#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
/* TODO(match-debt): empty loop needed to match (scheduling barrier). Assumed to be a debug hook
 * compiled out in release builds; verify against a real cause when one turns up. */
#define DEBUG_NOP() do { } while (0)
typedef struct { s32 d[0x22]; } B88_;
typedef Vec3f V3_1B0;
typedef Mtx3f M3_1B0;
extern void (*svcModelWreck)(u8, u8, V3_1B0);
extern void (*sysInputRumble)(u8, u8);
extern void (*audCarSndSetLoopCVolume)(u8, f32);
extern void (*audCarSndPlayLoopC)(u8, s32, s32);
extern u8 D_80138D6A;
extern u8 D_80138D6D;
extern u8 D_80138D6E;
extern u8 D_80138D6F;
extern u8 D_80138D70;
extern u8 D_80138D71;
void Debris_SpawnFromCar(u8 *);
void RaceCam_Shake(u8);
extern s32 D_8011B320;
extern s32 (*svcRandRange)(s32);
extern void (*audSfxPlay)(s32, s32, f32, f32);
u8 Car_UseTurbo(u8 *);
void Car_Simulate(u8 *);
void Car_AirControl(u8 *);
u32 RaceClock_Get(void);
void Replay_Record(void);
void Car_UpdateWheelSpin(Car *);
void Car_UpdateAirTricks(Car *, f32);
void Car_Unstick(u8 *);
void Car_Respawn(void);
void Ai_RestoreSlot(u8 *, u8 *);
B88_ *Ai_GetSlot(u8 *);
void RigidBody_Integrate(void *, f32);
void Mtx3_Orthonormalize(void *);
extern void (*sysInputUpdate)(s32);
extern s32 (*sysInputGetControl)(s32, s32);
extern void (*sysInputHasRumble)(s32, s32, s32);
s32 Race_GetPhase(void);
void Car_GetSpeedAndGrip(u8, u8 *, u8 *);
extern u8 gImpactCooldown[];
extern s32 (*svcTimeGetMillis)();
s32 Coll_AddCar(void *);
s32 Coll_RemoveById(void *);

extern s32 gCarCount;

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Cars_Init);

void RaceAudio_QueueAnnouncer(s32);

/* Adds to the car's counter at 0x6CC, capped at 10; returns how much was added. */
u8 Car_AddTurbos(Car *c, s32 add) {
    u8 added = add;
    s32 total = c->turbos + add;

    c->turbos = total;
    if ((u8)total > 10) {
        added = add + 10 - total;
        c->turbos = 10;
    }
    /* TODO(match-debt): n6CC == 10 && unk6CD == 0 is read as one word. */
    if ((*(u32 *)&c->turbos & 0xFFFF0000) == 0x0A000000) {
        RaceAudio_QueueAnnouncer(3);
        c->unk6CD = 1;
    }
    return added;
}

/* Per-car result block filled by Cars_Shutdown (0x2C bytes from RaceResult + 0x1C). */
typedef struct { s32 lap[8]; s32 best; u8 laps; u8 p25; u8 points; u8 p27; s32 score; } CarResult_;
/* Race result as written by Cars_Shutdown: the per-car blocks start at 0x1C. */
typedef struct { u8 pad0[0x1C]; CarResult_ car[1]; } CarsResult_;
extern u8 D_8011B31C;
extern u8 D_8011B31D;
extern u8 (*svcModelUnload)(u8);

/* Copies each car's lap times (or only its best lap) and score into the race result, then removes the
 * cars from the collision lists and unloads their models; returns 0 if a model fails to unload. */
u8 Cars_Shutdown(CarsResult_ *res) {
    u8 i;
    u8 j;

    for (i = 0; i < (u32)gCarCount; i++) {
        if (D_8011B31D == 1) {
            res->car[i].lap[0] = gCars[i].bestLap;
            if (gCars[i].lapStart[0] != 0) {
                res->car[i].laps = 1;
            } else {
                res->car[i].laps = 0;
            }
        } else {
            for (j = 0; j < 8; j++) {
                res->car[i].lap[j] = gCars[i].lapStart[j] - gCars[i].unk440;
            }
            res->car[i].laps = gCars[i].lap;
        }
        res->car[i].best = gCars[i].bestLap;
        if (D_8011B31C == 1) {
            res->car[i].score = gCars[i].score;
        }
        Coll_RemoveById(&gCars[i]);
        if (!svcModelUnload(i)) {
            return 0;
        }
    }
    return 1;
}

void Car_NoteGroundContact(Car *car) {
    s32 now;
    u32 dt;

    now = svcTimeGetMillis();
    dt = now - car->contactLast;
    if (dt <= 100) {
        car->contactTime += dt;
    } else {
        car->contactTime = 0;
    }
    car->contactLast = now;
    if (car->airborne != 0 && car->contactTime > 500) {
        car->airborne = 0;
    }
}

typedef Vec3f V3_E30;
extern u8 D_8011B31C;
void Trick_Evaluate(s32, s32, s32, s32, s32, s32, s32, u8 *, u16 *, u32 *);
void Hud_ShowTrick(u8, u32, u16, u8);
static inline V3_E30 mul_E30(Mtx3f *m, V3_E30 *v) {
    V3_E30 r;

    r.x = m->m[0][0] * v->x + m->m[0][1] * v->y + m->m[0][2] * v->z;
    r.y = m->m[1][0] * v->x + m->m[1][1] * v->y + m->m[1][2] * v->z;
    r.z = m->m[2][0] * v->x + m->m[2][1] * v->y + m->m[2][2] * v->z;
    return r;
}
void Car_UpdateAirTricks(Car *p, f32 dt) {
    s32 a[3];
    s32 b[3];
    V3_E30 v;
    V3_E30 w;
    Mtx3f t;
    s32 ms;
    u8 n;
    u16 x;
    u32 pts;
    u8 c;
    s32 z;
    s32 k;
    s32 q;
    s32 el;

    ms = dt * 1000.0f;
    pts = 0;
    x = 0;
    n = 0;
    if (p->airborne == 0) {
        if (p->cnt3E7 == 0) {
            p->airborne = 1;
            p->airTime = 0;
            t.m[0][0] = p->rot.m[0][0];
            t.m[0][1] = p->rot.m[1][0];
            t.m[0][2] = p->rot.m[2][0];
            t.m[1][0] = p->rot.m[0][1];
            t.m[1][1] = p->rot.m[1][1];
            t.m[1][2] = p->rot.m[2][1];
            t.m[2][0] = p->rot.m[0][2];
            t.m[2][1] = p->rot.m[1][2];
            t.m[2][2] = p->rot.m[2][2];
            v = p->angVel;
            w = mul_E30(&t, &v);
            p->v47C = w;
            p->v488.x = 0.0f;
            p->v488.y = 0.0f;
            p->v488.z = 0.0f;
            p->v494.x = 0.0f;
            p->v494.y = 0.0f;
            p->v494.z = 0.0f;
        }
    } else {
        if (p->cnt3E8 == 0) {
            if (p->cnt3E7 != 0) {
                p->airborne = 0;
            }
            p->airTime += ms;
            t.m[0][0] = p->rot.m[0][0];
            t.m[0][1] = p->rot.m[1][0];
            t.m[0][2] = p->rot.m[2][0];
            t.m[1][0] = p->rot.m[0][1];
            t.m[1][1] = p->rot.m[1][1];
            t.m[1][2] = p->rot.m[2][1];
            t.m[2][0] = p->rot.m[0][2];
            t.m[2][1] = p->rot.m[1][2];
            t.m[2][2] = p->rot.m[2][2];
            v = p->angVel;
            v = mul_E30(&t, &v);
            if (!(v.x * p->v47C.x >= 0.0f)) {
                if (((p->v494.x) < 0.0f ? -(p->v494.x) : (p->v494.x)) < ((p->v488.x) < 0.0f ? -(p->v488.x) : (p->v488.x))) {
                    p->v494.x = p->v488.x;
                }
            }
            if (!(v.y * p->v47C.y >= 0.0f)) {
                if (((p->v494.y) < 0.0f ? -(p->v494.y) : (p->v494.y)) < ((p->v488.y) < 0.0f ? -(p->v488.y) : (p->v488.y))) {
                    p->v494.y = p->v488.y;
                }
            }
            if (!(v.z * p->v47C.z >= 0.0f)) {
                if (((p->v494.z) < 0.0f ? -(p->v494.z) : (p->v494.z)) < ((p->v488.z) < 0.0f ? -(p->v488.z) : (p->v488.z))) {
                    p->v494.z = p->v488.z;
                }
            }
            w.x = v.x * dt;
            w.y = v.y * dt;
            w.z = v.z * dt;
            p->v488.x = w.x + p->v488.x;
            p->v488.y = w.y + p->v488.y;
            p->v488.z = w.z + p->v488.z;
            p->v47C = v;
        } else {
            p->airTime += ms;
            p->airborne = 0;
            el = p->airTime;
            a[0] = (s32)(p->v494.x * 180.0f / 3.14159f);
            b[0] = (s32)(p->v488.x * 180.0f / 3.14159f) - a[0];
            a[1] = (s32)(p->v494.y * 180.0f / 3.14159f);
            b[1] = (s32)(p->v488.y * 180.0f / 3.14159f) - a[1];
            a[2] = (s32)(p->v494.z * 180.0f / 3.14159f);
            b[2] = (s32)(p->v488.z * 180.0f / 3.14159f) - a[2];
            Trick_Evaluate(a[0], b[0], a[1], b[1], a[2], b[2], el, &n, &x, &pts);
            if (pts != 0) {
                if ((p->flags & 1) && (pts > 1000 || svcRandRange(8) == 0)) {
                    if (svcRandRange(2) != 0) {
                        RaceAudio_QueueAnnouncer(1);
                    } else {
                        RaceAudio_QueueAnnouncer(2);
                    }
                }
                if (D_8011B31C == 0) {
                    pts = 0;
                }
                Hud_ShowTrick(p->unk00C, pts, x, n);
                q = p->score;
                k = n;
                p->score = q + pts;
                c = k;
                p->turbos += k;
                if (p->turbos > 10) {
                    c = k + 10 - p->turbos;
                    p->turbos = 10;
                }
                if ((*(u32 *)((u8 *)p + 0x6CC) & 0xFFFF0000) == 0x0A000000) {
                    RaceAudio_QueueAnnouncer(3);
                    p->unk6CD = 1;
                }
                z = c;
                n = z;
                if (n) {
                    audSfxPlay(0x3A, 1, 1.0f, 1.0f);
                } else {
                    audSfxPlay(0x3B, 1, 1.0f, 1.0f);
                }
            }
        }
        p->unk4A4 += ms;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_AirControl);

void Car_UpdatePhysics(Car *p, f32 dt) {
    Vec3f col;
    u8 f;
    f32 v;

    if (p->unk6C1 != 0) {
        Car_Respawn();
        p->unk6C1 = 0;
    } else if (p->b4B8 != 0) {
        if (p->unk025 != 0 && p->unk4BC >= 1000) {
            ((void (*)(u8 *))Car_Respawn)(p);
        }
    } else if (p->unk025 != 0 && p->unk774 == 0 && (p->cnt3E7 != p->nPoints || p->fBC < 176.0f)) {
        ((void (*)(u8 *))Car_Respawn)(p);
    }
    if (p->b4B8 == 0) {
        if (p->unk027 != 0 && p->unk029 == 0) {
            if (D_8011B320 & 0x40) {
                Car_UseTurbo(p);
            } else if (p->turbos != 0) {
                if (Car_UseTurbo(p)) {
                    p->turbos--;
                }
            } else {
                audSfxPlay(0x2C, 0, 1.0f, 1.0f);
            }
            p->unk6CD = 1;
        }
        p->unk028 = p->unk026;
        p->unk029 = p->unk027;
        f = 0;
        if (p->cnt3E7 == p->nPoints && p->rot.m[2][2] > 0.9848f && p->roomObj->list->next == NULL && p->b4B8 == 0 && (p->flags & 0x20) && p->fBC > 88.0f) {
            f = 1;
        }
        f = f || p->savedRoom == -1;
        if (f) {
            p->savedPos = p->pos;
            p->savedRot.m[0][0] = p->rot.m[0][0];
            p->savedRot.m[0][1] = p->rot.m[0][1];
            p->savedRot.m[0][2] = p->rot.m[0][2];
            p->savedRot.m[1][0] = p->rot.m[1][0];
            p->savedRot.m[1][1] = p->rot.m[1][1];
            p->savedRot.m[1][2] = p->rot.m[1][2];
            p->savedRot.m[2][0] = p->rot.m[2][0];
            p->savedRot.m[2][1] = p->rot.m[2][1];
            p->savedRot.m[2][2] = p->rot.m[2][2];
            p->savedRoom = p->roomObj->list->key;
        }
        if (p->pickupFx[0] != 0 && (p->unk024 != 0 || p->unk018 > 0.2f) && p->cnt3E7 == p->nPoints) {
            col.x = p->rot.m[0][1];
            col.y = p->rot.m[1][1];
            col.z = p->rot.m[2][1];
            if (p->vel.x * col.x + p->vel.y * col.y + p->vel.z * col.z > 0.0f) {
                p->impulse.x *= 0.9f;
                p->impulse.y *= 0.9f;
                p->impulse.z *= 0.9f;
            }
        }
        if (p->b13C == 0) {
            Car_Simulate(p);
        }
        if (p->cnt3E7 == 0) {
            Car_AirControl(p);
        } else {
            p->unk6C3 = 0;
        }
    }
    f = 0;
    if ((p->flags & 1) && p->airborne != 0 && p->airTime > 500 && p->vel.z > 0.0f) {
        f = RaceClock_Get() - p->unk770 > 500;
    }
    RigidBody_Integrate((u8 *)&p->m2C, dt);
    if (f && p->vel.z < 0.0f && svcRandRange(2) == 0) {
        Replay_Record();
    }
    if ((p->flags & 1) && p->b4B8 != 0 && p->unk4BC == 500 && svcRandRange(2) == 0) {
        Replay_Record();
    }
    if (p->b13C == 0) {
        Mtx3_Orthonormalize((u8 *)&p->rot);
        Car_UpdateWheelSpin(p);
    }
    if (p->b4B8 == 0) {
        if (p->flags & 1) {
            Car_UpdateAirTricks(p, dt);
        }
        if (p->unk014 < 0.0f || p->unk014 > 0.0f || p->unk018 < 0.0f || p->unk018 > 0.0f) {
            p->b13C = 0;
        }
    }
    if (p->r3EC.b8 != 0) {
        v = p->unk014;
    } else {
        v = p->unk018;
    }
    if (v > 0.0f && p->b4B8 == 0) {
        p->flags2 |= 4;
    } else {
        p->flags2 &= ~4;
    }
    if (p->unk014 > 0.5f && (p->yaw < 0.0f ? -p->yaw : p->yaw) > 0.5f && p->cnt3E7 == p->nPoints && p->fBC < 88.0f && RaceClock_Get() - p->unk770 < 100) {
        p->unk76C += 10;
    } else {
        p->unk76C = 0;
    }
    if (p->unk76C > 500) {
        Car_Unstick(p);
    }
}

void Car_UpdateOnPath(Car *p, f32 dt) {
    u8 f;

    if (p->unk6C1 != 0) {
        Car_Respawn();
        Ai_RestoreSlot(p, p->savedState);
        p->unk6C1 = 0;
    }
    f = 0;
    if (p->cnt3E7 == p->nPoints && p->rot.m[2][2] > 0.9848f && p->b4B8 == 0) {
        f = (p->flags >> 5) & 1;
    }
    f = f | (p->savedRoom == -1);
    if (f != 0) {
        p->savedPos = p->pos;
        p->savedRot.m[0][0] = p->rot.m[0][0];
        p->savedRot.m[0][1] = p->rot.m[0][1];
        p->savedRot.m[0][2] = p->rot.m[0][2];
        p->savedRot.m[1][0] = p->rot.m[1][0];
        p->savedRot.m[1][1] = p->rot.m[1][1];
        p->savedRot.m[1][2] = p->rot.m[1][2];
        p->savedRot.m[2][0] = p->rot.m[2][0];
        p->savedRot.m[2][1] = p->rot.m[2][1];
        p->savedRot.m[2][2] = p->rot.m[2][2];
        p->savedRoom = p->roomObj->list->key;
        *(B88_ *)p->savedState = *Ai_GetSlot(p);
    }
    if (p->b4B8 != 0 || p->finished != 0) {
        RigidBody_Integrate((u8 *)&p->m2C, dt);
        Mtx3_Orthonormalize((u8 *)&p->rot);
    }
}

void func_80093990(void) {

}

extern void (*svcModelFlashOff)(u8);
void Ai_Update(s32);
void Ai_SyncCars(void);
void Cars_UpdatePositions(u32);

void Cars_Update(u32 dt) {
    f32 sec = dt / 1000.0f;
    u8 i;
    Car *c;

    for (c = gCars, i = 0; i < (u32)gCarCount; c++, i++) {
        if (c->b4B8 != 0) {
            c->unk4BC += dt;
            if (c->unk4BC >= 500 && c->unk4B9 != 0) {
                RaceAudio_QueueAnnouncer(0);
                c->unk4B9 = 0;
            }
            if (c->unk4BC >= 3000) {
                c->unk6C1 = 1;
            }
        }
        if (c->unk774 != 0) {
            if (dt < c->unk774) {
                c->unk774 -= dt;
            } else {
                c->unk774 = 0;
                c->roomObj->flags &= ~1;
            }
        }
        if (c->b4C0 != 0 && c->fBC + 528.0f < c->f4C4) {
            svcModelFlashOff(c->id);
            c->b4C0 = 0;
        }
        switch ((u32)c->state) {
            case 2:
                Car_UpdatePhysics(c, sec);
                break;
            case 1:
                Car_UpdateOnPath(c, sec);
                break;
            case 0:
                break;
        }
    }
    Ai_Update(10);
    Ai_SyncCars();
    Cars_UpdatePositions(dt);
}

extern u8 gCarOrder[];
void Car_CheckWrongWay(Car *c, u32 dt);

/* Race positions: sorts gCarOrder by lap (more first) then distance key, stores each car's position
 * and checks wrong-way driving. */
void Cars_UpdatePositions(u32 dt) {
    u32 key[6];
    u32 i;
    u32 j;
    Car *c;
    u8 a;
    u8 b;

    for (c = gCars, i = 0; i < (u32)gCarCount; c++, i++) {
        key[i] = c->unk474 + ((0x7F - c->lap) << 24);
    }
    for (i = 0; i < (u32)gCarCount - 1; i++) {
        for (j = i + 1; j < (u32)gCarCount; j++) {
            a = gCarOrder[i];
            b = gCarOrder[j];
            if (key[a] > key[b]) {
                gCarOrder[i] = b;
                gCarOrder[j] = a;
            }
        }
    }
    for (i = 0; i < (u32)gCarCount; i++) {
        gCars[gCarOrder[i]].position = i;
    }
    for (c = gCars, i = 0; i < (u32)gCarCount; c++, i++) {
        if (c->flags & 1) {
            Car_CheckWrongWay(c, dt);
        }
    }
}

void Car_UpdatePoints(Car *car) {
    Vec3f col1;
    Vec3f col0;
    Vec3f r1;
    Vec3f r2;
    Mtx34f m;
    Vec3f rel;
    Vec3f t;
    Vec3f a;
    Vec3f b;
    Vec3f d;
    Vec3f cr;
    f32 c, s, inv;
    u32 i;
    f32 rx, ry, rz;

    m.m[0][0] = car->rot.m[0][0];
    m.m[0][1] = car->rot.m[0][1];
    m.m[0][2] = car->rot.m[0][2];
    m.m[0][3] = car->pos.x;
    m.m[1][0] = car->rot.m[1][0];
    m.m[1][1] = car->rot.m[1][1];
    m.m[1][2] = car->rot.m[1][2];
    m.m[1][3] = car->pos.y;
    m.m[2][0] = car->rot.m[2][0];
    m.m[2][1] = car->rot.m[2][1];
    m.m[2][2] = car->rot.m[2][2];
    m.m[2][3] = car->pos.z;
    col0.x = car->rot.m[0][0];
    col0.y = car->rot.m[1][0];
    col0.z = car->rot.m[2][0];
    col1.x = car->rot.m[0][1];
    col1.y = car->rot.m[1][1];
    col1.z = car->rot.m[2][1];
    c = cosf(-car->yaw);
    s = sinf(-car->yaw);
    a.x = col0.x * s;
    a.y = col0.y * s;
    a.z = col0.z * s;
    b.x = col1.x * c;
    b.y = col1.y * c;
    b.z = col1.z * c;
    r1.x = b.x - a.x;
    r1.y = b.y - a.y;
    r1.z = b.z - a.z;
    r2.x = b.x + a.x;
    r2.y = b.y + a.y;
    r2.z = b.z + a.z;
    car->cnt3E7 = 0;
    car->cnt3E8 = 0;
    for (i = 0; i < car->nPoints; i++) {
        if (car->points[i].flags & 2) {
            if (i < 2) {
                car->points[i].dir = r1;
            } else {
                car->points[i].dir = r2;
            }
        } else {
            car->points[i].dir = col1;
        }
        if (car->points[i].contact) {
            t.x = car->points[i].n.x * (car->points[i].dir.x * car->points[i].n.x + car->points[i].dir.y * car->points[i].n.y + car->points[i].dir.z * car->points[i].n.z);
            t.y = car->points[i].n.y * (car->points[i].dir.x * car->points[i].n.x + car->points[i].dir.y * car->points[i].n.y + car->points[i].dir.z * car->points[i].n.z);
            t.z = car->points[i].n.z * (car->points[i].dir.x * car->points[i].n.x + car->points[i].dir.y * car->points[i].n.y + car->points[i].dir.z * car->points[i].n.z);
            t.x = car->points[i].dir.x - t.x;
            t.y = car->points[i].dir.y - t.y;
            t.z = car->points[i].dir.z - t.z;
            inv = 1.0f / sqrtf(t.x * t.x + t.y * t.y + t.z * t.z);
            t.x *= inv;
            t.y *= inv;
            t.z *= inv;
            car->points[i].dir = t;
        }
        rel.x = rx = car->points[i].pos.x - car->center.x;
        rel.y = ry = car->points[i].pos.y - car->center.y;
        rel.z = rz = car->points[i].pos.z - car->center.z;
        car->points[i].world.x = m.m[0][0] * rx + m.m[0][1] * ry + m.m[0][2] * rz + m.m[0][3];
        car->points[i].world.y = m.m[1][0] * rx + m.m[1][1] * ry + m.m[1][2] * rz + m.m[1][3];
        car->points[i].world.z = m.m[2][0] * rx + m.m[2][1] * ry + m.m[2][2] * rz + m.m[2][3];
        if (car->points[i].contact) {
            d.x = car->points[i].pt.x - car->pos.x;
            d.y = car->points[i].pt.y - car->pos.y;
            d.z = car->points[i].pt.z - car->pos.z;
            cr.x = car->angVel.y * d.z - d.y * car->angVel.z;
            cr.y = d.x * car->angVel.z - car->angVel.x * d.z;
            cr.z = car->angVel.x * d.y - d.x * car->angVel.y;
            car->points[i].vel.x = car->vel.x + cr.x;
            car->points[i].vel.y = car->vel.y + cr.y;
            car->points[i].vel.z = car->vel.z + cr.z;
            if (car->points[i].contact) {
                car->cnt3E7++;
                if (-(car->points[i].n.x * car->up.x + car->points[i].n.y * car->up.y + car->points[i].n.z * car->up.z) > 0.5f) {
                    car->cnt3E8++;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_Respawn);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_ApplyAero);

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_Simulate);

/* Sets each wheel's spin rate: from the ground speed when it touches the ground, else from the
 * engine rpm (driven wheels) or zero. */
void Car_UpdateWheelSpin(Car *c) {
    Vec3f vel = c->vel;
    f32 speed = c->fBC;
    u8 i;
    CarPoint *w;
    f32 a;
    f32 b;
    f32 r;

    for (i = 0, w = c->points; i < c->nPoints; w++, i++) {
        if (w->contact != 0 && w->b60 == 0) {
            r = speed / w->h * 2.0f;
            w->rate = r;
            if (vel.x * w->dir.x + vel.y * w->dir.y + vel.z * w->dir.z < 0.0f) {
                w->rate = -r;
            }
        } else if (i >= 2 && c->unk024 != 0) {
            w->rate = 0.0f;
        } else {
            if (c->r3EC.b8 != 0) {
                a = c->unk018;
                b = c->unk014;
            } else {
                a = c->unk014;
                b = c->unk018;
            }
            if (w->flags & 4) {
                if (!(a < b)) {
                    w->rate = c->r3EC.rpm * 6.28318f / 60.0f;
                    continue;
                }
            } else if (!(0.5f < b)) {
                continue;
            }
            w->rate = 0.0f;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_ComputeMass);

/* Returns the car state: position (with offset), rotation, velocity, flags and the 0x13C byte. */
void Car_GetState(u8 id, Vec3f *pos, Mtx3f *rot, Vec3f *vel, u32 *flags, u8 *out) {
    Car *c = &gCars[id];

    pos->x = c->pos.x + c->v8C.x;
    pos->y = c->pos.y + c->v8C.y;
    pos->z = c->pos.z + c->v8C.z;
    rot->m[0][0] = c->rot.m[0][0];
    DEBUG_NOP();
    rot->m[0][1] = c->rot.m[0][1];
    rot->m[0][2] = c->rot.m[0][2];
    rot->m[1][0] = c->rot.m[1][0];
    rot->m[1][1] = c->rot.m[1][1];
    rot->m[1][2] = c->rot.m[1][2];
    rot->m[2][0] = c->rot.m[2][0];
    rot->m[2][1] = c->rot.m[2][1];
    rot->m[2][2] = c->rot.m[2][2];
    *vel = c->vel;
    *flags = c->flags;
    *out = Race_GetPhase() ? c->b13C : 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_GetAudioInfo);

extern u8 D_800F3890[];
/* Returns the speed scaled to 0..255 and the average surface value of the wheels in contact. */
void Car_GetSpeedAndGrip(u8 id, u8 *grip, u8 *speed) {
    Car *c = &gCars[id];
    s32 v;
    s32 i;
    s32 sum;
    CarPoint *p;

    v = c->fBC / 2304.0f * 255.0f;
    if (v < 0) {
        v = 0;
    }
    if (v > 255) {
        v = 255;
    }
    *speed = v;
    sum = 0;
    for (i = 0, p = c->points; i < c->nPoints; i++, p++) {
        if (p->contact) {
            sum += D_800F3890[p->c31];
        }
    }
    *grip = sum / c->nPoints;
}

Car *Car_Get(u8 i) {
    return &gCars[i];
}

s32 Cars_GetCount(void) {
    return gCarCount;
}

s32 Car_IsPlayer(u8 i) {
    return gCars[i].flags & 1;
}

void Car_SetState(u8 i, s32 state) {
    Car *car = &gCars[i];

    if (car->state == 2) {
        Coll_RemoveById(car);
    }
    car->state = state;
    if (state == 2) {
        Coll_AddCar(car);
    }
}

void Car_StartCrash(Car *p, u8 id, u8 flip) {
    V3_1B0 v;
    M3_1B0 t;
    M3_1B0 a;
    M3_1B0 b;
    V3_1B0 w;
    M3_1B0 *r;
    M3_1B0 *in;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 one;
    s32 (**rnd)(s32);

    if (p->b4B8 == 0) {
        one = 1;
        p->b4B8 = one;
        if (flip) {
            p->vel.x = 0.0f;
            p->vel.y = 0.0f;
            p->vel.z = 0.0f;
            p->angVel.x = -p->angVel.x;
            p->angVel.y = -p->angVel.y;
            p->angVel.z = -p->angVel.z;
        }
        p->vel.x *= 0.5f;
        p->vel.y *= 0.5f;
        p->vel.z *= 0.5f;
        v = p->vel;
        if (p->flags & 1) {
            Debris_SpawnFromCar(p);
        }
        rnd = &svcRandRange;
        p->vel.z += ((*rnd)(10) + 10) * 17.6f;
        p->impulse.x = p->vel.x * p->scale;
        p->impulse.y = p->vel.y * p->scale;
        p->impulse.z = p->vel.z * p->scale;
        dx = (*rnd)(24) - 12;
        dy = (*rnd)(24) - 12;
        dz = (*rnd)(24) - 12;
        p->angVel.x += dx;
        p->angVel.y += dy;
        p->angVel.z += dz;
        r = &p->rot;
        in = &p->m2C;
        t.m[0][0] = r->m[0][0];
        t.m[0][1] = r->m[1][0];
        t.m[0][2] = r->m[2][0];
        t.m[1][0] = r->m[0][1];
        t.m[1][1] = r->m[1][1];
        t.m[1][2] = r->m[2][1];
        t.m[2][0] = r->m[0][2];
        t.m[2][1] = r->m[1][2];
        t.m[2][2] = r->m[2][2];
        a.m[0][0] = r->m[0][0] * in->m[0][0] + r->m[0][1] * in->m[1][0] + r->m[0][2] * in->m[2][0];
        a.m[0][1] = r->m[0][0] * in->m[0][1] + r->m[0][1] * in->m[1][1] + r->m[0][2] * in->m[2][1];
        a.m[0][2] = r->m[0][0] * in->m[0][2] + r->m[0][1] * in->m[1][2] + r->m[0][2] * in->m[2][2];
        a.m[1][0] = r->m[1][0] * in->m[0][0] + r->m[1][1] * in->m[1][0] + r->m[1][2] * in->m[2][0];
        a.m[1][1] = r->m[1][0] * in->m[0][1] + r->m[1][1] * in->m[1][1] + r->m[1][2] * in->m[2][1];
        a.m[1][2] = r->m[1][0] * in->m[0][2] + r->m[1][1] * in->m[1][2] + r->m[1][2] * in->m[2][2];
        a.m[2][0] = r->m[2][0] * in->m[0][0] + r->m[2][1] * in->m[1][0] + r->m[2][2] * in->m[2][0];
        a.m[2][1] = r->m[2][0] * in->m[0][1] + r->m[2][1] * in->m[1][1] + r->m[2][2] * in->m[2][1];
        a.m[2][2] = r->m[2][0] * in->m[0][2] + r->m[2][1] * in->m[1][2] + r->m[2][2] * in->m[2][2];
        b.m[0][0] = a.m[0][0] * t.m[0][0] + a.m[0][1] * t.m[1][0] + a.m[0][2] * t.m[2][0];
        b.m[0][1] = a.m[0][0] * t.m[0][1] + a.m[0][1] * t.m[1][1] + a.m[0][2] * t.m[2][1];
        b.m[0][2] = a.m[0][0] * t.m[0][2] + a.m[0][1] * t.m[1][2] + a.m[0][2] * t.m[2][2];
        b.m[1][0] = a.m[1][0] * t.m[0][0] + a.m[1][1] * t.m[1][0] + a.m[1][2] * t.m[2][0];
        b.m[1][1] = a.m[1][0] * t.m[0][1] + a.m[1][1] * t.m[1][1] + a.m[1][2] * t.m[2][1];
        b.m[1][2] = a.m[1][0] * t.m[0][2] + a.m[1][1] * t.m[1][2] + a.m[1][2] * t.m[2][2];
        b.m[2][0] = a.m[2][0] * t.m[0][0] + a.m[2][1] * t.m[1][0] + a.m[2][2] * t.m[2][0];
        b.m[2][1] = a.m[2][0] * t.m[0][1] + a.m[2][1] * t.m[1][1] + a.m[2][2] * t.m[2][1];
        b.m[2][2] = a.m[2][0] * t.m[0][2] + a.m[2][1] * t.m[1][2] + a.m[2][2] * t.m[2][2];
        w.x = b.m[0][0] * p->angVel.x + b.m[0][1] * p->angVel.y + b.m[0][2] * p->angVel.z;
        w.y = b.m[1][0] * p->angVel.x + b.m[1][1] * p->angVel.y + b.m[1][2] * p->angVel.z;
        w.z = b.m[2][0] * p->angVel.x + b.m[2][1] * p->angVel.y + b.m[2][2] * p->angVel.z;
        p->vE4 = w;
        p->flags2 &= ~2;
        svcModelWreck(p->id, id, v);
        if (p->flags & 1) {
            if ((*rnd)(2) != 0) {
                p->unk4B9 = one;
            }
            RaceCam_Shake(p->id);
            sysInputRumble(p->id, D_80138D6A);
            audSfxPlay(0x1D, 1, 1.0f, 1.0f);
        }
        audCarSndSetLoopCVolume(p->id, 1.0f);
        audCarSndPlayLoopC(p->id, 0x1D, 1);
    }
}

static inline void push_954(Car *c, Vec3f *at, Vec3f *f, Vec3f *a) {
    Vec3f tq;

    a->x = at->x - c->pos.x;
    a->y = at->y - c->pos.y;
    a->z = at->z - c->pos.z;
    tq.x = a->y * f->z - f->y * a->z;
    tq.y = f->x * a->z - a->x * f->z;
    tq.z = a->x * f->y - f->x * a->y;
    c->torque.x = tq.x + c->torque.x;
    c->torque.y = tq.y + c->torque.y;
    c->torque.z = tq.z + c->torque.z;
    c->force.x = c->force.x + f->x;
    c->force.y = c->force.y + f->y;
    c->force.z = c->force.z + f->z;
}

void Car_SelfRight(Car *c) {
    Vec3f axis;
    Vec3f pos;
    Vec3f pt;
    Vec3f fl;
    Vec3f F;
    Vec3f tmp;
    f32 d;
    f32 f;
    s32 (**rnd)(s32);

    c->unk6C3 = 0;
    pos.x = c->pos.x + c->v8C.x;
    pos.y = c->pos.y + c->v8C.y;
    pos.z = c->pos.z + c->v8C.z;
    axis.x = c->rot.m[0][0];
    axis.y = c->rot.m[1][0];
    axis.z = c->rot.m[2][0];
    d = axis.x * c->planeN.x + axis.y * c->planeN.y + axis.z * c->planeN.z;
    if ((d < 0.0f ? -d : d) > 0.7071f) {
        /* TODO(match-debt): the 0x75C/0x760 counters only match through raw casts. */
        *(u32 *)((u8 *)c + 0x75C) += 10;
        if (*(u32 *)((u8 *)c + 0x75C) > D_80138D6D) {
            Vec3f tq;
            f = c->unk5E8 * c->unk5E8 + c->unk5F0 * c->unk5F0;
            if (d > 0.0f) {
                f = -f;
            }
            fl.x = 0.0f;
            fl.y = 0.0f;
            fl.z = f;
            tmp.x = c->rot.m[0][0] * fl.x + c->rot.m[0][1] * fl.y + c->rot.m[0][2] * fl.z; tmp.y = c->rot.m[1][0] * fl.x + c->rot.m[1][1] * fl.y + c->rot.m[1][2] * fl.z; tmp.z = c->rot.m[2][0] * fl.x + c->rot.m[2][1] * fl.y + c->rot.m[2][2] * fl.z; F = tmp; axis.x *= c->scale; axis.y *= c->scale; axis.z *= c->scale; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; tmp.x = pt.x - c->pos.x; tmp.y = pt.y - c->pos.y; tmp.z = pt.z - c->pos.z; tq.x = tmp.y * F.z - F.y * tmp.z; tq.y = F.x * tmp.z - tmp.x * F.z; tq.z = tmp.x * F.y - F.x * tmp.y; c->torque.x = tq.x + c->torque.x; c->torque.y = tq.y + c->torque.y; c->torque.z = tq.z + c->torque.z; c->force.x = c->force.x + F.x; c->force.y = c->force.y + F.y; c->force.z = c->force.z + F.z; axis.x = -axis.x; axis.y = -axis.y; axis.z = -axis.z; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; F.x = -F.x; F.y = -F.y; F.z = -F.z; push_954(c, &pt, &F, &tmp);
        }
    } else {
        *(u32 *)((u8 *)c + 0x75C) = 0;
    }
    axis.x = c->rot.m[0][1];
    axis.y = c->rot.m[1][1];
    axis.z = c->rot.m[2][1];
    d = axis.x * c->planeN.x + axis.y * c->planeN.y + axis.z * c->planeN.z;
    if ((d < 0.0f ? -d : d) > 0.7071f) {
        *(u32 *)((u8 *)c + 0x760) += 10;
        if (*(u32 *)((u8 *)c + 0x760) > D_80138D6E) {
            f = c->unk5EC * c->unk5EC + c->unk5F0 * c->unk5F0;
            if (d > 0.0f) {
                f = -f;
            }
            fl.x = 0.0f;
            fl.y = 0.0f;
            fl.z = f;
            tmp.x = c->rot.m[0][0] * fl.x + c->rot.m[0][1] * fl.y + c->rot.m[0][2] * fl.z; tmp.y = c->rot.m[1][0] * fl.x + c->rot.m[1][1] * fl.y + c->rot.m[1][2] * fl.z; tmp.z = c->rot.m[2][0] * fl.x + c->rot.m[2][1] * fl.y + c->rot.m[2][2] * fl.z; F = tmp; axis.x *= c->scale; axis.y *= c->scale; axis.z *= c->scale; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; push_954(c, &pt, &F, &tmp); axis.x = -axis.x; axis.y = -axis.y; axis.z = -axis.z; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; F.x = -F.x; F.y = -F.y; F.z = -F.z; push_954(c, &pt, &F, &tmp);
        }
    } else {
        *(u32 *)((u8 *)c + 0x760) = 0;
    }
    axis.x = c->rot.m[0][2];
    axis.y = c->rot.m[1][2];
    axis.z = c->rot.m[2][2];
    d = axis.x * c->planeN.x + axis.y * c->planeN.y + axis.z * c->planeN.z;
    if (-d > 0.7071f) {
        if (c->unk764 == 0) {
            rnd = &svcRandRange;
            c->unk768 = (u32)(*rnd)(100) > D_80138D70;
            if (c->unk768) {
                c->unk769 = (*rnd)(2);
            }
        }
        c->unk764 += 10;
        if (c->unk768) {
            if (c->unk764 > D_80138D6F) {
                f = c->unk5E8 * c->unk5E8 + c->unk5F0 * c->unk5F0;
                if (c->unk769) {
                    f = -f;
                }
                fl.x = f;
                fl.y = 0.0f;
                fl.z = 0.0f;
                tmp.x = c->rot.m[0][0] * fl.x + c->rot.m[0][1] * fl.y + c->rot.m[0][2] * fl.z; tmp.y = c->rot.m[1][0] * fl.x + c->rot.m[1][1] * fl.y + c->rot.m[1][2] * fl.z; tmp.z = c->rot.m[2][0] * fl.x + c->rot.m[2][1] * fl.y + c->rot.m[2][2] * fl.z; F = tmp; axis.x *= c->scale; axis.y *= c->scale; axis.z *= c->scale; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; push_954(c, &pt, &F, &tmp); axis.x = -axis.x; axis.y = -axis.y; axis.z = -axis.z; pt.x = pos.x + axis.x; pt.y = pos.y + axis.y; pt.z = pos.z + axis.z; F.x = -F.x; F.y = -F.y; F.z = -F.z; push_954(c, &pt, &F, &tmp);
            }
        } else if (c->unk764 > D_80138D71 * 10) {
            Car_StartCrash(c, 0, 0);
        }
    } else {
        c->unk764 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_DirectionalBoost);

extern void (*svcModelFlashOn)(u8);
/* Fires a turbo: needs two wheels on the ground and some speed; sets the impulse to 2288 along the velocity. */
u8 Car_UseTurbo(u8 *arg) {
    Car *c = (Car *)arg;
    Vec3f v;

    if (c->cnt3E7 < 2) {
        return 0;
    }
    if (c->b4B8 != 0) {
        return 0;
    }
    if (c->fBC < 35.2f) {
        return 0;
    }
    if (c->unk774 != 0 && c->fBC < 176.0f) {
        return 0;
    }
    v = c->vel;
    if ((v.x > 0.0f || v.x < 0.0f) && (v.y > 0.0f || v.y < 0.0f) && (v.z > 0.0f || v.z < 0.0f)) {
        VEC3_NORMALIZE(v);
        VEC3_SCALE(v, v, 2288.0f);
        c->impulse.x = v.x * c->scale;
        c->impulse.y = v.y * c->scale;
        c->impulse.z = v.z * c->scale;
        if (c->flags & 1) {
            audSfxPlay(1, 1, 1.0f, 1.0f);
        }
        svcModelFlashOn(c->id);
        c->b4C0 = 1;
        c->f4C4 = 2288.0f;
        c->fBC = 2288.0f;
        return 1;
    }
    return 0;
}

extern void (*svcModelFlashOn)(u8);
/* Scales the impulse so that the speed becomes 2288 and marks the car (once) as boosted. */
void Car_Boost(Car *c) {
    if (2288.0f - c->fBC < 88.0f || c->fBC < 35.2f) {
        return;
    }
    c->impulse.x *= 2288.0f / c->fBC;
    c->impulse.y *= 2288.0f / c->fBC;
    c->impulse.z *= 2288.0f / c->fBC;
    if (c->b4C0 == 0) {
        if (c->flags & 1) {
            audSfxPlay(1, 1, 1.0f, 1.0f);
        }
        svcModelFlashOn(c->id);
        c->b4C0 = 1;
        c->f4C4 = 2288.0f;
        c->fBC = 2288.0f;
    }
}

typedef Vec3f V3_B00;
typedef struct { V3_B00 n; f32 d; } P4_B00;
typedef CarRange R_B00;
typedef CarPoint W_B00;
typedef struct { void (*a)(u8, f32); void (*b)(u8, u8, P4_B00 *); void (*c)(u8, s32); } T_B00;
extern u8 D_8011B31B;
extern void (*svcModelSetTransform)(u8, V3_B00 *, void *);
extern void (*svcModelSetWheel)(u8, u8, f32, f32, f32, s32, s32, s32, V3_B00 *, V3_B00 *);
extern T_B00 svcModelSetEngineShake;
u32 RaceView_GetPlayerCount(void);
void RaceCam_Get(u8, V3_B00 *, V3_B00 *, V3_B00 *, V3_B00 *, s32 *, u8 *, u8 *);
static inline f32 maxf_B00(f32 a, f32 b) { return a > b ? a : b; }
void Cars_CollideObstacles(u32 dt) {
    V3_B00 d;
    V3_B00 o;
    V3_B00 v1;
    V3_B00 v2;
    V3_B00 v3;
    u8 near[8];
    P4_B00 pl;
    s32 s90;
    u8 s94;
    u8 j;
    u8 cnt;
    u8 i;
    Car *c;
    W_B00 *wh;
    s32 hit;
    f32 h;
    f32 r;
    f32 f;
    R_B00 *rr;

    cnt = D_8011B31B;
    for (i = 0; i < cnt; i++) {
        near[i] = 0;
    }
    for (i = 0; i < RaceView_GetPlayerCount(); i++) {
        RaceCam_Get(i, &o, &v1, &v2, &v3, &s90, &s94, &j);
        for (j = 0; j < cnt; j++) {
            if (near[j] == 0) {
                c = &gCars[j];
                d.x = c->pos.x + c->v8C.x;
                d.y = c->pos.y + c->v8C.y;
                d.z = c->pos.z + c->v8C.z;
                d.x -= o.x;
                d.y -= o.y;
                d.z -= o.z;
                f = ((((((d.x) < 0.0f ? -(d.x) : (d.x))) > ( ((d.y) < 0.0f ? -(d.y) : (d.y))) ? (((d.x) < 0.0f ? -(d.x) : (d.x))) : ( ((d.y) < 0.0f ? -(d.y) : (d.y))))) > ( ((d.z) < 0.0f ? -(d.z) : (d.z))) ? (((((d.x) < 0.0f ? -(d.x) : (d.x))) > ( ((d.y) < 0.0f ? -(d.y) : (d.y))) ? (((d.x) < 0.0f ? -(d.x) : (d.x))) : ( ((d.y) < 0.0f ? -(d.y) : (d.y))))) : ( ((d.z) < 0.0f ? -(d.z) : (d.z))));
                near[j] = f < 2400.0f;
            }
        }
    }
    for (i = 0; i < cnt; i++) {
        V3_B00 w;

        c = &gCars[i];
        o.x = c->pos.x + c->v8C.x;
        o.y = c->pos.y + c->v8C.y;
        o.z = c->pos.z + c->v8C.z;
        w.x = c->rot.m[0][0] * c->center.x + c->rot.m[0][1] * c->center.y + c->rot.m[0][2] * c->center.z;
        w.y = c->rot.m[1][0] * c->center.x + c->rot.m[1][1] * c->center.y + c->rot.m[1][2] * c->center.z;
        w.z = c->rot.m[2][0] * c->center.x + c->rot.m[2][1] * c->center.y + c->rot.m[2][2] * c->center.z;
        d = w;
        d.x = o.x - d.x;
        d.y = o.y - d.y;
        d.z = o.z - d.z;
        svcModelSetTransform(i, &d, (u8 *)&c->rot.m[0][0]);
        rr = &c->r3EC;
        if (near[i]) {
            for (j = 0, wh = &c->points[0].pos; j < c->nPoints; j++, wh++) {
                d.x = wh->dir.y * wh->n.z - wh->n.y * wh->dir.z;
                d.y = wh->n.x * wh->dir.z - wh->dir.x * wh->n.z;
                d.z = wh->dir.x * wh->n.y - wh->n.x * wh->dir.y;
                hit = 0;
                if (wh->b60 && wh->contact && c->b778 == 0) {
                    d.x *= wh->w;
                    d.y *= wh->w;
                    d.z *= wh->w;
                    if (j == 0 || j == 2 || j == 4) {
                        d.x = wh->pt.x + d.x;
                        d.y = wh->pt.y + d.y;
                        d.z = wh->pt.z + d.z;
                    } else {
                        d.x = wh->pt.x - d.x;
                        d.y = wh->pt.y - d.y;
                        d.z = wh->pt.z - d.z;
                    }
                    hit = 1;
                }
                h = 0.0f;
                if (wh->flags & 2) {
                    h = c->yaw;
                }
                if (j >= 2) {
                    h = -h;
                }
                r = wh->h64 - ((wh->flags & 1) ? c->k[21] : c->k[15]);
                if (c->b13C == 0) {
                    wh->ang += wh->rate * ((f32) dt / 1000.0f);
                }
                while (wh->ang < -314.159f) {
                    wh->ang += 314.159f;
                }
                while (wh->ang > 314.159f) {
                    wh->ang -= 314.159f;
                }
                svcModelSetWheel(i, j, h, r, wh->ang, wh->contact, wh->c31, hit, &wh->pt, &d);
            }
        }
        f = 2.0f * ((rr->cur - rr->lo) / (rr->hi - rr->lo)) - 1.0f;
        f = maxf_B00(f, 0.0f);
        if (c->b4B8 != 0) {
            f = 0.0f;
        }
        svcModelSetEngineShake.a(i, f);
        if (c->unk710 != 0) {
            pl.n = c->planeN;
            pl.d = c->unk72C - (pl.n.x * c->planePt.x + pl.n.y * c->planePt.y + pl.n.z * c->planePt.z);
        }
        svcModelSetEngineShake.b(i, c->unk710, &pl);
        svcModelSetEngineShake.c(i, c->flags2);
        c->b778 = 0;
    }
}

/* Replay record of a car: flags, ground state and contact plane (normal x4096 and distance), first two
 * rotation rows (s8, x127) and position. */
typedef struct { u16 flags; u8 b4B8; u8 onPlane; s16 n[3]; s16 d; s8 rot[6]; s16 pos[3]; } CarRec_;

/* Packs the cars into 24-byte records; returns the bytes written, 0 if out (size bytes) is too small. */
u32 Cars_SaveReplay(u8 *buf, u32 size) {
    CarRec_ *out;
    u32 n;
    u32 i;
    Car *c;
    s32 v;
    f32 d;

    n = gCarCount * sizeof(CarRec_);
    if (size < n) {
        return 0;
    }
    out = (CarRec_ *)buf;
    for (c = gCars, i = 0; i < (u32)gCarCount; out++, c++, i++) {
        out->flags = c->flags2;
        out->b4B8 = c->b4B8;
        if ((out->onPlane = c->unk710) != 0) {
            d = c->unk72C - (c->planeN.x * c->planePt.x + c->planeN.y * c->planePt.y + c->planeN.z * c->planePt.z);
            out->n[0] = c->planeN.x * 4096.0f;
            out->n[1] = c->planeN.y * 4096.0f;
            out->n[2] = c->planeN.z * 4096.0f;
            out->d = d;
        }
        v = c->rot.m[0][0] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[0] = v;
        v = c->rot.m[0][1] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[1] = v;
        v = c->rot.m[0][2] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[2] = v;
        v = c->rot.m[1][0] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[3] = v;
        v = c->rot.m[1][1] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[4] = v;
        v = c->rot.m[1][2] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[5] = v;
        out->pos[0] = c->pos.x;
        out->pos[1] = c->pos.y;
        out->pos[2] = c->pos.z;
    }
    return n;
}

extern void (*svcModelRepair)(u8);

/* Restores the cars from Cars_SaveReplay records: flags, wreck state, contact plane, rotation (third
 * row rebuilt as a cross product) and position; clears the motion state. */
void Cars_LoadReplay(u8 *buf) {
    V3_1B0 zero = { 0.0f, 0.0f, 0.0f };
    CarRec_ *in;
    Car *c;
    u32 i;
    u32 j;

    in = (CarRec_ *)buf;
    for (c = gCars, i = 0; i < (u32)gCarCount; in++, c++, i++) {
        c->flags2 = in->flags | 8;
        c->unk774 = 0;
        if (in->b4B8 != 0) {
            svcModelWreck(i, 2, zero);
        } else {
            svcModelRepair(i);
        }
        if ((c->unk710 = in->onPlane) != 0) {
            c->planeN.x = in->n[0] / 4096.0f;
            c->planeN.y = in->n[1] / 4096.0f;
            c->planeN.z = in->n[2] / 4096.0f;
            c->unk72C = in->d;
            c->planePt.x = 0.0f;
            c->planePt.y = 0.0f;
            c->planePt.z = 0.0f;
        }
        c->rot.m[0][0] = in->rot[0] / 127.0f;
        c->rot.m[0][1] = in->rot[1] / 127.0f;
        c->rot.m[0][2] = in->rot[2] / 127.0f;
        c->rot.m[1][0] = in->rot[3] / 127.0f;
        c->rot.m[1][1] = in->rot[4] / 127.0f;
        c->rot.m[1][2] = in->rot[5] / 127.0f;
        VEC3_CROSS(*(Vec3f *)c->rot.m[2], *(Vec3f *)c->rot.m[0], *(Vec3f *)c->rot.m[1]);
        c->pos.x = in->pos[0];
        c->pos.y = in->pos[1];
        c->pos.z = in->pos[2];
        c->yaw = 0.0f;
        c->b13C = 0;
        for (j = 0; j < c->nPoints; j++) {
            c->points[j].rate = 0.0f;
            c->points[j].h64 = 0.0f;
        }
        svcModelFlashOff(i);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_31E10", Car_Unstick);

s32 Rumble_Init(void) {
    memset(gImpactCooldown, 0, 8);
    return 1;
}


s32 Rumble_Shutdown(void) {
    return 1;
}

s32 Race_PollPauseButton(void) {
    u8 a;
    u8 b;
    u8 i;
    s32 n;
    void *e;
    void (**p)(s32);

    n = 0;
    for (i = 0; i < 2; i++) {
        if (((s32 *) gImpactCooldown)[i] != 0) {
            ((s32 *) gImpactCooldown)[i]--;
        }
    }
    for (i = 0; i < 2; i++) {
        e = Car_Get(i);
        if (e != NULL && (((s32 *) e)[1] & 1)) {
            if (Race_GetPhase() == 1) {
                Car_GetSpeedAndGrip(i, &a, &b);
                sysInputHasRumble(i, a, b);
            }
            n++;
        }
    }
    p = &sysInputUpdate;
    (*p)(0);
    (*p)(1);
    if (n == 2) {
        if ((u8) sysInputGetControl(0, 0xD)) {
            return 1;
        }
        if ((u8) sysInputGetControl(1, 0xD)) {
            return 2;
        }
    } else if (n == 1) {
        if ((u8) sysInputGetControl(0, 0xD)) {
            return 1;
        }
    }
    return 0;
}

typedef struct { u8 pad0[0x64]; Car *car; } HitObj98_;
typedef struct { HitObj98_ *obj; u8 pad4[0xC]; Vec3f dir; } HitEnt98_;
/* On a hard hit against dir, sends the impact strength (0..255) once every 20 frames. */
void Car_ReportImpact(HitEnt98_ *e) {
    Car *c = e->obj->car;
    f32 d;
    s32 v;

    if (((s32 *)gImpactCooldown)[c->id] == 0) {
        d = -(c->vel.x * e->dir.x + c->vel.y * e->dir.y + c->vel.z * e->dir.z);
        if (d > 352.0f) {
            v = d / 2304.0f * 255.0f;
            if (v < 0) {
                v = 0;
            }
            if (v > 255) {
                v = 255;
            }
            sysInputRumble(c->id, v);
            ((s32 *)gImpactCooldown)[c->id] = 20;
        }
    }
}

typedef struct { u8 pad0[0x84]; Vec3f vel; } PairBody98_;
typedef struct { u8 pad0[4]; s32 flags; u8 pad8[0x58]; PairBody98_ *body; Car *car; } PairObj98_;
extern u8 D_80138D68;
extern u8 D_80138D69;
f32 sqrtf(f32);
/* Rumble for a car-vs-object contact: fixed strengths against flagged objects (2/4), otherwise
   scaled by the relative speed, at most once every 20 frames. */
void Car_ReportPairImpact(PairObj98_ **pair, u8 side) {
    PairObj98_ *a;
    PairObj98_ *b;
    Car *car;
    Vec3f d;
    f32 dist;
    f32 f;
    s32 v;

    a = side == 0 ? pair[0] : pair[1];
    b = side == 0 ? pair[1] : pair[0];
    car = a->car;
    if (b->flags & 6) {
        if (b->flags & 2) {
            v = D_80138D68;
        } else {
            v = D_80138D69;
        }
        sysInputRumble(car->id, v);
        return;
    }
    if (((s32 *)gImpactCooldown)[car->id] == 0) {
        if (b->body != NULL) {
            d.x = car->vel.x - b->body->vel.x;
            d.y = car->vel.y - b->body->vel.y;
            d.z = car->vel.z - b->body->vel.z;
        } else {
            d = car->vel;
        }
        dist = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        if (dist > 352.0f) {
            f = dist / 2304.0f;
            f = (f < 1.0f) ? f : 1.0f;
            f *= 255.0f;
            v = f;
            if (v < 0) {
                v = 0;
            }
            if (v > 255) {
                v = 255;
            }
            sysInputRumble(car->id, v);
            ((s32 *)gImpactCooldown)[car->id] = 20;
        }
    }
}
