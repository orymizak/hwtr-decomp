#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[8]; s32 n; u8 pC[4]; s32 a[600]; s32 b[600]; f32 c[600]; } Queue7F_;
typedef struct { u8 pad[4]; s32 a; s32 b; void *c; } Buf20_;
s32 func_8007FD40(s32);
s32 func_80080070(s32);
extern s32 D_800F2690;

typedef struct { u32 w0; s32 w4; s32 w8; void *wC; } Hp2C0_;
typedef struct { Hp2C0_ *a; Hp2C0_ *b; } Hp2_;
typedef struct { u16 n; u8 p2[6]; s16 *v; u8 pC[4]; u8 *p10; } MeshFC_;
extern Hp2_ D_8010C410;
extern u8 D_8010C420[];

void func_8007FC30(void *arg0, MeshFC_ *m, s32 v, f32 *mtx) {
    s32 j;
    s32 i;
    s16 *s;

    if ((u32)(m->n + *(s32 *)((u8 *)arg0 + 8)) < 0x257U) {
        j = *(s32 *)((u8 *)arg0 + 8);
        for (i = 0; i < m->n; j++, i++) {
            *(u8 **)((u8 *)arg0 + j * 4 + 0x10) = m->p10 + i * 12;
            *(s32 *)((u8 *)arg0 + j * 4 + 0x970) = v;
            *(f32 *)((u8 *)arg0 + j * 4 + 0x12D0) = mtx[8] * m->v[i * 3] + mtx[9] * m->v[i * 3 + 1] + mtx[10] * m->v[i * 3 + 2] + mtx[11];
        }
        *(s32 *)((u8 *)arg0 + 8) = j;
    }
}

void func_8007FD04(void *arg0, s32 a, s32 b, f32 c) {
    Queue7F_ *q = arg0;
    s32 n = q->n;

    if ((u32)(n + 1) < 599) {
        q->a[n] = a;
        q->b[n] = b;
        q->c[n] = c;
        q->n++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_8007FD40);

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080070);

void func_800802C0(void) {
    D_8010C410.a = (Hp2C0_ *)D_8010C420;
    D_8010C410.b = (Hp2C0_ *)(D_8010C420 + 0x41F0);
    D_8010C410.a->w8 = 0;
    D_8010C410.b->w8 = 0;
    D_8010C410.a->w4 = 0;
    D_8010C410.b->w4 = 0;
    D_8010C410.a->wC = (u8 *)D_8010C410.a + 0x1C30;
    D_8010C410.b->wC = (u8 *)D_8010C410.b + 0x1C30;
    D_8010C410.a->w0 = 0xA87BC3E8;
    D_8010C410.b->w0 = 0xA87BC3E8;
    D_800F2690 = 0;
}

void func_80080360(void) {
    D_8010C410.a = 0;
    D_8010C410.b = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_20830", func_80080374);

void func_80080460(s32 arg0, s32 arg1, f32 arg2) {
  s32 temp_a2;
  void *temp_v0;
  void *temp_v1;
  temp_v1 = *((s32 *) (((u8 *) (&(*(u8 *)&D_8010C410))) + (D_800F2690 * 4)));
  temp_a2 = *((s32 *) (((u8 *) temp_v1) + 0x8));
  if (((u32) (temp_a2 + 1)) < 0x257U) {
    ;
    *((s32 *) (((u8 *) ((temp_a2 * 4) + temp_v1)) + 0x10)) = arg0;
    *((s32 *) (((u8 *) ((temp_a2 * 4) + temp_v1)) + 0x970)) = arg1;
    *((f32 *) (((u8 *) ((temp_a2 * 4) + temp_v1)) + 0x12D0)) = arg2;
    *((s32 *) (((u8 *) temp_v1) + 0x8)) = (s32) ((*((s32 *) (((u8 *) temp_v1) + 0x8))) + 1);
  }
}

void func_800804B4(void) {
    func_8007FD40(*(s32 *)(&(*(u8 *)&D_8010C410) + (D_800F2690 * 4)));
    func_80080070(*(s32 *)(&(*(u8 *)&D_8010C410) + (D_800F2690 * 4)));
}

void func_80080500(void) {
    s32 i = D_800F2690 ^ 1;

    ((Buf20_ * *)&D_8010C410)[i]->a = 0;
    ((Buf20_ * *)&D_8010C410)[i]->b = 0;
    D_800F2690 = i;
    ((Buf20_ * *)&D_8010C410)[i]->c = (u8 *)((Buf20_ * *)&D_8010C410)[i] + 0x1C30;
}

