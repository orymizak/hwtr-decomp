#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
/* Voice groups for Snd_FadeOutAll (bit mask). */
#define SND_GROUP_SFX 1 /* voices playing a bank sound effect (unk78 != NULL) */
#define SND_GROUP_SEQ 2 /* voices owned by the music sequencer */
#define SND_MUSIC_VOICES 4 /* voices 0-3 are reserved for music */

/* Sound bank (sfx) loaded with Snd_LoadSfxBank. */
typedef struct { u8 p0[8]; s32 unk8; u8 pC[4]; s32 unk10; s32 unk14; struct { s32 a; s32 b; } entries[1]; } SndBank;
/* Sequence (song) played by a music voice. */
typedef struct { u8 p0[8]; s32 unk8; u8 pC[0xC]; u8 *unk18; u8 *unk1C; s32 unk20; } SndSeq;
/* Driver voice (0x13C): voices 0-3 play music sequences (unk74), the rest sound effects
   from a bank (unk78). unk44 is the handle returned to the game. */
typedef struct {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;  /* also seen as u32 */
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8* unk34;  /* pitch curve stream */
    /* 0x38 */ u8* unk38;  /* also seen as s32 */
    /* 0x3C */ u32 unk3C;  /* also seen as s32 */
    /* 0x40 */ u32 unk40;  /* also seen as s32 */
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ u32 unk54;  /* also seen as s32 */
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ u32 unk64;  /* also seen as s32 */
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ SndSeq* unk74;  /* also seen as SndSeq*, s32 */
    /* 0x78 */ SndBank* unk78;  /* also seen as s32, void* */
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8* unk84;  /* also seen as s32 */
    /* 0x88 */ u8* unk88;  /* pitch curve base */
    /* 0x8C */ u8* unk8C;  /* volume curve base */
    /* 0x90 */ f32 unk90;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u16 unk9A;
    /* 0x9C */ u16 unk9C;  /* also seen as s16 */
    /* 0x9E */ s16 unk9E;
    /* 0xA0 */ u16 unkA0;
    /* 0xA2 */ u16 unkA2;
    /* 0xA4 */ u16 unkA4;
    /* 0xA6 */ s16 unkA6;
    /* 0xA8 */ u16 unkA8;
    /* 0xAA */ u16 unkAA;
    /* 0xAC */ u16 unkAC;  /* also seen as s16 */
    /* 0xAE */ u16 unkAE;
    /* 0xB0 */ s16 unkB0;
    /* 0xB2 */ u16 unkB2;  /* also seen as s16 */
    /* 0xB4 */ u16 unkB4;  /* also seen as s16 */
    /* 0xB6 */ u8 unkB6;
    /* 0xB7 */ u8 unkB7;
    /* 0xB8 */ u8 unkB8;
    /* 0xB9 */ u8 unkB9;
    /* 0xBA */ u8 unkBA;
    /* 0xBB */ u8 unkBB;
    /* 0xBC */ u8 unkBC;
    /* 0xBD */ u8 unkBD;
    /* 0xBE */ u8 unkBE;
    /* 0xBF */ u8 unkBF;
    /* 0xC0 */ u8 unkC0;
    /* 0xC1 */ u8 unkC1;
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 unkC3;
    /* 0xC4 */ u8 unkC4;
    /* 0xC5 */ u8 unkC5;
    /* 0xC6 */ u8 unkC6;
    /* 0xC7 */ u8 unkC7;
    /* 0xC8 */ u8 unkC8;
    /* 0xC9 */ u8 unkC9;
    /* 0xCA */ u8 unkCA;
    /* 0xCB */ u8 unkCB;
    /* 0xCC */ u8 unkCC;
    /* 0xCD */ u8 unkCD;
    /* 0xCE */ u8 unkCE;
    /* 0xCF */ u8 unkCF;
    /* 0xD0 */ u8 unkD0;
    /* 0xD1 */ s8 unkD1;
    /* 0xD2 */ u8 unkD2;
    /* 0xD3 */ u8 unkD3;
    /* 0xD4 */ u8 unkD4;
    /* 0xD5 */ u8 unkD5;
    /* 0xD6 */ u8 unkD6;
    /* 0xD7 */ u8 unkD7;
    /* 0xD8 */ u8 unkD8;
    /* 0xD9 */ u8 unkD9;
    /* 0xDA */ u8 unkDA;
    /* 0xDB */ u8 unkDB;
    /* 0xDC */ f32 unkDC;
    /* 0xE0 */ u8* unkE0[4];
    /* 0xF0 */ s32 unkF0[4];
    /* 0x100 */ s32 unk100[4];
    /* 0x110 */ u16 unk110[4];
    /* 0x118 */ u16 unk118[4];
    /* 0x120 */ u8 unk120[4];
    /* 0x124 */ u8 unk124[4];
    /* 0x128 */ f32 unk128[4];
    /* 0x138 */ u8 p138[0x4];
} SndVoice; /* size = 0x13C */

    /* Sequence player handle (fields still unidentified). */
typedef struct { u8 p0[4]; s32 unk4; u8 p8[8]; s32 unk10; } SeqPl6_;
/* Sequence channel as seen by the command handlers (fields still unidentified). */
typedef struct { u8 p0[0x7C]; s32 unk7C; } SeqY2_;
extern s32 gSndFrameRate;
extern SndVoice *gSndSfxVoices;
extern s32 gSndNextHandle;
void SndVoice_Reset(SndVoice *v);
typedef struct { u8 b[8]; } AudMsgE_;
void Snd_MemMove(void *dst, void *src, s32 n);
s32 SndMsg_Post(AudMsgE_ *msg);
extern s32 gSndNumVoices;
typedef struct { s32 unk0; s32 unk4; } Entry_;
extern s32 gSndFxPreset;
void Snd_Resume(s32 arg);
typedef struct { u8 type; u8 pad[3]; s32 arg; } AudMsg_;
extern s32 gSndFxPresetCount;
extern u8 gSndFxPresets;
s32 SndVoice_PlaySfx(SndBank *b, s32 idx, s32 a2, s32 a3, s32 prio);
typedef struct { s32 a; s32 b; } Ent89_;
typedef struct { s32 n; u8 p4[8]; s32 flags; s32 unk10; s32 unk14; Ent89_ e[1]; } Bank89_;
void SndFx_Alloc();
extern void *gSndVoices;
extern u8 D_800FACF8;
extern s32 D_800FB170;
extern void *n_syn;
/* Local mirror of the n_audio synthesizer types (n_audio.h), used by the game's own copy of
   n_alSynNew. Sizes: ALParam 0x1C, N_PVoice 0x8C, N_ALAuxBus 0x44, N_ALMainBus 0x14. */
typedef struct SndParam { struct SndParam *next; u8 p4[0x18]; } SndParam;
typedef struct { u8 node[8]; s32 vvoice; u8 pC[0x80]; } SndPVoice;
typedef struct { u8 p0[0x14]; s32 sourceCount; s32 maxSources; SndPVoice **sources; s32 fx; s32 unk24[8]; } SndAuxBus;
typedef struct { u8 p0[4]; void *pull; } SndMainBus;
typedef struct { s32 next; s32 prev; } SndLink;
typedef struct {
    /* 0x00 */ s32 head;
    /* 0x04 */ SndLink pFreeList;
    /* 0x0C */ SndLink pAllocList;
    /* 0x14 */ SndLink pLameList;
    /* 0x1C */ s32 paramSamples;
    /* 0x20 */ s32 curSamples;
    /* 0x24 */ s32 dma;
    /* 0x28 */ void *heap;
    /* 0x2C */ SndParam *paramList;
    /* 0x30 */ SndMainBus *mainBus;
    /* 0x34 */ SndAuxBus *auxBus;
    /* 0x38 */ s32 numPVoices;
    /* 0x3C */ u8 p3C[4];
    /* 0x40 */ s32 outputRate;
    /* 0x44 */ s32 maxOutSamples;
    /* 0x48 */ s32 outBuf;
    /* 0x4C */ s32 maxAuxBusses;
} SndSynth; /* N_ALSynth */
/* ALSynConfig */
typedef struct { u8 p0[4]; s32 maxPVoices; s32 maxUpdates; u8 pC[4]; s32 dmaproc; void *heap; s32 outputRate; u8 fxType; u8 p1D[3]; void *params; } SndSynConfig;
void SndSyn_New(SndSynConfig *c);
extern s32 D_8014C6D0;
extern s32 gSndSeqFxControl;
void SndWave_Relocate();
extern s32 gSndDefWaveBank;

