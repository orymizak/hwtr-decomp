#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
extern s32 gCollFrame;
extern s32 D_8011C820;
extern s32 gCollLapLength;
extern s32 D_8011C828;
extern u8 gCollIdUsed[];
extern void (*svcWorldSetCollLevel)(void *);
s32 Bld_GetLapLength(void);
u8 Coll_LoadLevel(u8 *);
void TrackSfx_Init(s32 n, s32 b);
extern s32 gCollMovers;
extern s32 gCollCars;
extern s32 gCollBodies;
extern s32 gCollPathCars;
void TrackSfx_Shutdown(void);
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } V16_;
typedef struct { u8 pad[0x18]; V12_ v[6]; V16_ q[1]; } Cam41_;
typedef struct Nd4_ { s32 a; struct Nd4_ *prev; struct Nd4_ *next; u16 cnt; } Nd4_;
typedef struct { u8 pad[0x4C]; void *a; void *b; void *c; u8 pad2[0xC]; s32 id; } Obj95_;
typedef struct Lnk95_ { Obj95_ *obj; s32 x; struct Lnk95_ *next; } Lnk95_;
extern Lnk95_ *gCollAll;
void Coll_Unregister(s32 *);
typedef struct { u8 pad[0x110]; u8 b110; } Obj60_;
typedef struct { u8 pad[8]; s32 w8; } Obj70_;
typedef struct { u8 pad[0x60]; Obj60_ *p60; u8 pad2[0xC]; Obj70_ *p70; } ObjA2_;
typedef struct LnkA2_ { ObjA2_ *obj; s32 x; struct LnkA2_ *next; } LnkA2_;
extern LnkA2_ *gCollRacers;
void Coll_BoxVsWorldEdges(ObjA2_ *);
typedef struct { u8 pad[0x40]; void *a; void *b; void *c; } Res40_;
void Memory_Free(void *);
typedef struct Nd3_ { s32 a; struct Nd3_ *prev; struct Nd3_ *next; } Nd3_;
/* Room (0x14), portal (0xC) and level header shared by the room/collision code. */
typedef struct {
    /* 0x00 */ u16 flags;
    /* 0x02 */ union { u16 val; u8 b[2]; } param;
    /* 0x04 */ u8 b4;
    /* 0x05 */ s8 x;
    /* 0x06 */ s8 y;
    /* 0x07 */ s8 z;
    /* 0x08 */ u16 time;
    /* 0x0A */ u8 nPortals;
    /* 0x0B */ u8 pB;
    /* 0x0C */ u16 firstPortal;
    /* 0x0E */ u8 pE[2];
    /* 0x10 */ Nd4_ *list;
} Room_;
typedef struct {
    /* 0x00 */ s16 nx;
    /* 0x02 */ s16 ny;
    /* 0x04 */ s16 nz;
    /* 0x06 */ s16 d;
    /* 0x08 */ u16 to;
    /* 0x0A */ u8 flag;
    /* 0x0B */ u8 pB;
} Portal_;
typedef struct {
    /* 0x00 */ u32 nRooms;
    /* 0x04 */ u8 p4[0xC];
    /* 0x10 */ s32 x10;
    /* 0x14 */ s32 nSpawns;
    /* 0x18 */ u8 p18[0xA8];
    /* 0xC0 */ Room_ *rooms;
    /* 0xC4 */ u8 pC4[4];
    /* 0xC8 */ Portal_ *portals;
    /* 0xCC */ u8 pCC[4];
    /* 0xD0 */ s32 xD0;
    /* 0xD4 */ u8 *spawns;
} Level_;
/* Object tracked by the room system. */
typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ u8 p4[0x44];
    /* 0x48 */ u8 nRooms;
    /* 0x49 */ u8 p49[0xB];
    /* 0x54 */ u16 *rooms;
    /* 0x58 */ u8 p58[4];
    /* 0x5C */ s32 type;
    /* 0x60 */ void *p60;
    /* 0x64 */ Car *car;
    /* 0x68 */ u8 p68[8];
    /* 0x70 */ Nd4_ *list;
} RObj_;
typedef struct { f32 a; f32 b; f32 c; f32 d; s16 e; u16 f; } PlF_;
extern u32 D_8011C830[];
void Lap_PassCheckpoint(u8, u8, s32);
void Car_Boost(u8 *);
void Car_DirectionalBoost(u8 *, u16, u8);
void TrackSfx_Trigger(u16);
void Coll_ApplyRoomFlags(RObj_ *o, u16 idx, s32 arg2);
u16 Coll_FindRoom(Vec3f pos);
void Coll_Register(RObj_ *o);
void CollShape_InitPoint(u8 *n, u8 *o);
void *Memory_Alloc(s32 size);
void CollShape_InitCar(u8 *n, Car *o);

extern void *gCollNodeFree;
extern Level_ *gCollLevel;

/* 155/159 words (628/636 B): original reloads gCollLevel twice at 0x80099110 (into $a0 and $v1) before adding 0xD8. */
#ifdef NON_MATCHING
typedef struct { u16 idx; u16 h4; u16 h6; u16 val; u8 pad[0x14 - 10]; } Sub90_;
typedef struct { u16 flags; Sub90_ s; } Ent90_;
typedef struct { u16 flags; u16 h2; u16 h4; u8 pad[6]; } Ev90_;
typedef struct { s32 n0; s32 n4; s32 n8; s32 nC; s32 n10; u8 pad[0xC0 - 0x14]; Ent90_ *c0; u8 *c4; u8 *c8; u8 *cc; Ev90_ *d0; u8 *d4; } Hdr90_;
extern u32 D_8011C82C[];
extern char D_801009A0[];
extern s32 (*svcFileGetSize)(char *);
extern s32 (*svcFileLoad)(char *, void *);
s32 AnimTrack_Start(u32 index);

