#include "common.h"
#include "math.h"
#include "physics.h"
#include "include_asm.h"
typedef struct { u32 flags; u8 pad4[3]; u8 b7; u8 pad8[3]; u8 bB; } Inf12b_;
extern void (*svcTrackSmashObject[])(s32, Vec3f);
u8 RaceSfx_Play(Vec3i, u8, u8, s32);
extern u32 (*svcTrackGetLinkCount)(void);
extern Vec3f D_80100B00;
void *Memory_Alloc(s32 size);
void CollShape_InitBox(Vec3i *, Vec3f, Vec3f, f32 *, s32);
void RigidBody_Init(RigidBody *, f32, f32, f32, f32, Vec3f);
void Coll_Register(Collider *o);
extern void (*svcTrackGetLink)(u32, Vec3i *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void (*svcTrackSetLinkTransform)(u32, f32, f32, f32, void *);
void RigidBody_Integrate(void *, f32);
void Mtx3_Orthonormalize(void *);
extern Collider *gTrackObjs;
extern void *gTrackObjInfo;
extern u32 gTrackObjCount;
s32 Coll_Unregister(s32 *);
void CollShape_Free(void *);
void Memory_Free(void *);



s32 TrackObj_Init(void) {
    Vec3f pos;
    Vec3f dir;
    f32 m[9];
    Vec3f dflt;
    u32 val;
    Collider *e;
    u32 *info;
    u32 i;
    RigidBody *obj;
    s32 fl;

    dflt = D_80100B00;
    gTrackObjCount = svcTrackGetLinkCount();
    if (gTrackObjCount == 0) {
        return 1;
    }
    gTrackObjs = Memory_Alloc(gTrackObjCount * sizeof(Collider));
    memset(gTrackObjs, 0, gTrackObjCount * sizeof(Collider));
    gTrackObjInfo = Memory_Alloc(gTrackObjCount * 12);
    memset(gTrackObjInfo, 0, gTrackObjCount * 12);
    e = gTrackObjs;
    info = gTrackObjInfo;
    for (i = 0; i < gTrackObjCount; e++, i++, info += 3) {
        svcTrackGetLink(i, (Vec3i *)&pos, m, (f32 *)&dir, (f32 *)&val, (f32 *)info, (f32 *)(info + 1), (f32 *)(info + 2));
        if (info[0] & 1) {
            e->flags |= 2;
        }
        if (info[0] & 2) {
            e->flags |= 4;
        }
        if (info[0] & 0x40) {
            e->flags |= 8;
        }
        if (info[0] & 8) {
            e->kind = 2;
        } else if (info[0] & 0x10) {
            e->kind = 1;
        } else {
            e->kind = 0;
        }
        e->weight = val;
        fl = (info[0] >> 4) & 1;
        CollShape_InitBox(&e->pos, dir, pos, m, fl);
        if (e->kind == 1) {
            e->body = obj = Memory_Alloc(sizeof(RigidBody));
            memset(obj, 0, sizeof(RigidBody));
            RigidBody_Init(e->body, (f32) val / GRAVITY, dir.x, dir.y, dir.z, dflt);
            e->body->r.m[0][0] = m[0];
            e->body->r.m[0][1] = m[1];
            e->body->r.m[0][2] = m[2];
            e->body->r.m[1][0] = m[3];
            e->body->r.m[1][1] = m[4];
            e->body->r.m[1][2] = m[5];
            e->body->r.m[2][0] = m[6];
            e->body->r.m[2][1] = m[7];
            e->body->r.m[2][2] = m[8];
            e->body->pos.x = pos.x - e->body->f60.x;
            e->body->pos.y = pos.y - e->body->f60.y;
            e->body->pos.z = pos.z - e->body->f60.z;
            e->body->frozen = 1;
        }
        Coll_Register(e);
    }
    return 1;
}

s32 TrackObj_Shutdown(void) {
    Collider *e;
    u32 i;

    if (gTrackObjs != NULL) {
        e = gTrackObjs;
        for (i = 0; i < gTrackObjCount; i++) {
            Coll_Unregister((s32 *)e);
            if (e->kind == 1) {
                Memory_Free(e->body);
            }
            CollShape_Free(&e->pos);
            e++;
        }
        Memory_Free(gTrackObjs);
        Memory_Free(gTrackObjInfo);
        gTrackObjs = NULL;
        gTrackObjInfo = NULL;
    }
    return 1;
}


void TrackObj_Update(void) {
    u32 i;
    Collider *e;
    f32 v[3];
    Vec3i pos;
    s32 pad;
    f32 w[4];
    f32 m[10];
    f32 a;
    f32 b;
    f32 c;
    f32 d;

    e = gTrackObjs;
    for (i = 0; i < gTrackObjCount; i++, e++) {
        if (e->kind == 1) {
            if (e->body->frozen == 0) {
                RigidBody_Integrate(e->body, 0.01f);
                Mtx3_Orthonormalize(&e->body->r);
                v[0] = e->body->pos.x + e->body->f60.x;
                v[1] = e->body->pos.y + e->body->f60.y;
                v[2] = e->body->pos.z + e->body->f60.z;
                svcTrackSetLinkTransform(i, v[0], v[1], v[2], &e->body->r);
            }
            e->state = e->body->frozen;
        } else if (e->kind == 2) {
            svcTrackGetLink(i, &pos, m, w, &a, &b, &c, &d);
            e->pos = pos;
            e->m[0] = m[0];
            e->m[1] = m[1];
            e->m[2] = m[2];
            e->m[3] = m[3];
            e->m[4] = m[4];
            e->m[5] = m[5];
            e->m[6] = m[6];
            e->m[7] = m[7];
            e->m[8] = m[8];
        }
    }
}

void TrackObj_Smash(Collider *e, Vec3f v) {
    s32 idx = e - gTrackObjs;
    Inf12b_ *inf = &((Inf12b_ *)gTrackObjInfo)[idx];

    e->flags |= 1;
    svcTrackSmashObject[0](idx, v);
    if (inf->flags & 4) {
        RaceSfx_Play(e->pos, inf->b7, inf->bB, 0);
    }
}

s32 TrackObj_SaveReplay(s32 arg0, u32 arg1) {
    return (arg1 >= 4) ? 4 : 0;
}

void TrackObj_LoadReplay(void) {

}