extern s16 D_8012794C;
extern s16 D_8012794E;
s32 SndSong_Start();
extern void *gSndCurWaveBank;
extern s32 gSndSfxBank;
extern s32 gSndSfxBankOnce;
extern s32 gSndTaskHooks;
extern s32 gSndMarkerCallback;
s32 SndHeap_Alloc(s32);
extern s32 gSndMsgRead;
extern s32 gSndMsgWrite;
extern s32 gSndMsgCount;
extern s32 gSndMsgQueue;
s32 n_alSynStartVoice(s32, s32);
s32 n_alSynStopVoice(s32);
extern s32 gSndPVoices;
extern s32 gSndTickUsec;
extern s32 gSndRandSeed;
extern f64 D_801016A0;

/* Sets ADSR envelope preset parameters */
INCLUDE_ASM("asm/nonmatchings/unit_6E610", SndCmd_SetEnvelopePreset);

typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; s32 unk10; s32 unk14; s32 unk18; s32 unk1C; s32 unk20; s32 unk24; u8 p28[0x18]; s32 unk40; } AudCfgB_;
typedef struct { u16 a; u16 b; u8 c; } AudMsgB_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; } Seq12_;
extern s32 osTvType;
extern s32 D_80138144;
extern s32 D_80145564;
extern s32 D_80127920;
extern s32 *D_80127924;
extern void *D_80127928;
void SndHeap_Init(s32, s32);
void Snd_SetScheduler(s32);
void SndMsg_InitQueue(s32 arg0);
void Snd_LoadWaveBank(s32 arg0);
void Snd_LoadSfxBank(u8 *base);
void Snd_CreateManager(void *, s32, s32);
void Snd_SetMasterVolume(s32 arg0, s16 arg1);
void n_alSynAddPlayer(void *);
void n_alSynAllocVoice(s32, void *);
void SndHeap_GetUsed(void);
s32 Snd_Update(void);

/* Starts the sound driver: voices (4 music + c->unk4 sfx), message queue, banks and the
 * synthesizer player; ticks at the TV frame rate. */
void Snd_Init(AudCfgB_ *c) {
    s32 i;
    AudMsgB_ msg;
    s32 *q;

    D_80138144 = c->unk40;
    D_80145564 = c->unk0;
    gSndNumVoices = c->unk4 + SND_MUSIC_VOICES;
    if (osTvType == OS_TV_PAL) {
        gSndFrameRate = 50;
    } else {
        gSndFrameRate = 60;
    }
    gSndTickUsec = 1000000 / gSndFrameRate; /* microseconds per audio tick */
    SndHeap_Init(c->unk10, c->unk14);
    Snd_SetScheduler(c->unk8);
    gSndPVoices = SndHeap_Alloc((gSndNumVoices - SND_MUSIC_VOICES) * 0x1C);
    gSndVoices = (void *)SndHeap_Alloc(gSndNumVoices * sizeof(SndVoice));
    gSndSfxVoices = (SndVoice *)&((SndVoice *)gSndVoices)[SND_MUSIC_VOICES];
    SndMsg_InitQueue(c->unk24);
    gSndCurWaveBank = 0;
    gSndDefWaveBank = 0;
    if (c->unk18 != 0 && c->unk1C != 0) {
        ((void (*)(s32, s32))Snd_LoadWaveBank)(c->unk18, c->unk1C);
    }
    gSndSfxBankOnce = 0;
    gSndSfxBank = 0;
    if (c->unk20 != 0) {
        Snd_LoadSfxBank((u8 *)c->unk20);
    }
    gSndMarkerCallback = 0;
    gSndFxPreset = 2;
    Snd_CreateManager(c, gSndFrameRate, 2);
    Snd_SetMasterVolume(3, 0x7FFF);
    gSndNextHandle = 1;
    gSndRandSeed = 0x12345678;
    q = &D_80127920;
    *q = 0;
    D_80127928 = Snd_Update;
    D_80127924 = q;
    n_alSynAddPlayer(q);
    for (i = 0; i < gSndNumVoices; i++) {
        ((SndVoice *)gSndVoices)[i].unkC9 = 0;
        SndVoice_Reset((SndVoice *)&((SndVoice *)gSndVoices)[i]);
        msg.c = 0;
        msg.a = c->unkC;
        msg.b = 0;
        if (i >= 4) {
            n_alSynAllocVoice(gSndPVoices + (i - 4) * 0x1C, &msg);
        }
    }
    SndHeap_GetUsed();
}

void Snd_SetMasterVolume(s32 arg0, s16 arg1) {
    if (arg0 & 1) {
        D_8012794C = arg1;
    }
    if (arg0 & 2) {
        D_8012794E = arg1;
    }
}

s32 Snd_PlaySong(void) {
    s32 h;

    h = SndSong_Start();
    Snd_Resume(h);
    return h;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", Snd_PlaySongAtMarker);

s32 Snd_PlaySfxDefault(s32 id) {
    s32 bank;
    s32 h;

    bank = gSndSfxBankOnce;
    if (bank == 0) {
        bank = gSndSfxBank;
        if (bank == 0) {
            gSndCurWaveBank = 0;
            return 0;
        }
    } else {
        gSndSfxBankOnce = 0;
    }
    if (gSndCurWaveBank == 0) {
        gSndCurWaveBank = (void *)((SeqPl6_ *)bank)->unk10;
    }
    h = SndVoice_PlaySfx((SndBank *)bank, id, 0x80, 0x80, -1);
    gSndCurWaveBank = 0;
    return h;
}

s32 SndVoice_Setup(SndVoice *v, SndBank *b, s32 idx, s32 a3, s32 a4, s32 a5);

s32 Snd_StartSound(s32 id, s32 a1, s32 a2, s32 retrig, s32 prio) {
    s32 bank;
    s32 i;
    SndVoice *v;
    s32 h;

    if (gSndSfxBankOnce != 0) {
        bank = gSndSfxBankOnce;
        gSndSfxBankOnce = 0;
    } else {
        bank = gSndSfxBank;
        if (bank == 0) {
            gSndCurWaveBank = 0;
            return 0;
        }
    }
    if (gSndCurWaveBank == 0) {
        gSndCurWaveBank = (void *)((SndBank *)bank)->unk10;
    }
    if (retrig != 0) {
        for (i = 4, v = gSndSfxVoices; i < gSndNumVoices; i++, v++) {
            if ((u16)v->unkA6 == id && v->unk78 == (SndBank *)bank) {
                if (prio == -1) {
                    prio = ((SndBank *)bank)->entries[id].b;
                }
                h = SndVoice_Setup(v, (SndBank *)bank, id, a1, a2, prio);
                gSndCurWaveBank = 0;
                return h;
            }
        }
    }
    h = SndVoice_PlaySfx((SndBank *)bank, id, a1, a2, prio);
    gSndCurWaveBank = 0;
    return h;
}

/* Fades out (or releases) every voice of the groups in flags (SND_GROUP_*). */
void Snd_FadeOutAll(s32 flags, s32 val) {
    s32 i;
    s32 rate;
    SndVoice *v;

    rate = val;
    if (val == 0) {
        rate = 1;
    }
    v = *(SndVoice **)&gSndVoices;
    for (i = 0; i < gSndNumVoices; i++, v++) {
        if (!((v->unk78 != NULL) ? (flags & SND_GROUP_SFX) : (flags & SND_GROUP_SEQ))) {
            continue;
        }
        if (v->unk4 == 0) {
            continue;
        }
        if (v->unk10 != -1) {
            continue;
        }
        if (v->unk0 & 1) {
            v->unk1C = 1;
            v->unk10 = 0;
            v->unk0 &= ~1;
        } else {
            v->unk1C = rate;
            v->unk10 = val;
        }
    }
}

s32 Snd_CountActive(s32 flags) {
    s32 i;
    s32 n;
    SndVoice *v;

    n = 0;
    v = *(SndVoice **)&gSndVoices;
    for (i = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk4 == 0) {
            continue;
        }
        if ((v->unk78 != 0) ? (flags & 1) : (flags & 2)) {
            n++;
        }
    }
    return n;
}

