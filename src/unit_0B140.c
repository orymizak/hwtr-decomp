#include "common.h"
#include "include_asm.h"
typedef struct { s32 state; u8 pad[0x50]; } SlotC_;
extern SlotC_ D_800F03F8[];
extern u8 D_800F0548[];
void func_800D7150(void);
void *memset(void *dst, s32 value, u32 size);
typedef struct { u16 button; s8 stick_x; s8 stick_y; u8 errnum; u8 pad; } Pad_;
typedef struct { u8 buf[8]; s32 st; s32 mode; } PakSt_;
typedef struct { s32 keys[6]; u8 pad[0x54 - 0x18]; } KeySet_;
typedef struct { u8 c[0x54]; } Row6B_;
typedef struct { u32 w0; u32 w1; } GfxWAA_;
typedef union { GfxWAA_ words; s64 force; } GfxAA_;
typedef struct { GfxAA_ *f0; GfxAA_ *f4; u8 p8[0x318]; } PoolAA_;
typedef struct { f32 x; f32 y; f32 z; f32 vx; f32 vy; u8 type; } EntAA_;
/* TODO (Clean version):
Estructura duplicada idéntica a KeySet_ y no referenciada; en C limpio debería eliminarse. Se mantiene para el match.
*/
typedef struct { s32 keys[6]; u8 pad[0x54 - 0x18]; } KeySet2_;
typedef struct { s32 keys[21]; } KeySetB_;
typedef struct { s32 mode; s32 k[15]; u8 pad[0x54 - 0x40]; } Bind6C_;
s32 func_8006BA98();
s32 func_8006C274();
extern void *D_801455A0;
extern void *D_80145598;
extern void *D_8014559C;
extern void *D_801455A8;
extern void *D_801455AC;
extern s32 D_800F03E8;
extern Bind6C_ D_800F0400[];
s32 func_8006C0EC(u8 pad, u8 mode);
extern s32 D_800F0394[];
extern PoolAA_ D_8014C740[];
extern s32 D_8014C540;
extern GfxAA_ *D_8014EC78;
void GfxPool_Grow(PoolAA_ *, u32);
extern s32 D_80101E90;
extern u8 D_80101E94;
extern u8 D_80101E95;
extern u8 D_80101E96;
extern EntAA_ D_80101EA8[];
extern f32 D_800F02D0;
extern u8 D_800F0318[];
extern u8 D_800F0338[];
void func_8006B1AC(void);
void func_8007AFD4(s32, f32 (*)[4], f32 (*)[4]);
s32 func_80070BA4(f32 (*)[4]);
extern Row6B_ D_800F043C[];
extern KeySet_ D_800F0404[];
void func_800D63C0(Pad_ *, s32);
s32 func_800D6FA0(s32);
void func_800D7030(s32, s32);
void func_800D66D0(PakSt_ *, s32);
extern void func_800D6420(Pad_ *);
extern u8 D_800F0403;
extern u8 D_800F0390;
extern u8 D_800F03EB;
extern s32 D_80145568;
extern s32 D_8014556C;
extern s32 D_80145570;
extern s32 D_80145574;
extern s32 D_80145578;
extern s32 D_8014557C;
extern s32 D_80145580;
extern s32 D_80145584;
extern s32 D_80145588;
extern s32 D_8014558C;
extern s32 D_80145590;
extern s32 D_80145594;
extern s32 D_801455A4;
extern s32 D_801455B0;

s32 Memory_Free(s32);
extern s32 D_80102208;
extern s32 D_800F0414;
extern u8 D_800F03F4;
s32 func_800D7130();

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006A540);

/* TODO (Clean version):
D_80102208 se accede como arreglo mediante (&D_80102208)[i], pero fue declarada como escalar s32. En C limpio debería declararse como s32 D_80102208[3] y accederse con D_80102208[i]. Se mantiene para el match.
*/
s32 func_8006AA34(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Memory_Free((&D_80102208)[i]);
        (&D_80102208)[i] = 0;
    }
    return 1;
}

