#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { s32 *t; s16 a; s16 b; s32 c; s32 d; s32 e; s16 f; s16 g; } Seq9_;
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { u16 flags; u16 a; u16 b; u16 c; u8 d8; u8 d9; u8 dA; u8 dB; } E0Cc_;
typedef struct { u8 p0[0xC]; s16 *ids; u8 p10[3]; s8 n; } Tb9F_;
typedef struct { u8 p0[8]; Tb9F_ *t; u8 pC[0xA]; s16 cur; } Ob9F_;
typedef struct { s32 a; u8 p4[0x10]; } SbA00_;
typedef struct { u8 p0[0x18]; u8 n; u8 p19[3]; SbA00_ *sub; u8 b20; u8 p21[7]; s32 w28; } TrA00_;
typedef struct { char *buf; s16 len; u8 p6[0xB]; u8 b11; u8 p12[2]; } FldX_;
typedef struct { u8 p0[0x18]; u8 n; u8 p19[3]; FldX_ *f; } MenuX_;
void MenuFields_AllocBuffers(void);
s32 func_800C9590();
extern char D_80101080[];
extern s32 (*sysFileGetSize)(char *);
extern s32 (*sysFileLoad)(char *, void *);
extern u8 D_80138D38[];
void AnimTrack_SeekKey(u16, u16, s32);
void AnimTrack_GetPos(u16, V12_ *);
u8 RaceSfx_Play(V12_, u8, u8, s32);
s32 RaceSfx_Stop(u8);

s32 Memory_Alloc(s32);
s32 func_800C9280();
extern void *gMenuObjs;
s32 Memory_Free(s32);
extern s32 gTrackSfxCount;
extern s32 gTrackSfxDefs;
extern s32 gTrackSfxHandles;

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9280);

INCLUDE_ASM("asm/nonmatchings/unit_69E80", func_800C9590);

