#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
#include "math.h"
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
void Coll_BoxVsWorldEdges(void *);
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

typedef struct { u16 idx; u16 h4; u16 h6; u16 val; u8 pad[0x14 - 10]; } Sub90_;
typedef struct { u16 flags; Sub90_ s; } Ent90_;
typedef struct { u16 flags; u16 h2; u16 h4; u8 pad[6]; } Ev90_;
typedef struct { s32 n0; s32 n4; s32 n8; s32 nC; s32 n10; u8 pad[0xC0 - 0x14]; Ent90_ *c0; u8 *c4; u8 *c8; u8 *cc; Ev90_ *d0; u8 *d4; } Hdr90_;
typedef struct { void *p; } CollPtr_;
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

    /* TODO(match-debt): MEM_IN_STRUCT_P keeps the second gCollLevel load into $v1 before +0xD8. */
    ((Hdr90_ *)gCollLevel)->c0 = (Ent90_ *)((u8 *)((CollPtr_ *)&gCollLevel)->p + 0xD8);
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

typedef struct {
    /* 0x000 */ u8 p0[0x60];
    /* 0x060 */ Vec3f pos;
    /* 0x06C */ Vec3f offset;
    /* 0x078 */ u8 p78[0x18];
    /* 0x090 */ f32 speed;
    /* 0x094 */ f32 rot[9];
    /* 0x0B8 */ u8 pB8[0x3C];
    /* 0x0F4 */ f32 angSpeed;
    /* 0x0F8 */ u8 pF8[0x18];
    /* 0x110 */ u8 asleep;
    /* 0x111 */ u8 p111[3];
    /* 0x114 */ u32 sleepTimer;
} BodySync_;

typedef struct {
    /* 0x000 */ u8 p0[0x14];
    /* 0x014 */ f32 f14;
    /* 0x018 */ f32 f18;
    /* 0x01C */ u8 p1C[0x3C8];
    /* 0x3E4 */ u8 b3E4;
    /* 0x3E5 */ u8 p3E5[2];
    /* 0x3E7 */ u8 b3E7;
} CarSync_;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 pC[0xC];
    /* 0x18 */ f32 rot[9];
    /* 0x3C */ u8 nVerts;
    /* 0x3D */ u8 p3D[3];
    /* 0x40 */ Vec3f *localVerts;
    /* 0x44 */ Vec3f *worldVerts;
} ShapeSync_;

typedef struct {
    /* 0x00 */ u8 p0[0xC];
    /* 0x0C */ ShapeSync_ shape;
    /* 0x54 */ u8 p54[8];
    /* 0x5C */ s32 type;
    /* 0x60 */ BodySync_ *body;
    /* 0x64 */ CarSync_ *car;
} ObjSync_;

typedef struct LnkSync_ {
    ObjSync_ *obj;
    s32 x;
    struct LnkSync_ *next;
} LnkSync_;

#define TRANSFORM_SHAPE_(sp) { \
    s = (sp); \
    m[0] = s->rot[0]; \
    m[1] = s->rot[1]; \
    m[2] = s->rot[2]; \
    m[3] = s->pos.x; \
    m[4] = s->rot[3]; \
    m[5] = s->rot[4]; \
    m[6] = s->rot[5]; \
    m[7] = s->pos.y; \
    m[8] = s->rot[6]; \
    m[9] = s->rot[7]; \
    m[10] = s->rot[8]; \
    m[11] = s->pos.z; \
    for (i = 0, src = s->localVerts, dst = s->worldVerts; i < s->nVerts; src++, dst++, i++) { \
        f32 x = src->x; \
        f32 y = src->y; \
        f32 z = src->z; \
        dst->x = m[0] * x + m[1] * y + m[2] * z + m[3]; \
        dst->y = m[4] * x + m[5] * y + m[6] * z + m[7]; \
        dst->z = m[8] * x + m[9] * y + m[10] * z + m[11]; \
    } \
}

void Coll_SyncObjects(void) {
    LnkSync_ *it;
    ObjSync_ *o;
    BodySync_ *b;
    CarSync_ *c;
    ShapeSync_ *s;
    f32 m[12];
    u8 i;
    Vec3f *src;
    Vec3f *dst;

    for (it = (LnkSync_ *)gCollAll; it != NULL; it = it->next) {
        o = it->obj;
        if (o->type != 0 && o->type != 2 && o->type != 5) {
            b = o->body;
            if (!b->asleep) {
                f32 speed;
                f32 angSpeed;

                o->shape.pos.x = b->offset.x + b->pos.x;
                o->shape.pos.y = b->offset.y + b->pos.y;
                o->shape.pos.z = b->offset.z + b->pos.z;
                o->shape.rot[0] = b->rot[0];
                o->shape.rot[1] = b->rot[1];
                o->shape.rot[2] = b->rot[2];
                o->shape.rot[3] = b->rot[3];
                o->shape.rot[4] = b->rot[4];
                o->shape.rot[5] = b->rot[5];
                o->shape.rot[6] = b->rot[6];
                o->shape.rot[7] = b->rot[7];
                o->shape.rot[8] = b->rot[8];

                TRANSFORM_SHAPE_(&o->shape);

                speed = b->speed;
                angSpeed = b->angSpeed;
                if (o->type == 3) {
                    c = o->car;
                    if (c->b3E7 == c->b3E4) {
                        if (((speed < 24.0f) & (angSpeed < 0.17453279f)) &&
                            !(c->f14 < 0.0f) && !(c->f14 > 0.0f) &&
                            !(c->f18 < 0.0f) && !(c->f18 > 0.0f)) {
                            b->sleepTimer++;
                            if (b->sleepTimer >= 40) {
                                b->asleep = 1;
                            }
                        } else {
                            b->sleepTimer = 0;
                        }
                    }
                } else {
                    if ((speed < 96.0f) & (angSpeed < 1.570795f)) {
                        b->sleepTimer++;
                        if (b->sleepTimer >= 100) {
                            b->asleep = 1;
                        }
                    } else {
                        b->sleepTimer = 0;
                    }
                }
            }
        } else if (o->type == 2) {
            TRANSFORM_SHAPE_(&o->shape);
        }
    }
}

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

extern f32 D_8011B764[];

static inline Vec3f Mat3T_MulVec_FSA_(f32 m[3][3], Vec3f *v) {
    Vec3f r;
    r.x = m[0][0] * v->x + m[0][1] * v->y + m[0][2] * v->z;
    r.y = m[1][0] * v->x + m[1][1] * v->y + m[1][2] * v->z;
    r.z = m[2][0] * v->x + m[2][1] * v->y + m[2][2] * v->z;
    return r;
}

u8 CollBox_FindSeparatingAxis(BoxC7_ *a, BoxC7_ *b) {
    f32 mt[3][3];
    f32 R[3][3];
    Vec3f d;
    Vec3f t;
    f32 tx, ty, tz;
    f32 ax, ay, az;
    f32 bx, by, bz;
    f32 r00, r01, r02, r10, r11, r12, r20, r21, r22;
    f32 a00, a01, a02, a10, a11, a12, a20, a21, a22;
    f32 v;
    f32 sep;
    f32 eps = 0.001f;

    mt[0][0] = a->m[0][0]; mt[0][1] = a->m[1][0]; mt[0][2] = a->m[2][0];
    mt[1][0] = a->m[0][1]; mt[1][1] = a->m[1][1]; mt[1][2] = a->m[2][1];
    mt[2][0] = a->m[0][2]; mt[2][1] = a->m[1][2]; mt[2][2] = a->m[2][2];

    R[0][0] = mt[0][0] * b->m[0][0] + mt[0][1] * b->m[1][0] + mt[0][2] * b->m[2][0];
    R[0][1] = mt[0][0] * b->m[0][1] + mt[0][1] * b->m[1][1] + mt[0][2] * b->m[2][1];
    R[0][2] = mt[0][0] * b->m[0][2] + mt[0][1] * b->m[1][2] + mt[0][2] * b->m[2][2];
    R[1][0] = mt[1][0] * b->m[0][0] + mt[1][1] * b->m[1][0] + mt[1][2] * b->m[2][0];
    R[1][1] = mt[1][0] * b->m[0][1] + mt[1][1] * b->m[1][1] + mt[1][2] * b->m[2][1];
    R[1][2] = mt[1][0] * b->m[0][2] + mt[1][1] * b->m[1][2] + mt[1][2] * b->m[2][2];
    R[2][0] = mt[2][0] * b->m[0][0] + mt[2][1] * b->m[1][0] + mt[2][2] * b->m[2][0];
    R[2][1] = mt[2][0] * b->m[0][1] + mt[2][1] * b->m[1][1] + mt[2][2] * b->m[2][1];
    R[2][2] = mt[2][0] * b->m[0][2] + mt[2][1] * b->m[1][2] + mt[2][2] * b->m[2][2];

    d.x = b->pos.x - a->pos.x;
    d.y = b->pos.y - a->pos.y;
    d.z = b->pos.z - a->pos.z;
    t = Mat3T_MulVec_FSA_(mt, &d);

    tx = t.x; ty = t.y; tz = t.z;
    ax = a->ext[0]; ay = a->ext[1]; az = a->ext[2];
    bx = b->ext[0]; by = b->ext[1]; bz = b->ext[2];
    r00 = R[0][0]; r01 = R[0][1]; r02 = R[0][2];
    r10 = R[1][0]; r11 = R[1][1]; r12 = R[1][2];
    r20 = R[2][0]; r21 = R[2][1]; r22 = R[2][2];

    a00 = r00; if (a00 < 0.0f) a00 = -a00; a00 += eps;
    a01 = r01; if (a01 < 0.0f) a01 = -a01; a01 += eps;
    a02 = r02; if (a02 < 0.0f) a02 = -a02; a02 += eps;
    a10 = r10; if (a10 < 0.0f) a10 = -a10; a10 += eps;
    a11 = r11; if (a11 < 0.0f) a11 = -a11; a11 += eps;
    a12 = r12; if (a12 < 0.0f) a12 = -a12; a12 += eps;
    a20 = r20; if (a20 < 0.0f) a20 = -a20; a20 += eps;
    a21 = r21; if (a21 < 0.0f) a21 = -a21; a21 += eps;
    a22 = r22; if (a22 < 0.0f) a22 = -a22; a22 += eps;

    v = tx; if (v < 0.0f) v = -v;
    sep = v - (ax + (bx * a00 + (by * a01 + bz * a02)));
    if (sep > 0.0f) return 1;
    D_8011B764[0] = sep;

    v = tx * r00 + (ty * r10 + tz * r20); if (v < 0.0f) v = -v;
    sep = v - (bx + (ax * a00 + (ay * a10 + az * a20)));
    if (sep > 0.0f) return 2;
    D_8011B764[1] = sep;

    v = ty; if (v < 0.0f) v = -v;
    sep = v - (ay + (bx * a10 + (by * a11 + bz * a12)));
    if (sep > 0.0f) return 3;
    D_8011B764[2] = sep;

    v = tx * r01 + (ty * r11 + tz * r21); if (v < 0.0f) v = -v;
    sep = v - (by + (ax * a01 + (ay * a11 + az * a21)));
    if (sep > 0.0f) return 4;
    D_8011B764[3] = sep;

    v = tz; if (v < 0.0f) v = -v;
    sep = v - (az + (bx * a20 + (by * a21 + bz * a22)));
    if (sep > 0.0f) return 5;
    D_8011B764[4] = sep;

    v = tx * r02 + (ty * r12 + tz * r22); if (v < 0.0f) v = -v;
    sep = v - (bz + (ax * a02 + (ay * a12 + az * a22)));
    if (sep > 0.0f) return 6;
    D_8011B764[5] = sep;

    v = tz * r10 - ty * r20; if (v < 0.0f) v = -v;
    sep = v - (ay * a20 + (az * a10 + (by * a02 + bz * a01)));
    if (sep > 0.0f) return 7;
    D_8011B764[6] = sep;

    v = tz * r11 - ty * r21; if (v < 0.0f) v = -v;
    sep = v - (ay * a21 + (az * a11 + (bx * a02 + bz * a00)));
    if (sep > 0.0f) return 8;
    D_8011B764[7] = sep;

    v = tz * r12 - ty * r22; if (v < 0.0f) v = -v;
    sep = v - (ay * a22 + (az * a12 + (bx * a01 + by * a00)));
    if (sep > 0.0f) return 9;
    D_8011B764[8] = sep;

    v = tx * r20 - tz * r00; if (v < 0.0f) v = -v;
    sep = v - (ax * a20 + (az * a00 + (by * a12 + bz * a11)));
    if (sep > 0.0f) return 10;
    D_8011B764[9] = sep;

    v = tx * r21 - tz * r01; if (v < 0.0f) v = -v;
    sep = v - (ax * a21 + (az * a01 + (bx * a12 + bz * a10)));
    if (sep > 0.0f) return 11;
    D_8011B764[10] = sep;

    v = tx * r22 - tz * r02; if (v < 0.0f) v = -v;
    sep = v - (ax * a22 + (az * a02 + (bx * a11 + by * a10)));
    if (sep > 0.0f) return 12;
    D_8011B764[11] = sep;

    v = ty * r00 - tx * r10; if (v < 0.0f) v = -v;
    sep = v - (ax * a10 + (ay * a00 + (by * a22 + bz * a21)));
    if (sep > 0.0f) return 13;
    D_8011B764[12] = sep;

    v = ty * r01 - tx * r11; if (v < 0.0f) v = -v;
    sep = v - (ax * a11 + (ay * a01 + (bx * a22 + bz * a20)));
    if (sep > 0.0f) return 14;
    D_8011B764[13] = sep;

    v = ty * r02 - tx * r12; if (v < 0.0f) v = -v;
    sep = v - (ax * a12 + (ay * a02 + (bx * a21 + by * a20)));
    if (sep > 0.0f) return 15;
    D_8011B764[14] = sep;

    return 0;
}

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