s32 Snd_Stop(s32 id, s32 val) {
    s32 i;
    s32 n;
    s32 rate;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    rate = val;
    if (val == 0) {
        rate = 1;
    }
    i = 0;
    v = *(SndVoice **)&gSndVoices;
    for (n = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id && v->unk10 == -1) {
            if (v->unk0 & 1) {
                v->unk1C = 1;
                v->unk10 = 0;
                v->unk0 &= ~1;
            } else {
                v->unk1C = rate;
                v->unk10 = val;
            }
            n++;
        }
    }
    return n;
}

s32 Snd_IsPlaying(s32 id) {
    s32 i;
    SndVoice *v;
    s32 n;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = *(SndVoice **)&gSndVoices;
    for (n = 0; i < gSndNumVoices; i++, v++) {
        n += (v->unk44 == id);
    }
    return n;
}

s32 Snd_SetVolume(s32 id, s16 val) {
    s32 i;
    s32 n;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = (*(SndVoice * *)&gSndVoices);
    for (n = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            v->unk9E = val;
            n++;
        }
    }
    return n;
}

s32 Snd_SetPan(s32 id, s16 val) {
    s32 i;
    s32 n;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = (*(SndVoice * *)&gSndVoices);
    for (n = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            n++;
            v->unkB0 = val;
            v->unkBE = 0xFF;
        }
    }
    return n;
}


s32 Snd_SetPitch(s32 id, f32 val) {
    s32 i;
    s32 n;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = (*(SndVoice * *)&gSndVoices);
    for (n = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            v->unk30 = val + v->unk90;
            n++;
        }
    }
    return n;
}


s32 Snd_SetTempoScale(s32 id, s32 val) {
    s32 i;
    s32 n;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    if (val <= 0) {
        val = 1;
    } else if (val > 256) {
        val = 256;
    }
    i = 0;
    v = (*(SndVoice * *)&gSndVoices);
    for (n = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            n++;
            v->unk98 = val;
            v->unk9C = (v->unkA8 * val) >> 7;
        }
    }
    return n;
}


s32 Snd_SetFxMix(s32 id, s32 val) {
    s32 i;
    s32 n;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    if (val < 0) {
        val = 0;
    } else if (val >= 128) {
        val = 127;
    }
    i = 0;
    v = (*(SndVoice * *)&gSndVoices);
    for (n = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            n++;
            v->unkCB = val;
            v->unkCC = 0xFF;
        }
    }
    return n;
}


void Snd_LoadWaveBank(s32 arg0) {
    SndWave_Relocate();
    if (gSndDefWaveBank == 0) {
        gSndDefWaveBank = arg0;
    }
}

void *Snd_UseWaveBank(SeqPl6_ *pl) {
    if (pl != NULL && pl->unk10 < 0) {
        gSndCurWaveBank = pl;
    }
    return gSndCurWaveBank;
}

void Snd_SetDefaultWaveBank(SeqPl6_ *pl) {
    if (pl != NULL && pl->unk10 < 0) {
        gSndDefWaveBank = (s32)pl;
    }
}

s32 Snd_GetDefaultWaveBank(void) {
    return gSndDefWaveBank;
}

s32 Snd_GetTag(s32 id) {
    s32 i;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    v = (*(SndVoice * *)&gSndVoices);
    for (i = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            return v->unk7C;
        }
    }
    return 0;
}


void Snd_Pause(s32 arg) {
    AudMsg_ msg;
    msg.type = 0;
    msg.arg = arg;
    SndMsg_Post(&msg);
}


void Snd_Resume(s32 arg) {
    AudMsg_ msg;
    msg.type = 1;
    msg.arg = arg;
    SndMsg_Post(&msg);
}


void Snd_SelectFx(s32 arg) {
    AudMsg_ msg;
    msg.type = 2;
    msg.arg = arg;
    gSndFxPreset = arg;
    SndMsg_Post(&msg);
}


s32 Snd_SetSeqFxControl(s32 a) {
    struct { u8 type; u8 p[3]; s32 val; } m;
    s32 r;

    r = 1;
    if (a == 0) {
        m.type = 2;
        m.val = gSndFxPreset;
        gSndFxPreset = m.val;
        r = SndMsg_Post((AudMsgE_ *)&m);
    }
    if (r != 0) {
        gSndSeqFxControl = a;
    }
    return r;
}

void Snd_LoadSfxBank(u8 *base) {
    s32 i;
    Bank89_ *b;

    b = (Bank89_ *)base;
    if (b->flags & 1) {
        return;
    }
    if (gSndSfxBank == 0) {
        gSndSfxBank = (s32)base;
    }
    b->flags = 1;
    b->unk10 = 0;
    b->unk14 = (s32)base + b->unk14;
    for (i = 0; i < b->n; i++) {
        b->e[i].a = (s32)base + b->e[i].a;
    }
}


s32 func_800CE920(SeqPl6_ *pl) {
    return pl->unk4;
}

void Snd_SetSfxBank(s32 arg0) {
    gSndSfxBank = arg0;
}

void Snd_SetSfxBankOnce(s32 arg0) {
    gSndSfxBankOnce = arg0;
}

s32 Snd_GetSfxBank(void) {
    return gSndSfxBank;
}

void SndBank_SetWaveBank(SeqPl6_ *pl, s32 v) {
    pl->unk10 = v;
}

s32 SndBank_GetWaveBank(SeqPl6_ *pl) {
    return pl->unk10;
}

void Snd_SetTaskHooks(s32 arg0) {
    gSndTaskHooks = arg0;
}

s32 func_800CE968(s32 id) {
    s32 i;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    v = (*(SndVoice **)&gSndVoices);
    for (i = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            if (v->unk74 != 0) {
                return v->unk74->unk8;
            }
            return v->unk78->unk8;
        }
    }
    return 0;
}

s32 func_800CE9CC(s32 id) {
    s32 i;
    SndVoice *v;

    if (id == 0) {
        return 0;
    }
    v = (*(SndVoice * *)&gSndVoices);
    for (i = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            if (v->unk74 != 0) {
                return v->unk74->unk20;
            }
            return v->unk78->unk14;
        }
    }
    return 0;
}


void Snd_SetMarkerCallback(s32 arg0) {
    gSndMarkerCallback = arg0;
}

void SndMsg_InitQueue(s32 n) {
    if (n < 0x40) {
        n = 0x40;
    } else if (n > 0x400) {
        n = 0x400;
    }
    gSndMsgQueue = SndHeap_Alloc(n * 8);
    gSndMsgCount = n;
    gSndMsgWrite = 0;
    gSndMsgRead = 0;
}

void SndMsg_Dispatch(AudMsg_ *msg) {
    switch (msg->type) {
    case 0:
        SndVoice_SetFlags(msg->arg, -2, 1);
        return;
    case 1:
        SndVoice_SetFlags(msg->arg, -2, 0);
        return;
    case 2:
        SndFx_SelectPreset(msg->arg);
        return;
    }
}

s32 SndMsg_Post(AudMsgE_ *msg) {
    s32 next = (gSndMsgWrite + 1) % gSndMsgCount;

    if (next == gSndMsgRead) {
        return 0;
    }
    Snd_MemMove(&(*(AudMsgE_ * *)&gSndMsgQueue)[gSndMsgWrite], msg, 8);
    gSndMsgWrite = next;
    return 1;
}


