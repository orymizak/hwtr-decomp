#include "common.h"
#include "include_asm.h"
#include "car.h"

f32 sqrtf(f32);

/* Engine state at Car.r3EC. */
typedef struct { f32 rpm; f32 ratio; u8 rev; u8 gear; u8 padA[2]; f32 c; f32 f10; f32 max; f32 min; u32 ngears; f32 f20; f32 gears[6]; f32 rgear; f32 f40; u8 curve[17]; } EngB_;

/* Averages the wheel contact points and normals, then derives the engine rpm and drive force. */
void Car_AverageGroundContacts(Car *a) {
    Vec3f P;
    Vec3f N;
    Vec3f t;
    Vec3f v;
    Vec3f r;
    Vec3f wr;
    f32 unused[2];
    EngB_ *e;
    CarPoint *p;
    f32 rad;
    f32 d;
    f32 sp;
    u32 cnt;
    u32 i;
    s32 nslip;
    s32 idx;

    cnt = 0;
    rad = 0.0f;
    i = 0;
    P.x = 0.0f;
    P.y = 0.0f;
    P.z = 0.0f;
    N.x = 0.0f;
    N.y = 0.0f;
    N.z = 0.0f;
    nslip = 0;
    e = (EngB_ *)&a->r3EC;
    if (a->nPoints != 0) {
        p = a->points;
        do {
            if ((p->flags & 4) && p->contact) {
                P.x += p->pt.x;
                P.y += p->pt.y;
                P.z += p->pt.z;
                N.x += p->n.x;
                N.y += p->n.y;
                N.z += p->n.z;
                if (cnt == 0) {
                    rad = p->h;
                }
                cnt++;
                nslip += p->b60 == 0;
            }
            i++;
            p++;
        } while (i < a->nPoints);
    }
    if ((cnt != 0) & (rad > 0.0f)) {
        P.x *= 1.0f / (f32)cnt;
        P.y *= 1.0f / (f32)cnt;
        P.z *= 1.0f / (f32)cnt;
        N.x *= 1.0f / (f32)cnt;
        N.y *= 1.0f / (f32)cnt;
        N.z *= 1.0f / (f32)cnt;
        d = 1.0f / sqrtf(N.x * N.x + N.y * N.y + N.z * N.z);
        N.x *= d;
        N.y *= d;
        N.z *= d;
        r.x = P.x - a->pos.x;
        r.y = P.y - a->pos.y;
        r.z = P.z - a->pos.z;
        wr.x = a->angVel.y * r.z - r.y * a->angVel.z;
        wr.y = r.x * a->angVel.z - a->angVel.x * r.z;
        wr.z = a->angVel.x * r.y - r.x * a->angVel.y;
        v.x = a->vel.x + wr.x;
        v.y = a->vel.y + wr.y;
        v.z = a->vel.z + wr.z;
        d = v.x * N.x + v.y * N.y + v.z * N.z;
        t.x = N.x * d;
        t.y = N.y * d;
        t.z = N.z * d;
        P.x = v.x - t.x;
        P.y = v.y - t.y;
        P.z = v.z - t.z;
        N.x = a->rot.m[0][0];
        N.y = a->rot.m[1][0];
        N.z = a->rot.m[2][0];
        d = P.x * N.x + P.y * N.y + P.z * N.z;
        t.x = N.x * d;
        t.y = N.y * d;
        t.z = N.z * d;
        P.x = P.x - t.x;
        P.y = P.y - t.y;
        P.z = P.z - t.z;
        N.x = a->rot.m[0][1];
        N.y = a->rot.m[1][1];
        N.z = a->rot.m[2][1];
        e->rev = P.x * N.x + P.y * N.y + P.z * N.z < 0.0f;
        sp = sqrtf(P.x * P.x + P.y * P.y + P.z * P.z);
        if (e->rev) {
            e->c = e->f20 * e->rgear;
            e->rpm = e->c * ((sp / (rad * 3.14159f)) * 60.0f);
            e->c = -e->c;
        } else {
            for (i = 0; i < e->ngears; i++) {
                e->gear = i;
                e->c = e->f20 * e->gears[i];
                e->rpm = e->c * ((sp / (rad * 3.14159f)) * 60.0f);
                if (e->rpm < e->max) {
                    break;
                }
            }
        }
        if (nslip == 0) {
            if (e->rev) {
                e->rpm = MAX(e->rpm, e->max * a->unk018);
            } else {
                e->rpm = MAX(e->rpm, e->max * a->unk014);
            }
        }
        if (e->rpm > e->max) {
            e->f10 = 0.0f;
            e->rpm = e->max;
        } else {
            idx = ((e->rpm - e->min) / (e->max - e->min)) * 16.0f;
            idx = idx > 16 ? 16 : (idx < 0 ? 0 : (idx > 16 ? 16 : idx));
            e->f10 = ((f32)e->curve[idx] / 255.0f) * e->f40 * 12.0f;
            e->f10 = (e->f10 * e->c) / (rad * 0.5f);
            if (e->rev) {
                e->f10 = e->f10 * a->unk018;
            } else {
                e->f10 = e->f10 * a->unk014;
            }
        }
    } else {
        if (e->rev) {
            e->c = e->f20 * e->rgear;
            e->rpm = e->max * a->unk018;
        } else {
            e->c = e->f20 * e->gears[e->gear];
            e->rpm = e->max * a->unk014;
        }
        e->f10 = 0.0f;
    }
    e->ratio = ((EngB_ *)&a->r3EC)->rpm / ((EngB_ *)&a->r3EC)->c;
    e->rpm = (e->rpm > e->min) ? e->rpm : e->min;
}
