#include "common.h"
#include "include_asm.h"
#include "car.h"
#include "race.h"
typedef struct { Vec3f p; u8 pad[0x7C]; } BlkD_;
typedef struct { u8 id; u8 p1[0xBF]; f32 m[3][3]; } CarD_;
typedef struct {
    /* 0x000 */ u8 id;
    /* 0x001 */ u8 p1[3];
    /* 0x004 */ u8 *car;
    /* 0x008 */ union {
        struct { u8 p8[0x1C]; f32 f24; } s;
        BlkD_ blk;
    } u8_;
    /* 0x090 */ u8 ok;
    /* 0x091 */ u8 p91[3];
    /* 0x094 */ Vec3f v94[4];
    /* 0x0C4 */ f32 m[3][3];
    /* 0x0E8 */ Vec3f h1[10];
    /* 0x160 */ s32 n160;
    /* 0x164 */ Vec3f h2[10];
    /* 0x1DC */ s32 n1DC;
    /* 0x1E0 */ Vec3f v1E0;
    /* 0x1EC */ Vec3f v1EC;
    /* 0x1F8 */ s32 z1F8;
    /* 0x1FC */ f32 f1FC;
    /* 0x200 */ s32 z200;
    /* 0x204 */ s32 z204;
    /* 0x208 */ u8 braking;
    /* 0x209 */ u8 p209[3];
    /* 0x20C */ f32 f20C;
    /* 0x210 */ u8 p210[4];
    /* 0x214 */ s32 z214;
    /* 0x218 */ f32 f218;
    /* 0x21C */ Vec3f v21C;
    /* 0x228 */ Vec3f v228;
    /* 0x234 */ Vec3f v234;
    /* 0x240 */ Vec3f v240;
    /* 0x24C */ s32 ctlTimer;
    /* 0x250 */ f32 throttle;
    /* 0x254 */ f32 steer;
    /* 0x258 */ f32 f258;
    /* 0x25C */ f32 f25C;
    /* 0x260 */ f32 f260;
    /* 0x264 */ u32 u264;
    /* 0x268 */ s32 z268;
    /* 0x26C */ s32 z26C;
    /* 0x270 */ u8 p270[0x30];
    /* 0x2A0 */ s32 s2A0;
    /* 0x2A4 */ u8 b2A4;
    /* 0x2A5 */ u8 b2A5;
    /* 0x2A6 */ u8 p2A6[2];
} Slot85_;
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
#ifndef NON_MATCHING
extern Slot85_ gAi[];
#endif
extern s32 D_80115824;
extern f32 D_80115820;
void Ai_ApplyImpulse(Car *);
extern u8 D_80114838;

/* TODO(match-debt): 3037/3280 words (13072/13120 B, delta -48 B, frame 0x3C8 exact, s68).
 * Loops 1-2, 5-16, 20-21 match ~100%; remaining -48 B / 243w diff is concentrated in Loop 17
 * (rollM identity/RotZ register scheduling) and Loop 18-19 (divu vs andi in Ai_Rand_Step_(2) and
 * skewM identity zeroing). */
#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 c;
    /* 0x10 */ f32 d10;
    /* 0x14 */ f32 ang;
    /* 0x18 */ u32 flags;
    /* 0x1C */ f32 f1C;
    /* 0x20 */ f32 f20;
    /* 0x24 */ s32 n;
    /* 0x28 */ u32 w[2];
    /* 0x30 */ u8 nm[2][16];
    /* 0x50 */ s32 sel;
    /* 0x54 */ Vec3f p0;
    /* 0x60 */ Vec3f p1;
    /* 0x6C */ f32 a0;
    /* 0x70 */ f32 b0;
    /* 0x74 */ f32 a1;
    /* 0x78 */ f32 b1;
    /* 0x7C */ u16 idx;
    /* 0x7E */ u8 p7E[2];
    /* 0x80 */ f32 dur;
    /* 0x84 */ f32 time;
} TrkAi_; /* size 0x88 */

typedef struct {
    /* 0x000 */ u8 id;
    /* 0x001 */ u8 p1[3];
    /* 0x004 */ Car *car;
    /* 0x008 */ TrkAi_ blk;
    /* 0x090 */ u8 ok;
    /* 0x091 */ u8 p91[3];
    /* 0x094 */ Vec3f pos;
    /* 0x0A0 */ Vec3f prevPos;
    /* 0x0AC */ Vec3f centerPos;
    /* 0x0B8 */ Vec3f prevCenterPos;
    /* 0x0C4 */ f32 m[3][3];
    /* 0x0E8 */ Vec3f h1[10];
    /* 0x160 */ u32 n160;
    /* 0x164 */ Vec3f h2[10];
    /* 0x1DC */ u32 n1DC;
    /* 0x1E0 */ Vec3f v1E0;
    /* 0x1EC */ Vec3f v1EC;
    /* 0x1F8 */ f32 latPos;
    /* 0x1FC */ f32 latTarget;
    /* 0x200 */ f32 latVel;
    /* 0x204 */ f32 latAcc;
    /* 0x208 */ u8 braking;
    /* 0x209 */ u8 p209[3];
    /* 0x20C */ f32 f20C;
    /* 0x210 */ f32 stepDist;
    /* 0x214 */ f32 rampScale;
    /* 0x218 */ f32 f218;
    /* 0x21C */ Vec3f impVel;
    /* 0x228 */ Vec3f impForce;
    /* 0x234 */ Vec3f angVel;
    /* 0x240 */ Vec3f angTorque;
    /* 0x24C */ s32 ctlTimer;
    /* 0x250 */ f32 throttle;
    /* 0x254 */ f32 steer;
    /* 0x258 */ f32 minRubber;
    /* 0x25C */ f32 maxRubber;
    /* 0x260 */ f32 curRubber;
    /* 0x264 */ u32 rubberTime;
    /* 0x268 */ f32 rollTarget;
    /* 0x26C */ f32 rollCur;
    /* 0x270 */ s32 rollDir;
    /* 0x274 */ u16 stuntFlags;
    /* 0x276 */ u8 p276[2];
    /* 0x278 */ f32 stuntDur;
    /* 0x27C */ f32 stuntM[3][3];
    /* 0x2A0 */ u32 crashChance;
    /* 0x2A4 */ u8 b2A4;
    /* 0x2A5 */ u8 b2A5;
    /* 0x2A6 */ u8 p2A6[2];
} SlotAi_; /* size 0x2A8 */
extern SlotAi_ gAi[];