/* TODO(match-debt): 899/1061 words (4244/4244 B exact, s67, up from 887/1061 in s63 via DotPW_(&carUp[id], &tmp)
 * and `dist = ...; if (dist < o->b.rad)` in Pass 3). Passes 1 (87/90) and 4 (162/164) match structurally 100%;
 * residual is frame 0x3F0 vs 0x400 (in Pass 2, GCC loop.c runs out of hoisting threshold 1 insn before moving
 * `id*12` out of the `rn` loop and `$sp+0x10` out of the `node` loop, keeping `($sp+0x10)+id*24` in $fp instead
 * of spilling to 0x394($sp) and forcing $fp/$s7->$t8 across passes). */
#ifdef NON_MATCHING
typedef struct {
    /* 0x00 */ f32 f154;
    /* 0x04 */ u8 flags;
    /* 0x05 */ u8 pad159[0x1B];
    /* 0x20 */ u8 contact;
    /* 0x21 */ u8 surfType;
    /* 0x22 */ u8 pad176[2];
    /* 0x24 */ Vec3f normal;
    /* 0x30 */ Vec3f point;
    /* 0x3C */ u8 pad190[0x0C];
    /* 0x48 */ f32 grip;
    /* 0x4C */ f32 force;
    /* 0x50 */ u8 pad1A4[4];
    /* 0x54 */ f32 compression;
    /* 0x58 */ u8 pad1AC[0x18];
} WheelPW_;

typedef struct {
    /* 0x000 */ u8 id;
    /* 0x001 */ u8 pad1[0xC7];
    /* 0x0C8 */ f32 fC8;
    /* 0x0CC */ u8 padCC[8];
    /* 0x0D4 */ f32 fD4;
    /* 0x0D8 */ u8 padD8[8];
    /* 0x0E0 */ f32 fE0;
    /* 0x0E4 */ u8 padE4[0x58];
    /* 0x13C */ u8 b13C;
    /* 0x13D */ u8 pad13D[0x17];
    /* 0x154 */ WheelPW_ wheels[4];
    /* 0x314 */ u8 pad314[0xD0];
    /* 0x3E4 */ u8 nWheels;
    /* 0x3E5 */ u8 pad3E5[0xD3];
    /* 0x4B8 */ u8 b4B8;
    /* 0x4B9 */ u8 pad4B9[0x3F];
    /* 0x4F8 */ f32 f4F8;
    /* 0x4FC */ u8 pad4FC[4];
    /* 0x500 */ f32 f500;
    /* 0x504 */ u8 pad504[0x0C];
    /* 0x510 */ f32 f510;
    /* 0x514 */ u8 pad514[4];
    /* 0x518 */ f32 f518;
    /* 0x51C */ u8 pad51C[0xD8];
    /* 0x5F4 */ f32 f5F4;
    /* 0x5F8 */ f32 f5F8;
    /* 0x5FC */ f32 f5FC;
} CarPW_;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 pad0C[0x38];
    /* 0x44 */ Vec3f *pts;
    /* 0x48 */ u16 *rooms;
    /* 0x4C */ f32 rad;
} BodyPW_;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ BodyPW_ b;
    /* 0x5C */ s32 type;
    /* 0x60 */ void *p60;
    /* 0x64 */ CarPW_ *car;
    /* 0x68 */ u8 pad68[8];
    /* 0x70 */ Nd4_ *list;
} RObjPW_;

typedef struct LnkPW_ {
    RObjPW_ *obj;
    s32 x;
    struct LnkPW_ *next;
} LnkPW_;

typedef struct {
    s16 x0, y0, z0;
    u8 p6[6];
    s16 x1, y1, z1;
    u8 p12[6];
} SegPW_;

typedef struct { Vec3f n; f32 d; u16 e; u16 f; } PlPW_;

