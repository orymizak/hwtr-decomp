#include "common.h"
#include "include_asm.h"
#include "car.h"
extern s32 D_8011B330;
extern s32 D_8011C820;
extern s32 D_8011C824;
extern s32 D_8011C828;
extern u8 D_8011C7A0[];
extern void (*D_801416F8)(void *);
s32 func_8008182C(void);
u8 func_80099090(u8 *);
void func_800C9AF0(s32 n, s32 b);
extern s32 D_8011B33C;
extern s32 D_8011B344;
extern s32 D_8011B348;
extern s32 D_8011B34C;
void func_800C9B40(void);
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } V16_;
typedef struct { u8 pad[0x18]; V12_ v[6]; V16_ q[1]; } Cam41_;
typedef struct Nd4_ { s32 a; struct Nd4_ *prev; struct Nd4_ *next; u16 cnt; } Nd4_;
typedef struct { u8 pad[0x4C]; void *a; void *b; void *c; u8 pad2[0xC]; s32 id; } Obj95_;
typedef struct Lnk95_ { Obj95_ *obj; s32 x; struct Lnk95_ *next; } Lnk95_;
extern Lnk95_ *D_8011B338;
s32 func_8009A770(s32 *);
typedef struct { u8 pad[0x110]; u8 b110; } Obj60_;
typedef struct { u8 pad[8]; s32 w8; } Obj70_;
typedef struct { u8 pad[0x60]; Obj60_ *p60; u8 pad2[0xC]; Obj70_ *p70; } ObjA2_;
typedef struct LnkA2_ { ObjA2_ *obj; s32 x; struct LnkA2_ *next; } LnkA2_;
extern LnkA2_ *D_8011B340;
void func_800A25EC(ObjA2_ *);
typedef struct { u8 pad[0x40]; void *a; void *b; void *c; } Res40_;
void Memory_Free(void *);
typedef struct Nd3_ { s32 a; struct Nd3_ *prev; struct Nd3_ *next; } Nd3_;
typedef struct { f32 x; f32 y; f32 z; } Vec3f_;
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
void func_800A780C(u8, u8, s32);
void func_800979F4(u8 *);
void func_8009751C(u8 *, u16, u8);
void func_800C9B84(u16);
void func_800A3978(RObj_ *o, u16 idx, s32 arg2);
u16 func_8009A9A0(Vec3f_ pos);
void func_8009A4E4(RObj_ *o);
void func_80099C94(u8 *n, u8 *o);
void *Memory_Alloc(s32 size);
void func_800996B8(u8 *, u8 *);

extern void *D_8011E860;
extern Level_ *D_8011B334;

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_80099090);

s32 func_8009930C(u8 *o) {
    s32 v = *(s32 *)(o + 0x1C);

    D_8011B330 = 100;
    D_8011C820 = v;
    D_8011C824 = func_8008182C();
    D_8011C828 = o[0x1A];
    func_800A3CD8();
    if (func_80099090(o) == 0) {
        return 0;
    }
    D_801416F8(D_8011B334);
    func_800C9AF0(D_8011B334->x10, D_8011B334->xD0);
    memset(D_8011C7A0, 0, 0x80);
    return 1;
}


void func_800A3D64(Nd3_ **list);
s32 func_800993B8(void) {
    func_800C9B40();
    func_800A3D64((Nd3_ **)&(*(s32 *)&D_8011B338));
    func_800A3D64((Nd3_ **)&D_8011B33C);
    func_800A3D64((Nd3_ **)&(*(s32 *)&D_8011B340));
    func_800A3D64((Nd3_ **)&D_8011B344);
    func_800A3D64((Nd3_ **)&D_8011B348);
    func_800A3D64((Nd3_ **)&D_8011B34C);
    Memory_Free(D_8011B334);
    D_8011B334 = NULL;
    return 1;
}


