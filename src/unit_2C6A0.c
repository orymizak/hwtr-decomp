#include "common.h"
#include "include_asm.h"
extern void (*D_80141780)(void);
extern void (*D_8014C0A8)(s32, s32, f32, f32);
extern u8 (*D_8014C120)(void);
void func_8008E700(void);
typedef struct { u8 id; u8 b; u8 pad[2]; s32 c; } Ent8_;
typedef struct { u8 b0; u8 pad[0xB]; s32 wC; } E10_;
typedef struct { u8 b0; u8 pad[0xF]; } E10b_;
extern E10b_ D_801184C8[];
extern u8 D_801184C2;
extern s32 D_80118528;
extern void (*D_8014C13C)(u8, u8, u8 *);
typedef struct { s32 w; u8 p[16]; } E14t_;
typedef struct { u8 pad[0x30]; E14t_ e[1]; } T8D_;
typedef struct { u8 pad[0x12]; u8 b; u8 pad2[0x19]; } E2Cd_;
typedef struct { u8 pad[0x30]; E2Cd_ e[1]; } T8B_;
typedef struct { f32 x; f32 y; f32 z; } V3E0_;
typedef struct { s32 kind; u8 p4; u8 slot; u8 p6[14]; } E20_;
typedef struct { u8 p0[0x30]; E20_ ent[6]; } Cfg8B_;
extern u8 D_8011849A[];
extern u8 D_801184A8;
extern u8 D_801184A9;
extern u8 D_801184AA;
extern u8 D_801184AC;
extern u8 D_801184AE;
extern s32 D_801184A4;
extern s32 D_800F36E0;
extern u8 (*D_801417A8)(u8 *);
extern u8 (*D_801416E8)(u8 *);
extern u8 (*D_801417AC)(u8 *);
extern void (*D_8014559C)(void);
u8 func_8008D5EC(void);
void func_800A76F0(void);
void func_800908E8(void);
s32 func_800908A8(void);
void func_80090934(void);
void func_8009ABC8(void);
void func_80085780(void);
u8 func_800A90F0(u8 *);
u8 func_800AEA40(u8 *);
u8 func_80087990(u8 *);
u8 func_80080550(u8 *);
u8 func_80098BC0(u8 *);
u8 func_8009930C(u8 *);
u8 func_800853F4(u8 *);
u8 func_80091210(u8 *);
u8 func_8008E8F0(u8 *);
u8 func_800AF230(u8 *);
u8 func_8008955C(u8 *);
u8 func_800A7B88(u8 *);
u8 func_800A58D0(u8 *);
u8 func_800AEF80(u8 *);
extern void (*D_8014C10C)(u8, s32, f32);
extern u32 (*D_80141788)(void);
extern void (*D_80141784)(s32);
extern s32 (*D_80141698)(void);
extern void (*D_801417B8)(s32);
extern u8 D_801184B0;
extern u8 D_801184AF;
extern u8 D_801184AB;
extern s32 D_800F36DC;
extern s32 D_800F36D8;
extern u8 (*D_801417B0)(u8 *);
extern u8 (*D_801417B4)(u8 *);
extern u8 (*D_801416EC)();
extern u32 D_80118494;
extern u8 D_8011849C[];
extern s32 D_800F36F0;
u8 func_800AEFBC(u8 *);
u8 func_800A5950(u8 *);
u8 func_800A7C2C(u8 *);
u8 func_80089848(u8 *);
u8 func_800AF5B4(u8 *);
u8 func_8008EA64(u8 *);
u8 func_8008D848(u8 *);
u8 func_80091BA0(u8 *);
u8 func_800858E8(u8 *);
u8 func_800AEAFC(u8 *);
u8 func_800993B8(u8 *);
u8 func_80098BEC(u8 *);
u8 func_800806FC();
u8 func_80087AD4(u8 *);
u8 func_800A9370(u8 *);
extern u8 D_801184C1;
extern u8 D_800F3700;
extern E10_ *D_800F3704;
extern void (*D_8014C0DC)(u8, s32);
extern void (*D_8014C12C)(u8, s32, s32, s32);
extern s32 (*D_8014C0B0)();
extern void (*D_8014C0AC)(s32);
extern u8 D_800F36E8;
extern u32 D_800F36EC;
extern u32 D_800F36E4;
extern u8 D_80118498[];
u32 func_800A76D8(void);
u8 func_800A7C34(u8);
extern Ent8_ D_80118538[];
extern void (*D_8014C130)(s32);
extern s32 D_800F378C[];
extern s32 D_800F3770[];
extern s32 D_80141448;
extern s32 D_800F36D0[];
extern s32 D_8014C1A8;

extern s32 D_800F36F4;
extern s32 D_801184B4;
extern s32 D_801184B8;
extern s32 (*D_801416A0)(s32);
extern s32 D_8011852C;
extern u8 D_80118530;
extern u32 D_80118534;

