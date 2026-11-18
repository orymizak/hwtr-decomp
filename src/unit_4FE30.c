#include "common.h"
#include "math.h"
#include "include_asm.h"
typedef struct { u8 p0[0x94]; f32 m[3][3]; f32 v[3]; } Body4F_;
    /* TODO (Clean version): Define unified engine structs for E88_ and its dynamically allocated child object (0x118 bytes):
    typedef struct {
        u8 pad0[0x60];
        Vec3f base_pos;
        Vec3f offset_pos;
        u8 pad78[0x1C];
        f32 rot[3][3];    // 0x94
        u8 padB8[0x58];
        u8 state;          // 0x110
        u8 pad111[7];
    } PhysicsObj;          // sizeof 0x118

    typedef struct {
        u32 pad0;
        u32 flags;         // 0x04
        u8 state;          // 0x08
        u8 pad9[3];
        Vec3i pos;          // 0x0C
        u8 pad18[0xC];
        f32 m[9];          // 0x24
        u8 pad48[0x14];
        s32 kind;          // 0x5C
        PhysicsObj *obj;   // 0x60
        u8 pad64[0x20];
        u32 val;           // 0x84
    } Entity88;            // sizeof 0x88
    */
typedef struct { u8 pad0[8]; u8 state; u8 pad9[3]; Vec3i pos; u8 pad18[0xC]; f32 m[9]; u8 pad48[0x14]; s32 kind; u8 *obj; u8 pad64[0x24]; } E88_;
typedef struct { u8 pad0[4]; u32 flags; u8 pad8[4]; Vec3i pos; u8 pad18[0x70]; } E88b_;
typedef struct { u32 flags; u8 pad4[3]; u8 b7; u8 pad8[3]; u8 bB; } Inf12b_;
extern void (*svcTrackSmashObject[])(s32, Vec3f);
u8 func_8008E38C(Vec3i, u8, u8, s32);
extern u32 (*svcTrackGetLinkCount)(void);
extern Vec3f D_80100B00;
void *Memory_Alloc(s32 size);
void func_80099E78(Vec3i *, Vec3f, Vec3f, f32 *, s32);
void func_800AC880(u8 *, f32, f32, f32, f32, Vec3f);
void func_8009A4E4(u8 *o);
extern void (*svcTrackGetLink)(u32, Vec3i *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void (*svcTrackSetLinkTransform)(u32, f32, f32, f32, void *);
void func_800ACDC8(void *, f32);
void func_8007A9E0(void *);
extern u8 *D_800F3B90;
extern void *D_800F3B94;
extern u32 D_801206C0;
s32 func_8009A770(s32 *);
void func_8009A488(void *);
void Memory_Free(void *);



s32 func_800AF230(void) {
    Vec3f pos;
    Vec3f dir;
    f32 m[9];
    Vec3f dflt;
    u32 val;
    E88_ *e;
    u32 *info;
    u32 i;
    u8 *obj;
    s32 fl;

    dflt = D_80100B00;
    D_801206C0 = svcTrackGetLinkCount();
    if (D_801206C0 == 0) {
        return 1;
    }
    D_800F3B90 = Memory_Alloc(D_801206C0 * 0x88);
    memset(D_800F3B90, 0, D_801206C0 * 0x88);
    D_800F3B94 = Memory_Alloc(D_801206C0 * 12);
    memset(D_800F3B94, 0, D_801206C0 * 12);
    e = (E88_ *)D_800F3B90;
    info = D_800F3B94;
    for (i = 0; i < D_801206C0; e++, i++, info += 3) {
        svcTrackGetLink(i, (Vec3i *)&pos, m, (f32 *)&dir, (f32 *)&val, (f32 *)info, (f32 *)(info + 1), (f32 *)(info + 2));
        if (info[0] & 1) {
            *(s32 *)((u8 *)e + 4) |= 2;
        }
        if (info[0] & 2) {
            *(s32 *)((u8 *)e + 4) |= 4;
        }
        if (info[0] & 0x40) {
            *(s32 *)((u8 *)e + 4) |= 8;
        }
        if (info[0] & 8) {
            e->kind = 2;
        } else if (info[0] & 0x10) {
            e->kind = 1;
        } else {
            e->kind = 0;
        }
        *(u32 *)((u8 *)e + 0x84) = val;
        fl = (info[0] >> 4) & 1;
        func_80099E78(&e->pos, dir, pos, m, fl);
        if (e->kind == 1) {
            e->obj = obj = Memory_Alloc(0x118);
            memset(obj, 0, 0x118);
            /* TODO (Clean version): 386.0f is gravitational acceleration in inches/sec^2 (~32.174 ft/s^2 * 12), converting weight into mass:
               #define GRAVITY_INCHES_PER_SEC2 386.0f
               func_800AC880(e->obj, (f32)val / GRAVITY_INCHES_PER_SEC2, dir.x, dir.y, dir.z, dflt);
            */
            func_800AC880(e->obj, (f32) val / 386.0f, dir.x, dir.y, dir.z, dflt);
            *(f32 *)(e->obj + 0x94) = m[0];
            *(f32 *)(e->obj + 0x98) = m[1];
            *(f32 *)(e->obj + 0x9C) = m[2];
            *(f32 *)(e->obj + 0xA0) = m[3];
            *(f32 *)(e->obj + 0xA4) = m[4];
            *(f32 *)(e->obj + 0xA8) = m[5];
            *(f32 *)(e->obj + 0xAC) = m[6];
            *(f32 *)(e->obj + 0xB0) = m[7];
            *(f32 *)(e->obj + 0xB4) = m[8];
            *(f32 *)(e->obj + 0x6C) = pos.x - *(f32 *)(e->obj + 0x60);
            *(f32 *)(e->obj + 0x70) = pos.y - *(f32 *)(e->obj + 0x64);
            *(f32 *)(e->obj + 0x74) = pos.z - *(f32 *)(e->obj + 0x68);
            e->obj[0x110] = 1;
        }
        func_8009A4E4((u8 *)e);
    }
    return 1;
}

s32 func_800AF5B4(void) {
    u8 *e;
    u32 i;

    if (D_800F3B90 != NULL) {
        e = D_800F3B90;
        /* TODO (Clean version): Avoid raw byte-offset casting; access fields via E88_ struct:
           for (i = 0; i < D_801206C0; i++) {
               func_8009A770((s32 *)e);
               if (e->kind == 1) {
                   Memory_Free(e->obj);
               }
               func_8009A488(&e->pos);
               e++;
           }
        */
        for (i = 0; i < D_801206C0; i++) {
            func_8009A770((s32 *)e);
            if (*(s32 *)(e + 0x5C) == 1) {
                Memory_Free(*(void **)(e + 0x60));
            }
            func_8009A488(e + 0xC);
            e += 0x88;
        }
        Memory_Free(D_800F3B90);
        Memory_Free(D_800F3B94);
        D_800F3B90 = NULL;
        D_800F3B94 = NULL;
    }
    return 1;
}


void func_800AF67C(void) {
    u32 i;
    E88_ *e;
    f32 v[3];
    Vec3i pos;
    s32 pad;
    f32 w[4];
    f32 m[10];
    f32 a;
    f32 b;
    f32 c;
    f32 d;

    e = (E88_ *) D_800F3B90;
    for (i = 0; i < D_801206C0; i++, e++) {
        if (e->kind == 1) {
            if (e->obj[0x110] == 0) {
                func_800ACDC8(e->obj, 0.01f);
                func_8007A9E0(e->obj + 0x94);
                v[0] = *(f32 *) (e->obj + 0x6C) + *(f32 *) (e->obj + 0x60);
                v[1] = *(f32 *) (e->obj + 0x70) + *(f32 *) (e->obj + 0x64);
                v[2] = *(f32 *) (e->obj + 0x74) + *(f32 *) (e->obj + 0x68);
                svcTrackSetLinkTransform(i, v[0], v[1], v[2], e->obj + 0x94);
            }
            e->state = e->obj[0x110];
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

void func_800AF820(E88b_ *e, Vec3f v) {
    s32 idx = e - (E88b_ *)D_800F3B90;
    Inf12b_ *inf = &((Inf12b_ *)D_800F3B94)[idx];

    e->flags |= 1;
    svcTrackSmashObject[0](idx, v);
    if (inf->flags & 4) {
        func_8008E38C(e->pos, inf->b7, inf->bB, 0);
    }
}

s32 func_800AF8EC(s32 arg0, u32 arg1) {
    /* TODO (Clean version): Simplify conditional expression:
       return (arg1 >= 4) ? 4 : 0;
    */
    return (arg1 >= 4U) * 4;
}

void func_800AF8FC(void) {

}
