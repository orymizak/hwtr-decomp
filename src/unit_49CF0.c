#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
#include "race.h"
extern u8 D_800F3B3C[];
extern u16 gHudFlags[];
extern u16 D_800F3B38[];
typedef struct { u8 a[32]; } E32_;
typedef struct { E32_ e[7]; u8 pad[16]; } E240_;
typedef struct { s32 t; u8 a; u8 b; s8 c; u8 d; s32 pad; } E0Cb_;
typedef struct { s16 x0; s16 x1; s16 x; u16 y; s16 h8; u16 w; u8 bC; u8 bD; u8 gap; u8 pF; } El10_;
typedef struct { u8 on; u8 p1[0x27]; } Blk28_;
typedef struct { s32 t; s32 w4; s32 w8; u8 pC[4]; El10_ el[6]; Blk28_ blk[3]; u8 pE8; u8 flag; u8 bEA; u8 bEB; u8 pEC[4]; } HudF0_;
extern u8 gHudRacerCount;
extern char D_80100A94[];
extern HudF0_ D_800F3934[];
/* Wrong-way indicator per player (same 0xF0 stride as D_800F3934). */
typedef struct { s32 t; u8 pad[0xEC]; } HudWrongWayT_;
typedef struct { u8 on; u8 pad[0xEF]; } HudWrongWayOn_;
extern HudWrongWayT_ D_800F3930[];
extern HudWrongWayOn_ D_800F3A1C[];
void Hud_DrawSlideFromLeft(Blk28_ *, El10_ *, s32, s32);
void Hud_DrawSlideFromRight(Blk28_ *, El10_ *, s32, s32);
extern s32 gHudTimeLimit;
extern char D_80100A9C[];
extern s32 (*svcFontDrawChar)(s32, s32, u16, u16, s32);
extern void (*svcDraw2DSetColor)(s32, s32, s32);
extern E0Cb_ D_800F3B18[];
extern u8 gHudPlayerCount;
extern E240_ D_800F3951[];
extern E240_ D_800F3961[];
extern s32 (*svcFontFree)(s32);
s32 RaceClock_Get(s32);
extern u8 D_800F3938;
extern u8 D_800F393C;
extern u8 D_800F3A1E;
extern u8 D_800F3A1F;
extern s8 D_8011F68F;
extern u8 D_800F3B10;
extern u8 gHudRacer;

extern s8 D_800F3B45;
s8 Font_GetKerning(u8 font, u8 a, u8 b);

extern u8 gHudLapCount;
extern u8 gHudCountdown;
extern u8 D_800F3B3E;
extern u8 (*svcFontLoad)(s32, char *);
extern char D_80100A70[];
extern char D_80100A78[];
extern char D_80100A84[];

