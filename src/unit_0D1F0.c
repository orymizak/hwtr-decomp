#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[4]; u16 *unk4; } BufD_;
typedef struct { u32 w0; u32 w1; } GfxA_;
typedef struct { u8 p0[0x34]; GfxA_ *unk34; } ObjA2_;
extern s32 D_80138AA8;
extern s32 D_8014C1A8;
typedef struct { s32 f0; s32 f4; u8 p8[0x548]; } S_D_801455C0;
void func_80075600(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void func_80075640(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
f32 sinf(f32);
typedef struct { s16 x; s16 y; s16 z; u8 pad[10]; } Vtx6D_;
typedef struct { Vtx6D_ v[32]; u8 p200[0x30]; f32 r; f32 a; u8 p238; u8 mode; } Ring6D_;
    /* TODO (Clean version):
    Definición duplicada idéntica a S_D_801455C0 empleada para forzar el casteo en func_8006DC28. En C limpio debe eliminarse y reutilizar S_D_801455C0; se mantiene para preservar el matching binario.
    */
typedef struct { s32 f0; s32 f4; u8 p8[0x548]; } S_D2_;
typedef struct { f32 x; f32 y; f32 z; } V3D6_;
typedef struct { u32 t; V3D6_ pos; V3D6_ vel; f32 m[9]; } MvD6_;
void func_8006D608(MvD6_ *o, u32 dt);
f64 func_800E6E80(f64);
f64 func_800E6F60(f64);
void func_8006D6EC(Ring6D_ *s, u32 t);
void func_8006CF10(s32 *p, s32 dt);
extern S_D_801455C0 D_801455C0[];
void func_8006CA88(S_D_801455C0 *, s32, f32, u8);
s32 func_8007F5C4(s32,s32);
s32 func_8006DD60(s32 *,s32,s32);
extern s32 D_8014D980;

s32 func_800757E8(s32);
s32 func_8006D15C(s32 *, s32, s32);
s32 Memory_Free(s32);
extern s32 D_800F05E0;
s32 func_8007F57C(s32);
extern s32 D_800F05E4;

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006C5F0);

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006CA88);

    /* TODO (Clean version):
    Aritmética manual de punteros y accesos a campos reinterpretando un arreglo s32* (como *(f32 *)&p[135], (Ring6D_ *)(p + 4) y p + 148 + i * 16). En C limpio se debería definir y pasar un puntero a una estructura completa con sus miembros (Ring6D_, coordenadas float y arreglo de MvD6_); se mantiene para el match.
    */
void func_8006CF10(s32 *p, s32 dt) {
    u8 v;
    f32 t;
    u32 i;

    if (dt >= p[0]) {
        p[0] = 0;
        if (p[1] != -1) {
            func_800757E8(p[1]);
            p[1] = -1;
        }
        return;
    }
    if (p[1] != -1) {
        v = p[0] * 255.0f / 1000.0f;
        func_80075600(p[1], v, v, v);
        func_80075640(p[1], sinf(p[0] / 1000.0f * 3.141592f) * 1800.0f, *(f32 *)&p[135], *(f32 *)&p[139], *(f32 *)&p[143]);
    }
    p[0] -= dt;
    if (p[0] < 900) {
        t = (u32)dt / ((930 - p[0]) / 30);
    } else {
        t = (u32)dt;
    }
    func_8006D6EC((Ring6D_ *)(p + 4), t);
    for (i = 0; i < 12; i++) {
        func_8006D608(p + 148 + i * 16, dt);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006D15C);

void func_8006D608(MvD6_ *o, u32 dt) {
    V3D6_ v;
    f32 x;
    f32 y;
    f32 z;

    if (dt >= o->t) {
        o->t = 0;
        return;
    }
    o->t -= dt;
    o->pos.x += o->vel.x;
    o->pos.y += o->vel.y;
    o->pos.z += o->vel.z;
    x = o->vel.x;
    y = o->vel.y;
    z = o->vel.z;
    v.x = o->m[0] * x + o->m[1] * y + o->m[2] * z;
    v.y = o->m[3] * x + o->m[4] * y + o->m[5] * z;
    v.z = o->m[6] * x + o->m[7] * y + o->m[8] * z;
    o->vel.x = v.x;
    o->vel.y = v.y;
    o->vel.z = v.z;
}

void func_8006D6EC(Ring6D_ *s, u32 t) {
    u32 i;
    f32 ang;

    if (s->mode == 0) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].x = func_800E6E80(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].y = func_800E6F60(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].z = -((i & 1) * 65.0 / 3.0);
        }
    } else if (s->mode != 2) {
        for (i = 0; i < 32; i++) {
            ang = (f32) i * 3.14159265 / 16.0 + s->a;
            s->v[i].x = func_800E6E80(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].y = func_800E6F60(ang) * (s->r + (i & 1) * 65.0);
            s->v[i].z = 0;
        }
    } else {
        return;
    }
    s->r = s->r + (f32) t * 3.5;
    s->a = s->a + (f32) t * 0.001;
}