void SndVoice_ProcessEvents(void *arg0, s32 arg1);
void SndPitchCurve_Update(void *arg0);
void SndEnv_Update(SndVoice *s);
void SndAutoPan_Update(SndVoice *s);
f32 SndVibrato_Update(SndVoice *c);
f32 SndTrill_Update(SndVoice *c);
void SndVolCurve_Update(SndVoice *s);
void SndVoice_UpdatePitch(void *arg0, s32 arg1, f32 arg2);
void SndVoice_UpdateVolPan(void *arg0, s32 arg1);
s32 Snd_Update(void) {
    s32 i;
    SndVoice *v;
    f32 f;

    while (gSndMsgRead != gSndMsgWrite) {
        SndMsg_Dispatch((void *)(gSndMsgQueue + gSndMsgRead * 8));
        gSndMsgRead++;
        if (gSndMsgRead == gSndMsgCount) {
            gSndMsgRead = 0;
        }
    }
    for (i = -4, v = (SndVoice *)gSndVoices; i < gSndNumVoices - 4; i++, v++) {
        if (v->unk4 == 0) {
            continue;
        }
        if (v->unk0 & 1) {
            continue;
        }
        if (v->unk8 != 0) {
            SndVoice_KeyOn(v, i);
        }
        v->unkC += v->unk9C;
        if (v->unk9A != 0x7FFF) {
            while ((s32)(v->unk3C - v->unkC) < 0) {
                if (v->unk4 == 0) {
                    break;
                }
                SndVoice_ProcessEvents(v, i);
            }
            if (v->unk4 == 0) {
                continue;
            }
        }
        if (v->unk38 != 0 && (s32)(v->unk14 - v->unkC) < 0) {
            SndVolCurve_Update((SndVoice *)v);
        }
        if (v->unk34 != 0 && (s32)(v->unk18 - v->unkC) < 0) {
            SndPitchCurve_Update(v);
        }
        if (v->unk10 != -1 && --v->unk10 == -1) {
            v->unk38 = 0;
            v->unk34 = 0;
            v->unk74 = 0;
            v->unk78 = 0;
            v->unk44 = 0;
            v->unk8 = 0;
            v->unk4 = 0;
            if (v->unkC9 != 0) {
                v->unkC9 = 0;
                n_alSynStopVoice(gSndPVoices + i * 0x1C);
            }
        }
        if (v->unkC9 != 0) {
            if (v->unkC3 != 0) {
                SndEnv_Update((SndVoice *)v);
            }
            if (v->unkD4 != 0 && (s32)(v->unk94 - v->unkC) < 0) {
                SndAutoPan_Update((SndVoice *)v);
            }
            f = v->unk30;
            if (v->unkD5 != 0) {
                f += SndVibrato_Update((SndVoice *)v);
            }
            if (v->unkCE != 0) {
                f += SndTrill_Update((SndVoice *)v);
            }
            if (v->unk8 == 0) {
                SndVoice_UpdatePitch(v, i, f);
                SndVoice_UpdateVolPan(v, i);
            }
        }
        v->unkAA = (v->unkC - v->unk40) >> 8;
    }
    return gSndTickUsec;
}


INCLUDE_ASM("asm/nonmatchings/unit_6E610", SndVoice_ProcessEvents);

void SndVoice_KeyOn(SndVoice *c, s32 i) {
    if (c->unkC9 != 0) {
        n_alSynStopVoice(gSndPVoices + i * 0x1C);
    }
    c->unkC9 = 1;
    n_alSynStartVoice(gSndPVoices + i * 0x1C, c->unk8);
    c->unk8 = 0;
}

void n_alSynSetVol(s32, s16, s32);
void n_alSynSetPan(s32, u8);
void SndVoice_UpdateVolPan(void *vp, s32 i) {
    SndVoice *v = vp;
    u32 vol;

    vol = (u32)(v->unkBC * v->unkC4 * v->unkBB * v->unk9E) >> 13;
    if (vol > 0x7FFF) {
        vol = 0x7FFF;
    }
    vol *= (u16)(v->unk78 != NULL ? D_8012794C : D_8012794E);
    vol >>= 15;
    if (v->unk10 != -1) {
        vol = vol * v->unk10 / (u32)v->unk1C;
    }
    if (vol != v->unkA0) {
        v->unkA0 = vol;
        n_alSynSetVol(gSndPVoices + i * 0x1C, vol, gSndTickUsec);
    }
    vol = (v->unkBD * v->unkB0 >> 7) & 0x7F;
    if (vol != v->unkBE) {
        v->unkBE = vol;
        n_alSynSetPan(gSndPVoices + i * 0x1C, vol);
    }
}

f32 Snd_Exp2(f32 x);
void n_alSynSetPitch(s32, f32);
void SndVoice_UpdatePitch(void *vp, s32 i, f32 bend) {
    SndVoice *v = vp;
    f32 pitch = v->unk2C;
    u8 n = v->unkB8;

    if (n != 0) {
        if (n >= v->unkAA) {
            pitch = v->unk4C + (pitch - v->unk4C) / n * v->unkAA;
        }
        v->unk50 = pitch;
    }
    pitch += bend + v->unk24;
    if (pitch != v->unk28) {
        v->unk28 = pitch;
        pitch = Snd_Exp2(pitch * (1.0 / 12.0));
        if (pitch > 2.0) {
            pitch = 2.0f;
            v->unkBB = 0;
        }
        n_alSynSetPitch((gSndPVoices + i * 0x1C), pitch);
    }
}

void SndEnv_Start(SndVoice *s) {
    if (s->unk9A != 0x7FFF) {
        if (s->unkB2 != 0) {
            s->unk54 = s->unk40 + (s->unkB2 << 8);
        } else {
            s->unk54 = s->unk3C - (s->unkB4 << 8);
        }
    } else {
        s->unk54 = s->unk40 + 0x7FFFFFFF;
    }
    s->unkC3 = 1;
    s->unkC4 = s->unkC0;
    s->unkC5 = s->unkBF;
}


void SndEnv_Update(SndVoice *s) {
    s32 t;

    if ((s32)(s->unk54 - s->unkC) < 0 && s->unkC3 < 4) {
        s->unkC3 = 4;
        s->unkC5 = 1;
        s->unkCD = s->unkC4;
    }
    s->unkC5 = s->unkBF;
    switch (s->unkC3) {
    case 1:
        t = (((s->unkC - s->unk40) >> 8) * s->unk64) >> 10;
        if (t < s->unkC6) {
            s->unkC4 = s->unkC0 + (s32)(s->unk58 * t);
        } else {
            s->unkC3++;
            s->unkC4 = s->unkC1;
        }
        break;
    case 2:
        t = ((((s->unkC - s->unk40) >> 8) - s->unkC6) * s->unk64) >> 10;
        if (t < s->unkC7) {
            s->unkC4 = s->unkC1 + (s32)(s->unk5C * t);
        } else {
            s->unkC3++;
            s->unkC4 = s->unkC2;
        }
        break;
    case 3:
        break;
    case 4:
        t = (((s->unkC - s->unk54) >> 8) * s->unk64) >> 10;
        if (t < s->unkC8) {
            s->unkC4 = s->unkCD - (s32)(s->unk60 * t * s->unkCD);
        } else {
            s->unkC3++;
            s->unkC4 = 0;
        }
        break;
    }
}

void SndAutoPan_Reset(SndVoice *c) {
    c->unkD9 = 0;
    c->unk94 = c->unk40;
    c->unkDA = c->unkBD & 0x40;
}

void SndAutoPan_Update(SndVoice *s) {
    u32 t;

    do {
        s->unk94 += 0x100;
        t = s->unkD9 + s->unkD4;
        if (t < 0x40) {
            s->unkD9 = t;
        } else {
            s->unkD9 = t & 0x3F;
            t >>= 6;
            if (s->unkDA == 0) {
                s->unkBD += t;
                if (s->unkBD >= 0x80) {
                    s->unkBD = 0x7F;
                    s->unkDA = 1;
                }
            } else {
                s->unkBD -= t;
                if (s->unkBD >= 0x80 || s->unkBD == 0) {
                    s->unkBD = 0;
                    s->unkDA = 0;
                }
            }
        }
    } while (s->unk94 - s->unkC < 0);
}


f32 SndTrill_Update(SndVoice *c) {
    if (--c->unkD0 == 0) {
        if (c->unkD1 == 0) {
            c->unkD1 = c->unkD8;
            c->unkD0 = c->unkCE;
        } else {
            c->unkD1 = 0;
            c->unkD0 = c->unkCF;
        }
    }
    return c->unkD1;
}

    /* TODO (Clean version / Bug check): [evaluated 2026-11-18: an explicit return changes the ROM]
    The original code is missing an explicit return statement for the float return type.
    In the MIPS 'o32' ABI, the result of the float multiplication is computed directly into $f0,
    which happens to be the float return register, so $f0 was returned by caller convention:
        return c->unk68 = sinf(t * c->unkDC) * c->unk20;
    */
f32 SndVibrato_Update(SndVoice *c) {
    s32 t = c->unkAA - c->unkB6;

    if (t <= 0) {
        return 0.0f;
    }
    /* Original bug: no return here; the value is left in $f0 by the store below. */
    c->unk68 = sinf(t * c->unkDC) * c->unk20;
}