/* Sets up the HUD for a race: player count, visible elements and fonts. Returns 0 if a font failed to load. */
u8 Hud_Init(RaceSetup *cfg) {
    u8 (**load)(s32, char *);
    u16 flags;
    u8 i;
    u8 ok;

    flags = 0;
    if (cfg->count == 2 && cfg->p[0].kind == 1 && cfg->p[1].kind == 1) {
        gHudPlayerCount = 2;
        flags = 0xD91;
    } else if (cfg->p[0].kind == 1) {
        gHudPlayerCount = 1;
        flags |= 0xD11;
    } else {
        gHudPlayerCount = 0;
    }
    gHudRacerCount = cfg->count;
    if (cfg->flags & 2) {
        D_800F3B3E = 1;
        flags |= 0x220;
    } else {
        D_800F3B3E = 0;
        flags |= 0x62;
        if (!(cfg->flags & 8)) {
            flags |= 0xC;
            gHudLapCount = cfg->f19;
        }
    }
    if (cfg->flags & 4) {
        gHudCountdown = 1;
        gHudTimeLimit = cfg->timeLimit;
    } else {
        gHudCountdown = 0;
    }
    memset(D_800F3B18, 0, 0x18);
    for (i = 0; i < 2; i++) {
        D_800F3B18[i].c = -1;
        ((s32 *)&D_800F3B10)[i] = 0;
        gHudFlags[i] = flags;
        D_800F3B38[i] = flags;
        D_800F3B3C[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        (&D_8011F68F + 1)[i] = -1;
        (&D_8011F68F + 7)[i] = cfg->p[i].car;
    }
    D_800F3B45 = -1;
    load = &svcFontLoad;
    ok = (*load)(0, D_80100A70);
    if (ok) {
        if (gHudPlayerCount == 1) {
            ok = (*load)(1, D_80100A78);
        } else {
            ok = (*load)(1, D_80100A84);
        }
    }
    if (ok) {
        memset(D_800F3930, 0, 0x1E0);
        return 1;
    }
    return 0;
}

s32 Hud_Shutdown(void) {
    s32 (**fp)(s32) = &svcFontFree;
    s32 a = (*fp)(0) & 1;

    return a & (*fp)(1);
}


typedef struct { u8 pad[0xBC]; f32 spd; u8 pad2[0x7C]; u8 flag; } CarA_;
extern char D_80100A90[];
void Hud_DrawSpeed(u8 arg0, u8 arg1) {
    u8 buf[0x50];
    u8 idx;
    CarA_ *c;
    u16 x;
    u16 y;
    f32 f;
    s32 t;
    s32 v;
    s32 len;
    u8 *p;

    idx = arg0;
    c = (CarA_ *)Car_Get((&gHudRacer)[idx]);
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (!arg1) {
        x = 0;
        y = 0x15;
    } else {
        x = 0;
        y = idx ? 0x7F : 0x15;
    }
    svcFontDrawChar(1, 0, x, y, 2);
    svcDraw2DSetColor(0, 0xFF, 0xFF);
    f = c->spd;
    f *= 0.056818184f;
    if (f <= 80.0f) {
        f *= 100.0f;
        f /= 80.0f;
    } else {
        f -= 80.0f;
        f *= 80.0f;
        f /= 50.0f;
        f += 100.0f;
    }
    t = (s32)f;
    if (t > 180) {
        v = 180;
    } else {
        v = t;
    }
    v = c->flag == 0 ? v : 0;
    sprintf((char *)buf, D_80100A90, (u16)v);
    len = strlen((char *)buf);
    if (!arg1) {
        x = 0x24 - (((s8)len * 12) | 1) / 2;
        y = 0x12;
    } else {
        x = 0x24 - (((s8)len * 12) | 1) / 2;
        if (!arg0) {
            y = 0x12;
        } else {
            y = 0x7C;
        }
    }
    for (p = buf; *p != 0; p++) {
        svcFontDrawChar(1, *p, x, y, 1);
        x += 12;
    }
}

/* Draws the race position as "pos/racers" (yellow while leading). */
void Hud_DrawPosition(u8 idx, u8 flag) {
    u8 buf[80];
    Car *p;
    u8 *c;
    s32 x;
    s32 y;
    s32 pos;

    p = Car_Get((&gHudRacer)[idx]);
    if (p->flags & 0x4000) {
        return;
    }
    pos = p->position;
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0x103;
        y = 0xD7;
    } else {
        x = 0x103;
        y = 0xD7;
        if (idx == 0) {
            y = 0x6B;
        }
    }
    svcFontDrawChar(1, 0, x, y, 2);
    if (pos == 0) {
        svcDraw2DSetColor(0xFF, 0xFF, 0);
    } else {
        svcDraw2DSetColor(0, 0xFF, 0);
    }
    sprintf(buf, D_80100A94, pos + 1, gHudRacerCount);
    if (flag == 0) {
        x = 0x108;
        y = 0xD4;
    } else {
        x = 0x108;
        y = 0xD4;
        if (idx == 0) {
            y = 0x68;
        }
    }
    for (c = buf; *c != 0; c++) {
        svcFontDrawChar(1, *c, x, y, 1);
        if (*c < '0' || *c > '9') {
            x += 9;
        } else {
            x += 13;
        }
    }
}