void func_8006DA40(S_D_801455C0 *e) {
    if (e->f4 != -1) {
        func_800757E8(e->f4);
    }
    e->f0 = 0;
    e->f4 = -1;
}

void func_8006DA88(void) {
    u32 i;

    for (i = 0; i < 8; i++) {
        D_801455C0[i].f0 = 0;
        D_801455C0[i].f4 = -1;
    }
}

void func_8006DAC4(s32 dt) {
    u32 i;

    for (i = 0; i < 8; i++) {
        func_8006CF10((s32 *)&D_801455C0[i], dt);
    }
}

void func_8006DB1C(s32 a, s32 b) {
    u32 i;

    for (i = 0; i < 8; i++) {
        func_8006D15C((s32 *)&D_801455C0[i], a, b & 0xFF);
    }
}

void func_8006DB84(s32 a, f32 b, u8 c) {
    s32 min = D_801455C0[0].f0;
    u32 best = 0;
    u32 i;

    for (i = 1; i < 8; i++) {
        s32 v = D_801455C0[i].f0;

        if (v == 0) {
            best = i;
            break;
        }
        if (v < min) {
            best = i;
            min = v;
        }
    }
    func_8006CA88(&D_801455C0[best], a, b, c);
}


    /* TODO (Clean version):
    Casteo innecesario a la estructura duplicada S_D2_ (&((S_D2_ *)&D_801455C0)[i]). En C limpio se iteraría directamente sobre el arreglo original (ej. S_D_801455C0 *p = &D_801455C0[i];); se mantiene para el match.
    */
void func_8006DC28(void) {
    u32 i;
    S_D2_ *p;

    for (i = 0; i < 8; i++) {
        p = &((S_D2_ *)&D_801455C0)[i];
        if (p->f4 != -1) {
            func_800757E8(p->f4);
        }
        p->f0 = 0;
        p->f4 = -1;
    }
}

void func_8006DC90(void) {
    func_8006DD60(&D_8014D980, 0x140, 0xF0);
}

void func_8006DCB8(void) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
}

void func_8006DCE8(s32 arg0) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F57C(arg0);
}

    /* TODO (Clean version):
    Reinterpretación forzada de la variable global D_800F05E0 mediante doble puntero (*(GfxA_ * *)&D_800F05E0). En C limpio, con D_800F05E0 correctamente tipada (o con casteo simple (GfxA_ *)D_800F05E0), se escribiría:
    if (D_800F05E0 != NULL) {
        *o->unk34 = *(GfxA_ *)D_800F05E0;
    }
    Se mantiene para el match.
    */
void func_8006DD34(ObjA2_ *o) {
    if ((*(GfxA_ * *)&D_800F05E0) != 0) {
        *o->unk34 = *(*(GfxA_ * *)&D_800F05E0);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_0D1F0", func_8006DD60);

    /* TODO (Clean version):
    Casteo forzado mediante doble puntero (*(BufD_ * *)&D_800F05E0). En C limpio bastaría con ((BufD_ *)D_800F05E0)->unk4 o tipar D_800F05E0 como puntero; se mantiene para el match.
    */
void func_8006DEF4(u16 *pal, s32 *idx) {
    s32 i;
    u16 *dst = (*(BufD_ * *)&D_800F05E0)->unk4;

    for (i = 0; i < 0x400; i++) {
        *dst++ = pal[*idx++];
    }
}

    /* TODO (Clean version):
    Múltiples artefactos de permuter para emparejar registros e instrucciones: uso explícito de 'register', casteo de puntero *(void **)&D_800F05E0, truco sin ramas con '&' bitwise en la condición ((1 != D_8014C1A8) & (D_80138AA8 != 0)), y bucle do-while con punteros incrementados manualmente. En C limpio se escribiría con condiciones lógicas estándar (&&) y un bucle for indexado (dst[i] = samples[idx[i]]); se mantiene para el match.
    */
void func_8006DF34(s16 *samples) {
    register s32 i;
    register s32 *idx;
    register u16 *dst;
    register s16 v;

    if (*(void **)&D_800F05E0 != NULL && D_800F05E4 != 0 && ((1 != D_8014C1A8) & (D_80138AA8 != 0))) {
        dst = ((BufD_ *)*(void **)&D_800F05E0)->unk4;
        i = 0;
        idx = &D_8014D980;
        do {
            v = samples[*idx];
            idx++;
            *dst = v;
            dst++;
            i++;
        } while (i < 0x400);
    }
}

void func_8006DFBC(void) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F5C4(0x20, 0x20);
    D_800F05E4 = 1;
}

void func_8006E00C(s32 arg0) {
    if (D_800F05E0 != 0) {
        Memory_Free(D_800F05E0);
        D_800F05E0 = 0;
    }
    D_800F05E0 = func_8007F57C(arg0);
    D_800F05E4 = 0;
}