/* TODO (Clean version):
Comandos gráficos GBI (Fast3D) y verificación de crecimiento del pool de display lists expandidos manualmente con escrituras a GfxAA_ (w0/w1 en hexadecimal) y aritmética de punteros (&D_80102208)[i]. En C limpio deberían usarse las macros estándar de GBI (ej. gSPSetOtherMode, gDPSetEnvColor, etc.). Se mantiene para el match.
*/
s32 func_8006AA84(s32 cam, f32 *pos) {
    f32 m[3][4];
    f32 o[3][4];
    f32 d[2];
    s32 i;
    s32 j;
    s32 n = 0;
    s32 mtx;
    EntAA_ *e;
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][1] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
    m[0][0] = m[1][1] = m[2][2] = 1.0f;
    func_8006B1AC();
    if ((u32) (D_8014EC78 + (9)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (9)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040004); _g->words.w1 = (u32) ( 1); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04000C); _g->words.w1 = (u32) ( 1); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040014); _g->words.w1 = (u32) ( 0xFFFF); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04001C); _g->words.w1 = (u32) ( 0xFFFF); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDE000000); _g->words.w1 = (u32) ( D_800F0338); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE7000000); _g->words.w1 = (u32) ( 0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xFA000000); _g->words.w1 = (u32) ( (D_80101E94 << 24) | (D_80101E95 << 16) | (D_80101E96 << 8) | 0xC0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE2001D00); _g->words.w1 = (u32) ( 4); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xEE000000); _g->words.w1 = (u32) ( 0xFFFC0000); };
    for (i = 0; i < 3; i++) {
        if ((u32) (D_8014EC78 + (7)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (7)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xFD900000); _g->words.w1 = (u32) ( (&D_80102208)[i]); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF5900000); _g->words.w1 = (u32) ( 0x07000000); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE6000000); _g->words.w1 = (u32) ( 0); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF3000000); _g->words.w1 = (u32) ( 0x070FF400); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE7000000); _g->words.w1 = (u32) ( 0); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF5800400); _g->words.w1 = (u32) ( 0); };
        { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xF2000000); _g->words.w1 = (u32) ( 0x0007C07C); };
        for (j = 0, e = D_80101EA8; j < D_80101E90; e++, j++) {
            if (e->type == i) {
                d[0] = pos[0] - e->x;
                d[1] = pos[1] - e->y;
                if (d[0] * d[0] + d[1] * d[1] < 1.296e9f) {
                    m[0][3] = e->x;
                    m[1][3] = e->y;
                    m[2][3] = e->z;
                    func_8007AFD4(cam, m, o);
                    o[0][3] *= D_800F02D0;
                    o[1][3] *= D_800F02D0;
                    o[2][3] *= D_800F02D0;
                    mtx = func_80070BA4(o);
                    if ((u32) (D_8014EC78 + (2)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (2)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
                    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDA380003); _g->words.w1 = (u32) ( mtx); };
                    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDE000000); _g->words.w1 = (u32) ( D_800F0318); };
                    n += 2;
                }
            }
        }
    }
    if ((u32) (D_8014EC78 + (7)) >= (u32) D_8014C740[D_8014C540].f4) { D_8014C740[D_8014C540].f0 = D_8014EC78; GfxPool_Grow(&D_8014C740[D_8014C540], (7)); D_8014EC78 = D_8014C740[D_8014C540].f0; };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040004); _g->words.w1 = (u32) ( 3); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04000C); _g->words.w1 = (u32) ( 3); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB040014); _g->words.w1 = (u32) ( 0xFFFD); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xDB04001C); _g->words.w1 = (u32) ( 0xFFFD); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE7000000); _g->words.w1 = (u32) ( 0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE3001801); _g->words.w1 = (u32) ( 0); };
    { GfxAA_ *_g = (GfxAA_ *) (D_8014EC78++); _g->words.w0 = (0xE2001D00); _g->words.w1 = (u32) ( 0); };
    return n;
}