void SndVolCurve_Update(SndVoice *s) {
    u8 b;
    u16 v;

    do {
        s->unk14 += 0x100;
        if (--s->unkA2 == 0) {
            b = *s->unk38++;
            if (b >= 0x80) {
                s->unkBC = b & 0x7F;
                b = *s->unk38++;
                if (b >= 0x80) {
                    v = (b & 0x7F) << 8;
                    s->unkA2 = v;
                    v += 2;
                    s->unkA2 = *s->unk38++ + v;
                } else {
                    s->unkA2 = b + 2;
                }
            } else {
                s->unkBC = b;
                s->unkA2 = 1;
            }
        }
    } while (s->unk14 - s->unkC < 0);
}

void SndPitchCurve_Update(void *arg) {
    SndVoice *s = arg;
    u8 b;
    u16 v;
    f32 f;

    do {
        s->unk18 += 0x100;
        if (--s->unkA4 == 0) {
            b = *s->unk34++;
            if (b >= 0x80) {
                f = (f32)(b & 0x7F) - 64.0;
                s->unk70 = f;
                s->unk24 = f * s->unk6C;
                b = *s->unk34++;
                if (b >= 0x80) {
                    v = (b & 0x7F) << 8;
                    s->unkA4 = v;
                    v += 2;
                    s->unkA4 = *s->unk34++ + v;
                } else {
                    s->unkA4 = b + 2;
                }
            } else {
                f = (f32)b - 64.0;
                s->unkA4 = 1;
                s->unk70 = f;
                s->unk24 = f * s->unk6C;
            }
        }
    } while (s->unk18 - s->unkC < 0);
}

    /* TODO (Clean version): [evaluated 2026-11-18: kept as documentation; the code is the
    original algorithm]
    Computes 2.0f^x (exp2f(x)) using degree-6 Taylor series polynomial expansion:
        2^x = exp(x * ln(2)) = 1 + c1*x + c2*x^2 + c3*x^3 + c4*x^4 + c5*x^5 + c6*x^6
    where c_k = (ln 2)^k / k!:
        c1 = 0.693147180559945   (ln 2)
        c2 = 0.240226506959101   ((ln 2)^2 / 2)
        c3 = 0.055504108664822   ((ln 2)^3 / 6)
        c4 = 0.009618129107628   ((ln 2)^4 / 24)
        c5 = 0.001333355814643   ((ln 2)^5 / 120)
        c6 = 0.000154035303934   ((ln 2)^6 / 720)
    Negative values evaluate 1.0 / exp2(abs(x)). Unused variables x3, x5, x6 can be removed.
    */
f32 Snd_Exp2(f32 x) {
    f32 x2;
    f32 x4;
    f32 x3;
    f32 x5;
    f32 x6;

    if (x == 0.0f) {
        return 1.0f;
    }
    if (x > 0.0f) {
        x2 = x * x;
        return 1.0 + x * 0.69314718055994495 + x2 * 0.240226506959101 + x2 * x * 0.055504108664821597
            + x2 * x2 * 0.0096181291076284803 + x2 * x2 * x * 0.00133335581464284 + x2 * x2 * x2 * 0.00015403530393381601;
    } else {
        x = -x;
        x2 = x * x;
        x4 = x2 * x2;
        return 1.0 / (1.0 + x * 0.69314718055994495 + x2 * 0.240226506959101 + (x2 * x) * 0.055504108664821597
            + x4 * 0.0096181291076284803 + (x4 * x) * 0.00133335581464284 + (x4 * x2) * 0.00015403530393381601);
    }
}

void Snd_RelocatePtrs(void *arg0, s32 arg1, s32 arg2);
void osWritebackDCacheAll(void);
typedef struct { u32 base; u8 pad4[4]; u8 b8; u8 b9; u8 padA[2]; s32 pC; s32 p10; } SndWave_;
typedef struct { u8 pad0[0x10]; u32 flags; u8 pad14[0xC]; s32 count; u8 *tune; f32 *pitch; SndWave_ **waves; } SndWaveBank_;
#define SND_S8(x) (((x) & 0x80) ? ((x) & 0xFF) - 0x100 : ((x) & 0xFF))

/* Relocates a wave bank once: pitch and tune tables to floats, wave pointers to the ROM table and the bank. */
void SndWave_Relocate(u8 *base, s32 table) {
    SndWaveBank_ *b = (SndWaveBank_ *)base;
    s32 i;
    u8 *p;
    s32 v;
    u8 t;
    f32 f;
    u32 a;

    if (!(b->flags & 0x80000000)) {
        b->flags |= 0x80000000;
        Snd_RelocatePtrs(&b->tune, (s32)b, 3);
        Snd_RelocatePtrs(b->waves, (s32)b, b->count);
        for (i = 0; i < b->count; i++) {
            p = (u8 *)&b->pitch[i];
            v = SND_S8(*p);
            f = v;
            *(f32 *)p = f / 100.0;
            t = b->tune[i] - 0x30;
            f = SND_S8(t);
            *(f32 *)p += f;
            if (b->waves[i]->b9 == 0) {
                a = b->waves[i]->base;
                if ((a & 0xFF000000) != 0xFF000000) {
                    a += table;
                    b->waves[i]->base = a;
                }
                b->waves[i]->b9 = 1;
                if (b->waves[i]->pC != 0) {
                    b->waves[i]->pC += (s32)base;
                }
                if (b->waves[i]->b8 == 0) {
                    b->waves[i]->p10 += (s32)base;
                }
            }
        }
        osWritebackDCacheAll();
    }
}

    /* TODO (Clean version): [evaluated 2026-11-18: kept as documentation; the code is the
    original algorithm]
    Implements a 32-bit Linear Feedback Shift Register (LFSR) pseudo-random number generator,
    stepped 8 iterations per sample with polynomial taps at bits 30 and 27 (mask 0x48000000),
    returning (rand / 2^32) * range:
        s32 audioRandRange(s32 range) {
            for (s32 i = 0; i < 8; i++) {
                s32 next = lfsrState * 2;
                s32 tap = lfsrState & 0x48000000;
                s32 feedback = (tap == 0x48000000) || (tap == 0x08000000);
                lfsrState = feedback ? (next | 1) : next;
            }
            return (s32)((f32)range * ((f32)(u32)lfsrState / 4294967296.0f));
        }
    */
s32 Snd_Random(s32 range) {
    s32 next;
    s32 tap;
    s32 feedback;
    s32 i;

    i = 0;
    do {
        next = gSndRandSeed * 2;
        tap = gSndRandSeed & 0x48000000;
        feedback = (tap == 0x48000000) | (tap == 0x08000000);
        gSndRandSeed = next;
        i++;
        if (feedback) {
            gSndRandSeed = next | 1;
        }
    } while (i < 8);
    return (f32)range * ((f32)gSndRandSeed / 65536.0f / 65536.0f);
}

void SndVoice_Reset(SndVoice *voice) {
    register SndVoice *v asm("$6") = voice;
    register u8 *p asm("$4") = (u8 *)voice;
    register s32 i asm("$3") = 0;
    register u8 c9 asm("$7") = v->unkC9;
    register u8 ff asm("$5");
    register s32 rate asm("$4");
    register s32 v7f asm("$3");
    register void *wb asm("$3");

    v->unk4 = 0;
    do {
        *p++ = 0;
    } while (++i < sizeof(SndVoice));

    ff = 0xFF;
    v->unkCC = ff;
    v->unkBE = ff;
    rate = 0x6000 / gSndFrameRate;
    v7f = 0x7F;
    v->unkD3 = v7f;
    v->unkBC = v7f;
    v->unkC1 = v7f;
    v->unkC7 = ff;
    v->unkC2 = v7f;
    v->unkBD = 0x40;
    v->unkBF = 1;
    v->unkC6 = 1;
    v->unkC8 = 0xF;
    wb = gSndCurWaveBank;
    v->unkA0 = -1;
    v->unk9A = 1;
    v->unkA2 = 1;
    v->unkA4 = 1;
    v->unk10 = -1;
    v->unk9E = 0x80;
    v->unkB0 = 0x80;
    v->unk98 = 0x80;
    v->unk28 = 99.9f;
    v->unk6C = 0.03125f;
    v->unk58 = 1.0f;
    v->unk5C = 1.0f / 255.0f;
    v->unk60 = 1.0f / 15.0f;
    v->unkA8 = rate;
    v->unk9C = rate;
    if (wb == NULL) {
        wb = (void *)gSndDefWaveBank;
    }
    v->unk7C = (s32)wb;
    v->unkC9 = c9;
}