u8 Coll_LoadLevel(u8 *o) {
    char buf[0x100];
    s32 size;
    s32 r2;
    void *mem;
    Hdr90_ *h;
    Ent90_ *p;
    Sub90_ *q;
    Ev90_ *e;
    u32 i;
    u8 r;

    sprintf(buf, D_801009A0, o + 4, o[0x17]);
    size = svcFileGetSize(buf);
    if (size == 0) {
        return 0;
    }
    mem = Memory_Alloc(size);
    gCollLevel = mem;
    if (svcFileLoad(buf, mem) != size) {
        return 0;
    }

    ((Hdr90_ *)gCollLevel)->c0 = (Ent90_ *)((u8 *)gCollLevel + 0xD8);
    ((Hdr90_ *)gCollLevel)->c4 = (u8 *)((Hdr90_ *)gCollLevel)->c0 + ((Hdr90_ *)gCollLevel)->n0 * 0x14;
    ((Hdr90_ *)gCollLevel)->c8 = ((Hdr90_ *)gCollLevel)->c4 + ((Hdr90_ *)gCollLevel)->n4 * 0x18;
    ((Hdr90_ *)gCollLevel)->cc = ((Hdr90_ *)gCollLevel)->c8 + ((Hdr90_ *)gCollLevel)->n8 * 0xC;
    ((Hdr90_ *)gCollLevel)->d0 = (Ev90_ *)(((Hdr90_ *)gCollLevel)->cc + ((Hdr90_ *)gCollLevel)->nC * 0x14);
    ((Hdr90_ *)gCollLevel)->d4 = (u8 *)((Hdr90_ *)gCollLevel)->d0 + ((Hdr90_ *)gCollLevel)->n10 * 0xC;

    for (i = 0; i < D_8011C828; i++) {
        D_8011C830[i] = gCollLapLength;
    }

    p = ((Hdr90_ *)gCollLevel)->c0;

    for (i = 0; i < ((Hdr90_ *)gCollLevel)->n0; p++, i++) {
        if (p->flags & 1) {
            q = &p->s;
            if (q->val * 10 < D_8011C82C[q->idx]) {
                D_8011C82C[q->idx] = q->val * 10;
            }
        }
    }

    p = ((Hdr90_ *)gCollLevel)->c0;
    for (i = 0; i < ((Hdr90_ *)gCollLevel)->n0; p++, i++) {
        if (p->flags & 2) {
            e = &((Hdr90_ *)gCollLevel)->d0[p->s.idx];
            if (e->flags & 2) {
                r = AnimTrack_Start(e->h2);
                if (!r) {
                    return 0;
                }
                if (e->flags & 4) {
                    r = AnimTrack_Start(e->h4);
                    if (!r) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_LoadLevel);
#endif

s32 Coll_Init(u8 *o) {
    s32 v = *(s32 *)(o + 0x1C);

    gCollFrame = 100;
    D_8011C820 = v;
    gCollLapLength = Bld_GetLapLength();
    D_8011C828 = o[0x1A];
    CollList_InitPool();
    if (Coll_LoadLevel(o) == 0) {
        return 0;
    }
    svcWorldSetCollLevel(gCollLevel);
    TrackSfx_Init(gCollLevel->x10, gCollLevel->xD0);
    memset(gCollIdUsed, 0, 0x80);
    return 1;
}


void CollList_Clear(Nd3_ **list);
s32 Coll_Shutdown(void) {
    TrackSfx_Shutdown();
    CollList_Clear((Nd3_ **)&(*(s32 *)&gCollAll));
    CollList_Clear((Nd3_ **)&gCollMovers);
    CollList_Clear((Nd3_ **)&(*(s32 *)&gCollRacers));
    CollList_Clear((Nd3_ **)&gCollCars);
    CollList_Clear((Nd3_ **)&gCollBodies);
    CollList_Clear((Nd3_ **)&gCollPathCars);
    Memory_Free(gCollLevel);
    gCollLevel = NULL;
    return 1;
}


void Coll_AddCar(u8 *o) {
    u8 *n = Memory_Alloc(0x88);
    u8 i;

    memset(n, 0, 0x88);
    if (*(s32 *)(o + 0x6D8) == 2) {
        CollShape_InitCar(n + 0xC, o);
        *(s32 *)(n + 0x5C) = 3;
    } else if (*(s32 *)(o + 0x6D8) == 1) {
        CollShape_InitPoint(n + 0xC, o);
        *(s32 *)(n + 0x5C) = 4;
    }
    *(u8 **)(n + 0x60) = o + 0x2C;
    *(u8 **)(n + 0x64) = o;
    Coll_Register(n);
    for (i = 0; i < n[0x48]; i++) {
        Coll_ApplyRoomFlags(n, (*(u16 **)(n + 0x54))[i], 1);
    }
    *(u8 **)(o + 0x758) = n;
}

void Coll_RemoveById(s32 id) {
    Lnk95_ *n = gCollAll;
    Obj95_ *o;

    while (n != NULL) {
        o = n->obj;
        if (o->id == id) {
            break;
        }
        n = n->next;
    }
    if (n != NULL) {
        Coll_Unregister((s32 *)o);
        Memory_Free(o->a);
        Memory_Free(o->b);
        Memory_Free(o->c);
        Memory_Free(o);
    }
}


void CollList_Add(Nd4_ **list, s32 key);
void CollList_Remove(Nd4_ **list, s32 key);
void Coll_SetCarRoom(u8 *obj, s32 room) {
    RObj_ *o;
    u8 i;
    u16 k;

    o = *(RObj_ **)(obj + 0x758);
    for (i = 0; i < o->nRooms; i++) {
        k = o->rooms[i];
        CollList_Remove(&gCollLevel->rooms[k].list, (s32)o);
        CollList_Remove(&o->list, k);
    }
    for (i = 0; i < o->nRooms; i++) {
        o->rooms[i] = room;
        CollList_Add(&gCollLevel->rooms[room].list, (s32)o);
        CollList_Add(&o->list, room);
    }
    Coll_ApplyRoomFlags(o, room, 1);
}

void CollShape_InitCar(u8 *n, Car *o) {
    Vec3f c;
    Vec3f v;
    Vec3f t;
    Vec3f p;
    f32 m[3][4];
    Vec3f *q;
    u8 cnt;
    u8 i;
    f32 x;
    f32 y;
    f32 z;

    c.x = 0.0f;
    c.y = 0.0f;
    c.z = 0.0f;
    v = o->extent;
    cnt = o->nPoints;
    v.x *= 0.5f;
    v.y *= 0.5f;
    v.z *= 0.5f;
    *(Vec3f *)(n + 0xC) = v;
    *(f32 *)(n + 0xC) += 1.0f;
    *(f32 *)(n + 0x10) += 1.0f;
    *(f32 *)(n + 0x14) += 1.0f;
    n[0x3C] = cnt + 8;
    *(void **)(n + 0x40) = Memory_Alloc(n[0x3C] * 0xC);
    *(void **)(n + 0x44) = Memory_Alloc(n[0x3C] * 0xC);
    memset(*(void **)(n + 0x40), 0, n[0x3C] * 0xC);
    memset(*(void **)(n + 0x44), 0, n[0x3C] * 0xC);
    for (i = 0; i < cnt; i++) {
        (*(Vec3f **)(n + 0x40))[i].x = o->points[i].pos.x - o->center.x;
        (*(Vec3f **)(n + 0x40))[i].y = o->points[i].pos.y - o->center.y;
        (*(Vec3f **)(n + 0x40))[i].z = o->points[i].pos.z - o->center.z;
    }
    (*(Vec3f **)(n + 0x40))[cnt + 0].x = c.x + v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 0].y = c.y + v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 0].z = c.z + v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 1].x = c.x + v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 1].y = c.y + v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 1].z = c.z - v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 2].x = c.x + v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 2].y = c.y - v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 2].z = c.z + v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 3].x = c.x + v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 3].y = c.y - v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 3].z = c.z - v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 4].x = c.x - v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 4].y = c.y + v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 4].z = c.z + v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 5].x = c.x - v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 5].y = c.y + v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 5].z = c.z - v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 6].x = c.x - v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 6].y = c.y - v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 6].z = c.z + v.z;
    (*(Vec3f **)(n + 0x40))[cnt + 7].x = c.x - v.x;
    (*(Vec3f **)(n + 0x40))[cnt + 7].y = c.y - v.y;
    (*(Vec3f **)(n + 0x40))[cnt + 7].z = c.z - v.z;
    *(void **)(n + 0x48) = Memory_Alloc(n[0x3C] * 2);
    t.x = o->pos.x + o->v8C.x;
    t.y = o->pos.y + o->v8C.y;
    t.z = o->pos.z + o->v8C.z;
    m[0][0] = o->rot.m[0][0];
    m[0][1] = o->rot.m[0][1];
    m[0][2] = o->rot.m[0][2];
    m[0][3] = t.x;
    m[1][0] = o->rot.m[1][0];
    m[1][1] = o->rot.m[1][1];
    m[1][2] = o->rot.m[1][2];
    m[1][3] = t.y;
    m[2][0] = o->rot.m[2][0];
    m[2][1] = o->rot.m[2][1];
    m[2][2] = o->rot.m[2][2];
    m[2][3] = t.z;
    for (i = 0; i < n[0x3C]; i++) {
        x = (*(Vec3f **)(n + 0x40))[i].x;
        y = (*(Vec3f **)(n + 0x40))[i].y;
        z = (*(Vec3f **)(n + 0x40))[i].z;
        p.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
        p.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
        p.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
        (*(s16 **)(n + 0x48))[i] = Coll_FindRoom(p);
    }
    *(f32 *)(n + 0x4C) = sqrtf(*(f32 *)(n + 0xC) * *(f32 *)(n + 0xC) + *(f32 *)(n + 0x10) * *(f32 *)(n + 0x10) + *(f32 *)(n + 0x14) * *(f32 *)(n + 0x14));
}

void CollShape_InitPoint(u8 *n, u8 *o) {
    s32 zero[3];
    Vec3f v;
    Vec3f p;
    s16 id;
    u8 i;

    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    v = *(Vec3f *)(o + 0x5E8);
    v.x *= 0.5f;
    v.y *= 0.5f;
    v.z *= 0.5f;
    *(Vec3f *)(n + 0xC) = v;
    n[0x3C] = 1;
    *(void **)(n + 0x40) = Memory_Alloc(0xC);
    *(void **)(n + 0x44) = Memory_Alloc(n[0x3C] * 0xC);
    memset(*(void **)(n + 0x40), 0, n[0x3C] * 0xC);
    memset(*(void **)(n + 0x44), 0, n[0x3C] * 0xC);
    (*(s32 **)(n + 0x40))[0] = 0;
    (*(s32 **)(n + 0x40))[1] = 0;
    (*(s32 **)(n + 0x40))[2] = 0;
    *(void **)(n + 0x48) = Memory_Alloc(n[0x3C] * 2);
    p.x = *(f32 *)(o + 0x98) + *(f32 *)(o + 0x8C);
    p.y = *(f32 *)(o + 0x9C) + *(f32 *)(o + 0x90);
    p.z = *(f32 *)(o + 0xA0) + *(f32 *)(o + 0x94);
    id = Coll_FindRoom(p);
    for (i = 0; i < n[0x3C]; i++) {
        (*(s16 **)(n + 0x48))[i] = id;
    }
    *(f32 *)(n + 0x4C) = sqrtf(*(f32 *)(n + 0xC) * *(f32 *)(n + 0xC) + *(f32 *)(n + 0x10) * *(f32 *)(n + 0x10) + *(f32 *)(n + 0x14) * *(f32 *)(n + 0x14));
}

