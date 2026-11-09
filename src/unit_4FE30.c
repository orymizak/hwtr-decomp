#include "common.h"
#include "include_asm.h"
typedef struct { s32 x; s32 y; s32 z; } V3i_;
typedef struct { u8 pad0[8]; u8 state; u8 pad9[3]; V3i_ pos; u8 pad18[0xC]; f32 m[9]; u8 pad48[0x14]; s32 kind; u8 *obj; u8 pad64[0x24]; } E88_;
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



INCLUDE_ASM("asm/nonmatchings/unit_4FE30", func_800AF230);

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
