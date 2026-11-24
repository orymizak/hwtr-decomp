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
s32 MenuTexts_Free();
extern char D_80101080[];
extern s32 (*sysFileGetSize)(char *);
extern s32 (*sysFileLoad)(char *, void *);
extern u8 D_80138D38[];
void AnimTrack_SeekKey(u16, u16, s32);
void AnimTrack_GetPos(u16, V12_ *);
u8 RaceSfx_Play(V12_, u8, u8, s32);
s32 RaceSfx_Stop(u8);

s32 Memory_Alloc(s32);
s32 MenuTexts_Alloc();
extern void *gMenuObjs;
s32 Memory_Free(s32);
extern s32 gTrackSfxCount;
extern s32 gTrackSfxDefs;
extern s32 gTrackSfxHandles;

typedef struct { u8 c; u8 p1[9]; } MnChar_;
typedef struct { char str[0x18]; u8 nChars; u8 p19[3]; MnChar_ *chars; u8 p20[0xC]; } MnTextC_;
u32 strlen(const char *);
typedef struct { u16 id; u8 len; u8 p3; u8 *buf; u8 p8[0x10]; } MnSpriteA_;
typedef struct { u8 nText; u8 p1[3]; MnTextC_ *text; u8 nSprites; u8 p9[3]; MnSpriteA_ *sprites; u8 nButtons; u8 p11[3]; MnSpriteA_ *buttons; } MenuA_;
extern char *gLangText[];

s32 MenuTexts_Alloc(void) {
    s32 i;
    s32 j;
    s32 k;
    MnTextC_ *t;
    MnSpriteA_ *s;

    /* TODO(match-debt): integer address math for the element pointers, as in MenuObjs_CopyTexts. */
    for (i = 0; i < 33; i++) {
        for (j = 0; j < ((MenuA_ **)&gMenuObjs)[i]->nText; j++) {
            {
                MnTextC_ *n = (MnTextC_ *)(j * sizeof(MnTextC_) + (s32)((MenuA_ **)&gMenuObjs)[i]->text);

                n->chars = (MnChar_ *)Memory_Alloc(0xF0);
            }
            t = (MnTextC_ *)(j * sizeof(MnTextC_) + (s32)((MenuA_ **)&gMenuObjs)[i]->text);
            t->nChars = strlen(t->str);
            if (t->nChars != 0) {
                if (t->nChars > 24) {
                    t->nChars = 24;
                }
                for (k = 0; k < t->nChars; k++) {
                    t->chars[k].c = t->str[k];
                }
            }
        }
        for (j = 0; j < ((MenuA_ **)&gMenuObjs)[i]->nSprites; j++) {
            s = (MnSpriteA_ *)(j * sizeof(MnSpriteA_) + (s32)((MenuA_ **)&gMenuObjs)[i]->sprites);
            if (s->buf == NULL) {
                s->len = strlen(gLangText[s->id]);
                if (s->len != 0) {
                    s->buf = (u8 *)Memory_Alloc(s->len * 10);
                    for (k = 0; k < s->len; k++) {
                        s->buf[k * 10] = gLangText[s->id][k];
                    }
                }
            }
        }
        for (j = 0; j < ((MenuA_ **)&gMenuObjs)[i]->nButtons; j++) {
            s = (MnSpriteA_ *)(j * sizeof(MnSpriteA_) + (s32)((MenuA_ **)&gMenuObjs)[i]->buttons);
            if (s->buf == NULL) {
                s->len = strlen(gLangText[s->id]);
                s->buf = (u8 *)Memory_Alloc(s->len * 10);
                for (k = 0; k < s->len; k++) {
                    s->buf[k * 10] = gLangText[s->id][k];
                }
            }
        }
    }
}

typedef struct { u8 p0[0x1C]; s32 chars; u8 p20[0xC]; } MnTextF_;
typedef struct { u8 p0[4]; s32 buf; u8 p8[0x10]; } MnSpriteF_;
typedef struct { u8 nText; u8 p1[3]; MnTextF_ *text; u8 nSprites; u8 p9[3]; MnSpriteF_ *sprites; u8 nButtons; u8 p11[3]; MnSpriteF_ *buttons; } MenuF_;