extern f32 D_80115834;
extern f32 D_8011583C;
extern f32 D_80115840;
extern u8 D_80115844;
extern s8 gAiPhysicsCarCount;
extern Car *gAiPhysicsCar0;
typedef struct {
    u8 b0; /* D_80138D51.b0 */
    u8 b1; /* D_80138D51.b1 */
    u8 b2; /* D_80138D51.b2 */
    u8 b3; /* D_80138D51.b3 */
    u8 b4; /* D_80138D51.b4 */
    u8 b5; /* D_80138D51.b5 */
    u8 b6; /* D_80138D51.b6 */
} AiCfg_;
extern AiCfg_ D_80138D51;

typedef struct { f32 x, y, z, d; } PlaneAi_;
u8 Bld_StepRacer(TrkAi_ *a, f32 dt, u8 flag, PlaneAi_ n);
void Mtx3_Orthonormalize(void *);
void Car_StartCrash(Car *p, u8 id, u8 flip);
f32 cosf(f32);
f32 sinf(f32);

#define FABS_(x) ((x) < 0.0f ? -(x) : (x))

static inline u32 Ai_Rand_Step_(u32 n) {
    D_80115854 = D_80115854 * 0x19660D + 0x3C6EF35F;
    return D_80115854 % n;
}

void Ai_Step(u32 dtMs) {
    Vec3f vtmp;
    Vec3f angAcc[6];
    f32 tmpM[6][3][3];
    f32 invM[6][3][3];
    f32 skewRes[3][3];
    f32 skewM[3][3];
    Vec3f t1, t2, t3, t4, t5;
    f32 res4[3][4];
    s32 cand[2];
    s32 weight[2];
    f32 rollM[3][4];
    Vec3f diff, proj;
    s32 i, j, k;
    Car *car;
    SlotAi_ *ai;
    s32 totalWeight = 0;
    s32 numFree = 0;
    f32 dt = (f32)dtMs / 1000.0f;

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (ai->b2A5 != 0 && car->finished != 0) {
            f32 spin = (f32)(Ai_Rand_Step_(5) - 4);
            ai->b2A5 = 0;
            car->impulse.x = ai->pos.x - ai->prevPos.x;
            car->impulse.y = ai->pos.y - ai->prevPos.y;
            car->impulse.z = ai->pos.z - ai->prevPos.z;
            car->impulse.x *= car->scale / dt;
            car->impulse.y *= car->scale / dt;
            car->impulse.z *= car->scale / dt;
            car->vE4.x = car->unk748 * (spin * car->scale);
            car->vE4.y = car->unk74C * (spin * car->scale);
            car->vE4.z = car->unk750 * (spin * car->scale);
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            ai->prevPos = ai->pos;
            ai->prevCenterPos = ai->centerPos;
            ai->throttle *= 0.99f;
            ai->steer *= 0.99f;
            ai->stepDist = (f32)dtMs * ((ai->f20C + 100.0f) / 100.0f);
            ai->stepDist *= ai->curRubber;
            ai->stepDist *= ((ai->throttle + 100.0f) / 100.0f);
            if (ai->rampScale < 1.0f) {
                ai->rampScale += 0.05f;
                ai->stepDist *= ai->rampScale;
            }
            if (ai->stepDist < 5.0f) {
                ai->stepDist = 5.0f;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            f32 h;
            while (!Bld_StepRacer(&ai->blk, ai->stepDist, car->b744, *(PlaneAi_ *)&car->unk748)) {
                for (j = 0; j < 2; j++) {
                    weight[j] = 0;
                    cand[j] = 0;
                }
                for (j = 0; j < ai->blk.n; j++) {
                    if ((u16)ai->blk.w[j] != 0) {
                        cand[j] = 2;
                    } else {
                        cand[j] = 1;
                        numFree++;
                    }
                }
                if (numFree != 0) {
                    for (j = 0; j < 2; j++) {
                        if (cand[j] == 2 && (car->w540 & (u16)ai->blk.w[j]) == 0) {
                            cand[j] = 0;
                        }
                    }
                    for (j = 0; j < 2; j++) {
                        if (cand[j] != 0) {
                            if (ai->latPos > 0.0f && (ai->blk.w[j] & 0x20000000) != 0) {
                                weight[j] += 20;
                            } else if (ai->latPos < 0.0f && (ai->blk.w[j] & 0x40000000) != 0) {
                                weight[j] += 20;
                            }
                        }
                        if (cand[j] == 2) {
                            s32 best = 0;
                            for (k = 0; k < 16; k++) {
                                if ((car->w540 & (1 << k) & ai->blk.w[j]) != 0) {
                                    if (ai->blk.nm[j][k] > best) {
                                        best = ai->blk.nm[j][k];
                                    }
                                }
                            }
                            weight[j] += best;
                        }
                    }
                    for (j = 0; j < 2; j++) {
                        totalWeight += weight[j];
                    }
                    totalWeight = (totalWeight < 256) ? totalWeight : 255;
                    for (j = 0; j < 2; j++) {
                        if (cand[j] == 1) {
                            weight[j] += (255 - totalWeight) / numFree;
                        }
                    }
                    ai->blk.sel = 0;
                    {
                        s32 r = Ai_Rand_Step_(255);
                        for (j = 0; j < 2; j++) {
                            if (r < weight[j]) {
                                ai->blk.sel = j;
                                break;
                            }
                            r -= weight[j];
                        }
                    }
                }
            }
            vtmp.x = ai->m[0][2];
            vtmp.y = ai->m[1][2];
            vtmp.z = ai->m[2][2];
            h = (car->k[15] > car->k[21]) ? car->k[15] : car->k[21];
            vtmp.x = vtmp.x * (car->center.z + h - 36.0f);
            vtmp.y = vtmp.y * (car->center.z + h - 36.0f);
            vtmp.z = vtmp.z * (car->center.z + h - 36.0f);
            ai->centerPos.x = ai->blk.pos.x + vtmp.x;
            ai->centerPos.y = ai->blk.pos.y + vtmp.y;
            ai->centerPos.z = ai->blk.pos.z + vtmp.z;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 && (ai->blk.flags & 2) == 0) {
            f32 tgt, lim;
            if (ai->ctlTimer == 2) {
                if (FABS_(ai->steer) < 1.0f) {
                    ai->steer = 0.0f;
                    ai->ctlTimer = 0;
                }
                if (ai->braking != 0) {
                    ai->braking = 0;
                    ai->latTarget = -ai->latTarget;
                }
            } else if (ai->ctlTimer == 0) {
                if (FABS_(ai->latPos - ai->latTarget) < 1.0f) {
                    ai->latTarget = (s32)Ai_Rand_Step_(201) - 100;
                    if (FABS_(ai->latPos) < (f32)D_80138D51.b1) {
                        if (ai->latPos < 0.0f) {
                            ai->latPos = -(f32)D_80138D51.b1;
                        } else {
                            ai->latPos = (f32)D_80138D51.b1;
                        }
                    }
                }
            }
            tgt = ai->latTarget + ai->steer;
            lim = ai->blk.c - (f32)D_80138D51.b0;
            tgt = (tgt < lim) ? tgt : lim;
            lim = -ai->blk.d10 + (f32)D_80138D51.b0;
            tgt = (tgt > lim) ? tgt : lim;
            ai->latAcc = tgt - (ai->latPos + ai->latVel);
            ai->latAcc = (ai->latAcc < 200.0f) ? ai->latAcc : 200.0f;
            ai->latAcc = (ai->latAcc > -200.0f) ? ai->latAcc : -200.0f;
            ai->latVel += dt * ai->latAcc;
            ai->latVel = (ai->latVel < 40.0f) ? ai->latVel : 40.0f;
            ai->latVel = (ai->latVel > -40.0f) ? ai->latVel : -40.0f;
            ai->latPos += dt * ai->latVel;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            t4.x = ai->m[0][0];
            t4.y = ai->m[1][0];
            t4.z = ai->m[2][0];
            ai->latPos = (ai->latPos > ai->blk.c) ? ai->blk.c : ai->latPos;
            ai->latPos = (ai->latPos < -ai->blk.d10) ? -ai->blk.d10 : ai->latPos;
            t4.x = t4.x * ai->latPos;
            t4.y = t4.y * ai->latPos;
            t4.z = t4.z * ai->latPos;
            ai->pos.x = ai->centerPos.x + t4.x;
            ai->pos.y = ai->centerPos.y + t4.y;
            ai->pos.z = ai->centerPos.z + t4.z;

            t5.x = ai->impForce.x * (dt / car->k[0]);
            t5.y = ai->impForce.y * (dt / car->k[0]);
            t5.z = ai->impForce.z * (dt / car->k[0]);
            ai->impVel.x += t5.x;
            ai->impVel.y += t5.y;
            ai->impVel.z += t5.z;
            ai->impVel.x *= 0.95f;
            ai->impVel.z *= 0.95f;
            ai->impVel.y *= 0.95f;
            ai->impForce.x *= 0.95f;
            ai->impForce.y *= 0.95f;
            ai->impForce.z *= 0.95f;
            ai->pos.x += ai->impVel.x;
            ai->pos.y += ai->impVel.y;
            ai->pos.z += ai->impVel.z;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            (ai->stuntFlags & 2) == 0 && (ai->blk.flags & 2) == 0) {
            f32 inv, d;
            diff.x = ai->pos.x - ai->centerPos.x;
            diff.y = ai->pos.y - ai->centerPos.y;
            diff.z = ai->pos.z - ai->centerPos.z;
            proj.x = car->unk748 * (car->unk748 * diff.x + car->unk74C * diff.y + car->unk750 * diff.z);
            proj.y = car->unk74C * (car->unk748 * diff.x + car->unk74C * diff.y + car->unk750 * diff.z);
            proj.z = car->unk750 * (car->unk748 * diff.x + car->unk74C * diff.y + car->unk750 * diff.z);
            proj.x = diff.x - proj.x;
            proj.y = diff.y - proj.y;
            proj.z = diff.z - proj.z;
            t2.x = ai->prevCenterPos.x - ai->centerPos.x;
            t2.y = ai->prevCenterPos.y - ai->centerPos.y;
            t2.z = ai->prevCenterPos.z - ai->centerPos.z;
            t4.x = t2.y * car->unk750 - car->unk74C * t2.z;
            t4.y = car->unk748 * t2.z - t2.x * car->unk750;
            t4.z = t2.x * car->unk74C - car->unk748 * t2.y;
            inv = 1.0f / sqrtf(t4.x * t4.x + t4.y * t4.y + t4.z * t4.z);
            t4.x *= inv;
            t4.y *= inv;
            t4.z *= inv;
            d = t4.x * proj.x + t4.y * proj.y + t4.z * proj.z;
            if (d < -ai->blk.d10) {
                proj.x *= -ai->blk.d10 / d;
                proj.y *= -ai->blk.d10 / d;
                proj.z *= -ai->blk.d10 / d;
            } else if (d > ai->blk.c) {
                proj.x *= ai->blk.c / d;
                proj.y *= ai->blk.c / d;
                proj.z *= ai->blk.c / d;
            }
            ai->pos.x = ai->centerPos.x + proj.x;
            ai->pos.y = ai->centerPos.y + proj.y;
            ai->pos.z = ai->centerPos.z + proj.z;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            f32 (*ib)[3] = (f32 (*)[3])car->pad050;
            invM[i][0][0] = car->rot.m[0][0]; invM[i][0][1] = car->rot.m[1][0]; invM[i][0][2] = car->rot.m[2][0];
            invM[i][1][0] = car->rot.m[0][1]; invM[i][1][1] = car->rot.m[1][1]; invM[i][1][2] = car->rot.m[2][1];
            invM[i][2][0] = car->rot.m[0][2]; invM[i][2][1] = car->rot.m[1][2]; invM[i][2][2] = car->rot.m[2][2];
            tmpM[i][0][0] = car->rot.m[0][0] * ib[0][0] + car->rot.m[0][1] * ib[1][0] + car->rot.m[0][2] * ib[2][0];
            tmpM[i][0][1] = car->rot.m[0][0] * ib[0][1] + car->rot.m[0][1] * ib[1][1] + car->rot.m[0][2] * ib[2][1];
            tmpM[i][0][2] = car->rot.m[0][0] * ib[0][2] + car->rot.m[0][1] * ib[1][2] + car->rot.m[0][2] * ib[2][2];
            tmpM[i][1][0] = car->rot.m[1][0] * ib[0][0] + car->rot.m[1][1] * ib[1][0] + car->rot.m[1][2] * ib[2][0];
            tmpM[i][1][1] = car->rot.m[1][0] * ib[0][1] + car->rot.m[1][1] * ib[1][1] + car->rot.m[1][2] * ib[2][1];
            tmpM[i][1][2] = car->rot.m[1][0] * ib[0][2] + car->rot.m[1][1] * ib[1][2] + car->rot.m[1][2] * ib[2][2];
            tmpM[i][2][0] = car->rot.m[2][0] * ib[0][0] + car->rot.m[2][1] * ib[1][0] + car->rot.m[2][2] * ib[2][0];
            tmpM[i][2][1] = car->rot.m[2][0] * ib[0][1] + car->rot.m[2][1] * ib[1][1] + car->rot.m[2][2] * ib[2][1];
            tmpM[i][2][2] = car->rot.m[2][0] * ib[0][2] + car->rot.m[2][1] * ib[1][2] + car->rot.m[2][2] * ib[2][2];
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            f32 (*iw)[3] = (f32 (*)[3])car->pad0F0;
            iw[0][0] = tmpM[i][0][0] * invM[i][0][0] + tmpM[i][0][1] * invM[i][1][0] + tmpM[i][0][2] * invM[i][2][0];
            iw[0][1] = tmpM[i][0][0] * invM[i][0][1] + tmpM[i][0][1] * invM[i][1][1] + tmpM[i][0][2] * invM[i][2][1];
            iw[0][2] = tmpM[i][0][0] * invM[i][0][2] + tmpM[i][0][1] * invM[i][1][2] + tmpM[i][0][2] * invM[i][2][2];
            iw[1][0] = tmpM[i][1][0] * invM[i][0][0] + tmpM[i][1][1] * invM[i][1][0] + tmpM[i][1][2] * invM[i][2][0];
            iw[1][1] = tmpM[i][1][0] * invM[i][0][1] + tmpM[i][1][1] * invM[i][1][1] + tmpM[i][1][2] * invM[i][2][1];
            iw[1][2] = tmpM[i][1][0] * invM[i][0][2] + tmpM[i][1][1] * invM[i][1][2] + tmpM[i][1][2] * invM[i][2][2];
            iw[2][0] = tmpM[i][2][0] * invM[i][0][0] + tmpM[i][2][1] * invM[i][1][0] + tmpM[i][2][2] * invM[i][2][0];
            iw[2][1] = tmpM[i][2][0] * invM[i][0][1] + tmpM[i][2][1] * invM[i][1][1] + tmpM[i][2][2] * invM[i][2][1];
            iw[2][2] = tmpM[i][2][0] * invM[i][0][2] + tmpM[i][2][1] * invM[i][1][2] + tmpM[i][2][2] * invM[i][2][2];
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            f32 (*iw)[3] = (f32 (*)[3])car->pad0F0;
            diff.x = iw[0][0] * ai->angTorque.x + iw[0][1] * ai->angTorque.y + iw[0][2] * ai->angTorque.z;
            diff.y = iw[1][0] * ai->angTorque.x + iw[1][1] * ai->angTorque.y + iw[1][2] * ai->angTorque.z;
            diff.z = iw[2][0] * ai->angTorque.x + iw[2][1] * ai->angTorque.y + iw[2][2] * ai->angTorque.z;
            angAcc[car->id] = diff;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            t1.x = angAcc[car->id].x * dt;
            t1.y = angAcc[car->id].y * dt;
            t1.z = angAcc[car->id].z * dt;
            ai->angVel.x += t1.x;
            ai->angVel.y += t1.y;
            ai->angVel.z += t1.z;
            ai->angTorque.x *= 0.95f;
            ai->angTorque.y *= 0.95f;
            ai->angTorque.z *= 0.95f;
            ai->angVel.x *= 0.95f;
            ai->angVel.y *= 0.95f;
            ai->angVel.z *= 0.95f;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->finished != 0) {
            car->unk014 = 0.0f;
            car->r3EC.cur = car->r3EC.lo;
        }
        if (car->b4B8 == 0 && car->finished == 0 && gAi[i].car->state == 1 && ai->ok != 0) {
            f32 invDt = 1.0f / dt;
            car->pos = gAi[car->id].pos;
            t5.x = gAi[car->id].centerPos.x - gAi[car->id].prevCenterPos.x;
            t5.y = gAi[car->id].centerPos.y - gAi[car->id].prevCenterPos.y;
            t5.z = gAi[car->id].centerPos.z - gAi[car->id].prevCenterPos.z;
            car->vel.x = t5.x * invDt;
            car->vel.y = t5.y * invDt;
            car->vel.z = t5.z * invDt;
            car->fBC = sqrtf(car->vel.x * car->vel.x + car->vel.y * car->vel.y + car->vel.z * car->vel.z);
            if (car->fBC > 2304.0f) {
                car->vel.x *= 2304.0f / car->fBC;
                car->vel.y *= 2304.0f / car->fBC;
                car->vel.z *= 2304.0f / car->fBC;
                car->fBC = 2304.0f;
            }
            car->r3EC.cur = car->r3EC.lo + (car->fBC / 2288.0f) * (car->r3EC.hi - car->r3EC.lo);
            car->unk014 = (car->r3EC.cur - car->r3EC.lo) / (car->r3EC.hi - car->r3EC.lo);
            car->cnt3E7 = ((gAi[car->id].blk.flags & 2) == 0) ? car->nPoints : 0;
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            ai->rollTarget = (ai->blk.ang * dt) * ((car->fBC / 2304.0f) * (car->scale / 10.0f)) * 50.0f;
            ai->rollTarget = (ai->rollTarget < 0.349f) ? ai->rollTarget : 0.349f;
            ai->rollTarget = (ai->rollTarget > -0.349f) ? ai->rollTarget : -0.349f;
            if (ai->rollCur < 0.0f) {
                if (ai->rollTarget > ai->rollCur) {
                    if (ai->rollDir > 0) {
                        ai->rollCur += dt * 0.65f;
                    }
                    ai->rollDir = 1;
                    if (ai->rollCur > ai->rollTarget) {
                        ai->rollCur = ai->rollTarget;
                    }
                } else if (ai->rollTarget < ai->rollCur) {
                    if (ai->rollDir < 0) {
                        ai->rollCur -= dt * 0.15f;
                    }
                    ai->rollDir = -1;
                    if (ai->rollCur < ai->rollTarget) {
                        ai->rollCur = ai->rollTarget;
                    }
                }
            } else if (ai->rollTarget > ai->rollCur) {
                if (ai->rollDir > 0) {
                    ai->rollCur += dt * 0.15f;
                }
                ai->rollDir = 1;
                if (ai->rollCur > ai->rollTarget) {
                    ai->rollCur = ai->rollTarget;
                }
            } else if (ai->rollTarget < ai->rollCur) {
                if (ai->rollDir < 0) {
                    ai->rollCur -= dt * 0.65f;
                }
                ai->rollDir = -1;
                if (ai->rollCur < ai->rollTarget) {
                    ai->rollCur = ai->rollTarget;
                }
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            if ((ai->stuntFlags & 2) != 0) {
                ai->v1EC.x = car->rot.m[0][1];
                ai->v1EC.y = car->rot.m[1][1];
                ai->v1EC.z = car->rot.m[2][1];
            } else {
                if (FABS_(car->vel.x) < 1.0f && FABS_(car->vel.y) < 1.0f && FABS_(car->vel.z) < 1.0f) {
                    ai->v1EC.x = ai->m[0][1];
                    ai->v1EC.y = ai->m[1][1];
                    ai->v1EC.z = ai->m[2][1];
                } else {
                    ai->v1EC = car->vel;
                }
            }
            if ((ai->stuntFlags & 2) != 0 || (ai->blk.flags & 2) != 0) {
                ai->v1E0.x = car->rot.m[0][2];
                ai->v1E0.y = car->rot.m[1][2];
                ai->v1E0.z = car->rot.m[2][2];
            } else if (car->b744 != 0) {
                ai->v1E0 = *(Vec3f *)&car->unk748;
            } else {
                ai->v1E0.x = 0.0f;
                ai->v1E0.y = 0.0f;
                ai->v1E0.z = 1.0f;
            }
            ai->h2[ai->n1DC] = ai->v1EC;
            ai->n1DC++;
            if (ai->n1DC > 9) {
                ai->n1DC = 0;
            }
            ai->h1[ai->n160] = ai->v1E0;
            ai->n160++;
            if (ai->n160 > 9) {
                ai->n160 = 0;
            }
            if ((ai->stuntFlags & 2) == 0 && (ai->blk.flags & 2) == 0) {
                ai->v1EC.x = 0.0f;
                ai->v1EC.y = 0.0f;
                ai->v1EC.z = 0.0f;
                for (j = 0; j < 10; j++) {
                    t1.x = ai->h2[j].x * 0.1f;
                    t1.y = ai->h2[j].y * 0.1f;
                    t1.z = ai->h2[j].z * 0.1f;
                    ai->v1EC.x += t1.x;
                    ai->v1EC.y += t1.y;
                    ai->v1EC.z += t1.z;
                }
                ai->v1E0.x = 0.0f;
                ai->v1E0.y = 0.0f;
                ai->v1E0.z = 0.0f;
                for (j = 0; j < 10; j++) {
                    t1.x = ai->h1[j].x * 0.1f;
                    t1.y = ai->h1[j].y * 0.1f;
                    t1.z = ai->h1[j].z * 0.1f;
                    ai->v1E0.x += t1.x;
                    ai->v1E0.y += t1.y;
                    ai->v1E0.z += t1.z;
                }
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            (ai->stuntFlags & 2) == 0 && (ai->blk.flags & 2) == 0) {
            t2 = ai->v1EC;
            t3 = ai->v1E0;
            if ((t3.x < 0.0f || t3.x > 0.0f || t3.y < 0.0f || t3.y > 0.0f || t3.z < 0.0f || t3.z > 0.0f) &&
                (t2.x < 0.0f || t2.x > 0.0f || t2.y < 0.0f || t2.y > 0.0f || t2.z < 0.0f || t2.z > 0.0f)) {
                f32 inv;
                inv = 1.0f / sqrtf(t3.x * t3.x + t3.y * t3.y + t3.z * t3.z);
                t3.x *= inv; t3.y *= inv; t3.z *= inv;
                inv = 1.0f / sqrtf(t2.x * t2.x + t2.y * t2.y + t2.z * t2.z);
                t2.x *= inv; t2.y *= inv; t2.z *= inv;
                t4.x = t2.y * t3.z - t3.y * t2.z;
                t4.y = t3.x * t2.z - t2.x * t3.z;
                t4.z = t2.x * t3.y - t3.x * t2.y;
                t2.x = t3.y * t4.z - t4.y * t3.z;
                t2.y = t4.x * t3.z - t3.x * t4.z;
                t2.z = t3.x * t4.y - t4.x * t3.y;
                ai->m[0][0] = t4.x; ai->m[1][0] = t4.y; ai->m[2][0] = t4.z;
                ai->m[0][1] = t2.x; ai->m[1][1] = t2.y; ai->m[2][1] = t2.z;
                ai->m[0][2] = t3.x; ai->m[1][2] = t3.y; ai->m[2][2] = t3.z;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            (ai->stuntFlags & 2) == 0 && (ai->blk.flags & 2) == 0) {
            skewM[0][0] = 0.0f;            skewM[0][1] = -ai->angVel.z; skewM[0][2] = ai->angVel.y;
            skewM[1][0] = ai->angVel.z; skewM[1][1] = 0.0f;             skewM[1][2] = -ai->angVel.x;
            skewM[2][0] = -ai->angVel.y; skewM[2][1] = ai->angVel.x; skewM[2][2] = 0.0f;
            skewRes[0][0] = skewM[0][0] * ai->m[0][0] + skewM[0][1] * ai->m[1][0] + skewM[0][2] * ai->m[2][0];
            skewRes[0][1] = skewM[0][0] * ai->m[0][1] + skewM[0][1] * ai->m[1][1] + skewM[0][2] * ai->m[2][1];
            skewRes[0][2] = skewM[0][0] * ai->m[0][2] + skewM[0][1] * ai->m[1][2] + skewM[0][2] * ai->m[2][2];
            skewRes[1][0] = skewM[1][0] * ai->m[0][0] + skewM[1][1] * ai->m[1][0] + skewM[1][2] * ai->m[2][0];
            skewRes[1][1] = skewM[1][0] * ai->m[0][1] + skewM[1][1] * ai->m[1][1] + skewM[1][2] * ai->m[2][1];
            skewRes[1][2] = skewM[1][0] * ai->m[0][2] + skewM[1][1] * ai->m[1][2] + skewM[1][2] * ai->m[2][2];
            skewRes[2][0] = skewM[2][0] * ai->m[0][0] + skewM[2][1] * ai->m[1][0] + skewM[2][2] * ai->m[2][0];
            skewRes[2][1] = skewM[2][0] * ai->m[0][1] + skewM[2][1] * ai->m[1][1] + skewM[2][2] * ai->m[2][1];
            skewRes[2][2] = skewM[2][0] * ai->m[0][2] + skewM[2][1] * ai->m[1][2] + skewM[2][2] * ai->m[2][2];
            ai->m[0][0] = skewRes[0][0] + ai->m[0][0]; ai->m[0][1] = skewRes[0][1] + ai->m[0][1]; ai->m[0][2] = skewRes[0][2] + ai->m[0][2];
            ai->m[1][0] = skewRes[1][0] + ai->m[1][0]; ai->m[1][1] = skewRes[1][1] + ai->m[1][1]; ai->m[1][2] = skewRes[1][2] + ai->m[1][2];
            ai->m[2][0] = skewRes[2][0] + ai->m[2][0]; ai->m[2][1] = skewRes[2][1] + ai->m[2][1]; ai->m[2][2] = skewRes[2][2] + ai->m[2][2];
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            (ai->stuntFlags & 2) == 0 && (ai->blk.flags & 2) == 0) {
            Mtx3_Orthonormalize(ai->m);
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            (ai->stuntFlags & 2) == 0 && (ai->blk.flags & 2) == 0) {
            f32 c, s;
            rollM[2][3] = 0.0f; rollM[2][2] = 0.0f; rollM[2][1] = 0.0f; rollM[2][0] = 0.0f;
            rollM[1][3] = 0.0f; rollM[1][2] = 0.0f; rollM[1][1] = 0.0f; rollM[1][0] = 0.0f;
            rollM[0][3] = 0.0f; rollM[0][2] = 0.0f; rollM[0][1] = 0.0f; rollM[0][0] = 0.0f;
            rollM[2][2] = 1.0f;
            rollM[1][1] = 1.0f;
            rollM[0][0] = 1.0f;
            c = cosf(ai->rollCur);
            s = sinf(ai->rollCur);
            {
                f32 r00 = rollM[0][0] * c + rollM[0][1] * s;
                f32 r01 = rollM[0][1] * c - rollM[0][0] * s;
                f32 r10 = rollM[1][0] * c + rollM[1][1] * s;
                f32 r11 = rollM[1][1] * c - rollM[1][0] * s;
                f32 r20 = rollM[2][0] * c + rollM[2][1] * s;
                f32 r21 = rollM[2][1] * c - rollM[2][0] * s;
                rollM[0][0] = r00; rollM[0][1] = r01;
                rollM[1][0] = r10; rollM[1][1] = r11;
                rollM[2][0] = r20; rollM[2][1] = r21;
            }

            res4[0][0] = ai->m[0][0] * rollM[0][0] + ai->m[0][1] * rollM[1][0] + ai->m[0][2] * rollM[2][0];
            res4[0][1] = ai->m[0][0] * rollM[0][1] + ai->m[0][1] * rollM[1][1] + ai->m[0][2] * rollM[2][1];
            res4[0][2] = ai->m[0][0] * rollM[0][2] + ai->m[0][1] * rollM[1][2] + ai->m[0][2] * rollM[2][2];
            res4[0][3] = ai->m[0][0] * rollM[0][3] + ai->m[0][1] * rollM[1][3] + ai->m[0][2] * rollM[2][3];
            res4[1][0] = ai->m[1][0] * rollM[0][0] + ai->m[1][1] * rollM[1][0] + ai->m[1][2] * rollM[2][0];
            res4[1][1] = ai->m[1][0] * rollM[0][1] + ai->m[1][1] * rollM[1][1] + ai->m[1][2] * rollM[2][1];
            res4[1][2] = ai->m[1][0] * rollM[0][2] + ai->m[1][1] * rollM[1][2] + ai->m[1][2] * rollM[2][2];
            res4[1][3] = ai->m[1][0] * rollM[0][3] + ai->m[1][1] * rollM[1][3] + ai->m[1][2] * rollM[2][3];
            res4[2][0] = ai->m[2][0] * rollM[0][0] + ai->m[2][1] * rollM[1][0] + ai->m[2][2] * rollM[2][0];
            res4[2][1] = ai->m[2][0] * rollM[0][1] + ai->m[2][1] * rollM[1][1] + ai->m[2][2] * rollM[2][1];
            res4[2][2] = ai->m[2][0] * rollM[0][2] + ai->m[2][1] * rollM[1][2] + ai->m[2][2] * rollM[2][2];
            res4[2][3] = ai->m[2][0] * rollM[0][3] + ai->m[2][1] * rollM[1][3] + ai->m[2][2] * rollM[2][3];

            ai->m[0][0] = res4[0][0]; ai->m[0][1] = res4[0][1]; ai->m[0][2] = res4[0][2];
            ai->m[1][0] = res4[1][0]; ai->m[1][1] = res4[1][1]; ai->m[1][2] = res4[1][2];
            ai->m[2][0] = res4[2][0]; ai->m[2][1] = res4[2][1]; ai->m[2][2] = res4[2][2];
            t1.x = res4[0][3]; t1.y = res4[1][3]; t1.z = res4[2][3];
            car->rot.m[0][0] = ai->m[0][0]; car->rot.m[0][1] = ai->m[0][1]; car->rot.m[0][2] = ai->m[0][2];
            car->rot.m[1][0] = ai->m[1][0]; car->rot.m[1][1] = ai->m[1][1]; car->rot.m[1][2] = ai->m[1][2];
            car->rot.m[2][0] = ai->m[2][0]; car->rot.m[2][1] = ai->m[2][1]; car->rot.m[2][2] = ai->m[2][2];
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            if ((ai->blk.flags & 2) != 0) {
                ai->b2A4 = 1;
                if (ai->stuntFlags == 0) {
                    u8 willCrash;
                    u32 axis;
                    f32 t1s, t2s, t3s;
                    ai->stuntDur = ai->blk.f20;
                    willCrash = Ai_Rand_Step_(100) < ai->crashChance;
                    axis = Ai_Rand_Step_(3);
                    if (willCrash && axis == 1) {
                        axis = Ai_Rand_Step_(2) * 2;
                    }
                    t1s = t2s = t3s = 0.0f;
                    if (axis == 0) {
                        axis = 4;
                        if (car->k[22] != 0.0f) {
                            t1s = 1.5f / car->k[22];
                            t2s = 2.5f / car->k[22];
                            t3s = 3.5f / car->k[22];
                        }
                    } else if (axis == 1) {
                        axis = 8;
                        if (car->k530 != 0.0f) {
                            t1s = 1.5f / car->k530;
                            t2s = 2.5f / car->k530;
                            t3s = 3.5f / car->k530;
                        }
                    } else {
                        axis = 0x10;
                        if (car->k[23] != 0.0f) {
                            t1s = 1.5f / car->k[23];
                            t2s = 2.5f / car->k[23];
                            t3s = 3.5f / car->k[23];
                        }
                    }
                    if (ai->stuntDur > t1s) {
                        if (Ai_Rand_Step_(2) == 0 || willCrash) {
                            ai->stuntFlags = axis | 2;
                            if (willCrash) {
                                ai->stuntFlags |= 0x100;
                            }
                            if (Ai_Rand_Step_(2) == 0) {
                                ai->stuntFlags |= 0x20;
                            }
                            if (ai->stuntDur > t2s) {
                                if (Ai_Rand_Step_(2) == 0) {
                                    ai->stuntFlags |= 0x40;
                                } else if (ai->stuntDur > t3s && Ai_Rand_Step_(2) == 0) {
                                    ai->stuntFlags |= 0x80;
                                }
                            }
                            ai->stuntM[0][0] = car->rot.m[0][0]; ai->stuntM[0][1] = car->rot.m[0][1]; ai->stuntM[0][2] = car->rot.m[0][2];
                            ai->stuntM[1][0] = car->rot.m[1][0]; ai->stuntM[1][1] = car->rot.m[1][1]; ai->stuntM[1][2] = car->rot.m[1][2];
                            ai->stuntM[2][0] = car->rot.m[2][0]; ai->stuntM[2][1] = car->rot.m[2][1]; ai->stuntM[2][2] = car->rot.m[2][2];
                        } else {
                            ai->stuntFlags = 1;
                        }
                    } else {
                        ai->stuntFlags = 1;
                    }
                }
            } else {
                if ((ai->stuntFlags & 0x100) != 0) {
                    Car_StartCrash(car, 0, 0);
                }
                ai->stuntFlags = 0;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            (ai->stuntFlags & 2) != 0) {
            f32 ang = (1.0f - cosf((1.0f - ai->blk.f20 / ai->stuntDur) * 3.14159f)) * 0.5f * 6.28318f;
            f32 mult;
            f32 c, s;
            if ((ai->stuntFlags & 0x20) != 0) {
                ang = -ang;
            }
            if ((ai->stuntFlags & 0x80) != 0) {
                mult = 3.0f;
            } else if ((ai->stuntFlags & 0x40) != 0) {
                mult = 2.0f;
            } else {
                mult = 1.0f;
            }
            if ((ai->stuntFlags & 0x100) != 0) {
                mult += 0.5f;
            }
            ang *= mult;
            c = cosf(ang);
            s = sinf(ang);
            skewM[2][2] = 0.0f; skewM[2][1] = 0.0f; skewM[2][0] = 0.0f;
            skewM[1][2] = 0.0f; skewM[1][1] = 0.0f; skewM[1][0] = 0.0f;
            skewM[0][2] = 0.0f; skewM[0][1] = 0.0f; skewM[0][0] = 0.0f;
            skewM[2][2] = 1.0f;
            skewM[1][1] = 1.0f;
            skewM[0][0] = 1.0f;
            if ((ai->stuntFlags & 4) != 0) {
                skewM[1][1] = c; skewM[1][2] = -s;
                skewM[2][1] = s; skewM[2][2] = c;
            } else if ((ai->stuntFlags & 8) != 0) {
                skewM[0][0] = c; skewM[0][1] = -s;
                skewM[1][0] = s; skewM[1][1] = c;
            } else if ((ai->stuntFlags & 0x10) != 0) {
                skewM[0][0] = c; skewM[0][2] = s;
                skewM[2][0] = -s; skewM[2][2] = c;
            }
            car->rot.m[0][0] = ai->stuntM[0][0] * skewM[0][0] + ai->stuntM[0][1] * skewM[1][0] + ai->stuntM[0][2] * skewM[2][0];
            car->rot.m[0][1] = ai->stuntM[0][0] * skewM[0][1] + ai->stuntM[0][1] * skewM[1][1] + ai->stuntM[0][2] * skewM[2][1];
            car->rot.m[0][2] = ai->stuntM[0][0] * skewM[0][2] + ai->stuntM[0][1] * skewM[1][2] + ai->stuntM[0][2] * skewM[2][2];
            car->rot.m[1][0] = ai->stuntM[1][0] * skewM[0][0] + ai->stuntM[1][1] * skewM[1][0] + ai->stuntM[1][2] * skewM[2][0];
            car->rot.m[1][1] = ai->stuntM[1][0] * skewM[0][1] + ai->stuntM[1][1] * skewM[1][1] + ai->stuntM[1][2] * skewM[2][1];
            car->rot.m[1][2] = ai->stuntM[1][0] * skewM[0][2] + ai->stuntM[1][1] * skewM[1][2] + ai->stuntM[1][2] * skewM[2][2];
            car->rot.m[2][0] = ai->stuntM[2][0] * skewM[0][0] + ai->stuntM[2][1] * skewM[1][0] + ai->stuntM[2][2] * skewM[2][0];
            car->rot.m[2][1] = ai->stuntM[2][0] * skewM[0][1] + ai->stuntM[2][1] * skewM[1][1] + ai->stuntM[2][2] * skewM[2][1];
            car->rot.m[2][2] = ai->stuntM[2][0] * skewM[0][2] + ai->stuntM[2][1] * skewM[1][2] + ai->stuntM[2][2] * skewM[2][2];
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0) {
            if (car->cnt3E7 != 0) {
                f32 steer = ai->blk.ang / 2.0f - ai->rollCur * 2.25f;
                steer = (steer > -0.5235984f) ? steer : -0.5235984f;
                steer = (steer < 0.5235984f) ? steer : 0.5235984f;
                car->yaw = -steer;
                if (car->col[3] != 0) {
                    car->yaw = -steer * 0.25f;
                }
            } else {
                car->yaw = 0.0f;
            }
            for (j = 0; j < car->nPoints; j++) {
                car->points[j].rate = (car->fBC / car->points[j].h) * 2.0f;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        ai = &gAi[i];
        car = ai->car;
        if (car->b4B8 == 0 && car->finished == 0 && car->state == 1 && ai->ok != 0 &&
            RaceClock_Get() - ai->rubberTime > (u32)D_80138D51.b2 * 10) {
            ai->rubberTime = RaceClock_Get();
            if (gAiPhysicsCarCount == 1) {
                f32 dist = gAi[gAiPhysicsCar0->id].f218 - ai->f218;
                if (car->lap >= D_80115844 - 1) {
                    dist += D_80115840;
                }
                if (dist > D_80115834 + D_8011583C) {
                    ai->curRubber -= (f32)D_80138D51.b3 / 100.0f;
                } else if (dist < D_80115834 - D_8011583C) {
                    ai->curRubber += (f32)D_80138D51.b6 / 100.0f;
                } else if (ai->curRubber > 0.0f) {
                    ai->curRubber -= (f32)D_80138D51.b5 / 100.0f;
                } else {
                    ai->curRubber += (f32)D_80138D51.b4 / 100.0f;
                }
            } else {
                ai->curRubber = 1.0f;
                continue;
            }
            ai->curRubber = (ai->curRubber < ai->minRubber) ? ai->minRubber : ai->curRubber;
            ai->curRubber = (ai->curRubber > ai->maxRubber) ? ai->maxRubber : ai->curRubber;
        }
    }
}
#else
s32 Ai_Step();

INCLUDE_ASM("asm/nonmatchings/unit_22440", Ai_Step);
#endif

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
            gAi[i].u8_.s.f24 = c1->unk474 / 1000.0f;
        }
        n = D_80115824 - 1;
        gAi[i].f218 = (u32)(n - c1->lap) * D_80115820 + gAi[i].u8_.s.f24;
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

static inline u32 Ai_Rand_(u32 n) {
    D_80115854 = D_80115854 * 0x19660D + 0x3C6EF35F;
    return D_80115854 % n;
}

void Ai_InitRacer(CarG_ *car, u8 arg1, RaceSetup *cfg) {
    u32 i;

    gAi[car->id].id = car->id;
    gAi[car->id].s2A0 = (cfg->f9C / 255.0f) * ((f32)D_80138D3F - (f32)D_80138D3E) + (f32)D_80138D3E;
    gAi[car->id].v94[2] = car->pos;
    gAi[car->id].v94[3] = car->pos;
    gAi[car->id].v94[0] = car->pos;
    gAi[car->id].v94[1] = car->pos;
    gAi[car->id].m[0][0] = car->m[0][0];
    gAi[car->id].m[0][1] = car->m[0][1];
    gAi[car->id].m[0][2] = car->m[0][2];
    gAi[car->id].m[1][0] = car->m[1][0];
    gAi[car->id].m[1][1] = car->m[1][1];
    gAi[car->id].m[1][2] = car->m[1][2];
    gAi[car->id].m[2][0] = car->m[2][0];
    gAi[car->id].m[2][1] = car->m[2][1];
    gAi[car->id].m[2][2] = car->m[2][2];

    car->v7C.x = 0.0f;
    car->v7C.y = 0.0f;
    car->v7C.z = 0.0f;
    car->vA4.x = 0.0f;
    car->vA4.y = 0.0f;
    car->vA4.z = 0.0f;
    car->vE4.x = 0.0f;
    car->vE4.y = 0.0f;
    car->vE4.z = 0.0f;
    car->f88 = -1.0f;

    gAi[car->id].v228.x = 0.0f;
    gAi[car->id].v228.y = 0.0f;
    gAi[car->id].v228.z = 0.0f;
    gAi[car->id].v21C.x = 0.0f;
    gAi[car->id].v21C.y = 0.0f;
    gAi[car->id].v21C.z = 0.0f;
    gAi[car->id].v240.x = 0.0f;
    gAi[car->id].v240.y = 0.0f;
    gAi[car->id].v240.z = 0.0f;
    gAi[car->id].v234.x = 0.0f;
    gAi[car->id].v234.y = 0.0f;
    gAi[car->id].v234.z = 0.0f;

    car->zBC = 0;
    if (Bld_InitRacer((BlkG_ *)&gAi[car->id].u8_.blk, arg1) == 0) {
        gAi[car->id].ok = 0;
    } else {
        gAi[car->id].ok = 1;
        D_80115824 = cfg->f19;
        for (i = 0; i < 10; i++) {
            gAi[car->id].h1[i].x = car->m[0][2];
            gAi[car->id].h1[i].y = car->m[1][2];
            gAi[car->id].h1[i].z = car->m[2][2];
        }
        gAi[car->id].n160 = 0;
        for (i = 0; i < 10; i++) {
            gAi[car->id].h2[i].x = car->m[0][1];
            gAi[car->id].h2[i].y = car->m[1][1];
            gAi[car->id].h2[i].z = car->m[2][1];
        }
        gAi[car->id].n1DC = 0;
        gAi[car->id].z1F8 = 0;
        gAi[car->id].z200 = 0;
        gAi[car->id].z204 = 0;
        gAi[car->id].f1FC = (s32)Ai_Rand_(201) - 100;
        gAi[car->id].braking = 0;
        gAi[car->id].f20C = car->f534 * 100.0f - 100.0f;
        gAi[car->id].ctlTimer = 0;
        gAi[car->id].throttle = 0.0f;
        gAi[car->id].steer = 0.0f;
        gAi[car->id].f20C = (gAi[car->id].f20C + 100.0f) * D_80115828 - 100.0f;
        gAi[car->id].f258 = D_8011582C;
        gAi[car->id].f25C = D_80115830;
        gAi[car->id].f260 = 1.0f;
        gAi[car->id].u264 = RaceClock_Get();
        gAi[car->id].z268 = 0;
        gAi[car->id].z26C = 0;
        gAi[car->id].b2A4 = 0;
        gAi[car->id].b2A5 = 1;
        gAi[car->id].f218 = (f32)(u32)D_80115824 * D_80115820;
    }
}

void *Ai_GetSlot(u8 *arg0) {
    return (*arg0 * 0x2A8) + &D_80114838;
}

void Ai_RestoreSlot(CarD_ *car, BlkD_ *blk) {
    s32 i;

    gAi[car->id].u8_.blk = *blk;
    gAi[car->id].z1F8 = 0;
    gAi[car->id].z200 = 0;
    gAi[car->id].z204 = 0;
    gAi[car->id].z214 = 0;
    gAi[car->id].v94[0] = gAi[car->id].u8_.blk.p;
    gAi[car->id].v94[1] = gAi[car->id].u8_.blk.p;
    gAi[car->id].v94[2] = gAi[car->id].u8_.blk.p;
    gAi[car->id].v94[3] = gAi[car->id].u8_.blk.p;
    gAi[car->id].m[0][0] = car->m[0][0];
    gAi[car->id].m[0][1] = car->m[0][1];
    gAi[car->id].m[0][2] = car->m[0][2];
    gAi[car->id].m[1][0] = car->m[1][0];
    gAi[car->id].m[1][1] = car->m[1][1];
    gAi[car->id].m[1][2] = car->m[1][2];
    gAi[car->id].m[2][0] = car->m[2][0];
    gAi[car->id].m[2][1] = car->m[2][1];
    gAi[car->id].m[2][2] = car->m[2][2];
    for (i = 0; i < 10; i++) {
        gAi[car->id].h1[i].x = car->m[0][2];
        gAi[car->id].h1[i].y = car->m[1][2];
        gAi[car->id].h1[i].z = car->m[2][2];
    }
    for (i = 0; i < 10; i++) {
        gAi[car->id].h2[i].x = car->m[0][1];
        gAi[car->id].h2[i].y = car->m[1][1];
        gAi[car->id].h2[i].z = car->m[2][1];
    }
    gAi[car->id].n160 = 0;
    gAi[car->id].n1DC = 0;
    gAi[car->id].v1E0.x = car->m[0][2];
    gAi[car->id].v1E0.y = car->m[1][2];
    gAi[car->id].v1E0.z = car->m[2][2];
    gAi[car->id].v1EC.x = car->m[0][2];
    gAi[car->id].v1EC.y = car->m[1][2];
    gAi[car->id].v1EC.z = car->m[2][2];
    gAi[car->id].v228.x = 0.0f;
    gAi[car->id].v228.y = 0.0f;
    gAi[car->id].v228.z = 0.0f;
    gAi[car->id].v21C.x = 0.0f;
    gAi[car->id].v21C.y = 0.0f;
    gAi[car->id].v21C.z = 0.0f;
    gAi[car->id].v240.x = 0.0f;
    gAi[car->id].v240.y = 0.0f;
    gAi[car->id].v240.z = 0.0f;
    gAi[car->id].v234.x = 0.0f;
    gAi[car->id].v234.y = 0.0f;
    gAi[car->id].v234.z = 0.0f;
    gAi[car->id].ctlTimer = 0;
    gAi[car->id].throttle = 0.0f;
    gAi[car->id].steer = 0.0f;
    gAi[car->id].z268 = 0;
    gAi[car->id].z26C = 0;
}