void Hud_DrawTimeLeft(u8 idx, u8 flag) {
    u8 buf[80];
    Car *p;
    u8 *c;
    s32 x;
    s32 color;
    s32 t;

    p = Car_Get((&gHudRacer)[idx]);
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xCB;
        color = 0x15;
    } else {
        x = 0xCB;
        color = (-(idx != 0) & 0x7F) | 0x15;
    }
    svcFontDrawChar(1, 1, x, color, 2);
    svcDraw2DSetColor(0, 0xFF, 0);
    t = gHudTimeLimit - (((s32 (*)(void))RaceClock_Get)() - p->unk440);
    if (t < 0) {
        t = 0;
    }
    sprintf(buf, D_80100A9C, t / 60000, (u32)(t % 60000) / 1000, ((u32)(t % 60000) % 1000) / 100, (((u32)(t % 60000) % 1000) % 100) / 10);
    if (flag == 0) {
        x = 0xCF;
        color = 0x12;
    } else {
        x = 0xCF;
        color = 0x7C;
        if (idx == 0) {
            color = 0x12;
        }
    }
    for (c = buf; *c != 0; c++) {
        svcFontDrawChar(1, *c, x, color, 1);
        if (*c < '0' || *c > '9') {
            x += 7;
        } else {
            x += 13;
        }
    }
}

void Hud_DrawLapTimer(u8 idx, u8 flag) {
    u8 buf[80];
    Car *p;
    u8 *c;
    s32 x;
    s32 color;
    u32 t;

    p = Car_Get((&gHudRacer)[idx]);
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xCB;
        color = 0x15;
    } else {
        x = 0xCB;
        color = (-(idx != 0) & 0x7F) | 0x15;
    }
    svcFontDrawChar(1, 1, x, color, 2);
    svcDraw2DSetColor(0, 0xFF, 0);
    t = ((s32 (*)(void))RaceClock_Get)() - p->lapStart[p->lap - 1];
    sprintf(buf, D_80100A9C, t / 60000, (t % 60000) / 1000, ((t % 60000) % 1000) / 100, (((t % 60000) % 1000) % 100) / 10);
    if (flag == 0) {
        x = 0xCF;
        color = 0x12;
    } else {
        x = 0xCF;
        color = 0x7C;
        if (idx == 0) {
            color = 0x12;
        }
    }
    for (c = buf; *c != 0; c++) {
        svcFontDrawChar(1, *c, x, color, 1);
        if (*c < '0' || *c > '9') {
            x += 7;
        } else {
            x += 13;
        }
    }
}

/* Shows the time of the lap just completed for 5 s (blinking every 500 ms; yellow for a best lap). */
u8 Hud_DrawLapTimePopup(u8 idx, u8 flag) {
    u8 buf[80];
    u8 *c;
    s32 x;
    s32 color;
    u8 show;
    u32 t;

    if (D_800F3934[idx].bEA == 0) {
        return 0;
    }
    if ((u32)(((s32 (*)(void))RaceClock_Get)() - D_800F3934[idx].w8) > 5000) {
        D_800F3934[idx].bEA = 0;
        return 0;
    }
    show = ((u32)(((s32 (*)(void))RaceClock_Get)() - D_800F3934[idx].w8) / 500 & 1) ^ 1;
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xCB;
        color = 0x23;
    } else {
        x = 0xCB;
        color = 0x8D;
        if (idx == 0) {
            color = 0x23;
        }
    }
    svcFontDrawChar(1, 1, x, color, 2);
    if (show) {
        if (D_800F3934[idx].bEB) {
            svcDraw2DSetColor(0xFF, 0xFF, 0);
        } else {
            svcDraw2DSetColor(0xFF, 0, 0);
        }
        t = D_800F3934[idx].w4;
        sprintf(buf, D_80100A9C, t / 60000, (t % 60000) / 1000, ((t % 60000) % 1000) / 100, (((t % 60000) % 1000) % 100) / 10);
        if (flag == 0) {
            x = 0xCF;
            color = 0x20;
        } else {
            x = 0xCF;
            color = 0x8A;
            if (idx == 0) {
                color = 0x20;
            }
        }
        for (c = buf; *c != 0; c++) {
            svcFontDrawChar(1, *c, x, color, 1);
            if (*c < '0' || *c > '9') {
                x += 7;
            } else {
                x += 13;
            }
        }
    }
    return 1;
}