void CollShape_InitBox(u8 *n, Vec3f size, Vec3f pos, f32 *rot, u8 box) {
    f32 r[3][4];
    f32 m[3][4];
    Vec3f v;
    Vec3f p;
    Vec3f *q;
    u8 i;
    f32 x;
    f32 y;
    f32 z;

    v = size;
    v.x *= 0.5f;
    v.y *= 0.5f;
    v.z *= 0.5f;
    *(Vec3f *)(n + 0xC) = v;
    n[0x3C] = box ? 8 : 1;
    *(void **)(n + 0x40) = Memory_Alloc(n[0x3C] * 0xC);
    *(void **)(n + 0x44) = Memory_Alloc(n[0x3C] * 0xC);
    memset(*(void **)(n + 0x40), 0, n[0x3C] * 0xC);
    memset(*(void **)(n + 0x44), 0, n[0x3C] * 0xC);
    if (box) {
        (*(Vec3f **)(n + 0x40))[0].x = v.x;
        (*(Vec3f **)(n + 0x40))[0].y = v.y;
        (*(Vec3f **)(n + 0x40))[0].z = v.z;
        (*(Vec3f **)(n + 0x40))[1].x = v.x;
        (*(Vec3f **)(n + 0x40))[1].y = v.y;
        (*(Vec3f **)(n + 0x40))[1].z = -v.z;
        (*(Vec3f **)(n + 0x40))[2].x = v.x;
        (*(Vec3f **)(n + 0x40))[2].y = -v.y;
        (*(Vec3f **)(n + 0x40))[2].z = v.z;
        (*(Vec3f **)(n + 0x40))[3].x = v.x;
        (*(Vec3f **)(n + 0x40))[3].y = -v.y;
        (*(Vec3f **)(n + 0x40))[3].z = -v.z;
        (*(Vec3f **)(n + 0x40))[4].x = -v.x;
        (*(Vec3f **)(n + 0x40))[4].y = v.y;
        (*(Vec3f **)(n + 0x40))[4].z = v.z;
        (*(Vec3f **)(n + 0x40))[5].x = -v.x;
        (*(Vec3f **)(n + 0x40))[5].y = v.y;
        (*(Vec3f **)(n + 0x40))[5].z = -v.z;
        (*(Vec3f **)(n + 0x40))[6].x = -v.x;
        (*(Vec3f **)(n + 0x40))[6].y = -v.y;
        (*(Vec3f **)(n + 0x40))[6].z = v.z;
        (*(Vec3f **)(n + 0x40))[7].x = -v.x;
        (*(Vec3f **)(n + 0x40))[7].y = -v.y;
        (*(Vec3f **)(n + 0x40))[7].z = -v.z;
    } else {
        (*(s32 **)(n + 0x40))[0] = 0;
        (*(s32 **)(n + 0x40))[1] = 0;
        (*(s32 **)(n + 0x40))[2] = 0;
    }
    *(void **)(n + 0x48) = Memory_Alloc(n[0x3C] * 2);
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][1] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    m[0][3] = m[0][0] * pos.x + m[0][1] * pos.y + m[0][2] * pos.z + m[0][3];
    m[1][3] = m[1][0] * pos.x + m[1][1] * pos.y + m[1][2] * pos.z + m[1][3];
    m[2][3] = m[2][0] * pos.x + m[2][1] * pos.y + m[2][2] * pos.z + m[2][3];
    r[0][0] = m[0][0] * rot[0] + m[0][1] * rot[3] + m[0][2] * rot[6];
    r[0][1] = m[0][0] * rot[1] + m[0][1] * rot[4] + m[0][2] * rot[7];
    r[0][2] = m[0][0] * rot[2] + m[0][1] * rot[5] + m[0][2] * rot[8];
    r[0][3] = m[0][3];
    r[1][0] = m[1][0] * rot[0] + m[1][1] * rot[3] + m[1][2] * rot[6];
    r[1][1] = m[1][0] * rot[1] + m[1][1] * rot[4] + m[1][2] * rot[7];
    r[1][2] = m[1][0] * rot[2] + m[1][1] * rot[5] + m[1][2] * rot[8];
    r[1][3] = m[1][3];
    r[2][0] = m[2][0] * rot[0] + m[2][1] * rot[3] + m[2][2] * rot[6];
    r[2][1] = m[2][0] * rot[1] + m[2][1] * rot[4] + m[2][2] * rot[7];
    r[2][2] = m[2][0] * rot[2] + m[2][1] * rot[5] + m[2][2] * rot[8];
    r[2][3] = m[2][3];
    for (i = 0; i < n[0x3C]; i++) {
        x = (*(Vec3f **)(n + 0x40))[i].x;
        y = (*(Vec3f **)(n + 0x40))[i].y;
        z = (*(Vec3f **)(n + 0x40))[i].z;
        p.x = r[0][0] * x + r[0][1] * y + r[0][2] * z + r[0][3];
        p.y = r[1][0] * x + r[1][1] * y + r[1][2] * z + r[1][3];
        p.z = r[2][0] * x + r[2][1] * y + r[2][2] * z + r[2][3];
        (*(Vec3f **)(n + 0x44))[i] = p;
        (*(s16 **)(n + 0x48))[i] = Coll_FindRoom(p);
    }
    *(Vec3f *)(n + 0x0) = pos;
    ((f32 *)(n + 0x18))[0] = rot[0];
    ((f32 *)(n + 0x18))[1] = rot[1];
    ((f32 *)(n + 0x18))[2] = rot[2];
    ((f32 *)(n + 0x18))[3] = rot[3];
    ((f32 *)(n + 0x18))[4] = rot[4];
    ((f32 *)(n + 0x18))[5] = rot[5];
    ((f32 *)(n + 0x18))[6] = rot[6];
    ((f32 *)(n + 0x18))[7] = rot[7];
    ((f32 *)(n + 0x18))[8] = rot[8];
    *(f32 *)(n + 0x4C) = sqrtf(*(f32 *)(n + 0xC) * *(f32 *)(n + 0xC) + *(f32 *)(n + 0x10) * *(f32 *)(n + 0x10) + *(f32 *)(n + 0x14) * *(f32 *)(n + 0x14));
}

void CollShape_Free(Res40_ *r) {
    if (r->a != NULL) {
        Memory_Free(r->a);
    }
    if (r->b != NULL) {
        Memory_Free(r->b);
    }
    if (r->c != NULL) {
        Memory_Free(r->c);
    }
}


void CollList_Add(Nd4_ **list, s32 key);
void Coll_Register(RObj_ *o) {
    u32 i;
    u16 k;
    Room_ *r;
    Portal_ *pt;
    u32 lo;
    u32 hi;

    for (i = 0; i < o->nRooms; i++) {
        k = o->rooms[i];
        CollList_Add(&gCollLevel->rooms[k].list, (s32)o);
        CollList_Add(&o->list, k);
    }
    if (o->type == 4) {
        r = &gCollLevel->rooms[o->rooms[0]];
        pt = &gCollLevel->portals[r->firstPortal];
        for (i = 0; i < r->nPortals; i++, pt++) {
            if (pt->flag == 0) {
                ((void (*)(void *, void *, u16))CollList_Add)(&gCollLevel->rooms[pt->to].list, o, pt->to);
            }
        }
    }
    CollList_Add((Nd4_ **)&gCollAll, (s32)o);
    if ((o->type != 0) & (o->type != 5)) {
        CollList_Add((Nd4_ **)&gCollMovers, (s32)o);
    }
    if (o->type == 3) {
        CollList_Add((Nd4_ **)&gCollRacers, (s32)o);
    }
    if (o->type == 3 || o->type == 4) {
        CollList_Add((Nd4_ **)&gCollCars, (s32)o);
    }
    if (((o->type == 1) | (o->type == 3)) || o->type == 6) {
        CollList_Add((Nd4_ **)&gCollBodies, (s32)o);
    }
    if (o->type == 4) {
        CollList_Add((Nd4_ **)&gCollPathCars, (s32)o);
    }
    if ((o->type == 0) | (o->type == 5)) {
        lo = 0x20;
        hi = 0x80;
    } else {
        lo = 0;
        hi = 0x20;
    }
    for (i = lo; i < hi; i++) {
        if (gCollIdUsed[i] == 0) {
            o->id = i;
            gCollIdUsed[i] = 1;
            break;
        }
    }
}

void Coll_Unregister(s32 *obj) {
    RObj_ *o = (RObj_ *)obj;
    u32 i;
    u16 k;
    Room_ *r;
    Portal_ *pt;

    gCollIdUsed[o->id] = 0;
    CollList_Remove((Nd4_ **)&gCollAll, (s32)o);
    if ((o->type != 0) & (o->type != 5)) {
        CollList_Remove((Nd4_ **)&gCollMovers, (s32)o);
    }
    if (o->type == 3) {
        CollList_Remove((Nd4_ **)&gCollRacers, (s32)o);
    }
    if (o->type == 3 || o->type == 4) {
        CollList_Remove((Nd4_ **)&gCollCars, (s32)o);
    }
    if (((o->type == 1) | (o->type == 3)) || o->type == 6) {
        CollList_Remove((Nd4_ **)&gCollBodies, (s32)o);
    }
    if (o->type == 4) {
        CollList_Remove((Nd4_ **)&gCollPathCars, (s32)o);
    }
    for (i = 0; i < o->nRooms; i++) {
        k = o->rooms[i];
        CollList_Remove(&gCollLevel->rooms[k].list, (s32)o);
        CollList_Remove(&o->list, k);
    }
    if (o->type == 4) {
        r = &gCollLevel->rooms[o->rooms[0]];
        pt = &gCollLevel->portals[r->firstPortal];
        for (i = 0; i < r->nPortals; i++, pt++) {
            if (pt->flag == 0) {
                CollList_Remove(&gCollLevel->rooms[pt->to].list, (s32)o);
            }
        }
    }
    CollList_Clear((Nd3_ **)&o->list);
}

