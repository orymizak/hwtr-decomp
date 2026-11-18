#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
    /* TODO (Clean version): Replace temporary types TAE_ and E14t_ with unified types once layout is verified:
    typedef struct {
        s32 active;
        u8 pad[16];
    } TextAnimEntry;

    typedef struct {
        u8 pad0[0x18];
        u8 count;
        u8 pad1[3];
        s32 flags;
        u8 pad2[0x10];
        TextAnimEntry entries[];
    } TextAnimGroup;
    */
typedef struct { s32 w; u8 p[16]; } E14t_;
typedef struct { u8 pad[0x18]; u8 n; u8 p2[3]; s32 w1C; u8 p3[0x10]; E14t_ e[1]; } TAE_;
extern s32 (*D_801416AC)();
extern s32 (*D_801416B0)();
extern void (*svcFontDrawChar)(s32, u8, u16, s32, s32);
extern char D_80100AF0[];
void Cars_CollideObstacles(void);
void Debris_Draw(u32);
void RaceView_DrawScene(u32);
void Hud_Draw(void);
u32 strlen(const char *);
extern s32 D_801415B8;
extern u8 gShowFps;
extern u8 D_800F3B81;

extern s32 (*svcScreenBeginFrame)();
extern s32 (*svcScreenEndFrame)();
extern s32 gRaceViewCount;

s32 RaceView_Init(TAE_ *t) {
    u8 i;

    gRaceViewCount = 0;
    for (i = 0; i < t->n; i++) {
        if (t->e[i].w == 1) {
            gRaceViewCount++;
        }
    }
    if (gRaceViewCount == 0) {
        gRaceViewCount = 1;
    }
    D_801415B8 = t->n;
    /* TODO (Clean version): Simplify flag assignment:
       D_800F3B81 = (t->w1C & 0x200) != 0;
    */
    if (t->w1C & 0x200) {
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

INCLUDE_ASM("asm/nonmatchings/unit_4F640", RaceView_DrawScene);

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
        /* TODO (Clean version / Optimization): Avoid calling strlen(buf) on each iteration; cache length in a local variable:
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