static inline f32 DotPW_(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void Coll_ProbeWheels(void) {
    Vec3f tmp;
    Vec3f q0;
    Vec3f q1;
    Vec3f q2;
    Vec3f q3;
    Vec3f u;
    PlPW_ pl;
    Vec3f rpos;
    Vec3f wpos;
    Vec3f rel;
    Vec3f p0;
    Vec3f p1;
    Vec3f v;
    f32 bestDist[2][6];
    f32 minDist[2][6];
    f32 maxDist[2][6];
    u32 surfType[2][6];
    u32 surfGrip[2][6];
    Vec3f hitNormal[2][6];
    f32 wheelRad[2][6];
    Vec3f carUp[2];
    Vec3f n0;
    Vec3f n1;
    LnkPW_ *node;
    RObjPW_ *o;
    CarPW_ *car;
    BodyPW_ *b;
    Nd4_ *rn;
    Room_ *room;
    Portal_ *pt;
    SegPW_ *seg;
    u32 id;
    u32 nWheels;
    u32 i;
    u32 j;
    u32 roomIdx;
    f32 d0, d1, t, s, inv, dist, dotN, pen, force, k;

    for (node = (LnkPW_ *)gCollRacers; node != NULL; node = node->next) {
        o = node->obj;
        car = o->car;
        if (car->b13C == 0 && car->b4B8 == 0) {
            id = car->id;
            nWheels = car->nWheels;
            carUp[id].x = car->fC8;
            carUp[id].y = car->fD4;
            carUp[id].z = car->fE0;
            for (i = 0; i < nWheels; i++) {
                minDist[id][i] = (car->wheels[i].flags & 1) ? car->f5FC : car->f5F8;
                maxDist[id][i] = (car->wheels[i].flags & 1) ? car->f518 : car->f500;
                bestDist[id][i] = -minDist[id][i];
                wheelRad[id][i] = car->wheels[i].f154 * 0.5f;
            }
        }
    }

    for (node = (LnkPW_ *)gCollRacers; node != NULL; node = node->next) {
        o = node->obj;
        car = o->car;
        if (car->b13C == 0 && car->b4B8 == 0) {
            b = &o->b;
            nWheels = car->nWheels;
            id = car->id;
            for (rn = o->list; rn != NULL; rn = rn->next) {
                roomIdx = rn->a;
                room = &gCollLevel->rooms[roomIdx];
                if (room->flags & 0x800) {
                    f32 pd0, pd1;
                    rpos.x = (f32)(room->x << 8);
                    rpos.y = (f32)(room->y << 8);
                    rpos.z = (f32)(room->z << 8);
                    pt = &gCollLevel->portals[room->firstPortal];
                    n0.x = pt->nx / 4096.0f;
                    n0.y = pt->ny / 4096.0f;
                    n0.z = pt->nz / 4096.0f;
                    pd0 = pt->d;
                    pt++;
                    n1.x = pt->nx / 4096.0f;
                    n1.y = pt->ny / 4096.0f;
                    n1.z = pt->nz / 4096.0f;
                    pd1 = pt->d;
                    seg = (SegPW_ *)(*(u8 **)&gCollLevel->pC4 + room->param.val * 0x18);
                    q0.x = seg->x0;
                    q0.y = seg->y0;
                    q0.z = seg->z0;
                    q1.x = seg->x1;
                    q1.y = seg->y1;
                    q1.z = seg->z1;
                    seg++;
                    q2.x = seg->x0;
                    q2.y = seg->y0;
                    q2.z = seg->z0;
                    q3.x = seg->x1;
                    q3.y = seg->y1;
                    q3.z = seg->z1;
                    for (j = 0; j < nWheels; j++) {
                        if (b->rooms[j] == roomIdx) {
                            wpos.x = b->pts[j].x - rpos.x;
                            wpos.y = b->pts[j].y - rpos.y;
                            wpos.z = b->pts[j].z - rpos.z;
                            d0 = wpos.x * n0.x + wpos.y * n0.y + wpos.z * n0.z + pd0;
                            d1 = wpos.x * n1.x + wpos.y * n1.y + wpos.z * n1.z + pd1;
                            t = d1 / (d0 + d1);
                            s = 1.0f - t;
                            p0.x = q0.x * t + q2.x * s;
                            p0.y = q0.y * t + q2.y * s;
                            p0.z = q0.z * t + q2.z * s;
                            p1.x = q3.x * s + q1.x * t;
                            p1.y = q3.y * s + q1.y * t;
                            p1.z = q3.z * s + q1.z * t;
                            u.x = (p1.x - p0.x) * 0.01f;
                            u.y = (p1.y - p0.y) * 0.01f;
                            u.z = (p1.z - p0.z) * 0.01f;
                            tmp.x = wpos.x - p0.x;
                            tmp.y = wpos.y - p0.y;
                            tmp.z = wpos.z - p0.z;
                            d0 = u.x * tmp.x + u.y * tmp.y + u.z * tmp.z;
                            tmp.x = p1.x - wpos.x;
                            tmp.y = p1.y - wpos.y;
                            tmp.z = p1.z - wpos.z;
                            d1 = u.x * tmp.x + u.y * tmp.y + u.z * tmp.z;
                            t = d0 / (d0 + d1);
                            s = 1.0f - t;
                            v.x = (q2.x - q0.x) * s + (q3.x - q1.x) * t;
                            v.y = (q2.y - q0.y) * s + (q3.y - q1.y) * t;
                            v.z = (q2.z - q0.z) * s + (q3.z - q1.z) * t;
                            tmp.x = u.y * v.z - v.y * u.z;
                            tmp.y = v.x * u.z - u.x * v.z;
                            tmp.z = u.x * v.y - v.x * u.y;
                            inv = 1.0f / sqrtf(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
                            tmp.x *= inv;
                            tmp.y *= inv;
                            tmp.z *= inv;
                            u.x = wpos.x - p0.x;
                            u.y = wpos.y - p0.y;
                            u.z = wpos.z - p0.z;
                            dist = tmp.x * u.x + tmp.y * u.y + tmp.z * u.z;
                            dotN = DotPW_(&carUp[id], &tmp);
                            if (dotN > 0.9f) {
                                pen = -((dist - wheelRad[id][j]) / dotN);
                                if (pen > bestDist[id][j]) {
                                    bestDist[id][j] = pen;
                                    hitNormal[id][j] = tmp;
                                    surfType[id][j] = room->b4;
                                    surfGrip[id][j] = D_800F38E4[room->b4];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (node = (LnkPW_ *)gCollRacers; node != NULL; node = node->next) {
        o = node->obj;
        car = o->car;
        if (car->b13C == 0 && car->b4B8 == 0) {
            b = &o->b;
            nWheels = car->nWheels;
            id = car->id;
            for (rn = o->list; rn != NULL; rn = rn->next) {
                roomIdx = rn->a;
                room = &gCollLevel->rooms[roomIdx];
                if (!(room->flags & 0x800)) {
                    rpos.x = (f32)(room->x << 8);
                    rpos.y = (f32)(room->y << 8);
                    rpos.z = (f32)(room->z << 8);
                    rel.x = o->b.pos.x - rpos.x;
                    rel.y = o->b.pos.y - rpos.y;
                    rel.z = o->b.pos.z - rpos.z;
                    pt = &gCollLevel->portals[room->firstPortal];
                    for (i = 0; i < room->nPortals; pt++, i++) {
                        if (pt->flag != 0 && pt->flag != 1 && pt->flag != 3 && pt->flag != 4) {
                            pl.n.x = pt->nx / 4096.0f;
                            pl.n.y = pt->ny / 4096.0f;
                            pl.n.z = pt->nz / 4096.0f;
                            pl.d = pt->d;
                            pl.e = pt->flag;
                            pl.f = pt->to;
                            dist = rel.x * pl.n.x + rel.y * pl.n.y + rel.z * pl.n.z + pl.d;
                            if (dist < o->b.rad) {
                                for (j = 0; j < nWheels; j++) {
                                    if (b->rooms[j] == roomIdx) {
                                        wpos.x = b->pts[j].x - rpos.x;
                                        wpos.y = b->pts[j].y - rpos.y;
                                        wpos.z = b->pts[j].z - rpos.z;
                                        dist = wpos.x * pl.n.x + wpos.y * pl.n.y + wpos.z * pl.n.z + pl.d;
                                        dotN = carUp[id].x * pl.n.x + carUp[id].y * pl.n.y + carUp[id].z * pl.n.z;
                                        if (dotN > 0.9f) {
                                            pen = -((dist - car->wheels[j].f154 * 0.5f) / dotN);
                                            if (pen > bestDist[id][j]) {
                                                bestDist[id][j] = pen;
                                                hitNormal[id][j] = pl.n;
                                                surfType[id][j] = pl.e;
                                                surfGrip[id][j] = D_800F38E4[pl.e];
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (node = (LnkPW_ *)gCollRacers; node != NULL; node = node->next) {
        o = node->obj;
        car = o->car;
        if (car->b13C == 0 && car->b4B8 == 0) {
            b = &o->b;
            nWheels = car->nWheels;
            id = car->id;
            for (i = 0; i < nWheels; i++) {
                if (bestDist[id][i] > -minDist[id][i]) {
                    bestDist[id][i] = (bestDist[id][i] < maxDist[id][i]) ? bestDist[id][i] : maxDist[id][i];
                    if (car->wheels[i].flags & 1) {
                        k = car->f510;
                    } else {
                        k = car->f4F8;
                    }
                    force = car->f5F4 + bestDist[id][i] * k;
                    force = (force < 0.0f) ? 0.0f : force;
                    car->wheels[i].contact = 1;
                    car->wheels[i].surfType = surfType[id][i];
                    car->wheels[i].normal = hitNormal[id][i];
                    car->wheels[i].grip = (f32)surfGrip[id][i] / 100.0f;
                    car->wheels[i].force = force;
                    tmp.x = hitNormal[id][i].x * (car->wheels[i].f154 * -0.5f);
                    tmp.y = hitNormal[id][i].y * (car->wheels[i].f154 * -0.5f);
                    tmp.z = hitNormal[id][i].z * (car->wheels[i].f154 * -0.5f);
                    car->wheels[i].point.x = b->pts[i].x + tmp.x;
                    car->wheels[i].point.y = b->pts[i].y + tmp.y;
                    car->wheels[i].point.z = b->pts[i].z + tmp.z;
                } else {
                    car->wheels[i].contact = 0;
                }
                car->wheels[i].compression = bestDist[id][i];
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/unit_39C90", Coll_ProbeWheels);
#endif

typedef struct { Vec3f n; f32 d; s16 e; u16 f; } PlFGP_;
typedef struct {
    u8 p0[0x0C];
    s16 x0, y0, z0;
    s16 x1, y1, z1;
    u8 p18[0x0C];
    s16 x2, y2, z2;
    s16 x3, y3, z3;
} SegPairFGP_;
typedef struct {
    u8 contact;
    u8 pad[3];
    Vec3f normal;
    Vec3f point;
    u8 pad2[0x54];
} WheelFGP_; /* 0x70 */
typedef struct {
    /* 0x000 */ u8 id;
    /* 0x001 */ u8 p1[3];
    /* 0x004 */ u32 flags;
    /* 0x008 */ u8 p8[0x78];
    /* 0x080 */ Vec3f grav;
    /* 0x08C */ u8 p8C[0xE8];
    /* 0x174 */ WheelFGP_ wheels[4];
    /* 0x334 */ u8 p334[0xB0];
    /* 0x3E4 */ u8 nWheels;
    /* 0x3E5 */ u8 p3E5[0x32B];
    /* 0x710 */ u8 hasUp;
    /* 0x711 */ u8 p711[3];
    /* 0x714 */ Vec3f upOrigin;
    /* 0x720 */ Vec3f upNormal;
    /* 0x72C */ f32 upD;
    /* 0x730 */ u8 p730[0x14];
    /* 0x744 */ u8 hasAll;
    /* 0x745 */ u8 p745[3];
    /* 0x748 */ Vec3f allNormal;
    /* 0x754 */ f32 allD;
} CarFGP_;
typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ u8 p4[0x08];
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ u8 p18[0x44];
    /* 0x5C */ s32 type;
    /* 0x60 */ Obj60_ *p60;
    /* 0x64 */ CarFGP_ *car;
    /* 0x68 */ u8 p68[8];
    /* 0x70 */ Nd4_ *list;
} RObjFGP_;
typedef struct LnkFGP_ { RObjFGP_ *obj; s32 x; struct LnkFGP_ *next; } LnkFGP_;

void Coll_FindGroundPlanes(void) {
    Vec3f n;
    Vec3f wn;
    Vec3f a;
    Vec3f b;
    PlFGP_ pl;
    Vec3f rpos;
    Vec3f rel;
    Vec3f p0;
    Vec3f p1;
    s32 needRoom[6];
    LnkFGP_ *node;
    RObjFGP_ *o;
    CarFGP_ *car;
    Nd4_ *rn;
    Room_ *room;
    Portal_ *pt;
    s32 i;
    f32 bestUp = 0.0f;
    f32 bestAll = 0.0f;
    f32 dist;

    for (node = *(LnkFGP_ **)&gCollCars; node != NULL; node = node->next) {
        o = node->obj;
        if (o->p60->b110 == 0) {
            u8 id;
            car = o->car;
            id = car->id;
            car->hasUp = 0;
            car->hasAll = 0;
            needRoom[id] = 1;
            if (o->type == 3) {
                for (i = car->nWheels - 1; i >= 0; i--) {
                    if (car->wheels[i].contact != 0) {
                        wn = car->wheels[i].normal;
                        car->hasAll = 1;
                        car->allNormal = wn;
                        car->allD = -(wn.x * car->wheels[i].point.x + wn.y * car->wheels[i].point.y + wn.z * car->wheels[i].point.z);
                        if ((car->flags & 0x400) || wn.z > 0.5f) {
                            car->hasUp = 1;
                            car->upOrigin.x = 0.0f;
                            car->upOrigin.y = 0.0f;
                            car->upOrigin.z = 0.0f;
                            car->upNormal = wn;
                            car->upD = car->allD;
                            if (car->flags & 0x400) {
                                car->grav.x = -wn.x;
                                car->grav.y = -wn.y;
                                car->grav.z = -wn.z;
                            }
                            needRoom[id] = 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (node = *(LnkFGP_ **)&gCollCars; node != NULL; node = node->next) {
        o = node->obj;
        if (o->p60->b110 == 0) {
            car = o->car;
            if (needRoom[car->id] != 0) {
                for (rn = o->list; rn != NULL; rn = rn->next) {
                    room = &gCollLevel->rooms[rn->a];
                    rpos.x = (f32)(room->x << 8);
                    rpos.y = (f32)(room->y << 8);
                    rpos.z = (f32)(room->z << 8);
                    rel.x = o->pos.x - rpos.x;
                    rel.y = o->pos.y - rpos.y;
                    rel.z = o->pos.z - rpos.z;
                    if (room->flags & 0x800) {
                        f32 d0, d1, t, s, inv;
                        SegPairFGP_ *seg;

                        pl.n.x = gCollLevel->portals[room->firstPortal].nx / 4096.0f;
                        pl.n.y = gCollLevel->portals[room->firstPortal].ny / 4096.0f;
                        pl.n.z = gCollLevel->portals[room->firstPortal].nz / 4096.0f;
                        pl.d = gCollLevel->portals[room->firstPortal].d;
                        pl.e = gCollLevel->portals[room->firstPortal].flag;
                        pl.f = gCollLevel->portals[room->firstPortal].to;
                        d0 = rel.x * pl.n.x + rel.y * pl.n.y + rel.z * pl.n.z + pl.d;

                        pl.n.x = gCollLevel->portals[room->firstPortal + 1].nx / 4096.0f;
                        pl.n.y = gCollLevel->portals[room->firstPortal + 1].ny / 4096.0f;
                        pl.n.z = gCollLevel->portals[room->firstPortal + 1].nz / 4096.0f;
                        pl.d = gCollLevel->portals[room->firstPortal + 1].d;
                        pl.e = gCollLevel->portals[room->firstPortal + 1].flag;
                        pl.f = gCollLevel->portals[room->firstPortal + 1].to;
                        d1 = rel.x * pl.n.x + rel.y * pl.n.y + rel.z * pl.n.z + pl.d;

                        t = d1 / (d0 + d1);
                        s = 1.0f - t;
                        seg = (SegPairFGP_ *)(*(u8 **)&gCollLevel->pC4 + room->param.val * 0x18);
                        a.x = seg->x0;
                        a.y = seg->y0;
                        a.z = seg->z0;
                        b.x = seg->x2;
                        b.y = seg->y2;
                        b.z = seg->z2;
                        p0.x = a.x * t + b.x * s;
                        p0.y = a.y * t + b.y * s;
                        p0.z = a.z * t + b.z * s;

                        a.x = seg->x1;
                        a.y = seg->y1;
                        a.z = seg->z1;
                        b.x = seg->x3;
                        b.y = seg->y3;
                        b.z = seg->z3;
                        p1.x = a.x * t + b.x * s;
                        p1.y = a.y * t + b.y * s;
                        p1.z = a.z * t + b.z * s;

                        n.x = p1.x - p0.x;
                        n.y = p1.y - p0.y;
                        n.z = p1.z - p0.z;
                        inv = 1.0f / sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
                        n.x *= inv;
                        n.y *= inv;
                        n.z *= inv;

                        a.x = rel.x - p0.x;
                        a.y = rel.y - p0.y;
                        a.z = rel.z - p0.z;
                        dist = n.x * a.x + n.y * a.y + n.z * a.z;

                        if (car->hasAll == 0 || dist < bestAll) {
                            car->hasAll = 1;
                            car->allNormal = n;
                            car->allD = -(p0.x * n.x + p0.y * n.y + p0.z * n.z);
                            bestAll = dist;
                        }
                        if ((car->hasUp == 0 || dist < bestUp) &&
                            -(n.x * car->grav.x + n.y * car->grav.y + n.z * car->grav.z) > 0.5f) {
                            car->hasUp = 1;
                            car->upOrigin = rpos;
                            car->upNormal = n;
                            car->upD = -(p0.x * n.x + p0.y * n.y + p0.z * n.z);
                            bestUp = dist;
                        }
                        if (room->flags & 0x2000) {
                            car->grav.x = -n.x;
                            car->grav.y = -n.y;
                            car->grav.z = -n.z;
                        }
                    } else {
                        pt = &gCollLevel->portals[room->firstPortal];
                        for (i = 0; i < room->nPortals; pt++, i++) {
                            if (pt->flag != 0) {
                                pl.n.x = pt->nx / 4096.0f;
                                pl.n.y = pt->ny / 4096.0f;
                                pl.n.z = pt->nz / 4096.0f;
                                pl.d = pt->d;
                                pl.e = pt->flag;
                                pl.f = pt->to;
                                dist = rel.x * pl.n.x + rel.y * pl.n.y + rel.z * pl.n.z + pl.d;
                                if (pl.e != 1) {
                                    if (car->hasAll == 0 || dist < bestAll) {
                                        car->hasAll = 1;
                                        car->allNormal = pl.n;
                                        car->allD = pl.d;
                                        bestAll = dist;
                                    }
                                    if ((car->hasUp == 0 || dist < bestUp) && pl.n.z > 0.5f) {
                                        car->hasUp = 1;
                                        car->upOrigin = rpos;
                                        car->upNormal = pl.n;
                                        car->upD = pl.d;
                                        bestUp = dist;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (node = *(LnkFGP_ **)&gCollCars; node != NULL; node = node->next) {
        o = node->obj;
        if (o->p60->b110 == 0) {
            car = o->car;
            if (car->hasUp != 0 && car->hasAll != 0) {
                car->allNormal = car->upNormal;
                car->allD = car->upD - (car->upNormal.x * car->upOrigin.x + car->upNormal.y * car->upOrigin.y + car->upNormal.z * car->upOrigin.z);
            }
        }
    }
}

typedef struct {
    u8 p0[0x60];
    Vec3f pos;
    Vec3f offset;
    u8 p78[0xC];
    Vec3f vel;
    u8 p90[0x58];
    Vec3f angVel;
    u8 pF4[0x1C];
    u8 asleep;
    u8 p111[3];
} BodyPvw_;

typedef struct {
    u8 p0[4];
    u32 flags;
    u8 p8[0x3DC];
    u8 b3E4;
    u8 p3E5[3];
} CarPvw_;

typedef struct LnkRoomPvw_ {
    u32 roomIdx;
    s32 w4;
    struct LnkRoomPvw_ *next;
} LnkRoomPvw_;

typedef struct {
    Vec3f pos;
    u8 pC[0x30];
    u8 nVerts;
    u8 p3D[7];
    Vec3f *worldVerts;
    u16 *vertRooms;
} ShapePvw_;

typedef struct {
    u8 p0[0xC];
    ShapePvw_ shape;
    f32 radius;
    s32 type;
    BodyPvw_ *body;
    CarPvw_ *car;
    u8 p68[8];
    LnkRoomPvw_ *rooms;
} ObjPvw_;

typedef struct LnkBodyPvw_ {
    ObjPvw_ *obj;
    s32 w4;
    struct LnkBodyPvw_ *next;
} LnkBodyPvw_;

typedef struct {
    u16 flags;
    u16 quadIdx;
    u8 b4;
    s8 ox, oy, oz;
    u16 u8;
    u8 planeCount;
    u8 edgeCount;
    u16 planeStart;
    u16 edgeStart;
    u8 p10[4];
} RoomPvw_;

typedef struct {
    s16 nx, ny, nz;
    s16 d;
    u16 to;
    u8 surf;
    u8 bB;
} PlanePvw_;

typedef struct {
    Vec3f n;
    f32 d;
    s16 e;
    u16 f;
} PlFPvw_;

typedef struct {
    s16 v[4][3];
} QuadPvw_;

typedef struct {
    u8 p0[0xC0];
    RoomPvw_ *rooms;
    QuadPvw_ *quads;
    PlanePvw_ *planes;
} LevelPvw_;

typedef struct {
    ObjPvw_ *obj;
    Vec3f pos;
    Vec3f norm;
    u8 type;
    u8 pad[3];
} ContactPvw_;

static inline void Vec3_Add_P_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}

static inline void Vec3_Sub_P_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->x - b->x;
    dst->y = a->y - b->y;
    dst->z = a->z - b->z;
}

static inline void Vec3_Scale_P_(Vec3f *dst, Vec3f *v, f32 s) {
    dst->x = v->x * s;
    dst->y = v->y * s;
    dst->z = v->z * s;
}

static inline f32 Vec3_Dot_P_(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline void Vec3_Cross_P_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->y * b->z - b->y * a->z;
    dst->y = b->x * a->z - a->x * b->z;
    dst->z = a->x * b->y - b->x * a->y;
}

#define VEC3_SCALE_M(dst, v, s) \
    { \
        (dst).x = (v).x * (s); \
        (dst).y = (v).y * (s); \
        (dst).z = (v).z * (s); \
    }

#define CHECK_WALL_PVW(idx, ca, cb) \
    { \
        Vec3_Sub_P_(&wn, &(cb), &(ca)); \
        Vec3_Sub_P_(&wv, &rel, &(ca)); \
        wd[idx] = Vec3_Dot_P_(&wn, &wv); \
        if (!(wd[idx] > 0.0f) || obj->type == 6) { \
            f32 inv = 1.0f / sqrtf(Vec3_Dot_P_(&wn, &wn)); \
            Vec3_Scale_P_(&wn, &wn, inv); \
            wd[idx] = Vec3_Dot_P_(&wn, &wv); \
            wnorm[idx] = wn; \
            if (obj->type == 6) { \
                wd[idx] -= obj->radius; \
            } \
        } \
    }

void Coll_PointsVsWorld(void) {
    Vec3f push;
    Vec3f wn;
    Vec3f wv;
    PlFPvw_ pl;
    Vec3f origin;
    Vec3f rel;
    Vec3f objRel;
    Vec3f c0, c1, c2, c3;
    f32 wd[4];
    Vec3f wnorm[4];
    Vec3f pt;
    Vec3f q0_0, q1_0, q0_2, q1_2, q0_1, q1_1, q0_3, q1_3;
    Vec3f pn0;
    s16 pd0s;
    u16 to0;
    Vec3f pn1;
    s16 pd1s;
    u16 to1;
    Vec3f r1;
    Vec3f vrot1;
    Vec3f vrot2;
    f32 pd0;
    s16 pe0;
    u16 pf0;
    f32 pd1;
    s16 pe1;
    u16 pf1;
    LnkBodyPvw_ *bnode;
    ObjPvw_ *obj;
    BodyPvw_ *body;
    ShapePvw_ *shape;
    CarPvw_ *car;
    u32 startVert;
    Vec3f *vp;
    LnkRoomPvw_ *rnode;
    RoomPvw_ *room;
    PlanePvw_ *plane;
    QuadPvw_ *quad;
    u32 roomIdx;
    u32 i, j;
    f32 dist;
    f32 vn;
    ContactPvw_ *c;

    for (bnode = (LnkBodyPvw_ *)gCollBodies; bnode != NULL; bnode = bnode->next) {
        obj = bnode->obj;
        body = obj->body;
        if (body->asleep) {
            continue;
        }
        shape = &obj->shape;
        if (obj->type == 3) {
            car = obj->car;
            startVert = car->b3E4;
        } else {
            car = NULL;
            startVert = 0;
        }
        for (rnode = obj->rooms; rnode != NULL; rnode = rnode->next) {
            roomIdx = rnode->roomIdx;
            room = &((LevelPvw_ *)gCollLevel)->rooms[roomIdx];
            if (room->flags & 0x800) {
                continue;
            }
            origin.x = (f32)(room->ox << 8);
            origin.y = (f32)(room->oy << 8);
            origin.z = (f32)(room->oz << 8);
            Vec3_Sub_P_(&objRel, &shape->pos, &origin);
            plane = &((LevelPvw_ *)gCollLevel)->planes[room->planeStart];
            for (i = 0; i < room->planeCount; plane++, i++) {
                if (plane->surf != 0) {
                    pl.n.x = (f32)plane->nx / 4096.0f;
                    pl.n.y = (f32)plane->ny / 4096.0f;
                    pl.n.z = (f32)plane->nz / 4096.0f;
                    pl.d = (f32)plane->d;
                    pl.e = plane->surf;
                    pl.f = plane->to;
                    dist = Vec3_Dot_P_(&objRel, &pl.n) + pl.d;
                    if (dist < obj->radius) {
                        vp = &shape->worldVerts[startVert];
                        for (j = startVert; j < shape->nVerts; vp++, j++) {
                            if (shape->vertRooms[j] == roomIdx) {
                                pt = *vp;
                                if (obj->type == 6) {
                                    VEC3_SCALE_M(wn, pl.n, obj->radius);
                                    Vec3_Sub_P_(&pt, &pt, &wn);
                                }
                                Vec3_Sub_P_(&wn, &pt, &origin);
                                dist = Vec3_Dot_P_(&wn, &pl.n) + pl.d;
                                if (!(dist > 0.0f)) {
                                    if (((u16)pl.e == 3) & (car != NULL)) {
                                        car->flags |= 0x2000;
                                    }
                                    if (((u16)pl.e == 4) & (car != NULL)) {
                                        car->flags |= 0x1000;
                                    }
                                    Vec3_Sub_P_(&r1, &pt, &body->offset);
                                    Vec3_Cross_P_(&vrot1, &body->angVel, &r1);
                                    Vec3_Add_P_(&wn, &body->vel, &vrot1);
                                    vn = Vec3_Dot_P_(&wn, &pl.n);
                                    if (vn < 0.0f) {
                                        if (gCollContactCount < 16) {
                                            c = &((ContactPvw_ *)gCollContacts)[gCollContactCount];
                                            c->obj = obj;
                                            c->pos = pt;
                                            c->norm = pl.n;
                                            c->type = pl.e;
                                            gCollContactCount++;
                                        }
                                    }
                                    if (-dist > 50.0f) {
                                        if (car != NULL) {
                                            car->flags |= 0x800;
                                        } else {
                                            body->asleep = 1;
                                        }
                                    } else {
                                        VEC3_SCALE_M(push, pl.n, -dist);
                                        Vec3_Add_P_(&body->offset, &body->offset, &push);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (bnode = (LnkBodyPvw_ *)gCollBodies; bnode != NULL; bnode = bnode->next) {
        obj = bnode->obj;
        body = obj->body;
        if (body->asleep) {
            continue;
        }
        shape = &obj->shape;
        if (obj->type == 3) {
            car = obj->car;
            startVert = car->b3E4;
        } else {
            car = NULL;
            startVert = 0;
        }
        for (rnode = obj->rooms; rnode != NULL; rnode = rnode->next) {
            roomIdx = rnode->roomIdx;
            room = &((LevelPvw_ *)gCollLevel)->rooms[roomIdx];
            if (!(room->flags & 0x800)) {
                continue;
            }
            origin.x = (f32)(room->ox << 8);
            origin.y = (f32)(room->oy << 8);
            origin.z = (f32)(room->oz << 8);
            plane = &((LevelPvw_ *)gCollLevel)->planes[room->planeStart];
            pn0.x = (f32)plane->nx / 4096.0f;
            pn0.y = (f32)plane->ny / 4096.0f;
            pn0.z = (f32)plane->nz / 4096.0f;
            pd0 = (f32)plane->d;
            pe0 = plane->surf;
            pf0 = plane->to;
            plane++;
            pn1.x = (f32)plane->nx / 4096.0f;
            pn1.y = (f32)plane->ny / 4096.0f;
            pn1.z = (f32)plane->nz / 4096.0f;
            pd1 = (f32)plane->d;
            pe1 = plane->surf;
            pf1 = plane->to;

            quad = &((LevelPvw_ *)gCollLevel)->quads[room->quadIdx];
            q0_0.x = (f32)quad->v[0][0]; q0_0.y = (f32)quad->v[0][1]; q0_0.z = (f32)quad->v[0][2];
            q0_2.x = (f32)quad->v[2][0]; q0_2.y = (f32)quad->v[2][1]; q0_2.z = (f32)quad->v[2][2];
            q0_1.x = (f32)quad->v[1][0]; q0_1.y = (f32)quad->v[1][1]; q0_1.z = (f32)quad->v[1][2];
            q0_3.x = (f32)quad->v[3][0]; q0_3.y = (f32)quad->v[3][1]; q0_3.z = (f32)quad->v[3][2];
            quad++;
            q1_0.x = (f32)quad->v[0][0]; q1_0.y = (f32)quad->v[0][1]; q1_0.z = (f32)quad->v[0][2];
            q1_2.x = (f32)quad->v[2][0]; q1_2.y = (f32)quad->v[2][1]; q1_2.z = (f32)quad->v[2][2];
            q1_1.x = (f32)quad->v[1][0]; q1_1.y = (f32)quad->v[1][1]; q1_1.z = (f32)quad->v[1][2];
            q1_3.x = (f32)quad->v[3][0]; q1_3.y = (f32)quad->v[3][1]; q1_3.z = (f32)quad->v[3][2];

            {
                vp = &shape->worldVerts[startVert];
                for (i = startVert; i < shape->nVerts; vp++, i++) {
                    if (shape->vertRooms[i] == roomIdx) {
                        f32 d0, d1, t;

                        Vec3_Sub_P_(&rel, vp, &origin);
                        d0 = Vec3_Dot_P_(&rel, &pn0) + pd0;
                        d1 = Vec3_Dot_P_(&rel, &pn1) + pd1;
                        t = d1 / (d0 + d1);

                        c0.x = q0_0.x * t + q1_0.x * (1.0f - t);
                        c0.y = q0_0.y * t + q1_0.y * (1.0f - t);
                        c0.z = q0_0.z * t + q1_0.z * (1.0f - t);
                        c2.x = q0_2.x * t + q1_2.x * (1.0f - t);
                        c2.y = q0_2.y * t + q1_2.y * (1.0f - t);
                        c2.z = q0_2.z * t + q1_2.z * (1.0f - t);
                        c1.x = q0_1.x * t + q1_1.x * (1.0f - t);
                        c1.y = q0_1.y * t + q1_1.y * (1.0f - t);
                        c1.z = q0_1.z * t + q1_1.z * (1.0f - t);
                        c3.x = q0_3.x * t + q1_3.x * (1.0f - t);
                        c3.y = q0_3.y * t + q1_3.y * (1.0f - t);
                        c3.z = q0_3.z * t + q1_3.z * (1.0f - t);

                        CHECK_WALL_PVW(0, c2, c3);
                        CHECK_WALL_PVW(1, c0, c2);
                        CHECK_WALL_PVW(2, c1, c0);
                        CHECK_WALL_PVW(3, c3, c1);

                        for (j = 0; j < 4; j++) {
                            if (!(wd[j] > 0.0f)) {
                                pt = *vp;
                                if (obj->type == 6) {
                                    VEC3_SCALE_M(wn, wnorm[j], obj->radius);
                                    Vec3_Sub_P_(&pt, &pt, &wn);
                                }
                                Vec3_Sub_P_(&r1, &pt, &body->offset);
                                Vec3_Cross_P_(&vrot2, &body->angVel, &r1);
                                Vec3_Add_P_(&wn, &body->vel, &vrot2);
                                vn = wn.x * wnorm[j].x + wn.y * wnorm[j].y + wn.z * wnorm[j].z;
                                if (vn < 0.0f) {
                                    if (gCollContactCount < 16) {
                                        c = &((ContactPvw_ *)gCollContacts)[gCollContactCount];
                                        c->obj = obj;
                                        c->pos = pt;
                                        c->norm = wnorm[j];
                                        if (j != 0) {
                                            c->type = 1;
                                        } else {
                                            c->type = 2;
                                        }
                                        gCollContactCount++;
                                    }
                                }
                                if (-wd[j] > 50.0f) {
                                    if (car != NULL) {
                                        car->flags |= 0x800;
                                    } else {
                                        body->asleep = 1;
                                    }
                                } else {
                                    Vec3_Add_P_(&wn, &body->offset, &body->pos);
                                    VEC3_SCALE_M(push, wnorm[j], -wd[j]);
                                    Vec3_Add_P_(&wn, &wn, &push);
                                    Vec3_Sub_P_(&body->offset, &wn, &body->pos);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#undef VEC3_SCALE_M
#undef CHECK_WALL_PVW

typedef struct {
    u8 p0[0x60];
    Vec3f pos;
    Vec3f offset;
    u8 p78[0xC];
    Vec3f vel;
    u8 p90[0x58];
    Vec3f angVel;
    u8 pF4[0x1C];
    u8 asleep;
} BodyBcw_;

typedef struct {
    u8 p0[0x4];
    u32 flags;
    u8 p8[0xB8];
    f32 m[3][3];
    u8 pE4[0x397];
    u8 b47B;
    u8 p47C[0x3C];
    u8 b4B8;
    u8 p4B9[0x12B];
    f32 f5E4;
} CarBcw_;

typedef struct {
    Vec3f pos;
    f32 hx;
    f32 hy;
} ShapeBcw_;

typedef struct {
    u8 p0[0xC];
    ShapeBcw_ shape;
    u8 p20[0x34];
    u16 *p54;
    u8 p58[0x8];
    BodyBcw_ *body;
    CarBcw_ *car;
} ObjBcw_;

typedef struct LnkBcw_ {
    ObjBcw_ *obj;
    s32 w4;
    struct LnkBcw_ *next;
} LnkBcw_;

typedef struct {
    u16 flags;
    u16 quadIdx;
    u8 b4;
    s8 ox, oy, oz;
    u8 b8, b9;
    u8 planeCount;
    u8 bB;
    u16 planeStart;
    u8 pE[6];
} RoomBcw_;

typedef struct {
    s16 nx, ny, nz;
    s16 d;
    u16 to;
    u8 surf;
    u8 pad;
} PlaneBcw_;

typedef struct {
    s16 v[4][3];
} QuadBcw_;

typedef struct {
    u8 p0[0xC0];
    RoomBcw_ *rooms;
    QuadBcw_ *quads;
    PlaneBcw_ *planes;
} LevelBcw_;

typedef struct {
    Vec3f n;
    f32 d;
    u16 e;
    u16 f;
} PlFBcw_;

typedef struct {
    ObjBcw_ *obj;
    Vec3f pos;
    Vec3f norm;
    u8 type;
    u8 pad[3];
} ContactBcw_;

static inline void Vec3_Add_B_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}

static inline void Vec3_Sub_B_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->x - b->x;
    dst->y = a->y - b->y;
    dst->z = a->z - b->z;
}

static inline void Vec3_Scale_B_(Vec3f *dst, Vec3f *a, f32 s) {
    dst->x = a->x * s;
    dst->y = a->y * s;
    dst->z = a->z * s;
}

static inline void Vec3_Neg_B_(Vec3f *dst, Vec3f *a) {
    dst->x = -a->x;
    dst->y = -a->y;
    dst->z = -a->z;
}

static inline f32 Vec3_Dot_B_(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline void Vec3_Cross_B_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->y * b->z - b->y * a->z;
    dst->y = b->x * a->z - a->x * b->z;
    dst->z = a->x * b->y - b->x * a->y;
}

#define VEC3_SCALE_M(dst, v, s) \
    { \
        (dst).x = (v).x * (s); \
        (dst).y = (v).y * (s); \
        (dst).z = (v).z * (s); \
    }

static inline f32 SubAbs_B_(f32 d, f32 p) {
    if (p < 0.0f) {
        return d + p;
    }
    return d - p;
}

#define CHECK_WALL_BCW(idx, ca, cb) \
    { \
        f32 inv; \
        Vec3_Sub_B_(&wn, &(cb), &(ca)); \
        Vec3_Sub_B_(&wv, &rel, &(ca)); \
        inv = 1.0f / sqrtf(Vec3_Dot_B_(&wn, &wn)); \
        Vec3_Scale_B_(&wn, &wn, inv); \
        wd[idx] = Vec3_Dot_B_(&wn, &wv); \
        wd[idx] = SubAbs_B_(wd[idx], Vec3_Dot_B_(&wn, &ax0)); \
        wd[idx] = SubAbs_B_(wd[idx], Vec3_Dot_B_(&wn, &ax1)); \
        wd[idx] = SubAbs_B_(wd[idx], Vec3_Dot_B_(&wn, &ax2)); \
        wnorm[idx] = wn; \
    }

void Coll_BoxCornersVsWorld(void) {
    Vec3f push;
    Vec3f wn;
    Vec3f wv;
    PlFBcw_ pl;
    Vec3f origin;
    Vec3f rel;
    Vec3f c0, c1, c2, c3;
    f32 wd[4];
    Vec3f wnorm[4];
    Vec3f pt;
    Vec3f q0_0, q1_0, q0_2, q1_2, q0_1, q1_1, q0_3, q1_3;
    Vec3f pn0;
    Vec3f pn1;
    Vec3f ax0, ax1, ax2;
    Vec3f r1;
    Vec3f vrot1;
    Vec3f vrot2;
    LnkBcw_ *bnode;
    ObjBcw_ *obj;
    BodyBcw_ *body;
    ShapeBcw_ *shape;
    CarBcw_ *car;
    RoomBcw_ *room;
    PlaneBcw_ *plane;
    QuadBcw_ *quad;
    u32 i, k;
    f32 dist;
    f32 vn;
    ContactBcw_ *c;

    for (bnode = (LnkBcw_ *)gCollPathCars; bnode != NULL; bnode = bnode->next) {
        obj = bnode->obj;
        body = obj->body;
        if (body->asleep) {
            continue;
        }
        car = obj->car;
        shape = &obj->shape;
        if (!car->b4B8 && !car->b47B) {
            continue;
        }
        room = &((LevelBcw_ *)gCollLevel)->rooms[*obj->p54];
        if (room->flags & 0x800) {
            continue;
        }
        ax0.x = car->m[0][0]; ax0.y = car->m[1][0]; ax0.z = car->m[2][0];
        ax1.x = car->m[0][1]; ax1.y = car->m[1][1]; ax1.z = car->m[2][1];
        ax2.x = car->m[0][2]; ax2.y = car->m[1][2]; ax2.z = car->m[2][2];
        VEC3_SCALE_M(ax0, ax0, obj->shape.hx);
        VEC3_SCALE_M(ax1, ax1, obj->shape.hy);
        VEC3_SCALE_M(ax2, ax2, car->f5E4);
        origin.x = (f32)(room->ox << 8);
        origin.y = (f32)(room->oy << 8);
        origin.z = (f32)(room->oz << 8);
        plane = &((LevelBcw_ *)gCollLevel)->planes[room->planeStart];
        for (i = 0; i < room->planeCount; plane++, i++) {
            if (plane->surf != 0) {
                pl.n.x = (f32)plane->nx / 4096.0f;
                pl.n.y = (f32)plane->ny / 4096.0f;
                pl.n.z = (f32)plane->nz / 4096.0f;
                pl.d = (f32)plane->d;
                pl.e = plane->surf;
                pl.f = plane->to;
                pt = shape->pos;
                if (Vec3_Dot_B_(&pl.n, &ax0) > 0.0f) { Vec3_Neg_B_(&ax0, &ax0); }
                if (Vec3_Dot_B_(&pl.n, &ax1) > 0.0f) { Vec3_Neg_B_(&ax1, &ax1); }
                if (Vec3_Dot_B_(&pl.n, &ax2) > 0.0f) { Vec3_Neg_B_(&ax2, &ax2); }
                Vec3_Add_B_(&pt, &pt, &ax0);
                Vec3_Add_B_(&pt, &pt, &ax1);
                Vec3_Add_B_(&pt, &pt, &ax2);
                Vec3_Sub_B_(&rel, &pt, &origin);
                dist = Vec3_Dot_B_(&rel, &pl.n) + pl.d;
                if (!(dist > 0.0f)) {
                    if (pl.e == 3) {
                        car->flags |= 0x2000;
                    }
                    if (pl.e == 4) {
                        car->flags |= 0x1000;
                    }
                    Vec3_Sub_B_(&r1, &pt, &body->offset);
                    Vec3_Cross_B_(&vrot1, &body->angVel, &r1);
                    Vec3_Add_B_(&wn, &body->vel, &vrot1);
                    vn = Vec3_Dot_B_(&wn, &pl.n);
                    if (vn < 0.0f) {
                        if (gCollContactCount < 16) {
                            c = &((ContactBcw_ *)gCollContacts)[gCollContactCount];
                            c->obj = obj;
                            c->pos = pt;
                            c->norm = pl.n;
                            c->type = pl.e;
                            gCollContactCount++;
                        }
                    }
                    if (-dist > 50.0f) {
                        if (car != NULL) {
                            car->flags |= 0x800;
                        } else {
                            body->asleep = 1;
                        }
                    } else {
                        VEC3_SCALE_M(push, pl.n, -dist);
                        Vec3_Add_B_(&body->offset, &body->offset, &push);
                    }
                }
            }
        }
    }

    for (bnode = (LnkBcw_ *)gCollPathCars; bnode != NULL; bnode = bnode->next) {
        s32 pd0s, pd1s;
        f32 d0, d1, t, s;

        obj = bnode->obj;
        body = obj->body;
        if (body->asleep) {
            continue;
        }
        car = obj->car;
        shape = &obj->shape;
        if (!car->b4B8 && !car->b47B) {
            continue;
        }
        room = &((LevelBcw_ *)gCollLevel)->rooms[*obj->p54];
        if (!(room->flags & 0x800)) {
            continue;
        }
        ax0.x = car->m[0][0]; ax0.y = car->m[1][0]; ax0.z = car->m[2][0];
        ax1.x = car->m[0][1]; ax1.y = car->m[1][1]; ax1.z = car->m[2][1];
        ax2.x = car->m[0][2]; ax2.y = car->m[1][2]; ax2.z = car->m[2][2];
        VEC3_SCALE_M(ax0, ax0, obj->shape.hx);
        VEC3_SCALE_M(ax1, ax1, obj->shape.hy);
        VEC3_SCALE_M(ax2, ax2, car->f5E4);
        pt = shape->pos;
        origin.x = (f32)(room->ox << 8);
        origin.y = (f32)(room->oy << 8);
        origin.z = (f32)(room->oz << 8);
        Vec3_Sub_B_(&rel, &pt, &origin);
        plane = &((LevelBcw_ *)gCollLevel)->planes[room->planeStart];
        pn0.x = (f32)plane->nx / 4096.0f;
        pn0.y = (f32)plane->ny / 4096.0f;
        pn0.z = (f32)plane->nz / 4096.0f;
        pd0s = plane->d;
        plane++;
        pn1.x = (f32)plane->nx / 4096.0f;
        pn1.y = (f32)plane->ny / 4096.0f;
        pn1.z = (f32)plane->nz / 4096.0f;
        pd1s = plane->d;

        quad = &((LevelBcw_ *)gCollLevel)->quads[room->quadIdx];
        q0_0.x = (f32)quad->v[0][0]; q0_0.y = (f32)quad->v[0][1]; q0_0.z = (f32)quad->v[0][2];
        q0_2.x = (f32)quad->v[2][0]; q0_2.y = (f32)quad->v[2][1]; q0_2.z = (f32)quad->v[2][2];
        q0_1.x = (f32)quad->v[1][0]; q0_1.y = (f32)quad->v[1][1]; q0_1.z = (f32)quad->v[1][2];
        q0_3.x = (f32)quad->v[3][0]; q0_3.y = (f32)quad->v[3][1]; q0_3.z = (f32)quad->v[3][2];
        quad++;
        q1_0.x = (f32)quad->v[0][0]; q1_0.y = (f32)quad->v[0][1]; q1_0.z = (f32)quad->v[0][2];
        q1_2.x = (f32)quad->v[2][0]; q1_2.y = (f32)quad->v[2][1]; q1_2.z = (f32)quad->v[2][2];
        q1_1.x = (f32)quad->v[1][0]; q1_1.y = (f32)quad->v[1][1]; q1_1.z = (f32)quad->v[1][2];
        q1_3.x = (f32)quad->v[3][0]; q1_3.y = (f32)quad->v[3][1]; q1_3.z = (f32)quad->v[3][2];

        d0 = Vec3_Dot_B_(&rel, &pn0) + (f32)pd0s;
        d1 = Vec3_Dot_B_(&rel, &pn1) + (f32)pd1s;
        t = d1 / (d0 + d1);
        s = 1.0f - t;

        c0.x = q0_0.x * t + q1_0.x * s;
        c0.y = q0_0.y * t + q1_0.y * s;
        c0.z = q0_0.z * t + q1_0.z * s;
        c2.x = q0_2.x * t + q1_2.x * s;
        c2.y = q0_2.y * t + q1_2.y * s;
        c2.z = q0_2.z * t + q1_2.z * s;
        c3.x = q0_3.x * t + q1_3.x * s;
        c3.y = q0_3.y * t + q1_3.y * s;
        c3.z = q0_3.z * t + q1_3.z * s;
        c1.x = q0_1.x * t + q1_1.x * s;
        c1.y = q0_1.y * t + q1_1.y * s;
        c1.z = q0_1.z * t + q1_1.z * s;

        CHECK_WALL_BCW(0, c2, c3);
        CHECK_WALL_BCW(1, c0, c2);
        CHECK_WALL_BCW(2, c1, c0);
        CHECK_WALL_BCW(3, c3, c1);

        for (k = 0; k < 4; k++) {
            if (!(wd[k] > 0.0f)) {
                pt = shape->pos;
                if (wnorm[k].x * ax0.x + wnorm[k].y * ax0.y + wnorm[k].z * ax0.z > 0.0f) { Vec3_Neg_B_(&ax0, &ax0); }
                if (wnorm[k].x * ax1.x + wnorm[k].y * ax1.y + wnorm[k].z * ax1.z > 0.0f) { Vec3_Neg_B_(&ax1, &ax1); }
                if (wnorm[k].x * ax2.x + wnorm[k].y * ax2.y + wnorm[k].z * ax2.z > 0.0f) { Vec3_Neg_B_(&ax2, &ax2); }
                Vec3_Add_B_(&pt, &pt, &ax0);
                Vec3_Add_B_(&pt, &pt, &ax1);
                Vec3_Add_B_(&pt, &pt, &ax2);
                Vec3_Sub_B_(&r1, &pt, &body->offset);
                Vec3_Cross_B_(&vrot2, &body->angVel, &r1);
                Vec3_Add_B_(&wn, &body->vel, &vrot2);
                vn = wn.x * wnorm[k].x + wn.y * wnorm[k].y + wn.z * wnorm[k].z;
                if (vn < 0.0f) {
                    if (gCollContactCount < 16) {
                        c = &((ContactBcw_ *)gCollContacts)[gCollContactCount];
                        c->obj = obj;
                        c->pos = pt;
                        c->norm = wnorm[k];
                        if (k != 0) {
                            c->type = 1;
                        } else {
                            c->type = 2;
                        }
                        gCollContactCount++;
                    }
                }
                if (-wd[k] > 50.0f) {
                    if (car != NULL) {
                        car->flags |= 0x800;
                    } else {
                        body->asleep = 1;
                    }
                } else {
                    Vec3_Add_B_(&wn, &body->offset, &body->pos);
                    VEC3_SCALE_M(push, wnorm[k], -wd[k]);
                    Vec3_Add_B_(&wn, &wn, &push);
                    Vec3_Sub_B_(&body->offset, &wn, &body->pos);
                }
            }
        }
    }
}
#undef VEC3_SCALE_M
#undef CHECK_WALL_BCW

void Coll_RacersVsWorldEdges(void) {
    LnkA2_ *n;

    for (n = gCollRacers; n != NULL; n = n->next) {
        ObjA2_ *o = n->obj;

        if (o->p60->b110 == 0 && o->p70->w8 != 0) {
            Coll_BoxVsWorldEdges(o);
        }
    }
}


typedef struct {
    Vec3f pos;
    Vec3f half;
    f32 m[3][3];
} CollBox_;

typedef struct {
    Vec3f pos;
    Vec3f dir;
    Vec3f norm;
    f32 len;
} CollSeg_;

typedef struct {
    u8 p0[0x60];
    Vec3f pos;
    Vec3f offset;
    u8 p78[0xC];
    Vec3f vel;
    u8 p90[4];
    f32 m[3][3];
} BodyBwe_;

typedef struct LnkTriBwe_ {
    s32 triIdx;
    s32 w4;
    struct LnkTriBwe_ *next;
} LnkTriBwe_;

typedef struct {
    u8 p0[0xC];
    CollBox_ box;
    u8 p48[0x18];
    BodyBwe_ *body;
    u8 p64[0xC];
    LnkTriBwe_ *tris;
} ObjBwe_;

typedef struct {
    u8 p0[0xB];
    u8 edgeCount;
    u16 uC;
    u16 edgeStart;
    u8 p10[4];
} TriBwe_;

typedef struct {
    s16 px, py, pz;
    s16 dx, dy, dz;
    s16 nx, ny, nz;
    s16 len;
} EdgeBwe_;

typedef struct {
    u8 p0[0xC0];
    TriBwe_ *tris;
    u8 pC4[8];
    EdgeBwe_ *edges;
} LevelBwe_;

typedef struct {
    ObjBwe_ *obj;
    Vec3f pos;
    Vec3f norm;
    u8 type;
    u8 pad[3];
} ContactBwe_;

u8 CollBox_EdgeSeparatingAxis(CollBox_ *a, CollSeg_ *b);

static inline Vec3f Mat3_MulVec_Bwe_(f32 m[3][3], Vec3f *v) {
    Vec3f r;
    r.x = m[0][0] * v->x + m[0][1] * v->y + m[0][2] * v->z;
    r.y = m[1][0] * v->x + m[1][1] * v->y + m[1][2] * v->z;
    r.z = m[2][0] * v->x + m[2][1] * v->y + m[2][2] * v->z;
    return r;
}

static inline void Vec3_Add_Bwe_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}

static inline void Vec3_Sub_Bwe_(Vec3f *dst, Vec3f *a, Vec3f *b) {
    dst->x = a->x - b->x;
    dst->y = a->y - b->y;
    dst->z = a->z - b->z;
}

static inline void Vec3_Scale_Bwe_(Vec3f *dst, Vec3f *v, f32 s) {
    dst->x = v->x * s;
    dst->y = v->y * s;
    dst->z = v->z * s;
}

static inline f32 Vec3_Dot_Bwe_(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline f32 MinF_Bwe_(f32 a, f32 b) {
    return (a < b) ? a : b;
}

void Coll_BoxVsWorldEdges(void *objPtr) {
    ObjBwe_ *obj = (ObjBwe_ *)objPtr;
    CollSeg_ seg;
    Vec3f tmp;
    Vec3f lpos;
    Vec3f ldir;
    Vec3f push;
    Vec3f p0;
    Vec3f p1;
    f32 mt[3][3];
    LnkTriBwe_ *node;
    BodyBwe_ *body;
    CollBox_ *box;
    TriBwe_ *tri;
    EdgeBwe_ *edge;
    u32 i;

    node = obj->tris;
    if (node->next == NULL) {
        return;
    }
    body = obj->body;
    box = &obj->box;
    while (node != NULL) {
        tri = &((LevelBwe_ *)gCollLevel)->tris[node->triIdx];
        edge = &((LevelBwe_ *)gCollLevel)->edges[tri->edgeStart];
        for (i = 0; i < tri->edgeCount; i++) {
            seg.pos.x = (f32)edge->px;
            seg.pos.y = (f32)edge->py;
            seg.pos.z = (f32)edge->pz;
            seg.dir.x = (f32)edge->dx / 4096.0f;
            seg.dir.y = (f32)edge->dy / 4096.0f;
            seg.dir.z = (f32)edge->dz / 4096.0f;
            seg.norm.x = (f32)edge->nx / 4096.0f;
            seg.norm.y = (f32)edge->ny / 4096.0f;
            seg.norm.z = (f32)edge->nz / 4096.0f;
            seg.len = (f32)edge->len;
            if (!CollBox_EdgeSeparatingAxis(box, &seg)) {
                f32 hx, hy, hz;
                f32 dxp, dxm, dyp, dym, dzp, dzm;
                f32 dx, dy, dz;
                f32 t;
                s32 axis;

                mt[0][0] = box->m[0][0]; mt[0][1] = box->m[1][0]; mt[0][2] = box->m[2][0];
                mt[1][0] = box->m[0][1]; mt[1][1] = box->m[1][1]; mt[1][2] = box->m[2][1];
                mt[2][0] = box->m[0][2]; mt[2][1] = box->m[1][2]; mt[2][2] = box->m[2][2];

                Vec3_Sub_Bwe_(&tmp, &seg.pos, &box->pos);
                lpos = Mat3_MulVec_Bwe_(mt, &tmp);
                ldir = Mat3_MulVec_Bwe_(mt, &seg.dir);
                push = Mat3_MulVec_Bwe_(mt, &seg.norm);

                tmp.x = ldir.x * seg.len;
                tmp.y = ldir.y * seg.len;
                tmp.z = ldir.z * seg.len;
                Vec3_Add_Bwe_(&p0, &lpos, &tmp);
                Vec3_Sub_Bwe_(&p1, &lpos, &tmp);

                hx = box->half.x;
                hy = box->half.y;
                hz = box->half.z;

                {
                    u8 a, b;

                    a = (p0.x < hx);
                    b = (p1.x < hx);
                    if (a != b) {
                        t = (hx - lpos.x) / ldir.x;
                        Vec3_Scale_Bwe_(&tmp, &ldir, t);
                        Vec3_Add_Bwe_(&tmp, &lpos, &tmp);
                        if (!a) {
                            p0 = tmp;
                        } else {
                            p1 = tmp;
                        }
                    }
                    a = (p0.x > -hx);
                    b = (p1.x > -hx);
                    if (a != b) {
                        t = (-hx - lpos.x) / ldir.x;
                        Vec3_Scale_Bwe_(&tmp, &ldir, t);
                        Vec3_Add_Bwe_(&tmp, &lpos, &tmp);
                        if (!a) {
                            p0 = tmp;
                        } else {
                            p1 = tmp;
                        }
                    }
                    a = (p0.y < hy);
                    b = (p1.y < hy);
                    if (a != b) {
                        t = (hy - lpos.y) / ldir.y;
                        Vec3_Scale_Bwe_(&tmp, &ldir, t);
                        Vec3_Add_Bwe_(&tmp, &lpos, &tmp);
                        if (!a) {
                            p0 = tmp;
                        } else {
                            p1 = tmp;
                        }
                    }
                    a = (p0.y > -hy);
                    b = (p1.y > -hy);
                    if (a != b) {
                        t = (-hy - lpos.y) / ldir.y;
                        Vec3_Scale_Bwe_(&tmp, &ldir, t);
                        Vec3_Add_Bwe_(&tmp, &lpos, &tmp);
                        if (!a) {
                            p0 = tmp;
                        } else {
                            p1 = tmp;
                        }
                    }
                    a = (p0.z < hz);
                    b = (p1.z < hz);
                    if (a != b) {
                        t = (hz - lpos.z) / ldir.z;
                        Vec3_Scale_Bwe_(&tmp, &ldir, t);
                        Vec3_Add_Bwe_(&tmp, &lpos, &tmp);
                        if (!a) {
                            p0 = tmp;
                        } else {
                            p1 = tmp;
                        }
                    }
                    a = (p0.z > -hz);
                    b = (p1.z > -hz);
                    if (a != b) {
                        t = (-hz - lpos.z) / ldir.z;
                        Vec3_Scale_Bwe_(&tmp, &ldir, t);
                        Vec3_Add_Bwe_(&tmp, &lpos, &tmp);
                        if (!a) {
                            p0 = tmp;
                        } else {
                            p1 = tmp;
                        }
                    }
                }

                dxp = hx - ((p0.x < p1.x) ? p0.x : p1.x);
                dxm = ((p0.x > p1.x) ? p0.x : p1.x) - (-hx);
                dyp = hy - ((p0.y < p1.y) ? p0.y : p1.y);
                dym = ((p0.y > p1.y) ? p0.y : p1.y) - (-hy);
                dzp = hz - ((p0.z < p1.z) ? p0.z : p1.z);
                dzm = ((p0.z > p1.z) ? p0.z : p1.z) - (-hz);

                dx = MinF_Bwe_(dxp, dxm);
                dy = MinF_Bwe_(dyp, dym);
                dz = MinF_Bwe_(dzp, dzm);

                if (dx > 0.0f && dy > 0.0f && dz > 0.0f) {
                    if (dx < dy && dx < dz) {
                        axis = 0;
                        t = (dxp < dxm) ? -dxp : dxm;
                    } else if (dy < dz) {
                        axis = 1;
                        t = (dyp < dym) ? -dyp : dym;
                    } else {
                        axis = 2;
                        t = (dzp < dzm) ? -dzp : dzm;
                    }
                    push.x = body->m[0][axis];
                    push.y = body->m[1][axis];
                    push.z = body->m[2][axis];
                    Vec3_Scale_Bwe_(&push, &push, t);
                    Vec3_Add_Bwe_(&tmp, &body->offset, &body->pos);
                    Vec3_Add_Bwe_(&tmp, &tmp, &push);
                    Vec3_Sub_Bwe_(&body->offset, &tmp, &body->pos);

                    t = Vec3_Dot_Bwe_(&seg.norm, &body->vel);
                    if (t < 0.0f) {
                        tmp = Mat3_MulVec_Bwe_(box->m, &p0);
                        Vec3_Add_Bwe_(&p0, &tmp, &box->pos);
                        tmp = Mat3_MulVec_Bwe_(box->m, &p1);
                        Vec3_Add_Bwe_(&p1, &tmp, &box->pos);
                        if (gCollContactCount < 16) {
                            ContactBwe_ *c = &((ContactBwe_ *)gCollContacts)[gCollContactCount];
                            c->obj = obj;
                            c->pos = p0;
                            c->norm = seg.norm;
                            c->type = 1;
                            gCollContactCount++;
                            if (gCollContactCount < 16) {
                                c = &((ContactBwe_ *)gCollContacts)[gCollContactCount];
                                c->obj = obj;
                                c->pos = p1;
                                c->norm = seg.norm;
                                c->type = 1;
                                gCollContactCount++;
                            }
                        }
                    }
                }
            }
            edge++;
        }
        node = node->next;
    }
}

extern f32 D_8011B764[];

static inline Vec3f Mat3T_MulVec_(f32 m[3][3], Vec3f *v) {
    Vec3f r;
    r.x = m[0][0] * v->x + m[0][1] * v->y + m[0][2] * v->z;
    r.y = m[1][0] * v->x + m[1][1] * v->y + m[1][2] * v->z;
    r.z = m[2][0] * v->x + m[2][1] * v->y + m[2][2] * v->z;
    return r;
}

u8 CollBox_EdgeSeparatingAxis(CollBox_ *a, CollSeg_ *b) {
    Vec3f dir;
    f32 mt[3][3];
    Vec3f d;
    Vec3f t;
    f32 dx, dy, dz;
    f32 tx, ty, tz;
    f32 hx, hy, hz;
    f32 len;
    f32 ax, ay, az;
    f32 v;
    f32 sep;
    f32 eps = 0.001f;

    mt[0][0] = a->m[0][0]; mt[0][1] = a->m[1][0]; mt[0][2] = a->m[2][0];
    mt[1][0] = a->m[0][1]; mt[1][1] = a->m[1][1]; mt[1][2] = a->m[2][1];
    mt[2][0] = a->m[0][2]; mt[2][1] = a->m[1][2]; mt[2][2] = a->m[2][2];

    dir = Mat3T_MulVec_(mt, &b->dir);

    d.x = b->pos.x - a->pos.x;
    d.y = b->pos.y - a->pos.y;
    d.z = b->pos.z - a->pos.z;
    t = Mat3T_MulVec_(mt, &d);

    tx = t.x;
    ty = t.y;
    tz = t.z;
    hx = a->half.x;
    hy = a->half.y;
    hz = a->half.z;
    len = b->len;
    dx = dir.x;
    dy = dir.y;
    dz = dir.z;

    ax = dx;
    if (ax < 0.0f) {
        ax = -ax;
    }
    ax += eps;

    ay = dy;
    if (ay < 0.0f) {
        ay = -ay;
    }
    ay += eps;

    az = dz;
    if (az < 0.0f) {
        az = -az;
    }
    az += eps;

    v = tx;
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (hx + len * ax);
    if (sep > 0.0f) {
        return 1;
    }
    D_8011B764[0] = sep;

    v = tx * dx + (ty * dy + tz * dz);
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (len + (hx * ax + (hy * ay + hz * az)));
    if (sep > 0.0f) {
        return 2;
    }
    D_8011B764[1] = sep;

    v = ty;
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (hy + len * ay);
    if (sep > 0.0f) {
        return 3;
    }
    D_8011B764[2] = sep;

    v = tz;
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (hz + len * az);
    if (sep > 0.0f) {
        return 5;
    }
    D_8011B764[4] = sep;

    v = tz * dy - ty * dz;
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (hy * az + hz * ay);
    if (sep > 0.0f) {
        return 7;
    }
    D_8011B764[6] = sep;

    v = tx * dz - tz * dx;
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (hx * az + hz * ax);
    if (sep > 0.0f) {
        return 10;
    }
    D_8011B764[9] = sep;

    v = ty * dx - tx * dy;
    if (v < 0.0f) {
        v = -v;
    }
    sep = v - (hx * ay + hy * ax);
    if (sep > 0.0f) {
        return 13;
    }
    D_8011B764[12] = sep;

    return 0;
}

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

typedef struct {
    /* 0x00 */ u8 p0[0x50];
    /* 0x50 */ Vec3f *pos;
    /* 0x54 */ u16 *rooms;
} RObjA_;
typedef struct { Vec3f n; f32 d; s16 e; u16 f; } PlFA_;

static inline void Vec3_Norm_(Vec3f *v) {
    f32 inv = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x *= inv;
    v->y *= inv;
    v->z *= inv;
}

s32 Coll_GetRoomAxis(u8 carIdx, Vec3f *outAxis, Vec3f *outUp) {
    PlFA_ pl;
    Vec3f n1;
    Vec3f n2;
    Vec3f org;
    Vec3f d1;
    Vec3f d2;
    Vec3f sum;
    Vec3f up;
    RObjA_ *obj;
    Room_ *r1;
    Room_ *r2;
    Portal_ *l;
    u32 i;
    u8 has1;
    u8 has2;
    f32 dist1;
    f32 dist2;
    f32 total;
    f32 w;

    dist2 = 0.0f;
    dist1 = 0.0f;
    obj = (RObjA_ *)Car_Get(carIdx)->roomObj;
    r1 = &gCollLevel->rooms[obj->rooms[0]];
    if ((r1->flags & 0x9000) != 0x9000) {
        return 0;
    }

    org.x = r1->x << 8;
    org.y = r1->y << 8;
    org.z = r1->z << 8;
    d1.x = obj->pos->x - org.x;
    d1.y = obj->pos->y - org.y;
    d1.z = obj->pos->z - org.z;

    has1 = 0;
    has2 = 0;
    r2 = NULL;
    l = &gCollLevel->portals[r1->firstPortal];
    for (i = 0; i < r1->nPortals; l++, i++) {
        if (l->flag == 0) {
            pl.n.x = l->nx / 4096.0f;
            pl.n.y = l->ny / 4096.0f;
            pl.n.z = l->nz / 4096.0f;
            pl.d = l->d;
            pl.e = l->flag;
            pl.f = l->to;
            if ((pl.n.z < 0.0f ? -pl.n.z : pl.n.z) < 0.5f) {
                if (!has1) {
                    dist1 = d1.x * pl.n.x + d1.y * pl.n.y + d1.z * pl.n.z + pl.d;
                    n1 = pl.n;
                    has1 = 1;
                } else if (!has2) {
                    dist2 = d1.x * pl.n.x + d1.y * pl.n.y + d1.z * pl.n.z + pl.d;
                    n2 = pl.n;
                    n2.x = -n2.x;
                    n2.y = -n2.y;
                    n2.z = -n2.z;
                    has2 = 1;
                }
            } else {
                r2 = &gCollLevel->rooms[pl.f];
            }
        }
    }

    dist1 = dist2 / (dist1 + dist2);
    n1.x *= dist1;
    n1.y *= dist1;
    n1.z *= dist1;
    dist2 = 1.0f - dist1;
    n2.x *= dist2;
    n2.y *= dist2;
    n2.z *= dist2;
    sum.x = n1.x + n2.x;
    sum.y = n1.y + n2.y;
    sum.z = n1.z + n2.z;
    Vec3_Norm_(&sum);
    *outAxis = sum;

    org.x = r2->x << 8;
    org.y = r2->y << 8;
    org.z = r2->z << 8;
    d2.x = obj->pos->x - org.x;
    d2.y = obj->pos->y - org.y;
    d2.z = obj->pos->z - org.z;

    total = 0.0f;
    l = &gCollLevel->portals[r1->firstPortal];
    for (i = 0; i < r1->nPortals; l++, i++) {
        if (l->flag != 0) {
            pl.n.x = l->nx / 4096.0f;
            pl.n.y = l->ny / 4096.0f;
            pl.n.z = l->nz / 4096.0f;
            pl.d = l->d;
            pl.e = l->flag;
            pl.f = l->to;
            total += d1.x * pl.n.x + d1.y * pl.n.y + d1.z * pl.n.z + pl.d;
        }
    }
    l = &gCollLevel->portals[r2->firstPortal];
    for (i = 0; i < r2->nPortals; l++, i++) {
        if (l->flag != 0) {
            pl.n.x = l->nx / 4096.0f;
            pl.n.y = l->ny / 4096.0f;
            pl.n.z = l->nz / 4096.0f;
            pl.d = l->d;
            pl.e = l->flag;
            pl.f = l->to;
            total += d2.x * pl.n.x + d2.y * pl.n.y + d2.z * pl.n.z + pl.d;
        }
    }

    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 0.0f;
    l = &gCollLevel->portals[r1->firstPortal];
    for (i = 0; i < r1->nPortals; l++, i++) {
        if (l->flag != 0) {
            pl.n.x = l->nx / 4096.0f;
            pl.n.y = l->ny / 4096.0f;
            pl.n.z = l->nz / 4096.0f;
            pl.d = l->d;
            pl.e = l->flag;
            pl.f = l->to;
            w = 1.0f - (d1.x * pl.n.x + d1.y * pl.n.y + d1.z * pl.n.z + pl.d) / total;
            sum.x = pl.n.x * w;
            sum.y = pl.n.y * w;
            sum.z = pl.n.z * w;
            up.x += sum.x;
            up.y += sum.y;
            up.z += sum.z;
        }
    }
    l = &gCollLevel->portals[r2->firstPortal];
    for (i = 0; i < r2->nPortals; l++, i++) {
        if (l->flag != 0) {
            pl.n.x = l->nx / 4096.0f;
            pl.n.y = l->ny / 4096.0f;
            pl.n.z = l->nz / 4096.0f;
            pl.d = l->d;
            pl.e = l->flag;
            pl.f = l->to;
            w = 1.0f - (d2.x * pl.n.x + d2.y * pl.n.y + d2.z * pl.n.z + pl.d) / total;
            sum.x = pl.n.x * w;
            sum.y = pl.n.y * w;
            sum.z = pl.n.z * w;
            up.x += sum.x;
            up.y += sum.y;
            up.z += sum.z;
        }
    }

    Vec3_Norm_(&up);
    *outUp = up;
    return 1;
}

s32 Coll_GetSpawnCount(void) {
    return gCollLevel->nSpawns;
}

s32 Coll_GetSpawn(s32 i) {
    return (s32)(gCollLevel->spawns + i * 0x18);
}
