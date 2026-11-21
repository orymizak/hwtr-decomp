#include "common.h"
#include "math.h"
#include "include_asm.h"
/* Text element: string id and an uppercase copy of the string (10 bytes per character). */
typedef struct { u16 id; u8 len; u8 p3; u8 *buf; u8 pad[0x10]; } E18c_;
typedef struct { u8 pad[8]; u8 n1; u8 p1[3]; E18c_ *a1; u8 n2; u8 p2[3]; E18c_ *a2; } TCB_;
/* Pause menu text: one positioned glyph per character (current and home position). */
typedef struct { u8 a; u8 pad; u16 x; u16 y; u16 x2; u16 y2; } TextGlyphPos;
typedef struct { s16 x; s16 y; } Point2s16;
typedef struct { u8 a; u8 pad; Point2s16 p; Point2s16 p2; } TextAnimItem;
/* mode: 0 = at home, 1 = slide in from the right, 2 = scattered. */
typedef struct { u8 p0[2]; u8 n; u8 p3; TextAnimItem *items; u8 p8[0xB]; u8 mode; } TextAnim;
void PauseUi_PlaceItem2(TextAnim *t);
extern s32 (*svcRandRange)(s32);
void PauseUi_PlaceItem(TextAnim *t);
extern void (*svcWorldUpdate)(s32);
extern void (*svcScreenBeginFrame)(void);
extern void (*D_801416AC)(void);
extern void (*svcCameraSetPerspective)(f32, f32, f32, f32);
extern void (*svcDraw2DBeginPrim)(s32, s32, s32, s32);
void RaceView_DrawScene(s32);
void PauseUi_CenterItem2(u8 *t);
extern s32 (*svcFontGetCharWidth)(s32, u8);
s8 Font_GetKerning(u8 font, u8 a, u8 b);
void PauseUi_CenterItem(u8 *t);
void PauseUi_BuildText(E18c_ *);
void PauseUi_BuildText2(E18c_ *);

extern u32 gPauseTextUsed;
extern u8 gPauseFadeTarget;
extern s8 gPauseFadeAlpha;
extern s32 (*svcScreenEndFrame)();
extern s32 (*D_801416B0)();

    /* TODO (Bug check / Clean version): [evaluated 2026-11-18: point 3 tried as a wrapper over
    an inline PauseUi_CenterItem: changes the ROM; the guard of point 1 too]
    1. If character count t[2] is 0, loop conditions and indexing into items[0] cause out-of-bounds access. Add guard:
        if (t[2] == 0) return;
    2. Horizontal text centering: centers text string between min_x (*(s16*)(t+8)) and max_x (*(s16*)(t+0xA)).
    3. PauseUi_CenterItem2 below is an identical duplicate of this function.
    */