/* Picks a voice for a sound of sequence seq (slot idx; idx < 0 may use the music voices): a free voice,
 * else the idle one with the lowest priority, else one not playing seq, else one already playing this
 * slot; as a last resort voice 4 + idx % (sfx voices). */
s32 SndVoice_Alloc(SndSeq *seq, s32 idx) {
    SndVoice *v;
    s32 i;
    s32 best;
    s32 pick;

    if (idx < 0) {
        for (i = 0, v = (SndVoice *)gSndVoices; i < SND_MUSIC_VOICES; i++, v++) {
            if (v->unk4 == 0) {
                return i;
            }
        }
    }
    for (i = SND_MUSIC_VOICES, v = gSndSfxVoices; i < gSndNumVoices; i++, v++) {
        if (v->unk4 == 0) {
            return i;
        }
    }
    for (i = SND_MUSIC_VOICES, v = gSndSfxVoices, best = 0x7FFFFFFF, pick = SND_MUSIC_VOICES - 1; i < gSndNumVoices; i++, v++) {
        if (v->unk78 != 0 && v->unk48 <= best) {
            best = v->unk48;
            pick = i;
        }
    }
    if (pick >= SND_MUSIC_VOICES) {
        return pick;
    }
    /* Bug: v is never advanced, so this tests the first sfx voice gSndNumVoices - 4 times. */
    for (i = SND_MUSIC_VOICES, v = gSndSfxVoices; i < gSndNumVoices; i++) {
        if (v->unk78 == 0 && v->unk74 != seq) {
            return i;
        }
    }
    for (i = SND_MUSIC_VOICES, v = gSndSfxVoices; i < gSndNumVoices; i++, v++) {
        if (v->unk74 == seq && (*(s32 **)seq->pC)[idx] == v->unk80) {
            return i;
        }
    }
    return idx % (gSndNumVoices - SND_MUSIC_VOICES) + SND_MUSIC_VOICES;
}

void Snd_RelocatePtrs(void *arg0, s32 arg1, s32 arg2) {
    s32 *p = arg0;
    s32 i;

    for (i = 0; i < arg2; i++, p++) {
        if (*p != 0) {
            *p += arg1;
        }
    }
}

inline s32 SndVoice_Setup(SndVoice *v, SndBank *b, s32 idx, s32 a3, s32 a4, s32 a5) {
    SndVoice_Reset(v);
    v->unkA6 = idx;
    v->unk78 = b;
    v->unk9E = a3;
    v->unkB0 = a4;
    v->unk44 = gSndNextHandle;
    v->unk48 = a5;
    gSndNextHandle++;
    if (b->unk10 != 0) {
        v->unk7C = b->unk10;
    }
    v->unk4 = v->unk80 = b->entries[idx].a;
    return v->unk44;
}