void func_8008D3B8(void *arg0);
void func_8008D3D4(void);
s32 func_8008D4E4(u8 *t);
s32 func_8008BAA0(u8 *cfg) {
    u8 i;

    D_800F36F0 = 0;
    D_80118494 = cfg[0x18];
    D_800F36E4 = 0;
    for (i = 0; i < D_80118494; i++) {
        D_8011849C[i] = 0;
        if (((Cfg8B_ *)cfg)->ent[i].kind == 1) {
            D_8011849A[((Cfg8B_ *)cfg)->ent[i].slot] = i;
            D_800F36E4++;
        }
    }
    for (i = 0; i < 2; i++) {
        D_80118498[i] = 0;
    }
    if (*(s32 *)cfg & 4) {
        D_800F36EC = *(s32 *)(cfg + 0x20);
        D_800F36E8 = 1;
    } else {
        D_800F36E8 = 0;
    }
    D_801184AC = *(s32 *)cfg & 1;
    D_801184AA = 0;
    D_801184A9 = 0;
    D_801184A8 = 0;
    D_801184AB = 0;
    D_801184AF = 1;
    D_801184AE = 0;
    D_801184B0 = 0;
    if (!D_801417A8(cfg)) {
        return 0;
    }
    ((void (*)(u8 *))func_8008D3B8)(cfg);
    func_8008D3D4();
    if (!func_800A90F0(cfg)) {
        return 0;
    }
    func_8008D3D4();
    func_8008D3D4();
    if (!D_801416E8(cfg)) {
        return 0;
    }
    func_8008D3D4();
    if (!func_80087990(cfg)) {
        return 0;
    }
    if (!func_80080550(cfg)) {
        return 0;
    }
    if (!func_80098BC0(cfg)) {
        return 0;
    }
    if (!func_8009930C(cfg)) {
        return 0;
    }
    func_8008D3D4();
    if (!func_800AEA40(cfg)) {
        return 0;
    }
    if (!func_800853F4(cfg)) {
        return 0;
    }
    if (!func_80091210(cfg)) {
        return 0;
    }
    if (!((u8 (*)(u8 *))func_8008D4E4)(cfg)) {
        return 0;
    }
    if (!func_8008E8F0(cfg)) {
        return 0;
    }
    if (!func_800AF230(cfg)) {
        return 0;
    }
    if (!func_8008955C(cfg)) {
        return 0;
    }
    if (!func_800A7B88(cfg)) {
        return 0;
    }
    if (!func_800A58D0(cfg)) {
        return 0;
    }
    if (!func_800AEF80(cfg)) {
        return 0;
    }
    if (!func_8008D5EC()) {
        return 0;
    }
    func_8008D3D4();
    if (!D_801417AC(cfg)) {
        return 0;
    }
    D_8014C1A8 = 0;
    D_800F36F4 = 0;
    D_801417B8(0);
    func_800A76F0();
    func_800908E8();
    if (D_800F36E4 == 1 && D_80118494 == 6) {
        D_801184A4 = func_800908A8();
    } else {
        D_801184A4 = 0;
        func_80090934();
    }
    D_800F36DC = 0;
    D_800F36D8 = 0;
    D_800F36E0 = D_80141698();
    func_8009ABC8();
    func_80085780();
    D_8014559C();
    return 1;
}

u8 func_8008BE7C(u8 *o) {
    s32 err;
    u8 i;

    memset(o, 0, 0x124);
    if (D_801417B0(o) == 0) {
        return 0;
    }
    err = func_800AEFBC(o) == 0;
    err += func_800A5950(o) == 0;
    err += func_800A7C2C(o) == 0;
    err += func_80089848(o) == 0;
    err += func_800AF5B4(o) == 0;
    err += func_8008EA64(o) == 0;
    err += func_8008D848(o) == 0;
    err += func_80091BA0(o) == 0;
    err += func_800858E8(o) == 0;
    err += func_800AEAFC(o) == 0;
    err += func_800993B8(o) == 0;
    err += func_80098BEC(o) == 0;
    err += func_800806FC() == 0;
    err += func_80087AD4(o) == 0;
    err += D_801416EC() == 0;
    err += func_800A9370(o) == 0;
    if (D_801417B4(o) == 0) {
        return 0;
    }
    for (i = 0; i < D_80118494; i++) {
        ((T8B_ *)o)->e[i].b = D_8011849C[i];
    }
    *(s32 *)o = D_800F36F0;
    return err == 0;
}


s32 func_8008C08C(void) {
  if (D_8014C1A8 != 2) {
    ((s32 (*)()) D_800F36D0[D_8014C1A8])();
    return 0;
  }
  if (D_8014C1A8) {
    return 1;
  }
  else
  {
    return 1;
  }
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008C0D4);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008C548);

void func_8008CDAC(void) {
    func_8008E700();
    D_80141780();
    D_8014C0A8(0xE, 0, 1.0f, 1.0f);
}