void PauseUi_CenterItem(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    if (*(s16 *)(t + 0xA) > 0x140) {
        *(s16 *)(t + 0xA) = 0x140;
    }
    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += svcFontGetCharWidth(0, (*(TextGlyphPos **)(t + 4))[i].a) + Font_GetKerning(0, (*(TextGlyphPos **)(t + 4))[i].a, (*(TextGlyphPos **)(t + 4))[i + 1].a);
    }
    w += svcFontGetCharWidth(0, (*(TextGlyphPos **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(TextGlyphPos **)(t + 4))[0].x2 = x;
    (*(TextGlyphPos **)(t + 4))[0].y2 = y;
    for (i = 1; i < t[2]; i++) {
        x += svcFontGetCharWidth(0, (*(TextGlyphPos **)(t + 4))[i - 1].a) + Font_GetKerning(0, (*(TextGlyphPos **)(t + 4))[i - 1].a, (*(TextGlyphPos **)(t + 4))[i].a);
        (*(TextGlyphPos **)(t + 4))[i].x2 = x;
        (*(TextGlyphPos **)(t + 4))[i].y2 = y;
    }
}

void PauseUi_CenterItem2(u8 *t) {
    u8 i;
    s32 w;
    s32 x;
    s32 y;

    if (*(s16 *)(t + 0xA) > 0x140) {
        *(s16 *)(t + 0xA) = 0x140;
    }
    i = 0;
    w = 0;
    x = (*(s16 *)(t + 8) + *(s16 *)(t + 0xA)) / 2;
    y = *(s16 *)(t + 0xC);
    for (; i < t[2] - 1; i++) {
        w += svcFontGetCharWidth(0, (*(TextGlyphPos **)(t + 4))[i].a) + Font_GetKerning(0, (*(TextGlyphPos **)(t + 4))[i].a, (*(TextGlyphPos **)(t + 4))[i + 1].a);
    }
    w += svcFontGetCharWidth(0, (*(TextGlyphPos **)(t + 4))[i].a);
    w /= 2;
    x -= w;
    (*(TextGlyphPos **)(t + 4))[0].x2 = x;
    (*(TextGlyphPos **)(t + 4))[0].y2 = y;
    for (i = 1; i < t[2]; i++) {
        x += svcFontGetCharWidth(0, (*(TextGlyphPos **)(t + 4))[i - 1].a) + Font_GetKerning(0, (*(TextGlyphPos **)(t + 4))[i - 1].a, (*(TextGlyphPos **)(t + 4))[i].a);
        (*(TextGlyphPos **)(t + 4))[i].x2 = x;
        (*(TextGlyphPos **)(t + 4))[i].y2 = y;
    }
}

void PauseUi_PlaceItem(TextAnim *t) {
    u8 i;

    for (i = 0; i < t->n; i++) {
        switch (t->mode) {
        case 0:
            t->items[i].p = t->items[i].p2;
            break;
        case 1:
            t->items[i].p.x = t->items[i].p2.x + 320;
            t->items[i].p.y = t->items[i].p2.y;
            break;
        case 2:
            ((u16 *)&t->items[i].p)[0] = svcRandRange(320);
            ((u16 *)&t->items[i].p)[1] = svcRandRange(240);
            break;
        }
    }
}

    /* TODO (Clean version): [evaluated 2026-11-18: a wrapper over an inline PauseUi_PlaceItem
    changes the ROM; the copy stays] PauseUi_PlaceItem2 is an exact duplicate of PauseUi_PlaceItem. Both animate item positions according to mode:
    mode 0: reset to p2
    mode 1: slide in from right (p2.x + 320)
    mode 2: scatter randomly across screen (rand(320), rand(240))
    */
void PauseUi_PlaceItem2(TextAnim *t) {
    u8 i;

    for (i = 0; i < t->n; i++) {
        switch (t->mode) {
        case 0:
            t->items[i].p = t->items[i].p2;
            break;
        case 1:
            t->items[i].p.x = t->items[i].p2.x + 320;
            t->items[i].p.y = t->items[i].p2.y;
            break;
        case 2:
            ((u16 *)&t->items[i].p)[0] = svcRandRange(320);
            ((u16 *)&t->items[i].p)[1] = svcRandRange(240);
            break;
        }
    }
}

void PauseUi_Open(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        PauseUi_CenterItem(&t->a1[i]);
        PauseUi_PlaceItem(&t->a1[i]);
    }
    for (i = 0; i < t->n2; i++) {
        PauseUi_CenterItem2(&t->a2[i]);
        PauseUi_PlaceItem2(&t->a2[i]);
    }
}


