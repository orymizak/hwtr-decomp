#include "common.h"
#include "include_asm.h"
#include "car.h"
typedef struct { s32 flags; u8 p4[0x13]; u8 b17; u8 p18; u8 b19; u8 b1A; } TrkCfg_;
typedef struct { u8 p0[0x440]; s32 w440; u8 b444[0xC]; s32 w450[8]; s32 w470; u8 p474[4]; u8 b478; u8 b479; } CrA76_;
extern void (*D_801416E4)(s32);
s32 func_8008182C(void);
extern s32 D_800F3914;
s32 Car_IsPlayer(s32);
extern void (*D_8014C0A8)(s32, s32, f32, f32);
void func_800ABEC8(u8, u32, u8);
extern s32 D_800F3908;
extern s32 D_800F390C;
extern s32 D_800F3910;
extern u8 D_800F3918;
extern u8 D_800F3919;
extern u8 D_800F391A;

extern s32 D_800F3900;
extern s32 D_800F3904;

void func_800A76C0(s32 arg0) {
    D_800F3900 += arg0;
}

s32 func_800A76D8(void) {
    return D_800F3900;
}

s32 func_800A76E4(void) {
    return D_800F3904;
}

void func_800A76F0(void) {
    D_800F3900 = 0;
}

void func_800A76FC(void) {
    u8 i;
    u8 j;
    s32 z;
    CrA76_ *c;

    D_800F3914 = func_8008182C();
    D_8014C0A8(9, 1, 1.0f, 1.0f);
    D_801416E4(0);
    z = 0;
    D_800F3904 = D_800F3900;
    D_800F3900 = 0;
    for (i = 0; i < 6; i++) {
        c = Car_Get(i);
        c->w440 = z;
        c->b478 = 0;
        c->w470 = 0;
        c->b479 = 0;
        for (j = 0; j < 10; j++) {
            c->b444[j] = 0;
        }
        for (j = 0; j < 8; j++) {
            c->w450[j] = 0;
        }
        if (D_800F3918) {
            c->w450[0] = z;
        }
    }
}

s32 func_800A780C(u8 ch, u8 cp) {
    Car *c;
    u8 ok;
    u8 k;
    u8 i;
    u8 best;
    u32 t;

    c = Car_Get(ch);
    best = 0;
    if (c->finished != 0) {
        return 1;
    }
    k = cp - 1;
    ok = 1;
    for (i = 0; i < k; i++) {
        if (c->checkpoints[i] == 0) {
            ok = 0;
            break;
        }
    }
    if ((D_800F391A == 0) & (ok == 0)) {
        if ((u8)Car_IsPlayer(ch) != 0) {
            if (c->checkpoint != 0 || k != D_800F3910 - 1) {
                if (k != c->checkpoint - 1) {
                    D_8014C0A8(11, 0, 1.0f, 1.0f);
                }
            }
        }
    }
    if (!ok || c->checkpoints[k] != 0) {
        return 0;
    }
    if (k < (u32)(D_800F3910 - 1)) {
        if (D_800F391A == 0) {
            if ((u8)Car_IsPlayer(ch) != 0) {
                D_8014C0A8(10, 0, 1.0f, 1.0f);
            }
        }
        c->checkpoints[k] = 1;
        c->checkpoint++;
        goto done;
    }
    if (k != D_800F3910 - 1) {
        return 1;
    }
    if (D_800F391A == 0) {
        if ((u8)Car_IsPlayer(ch) != 0) {
            D_8014C0A8(12, 0, 1.0f, 1.0f);
        }
        t = D_800F3900;
        if (D_800F3918 == 1) {
            if (c->bestLap == 0 || t - c->lapStart[0] < c->bestLap) {
                c->bestLap = t - c->lapStart[0];
                best = 1;
            }
            if ((u8)Car_IsPlayer(ch) != 0) {
                func_800ABEC8(c->unk00C, t - c->lapStart[0], best);
            }
            c->lapStart[0] = t;
            c->lap = 1;
        } else {
            if (c->bestLap == 0 || t - c->lapStart[c->lap - 1] < c->bestLap) {
                c->bestLap = t - c->lapStart[c->lap - 1];
                best = 1;
            }
            if ((u8)Car_IsPlayer(ch) != 0) {
                func_800ABEC8(c->unk00C, t - c->lapStart[c->lap - 1], best);
            }
            c->lapStart[c->lap] = t;
            c->lap++;
            if (D_800F3919 == 0 && c->lap == D_800F390C) {
                c->finished = 1;
            }
        }
    }
    c->checkpoint = 0;
    for (i = 0; i < 10; i++) {
        c->checkpoints[i] = 0;
    }
done:
    return 1;
}

s32 func_800A7B88(void *arg0) {
    TrkCfg_ *cfg = arg0;

    D_800F3904 = 0;
    D_800F3908 = cfg->b17;
    D_800F390C = cfg->b19;
    D_800F3910 = cfg->b1A;
    if (cfg->flags & 2) {
        D_800F391A = 1;
    } else {
        D_800F391A = 0;
    }
    if (cfg->flags & 4) {
        D_800F3919 = 1;
    } else {
        D_800F3919 = 0;
    }
    if (cfg->flags & 8) {
        D_800F3918 = 1;
    } else {
        D_800F3918 = 0;
    }
    return 1;
}

s32 func_800A7C2C(void) {
    return 1;
}

s32 func_800A7C34(u8 i) {
    Car *p = Car_Get(i);

    if (p->finished != 0) {
        p->unk014 = 0.0f;
        p->unk018 = 0.0f;
        p->unk024 = 1;
        p->unk025 = 0;
        p->unk027 = 0;
        p->yaw = 1.0f;
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_482C0", func_800A7C8C);