void func_80099434(u8 *o) {
    u8 *n = Memory_Alloc(0x88);
    u8 i;

    memset(n, 0, 0x88);
    if (*(s32 *)(o + 0x6D8) == 2) {
        func_800996B8(n + 0xC, o);
        *(s32 *)(n + 0x5C) = 3;
    } else if (*(s32 *)(o + 0x6D8) == 1) {
        func_80099C94(n + 0xC, o);
        *(s32 *)(n + 0x5C) = 4;
    }
    *(u8 **)(n + 0x60) = o + 0x2C;
    *(u8 **)(n + 0x64) = o;
    func_8009A4E4(n);
    for (i = 0; i < n[0x48]; i++) {
        func_800A3978(n, (*(u16 **)(n + 0x54))[i], 1);
    }
    *(u8 **)(o + 0x758) = n;
}

void func_8009951C(s32 id) {
    Lnk95_ *n = D_8011B338;
    Obj95_ *o;

    while (n != NULL) {
        o = n->obj;
        if (o->id == id) {
            break;
        }
        n = n->next;
    }
    if (n != NULL) {
        func_8009A770((s32 *)o);
        Memory_Free(o->a);
        Memory_Free(o->b);
        Memory_Free(o->c);
        Memory_Free(o);
    }
}


void func_800A3DB8(Nd4_ **list, s32 key);
void func_800A3E30(Nd4_ **list, s32 key);
void func_80099594(u8 *obj, s32 room) {
    RObj_ *o;
    u8 i;
    u16 k;

    o = *(RObj_ **)(obj + 0x758);
    for (i = 0; i < o->nRooms; i++) {
        k = o->rooms[i];
        func_800A3E30(&D_8011B334->rooms[k].list, (s32)o);
        func_800A3E30(&o->list, k);
    }
    for (i = 0; i < o->nRooms; i++) {
        o->rooms[i] = room;
        func_800A3DB8(&D_8011B334->rooms[room].list, (s32)o);
        func_800A3DB8(&o->list, room);
    }
    func_800A3978(o, room, 1);
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800996B8);

void func_80099C94(u8 *n, u8 *o) {
    s32 zero[3];
    Vec3f_ v;
    Vec3f_ p;
    s16 id;
    u8 i;

    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    v = *(Vec3f_ *)(o + 0x5E8);
    v.x *= 0.5f;
    v.y *= 0.5f;
    v.z *= 0.5f;
    *(Vec3f_ *)(n + 0xC) = v;
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
    id = func_8009A9A0(p);
    for (i = 0; i < n[0x3C]; i++) {
        (*(s16 **)(n + 0x48))[i] = id;
    }
    *(f32 *)(n + 0x4C) = sqrtf(*(f32 *)(n + 0xC) * *(f32 *)(n + 0xC) + *(f32 *)(n + 0x10) * *(f32 *)(n + 0x10) + *(f32 *)(n + 0x14) * *(f32 *)(n + 0x14));
}