void PauseUi_StepItem(TextAnim *t, f32 step) {
    u8 i;
    Point2s16 p[2], p_dummy;
    f32 fx;
    f32 fy;
    s32 dx;
    s32 dy;
    s32 near;

    for (i = 0; i < t->n; i++) {
        if (t) {
        }
        dx = t->items[i].p2.x - t->items[i].p.x;
        dy = t->items[i].p2.y - t->items[i].p.y;
        fx = ((f32)dx) * step;
        p[0] = t->items[i].p;
        fy = ((f32)dy) * step;
        p[1] = t->items[i].p2;
        dx = (s32)fx;
        dy = (s32)fy;
        near = 10;
        near = (((p[0].x < (p[1].x + 10)) && (p[1].x < (p[0].x + 10))) && (p[0].y < (p[1].y + near))) && (p[1].y < (p[0].y + 10));
        if (near) {
            t->items[i].p = t->items[i].p2;
        } else {
            t->items[i].p.x += dx;
            t->items[i].p.y += dy;
        }
    }
}

void PauseUi_StepItem2(TextAnim *t, f32 step) {
    u8 i;
    Point2s16 p[2], p_dummy;
    f32 fx;
    f32 fy;
    s32 dx;
    s32 dy;
    s32 near;

    for (i = 0; i < t->n; i++) {
        if (t) {
        }
        dx = t->items[i].p2.x - t->items[i].p.x;
        dy = t->items[i].p2.y - t->items[i].p.y;
        fx = ((f32)dx) * step;
        p[0] = t->items[i].p;
        fy = ((f32)dy) * step;
        p[1] = t->items[i].p2;
        dx = (s32)fx;
        dy = (s32)fy;
        near = 10;
        near = (((p[0].x < (p[1].x + 10)) && (p[1].x < (p[0].x + 10))) && (p[0].y < (p[1].y + near))) && (p[1].y < (p[0].y + 10));
        if (near) {
            t->items[i].p = t->items[i].p2;
        } else {
            t->items[i].p.x += dx;
            t->items[i].p.y += dy;
        }
    }
}

extern u32 gPauseUiTime;
extern u32 (*svcTimeGetMillis)(void);

/* Steps both element lists by elapsed ticks / 100 (0.15 on the first call, 0.1 after a gap of more than 100 ticks). */
void PauseUi_Animate(TCB_ *t) {
    f32 dt;
    u32 now;
    u32 d;
    u8 i;

    if (gPauseUiTime == 0) {
        dt = 0.15f;
        gPauseUiTime = svcTimeGetMillis();
    } else {
        now = svcTimeGetMillis();
        d = now - gPauseUiTime;
        if (d > 100) {
            dt = 0.1f;
        } else {
            dt = d / 100.0f;
        }
        gPauseUiTime = now;
    }
    for (i = 0; i < t->n1; i++) {
        PauseUi_StepItem(&t->a1[i], dt);
    }
    for (i = 0; i < t->n2; i++) {
        PauseUi_StepItem2(&t->a2[i], dt);
    }
}

void PauseUi_ResetTextBuffer(void) {
    gPauseTextUsed = 0;
}

extern char *gLangText[];
extern u8 gPauseTextBuf[];
extern u8 D_800FCA20[]; /* ctype table; bit 2 = lowercase */
u32 strlen(const char *);

/* Takes size bytes from the 0x800-byte text buffer; NULL when it runs out. */
static inline u8 *Buf_Alloc(u32 size) {
    u8 *p = gPauseTextBuf + gPauseTextUsed;

    gPauseTextUsed += size;
    return (gPauseTextUsed < 0x800) ? p : NULL;
}
/* Builds the uppercase copy of the element's string the first time it is needed. */
void PauseUi_BuildText(E18c_ *t) {
    s32 i;
    u8 c;

    if (t->buf == NULL) {
        t->len = strlen(gLangText[t->id]);
        t->buf = Buf_Alloc(t->len * 10);
        for (i = 0; i < t->len; i++) {
            c = gLangText[t->id][i];
            if (D_800FCA20[c] & 4) {
                t->buf[i * 10] = c - 0x20;
            } else {
                t->buf[i * 10] = c;
            }
        }
    }
}