s32 SndVoice_PlaySfx(SndBank *b, s32 idx, s32 a2, s32 a3, s32 prio) {
    s32 i;
    SndVoice *v;
    SndVoice *bv;
    s32 best;

    if (prio == -1) {
        prio = b->entries[idx].b;
    }
    best = prio + 1;
    for (i = 4, v = gSndSfxVoices; i < gSndNumVoices; i++, v++) {
        if (v->unk4 == 0) {
            return SndVoice_Setup(v, b, idx, a2, a3, prio);
        }
        if (v->unk78 != 0 && v->unk48 < best) {
            best = v->unk48;
            bv = v;
        }
    }
    if (best < prio) {
        return SndVoice_Setup(bv, b, idx, a2, a3, prio);
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", SndSong_Start);

void SndVoice_SetFlags(s32 id, s32 mask, s32 bits) {
    s32 i;
    SndVoice *v = (*(SndVoice * *)&gSndVoices);

    for (i = 0; i < gSndNumVoices; i++, v++) {
        if (v->unk44 == id) {
            v->unk0 = (v->unk0 & mask) | bits;
        }
    }
}


s32 SndCmd_End(SndVoice *c) {
    c->unk38 = 0;
    c->unk34 = 0;
    c->unk74 = 0;
    c->unk78 = 0;
    c->unk44 = 0;
    c->unk8 = 0;
    return 0;
}

u8 *SndCmd_Op81(SndVoice *s, u8 *p) {
    s32 v = *p++;
    if (v & 0x80) {
        v = *p++ | ((v & 0x7F) << 8);
    }
    s->unkAE = v;
    return p;
}


u8 *SndCmd_Op82(SndVoice *s, u8 *p) {
    u8 v = *p++;
    s->unkB8 = v;
    if (v) {
        s->unk50 = s->unk2C;
    }
    return p;
}


s32 SndCmd_Op83(SndVoice *c, s32 ptr) {
    c->unkB8 = 0;
    return ptr;
}

u8 *SndCmd_SetEnvelope(SndVoice *s, u8 *p) {
    u8 n;
    u8 c;

    n = *p++;
    if (n == 0) {
        n = 1;
    }
    s->unkBF = n;
    s->unk64 = 1024 / n;
    s->unkC0 = *p++;
    n = *p++;
    s->unkC6 = n;
    c = *p++;
    s->unk58 = (1.0 / (f32)n) * (f32)(c - s->unkC0);
    s->unkC1 = c;
    n = *p++;
    s->unkC7 = n;
    c = *p++;
    s->unk5C = (1.0 / (f32)n) * (f32)(c - s->unkC1);
    s->unkC2 = c;
    n = *p++;
    s->unkC8 = n;
    s->unk60 = 1.0 / (f32)n;
    return p;
}

u8 *SndCmd_SetTempo(SndVoice *s, u8 *p) {
    s32 tempo = ((*p * 0x6000) / 120) / gSndFrameRate;
    s32 scaled;
    s32 i;
    SndVoice *v;

    p++;
    scaled = (tempo * s->unk98) >> 7;
    if (s->unk78 != 0) {
        s->unk9C = tempo;
    } else {
        v = (*(SndVoice * *)&gSndVoices);
        for (i = 0; i < gSndNumVoices; i++, v++) {
            if (v->unk74 == s->unk74) {
                v->unkA8 = tempo;
                v->unk9C = scaled;
            }
        }
    }
    return p;
}

u8 *SndCmd_SetReleaseLead(SndVoice *c, u8 *ptr) {
    s16 v = *ptr;

    c->unkB2 = 0;
    c->unkB4 = v;
    return ptr + 1;
}

u8 *SndCmd_SetReleaseTime(SndVoice *s, u8 *p) {
    s32 hi = *p++;
    s32 lo = *p++;
    s->unkB4 = 0;
    s->unkB2 = lo | (hi << 8);
    return p;
}


u8 *SndCmd_VibratoOn(SndVoice *s, u8 *p) {
    s->unkB6 = *p++;
    s->unkD5 = *p++;
    s->unk20 = (f32)*p / 50.0;
    s->unkDC = 6.2831852 / (f32)s->unkD5;
    return p + 1;
}

u8 *SndCmd_VibratoOnNeg(SndVoice *s, u8 *p) {
    f32 a;
    f32 b;

    s->unkB6 = *p++;
    s->unkD5 = *p++;
    a = 6.2831852 / (f32)*(volatile u8 *)&s->unkD5;
    b = -(f32)*p++ / 50.0;
    s->unkDC = a;
    s->unk20 = b;
    return p;
}

s32 SndCmd_VibratoOff(SndVoice *c, s32 ptr) {
    c->unkD5 = 0;
    c->unk68 = 0;
    return ptr;
}

u8 *SndCmd_Op8B(SndVoice *s, u8 *p) {
    s32 v = *p++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *p++;
    }
    s->unkAC = v;
    return p;
}


s32 SndCmd_Op8C(SndVoice *c, s32 ptr) {
    c->unkB7 = 1;
    return ptr;
}

u8 *SndCmd_SetTranspose(SndVoice *c, u8 *ptr) {
    c->unkB9 = *ptr;
    return ptr + 1;
}

s32 SndCmd_IgnoreTranspose(SndVoice *c, s32 ptr) {
    c->unkBA = 1;
    return ptr;
}

u8 *SndCmd_SetFineTune(SndVoice *s, u8 *p) {
    s32 v = *p++;
    f32 f;

    if (v & 0x80) {
        v |= -0x100;
    }
    f = (f32)v / 100.0;
    s->unk30 = s->unk30 - s->unk90 + f;
    s->unk90 = f;
    return p;
}

s32 SndCmd_Op91(SndVoice *c, s32 ptr) {
    c->unkD6 = 1;
    return ptr;
}

s32 SndCmd_Op92(SndVoice *c, s32 ptr) {
    c->unkD6 = 0;
    return ptr;
}

s32 SndCmd_Op93(SndVoice *c, s32 ptr) {
    c->unkD7 = 1;
    return ptr;
}

s32 SndCmd_Op94(SndVoice *c, s32 ptr) {
    c->unkD7 = 0;
    return ptr;
}

u8 *SndCmd_LoopStart(SndVoice *s, u8 *p) {
    s32 i = s->unkDB;

    s->unk120[i] = *p++;
    s->unkE0[i] = p;
    s->unkF0[i] = s->unk38;
    s->unk100[i] = s->unk34;
    s->unk124[i] = s->unkBC;
    s->unk128[i] = s->unk70;
    s->unk110[i] = s->unkA2;
    s->unk118[i] = s->unkA4;
    s->unkDB++;
    return p;
}

u8 *SndCmd_LoopEnd(SndVoice *s, u8 *p) {
    s32 i = s->unkDB - 1;

    if (s->unk120[i] != 0xFF) {
        if (--s->unk120[i] == 0) {
            s->unkDB = i;
            i = -1;
        }
    }
    if (i >= 0) {
        p = s->unkE0[i];
        s->unk38 = s->unkF0[i];
        s->unk34 = s->unk100[i];
        s->unkBC = s->unk124[i];
        s->unk70 = s->unk128[i];
        s->unkA2 = s->unk110[i];
        s->unkA4 = s->unk118[i];
        s->unk24 = s->unk70 * s->unk6C;
    }
    return p;
}

u8 *SndCmd_TrillOn(SndVoice *s, u8 *p) {
    s->unkD8 = *p++;
    s->unkCE = *p++;
    s->unkCF = *p;
    return p + 1;
}

s32 SndCmd_TrillOff(SndVoice *c, s32 ptr) {
    c->unkCE = 0;
    return ptr;
}

s32 SndCmd_Op99(SndVoice *c, s32 ptr) {
    c->unkD2 = 1;
    return ptr;
}

s32 SndCmd_Op9A(SndVoice *c, s32 ptr) {
    c->unkD2 = 0;
    return ptr;
}

u8 *SndCmd_Op9B(SndVoice *c, u8 *ptr) {
    u8 v = *ptr;

    c->unkD2 = 0;
    c->unkD3 = v;
    return ptr + 1;
}

u8 *SndCmd_SetPan(SndVoice *c, u8 *ptr) {
    c->unkBD = *ptr >> 1;
    return ptr + 1;
}

s32 SndCmd_Skip2(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

u8 *SndCmd_SetPatch(SndVoice *s, u8 *p) {
    s32 v = *p++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *p++;
    }
    s->unk84 = s->unk74->unk1C + v * 6;
    return p;
}


s32 SndCmd_ClearPatch(SndVoice *c, s32 ptr) {
    c->unk84 = 0;
    return ptr;
}

s32 SndCmd_Skip1(s32 arg0, s32 arg1) {
    return arg1 + 1;
}

/* Sets the event, volume-curve and pitch-curve offsets (16-bit big-endian each). */
u8 *SndCmd_SetCurves(SndVoice *s, u8 *p) {
    s32 off;
    s32 v;

    off = *p++ << 8;
    off += *p++;
    v = *p++ << 8;
    v += *p++;
    s->unkA2 = 1;
    s->unk38 = s->unk8C + v;
    v = *p++ << 8;
    v += *p++;
    s->unkA4 = 1;
    s->unk34 = s->unk88 + v;
    return (u8 *)(s->unk80 + off);
}

u8 *SndCmd_SetFxMix(SndVoice *c, u8 *ptr) {
    c->unkCA = *ptr;
    return ptr + 1;
}

u8 *SndCmd_RandomTranspose(SndVoice *s, u8 *p) {
    s->unkB9 = Snd_Random(*p++);
    s->unkB9 += *p++;
    return p;
}

u8 *SndCmd_RandomVolume(SndVoice *s, u8 *p) {
    s->unkBC = Snd_Random(*p++);
    s->unkBC += *p++;
    return p;
}


u8 *SndCmd_RandomPan(SndVoice *s, u8 *p) {
    s->unkBD = Snd_Random(*p++);
    s->unkBD += *p++;
    return p;
}


u8 *SndCmd_SetVolume(SndVoice *c, u8 *ptr) {
    c->unkBC = *ptr;
    return ptr + 1;
}

u8 *SndCmd_PlaySfx(SndVoice *s, u8 *p) {
    s32 v = *p++;
    s32 id;
    s32 i;
    SndVoice *vc;

    if (v >= 0x80) {
        v = ((v & 0x7F) << 8) + *p++;
    }
    s->unk48++;
    id = SndVoice_PlaySfx(s->unk78, v, s->unk9E, s->unkB0, s->unk48);
    s->unk48--;
    if (id != 0) {
        vc = (*(SndVoice * *)&gSndVoices);
        for (i = 0; i < gSndNumVoices; i++, vc++) {
            if (vc->unk44 == id) {
                vc->unk44 = s->unk44;
                vc->unk7C = ((SeqY2_ *)s)->unk7C;
            }
        }
    }
    return p;
}

u8 *SndCmd_SetBendRange(SndVoice *c, u8 *ptr) {
    f32 v;

    v = (f32)*ptr * D_801016A0;
    c->unk6C = v;
    c->unk24 = c->unk70 * v;
    return ptr + 1;
}

u8 *SndCmd_SetAutoPanRate(SndVoice *c, u8 *ptr) {
    c->unkD4 = *ptr;
    return ptr + 1;
}

u8 *SndCmd_SelectFx(s32 arg0, u8 *ptr) {
    u8 v = *ptr++;

    if (gSndSeqFxControl == 1) {
        SndFx_SelectPreset(v);
    }
    return ptr;
}

u8 *SndCmd_Marker(Seq12_ *s, u8 *p) {
    u8 a;
    s32 len; /* variable-length field, decoded but unused */

    a = *p++;
    len = *p++;
    if (len & 0x80) {
        len = ((len & 0x7F) << 8) | *p++;
    }
    if ((s->unk0 & 3) == 2) {
        if (gSndMarkerCallback != 0) {
            ((void (*)(s32, u8))gSndMarkerCallback)(s->unk44, a);
        }
    }
    return p;
}


s32 SndCmd_OpAC(SndVoice *c, s32 ptr) {
    c->unkAC = 0;
    return ptr;
}

void SndSyn_Init(s32 arg0, s32 arg1) {
    if (D_800FB170 == 0) {
        D_800FB170 = arg0;
        if (n_syn == 0) {
            SndFx_UseCustomConfig(arg1);
            D_8014C6D0 = D_800FB170;
            n_syn = (void *)D_800FB170;
            SndSyn_New((SndSynConfig *)arg1);
        }
    }
}

void _n_initPVoice(void *, s32, void *);
s32 SndFx_Create(s32 arg0);
void n_alFxPull(void);
void n_alAuxBusPull(void);

/* The game's own copy of n_alSynNew (debug heap allocator), on mirror types of n_audio.h. */
void SndSyn_New(SndSynConfig *c) {
    void *hp = c->heap;
    s32 i;
    SndPVoice *pv;
    SndParam *q;

    ((SndSynth *)n_syn)->head = 0;
    ((SndSynth *)n_syn)->numPVoices = c->maxPVoices;
    ((SndSynth *)n_syn)->curSamples = 0;
    ((SndSynth *)n_syn)->paramSamples = 0;
    ((SndSynth *)n_syn)->outputRate = c->outputRate;
    ((SndSynth *)n_syn)->maxOutSamples = 184;
    ((SndSynth *)n_syn)->dma = c->dmaproc;
    ((SndSynth *)n_syn)->maxAuxBusses = 1;
    ((SndSynth *)n_syn)->outBuf = 0;
    ((SndSynth *)n_syn)->auxBus = (SndAuxBus *)alHeapDBAlloc(0, 0, hp, 1, 0x44);
    ((SndSynth *)n_syn)->auxBus->sourceCount = 0;
    ((SndSynth *)n_syn)->auxBus->maxSources = c->maxPVoices;
    ((SndSynth *)n_syn)->auxBus->sources = (SndPVoice **)alHeapDBAlloc(0, 0, hp, c->maxPVoices, 4);
    ((SndSynth *)n_syn)->mainBus = (SndMainBus *)alHeapDBAlloc(0, 0, hp, 1, 0x14);
    if (c->fxType != 0) {
        ((SndSynth *)n_syn)->auxBus->fx = ((s32 (*)(s32, SndSynConfig *, void *))SndFx_Create)(0, c, hp);
        ((SndSynth *)n_syn)->mainBus->pull = n_alFxPull;
    } else {
        ((SndSynth *)n_syn)->mainBus->pull = n_alAuxBusPull;
    }
    ((SndSynth *)n_syn)->pFreeList.next = 0;
    ((SndSynth *)n_syn)->pFreeList.prev = 0;
    ((SndSynth *)n_syn)->pLameList.next = 0;
    ((SndSynth *)n_syn)->pLameList.prev = 0;
    ((SndSynth *)n_syn)->pAllocList.next = 0;
    ((SndSynth *)n_syn)->pAllocList.prev = 0;
    pv = (SndPVoice *)alHeapDBAlloc(0, 0, hp, c->maxPVoices, 0x8C);
    for (i = 0; i < c->maxPVoices; i++) {
        alLink(&pv[i], &((SndSynth *)n_syn)->pFreeList);
        pv[i].vvoice = 0;
        _n_initPVoice(&pv[i], ((SndSynth *)n_syn)->dma, hp);
        ((SndSynth *)n_syn)->auxBus->sources[((SndSynth *)n_syn)->auxBus->sourceCount++] = &pv[i];
    }
    q = (SndParam *)alHeapDBAlloc(0, 0, hp, c->maxUpdates, 0x1C);
    ((SndSynth *)n_syn)->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        q[i].next = ((SndSynth *)n_syn)->paramList;
        ((SndSynth *)n_syn)->paramList = &q[i];
    }
    ((SndSynth *)n_syn)->heap = hp;
}

