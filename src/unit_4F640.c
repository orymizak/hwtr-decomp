#include "common.h"
#include "ultra64.h"
#include "race.h"
#include "include_asm.h"
extern s32 (*D_801416AC)();
extern s32 (*D_801416B0)();
extern void (*svcFontDrawChar)(s32, u8, u16, s32, s32);
extern char D_80100AF0[];
void Cars_CollideObstacles(void);
void Debris_Draw(u32);
s32 RaceView_DrawScene(u32);
void Hud_Draw(void);
u32 strlen(const char *);
extern s32 D_801415B8;
extern u8 gShowFps;
extern u8 D_800F3B81;

extern s32 (*svcScreenBeginFrame)();
extern s32 (*svcScreenEndFrame)();
extern s32 gRaceViewCount;

s32 RaceView_Init(RaceSetup *t) {
    u8 i;

    gRaceViewCount = 0;
    for (i = 0; i < t->count; i++) {
        if (t->p[i].kind == 1) {
            gRaceViewCount++;
        }
    }
    if (gRaceViewCount == 0) {
        gRaceViewCount = 1;
    }
    D_801415B8 = t->count;
    /* TODO (Clean version): [evaluated 2026-11-18: the (flags & 0x200) != 0 form changes the ROM] */
    if (t->cheats & 0x200) {
        D_800F3B81 = 1;
    } else {
        D_800F3B81 = 0;
    }
    gShowFps = 0;
    return 1;
}

s32 RaceView_Shutdown(void) {
    return 1;
}

void RaceView_BeginFrame(void) {
    svcScreenBeginFrame();
}

typedef struct { f32 x, y, z; } RvVec_;
typedef struct { u8 p0[0x774]; s32 timer; } RvCar_;
RvCar_ *Car_Get(u8 i);
extern u16 (*svcScreenGetWidth)(void);
extern u16 (*svcScreenGetHeight)(void);
extern void (*svcScreenSetViewport)(s16 x, s16 y, u16 w, u16 h);
extern void (*svcModelSetAlpha)(u8 id, s32 alpha);
extern void (*svcModelSetVisible)(u8 id, u8 on);
extern void (*svcCameraSetPerspective)(f32 near, f32 far, f32 fovy, f32 aspect);
extern s32 (*svcWorldDraw)(RvVec_ *pos, RvVec_ *a, RvVec_ *b, RvVec_ *c, u8 view);
extern void (*D_801416C0)(s16 x0, s16 y0, s16 x1, s16 y1); /* draws the split-screen divider */
void RaceCam_Get(u8 i, RvVec_ *pos, RvVec_ *a, RvVec_ *b, RvVec_ *c, f32 *fovy, u8 *inside, u8 *car);

/* Draws the world once per player view (split screen for two players), hiding the car whose
 * camera is inside it and fading cars whose timer ran out; returns the sum of svcWorldDraw. */
s32 RaceView_DrawScene(u32 dt) {
    u8 i;
    u8 j;
    u16 w;
    u16 h;
    u16 vh;
    s32 total;
    f32 near;
    f32 far;
    RvVec_ pos;
    RvVec_ a;
    RvVec_ b;
    RvVec_ c;
    f32 fovy;
    u8 inside;
    u8 camCar;
    RvCar_ *car;
    u8 hide;
    s32 alpha;
    s32 t;
    s32 b2;
    s32 mask;

    vh = 0;
    w = svcScreenGetWidth();
    h = svcScreenGetHeight();
    near = 10.0f;
    far = 4000.0f;
    total = 0;
    for (i = 0; i < (u32)gRaceViewCount; i++) {
        if ((u32)gRaceViewCount >= 2) {
            t = h / 2 + 1;
            b2 = h / 2 - 1;
            mask = i == 0;
            vh = b2;
            svcScreenSetViewport(0, -mask & t, w, vh);
        }
        RaceCam_Get(i, &pos, &a, &b, &c, &fovy, &inside, &camCar);
        for (j = 0; j < (u32)D_801415B8; j++) {
            car = Car_Get(j);
            hide = 0;
            if (j == camCar) {
                hide = inside != 0;
            }
            alpha = ((car->timer == 0 ? 0xFF : 0) & 0xFF) | 0x7F;
            if (D_800F3B81) {
                svcModelSetAlpha(j, 0x7F);
            } else {
                svcModelSetAlpha(j, alpha);
            }
            svcModelSetVisible(j, !hide);
        }
        if (gRaceViewCount == 1) {
            svcCameraSetPerspective(near, far, fovy, 1.3333334f);
        } else {
            svcCameraSetPerspective(near, far, fovy, 2.6666667f);
        }
        total += svcWorldDraw(&pos, &a, &b, &c, i);
    }
    svcScreenSetViewport(0, 0, w, h);
    if ((u32)gRaceViewCount >= 2) {
        D_801416C0(0, vh, w - 1, vh + 1);
    }
    return total;
}

void RaceView_EndFrame(void) {
    svcScreenEndFrame();
}

void RaceView_Draw(u32 dt) {
    char buf[0x100];
    f32 fps;
    u16 i;

    Cars_CollideObstacles();
    Debris_Draw(dt);
    svcScreenBeginFrame();
    D_801416AC();
    RaceView_DrawScene(dt);
    Hud_Draw();
    if (gShowFps) {
        fps = 0.0f;
        if (dt != 0) {
            fps = 1000.0f / (f32)dt;
        }
        sprintf(buf, D_80100AF0, (s32)(fps + 0.5f));
        /* TODO (Clean version / Optimization): [evaluated 2026-11-18: caching the length changes
           the ROM] Avoid calling strlen(buf) on each iteration; cache length in a local variable:
           u32 len = strlen(buf);
           for (i = 0; i < len; i++) {
               svcFontDrawChar(0, buf[i], i * 16 + 16, 16, 0);
           }
        */
        for (i = 0; i < strlen(buf); i++) {
            svcFontDrawChar(0, buf[i], i * 16 + 16, 16, 0);
        }
    }
    D_801416B0();
    svcScreenEndFrame();
}

s32 RaceView_GetPlayerCount(void) {
    return gRaceViewCount;
}