/* Allocates every menu text field that has no buffer yet and fills it with '_'. */
void MenuFields_AllocBuffers(void) {
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 33; i++) {
        for (j = 0; j < ((MenuX_ **)&gMenuObjs)[i]->n; j++) {
            if (((MenuX_ **)&gMenuObjs)[i]->f[j].buf == NULL) {
                ((MenuX_ **)&gMenuObjs)[i]->f[j].buf = (char *)Memory_Alloc(((MenuX_ **)&gMenuObjs)[i]->f[j].len);
                for (k = 0; k < ((MenuX_ **)&gMenuObjs)[i]->f[j].len; k++) {
                    ((MenuX_ **)&gMenuObjs)[i]->f[j].buf[k] = '_';
                }
                ((MenuX_ **)&gMenuObjs)[i]->f[j].b11 = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", MenuObjs_CopyTexts);

void MenuObjs_Alloc(void) {
    s32 i;
    u8 *t;

    func_800C9280();
    for (i = 0; i < 33; i++) {
        t = (&gMenuObjs)[i];
        if (t[0x20] != 0 && *(s32 *)(t + 0x28) == 0) {
            *(s32 *)((u8 *)(&gMenuObjs)[i] + 0x28) = Memory_Alloc(t[0x20] * 0x34);
        }
    }
    MenuFields_AllocBuffers();
}

void Menu_FreeAll(void) {
    s32 i;
    s32 j;

    func_800C9590();
    for (i = 0; i < 33; i++) {
        if (((TrA00_ **)&gMenuObjs)[i]->b20) {
            Memory_Free(((TrA00_ **)&gMenuObjs)[i]->w28);
            ((TrA00_ **)&gMenuObjs)[i]->w28 = 0;
        }
    }
    for (i = 0; i < 33; i++) {
        for (j = 0; j < ((TrA00_ **)&gMenuObjs)[i]->n; j++) {
            if (((TrA00_ **)&gMenuObjs)[i]->sub[j].a != 0) {
                Memory_Free(((TrA00_ **)&gMenuObjs)[i]->sub[j].a);
                ((TrA00_ **)&gMenuObjs)[i]->sub[j].a = 0;
            }
        }
    }
}

void TrackSfx_Init(s32 n, s32 b) {
    gTrackSfxCount = n;
    if (n != 0) {
        gTrackSfxDefs = b;
        gTrackSfxHandles = Memory_Alloc(n * 2);
        memset((void *)gTrackSfxHandles, 0, gTrackSfxCount * 2);
    }
}


void TrackSfx_Shutdown(void) {
    if (gTrackSfxCount != 0) {
        Memory_Free(gTrackSfxHandles);
        gTrackSfxHandles = 0;
        gTrackSfxDefs = 0;
        gTrackSfxCount = 0;
    }
}

void TrackSfx_Trigger(s32 i) {
    E0Cc_ *e = &((E0Cc_ *)gTrackSfxDefs)[i];
    u8 *s = (u8 *)gTrackSfxHandles + i * 2;
    V12_ v;
    u16 f = e->flags;

    if (f & 1) {
        return;
    }
    if (f & 2) {
        AnimTrack_SeekKey(e->a, e->c, i);
        AnimTrack_GetPos(e->a, &v);
        if (e->flags & 0x28) {
            RaceSfx_Play(v, e->d8, e->dA, 0);
        } else if (e->flags & 0x10) {
            s[0] = RaceSfx_Play(v, e->d8, e->dA, 1);
        }
        if (e->flags & 4) {
            AnimTrack_SeekKey(e->b, e->c, i);
            AnimTrack_GetPos(e->b, &v);
            if (e->flags & 0x28) {
                RaceSfx_Play(v, e->d9, e->dB, 0);
            } else if (e->flags & 0x10) {
                s[1] = RaceSfx_Play(v, e->d9, e->dB, 1);
            }
        }
    }
}

void TrackSfx_Stop(s32 i) {
    E0Cc_ *e = &((E0Cc_ *)gTrackSfxDefs)[i];
    u8 *s = (u8 *)gTrackSfxHandles + i * 2;

    if (e->flags & 2) {
        if (e->flags & 0x10) {
            if (s[0] != 0xFF) {
                RaceSfx_Stop(s[0]);
            }
            if (e->flags & 4) {
                if (s[1] != 0xFF) {
                    RaceSfx_Stop(s[1]);
                }
            }
        }
    }
}

void RaceSfx_SetPosition(u8, V12_);

void TrackSfx_UpdatePos(s32 i) {
    V12_ v;
    E0Cc_ *e = &((E0Cc_ *)gTrackSfxDefs)[i];
    u8 *s = &((u8 *)gTrackSfxHandles)[i * 2];

    if (e->flags & 2) {
        if (e->flags & 0x10) {
            if (s[0] != 0xFF) {
                AnimTrack_GetPos(e->a, &v);
                RaceSfx_SetPosition(s[0], v);
            }
            if ((e->flags & 4) && s[1] != 0xFF) {
                AnimTrack_GetPos(e->b, &v);
                RaceSfx_SetPosition(s[1], v);
            }
        }
    }
}

s32 Tuning_Load(void) {
    char buf[80];

    sprintf(buf, D_80101080);
    if (sysFileGetSize(buf) == 0x100) {
        return sysFileLoad(buf, D_80138D38) == 0x100;
    }
    return 0;
}

s32 Fsm_Reset(Seq9_ *s) {
    s->c = s->t[s->a];
    s->e = s->t[s->b];
    s->g = -1;
    s->f = 0;
    s->d = 0;
    return 0;
}


s32 Fsm_Trigger(Ob9F_ *o, s32 id) {
    s32 i;

    if (o->cur == -1) {
        for (i = 0; i < o->t->n; i++) {
            if (o->t->ids[i * 2] == id) {
                o->cur = id;
                break;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_69E80", Fsm_Update);