extern char D_80100A90[];
u32 strlen(const char *);

/* Draws car idx's trick score right-aligned on the HUD. */
void Hud_DrawScore(u8 idx, u8 flag) {
    u8 buf[80];
    u8 *c;
    s32 x;
    s32 y;
    s8 i;

    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xCB;
        y = 0x23;
    } else {
        x = 0xCB;
        y = 0x8D;
        if (idx == 0) {
            y = 0x23;
        }
    }
    svcFontDrawChar(1, 1, x, y, 2);
    svcDraw2DSetColor(0xFF, 0, 0);
    sprintf(buf, D_80100A90, Car_Get((&gHudRacer)[idx])->score);
    if (flag == 0) {
        x = 0x11E;
        y = 0x20;
    } else {
        x = 0x11E;
        y = 0x8A;
        if (idx == 0) {
            y = 0x20;
        }
    }
    i = strlen(buf) - 1;
    for (c = &buf[i]; i >= 0; i--, c--) {
        svcFontDrawChar(1, *c, x, y, 1);
        if (*c < '0' || *c > '9') {
            x -= 7;
        } else {
            x -= 13;
        }
    }
}

/* Draws the lap counter as "lap/laps" with its two label glyphs. */
void Hud_DrawLaps(u8 idx, u8 flag) {
    u8 buf[80];
    Car *p;
    u8 *c;
    u16 x;
    u16 y;

    p = Car_Get((&gHudRacer)[idx]);
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    if (flag == 0) {
        x = 0xDC;
        y = 0x23;
    } else {
        x = 0xDC;
        y = 0x8D;
        if (idx == 0) {
            y = 0x23;
        }
    }
    svcFontDrawChar(1, 3, x, y, 2);
    svcDraw2DSetColor(0xFF, 0, 0);
    if (flag == 0) {
        x = 0xE4;
        y = 0x20;
    } else {
        x = 0xE4;
        y = 0x8A;
        if (idx == 0) {
            y = 0x20;
        }
    }
    svcFontDrawChar(1, 4, x, y, 1);
    sprintf(buf, D_80100A94, (u8)(p->lap + 1), gHudLapCount);
    if (flag == 0) {
        x = 0x108;
        y = 0x20;
    } else {
        x = 0x108;
        y = 0x8A;
        if (idx == 0) {
            y = 0x20;
        }
    }
    for (c = buf; *c != 0; c++) {
        svcFontDrawChar(1, *c, x, y, 1);
        if (*c < '0' || *c > '9') {
            x += 9;
        } else {
            x += 13;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", Hud_DrawTurbos);

/* Draws the HUD elements enabled in gHudFlags[idx] for player idx. */
s32 Hud_DrawPlayer(u8 idx) {
    u8 two;
    u8 done;
    Car *p;
    u16 x;
    u16 y;

    two = gHudPlayerCount == 2;
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    done = 0;
    if (gHudFlags[idx] & 1) {
        Hud_DrawSpeed(idx, two);
    }
    if (gHudFlags[idx] & 0x10) {
        p = Car_Get((&gHudRacer)[idx]);
        svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
        if (p->unk6DC != 0) {
            if ((u8)two == 0) {
                x = 0x102;
                y = 0xB2;
            } else {
                x = 0x102;
                y = 0xB2;
                if (idx == 0) {
                    y = 0x46;
                }
            }
            svcFontDrawChar(1, (u8)(p->unk6DC + 10), x, y, 2);
        }
    }
    if (gHudFlags[idx] & 2) {
        if (gHudCountdown != 0) {
            Hud_DrawTimeLeft(idx, two);
        } else {
            Hud_DrawLapTimer(idx, two);
        }
        done = Hud_DrawLapTimePopup(idx, two);
    }
    if (gHudFlags[idx] & 0x200) {
        Hud_DrawTimeLeft(idx, two);
        Hud_DrawScore(idx, two);
    }
    if (gHudFlags[idx] & 8) {
        Hud_DrawPosition(idx, two);
    }
    if (((gHudFlags[idx] & 4) != 0) & (done == 0)) {
        Hud_DrawLaps(idx, two);
    }
    if (gHudFlags[idx] & 0x20) {
        Hud_DrawTurbos(idx, two);
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", Hud_DrawSlideFromLeft);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", Hud_DrawSlideFromRight);

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", Hud_DrawResults);

extern char D_80100ABC[];
extern char D_80100AC0[];
extern char *D_8014156C;
extern char D_80138D20[];
extern char D_801345B8[];

static inline void Hud_DrawKernedText(u8 *c, u16 x, u16 y) {
    s32 one;

    if (*c != 0) {
        one = 1;
        do {
            x += svcFontDrawChar(0, *c, x, y, one);
            x += Font_GetKerning(0, *c++, *c);
        } while (*c != 0);
    }
}

/* Airtime results: the best time and each player's name and score, leader first. */
void Hud_DrawScoreboard(void) {
    Car *cars[2];
    u8 buf[80];
    u8 *c;
    u16 x;
    u16 y;
    u16 one;
    u8 swap;
    u8 i;
    u32 t;

    cars[0] = Car_Get(0);
    swap = 0;
    if (gHudPlayerCount == 2) {
        cars[1] = Car_Get(1);
        swap = (u32)cars[1]->score > (u32)cars[0]->score;
    } else {
        cars[1] = NULL;
    }
    t = ((s32 (*)(void))RaceClock_Get)() - cars[0]->unk440;
    if (t < gHudTimeLimit) {
        gHudTimeLimit = t;
    }
    svcDraw2DSetColor(0, 0xFF, 0xFF);
    y = 0x3C;
    sprintf(buf, D_80100ABC);
    x = 0x1E;
    Hud_DrawKernedText(buf, x, y);
    sprintf(buf, D_80100AC0, D_8014156C);
    x = 0xC8;
    Hud_DrawKernedText(buf, x, y);
    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    y = 0x50;
    for (i = 0; i < gHudPlayerCount; i++) {
        one = 1;
        if (i == 0 && swap) {
            y += 0x14;
        }
        if (cars[i]->id == 0) {
            sprintf(buf, D_80100AC0, D_80138D20);
        } else {
            sprintf(buf, D_80100AC0, D_801345B8);
        }
        x = 0x1E;
        c = buf;
        if (*c != 0) {
            do {
                x += svcFontDrawChar(0, *c, x, y, one);
                x += Font_GetKerning(0, *c++, *c);
            } while (*c != 0);
        }
        sprintf(buf, D_80100A90, cars[i]->score);
        c = buf;
        x = 0xC8;
        if (*c != 0) {
            do {
                x += svcFontDrawChar(0, *c, x, y, one);
                x += Font_GetKerning(0, *c++, *c);
            } while (*c != 0);
        }
        if (swap == 0) {
            y += 0x14;
        } else {
            y -= 0x14;
        }
    }
}

extern char *D_80141440;

/* Blinks the "wrong way" text (500 ms phases) until 200 ms after the car stops driving the wrong way. */
void Hud_DrawWrongWay(u8 idx) {
    u8 buf[24];
    u8 *c;
    u16 x;
    u16 y;
    s32 one;
    u8 split;
    s32 phase;

    if (Car_Get((&gHudRacer)[idx])->wrongWay) {
        D_800F3A1C[idx].on = 1;
        D_800F3930[idx].t = ((s32 (*)(void))RaceClock_Get)();
    }
    if (D_800F3A1C[idx].on == 0) {
        return;
    }
    if ((u32)(((s32 (*)(void))RaceClock_Get)() - D_800F3930[idx].t) > 200) {
        D_800F3A1C[idx].on = 0;
        return;
    }
    phase = (u32)((s32 (*)(void))RaceClock_Get)() / 500 & 1;
    if (phase == 1) {
        return;
    }
    split = gHudPlayerCount == 2;
    sprintf(buf, D_80141440);
    if (split == 0) {
        x = 0x4A;
        y = 0x12;
    } else {
        x = 0x4A;
        y = 0x7C;
        if (idx == 0) {
            y = 0x12;
        }
    }
    svcDraw2DSetColor(0xFF, 0, 0);
    c = buf;
    if (*c != 0) {
        one = 1;
        do {
            x += svcFontDrawChar(0, *c, x, y, one);
            /* TODO(match-debt): unsequenced *c++ / *c; gcc 2.7 reads them left to right as the original did. */
            x += Font_GetKerning(0, *c++, *c);
        } while (*c != 0);
    }
}

void Hud_DrawTrickPopup(u8 p) {
    El10_ *e0;
    El10_ *e1;
    El10_ *e2;
    El10_ *e3;
    El10_ *e4;
    El10_ *e5;

    e0 = &D_800F3934[p].el[0];
    e1 = e0 + 1;
    e2 = e0 + 2;
    e3 = e0 + 3;
    e4 = e0 + 4;
    e5 = e0 + 5;
    if (D_800F3934[p].flag == 0) {
        return;
    }
    if (e0->bD && e2->bD && e4->bD && e1->bD && e3->bD && e5->bD) {
        D_800F3934[p].flag = 0;
        return;
    }
    if (D_800F3934[p].t == 0) {
        if (e0->bC || e2->bC || e4->bC || e1->bC || e3->bC || e5->bC) {
            D_800F3934[p].t = ((s32 (*)(void))RaceClock_Get)();
            e0->h8 = 0;
            e2->h8 = 0;
            e4->h8 = 0;
            e1->h8 = 0;
            e3->h8 = 0;
            e5->h8 = 0;
        }
    } else if ((u32)(((s32 (*)(void))RaceClock_Get)() - D_800F3934[p].t) > 1000) {
        e0->h8 = -20;
        e2->h8 = -20;
        e4->h8 = -20;
        e1->bD = 1;
        e3->bD = 1;
        e5->bD = 1;
    }
    svcDraw2DSetColor(0xFF, 0xFF, 0);
    if (D_800F3934[p].blk[0].on) {
        if (!e0->bD && !e1->bD) {
            Hud_DrawSlideFromLeft(&D_800F3934[p].blk[0], e0, 2, 1);
            Hud_DrawSlideFromRight(&D_800F3934[p].blk[0], e1, 2, 0);
        } else {
            Hud_DrawSlideFromLeft(&D_800F3934[p].blk[0], e0, 1, 0);
        }
    }
    if (D_800F3934[p].blk[1].on) {
        if (!e0->bD && !e1->bD) {
            Hud_DrawSlideFromLeft(&D_800F3934[p].blk[1], e2, 2, 1);
            Hud_DrawSlideFromRight(&D_800F3934[p].blk[1], e3, 2, 0);
        } else {
            Hud_DrawSlideFromLeft(&D_800F3934[p].blk[1], e2, 1, 0);
        }
    }
    if (D_800F3934[p].blk[2].on) {
        if (!e0->bD && !e1->bD) {
            Hud_DrawSlideFromLeft(&D_800F3934[p].blk[2], e4, 2, 1);
            Hud_DrawSlideFromRight(&D_800F3934[p].blk[2], e5, 2, 0);
        } else {
            Hud_DrawSlideFromLeft(&D_800F3934[p].blk[2], e4, 1, 0);
        }
    }
}

void Hud_SetPlayer(u8 i, s32 v, s8 id) {
    ((s32 *)&D_800F3B10)[i] = v;
    (&gHudRacer)[i] = id;
}

extern s32 gGameState;
extern char *D_80141438;
void Hud_DrawResults(void);
void Hud_DrawScoreboard(void);

/* Draws every player's HUD, the results once all of them finished, and the blinking banner in demo mode. */
void Hud_Draw(void) {
    u8 i;
    u8 finished;
    u8 *c;
    u16 x;
    s32 one;
    u8 ch;

    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    finished = 0;
    if (gGameState != 1) {
        for (i = 0; i < gHudPlayerCount; i++) {
            switch (((s32 *)&D_800F3B10)[i]) {
            case 1:
                Hud_DrawPlayer(i);
                if (gHudFlags[i] & 0x400) {
                    Hud_DrawTrickPopup(i);
                }
                Hud_DrawWrongWay(i);
                break;
            case 3:
                finished++;
                break;
            }
        }
    }
    if (finished == gHudPlayerCount) {
        if (gHudPlayerCount != 0) {
            if (D_800F3B3E) {
                Hud_DrawScoreboard();
            } else {
                Hud_DrawResults();
            }
        }
    }
    if (gHudPlayerCount == 0) {
        if (((u32)((s32 (*)(void))RaceClock_Get)() / 750 & 1) != 0) {
            return;
        }
        svcDraw2DSetColor(0xFF, 0, 0);
        c = D_80141438;
        x = 0x5E;
        if ((ch = *c) != 0) {
            one = 1;
            do {
                x += svcFontDrawChar(0, ch, x, 0x12, one);
                /* TODO(match-debt): unsequenced *c++ / *c (see Hud_DrawWrongWay). */
                x += Font_GetKerning(0, *c++, *c);
            } while ((ch = *c) != 0);
        }
    }
}

void Hud_SetResultRacer(s8 arg0, s32 arg1) {
    *(u8 *)(&D_8011F68F + (arg1 & 0xFF)) = arg0;
}

void Hud_SetBestLapRacer(s8 arg0) {
    D_800F3B45 = arg0;
}

void Hud_ShowLapTime(u8 i, s32 v, s8 b) {
    D_800F3934[i].bEB = b;
    D_800F3934[i].bEA = 1;
    D_800F3934[i].w4 = v;
    D_800F3934[i].w8 = RaceClock_Get(i);
}

INCLUDE_ASM("asm/nonmatchings/unit_49CF0", Hud_ShowTrick);

void Hud_ShowTurbosGained(u8 i, s32 n) {
    if (i < gHudPlayerCount) {
        D_800F3B18[i].t = ((s32 (*)(void))RaceClock_Get)();
        D_800F3B18[i].a = Car_Get((&gHudRacer)[i])->turbos;
        D_800F3B18[i].b = n;
        if (D_800F3B18[i].a + (u8)n >= 11) {
            D_800F3B18[i].b = 10 - D_800F3B18[i].a;
        }
        D_800F3B18[i].c = 9;
        D_800F3B18[i].d = 1;
    }
}


void Hud_SetVisible(u8 i, u8 v) {
    u8 j;

    if (D_800F3B3C[i] == v) {
        return;
    }
    if (v != 0) {
        if (gHudFlags[i] != D_800F3B38[i]) {
            gHudFlags[i] = D_800F3B38[i];
        } else {
            gHudFlags[i] = 0;
        }
    }
    D_800F3B3C[i] = v;
    for (j = 0; j < 3; j++) {
        D_800F3951[i].e[j].a[0] = 1;
        D_800F3961[i].e[j].a[0] = 1;
    }
}