extern f32 D_80101E98[2];
extern f32 D_80101EA0[2];
void func_8006B1AC(void) {
    s32 i;
    EntAA_ *p;

    for (i = 0, p = D_80101EA8; i < D_80101E90; p++, i++) {
        p->x += p->vx;
        p->y += p->vy;
        if (p->x > D_80101E98[0]) {
            p->x = D_80101EA0[0];
        } else if (p->x < D_80101EA0[0]) {
            p->x = D_80101E98[0];
        }
        if (p->y > D_80101E98[1]) {
            p->y = D_80101EA0[1];
        } else if (p->y < D_80101EA0[1]) {
            p->y = D_80101E98[1];
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006B2A0);

s32 func_8006B6B8(u8 ch, s32 key);
s32 func_8006B894(s32 key);
u8 func_8006BD78(u8 a, s32 b);
s32 func_8006BDA8(u8 ch, s32 slot);
s32 func_8006BE34(u8 ch, s32 slot);
u8 func_8006C0A0(void);
s32 func_8006C0AC(s32 arg0);
u8 func_8006C0C4(s32 arg0);
s32 func_8006C218(s32 arg0);
s32 func_8006C23C(u8 i);
s32 func_8006C334(void);
s32 func_8006C33C(void);
s32 func_8006C34C(u8 *buf, u8 id);
s32 func_8006C3D4(u8 *out, u8 ch);
s32 func_8006C468(void);
s32 func_8006C488(void);
/* TODO (Clean version):
Asignación de punteros a funciones a variables escalares mediante casts como (*(void * *)&D_80145568). En C limpio, las variables D_80145568..D_801455B0 deberían ser miembros de una estructura o arreglo de punteros a función. Se mantiene para el match.
*/
s32 func_8006B4F8(void) {
    (*(void * *)&D_80145568) = func_8006B894;
    D_801455A0 = func_8006B6B8;
    (*(void * *)&D_8014556C) = func_8006BA98;
    (*(void * *)&D_80145570) = func_8006BD78;
    (*(void * *)&D_801455A4) = func_8006C33C;
    (*(void * *)&D_80145574) = func_8006BDA8;
    (*(void * *)&D_80145578) = func_8006BE34;
    (*(void * *)&D_8014557C) = func_8006C0A0;
    (*(void * *)&D_80145580) = func_8006C0AC;
    (*(void * *)&D_80145584) = func_8006C0C4;
    (*(void * *)&D_80145588) = func_8006C0EC;
    (*(void * *)&D_801455B0) = func_8006C218;
    D_80145598 = func_8006C468;
    D_8014559C = func_8006C488;
    (*(void * *)&D_8014558C) = func_8006C23C;
    (*(void * *)&D_80145590) = func_8006C274;
    (*(void * *)&D_80145594) = func_8006C334;
    D_801455A8 = func_8006C34C;
    D_801455AC = func_8006C3D4;
    return 1;
}

/* TODO (Clean version):
Se limpian variables individuales escalares en lugar de usar memset sobre una estructura de punteros. Además, D_80145584 y D_80145588 se asignan a 0 dos veces mientras que las variables void* no se limpian. Se mantiene para el match.
*/
s32 func_8006B630(void) {
    D_80145568 = 0;
    D_8014556C = 0;
    D_80145570 = 0;
    D_801455A4 = 0;
    D_80145574 = 0;
    D_80145578 = 0;
    D_8014557C = 0;
    D_80145580 = 0;
    D_80145584 = 0;
    D_80145588 = 0;
    D_801455B0 = 0;
    D_8014558C = 0;
    D_80145590 = 0;
    D_80145594 = 0;
    D_80145584 = 0;
    D_80145588 = 0;
    return 1;
}

s32 func_8006B6B8(u8 ch, s32 key) {
    Pad_ pad[4];
    u16 mask;

    func_800D6420(pad);
    switch (key) {
    case 14:
        if (pad[ch].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case 15:
        if (pad[ch].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case 16:
        if (pad[ch].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case 17:
        if (pad[ch].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case 18:
        mask = 0x8000;
        break;
    case 19:
        mask = 0x4000;
        break;
    case 20:
        mask = 8;
        break;
    case 21:
        mask = 4;
        break;
    case 22:
        mask = 0x2000;
        break;
    case 23:
        mask = 0x10;
        break;
    case 24:
        mask = 2;
        break;
    case 25:
        mask = 1;
        break;
    case 26:
        mask = 0x1000;
        break;
    case 27:
        mask = 0x9000;
        break;
    default:
        return 0;
    }
    return (pad[ch].button & mask) != 0;
}

void func_8006B838(void) {
    s32 i;
    s32 t;

    for (i = 0; i < 4; i++) {
        t = D_800F0400[i].k[0];
        D_800F0400[i].k[0] = D_800F0400[i].k[1];
        D_800F0400[i].k[1] = t;
        t = D_800F0400[i].k[5];
        D_800F0400[i].k[5] = D_800F0400[i].k[4];
        D_800F0400[i].k[4] = t;
    }
}

s32 func_8006B894(s32 key) {
    Pad_ pad[4];
    u16 mask;

    func_800D6420(pad);
    switch (key) {
    case 14:
        if (pad[0].stick_y > 0x28 || pad[1].stick_y > 0x28 || pad[2].stick_y > 0x28 || pad[3].stick_y > 0x28) {
            return 1;
        }
        mask = 0x800;
        break;
    case 15:
        if (pad[0].stick_y < -0x28 || pad[1].stick_y < -0x28 || pad[2].stick_y < -0x28 || pad[3].stick_y < -0x28) {
            return 1;
        }
        mask = 0x400;
        break;
    case 16:
        if (pad[0].stick_x < -0x28 || pad[1].stick_x < -0x28 || pad[2].stick_x < -0x28 || pad[3].stick_x < -0x28) {
            return 1;
        }
        mask = 0x200;
        break;
    case 17:
        if (pad[0].stick_x > 0x28 || pad[1].stick_x > 0x28 || pad[2].stick_x > 0x28 || pad[3].stick_x > 0x28) {
            return 1;
        }
        mask = 0x100;
        break;
    case 18:
        mask = 0x8000;
        break;
    case 19:
        mask = 0x4000;
        break;
    case 20:
        mask = 8;
        break;
    case 21:
        mask = 4;
        break;
    case 22:
        mask = 0x2000;
        break;
    case 23:
        mask = 0x10;
        break;
    case 24:
        mask = 2;
        break;
    case 25:
        mask = 1;
        break;
    case 26:
        mask = 0x1000;
        break;
    case 27:
        mask = 0x9000;
        break;
    default:
        return 0;
    }
    if ((pad[0].button & mask) || (pad[1].button & mask) || (pad[2].button & mask) || (pad[3].button & mask)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006BA98);

u8 func_8006BD78(u8 a, s32 b) {
    return D_800F043C[a].c[b];
}

s32 func_8006BDA8(u8 ch, s32 slot) {
    s32 v;
    s32 i;
    s32 m;

    v = D_800F0404[ch].keys[slot];
    for (i = 1, m = 1; m != v && i < 21; i++) {
        m <<= 1;
    }
    if (i < 21) {
        return D_800F0394[i];
    }
    return D_800F0394[0];
}

/* TODO (Clean version):
Cadena repetitiva de comprobaciones "if (mask == 0 && ...)" para emular selección de prioridad sin ramas else-if. En C limpio se escribiría con una cadena if / else if directa. Se mantiene para el match.
*/
s32 func_8006BE34(u8 ch, s32 slot) {
    Pad_ pad;
    s32 mask;

    func_800D63C0(&pad, ch);
    mask = 0;
    if (mask == 0 && pad.stick_x > 0x28) mask = 0x10000;
    if (mask == 0 && pad.stick_x < -0x28) mask = 0x20000;
    if (mask == 0 && pad.stick_y > 0x28) mask = 0x80000;
    if (mask == 0 && pad.stick_y < -0x28) mask = 0x40000;
    if (mask == 0 && (pad.button & 0x8000)) mask = 0x8000;
    if (mask == 0 && (pad.button & 0x4000)) mask = 0x4000;
    if (mask == 0 && (pad.button & 0x20)) mask = 0x20;
    if (mask == 0 && (pad.button & 0x10)) mask = 0x10;
    if (mask == 0 && (pad.button & 0x2000)) mask = 0x2000;
    if (mask == 0 && (pad.button & 0x800)) mask = 0x800;
    if (mask == 0 && (pad.button & 0x200)) mask = 0x200;
    if (mask == 0 && (pad.button & 0x100)) mask = 0x100;
    if (mask == 0 && (pad.button & 0x400)) mask = 0x400;
    if (mask == 0 && (pad.button & 8)) mask = 8;
    if (mask == 0 && (pad.button & 2)) mask = 2;
    if (mask == 0 && (pad.button & 1)) mask = 1;
    if (mask == 0 && (pad.button & 4)) mask = 4;
    if (mask != 0) {
        if (slot == 0 || slot == 5) {
            D_800F0404[ch].keys[0] = mask;
            D_800F0404[ch].keys[5] = mask;
        } else if (slot == 1 || slot == 4) {
            D_800F0404[ch].keys[1] = mask;
            D_800F0404[ch].keys[4] = mask;
        } else {
            D_800F0404[ch].keys[slot] = mask;
        }
        return 1;
    }
    return 0;
}

u8 func_8006C0A0(void) {
    return D_800F03EB;
}

/* TODO (Clean version):
Aritmética manual de punteros con cast (*(s32 *)(&D_800F0390 + ((arg0 & 0xFF) * 4))) sobre una variable u8. En C limpio debería declararse extern s32 D_800F0390[] y accederse directamente como D_800F0390[(u8)arg0]. Se mantiene para el match.
*/
s32 func_8006C0AC(s32 arg0) {
    return *(s32 *)(&D_800F0390 + ((arg0 & 0xFF) * 4));
}

/* TODO (Clean version):
Aritmética manual de punteros (*(u8 *)(&D_800F0403 + ((arg0 & 0xFF) * 0x54))) calculando el stride de 0x54 bytes manualmente sobre un u8. En C limpio debería accederse a través de un arreglo de estructuras (ej. D_800F0400[(u8)arg0].campo). Se mantiene para el match.
*/
u8 func_8006C0C4(s32 arg0) {
    return *(u8 *)(&D_800F0403 + ((arg0 & 0xFF) * 0x54));
}

s32 func_8006C0EC(u8 pad, u8 mode) {
    if (mode >= D_800F03E8) {
        return 0;
    }
    D_800F0400[pad].mode = mode;
    if (mode == 0) {
        D_800F0400[pad].k[0] = 0x20000;
        D_800F0400[pad].k[1] = 0x10000;
        D_800F0400[pad].k[2] = 0x8000;
        D_800F0400[pad].k[3] = 0x4000;
        D_800F0400[pad].k[4] = 0x10000;
        D_800F0400[pad].k[5] = 0x20000;
        D_800F0400[pad].k[6] = 0x40000;
        D_800F0400[pad].k[7] = 0x80000;
        D_800F0400[pad].k[8] = 0x10;
        D_800F0400[pad].k[9] = 1;
        D_800F0400[pad].k[10] = 0x2000;
        D_800F0400[pad].k[11] = 4;
        D_800F0400[pad].k[12] = 2;
        D_800F0400[pad].k[13] = 0x1000;
    }
    return 1;
}

s32 func_8006C218(s32 arg0) {
    func_8006C0EC(arg0 & 0xFF, 0);
    return 1;
}

/* TODO (Clean version):
Arreglo local "unused[16]" no utilizado, forzado artificialmente para ajustar el tamaño del frame de pila y lograr el match binario. En C limpio debe eliminarse.
*/
s32 func_8006C23C(u8 i) {
    u8 unused[16];

    return D_800F03F8[i].state == 2;
}

INCLUDE_ASM("asm/nonmatchings/unit_0B140", func_8006C274);

s32 func_8006C334(void) {
    return 0;
}

s32 func_8006C33C(void) {
    return D_800F03F4 & 1;
}

/* TODO (Clean version):
D_800F0404 fue declarado con KeySet_ donde keys tiene tamaño 6 (s32 keys[6]), pero el bucle itera hasta 13 escribiendo fuera de los límites del arreglo sobre el padding. En C limpio la estructura debería definir el arreglo con al menos 13 elementos (ej. s32 keys[13]). Se mantiene para el match.
*/
s32 func_8006C34C(u8 *buf, u8 id) {
    s32 sum;
    s32 i;

    sum = 0;
    for (i = 0; i < 13; i++) {
        sum += buf[i];
    }
    if ((u8)sum != buf[13]) {
        return 0;
    }
    for (i = 0; i < 13; i++) {
        D_800F0404[id].keys[i] = 1 << buf[i];
    }
    return 1;
}

/* TODO (Clean version):
D_800F0404[ch].keys[j] accede hasta el índice 12 en un struct declarado con keys[6], indexando fuera de los límites sobre el padding. En C limpio la estructura debería tener el tamaño de arreglo adecuado. Se mantiene para el match.
*/
s32 func_8006C3D4(u8 *out, u8 ch) {
    s32 j;
    s32 n;
    s32 m;
    u8 sum;

    sum = 0;
    for (j = 0; j < 13; j++) {
        n = 0;
        m = 1;
        while (m != D_800F0404[ch].keys[j]) {
            m <<= 1;
            n++;
        }
        out[j] = n;
        sum += n;
    }
    out[13] = sum;
    return 1;
}

s32 func_8006C468(void) {
    func_800D7130();
    return 1;
}

s32 func_8006C488(void) {
    func_800D7150();
    memset(D_800F0548, 0, 2);
    return 1;
}


s32 func_8006C4BC(u8 i) {
    if (func_800D6FA0(i) == 0) {
        D_800F03F8[i].state = 2;
        func_800D7030(i, 2);
        return 1;
    }
    return 0;
}

/* TODO (Bug potential):
Si func_800D6FA0 retorna un estado distinto de 0 y 11 (o si st == 11 pero st != 10 tras func_800D66D0), s.mode queda sin inicializar en la primera iteración o retiene el valor de una iteración previa, asignando datos no inicializados o desactualizados a D_800F03F8[i].state.
*/
s32 func_8006C520(void) {
    PakSt_ s;
    s32 i;

    for (i = 0; i < 4; i++) {
        s.st = func_800D6FA0(i);
        if (s.st == 0) {
            s.mode = 2;
            func_800D7030(i, 2);
        } else if (s.st == 11) {
            func_800D66D0(&s, i);
            if (s.st == 10) {
                s.mode = 1;
            }
        }
        D_800F03F8[i].state = s.mode;
    }
    return 1;
}