s32 SndFx_Create(s32 arg0) {
    s32 i = (s16)arg0;

    SndFx_Alloc(&((SndSynth *)n_syn)->auxBus->unk24[i]);
    return ((SndSynth *)n_syn)->auxBus->unk24[i];
}


s32 alFxParamHdl();
extern s32 D_8014C6E0;
extern s32 D_8014C6E4;
extern s32 D_8014C6E8;
extern s32 D_8014C6EC;
extern s32 D_8014C6F0;
extern s32 D_8014C6F4;

void SndFx_Alloc(s32 *rp, u8 *c, void *hp) {
    s32 r;

    r = alHeapDBAlloc(0, 0, hp, 1, 0x2C);
    *rp = r;
    *(void **)(r + 0x28) = alFxParamHdl;
    D_8014C6E0 = alHeapDBAlloc(0, 0, hp, SndFx_MaxSections(), 0x28);
    D_8014C6E4 = alHeapDBAlloc(0, 0, hp, SndFx_MaxLength(), 2);
    D_8014C6E8 = alHeapDBAlloc(0, 0, hp, 1, 0x34);
    D_8014C6EC = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    D_8014C6F0 = alHeapDBAlloc(0, 0, hp, 1, 0x30);
    D_8014C6F4 = alHeapDBAlloc(0, 0, hp, 1, 8);
    SndFx_SetParams(*(s32 *)(c + 0x20));
}


typedef struct { s16 fc; s16 fgain; s32 pad4; s16 fccoef[16]; void *fstate; s32 first; } LPF_;
typedef struct { u8 filter[0x14]; void *state; f32 ratio; s32 upitch; f32 delta; s32 first; void *ctrlList; void *ctrlTail; s32 motion; } RSF_;
typedef struct { u32 input; u32 output; s16 ffcoef; s16 fbcoef; s16 gain; f32 rsinc; f32 rsval; s32 rsdelta; f32 rsgain; LPF_ *lp; RSF_ *rs; } DLF_;
typedef struct { u8 filter[0x14]; s16 *base; s16 *input; u32 length; DLF_ *delay; u8 section_count; s32 pad28; } FXF_;
typedef struct { s32 maxVVoices; s32 maxPVoices; s32 maxUpdates; s32 maxFXbusses; void *dmaproc; void *heap; s32 outputRate; u8 fxType; s32 *params; } SCF_;
void _init_lpfilter(void *);

extern s32 D_8014C6D0;
typedef struct { s32 delay; s32 base; s32 pool[4]; } FxPool_;

void SndFx_SetParams(s32 *param) {
    u16 i, j, k;
    DLF_ *d;
    FXF_ *r;
    s32 *pool;
    s32 *pool2;

    r = *(FXF_ **)(*(u8 **)(D_8014C6D0 + 0x34) + 0x24);
    j = 0;
    r->section_count = param[j++];
    r->length = param[j++];
    r->delay = (DLF_ *)((FxPool_ *)&D_8014C6E0)->delay;
    r->base = (s16 *)((FxPool_ *)&D_8014C6E0)->base;
    r->input = r->base;
    for (i = 0; i < r->length; i++) {
        r->base[i] = 0;
    }
    i = 0;
    if (r->section_count != 0) {
        pool = ((FxPool_ *)&D_8014C6E0)->pool;
        pool2 = pool + 2;
        do {
            d = &r->delay[i];
            d->input = param[j++];
            d->output = param[j++];
            d->fbcoef = param[j++];
            d->ffcoef = param[j++];
            d->gain = param[j++];
            if (param[j]) {
                d->rsinc = ((((f32)param[j++]) / 1000) * 2.0) / *(s32 *)(D_8014C6D0 + 0x40);
                d->rsgain = (((f32)param[j++]) / 173123.404906676) * (d->output - d->input);
                d->rsval = 1.0;
                d->rsdelta = 0.0;
                d->rs = (RSF_ *)pool[0];
                d->rs->state = (void *)pool[1];
                d->rs->delta = 0.0;
                d->rs->first = 1;
            } else {
                d->rs = 0;
                j++;
                j++;
            }
            if (param[j]) {
                d->lp = (LPF_ *)pool2[0];
                d->lp->fstate = (void *)pool2[1];
                d->lp->fc = param[j++];
                _init_lpfilter(d->lp);
            } else {
                d->lp = 0;
                j++;
            }
            i++;
        } while (i < r->section_count);
    }
}

void SndFx_UseCustomConfig(SndSynConfig *c) {
    c->fxType = 6; /* AL_FX_CUSTOM */
    c->params = &D_800FACF8;
}

s32 SndFx_MaxSections(void) {
    s32 i;
    s32 max = 0;

    for (i = 0; ((Entry_ * *)&gSndFxPresets)[i] != 0; i++) {
        if (max < ((Entry_ * *)&gSndFxPresets)[i]->unk0) {
            max = ((Entry_ * *)&gSndFxPresets)[i]->unk0;
        }
    }
    return max;
}


s32 SndFx_MaxLength(void) {
    s32 i;
    s32 max = 0;

    for (i = 0; ((Entry_ * *)&gSndFxPresets)[i] != 0; i++) {
        if (max < ((Entry_ * *)&gSndFxPresets)[i]->unk4) {
            max = ((Entry_ * *)&gSndFxPresets)[i]->unk4;
        }
    }
    return max;
}


s32 SndFx_SelectPreset(s32 arg0) {
    SndAuxBus *mb;

    if (arg0 >= gSndFxPresetCount) {
        return 1;
    } else {
        SndFx_SetParams(((s32 *)&gSndFxPresets)[arg0]);
        mb = ((SndSynth *)D_8014C6D0)->auxBus;
        alFxParam(mb->fx, 1, (s32)mb);
        return 0;
    }
}