u16 Coll_FindRoom(Vec3f pos) {
    Vec3f p;
    Vec3f d;
    PlF_ pl;
    f32 best;
    f32 m;
    f32 dist;
    Room_ *w;
    Portal_ *l;
    u16 i;
    u16 j;
    u16 bi;

    best = -60000.0f;
    bi = 0;
    m = 0.0f;
    w = gCollLevel->rooms;
    for (i = 0; i < gCollLevel->nRooms; w++, i++) {
        if (!(w->flags & 0x800)) {
            p.x = w->x << 8;
            p.y = w->y << 8;
            p.z = w->z << 8;
            d.x = pos.x - p.x;
            d.y = pos.y - p.y;
            d.z = pos.z - p.z;
            l = &gCollLevel->portals[w->firstPortal];
            for (j = 0; j < w->nPortals; l++, j++) {
                pl.a = l->nx / 4096.0f;
                pl.b = l->ny / 4096.0f;
                pl.c = l->nz / 4096.0f;
                pl.d = l->d;
                pl.e = l->flag;
                pl.f = l->to;
                dist = d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d;
                if ((j == 0) | (dist < m)) {
                    m = dist;
                }
            }
            if (0.0f < m) {
                return i;
            }
            if (best < m) {
                best = m;
                bi = i;
            }
        }
    }
    return bi;
}

