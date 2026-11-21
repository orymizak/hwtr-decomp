#include "common.h"
#include "ultra64.h"
#include "math.h"
#include "include_asm.h"
#include "car.h"
#include "race.h"
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef Vec3f Vec3f_;
/* Race camera (gRaceCams, gRaceCamCount entries of 0x64). */
typedef struct {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ Vec3f_ pos;
    /* 0x10 */ Vec3f_ vel;
    /* 0x1C */ f32 m[3][3];
    /* 0x40 */ f32 fov;
    /* 0x44 */ s32 mode;   /* 0 = follow car */
    /* 0x48 */ u8 pt;      /* trackside camera point */
    /* 0x49 */ u8 car;     /* car followed */
    /* 0x4A */ u8 b4A;
    /* 0x4B */ u8 b4B;
    /* 0x4C */ u8 reset;   /* set by RaceCam_Reset */
    /* 0x4D */ u8 p4D[3];
    /* 0x50 */ s32 timer;
    /* 0x54 */ u32 shake;  /* RaceCam_Shake sets 0xFA */
    /* 0x58 */ f32 f58;
    /* 0x5C */ u8 intro;   /* set during the intro (RaceCam_StartIntro) */
    /* 0x5D */ u8 p5D[3];
    /* 0x60 */ u32 b60;    /* 5000 when the intro ends */
} RaceCam; /* size = 0x64 */
extern RaceCam gRaceCams[];
extern u8 gRaceCamCount;
typedef struct { s32 mode; Vec3f_ ofs; f32 dist; } CamT_;
extern u8 D_80118550;
extern u8 D_80138D43;
s32 Race_GetPhase(void);
void RaceCam_UpdateIntro(RaceCam *);
void RaceCam_SmoothTo(Vec3f_, Vec3f_, Vec3f_, Vec3f_ *, f32, f32, f32, f32);
void Coll_ClipCamera(u8, Vec3f_ *);
u8 Coll_GetRoomAxis(u8, Vec3f_ *, Vec3f_ *);
Vec3f_ Vec3_Lerp(f32 t, Vec3f_ a, Vec3f_ b);
s32 Coll_GetSpawn(s32 arg0);
extern u8 (*svcTrackGetCameraCount)(void);
extern void (*svcTrackGetCamera)(u8, Vec3f_ *, void *, s32 *, s32 *);
extern u8 (*svcRandRange)(s32);
extern u8 D_80118621;
extern s32 *D_80118624;
s32 Cars_GetCount(void);
s32 Coll_GetSpawnCount(void);



extern u8 D_800F3878;
extern s32 D_800F37B0[];
extern s32 D_800F3814[];

/* One camera per human player (at least one); a race with no humans follows the cars freely. */
u8 RaceCam_Init(RaceSetup *cfg) {
    u32 i;
    u32 humans;

    gRaceCamCount = 1;
    D_80118621 = D_800F3878 - 1;
    humans = 0;
    for (i = 0; i < cfg->count; i++) {
        humans += cfg->p[i].kind == 1;
    }
    gRaceCamCount = MAX(gRaceCamCount, humans);
    D_80118624 = (gRaceCamCount == 2) ? D_800F3814 : D_800F37B0;
    D_80118550 = humans == 0;
    memset(gRaceCams, 0, 200);
    for (i = 0; i < gRaceCamCount; i++) {
        gRaceCams[i].unk0 = i;
        if (D_80118550 == 0) {
            gRaceCams[i].car = i;
        }
        gRaceCams[i].mode = *D_80118624;
        gRaceCams[i].reset = 1;
        gRaceCams[i].intro = 0;
        gRaceCams[i].fov = 1.570795f;
        gRaceCams[i].b60 = 0;
    }
    return 1;
}

s32 RaceCam_Shutdown(void) {
    return 1;
}