s32 MenuTexts_Free(void) {
    s32 i;
    s32 j;

    /* TODO(match-debt): the element pointers use integer address math (offset first), as in
     * MenuObjs_CopyTexts; &array[j] gives the other addu operand order. */
    for (i = 0; i < 33; i++) {
        for (j = 0; j < ((MenuF_ **)&gMenuObjs)[i]->nText; j++) {
            MnTextF_ *t = (MnTextF_ *)(j * sizeof(MnTextF_) + (s32)((MenuF_ **)&gMenuObjs)[i]->text);

            if (t->chars != 0) {
                Memory_Free(t->chars);
                t->chars = 0;
            }
        }
        for (j = 0; j < ((MenuF_ **)&gMenuObjs)[i]->nSprites; j++) {
            MnSpriteF_ *s = (MnSpriteF_ *)(j * sizeof(MnSpriteF_) + (s32)((MenuF_ **)&gMenuObjs)[i]->sprites);

            if (s->buf != 0) {
                Memory_Free(s->buf);
                s->buf = 0;
            }
        }
        for (j = 0; j < ((MenuF_ **)&gMenuObjs)[i]->nButtons; j++) {
            MnSpriteF_ *s = (MnSpriteF_ *)(j * sizeof(MnSpriteF_) + (s32)((MenuF_ **)&gMenuObjs)[i]->buttons);

            if (s->buf != 0) {
                Memory_Free(s->buf);
                s->buf = 0;
            }
        }
    }
}

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

typedef struct { u8 nText; u8 p1[3]; MnTextC_ *text; } MenuC_;

void MenuObjs_CopyTexts(void) {
    s32 i;
    s32 j;
    s32 k;
    MnTextC_ *t;

    for (i = 0; i < 33; i++) {
        for (j = 0; j < ((MenuC_ **)&gMenuObjs)[i]->nText; j++) {
            /* TODO(match-debt): integer address math (offset first) instead of &text[j];
             * only this operand order matches the original addu. */
            t = (MnTextC_ *)(j * sizeof(MnTextC_) + (s32)((MenuC_ **)&gMenuObjs)[i]->text);
            t->nChars = strlen(t->str);
            if (t->nChars != 0) {
                if (t->nChars > 24) {
                    t->nChars = 24;
                }
                for (k = 0; k < t->nChars; k++) {
                    t->chars[k].c = t->str[k];
                }
            }
        }
    }
}

void MenuObjs_Alloc(void) {
    s32 i;
    u8 *t;

    MenuTexts_Alloc();
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

    MenuTexts_Free();
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

typedef struct { s16 id; s16 target; } FsmTrans_;
typedef struct FsmState_ {
    void (**enter)(void);
    void (**update)(void);
    void (**exit)(void);
    FsmTrans_ *trans;
    s8 nEnter;
    s8 nUpdate;
    s8 nExit;
    s8 nTrans;
} FsmState_;
typedef struct {
    FsmState_ **states;
    s16 a;
    s16 b;
    FsmState_ *cur;
    FsmState_ *next;
    FsmState_ *end;
    s16 phase;
    s16 trigger;
} Fsm_;

s32 Fsm_Update(Fsm_ *f) {
    s32 i;

    if (f->phase == 3) {
        return 2;
    }
    switch (f->phase) {
    case 0:
        for (i = 0; i < f->cur->nEnter; i++) {
            f->cur->enter[i]();
        }
        f->phase = 1;
        break;
    case 1:
        for (i = 0; i < f->cur->nUpdate; i++) {
            f->cur->update[i]();
        }
        if (f->cur == f->end) {
            f->phase = 2;
            break;
        }
        if (f->trigger != -1) {
            for (i = 0; i < f->cur->nTrans; i++) {
                if (f->trigger == f->cur->trans[i].id) {
                    f->next = f->states[f->cur->trans[i].target];
                    f->phase = 2;
                    f->trigger = -1;
                    if (f->cur->nExit == 0) {
                        if (f->cur != f->end) {
                            f->cur = f->next;
                            if (f->cur->nEnter > 0) {
                                f->phase = 0;
                            } else {
                                f->phase = 1;
                            }
                        } else {
                            f->phase = 3;
                        }
                    }
                }
            }
        }
        break;
    case 2:
        for (i = 0; i < f->cur->nExit; i++) {
            f->cur->exit[i]();
        }
        if (f->cur != f->end) {
            f->cur = f->next;
            if (f->cur->nEnter > 0) {
                f->phase = 0;
            } else {
                f->phase = 1;
            }
        } else {
            f->phase = 3;
        }
        break;
    }
    return (f->phase == 3) * 2;
}