typedef struct {
    u8 pad0[4]; u32 flags; u8 pad8[0x4B0]; u8 b4B8; u8 pad4B9[0x205]; u8 b6BE; u8 b6BF; u8 pad6C0[0x50]; u8 b710; u8 pad711[0xF];
    Vec3f n720; u8 pad72C[0x44]; s32 w770;
} PlAB_;
typedef struct { u8 pad0[4]; u32 flags; u8 pad8[0x54]; s32 type; u8 pad60[4]; PlAB_ *pl; u8 pad68[0xC]; s32 frame; Vec3f pos; } ObjAB_;
typedef struct { ObjAB_ *obj; u8 pad4[0xC]; Vec3f pos; u8 idx; u8 pad1D[3]; } EntAB_;
typedef struct { ObjAB_ *a; ObjAB_ *b; u8 pad8[0x18]; } PairAB_;
typedef struct { Vec3f pos; f32 ext[3]; f32 m[3][3]; u8 cnt; u8 p3D[7]; Vec3f *verts; u8 p48[4]; f32 rad; } BoxC7_;
typedef struct { u8 p0[0x60]; Vec3f v60; Vec3f v6C; u8 p78[0xC]; Vec3f off; u8 p90[0x80]; u8 b110; } PrC7_;
typedef struct { s32 id; u32 flags; u8 p8[4]; BoxC7_ b; s32 type; PrC7_ *p60; Car *car; u8 p68[0x1C]; u32 w84; } ObjC7_;
typedef struct { ObjC7_ *a; ObjC7_ *b; Vec3f p; Vec3f n; } RecC7_;
typedef struct { f32 x; f32 y; f32 z; u8 pC[0x40]; f32 rad; } B_56C;
typedef struct { u8 p0[0x110]; u8 b110; } P60_56C;
typedef struct Ln_56C { void *obj; s32 x; struct Ln_56C *next; } Ln_56C;
typedef struct Ri_56C { s32 room; s32 x; struct Ri_56C *next; } Ri_56C;
typedef struct { s32 id; u32 flags; u8 p8[4]; B_56C b; s32 type; P60_56C *p60; s32 w64; s32 w68; s32 frame; Ri_56C *ri; } O_56C;
typedef struct { f32 a; f32 b; f32 c; f32 d; s16 e; u16 f; } Pl_67C;
typedef struct Ri_67C { s32 room; s32 x; struct Ri_67C *next; } Ri_67C;
typedef struct { u8 p0[0x110]; u8 b110; } P60_67C;
typedef struct { Vec3f pos; u8 pC[0x30]; u8 n; u8 p3D[7]; Vec3f *pts; u16 *rooms; } B_67C;
typedef struct { u8 p0[4]; u32 flags; u8 p8[4]; B_67C b; f32 rad; s32 type; P60_67C *p60; u8 p64[0xC]; Ri_67C *ri; } O_67C;
typedef struct Ln_67C { O_67C *obj; s32 x; struct Ln_67C *next; } Ln_67C;
typedef struct NdCD8_ { s32 a; struct NdCD8_ *prev; struct NdCD8_ *next; s32 c; } NdCD8_;
typedef struct { u8 p0[4]; u32 flags; } CrD57_;
typedef struct { u8 p0[4]; u32 flags; u8 p8[0x4C]; u16 *p54; u8 p58[0xC]; CrD57_ *car; } ObD57_;
typedef struct LnD57_ { ObD57_ *obj; s32 x; struct LnD57_ *next; } LnD57_;
void Car_StartCrash(Car *p, u8 id, u8 flip);
void Coll_ProbeWheels(void);
void Coll_FindGroundPlanes(void);
void Coll_PointsVsWorld(void);
void Coll_BoxCornersVsWorld(void);
void Coll_Solve(void);
extern NdCD8_ gCollNodePool[];
void CollList_InitPool(void);
extern u8 gCollTypeMask[][7];
void Pickup_Collect(s32, s32);
void Coll_BroadPhase(void);
extern u8 gCollSepCache[][128];
extern f32 D_8011B760[];
u8 CollBox_FindSeparatingAxis(BoxC7_ *, BoxC7_ *);
extern u16 gCollContactCount;
extern EntAB_ gCollContacts[];
extern u16 gCollPairCount;
extern PairAB_ gCollPairs[];
extern u8 D_800F38E4[];
extern void (*svcFxCarScrape)(EntAB_ *, f32);
void Coll_SyncObjects(void);
void RaceAudio_OnContact(EntAB_ *);
void Car_NoteGroundContact(PlAB_ *);
void Car_ReportImpact(EntAB_ *);
void Car_CheckCrashContact(EntAB_ *);
void Car_SelfRight(PlAB_ *, EntAB_ *);
s32 RaceClock_Get(void);
f32 RigidBody_ResolveWorldContact(EntAB_ *, f32);
void RaceAudio_OnCollision(PairAB_ *);
void Car_ReportPairImpact(PairAB_ *, s32);
void Car_CheckCrashPair(PairAB_ *, s32);
void RigidBody_ResolvePairContact(PairAB_ *);
void Replay_Record(void);
void Coll_Update(void) {
    u32 i;
    EntAB_ *e;
    PairAB_ *pr;
    PlAB_ *pl;
    PlAB_ *pa;
    PlAB_ *pb;
    u8 changed;
    u8 isRacer;
    u8 isCar;
    u8 flag;
    f32 f;
    f32 r;

    gCollPairCount = 0;
    gCollContactCount = 0;
    gCollFrame++;
    Coll_SyncObjects();
    Coll_BroadPhase();
    Coll_Solve();
    for (i = 0, e = gCollContacts; i < gCollContactCount; e++, i++) {
        changed = e->obj->frame != gCollFrame;
        if (changed) {
            RaceAudio_OnContact(e);
        }
        isRacer = e->obj->type == 3;
        isCar = 0;
        if (isRacer || e->obj->type == 4) {
            isCar = 1;
        }
        flag = 0;
        if (isCar) {
            flag = e->obj->pl->b4B8;
        }
        pl = e->obj->pl;
        if (isRacer && !flag) {
            if (changed) {
                Car_NoteGroundContact(pl);
                if (pl->flags & 1) {
                    Car_ReportImpact(e);
                }
            }
            Car_CheckCrashContact(e);
            if (pl->b710 && e->pos.x * pl->n720.x + e->pos.y * pl->n720.y + e->pos.z * pl->n720.z > 0.866f) {
                Car_SelfRight(pl, e);
            }
            pl->w770 = RaceClock_Get();
        }
        e->obj->frame = gCollFrame;
        e->obj->pos = e->pos;
        f = D_800F38E4[e->idx] / 100.0f;
        if (isCar && !isRacer && !flag) {
            f *= 4.0f;
        }
        r = RigidBody_ResolveWorldContact(e, f);
        if (isRacer & changed) {
            svcFxCarScrape(e, r);
        }
    }
    for (i = 0, pr = gCollPairs; i < gCollPairCount; pr++, i++) {
        pa = NULL;
        if (pr->a->type == 3 || pr->a->type == 4) {
            pa = pr->a->pl;
        }
        pb = NULL;
        if (pr->b->type == 3 || pr->b->type == 4) {
            pb = pr->b->pl;
        }
        RaceAudio_OnCollision(pr);
        if (pa != NULL) {
            if (pa->flags & 1) {
                Car_ReportPairImpact(pr, 0);
            }
            Car_CheckCrashPair(pr, 0);
        }
        if (pb != NULL) {
            if (pb->flags & 1) {
                Car_ReportPairImpact(pr, 1);
            }
            Car_CheckCrashPair(pr, 1);
        }
        if (pa != NULL) {
            if (pa->b6BE) {
                pr->a->flags |= 0x20;
            }
            if (pa->b6BF) {
                pr->a->flags |= 0x40;
            }
        }
        if (pb != NULL) {
            if (pb->b6BE) {
                pr->b->flags |= 0x20;
            }
            if (pb->b6BF) {
                pr->b->flags |= 0x40;
            }
        }
        if ((pr->a->flags & 0x20) && (pr->b->flags & 0x20)) {
            pr->a->flags &= ~0x20;
            pr->b->flags &= ~0x20;
        }
        if ((pr->a->flags & 0x40) && (pr->b->flags & 0x40)) {
            pr->a->flags &= ~0x40;
            pr->b->flags &= ~0x40;
        }
        RigidBody_ResolvePairContact(pr);
        if (pa != NULL) {
            pr->a->flags &= ~0x60;
        }
        if (pb != NULL) {
            pr->b->flags &= ~0x60;
        }
        if (pa != NULL && pb != NULL && (pa->flags & 1) && (pb->flags & 1)) {
            Replay_Record();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_SyncObjects);

void Coll_ResolveBoxPair(ObjC7_ *a, ObjC7_ *b);
#define ABS_56C(v) ((v) < 0.0f ? -(v) : (v))
#define MAX_56C(a, b) ((a) > (b) ? (a) : (b))
void Coll_BroadPhase(void) {
    f32 d[3];
    f32 r;
    f32 m;
    Ln_56C *it;
    Ln_56C *jt;
    Ri_56C *ri;
    O_56C *a;
    O_56C *b;
    B_56C *pa;
    B_56C *pb;
    s32 res;
    u8 busy;

    for (it = *(Ln_56C **)&gCollCars; it != NULL; it = it->next) {
        a = it->obj;
        if (a->flags & 1) {
            continue;
        }
        for (jt = it->next; jt != NULL; jt = jt->next) {
            b = jt->obj;
            if (b->flags & 1) {
                continue;
            }
            d[0] = b->b.x - a->b.x;
            d[1] = b->b.y - a->b.y;
            d[2] = b->b.z - a->b.z;
            pa = &a->b;
            pb = &b->b;
            m = MAX_56C(MAX_56C(ABS_56C(d[0]), ABS_56C(d[1])), ABS_56C(d[2]));
            r = pa->rad + pb->rad;
            if (m < r + r * 0.5f) {
                res = CollBox_FindSeparatingAxis(pa, pb);
                if (res != 0) {
                    gCollSepCache[a->id][b->id] = res;
                } else {
                    Coll_ResolveBoxPair(a, b);
                }
            }
        }
    }
    for (it = *(Ln_56C **)&gCollMovers; it != NULL; it = it->next) {
        a = it->obj;
        if ((a->p60 != NULL && a->p60->b110 != 0) || (a->flags & 1)) {
            continue;
        }
        for (ri = a->ri; ri != NULL; ri = ri->next) {
            Room_ *rm = &gCollLevel->rooms[ri->room];
            for (jt = rm->list; jt != NULL; jt = jt->next) {
                b = jt->obj;
                if (b == a || (b->flags & 1) || ((b->p60 == NULL || b->p60->b110 == 0) && b->frame == gCollFrame)) {
                    continue;
                }
                if (!gCollTypeMask[a->type][b->type]) {
                    continue;
                }
                d[0] = b->b.x - a->b.x;
                d[1] = b->b.y - a->b.y;
                d[2] = b->b.z - a->b.z;
                pa = &a->b;
                pb = &b->b;
                m = MAX_56C(MAX_56C(ABS_56C(d[0]), ABS_56C(d[1])), ABS_56C(d[2]));
                r = pa->rad + pb->rad;
                if (m < r + r * 0.5f) {
                    res = CollBox_FindSeparatingAxis(pa, pb);
                    if (res != 0) {
                        gCollSepCache[a->id][b->id] = res;
                    } else if (b->type == 5) {
                        Pickup_Collect(a->w64, b->w68);
                    } else {
                        Coll_ResolveBoxPair(a, b);
                    }
                }
            }
        }
        a->frame = gCollFrame;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", CollBox_FindSeparatingAxis);

void Coll_ResolveBoxPair(ObjC7_ *a, ObjC7_ *b) {
    Vec3f t;
    Vec3f u;
    Vec3f v;
    Vec3f n;
    BoxC7_ *pa;
    Vec3f *va;
    Vec3f *vb;
    u8 flag;
    BoxC7_ *pb;
    BoxC7_ *x;
    BoxC7_ *y;
    PrC7_ *qa;
    PrC7_ *qb;
    u8 code;
    Vec3f *p;
    f32 best;
    f32 d;
    f32 dd;
    u8 i;
    f32 inv;

    flag = 0;
    pb = &b->b;
    va = a->b.verts;
    pa = &a->b;
    vb = b->b.verts;
    x = pa;
    qa = a->p60;
    qb = b->p60;
    y = pb;
    if (a->type == 3) {
        va += a->car->nPoints;
        if (a->car->pickupFx[1] != 0 || (a->car->w540 & 1)) {
            flag = 1;
        }
    }
    if (b->type == 3) {
        vb += b->car->nPoints;
    }
    code = gCollSepCache[a->id][b->id];
    if (code == 0 && a->p60 != NULL) {
        t = a->b.pos;
        a->b.pos.x = t.x - a->p60->off.x;
        a->b.pos.y = t.y - a->p60->off.y;
        a->b.pos.z = t.z - a->p60->off.z;
        code = CollBox_FindSeparatingAxis(&a->b, &b->b);
        gCollSepCache[a->id][b->id] = code;
        a->b.pos = t;
    }
    if (code == 0) {
        n.x = x->pos.x - y->pos.x;
        n.y = x->pos.y - y->pos.y;
        n.z = x->pos.z - y->pos.z;
    } else {
        switch (code) {
            case 1: u.x = ( x)->m[0][ 1]; u.y = ( x)->m[1][ 1]; u.z = ( x)->m[2][ 1];; v.x = ( x)->m[0][ 2]; v.y = ( x)->m[1][ 2]; v.z = ( x)->m[2][ 2];; break;
            case 2: u.x = ( y)->m[0][ 1]; u.y = ( y)->m[1][ 1]; u.z = ( y)->m[2][ 1];; v.x = ( y)->m[0][ 2]; v.y = ( y)->m[1][ 2]; v.z = ( y)->m[2][ 2];; break;
            case 3: u.x = ( x)->m[0][ 2]; u.y = ( x)->m[1][ 2]; u.z = ( x)->m[2][ 2];; v.x = ( x)->m[0][ 0]; v.y = ( x)->m[1][ 0]; v.z = ( x)->m[2][ 0];; break;
            case 4: u.x = ( y)->m[0][ 2]; u.y = ( y)->m[1][ 2]; u.z = ( y)->m[2][ 2];; v.x = ( y)->m[0][ 0]; v.y = ( y)->m[1][ 0]; v.z = ( y)->m[2][ 0];; break;
            case 5: u.x = ( x)->m[0][ 0]; u.y = ( x)->m[1][ 0]; u.z = ( x)->m[2][ 0];; v.x = ( x)->m[0][ 1]; v.y = ( x)->m[1][ 1]; v.z = ( x)->m[2][ 1];; break;
            case 6: u.x = ( y)->m[0][ 0]; u.y = ( y)->m[1][ 0]; u.z = ( y)->m[2][ 0];; v.x = ( y)->m[0][ 1]; v.y = ( y)->m[1][ 1]; v.z = ( y)->m[2][ 1];; break;
            case 7: u.x = ( x)->m[0][ 0]; u.y = ( x)->m[1][ 0]; u.z = ( x)->m[2][ 0];; v.x = ( y)->m[0][ 0]; v.y = ( y)->m[1][ 0]; v.z = ( y)->m[2][ 0];; break;
            case 8: u.x = ( x)->m[0][ 0]; u.y = ( x)->m[1][ 0]; u.z = ( x)->m[2][ 0];; v.x = ( y)->m[0][ 1]; v.y = ( y)->m[1][ 1]; v.z = ( y)->m[2][ 1];; break;
            case 9: u.x = ( x)->m[0][ 0]; u.y = ( x)->m[1][ 0]; u.z = ( x)->m[2][ 0];; v.x = ( y)->m[0][ 2]; v.y = ( y)->m[1][ 2]; v.z = ( y)->m[2][ 2];; break;
            case 10: u.x = ( x)->m[0][ 1]; u.y = ( x)->m[1][ 1]; u.z = ( x)->m[2][ 1];; v.x = ( y)->m[0][ 0]; v.y = ( y)->m[1][ 0]; v.z = ( y)->m[2][ 0];; break;
            case 11: u.x = ( x)->m[0][ 1]; u.y = ( x)->m[1][ 1]; u.z = ( x)->m[2][ 1];; v.x = ( y)->m[0][ 1]; v.y = ( y)->m[1][ 1]; v.z = ( y)->m[2][ 1];; break;
            case 12: u.x = ( x)->m[0][ 1]; u.y = ( x)->m[1][ 1]; u.z = ( x)->m[2][ 1];; v.x = ( y)->m[0][ 2]; v.y = ( y)->m[1][ 2]; v.z = ( y)->m[2][ 2];; break;
            case 13: u.x = ( x)->m[0][ 2]; u.y = ( x)->m[1][ 2]; u.z = ( x)->m[2][ 2];; v.x = ( y)->m[0][ 0]; v.y = ( y)->m[1][ 0]; v.z = ( y)->m[2][ 0];; break;
            case 14: u.x = ( x)->m[0][ 2]; u.y = ( x)->m[1][ 2]; u.z = ( x)->m[2][ 2];; v.x = ( y)->m[0][ 1]; v.y = ( y)->m[1][ 1]; v.z = ( y)->m[2][ 1];; break;
            case 15: u.x = ( x)->m[0][ 2]; u.y = ( x)->m[1][ 2]; u.z = ( x)->m[2][ 2];; v.x = ( y)->m[0][ 2]; v.y = ( y)->m[1][ 2]; v.z = ( y)->m[2][ 2];; break;
        }
        n.x = u.y * v.z - v.y * u.z;
        n.y = v.x * u.z - u.x * v.z;
        n.z = u.x * v.y - v.x * u.y;
    }
    if (sqrtf(n.x * n.x + n.y * n.y + n.z * n.z) < 0.1f) {
        return;
    }
    inv = 1.0f / sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x *= inv;
    n.y *= inv;
    n.z *= inv;
    t.x = y->pos.x - x->pos.x;
    t.y = y->pos.y - x->pos.y;
    t.z = y->pos.z - x->pos.z;
    if (t.x * n.x + t.y * n.y + t.z * n.z < 0.0f) {
        n.x = -n.x;
        n.y = -n.y;
        n.z = -n.z;
    }
    if ((code == 1) | (code == 3) || code == 5) {
        if (code == 1) {
            t.x = n.x * x->ext[0];
            t.y = n.y * x->ext[0];
            t.z = n.z * x->ext[0];
        } else if (code == 3) {
            t.x = n.x * x->ext[1];
            t.y = n.y * x->ext[1];
            t.z = n.z * x->ext[1];
        } else {
            t.x = n.x * x->ext[2];
            t.y = n.y * x->ext[2];
            t.z = n.z * x->ext[2];
        }
        t.x += x->pos.x;
        t.y += x->pos.y;
        t.z += x->pos.z;
        dd = -(n.x * t.x + n.y * t.y + n.z * t.z);
        p = vb;
        best = 1000.0f;
        for (i = 0; i < ((pb->cnt) < ( 8) ? (pb->cnt) : ( 8)); p++, i++) {
            d = p->x * n.x + p->y * n.y + p->z * n.z + dd;
            if (d < 0.0f) {
                d = -d;
            }
            if (d < best) {
                best = d;
                t = *p;
            }
        }
        if (gCollPairCount < 16) {
            RecC7_ *r = &((RecC7_ *)&gCollPairs)[gCollPairCount];
            r->a = b;
            r->b = a;
            r->p = t;
            r->n = n;
            gCollPairCount++;
        }
    } else if ((code == 2) | (code == 4) || code == 6) {
        n.x = -n.x;
        n.y = -n.y;
        n.z = -n.z;
        if (code == 2) {
            t.x = n.x * y->ext[0];
            t.y = n.y * y->ext[0];
            t.z = n.z * y->ext[0];
        } else if (code == 4) {
            t.x = n.x * y->ext[1];
            t.y = n.y * y->ext[1];
            t.z = n.z * y->ext[1];
        } else {
            t.x = n.x * y->ext[2];
            t.y = n.y * y->ext[2];
            t.z = n.z * y->ext[2];
        }
        t.x += y->pos.x;
        t.y += y->pos.y;
        t.z += y->pos.z;
        dd = -(n.x * t.x + n.y * t.y + n.z * t.z);
        p = va;
        best = 1000.0f;
        for (i = 0; i < ((pa->cnt) < ( 8) ? (pa->cnt) : ( 8)); p++, i++) {
            d = p->x * n.x + p->y * n.y + p->z * n.z + dd;
            if (d < 0.0f) {
                d = -d;
            }
            if (d < best) {
                best = d;
                t = *p;
            }
        }
        if (gCollPairCount < 16) {
            RecC7_ *r = &((RecC7_ *)&gCollPairs)[gCollPairCount];
            r->a = a;
            r->b = b;
            r->p = t;
            r->n = n;
            gCollPairCount++;
        }
        n.x = -n.x;
        n.y = -n.y;
        n.z = -n.z;
    } else {
        t.x = y->pos.x - x->pos.x;
        t.y = y->pos.y - x->pos.y;
        t.z = y->pos.z - x->pos.z;
        t.x *= 0.5f;
        t.y *= 0.5f;
        t.z *= 0.5f;
        t.x = x->pos.x + t.x;
        t.y = x->pos.y + t.y;
        t.z = x->pos.z + t.z;
        if (gCollPairCount < 16) {
            RecC7_ *r = &((RecC7_ *)&gCollPairs)[gCollPairCount];
            r->a = b;
            r->b = a;
            r->p = t;
            r->n = n;
            gCollPairCount++;
        }
    }
    n.x *= D_8011B760[code];
    n.y *= D_8011B760[code];
    n.z *= D_8011B760[code];
    if ((((a)->type != 0) & ((a)->type != 2))) {
        if ((((b)->type != 0) & ((b)->type != 2))) {
            n.x *= 0.5f;
            n.y *= 0.5f;
            n.z *= 0.5f;
        }
        if ((((a)->type != 0) & ((a)->type != 2))) {
            if (!(b->flags & 4) && (!(b->flags & 2) || ((b->w84 >= 10000) & !flag))) {
                t.x = qa->v6C.x + qa->v60.x;
                t.y = qa->v6C.y + qa->v60.y;
                t.z = qa->v6C.z + qa->v60.z;
                t.x += n.x;
                t.y += n.y;
                t.z += n.z;
                qa->v6C.x = t.x - qa->v60.x;
                qa->v6C.y = t.y - qa->v60.y;
                qa->v6C.z = t.z - qa->v60.z;
                qa->b110 = 0;
            }
        }
    }
    n.x = -n.x;
    n.y = -n.y;
    n.z = -n.z;
    if ((((b)->type != 0) & ((b)->type != 2))) {
        t.x = qb->v6C.x + qb->v60.x;
        t.y = qb->v6C.y + qb->v60.y;
        t.z = qb->v6C.z + qb->v60.z;
        t.x += n.x;
        t.y += n.y;
        t.z += n.z;
        qb->v6C.x = t.x - qb->v60.x;
        qb->v6C.y = t.y - qb->v60.y;
        qb->v6C.z = t.z - qb->v60.z;
        qb->b110 = 0;
    }
}


void Coll_UpdateRooms(void);
void Coll_RacersVsWorldEdges(void);
void Coll_Solve(void) {
    LnD57_ *it;
    ObD57_ *o;
    CrD57_ *c;

    Coll_UpdateRooms();
    Coll_ProbeWheels();
    Coll_FindGroundPlanes();
    Coll_PointsVsWorld();
    Coll_BoxCornersVsWorld();
    Coll_RacersVsWorldEdges();
    for (it = *(LnD57_ **)&gCollCars; it != NULL; it = it->next) {
        o = it->obj;
        if (o->flags & 0x10) {
            Coll_ApplyRoomFlags((u8 *)o, *o->p54, 0);
            o->flags &= ~0x10;
        }
    }
    for (it = *(LnD57_ **)&gCollCars; it != NULL; it = it->next) {
        o = it->obj;
        c = o->car;
        if (c->flags & 0x800) {
            Car_StartCrash(c, 0, 1);
        } else if (c->flags & 0x1000) {
            Car_StartCrash(c, 0, 0);
        } else if (c->flags & 0x2000) {
            Car_StartCrash(c, 0, 0);
        }
    }
}

void CollList_Remove(Nd4_ **list, s32 key);
void Coll_UpdateRooms(void) {
    Pl_67C pl;
    Vec3f org;
    Vec3f d;
    Vec3f rel;
    s32 old[14];
    s32 new[14];
    Ln_67C *it;
    O_67C *o;
    B_67C *b;
    Ri_67C *ri;
    Room_ *rm;
    Portal_ *p;
    u32 n, i, k;
    s32 room;
    P60_67C *pp;
    u8 busy;
    f32 dot;

    for (it = ((Ln_67C *)gCollMovers); it != NULL; it = it->next) {
        o = it->obj;
        busy = 0;
        if ((pp = o->p60) != NULL) {
            busy = pp->b110;
        }
        if (busy) {
            continue;
        }
        n = o->b.n;
        b = &o->b;
        for (i = 0; i < n; i++) {
            new[i] = old[i] = b->rooms[i];
        }
        for (ri = o->ri; ri != NULL; ri = ri->next) {
            room = ri->room;
            rm = &gCollLevel->rooms[room];
            org.x = rm->x << 8;
            org.y = rm->y << 8;
            org.z = rm->z << 8;
            rel.x = o->b.pos.x - org.x;
            rel.y = o->b.pos.y - org.y;
            rel.z = o->b.pos.z - org.z;
            p = &gCollLevel->portals[rm->firstPortal];
            for (i = 0; i < rm->nPortals; p++, i++) {
                if (p->flag == 0) {
                    pl.a = p->nx / 4096.0f;
                    pl.b = p->ny / 4096.0f;
                    pl.c = p->nz / 4096.0f;
                    pl.d = p->d;
                    pl.e = p->flag;
                    pl.f = p->to;
                    dot = rel.x * pl.a + rel.y * pl.b + rel.z * pl.c + pl.d;
                    if (n == 1) {
                        if (dot < 0.0f) {
                            new[0] = pl.f;
                        }
                    } else if (dot < o->rad) {
                        for (k = 0; k < n; k++) {
                            if (old[k] == room) {
                                d.x = b->pts[k].x - org.x;
                                d.y = b->pts[k].y - org.y;
                                d.z = b->pts[k].z - org.z;
                                if ((dot = d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d) < 0.0f) {
                                    new[k] = pl.f;
                                }
                            }
                        }
                    }
                }
            }
        }
        for (i = 0; i < n; i++) {
            if (new[i] != old[i]) {
                CollList_Remove(&gCollLevel->rooms[old[i]].list, (s32)o);
                CollList_Remove(&o->ri, old[i]);
                b->rooms[i] = new[i];
                CollList_Add(&gCollLevel->rooms[new[i]].list, (s32)o);
                CollList_Add(&o->ri, new[i]);
                if (i == 0 && (o->type == 3 || o->type == 4)) {
                    o->flags |= 0x10;
                    if (o->type == 4) {
                        rm = &gCollLevel->rooms[old[0]];
                        p = &gCollLevel->portals[rm->firstPortal];
                        for (k = 0; k < rm->nPortals; p++, k++) {
                            if (p->flag == 0) {
                                CollList_Remove(&gCollLevel->rooms[p->to].list, (s32)o);
                            }
                        }
                        rm = &gCollLevel->rooms[new[0]];
                        p = &gCollLevel->portals[rm->firstPortal];
                        for (k = 0; k < rm->nPortals; p++, k++) {
                            if (p->flag == 0) {
                                CollList_Add(&gCollLevel->rooms[p->to].list, (s32)o);
                            }
                        }
                    }
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_ProbeWheels);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_FindGroundPlanes);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_PointsVsWorld);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_BoxCornersVsWorld);

void Coll_RacersVsWorldEdges(void) {
    LnkA2_ *n;

    for (n = gCollRacers; n != NULL; n = n->next) {
        ObjA2_ *o = n->obj;

        if (o->p60->b110 == 0 && o->p70->w8 != 0) {
            Coll_BoxVsWorldEdges(o);
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_BoxVsWorldEdges);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", CollBox_EdgeSeparatingAxis);

void Coll_ApplyRoomFlags(RObj_ *o, u16 idx, s32 arg2) {
    Room_ *r;
    Car *c;
    u32 m;

    r = &gCollLevel->rooms[idx];
    if (o->type == 3 || o->type == 4) {
        c = o->car;
        if (c->b4B8 == 0) {
            if ((r->flags & 1) && (u8)arg2 == 0) {
                Lap_PassCheckpoint(c->id, r->param.b[1], 1);
            }
            if (r->flags & 0x10) {
                c->flags2 |= 1;
            } else {
                c->flags2 &= ~1;
            }
            if ((r->flags & 8) || (D_8011C820 & 0x80)) {
                c->flags2 |= 2;
            } else {
                c->flags2 &= ~2;
            }
            c->flags &= ~0x1C;
            m = r->flags & 0x9000;
            if (m == 0x9000) {
                c->flags |= 0x10;
            } else if (m == 0x8000) {
                c->flags |= 8;
            } else if (m == 0x1000) {
                c->flags |= 4;
            }
            if (r->flags & 0x40) {
                c->flags |= 0x20;
            } else {
                c->flags &= ~0x20;
            }
            if (r->flags & 0x80) {
                c->flags |= 0x40;
            } else {
                c->flags &= ~0x40;
            }
            if (r->flags & 4) {
                c->flags |= 0x80;
            } else {
                c->flags &= ~0x80;
            }
            if (r->flags & 0x100) {
                c->flags |= 0x100;
            } else {
                c->flags &= ~0x100;
            }
            if (r->flags & 0x200) {
                c->flags |= 0x200;
            } else {
                c->flags &= ~0x200;
            }
            if (r->flags & 0x400) {
                c->flags |= 0x4000;
            } else {
                c->flags &= ~0x4000;
                c->unk474 = r->time * 10;
                if (c->unk474 < D_8011C830[c->checkpoint]) {
                    c->unk474 += gCollLapLength;
                }
            }
            if (r->flags & 0x2000) {
                c->flags |= 0x400;
            } else {
                c->up.x = 0.0f;
                c->up.y = 0.0f;
                c->flags &= ~0x400;
                c->up.z = -1.0f;
            }
            if (r->flags & 0x20) {
                c->topSpeed = r->param.val / 256.0f * 386.0f;
            } else {
                c->topSpeed = 386.0f;
            }
            if ((r->flags & 0x4000) && c->state == 2) {
                if (r->flags & 0x800) {
                    Car_Boost(c);
                } else {
                    Car_DirectionalBoost(c, r->param.val, r->b4);
                }
            }
            if (r->flags & 2) {
                TrackSfx_Trigger(r->param.val);
            }
        }
    }
}

void CollList_InitPool(void) {
    s32 i;

    gCollNodeFree = gCollNodePool;
    for (i = 0; i < 512; i++) {
        gCollNodePool[i].prev = (i != 0) ? &gCollNodePool[i - 1] : NULL;
        gCollNodePool[i].next = (i != 511) ? &gCollNodePool[i + 1] : NULL;
    }
}

void CollList_Clear(Nd3_ **list) {
    Nd3_ *n = *list;

    while (n != NULL) {
        Nd3_ *c = n;
        Nd3_ *old;

        n = n->next;
        old = (Nd3_ *)gCollNodeFree;
        c->prev = NULL;
        gCollNodeFree = c;
        c->next = old;
        old->prev = c;
    }
    memset(list, 0, 4);
}


void CollList_Add(Nd4_ **list, s32 key) {
    Nd4_ *n;
    Nd4_ *nx;

    for (n = *list; n != NULL; n = n->next) {
        if (n->a == key) {
            break;
        }
    }
    if (n == NULL) {
        n = (Nd4_ *)gCollNodeFree;
        nx = n->next;
        nx->prev = NULL;
        n->a = key;
        n->prev = NULL;
        n->next = *list;
        n->cnt = 0;
        *list = n;
        gCollNodeFree = nx;
        if (n->next != NULL) {
            n->next->prev = n;
        }
    }
    n->cnt++;
}


void CollList_Remove(Nd4_ **list, s32 key) {
    Nd4_ *n;
    Nd4_ *free;

    for (n = *list; n != NULL; n = n->next) {
        if (n->a == key) {
            break;
        }
    }
    if (n != NULL) {
        if (--n->cnt == 0) {
            if (n->next != NULL) {
                n->next->prev = n->prev;
            }
            if (n->prev != NULL) {
                n->prev->next = n->next;
            }
            if (n == *list) {
                *list = n->next;
            }
            free = (Nd4_ *)gCollNodeFree;
            n->prev = NULL;
            gCollNodeFree = n;
            n->next = free;
            free->prev = n;
        }
    }
}

/* Wrong-way detection: when every wheel touches the ground and the car moves forward through a room,
 * find the portal it is heading for (the one whose normal faces the velocity most); driving towards
 * a room with a lower time for over 500 ms raises the wrong-way flag. */
void Car_CheckWrongWay(Car *c, u32 dt) {
    Room_ *room;
    Portal_ *p;
    u32 i;
    s32 best;
    f32 bestDot;
    f32 dot;
    f32 fwdSpeed;
    PlF_ pl;
    Vec3f fwd;
    u16 curTime;
    u16 nextTime;

    bestDot = 0.0f;
    best = -1;
    if (c->cnt3E7 == c->nPoints) {
        fwd.x = c->rot.m[0][1];
        fwd.y = c->rot.m[1][1];
        fwd.z = c->rot.m[2][1];
        fwdSpeed = fwd.x * c->vel.x + fwd.y * c->vel.y + fwd.z * c->vel.z;
        room = &gCollLevel->rooms[c->roomObj->list->key];
        if (!(room->flags & 0x400) && fwdSpeed > 176.0f) {
            p = &gCollLevel->portals[room->firstPortal];
            for (i = 0; i < room->nPortals; p++, i++) {
                if (p->flag != 0) {
                    continue;
                }
                pl.a = p->nx / 4096.0f;
                pl.b = p->ny / 4096.0f;
                pl.c = p->nz / 4096.0f;
                pl.d = p->d;
                pl.e = p->flag;
                pl.f = p->to;
                dot = c->vel.x * pl.a + c->vel.y * pl.b + c->vel.z * pl.c;
                if (best == -1 || dot < bestDot) {
                    best = pl.f;
                    bestDot = dot;
                }
            }
            if (best == -1) {
                c->wrongWayTime = 0;
            } else {
                curTime = room->time;
                nextTime = gCollLevel->rooms[best].time;
                if (c->wrongWayTime != 0) {
                    c->wrongWayTime += dt;
                    if (nextTime < curTime) {
                        c->wrongWayTime = 0;
                    }
                } else if (curTime < nextTime) {
                    c->wrongWayTime = dt;
                }
            }
        } else {
            c->wrongWayTime = 0;
        }
    }
    if (c->wrongWayTime > 500) {
        c->wrongWay = 1;
    } else {
        c->wrongWay = 0;
    }
}

void Coll_GetStartPos(u8 i, V12_ *v, V16_ *q) {
    if (gCollLevel != NULL) {
        *v = ((Cam41_ *)gCollLevel)->v[i];
        *q = ((Cam41_ *)gCollLevel)->q[i];
    }
}


typedef struct { u16 flags; u16 quad; u8 pad4; s8 x; s8 y; s8 z; u8 pad8[2]; u8 n; u8 padB; u16 first; u8 padE[6]; } WpA41_;
typedef struct { s16 nx; s16 ny; s16 nz; s16 d; u16 to; u8 flag; u8 padB; } PlA41_;
typedef struct { f32 a; f32 b; f32 c; f32 d; s16 e; u16 f; } PlFA41_;
typedef struct { s16 v[4][3]; } QdA41_;
typedef struct { u16 pad0[2]; u16 cur; } P54A41_;
typedef struct { u8 pad0[0x54]; P54A41_ *p54; u8 pad58[4]; s32 type; u8 pad60[0x10]; s32 *p70; } OA41_;
#define WPS (*(WpA41_ **) ((u8 *) gCollLevel + 0xC0))
#define QDS (*(QdA41_ **) ((u8 *) gCollLevel + 0xC4))
#define PLS (*(PlA41_ **) ((u8 *) gCollLevel + 0xC8))
#define PLCONV(l) pl.a = (l).nx / 4096.0f; pl.b = (l).ny / 4096.0f; pl.c = (l).nz / 4096.0f; pl.d = (l).d; pl.e = (l).flag; pl.f = (l).to;
#define LERP(V, k) \
    t.x = q[0].v[k][0]; t.y = q[0].v[k][1]; t.z = q[0].v[k][2]; \
    s.x = t.x * r; s.y = t.y * r; s.z = t.z * r; \
    t.x = q[1].v[k][0]; t.y = q[1].v[k][1]; t.z = q[1].v[k][2]; \
    u.x = t.x * (1.0f - r); u.y = t.y * (1.0f - r); u.z = t.z * (1.0f - r); \
    V.x = s.x + u.x; V.y = s.y + u.y; V.z = s.z + u.z;
u8 Coll_ClipCamera(u8 id, Vec3f *pos) {
    PlFA41_ pl;
    Vec3f p;
    Vec3f d;
    Vec3f acc;
    Vec3f t;
    Vec3f s;
    Vec3f u;
    Vec3f v0;
    Vec3f v2;
    Vec3f v1;
    Vec3f v3;
    OA41_ *o;
    WpA41_ *w;
    PlA41_ *l;
    QdA41_ *q;
    u32 cnt;
    u8 found;
    s32 prev;
    s32 cur;
    u32 j;
    u8 hit;
    f32 d0;
    f32 d1;
    f32 r;
    f32 len;
    f32 dist;

    o = (OA41_ *)Car_Get(id)->roomObj;
    if (o->type == 3) {
        cur = o->p54->cur;
    } else {
        cur = *o->p70;
    }
    cnt = 0;
    found = 0;
    prev = -1;
    do {
        w = &WPS[cur];
        p.x = w->x << 8;
        p.y = w->y << 8;
        p.z = w->z << 8;
        d.x = pos->x - p.x;
        d.y = pos->y - p.y;
        d.z = pos->z - p.z;
        l = &PLS[w->first];
        for (j = 0; j < w->n; l++, j++) {
            if (l->flag == 0) {
                PLCONV(*l);
                if (d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d < 0.0f) {
                    if (pl.f == prev) {
                        found = 1;
                    }
                    prev = cur;
                    cur = pl.f;
                    break;
                }
            }
        }
        if (++cnt > 100) {
            return 0;
        }
        if (j == w->n) {
            found = 1;
        }
    } while (!found);
    w = &WPS[cur];
    p.x = w->x << 8;
    p.y = w->y << 8;
    p.z = w->z << 8;
    d.x = pos->x - p.x;
    d.y = pos->y - p.y;
    d.z = pos->z - p.z;
    acc.x = 0.0f;
    acc.y = 0.0f;
    acc.z = 0.0f;
    hit = 0;
    if (w->flags & 0x800) {
        PLCONV(PLS[w->first]);
        d0 = d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d;
        PLCONV(PLS[w->first + 1]);
        d1 = d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d;
        r = d1 / (d0 + d1);
        q = &QDS[w->quad];
        LERP(v0, 0);
        LERP(v1, 1);
        LERP(v2, 2);
        LERP(v3, 3);
        s.x = v2.x - v0.x;
        s.y = v2.y - v0.y;
        s.z = v2.z - v0.z;
        len = sqrtf(s.x * s.x + s.y * s.y + s.z * s.z);
        u.x = v1.x - v0.x;
        u.y = v1.y - v0.y;
        u.z = v1.z - v0.z;
        s.x *= 1.0f / len;
        s.y *= 1.0f / len;
        s.z *= 1.0f / len;
        len = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
        u.x *= 1.0f / len;
        u.y *= 1.0f / len;
        u.z *= 1.0f / len;
        t.x = d.x - v0.x;
        t.y = d.y - v0.y;
        t.z = d.z - v0.z;
        dist = t.x * s.x + t.y * s.y + t.z * s.z;
        if (dist < 48.0f) {
            t.x = s.x * (48.0f - dist);
            t.y = s.y * (48.0f - dist);
            t.z = s.z * (48.0f - dist);
            hit = 1;
            acc.x += t.x;
            acc.y += t.y;
            acc.z += t.z;
        }
        t.x = d.x - v0.x;
        t.y = d.y - v0.y;
        t.z = d.z - v0.z;
        dist = t.x * u.x + t.y * u.y + t.z * u.z;
        if (dist < 48.0f) {
            t.x = u.x * (48.0f - dist);
            t.y = u.y * (48.0f - dist);
            t.z = u.z * (48.0f - dist);
            hit = 1;
            acc.x += t.x;
            acc.y += t.y;
            acc.z += t.z;
        }
        t.x = v3.x - d.x;
        t.y = v3.y - d.y;
        t.z = v3.z - d.z;
        dist = t.x * s.x + t.y * s.y + t.z * s.z;
        if (dist < 48.0f) {
            t.x = s.x * (48.0f - dist);
            t.y = s.y * (48.0f - dist);
            t.z = s.z * (48.0f - dist);
            hit = 1;
            acc.x -= t.x;
            acc.y -= t.y;
            acc.z -= t.z;
        }
        t.x = v3.x - d.x;
        t.y = v3.y - d.y;
        t.z = v3.z - d.z;
        dist = t.x * u.x + t.y * u.y + t.z * u.z;
        if (dist < 48.0f) {
            t.x = u.x * (48.0f - dist);
            t.y = u.y * (48.0f - dist);
            t.z = u.z * (48.0f - dist);
            hit = 1;
            acc.x -= t.x;
            acc.y -= t.y;
            acc.z -= t.z;
        }
    } else {
        l = &PLS[w->first];
        for (j = 0; j < w->n; l++, j++) {
            if (l->flag != 0) {
                PLCONV(*l);
                dist = d.x * pl.a + d.y * pl.b + d.z * pl.c + pl.d;
                if (dist < 48.0f) {
                    t.x = pl.a * (48.0f - dist);
                    t.y = pl.b * (48.0f - dist);
                    t.z = pl.c * (48.0f - dist);
                    hit = 1;
                    acc.x += t.x;
                    acc.y += t.y;
                    acc.z += t.z;
                }
            }
        }
    }
    pos->x += acc.x;
    pos->y += acc.y;
    pos->z += acc.z;
    return hit;
}
#undef WPS
#undef QDS
#undef PLS
#undef PLCONV
#undef LERP

INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_GetRoomAxis);

s32 Coll_GetSpawnCount(void) {
    return gCollLevel->nSpawns;
}

s32 Coll_GetSpawn(s32 i) {
    return (s32)(gCollLevel->spawns + i * 0x18);
}