void RaceCam_UpdateTrackside(RaceCam *cam, u32 dt);
void RaceCam_Nop(RaceCam *cam, f32 k);
s32 RaceCam_GetIntroDuration(void);
void RaceCam_Update(u32 dt) {
    Vec3f_ pos;
    Vec3f_ vel;
    Vec3f_ t;
    Vec3f_ d;
    Vec3f_ f;
    Vec3f_ r;
    Vec3f_ u;
    Vec3f_ ofs;
    Vec3f_ eye;
    Vec3f_ c;
    f32 m[3][3];
    Vec3f_ tmp;
    s32 flags;
    u32 i;
    RaceCam *cam;
    f32 k;
    Car *car;
    u8 vis;
    u8 b3E7;
    f32 f4C4;
    f32 fBC;
    u8 b4C0;
    f32 s;
    f32 far;
    f32 near;
    f32 damp;
    f32 dist;
    f32 olen;
    f32 dp;
    f32 sum;
    f32 *sp;
    s32 (**rnd)(s32);
    s32 idx;

    k = 1000.0f / (f32)dt;
    cam = gRaceCams;
    for (i = 0; i < gRaceCamCount; cam++, i++) {
        if ((*cam).intro != 0 && RaceCam_GetIntroDuration() != 0) {
            RaceCam_UpdateIntro(&(*cam));
            continue;
        }
        vis = 1;
        car = Car_Get((*cam).car);
        pos.x = car->pos.x + car->v8C.x;
        pos.y = car->pos.y + car->v8C.y;
        pos.z = car->pos.z + car->v8C.z;
        m[0][0] = car->rot.m[0][0];
        m[0][1] = car->rot.m[0][1];
        m[0][2] = car->rot.m[0][2];
        m[1][0] = car->rot.m[1][0];
        m[1][1] = car->rot.m[1][1];
        m[1][2] = car->rot.m[1][2];
        m[2][0] = car->rot.m[2][0];
        m[2][1] = car->rot.m[2][1];
        m[2][2] = car->rot.m[2][2];
        vel = car->vel;
        flags = car->flags;
        if (Race_GetPhase() != 0) {
            vis = car->b13C;
        }
        b3E7 = car->cnt3E7;
        b4C0 = car->b4C0;
        f4C4 = car->f4C4;
        fBC = car->fBC;
        switch ((*cam).mode) {
        case 0:
            tmp.x = m[0][0] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.x + m[0][1] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.y + m[0][2] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.z;
            tmp.y = m[1][0] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.x + m[1][1] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.y + m[1][2] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.z;
            tmp.z = m[2][0] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.x + m[2][1] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.y + m[2][2] * (((CamT_ *)D_80118624)[(*cam).pt]).ofs.z;
            t = tmp;
            VEC3_ADD((*cam).pos, pos, t);
            (*cam).vel = vel;
            (*cam).m[0][0] = m[0][0];
            (*cam).m[0][1] = m[0][1];
            (*cam).m[0][2] = m[0][2];
            (*cam).m[1][0] = m[1][0];
            (*cam).m[1][1] = m[1][1];
            (*cam).m[1][2] = m[1][2];
            (*cam).m[2][0] = m[2][0];
            (*cam).m[2][1] = m[2][1];
            (*cam).m[2][2] = m[2][2];
            (*cam).fov = 1.570795f;
            Coll_ClipCamera((*cam).car, &(*cam).pos);
            break;
        case 1:
            if (flags & 0x10) {
                if (!Coll_GetRoomAxis((*cam).car, &t, &u)) {
                    flags &= ~0x10;
                }
            }
            if (flags & 0x10) {
                far = 30000.0f;
                near = 10000.0f;
                damp = 0.1f;
                s = VEC3_DOT(vel, t);
                VEC3_SCALE(vel, t, s);
                sum = VEC3_ABS_SUM(vel);
                if (sum < 1.0f) {
                    vel = t;
                }
                f = vel;
                VEC3_NORMALIZE(f);
                if (car->b744 == 0) {
                    u.x = 0.0f;
                    u.y = 0.0f;
                    u.z = 1.0f;
                }
                VEC3_CROSS(r, f, u);
                VEC3_NORMALIZE(r);
                VEC3_CROSS(u, r, f);
            } else {
                far = 30000.0f;
                near = 10000.0f;
                damp = 0.1f;
                if (!(flags & 4)) {
                    vel.z = 0.0f;
                }
                if (sqrtf(VEC3_DOT(vel, vel)) < D_80138D43 * 17.6f) {
                    if (b3E7 != 0 || (*cam).b60 != 0 || (*cam).reset != 0) {
                        t.x = m[0][1];
                        t.y = m[1][1];
                        t.z = m[2][1];
                    } else {
                        VEC3_SUB(t, pos, (*cam).pos);
                    }
                    t.z = 0.0f;
                    if (ABS(t.x) + ABS(t.y) > 0.0f) {
                        f = t;
                    } else {
                        f.x = m[0][2];
                        f.y = m[1][2];
                        f.z = m[2][2];
                    }
                } else {
                    f = vel;
                }
                VEC3_NORMALIZE(f);
                if (flags & 4) {
                    u.x = m[0][2];
                    u.y = m[1][2];
                    u.z = m[2][2];
                    VEC3_CROSS(r, f, u);
                    if (!(VEC3_ABS_SUM(r) > 0.0f)) {
                        r.x = 1.0f;
                        r.y = 0.0f;
                        r.z = 0.0f;
                    }
                    VEC3_NORMALIZE(r);
                    VEC3_CROSS(u, r, f);
                } else {
                    r.x = f.y;
                    r.y = -f.x;
                    r.z = 0.0f;
                    u.x = 0.0f;
                    u.y = 0.0f;
                    u.z = 1.0f;
                }
            }
            VEC3_SCALE(t, u, (((CamT_ *)D_80118624)[(*cam).pt]).dist);
            VEC3_ADD(pos, pos, t);
            ofs = (((CamT_ *)D_80118624)[(*cam).pt]).ofs;
            if (b4C0 != 0) {
                s = ((fBC - f4C4) + 528.0f) / 528.0f;
            } else {
                s = 0.0f;
            }
            (*cam).f58 += (s - (*cam).f58) / k;
            s = (*cam).f58 * 0.5f + 1.0f;
            ofs.x *= s;
            ofs.y *= s;
            eye = pos;
            VEC3_SCALE(t, r, ofs.x);
            VEC3_ADD(eye, eye, t);
            VEC3_SCALE(t, f, ofs.y);
            VEC3_ADD(eye, eye, t);
            VEC3_SCALE(t, u, ofs.z);
            VEC3_ADD(eye, eye, t);
            Coll_ClipCamera((*cam).car, &eye);
            VEC3_SUB(t, eye, pos);
            dist = sqrtf(VEC3_DOT(t, t));
            olen = sqrtf(VEC3_DOT(ofs, ofs));
            if (vis == 0 && (*cam).reset == 0 && (*cam).b60 == 0) {
                VEC3_SUB(t, (*cam).pos, pos);
                VEC3_SUB(d, eye, pos);
                if (VEC3_DOT(t, d) < 0.0f) {
                    eye.z += 120.0f;
                }
                RaceCam_SmoothTo((*cam).pos, (*cam).vel, eye, &eye, k, far, near, damp);
            }
            if (dist < 1.0f) {
                f.y = 1.0f;
            }
            VEC3_SUB(f, pos, eye);
            VEC3_NORMALIZE(f);
            if (flags & 0x10) {
                u.x = 0.0f;
                u.y = 0.0f;
                u.z = 1.0f;
            }
            VEC3_CROSS(r, f, u);
            VEC3_NORMALIZE(r);
            VEC3_CROSS(u, r, f);
            VEC3_SCALE(t, f, MIN(olen, dist));
            VEC3_SUB(eye, pos, t);
            if ((*cam).b60 != 0) {
                sp = (f32 *)Coll_GetSpawn(Coll_GetSpawnCount() - 1);
                s = (f32)(5000 - (*cam).b60) / 5000.0f;
                s = (sinf((s - 0.5f) * 3.14159f) + 1.0f) * 0.5f;
                tmp = Vec3_Lerp(s, *(Vec3f_ *)&sp[0], eye);
                (*cam).pos = tmp;
                c = Vec3_Lerp(s, *(Vec3f_ *)&sp[3], pos);
                VEC3_SUB(f, c, (*cam).pos);
                VEC3_NORMALIZE(f);
                r.x = f.y;
                r.y = -f.x;
                r.z = 0.0f;
                VEC3_NORMALIZE(r);
                VEC3_CROSS(u, r, f);
                MTX3_SET_COLUMNS((*cam).m, r, f, u);
                (*cam).b60 -= dt;
            } else {
                if ((*cam).reset == 0) {
                    VEC3_SUB((*cam).vel, eye, (*cam).pos);
                    VEC3_SCALE((*cam).vel, (*cam).vel, k);
                } else {
                    (*cam).vel = vel;
                }
                (*cam).pos = eye;
                MTX3_SET_COLUMNS((*cam).m, r, f, u);
                (*cam).fov = 1.570795f;
            }
            break;
        case 2:
            svcTrackGetCamera((*cam).pt, &(*cam).pos, (*cam).m, &flags, (s32 *)&(*cam).fov);
            (*cam).vel.x = 0.0f;
            (*cam).vel.y = 0.0f;
            (*cam).vel.z = 0.0f;
            break;
        case 3:
            svcTrackGetCamera((*cam).pt, &(*cam).pos, m, &flags, (s32 *)&(*cam).fov);
            (*cam).vel.x = 0.0f;
            (*cam).vel.y = 0.0f;
            (*cam).vel.z = 0.0f;
            VEC3_SUB(f, pos, (*cam).pos);
            VEC3_NORMALIZE(f);
            r.x = f.y;
            r.y = -f.x;
            r.z = 0.0f;
            VEC3_NORMALIZE(r);
            VEC3_CROSS(u, r, f);
            VEC3_NORMALIZE(u);
            MTX3_SET_COLUMNS((*cam).m, r, f, u);
            break;
        case 4:
            RaceCam_Nop(cam, k);
            break;
        }
        if ((*cam).shake > dt) {
            rnd = (s32 (**)(s32))&svcRandRange;
            s = (*rnd)((*cam).shake) - 125;
            (*cam).pos.x += s / 10.0f;
            s = (*rnd)((*cam).shake) - 125;
            (*cam).pos.y += s / 10.0f;
            s = (*rnd)((*cam).shake) - 125;
            (*cam).pos.z += s / 10.0f;
            (*cam).shake -= dt;
        } else {
            (*cam).shake = 0;
        }
        (*cam).reset = 0;
        if (D_80118550 != 0) {
            RaceCam_UpdateTrackside(&(*cam), dt);
        } else if ((*cam).b4A != (*cam).b4B) {
            if ((*cam).b4A != 0) {
                idx = ((*cam).pt + 1) % D_80118621;
                (*cam).pt = idx;
                (*cam).mode = (((CamT_ *)D_80118624)[idx]).mode;
                (*cam).reset = 1;
                if ((*cam).b60 != 0) {
                    car->cnt3E7 = car->nPoints;
                    (*cam).b60 = 0;
                }
            }
            (*cam).b4B = (*cam).b4A;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", RaceCam_SmoothTo);

void RaceCam_Get(u8 i, Vec3i *a1, f32 *a2, f32 *a3, f32 *a4, f32 *a5, s8 *a6, u8 *a7) {
    RaceCam *cam = &gRaceCams[i];

    *a1 = *(Vec3i *)&cam->pos;
    a2[0] = cam->m[0][0];
    a2[1] = cam->m[1][0];
    a2[2] = cam->m[2][0];
    a3[0] = cam->m[0][1];
    a3[1] = cam->m[1][1];
    a3[2] = cam->m[2][1];
    a4[0] = cam->m[0][2];
    a4[1] = cam->m[1][2];
    a4[2] = cam->m[2][2];
    *a5 = cam->fov;
    *a6 = cam->mode == 0;
    *a7 = cam->car;
}

void RaceCam_GetListener(V12_ *a, V12_ *b, Vec3f *c, Vec3f *d) {
    *a = *(V12_ *)&gRaceCams[0].pos;
    *b = *(V12_ *)&gRaceCams[0].vel;
    c->x = gRaceCams[0].m[0][1];
    c->y = gRaceCams[0].m[1][1];
    c->z = gRaceCams[0].m[2][1];
    d->x = gRaceCams[0].m[0][2];
    d->y = gRaceCams[0].m[1][2];
    d->z = gRaceCams[0].m[2][2];
}


void RaceCam_SetFlag4A(s32 arg0, s8 arg1) {
    gRaceCams[arg0 & 0xFF].b4A = arg1;
}

void RaceCam_UpdateTrackside(RaceCam *cam, u32 dt) {
    Vec3f_ pos;
    Vec3f_ p;
    Vec3f_ d;
    u8 b[0x28];
    s32 flags;
    s32 x;
    f32 radius;
    f32 mind;
    f32 dist;
    u32 i;
    u32 j;
    u32 count;
    u32 best;
    u32 bestCount;
    u32 bestPt;
    u32 bestCar;
    Car *car;
    s32 v;
    s32 w;

    best = 0;
    radius = 0.0f;
    bestCar = 0;
    if (cam->timer < dt) {
        bestCount = 0;
        bestPt = 0;
        for (i = 0; i < svcTrackGetCameraCount(); i++) {
            svcTrackGetCamera(i, &pos, b, &flags, &x);
            /* TODO (Clean version / Bug check): [evaluated 2026-11-18: original behaviour; the
             * forced bit is in the ROM] 'flags |= 1;' forces the 'if (flags & 1)' branch,
             * making the 'else' branch (radius = 0.0f) dead code. Verify if flags was intended
             * to be tested without unconditional mutation:
             *   radius = (flags & 1) ? 5400.0f : 0.0f;
             */
            flags |= 1;
            if (flags & 2) {
                continue;
            }
            count = 0;
            if (flags & 1) {
                p = pos;
                radius = 5400.0f;
            } else {
                radius = 0.0f;
            }
            mind = 0.0f;
            for (j = 0; j < Cars_GetCount(); j++) {
                car = Car_Get(j);
                d.x = p.x - car->pos.x;
                d.y = p.y - car->pos.y;
                d.z = p.z - car->pos.z;
                dist = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
                if (dist < radius && 0.0f < d.x * car->vel.x + d.y * car->vel.y + d.z * car->vel.z) {
                    count++;
                    if (mind < dist) {
                        mind = dist;
                        best = j;
                    }
                }
            }
            if (bestCount < count) {
                bestCount = count;
                bestPt = i;
                bestCar = best;
            }
        }
        if (bestCount != 0) {
            v = flags;
            cam->car = bestCar;
            cam->pt = bestPt;
            /* TODO (Clean version / Bug check): [evaluated 2026-11-18: the ternary changes the ROM]
             * '(-(v & 1) & 3) | 2' simplifies to (v & 1) ? 3 : 2.
             * Because 'flags |= 1' is unconditionally executed in the loop above, v & 1 is always 1 (mode 3).
             * Clean equivalent:
             *   cam->mode = (v & 1) ? 3 : 2;
             */
            cam->mode = (-(v & 1) & 3) | 2;
        } else {
            do {
                cam->pt = svcRandRange(D_80118621);
            } while ((w = D_80118624[cam->pt * 5]) == 0);
            cam->mode = w;
            for (j = 0; j < 10; j++) {
                cam->car = svcRandRange(Cars_GetCount());
                if (Car_Get(cam->car)->b4B8 == 0) {
                    break;
                }
            }
        }
        cam->reset = 1;
        cam->timer = 4000;
    }
    cam->timer -= dt;
}

void RaceCam_Nop(RaceCam *cam, f32 k) {

}

s32 RaceCam_GetIntroDuration(void) {
    s32 n = Coll_GetSpawnCount();

    if (n == 0) {
        return 0;
    }
    return (n - 1) * 200;
}

void RaceCam_StartIntro(void) {
    u32 i;

    for (i = 0; i < gRaceCamCount; i++) {
        gRaceCams[i].intro = 1;
    }
}


void RaceCam_EndIntro(void) {
    u32 i;

    for (i = 0; i < gRaceCamCount; i++) {
        if (gRaceCams[i].intro != 0) {
            gRaceCams[i].b60 = 5000;
            gRaceCams[i].intro = 0;
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_2F4F0", RaceCam_UpdateIntro);

/* Replay record of a race camera: mode, followed car, first two rotation rows (s8, x127) and position. */
typedef struct { u16 mode; u16 car; s8 rot[6]; s16 pos[3]; } CamRec_;

/* Packs the race cameras into 16-byte records; returns the bytes written, 0 if out (size bytes) is too small. */
u32 RaceCam_SaveReplay(CamRec_ *out, u32 size) {
    u32 n;
    u32 i;
    s32 v;

    n = gRaceCamCount * sizeof(CamRec_);
    if (size < n) {
        return 0;
    }
    for (i = 0; i < gRaceCamCount; out++, i++) {
        v = gRaceCams[i].m[0][0] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[0] = v;
        v = gRaceCams[i].m[0][1] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[1] = v;
        v = gRaceCams[i].m[0][2] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[2] = v;
        v = gRaceCams[i].m[1][0] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[3] = v;
        v = gRaceCams[i].m[1][1] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[4] = v;
        v = gRaceCams[i].m[1][2] * 127.0f;
        if (v < -127) {
            v = -127;
        }
        if (v > 127) {
            v = 127;
        }
        out->rot[5] = v;
        out->pos[0] = gRaceCams[i].pos.x;
        out->pos[1] = gRaceCams[i].pos.y;
        out->pos[2] = gRaceCams[i].pos.z;
        out->mode = gRaceCams[i].mode;
        out->car = gRaceCams[i].car;
    }
    return n;
}

/* Restores the race cameras from RaceCam_SaveReplay records; the third rotation row is the cross
 * product of the first two. */
void RaceCam_LoadReplay(CamRec_ *in) {
    u32 i;

    for (i = 0; i < gRaceCamCount; in++, i++) {
        gRaceCams[i].m[0][0] = in->rot[0] / 127.0f;
        gRaceCams[i].m[0][1] = in->rot[1] / 127.0f;
        gRaceCams[i].m[0][2] = in->rot[2] / 127.0f;
        gRaceCams[i].m[1][0] = in->rot[3] / 127.0f;
        gRaceCams[i].m[1][1] = in->rot[4] / 127.0f;
        gRaceCams[i].m[1][2] = in->rot[5] / 127.0f;
        gRaceCams[i].m[2][0] = gRaceCams[i].m[0][1] * gRaceCams[i].m[1][2] - gRaceCams[i].m[1][1] * gRaceCams[i].m[0][2];
        gRaceCams[i].m[2][1] = gRaceCams[i].m[1][0] * gRaceCams[i].m[0][2] - gRaceCams[i].m[0][0] * gRaceCams[i].m[1][2];
        gRaceCams[i].m[2][2] = gRaceCams[i].m[0][0] * gRaceCams[i].m[1][1] - gRaceCams[i].m[1][0] * gRaceCams[i].m[0][1];
        gRaceCams[i].pos.x = in->pos[0];
        gRaceCams[i].pos.y = in->pos[1];
        gRaceCams[i].pos.z = in->pos[2];
        gRaceCams[i].mode = in->mode;
        gRaceCams[i].car = in->car;
        gRaceCams[i].shake = 0;
    }
}

void RaceCam_Reset(s32 arg0) {
    gRaceCams[arg0 & 0xFF].reset = 1;
}

void RaceCam_Shake(s32 arg0) {
    gRaceCams[arg0 & 0xFF].shake = 0xFA;
}