void func_8008E80C(void);
void func_8008CDFC(void) {
    s32 v;

    switch (D_80141788()) {
        case 1:
            D_80141784(1);
            func_8008E80C();
            D_801184B0 = 0;
            D_801184AF = 0;
            v = D_80141698();
            D_800F36DC = v;
            D_800F36D8 = v;
            D_8014C1A8 = 0;
            break;
        case 2:
            D_80141784(2);
            D_801184B0 = 0;
            D_801184AF = 0;
            v = D_80141698();
            D_800F36DC = v;
            D_800F36D8 = v;
            D_8014C1A8 = 2;
            D_800F36F0 = 2;
            break;
        case 3:
            D_80141784(3);
            D_801184B0 = 0;
            D_801184AF = 0;
            v = D_80141698();
            D_800F36DC = v;
            D_800F36D8 = v;
            if (D_800F36F4 == 0) {
                D_8014C1A8 = 2;
            } else {
                D_8014C1A8 = 0;
                D_800F36F4 = 2;
                D_801417B8(3);
                D_801184AB = 1;
            }
            D_800F36F0 = 3;
            break;
    }
}

u8 func_8008CF90(void) {
    u8 ok = 1;
    u8 i;

    if (func_800A76D8() > 0x1B773F) {
        return 1;
    }
    if (D_800F36E8 != 0) {
        ok = !(func_800A76D8() < D_800F36EC);
    } else {
        for (i = 0; i < D_800F36E4; i++) {
            u8 v = func_800A7C34(i);

            if (v == 0) {
                ok = 0;
            }
            D_80118498[i] = v;
        }
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D060);

s32 func_8008D3AC(void) {
    return D_800F36F4;
}

void func_8008D3B8(void *arg0) {
    D_801184B4 = (*(u8 *)((u8 *)(arg0) + 0x18)) + 6;
    D_801184B8 = 0;
}

void func_8008D3D4(void) {
    D_801416A0((s32) (D_801184B8 * 0x64) / (s32) (D_801184B4 - 1));
    D_801184B8 += 1;
}

s32 func_8008D460(u32 i) {
    switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return D_800F3770[i];
    }
    return D_80141448;
}


s32 func_8008D48C(u32 i) {
    switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return D_800F378C[i];
    }
    return D_80141448;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D4B8);

s32 func_8008D4E4(u8 *t) {
    u8 n;
    u8 i;

    memset(D_801184C8, 0, 0x60);
    memset(&D_8011852C, 0, 0xC);
    D_80118530 = 1;
    n = t[0x18];
    D_801184C2 = 0;
    D_801184C1 = n;
    for (i = 0; i < D_801184C1; i++) {
        if (((T8D_ *)t)->e[i].w == 1) {
            D_801184C8[D_801184C1].b0 = i;
            D_801184C2++;
        }
    }
    D_8014C13C(D_801184C2, D_801184C1, t);
    D_80118528 = 0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D5EC);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D848);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008D9A0);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008DF18);

void func_8008E0BC(u8 **pair) {
    u8 *s;
    u8 *t;
    f32 d;
    V3E0_ v;

    if (D_80118528 != 1) {
        s = *(u8 **) (pair[0] + 0x64);
        t = *(u8 **) (pair[1] + 0x64);
        if ((u32) s < (u32) t) {
            s = t;
        }
        if (s != NULL) {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 0.0f;
            if (*(u8 **) (pair[0] + 0x60) != NULL) {
                v.x += *(f32 *) (*(u8 **) (pair[0] + 0x60) + 0x84);
                v.y += *(f32 *) (*(u8 **) (pair[0] + 0x60) + 0x88);
                v.z += *(f32 *) (*(u8 **) (pair[0] + 0x60) + 0x8C);
            }
            if (*(u8 **) (pair[1] + 0x60) != NULL) {
                v.x -= *(f32 *) (*(u8 **) (pair[1] + 0x60) + 0x84);
                v.y -= *(f32 *) (*(u8 **) (pair[1] + 0x60) + 0x88);
                v.z -= *(f32 *) (*(u8 **) (pair[1] + 0x60) + 0x8C);
            }
            d = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
            if (d > 176.0f) {
                if (d < 352.0f) {
                    D_8014C10C(*s, 2, d / 352.0f);
                } else if (d < 880.0f) {
                    D_8014C10C(*s, 3, d / 880.0f);
                } else {
                    D_8014C10C(*s, 4, d / 2304.0f);
                }
            }
        }
    }
}

void func_8008E2B0(void) {
    u32 i;

    memset(D_80118538, 0, 16);
    for (i = 0; i < 2; i++) {
        D_80118538[i].id = D_8014C120();
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E310);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E38C);

void func_8008E510(u8 i) {
    Ent8_ *e = &D_80118538[i];

    D_8014C130(e->id);
    e->b = 0;
    e->c = 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E558);

void func_8008E568(s32 arg0) {
    if ((arg0 == 3) || ((D_80118530 != 0) && ((u32) D_80118534 >= 0x3E9U))) {
        D_80118530 = 0;
        D_8011852C = arg0;
        D_80118534 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E5B8);

INCLUDE_ASM("asm/nonmatchings/unit_2C6A0", func_8008E700);

void func_8008E80C(void) {
    u8 i;

    for (i = 0; i < D_801184C1; i++) {
        D_8014C0DC(i, 0);
    }
    for (i = 0; i < D_800F3700; i++) {
        D_8014C12C(D_800F3704[i].b0, D_800F3704[i].wC, 0, 1);
    }
    D_8014C0AC(D_8014C0B0());
}