void func_80099E78(u8 *n, Vec3f_ size, Vec3f_ pos, f32 *rot, u8 box) {
    f32 r[3][4];
    f32 m[3][4];
    Vec3f_ v;
    Vec3f_ p;
    Vec3f_ *q;
    u8 i;
    f32 x;
    f32 y;
    f32 z;

    v = size;
    v.x *= 0.5f;
    v.y *= 0.5f;
    v.z *= 0.5f;
    *(Vec3f_ *)(n + 0xC) = v;
    n[0x3C] = box ? 8 : 1;
    *(void **)(n + 0x40) = Memory_Alloc(n[0x3C] * 0xC);
    *(void **)(n + 0x44) = Memory_Alloc(n[0x3C] * 0xC);
    memset(*(void **)(n + 0x40), 0, n[0x3C] * 0xC);
    memset(*(void **)(n + 0x44), 0, n[0x3C] * 0xC);
    if (box) {
        (*(Vec3f_ **)(n + 0x40))[0].x = v.x;
        (*(Vec3f_ **)(n + 0x40))[0].y = v.y;
        (*(Vec3f_ **)(n + 0x40))[0].z = v.z;
        (*(Vec3f_ **)(n + 0x40))[1].x = v.x;
        (*(Vec3f_ **)(n + 0x40))[1].y = v.y;
        (*(Vec3f_ **)(n + 0x40))[1].z = -v.z;
        (*(Vec3f_ **)(n + 0x40))[2].x = v.x;
        (*(Vec3f_ **)(n + 0x40))[2].y = -v.y;
        (*(Vec3f_ **)(n + 0x40))[2].z = v.z;
        (*(Vec3f_ **)(n + 0x40))[3].x = v.x;
        (*(Vec3f_ **)(n + 0x40))[3].y = -v.y;
        (*(Vec3f_ **)(n + 0x40))[3].z = -v.z;
        (*(Vec3f_ **)(n + 0x40))[4].x = -v.x;
        (*(Vec3f_ **)(n + 0x40))[4].y = v.y;
        (*(Vec3f_ **)(n + 0x40))[4].z = v.z;
        (*(Vec3f_ **)(n + 0x40))[5].x = -v.x;
        (*(Vec3f_ **)(n + 0x40))[5].y = v.y;
        (*(Vec3f_ **)(n + 0x40))[5].z = -v.z;
        (*(Vec3f_ **)(n + 0x40))[6].x = -v.x;
        (*(Vec3f_ **)(n + 0x40))[6].y = -v.y;
        (*(Vec3f_ **)(n + 0x40))[6].z = v.z;
        (*(Vec3f_ **)(n + 0x40))[7].x = -v.x;
        (*(Vec3f_ **)(n + 0x40))[7].y = -v.y;
        (*(Vec3f_ **)(n + 0x40))[7].z = -v.z;
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
        x = (*(Vec3f_ **)(n + 0x40))[i].x;
        y = (*(Vec3f_ **)(n + 0x40))[i].y;
        z = (*(Vec3f_ **)(n + 0x40))[i].z;
        p.x = r[0][0] * x + r[0][1] * y + r[0][2] * z + r[0][3];
        p.y = r[1][0] * x + r[1][1] * y + r[1][2] * z + r[1][3];
        p.z = r[2][0] * x + r[2][1] * y + r[2][2] * z + r[2][3];
        (*(Vec3f_ **)(n + 0x44))[i] = p;
        (*(s16 **)(n + 0x48))[i] = func_8009A9A0(p);
    }
    *(Vec3f_ *)(n + 0x0) = pos;
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

void func_8009A488(Res40_ *r) {
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


void func_800A3DB8(Nd4_ **list, s32 key);
void func_8009A4E4(RObj_ *o) {
    u32 i;
    u16 k;
    Room_ *r;
    Portal_ *pt;
    u32 lo;
    u32 hi;

    for (i = 0; i < o->nRooms; i++) {
        k = o->rooms[i];
        func_800A3DB8(&D_8011B334->rooms[k].list, (s32)o);
        func_800A3DB8(&o->list, k);
    }
    if (o->type == 4) {
        r = &D_8011B334->rooms[o->rooms[0]];
        pt = &D_8011B334->portals[r->firstPortal];
        for (i = 0; i < r->nPortals; i++, pt++) {
            if (pt->flag == 0) {
                ((void (*)(void *, void *, u16))func_800A3DB8)(&D_8011B334->rooms[pt->to].list, o, pt->to);
            }
        }
    }
    func_800A3DB8((Nd4_ **)&D_8011B338, (s32)o);
    if ((o->type != 0) & (o->type != 5)) {
        func_800A3DB8((Nd4_ **)&D_8011B33C, (s32)o);
    }
    if (o->type == 3) {
        func_800A3DB8((Nd4_ **)&D_8011B340, (s32)o);
    }
    if (o->type == 3 || o->type == 4) {
        func_800A3DB8((Nd4_ **)&D_8011B344, (s32)o);
    }
    if (((o->type == 1) | (o->type == 3)) || o->type == 6) {
        func_800A3DB8((Nd4_ **)&D_8011B348, (s32)o);
    }
    if (o->type == 4) {
        func_800A3DB8((Nd4_ **)&D_8011B34C, (s32)o);
    }
    if ((o->type == 0) | (o->type == 5)) {
        lo = 0x20;
        hi = 0x80;
    } else {
        lo = 0;
        hi = 0x20;
    }
    for (i = lo; i < hi; i++) {
        if (D_8011C7A0[i] == 0) {
            o->id = i;
            D_8011C7A0[i] = 1;
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009A770);

u16 func_8009A9A0(Vec3f_ pos) {
    Vec3f_ p;
    Vec3f_ d;
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
    w = D_8011B334->rooms;
    for (i = 0; i < D_8011B334->nRooms; w++, i++) {
        if (!(w->flags & 0x800)) {
            p.x = w->x << 8;
            p.y = w->y << 8;
            p.z = w->z << 8;
            d.x = pos.x - p.x;
            d.y = pos.y - p.y;
            d.z = pos.z - p.z;
            l = &D_8011B334->portals[w->firstPortal];
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

typedef struct { f32 x; f32 y; f32 z; } V3AB_;
typedef struct {
    u8 pad0[4]; u32 flags; u8 pad8[0x4B0]; u8 b4B8; u8 pad4B9[0x205]; u8 b6BE; u8 b6BF; u8 pad6C0[0x50]; u8 b710; u8 pad711[0xF];
    V3AB_ n720; u8 pad72C[0x44]; s32 w770;
} PlAB_;
typedef struct { u8 pad0[4]; u32 flags; u8 pad8[0x54]; s32 type; u8 pad60[4]; PlAB_ *pl; u8 pad68[0xC]; s32 frame; V3AB_ pos; } ObjAB_;
typedef struct { ObjAB_ *obj; u8 pad4[0xC]; V3AB_ pos; u8 idx; u8 pad1D[3]; } EntAB_;
typedef struct { ObjAB_ *a; ObjAB_ *b; u8 pad8[0x18]; } PairAB_;
typedef struct { f32 x; f32 y; f32 z; } V3C7_;
typedef struct { V3C7_ pos; f32 ext[3]; f32 m[3][3]; u8 cnt; u8 p3D[7]; V3C7_ *verts; u8 p48[4]; f32 rad; } BoxC7_;
typedef struct { u8 p0[0x60]; V3C7_ v60; V3C7_ v6C; u8 p78[0xC]; V3C7_ off; u8 p90[0x80]; u8 b110; } PrC7_;
typedef struct { s32 id; u32 flags; u8 p8[4]; BoxC7_ b; s32 type; PrC7_ *p60; Car *car; u8 p68[0x1C]; u32 w84; } ObjC7_;
typedef struct { ObjC7_ *a; ObjC7_ *b; V3C7_ p; V3C7_ n; } RecC7_;
typedef struct { f32 x; f32 y; f32 z; u8 pC[0x40]; f32 rad; } B_56C;
typedef struct { u8 p0[0x110]; u8 b110; } P60_56C;
typedef struct Ln_56C { void *obj; s32 x; struct Ln_56C *next; } Ln_56C;
typedef struct Ri_56C { s32 room; s32 x; struct Ri_56C *next; } Ri_56C;
typedef struct { s32 id; u32 flags; u8 p8[4]; B_56C b; s32 type; P60_56C *p60; s32 w64; s32 w68; s32 frame; Ri_56C *ri; } O_56C;
typedef struct { f32 x, y, z; } V3_67C;
typedef struct { f32 a; f32 b; f32 c; f32 d; s16 e; u16 f; } Pl_67C;
typedef struct Ri_67C { s32 room; s32 x; struct Ri_67C *next; } Ri_67C;
typedef struct { u8 p0[0x110]; u8 b110; } P60_67C;
typedef struct { V3_67C pos; u8 pC[0x30]; u8 n; u8 p3D[7]; V3_67C *pts; u16 *rooms; } B_67C;
typedef struct { u8 p0[4]; u32 flags; u8 p8[4]; B_67C b; f32 rad; s32 type; P60_67C *p60; u8 p64[0xC]; Ri_67C *ri; } O_67C;
typedef struct Ln_67C { O_67C *obj; s32 x; struct Ln_67C *next; } Ln_67C;
typedef struct NdCD8_ { s32 a; struct NdCD8_ *prev; struct NdCD8_ *next; s32 c; } NdCD8_;
typedef struct { u8 p0[4]; u32 flags; } CrD57_;
typedef struct { u8 p0[4]; u32 flags; u8 p8[0x4C]; u16 *p54; u8 p58[0xC]; CrD57_ *car; } ObD57_;
typedef struct LnD57_ { ObD57_ *obj; s32 x; struct LnD57_ *next; } LnD57_;
void func_800961B0(Car *p, u8 id, u8 flip);
void func_8009DBF8(void);
void func_8009EC8C(void);
void func_8009F7D4(void);
void func_800A0C44(void);
void func_8009D574(void);
extern NdCD8_ D_8011C860[];
void func_800A3CD8(void);
extern u8 D_800F38B0[][7];
void func_8008AA9C(s32, s32);
void func_8009B56C(void);
extern u8 D_8011B7A0[][128];
extern f32 D_8011B760[];
u8 func_8009BAF0(BoxC7_ *, BoxC7_ *);
f32 sqrtf(f32);
extern u16 D_8011B350;
extern EntAB_ D_8011B358[];
extern u16 D_8011B558;
extern PairAB_ D_8011B560[];
extern u8 D_800F38E4[];
extern void (*D_8014177C)(EntAB_ *, f32);
void func_8009B0AC(void);
void func_8008DF18(EntAB_ *);
void func_80091DC4(PlAB_ *);
void func_80098D84(EntAB_ *);
void func_800A66C0(EntAB_ *);
void func_80096954(PlAB_ *, EntAB_ *);
s32 func_800A76D8(void);
f32 func_800AD774(EntAB_ *, f32);
void func_8008E0BC(PairAB_ *);
void func_80098E84(PairAB_ *, s32);
void func_800A6850(PairAB_ *, s32);
void func_800ADE1C(PairAB_ *);
void func_800AEFF8(void);
void func_8009ABC8(void) {
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

    D_8011B558 = 0;
    D_8011B350 = 0;
    D_8011B330++;
    func_8009B0AC();
    func_8009B56C();
    func_8009D574();
    for (i = 0, e = D_8011B358; i < D_8011B350; e++, i++) {
        changed = e->obj->frame != D_8011B330;
        if (changed) {
            func_8008DF18(e);
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
                func_80091DC4(pl);
                if (pl->flags & 1) {
                    func_80098D84(e);
                }
            }
            func_800A66C0(e);
            if (pl->b710 && e->pos.x * pl->n720.x + e->pos.y * pl->n720.y + e->pos.z * pl->n720.z > 0.866f) {
                func_80096954(pl, e);
            }
            pl->w770 = func_800A76D8();
        }
        e->obj->frame = D_8011B330;
        e->obj->pos = e->pos;
        f = D_800F38E4[e->idx] / 100.0f;
        if (isCar && !isRacer && !flag) {
            f *= 4.0f;
        }
        r = func_800AD774(e, f);
        if (isRacer & changed) {
            D_8014177C(e, r);
        }
    }
    for (i = 0, pr = D_8011B560; i < D_8011B558; pr++, i++) {
        pa = NULL;
        if (pr->a->type == 3 || pr->a->type == 4) {
            pa = pr->a->pl;
        }
        pb = NULL;
        if (pr->b->type == 3 || pr->b->type == 4) {
            pb = pr->b->pl;
        }
        func_8008E0BC(pr);
        if (pa != NULL) {
            if (pa->flags & 1) {
                func_80098E84(pr, 0);
            }
            func_800A6850(pr, 0);
        }
        if (pb != NULL) {
            if (pb->flags & 1) {
                func_80098E84(pr, 1);
            }
            func_800A6850(pr, 1);
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
        func_800ADE1C(pr);
        if (pa != NULL) {
            pr->a->flags &= ~0x60;
        }
        if (pb != NULL) {
            pr->b->flags &= ~0x60;
        }
        if (pa != NULL && pb != NULL && (pa->flags & 1) && (pb->flags & 1)) {
            func_800AEFF8();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009B0AC);

void func_8009C758(ObjC7_ *a, ObjC7_ *b);
#define ABS_56C(v) ((v) < 0.0f ? -(v) : (v))
#define MAX_56C(a, b) ((a) > (b) ? (a) : (b))
void func_8009B56C(void) {
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

    for (it = *(Ln_56C **)&D_8011B344; it != NULL; it = it->next) {
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
                res = func_8009BAF0(pa, pb);
                if (res != 0) {
                    D_8011B7A0[a->id][b->id] = res;
                } else {
                    func_8009C758(a, b);
                }
            }
        }
    }
    for (it = *(Ln_56C **)&D_8011B33C; it != NULL; it = it->next) {
        a = it->obj;
        if ((a->p60 != NULL && a->p60->b110 != 0) || (a->flags & 1)) {
            continue;
        }
        for (ri = a->ri; ri != NULL; ri = ri->next) {
            Room_ *rm = &D_8011B334->rooms[ri->room];
            for (jt = rm->list; jt != NULL; jt = jt->next) {
                b = jt->obj;
                if (b == a || (b->flags & 1) || ((b->p60 == NULL || b->p60->b110 == 0) && b->frame == D_8011B330)) {
                    continue;
                }
                if (!D_800F38B0[a->type][b->type]) {
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
                    res = func_8009BAF0(pa, pb);
                    if (res != 0) {
                        D_8011B7A0[a->id][b->id] = res;
                    } else if (b->type == 5) {
                        func_8008AA9C(a->w64, b->w68);
                    } else {
                        func_8009C758(a, b);
                    }
                }
            }
        }
        a->frame = D_8011B330;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009BAF0);

void func_8009C758(ObjC7_ *a, ObjC7_ *b) {
    V3C7_ t;
    V3C7_ u;
    V3C7_ v;
    V3C7_ n;
    BoxC7_ *pa;
    V3C7_ *va;
    V3C7_ *vb;
    u8 flag;
    BoxC7_ *pb;
    BoxC7_ *x;
    BoxC7_ *y;
    PrC7_ *qa;
    PrC7_ *qb;
    u8 code;
    V3C7_ *p;
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
    code = D_8011B7A0[a->id][b->id];
    if (code == 0 && a->p60 != NULL) {
        t = a->b.pos;
        a->b.pos.x = t.x - a->p60->off.x;
        a->b.pos.y = t.y - a->p60->off.y;
        a->b.pos.z = t.z - a->p60->off.z;
        code = func_8009BAF0(&a->b, &b->b);
        D_8011B7A0[a->id][b->id] = code;
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
        if (D_8011B558 < 16) {
            RecC7_ *r = &((RecC7_ *)&D_8011B560)[D_8011B558];
            r->a = b;
            r->b = a;
            r->p = t;
            r->n = n;
            D_8011B558++;
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
        if (D_8011B558 < 16) {
            RecC7_ *r = &((RecC7_ *)&D_8011B560)[D_8011B558];
            r->a = a;
            r->b = b;
            r->p = t;
            r->n = n;
            D_8011B558++;
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
        if (D_8011B558 < 16) {
            RecC7_ *r = &((RecC7_ *)&D_8011B560)[D_8011B558];
            r->a = b;
            r->b = a;
            r->p = t;
            r->n = n;
            D_8011B558++;
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


void func_8009D67C(void);
void func_800A258C(void);
void func_8009D574(void) {
    LnD57_ *it;
    ObD57_ *o;
    CrD57_ *c;

    func_8009D67C();
    func_8009DBF8();
    func_8009EC8C();
    func_8009F7D4();
    func_800A0C44();
    func_800A258C();
    for (it = *(LnD57_ **)&D_8011B344; it != NULL; it = it->next) {
        o = it->obj;
        if (o->flags & 0x10) {
            func_800A3978((u8 *)o, *o->p54, 0);
            o->flags &= ~0x10;
        }
    }
    for (it = *(LnD57_ **)&D_8011B344; it != NULL; it = it->next) {
        o = it->obj;
        c = o->car;
        if (c->flags & 0x800) {
            func_800961B0(c, 0, 1);
        } else if (c->flags & 0x1000) {
            func_800961B0(c, 0, 0);
        } else if (c->flags & 0x2000) {
            func_800961B0(c, 0, 0);
        }
    }
}

void func_800A3E30(Nd4_ **list, s32 key);
void func_8009D67C(void) {
    Pl_67C pl;
    V3_67C org;
    V3_67C d;
    V3_67C rel;
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

    for (it = ((Ln_67C *)D_8011B33C); it != NULL; it = it->next) {
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
            rm = &D_8011B334->rooms[room];
            org.x = rm->x << 8;
            org.y = rm->y << 8;
            org.z = rm->z << 8;
            rel.x = o->b.pos.x - org.x;
            rel.y = o->b.pos.y - org.y;
            rel.z = o->b.pos.z - org.z;
            p = &D_8011B334->portals[rm->firstPortal];
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
                func_800A3E30(&D_8011B334->rooms[old[i]].list, (s32)o);
                func_800A3E30(&o->ri, old[i]);
                b->rooms[i] = new[i];
                func_800A3DB8(&D_8011B334->rooms[new[i]].list, (s32)o);
                func_800A3DB8(&o->ri, new[i]);
                if (i == 0 && (o->type == 3 || o->type == 4)) {
                    o->flags |= 0x10;
                    if (o->type == 4) {
                        rm = &D_8011B334->rooms[old[0]];
                        p = &D_8011B334->portals[rm->firstPortal];
                        for (k = 0; k < rm->nPortals; p++, k++) {
                            if (p->flag == 0) {
                                func_800A3E30(&D_8011B334->rooms[p->to].list, (s32)o);
                            }
                        }
                        rm = &D_8011B334->rooms[new[0]];
                        p = &D_8011B334->portals[rm->firstPortal];
                        for (k = 0; k < rm->nPortals; p++, k++) {
                            if (p->flag == 0) {
                                func_800A3DB8(&D_8011B334->rooms[p->to].list, (s32)o);
                            }
                        }
                    }
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009DBF8);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009EC8C);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_8009F7D4);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A0C44);

void func_800A258C(void) {
    LnkA2_ *n;

    for (n = D_8011B340; n != NULL; n = n->next) {
        ObjA2_ *o = n->obj;

        if (o->p60->b110 == 0 && o->p70->w8 != 0) {
            func_800A25EC(o);
        }
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A25EC);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A34B4);

void func_800A3978(RObj_ *o, u16 idx, s32 arg2) {
    Room_ *r;
    Car *c;
    u32 m;

    r = &D_8011B334->rooms[idx];
    if (o->type == 3 || o->type == 4) {
        c = o->car;
        if (c->b4B8 == 0) {
            if ((r->flags & 1) && (u8)arg2 == 0) {
                func_800A780C(c->id, r->param.b[1], 1);
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
                    c->unk474 += D_8011C824;
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
                    func_800979F4(c);
                } else {
                    func_8009751C(c, r->param.val, r->b4);
                }
            }
            if (r->flags & 2) {
                func_800C9B84(r->param.val);
            }
        }
    }
}

void func_800A3CD8(void) {
    s32 i;

    D_8011E860 = D_8011C860;
    for (i = 0; i < 512; i++) {
        D_8011C860[i].prev = (i != 0) ? &D_8011C860[i - 1] : NULL;
        D_8011C860[i].next = (i != 511) ? &D_8011C860[i + 1] : NULL;
    }
}

void func_800A3D64(Nd3_ **list) {
    Nd3_ *n = *list;

    while (n != NULL) {
        Nd3_ *c = n;
        Nd3_ *old;

        n = n->next;
        old = (Nd3_ *)D_8011E860;
        c->prev = NULL;
        D_8011E860 = c;
        c->next = old;
        old->prev = c;
    }
    memset(list, 0, 4);
}


void func_800A3DB8(Nd4_ **list, s32 key) {
    Nd4_ *n;
    Nd4_ *nx;

    for (n = *list; n != NULL; n = n->next) {
        if (n->a == key) {
            break;
        }
    }
    if (n == NULL) {
        n = (Nd4_ *)D_8011E860;
        nx = n->next;
        nx->prev = NULL;
        n->a = key;
        n->prev = NULL;
        n->next = *list;
        n->cnt = 0;
        *list = n;
        D_8011E860 = nx;
        if (n->next != NULL) {
            n->next->prev = n;
        }
    }
    n->cnt++;
}


void func_800A3E30(Nd4_ **list, s32 key) {
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
            free = (Nd4_ *)D_8011E860;
            n->prev = NULL;
            D_8011E860 = n;
            n->next = free;
            free->prev = n;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A3ED4);

void func_800A4120(u8 i, V12_ *v, V16_ *q) {
    if (D_8011B334 != NULL) {
        *v = ((Cam41_ *)D_8011B334)->v[i];
        *q = ((Cam41_ *)D_8011B334)->q[i];
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4198);

INCLUDE_ASM("asm/nonmatchings/unit_39C90", func_800A4EE8);

s32 func_800A5894(void) {
    return D_8011B334->nSpawns;
}

s32 func_800A58A4(s32 i) {
    return (s32)(D_8011B334->spawns + i * 0x18);
}
