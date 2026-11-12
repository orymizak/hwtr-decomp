#include "common.h"
#include "include_asm.h"
typedef struct { s32 x; s32 y; s32 z; } V3i_;
typedef struct { u8 pad0[8]; u8 state; u8 pad9[3]; V3i_ pos; u8 pad18[0xC]; f32 m[9]; u8 pad48[0x14]; s32 kind; u8 *obj; u8 pad64[0x24]; } E88_;
typedef struct { f32 x; f32 y; f32 z; } Vec3f_;
extern u32 (*D_801416FC)(void);
extern Vec3f_ D_80100B00;
void *Memory_Alloc(s32 size);
void *memset(void *dst, s32 value, u32 size);
void func_80099E78(V3i_ *, Vec3f_, Vec3f_, f32 *, s32);
void func_800AC880(u8 *, f32, f32, f32, f32, Vec3f_);
void func_8009A4E4(u8 *o);
extern void (*D_80141700)(u32, V3i_ *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void (*D_80141704)(u32, f32, f32, f32, void *);
void func_800ACDC8(void *, f32);
void func_8007A9E0(void *);
extern u8 *D_800F3B90;
extern void *D_800F3B94;
extern u32 D_801206C0;
s32 func_8009A770(s32 *);
void func_8009A488(void *);
void Memory_Free(void *);



s32 func_800AF230(void) {
    Vec3f_ pos;
    Vec3f_ dir;
    f32 m[9];
    Vec3f_ dflt;
    u32 val;
    E88_ *e;
    u32 *info;
    u32 i;
    u8 *obj;
    s32 fl;

    dflt = D_80100B00;
    D_801206C0 = D_801416FC();
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
        D_80141700(i, (V3i_ *)&pos, m, (f32 *)&dir, (f32 *)&val, (f32 *)info, (f32 *)(info + 1), (f32 *)(info + 2));
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
    V3i_ pos;
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
                D_80141704(i, v[0], v[1], v[2], e->obj + 0x94);
            }
            e->state = e->obj[0x110];
        } else if (e->kind == 2) {
            D_80141700(i, &pos, m, w, &a, &b, &c, &d);
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

INCLUDE_ASM("asm/nonmatchings/unit_4FE30", func_800AF820);

s32 func_800AF8EC(s32 arg0, u32 arg1) {
    return (arg1 >= 4U) * 4;
}

void func_800AF8FC(void) {

}