/* Same as PauseUi_BuildText, for the second list. */
void PauseUi_BuildText2(E18c_ *t) {
    s32 i;
    u8 c;

    if (t->buf == NULL) {
        t->len = strlen(gLangText[t->id]);
        t->buf = Buf_Alloc(t->len * 10);
        for (i = 0; i < t->len; i++) {
            c = gLangText[t->id][i];
            if (D_800FCA20[c] & 4) {
                t->buf[i * 10] = c - 0x20;
            } else {
                t->buf[i * 10] = c;
            }
        }
    }
}

void PauseUi_Load(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        PauseUi_BuildText(&t->a1[i]);
    }
    for (i = 0; i < t->n2; i++) {
        PauseUi_BuildText2(&t->a2[i]);
    }
}


void PauseUi_Unload(TCB_ *t) {
    u8 i;

    for (i = 0; i < t->n1; i++) {
        t->a1[i].buf = NULL;
    }
    for (i = 0; i < t->n2; i++) {
        t->a2[i].buf = NULL;
    }
}

/* Draws the race behind the pause menu and steps the black overlay alpha toward
 * gPauseFadeTarget (+15 per frame when darkening, -1 when clearing). */
void PauseUi_BeginFrame(void) {
    u8 a;
    u8 b;

    svcWorldUpdate(0);
    svcScreenBeginFrame();
    D_801416AC();
    RaceView_DrawScene(0);
    svcCameraSetPerspective(CAM_NEAR, CAM_FAR, CAM_FOV_60DEG, CAM_ASPECT_4_3);
    a = gPauseFadeAlpha;
    b = gPauseFadeTarget;
    if (a > b) {
        gPauseFadeAlpha = a - 1;
    } else if (a < b) {
        gPauseFadeAlpha = a + 15;
        if (b < (u8)gPauseFadeAlpha) {
            gPauseFadeAlpha = b;
        }
    }
    svcDraw2DBeginPrim(0, 0, 0, (u8)gPauseFadeAlpha);
}

void PauseUi_FadeIn(void) {
    gPauseFadeTarget = 0x80;
    gPauseFadeAlpha = 0;
}

void PauseUi_FadeOut(void) {
    gPauseFadeTarget = 0;
}

void PauseUi_EndFrame(void) {
    D_801416B0();
    svcScreenEndFrame();
}

typedef struct { u8 pad0[2]; u8 n; u8 pad3; TextGlyphPos *glyphs; u8 pad8[0x10]; } PauseMenuLine;
typedef struct { u8 pad[8]; u8 nTitle; u8 p9[3]; PauseMenuLine *title; u8 nItems; u8 p11[3]; PauseMenuLine *items; } PauseMenu;
extern s32 (*svcFontDrawChar)(s32, u8, u16, u16, s32);
extern void (*svcDraw2DSetColor)(s32, s32, s32);

static inline void PauseUi_DrawLine(PauseMenuLine *l, u8 selected) {
    u8 j;

    if (selected) {
        svcDraw2DSetColor(0xFF, 0, 0);
    }
    for (j = 0; j < l->n; j++) {
        svcFontDrawChar(0, l->glyphs[j].a, l->glyphs[j].x, l->glyphs[j].y, 0);
    }
    if (selected) {
        svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    }
}

void PauseUi_DrawMenu(PauseMenu *m, u8 sel) {
    u8 i;
    u8 j;

    svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    for (i = 0; i < m->nTitle; i++) {
        /* TODO(match-debt): &m->title[i] adds base + offset in the other order. */
        PauseMenuLine *l = (PauseMenuLine *)(i * sizeof(PauseMenuLine) + (s32)m->title);

        svcDraw2DSetColor(0, 0, 0xFF);
        for (j = 0; j < l->n; j++) {
            svcFontDrawChar(0, l->glyphs[j].a, l->glyphs[j].x, l->glyphs[j].y, 0);
        }
        svcDraw2DSetColor(0xFF, 0xFF, 0xFF);
    }
    for (i = 0; i < m->nItems; i++) {
        PauseUi_DrawLine(&m->items[i], sel == i);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_6AE90", PauseUi_DrawText);
