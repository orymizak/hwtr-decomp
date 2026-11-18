#include "common.h"
#include "include_asm.h"
s32 Jukebox_GetArtist(s32);
s32 Jukebox_GetTitle(s32);
void PauseUi_DrawText(s32, s32, s32, s32, s32, s32);
void PauseUi_ResetTextBuffer(void);
void PauseUi_Load(s32);
void PauseUi_FadeIn(void);
void PauseUi_Unload(s32);
    /* TODO (Clean version):
    Replace temporary decomp structures with documented layout:
    typedef struct {
        s16 id;
        s16 x;
    } IdEntry;

    typedef struct {
        u8 pad00[0xC];
        IdEntry *ids;
        u8 pad10[3];
        s8 count;
    } IdTable;
    */
typedef struct { s16 id; s16 x; } Id4_;
typedef struct { u8 pad[0xC]; Id4_ *ids; u8 pad2[3]; s8 n; } IdTab_;
extern s32 gPauseRepeatLeft;
extern s32 gPauseRepeatRight;
extern void (*audSfxPlay)(s32, s32, f32, f32);
u8 Volume_StepUp(u8);
extern u8 (*sysInputIsKeyHeld)(s32, s32);
extern u32 gPauseButtons;
extern IdTab_ *gPauseCurState;
extern u8 (*sysInputIsPad1Connected)(void);
s32 PauseUi_BeginFrame();
s32 PauseUi_EndFrame();
s32 PauseUi_DrawMenu(s32,s32);
s32 Fsm_Trigger(void *,s32);
extern u8 gPauseFsm;

extern s8 gPausePlayer;
s32 Pause_RequestQuit();
s32 Pause_RequestRestart();
s32 PauseUi_Animate(s32);
extern s32 gPauseConfirmMenu;
s32 PauseUi_Open(s32);
extern s8 D_800FA240;
extern s8 D_800FA241;
extern s32 gPauseMainMenu;
extern s32 gPauseOptionsMenu;
extern s32 gPauseAudioMenu;
void *Save_GetActiveProfile();
extern s32 (*sysAudioSetSfxVolume)(s32);
u8 Volume_StepDown(u8);
extern s32 (*sysAudioSetVoiceVolume)(s32);
extern s32 (*sysAudioSetMusicVolume)(s32);
extern s32 gPauseJukeboxMenu;
extern s32 (*audMusicPlayTrack)(s32);
extern s32 (*audMusicGetTrack)();
extern s32 (*audMusicStop)();

s32 Pause_StateHasEvent(s16 id) {
    Id4_ *p = gPauseCurState->ids;
    u8 i;

    for (i = 0; i < gPauseCurState->n; i++, p++) {
        if (p->id == id) {
            return 1;
        }
    }
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_6C240", Pause_Input);

    /* TODO (Clean version):
    Indirect call via 'fp' is a matching artifact to pin sysInputIsKeyHeld in a register.
    Logic performs edge detection for buttons 0x10 and 0x11 (bits 1 and 4 represent newly pressed state):
    u32 r = 0;
    s32 pad = ((u8)gPausePlayer != 1);
    if (sysInputIsKeyHeld(pad, 0x10)) r |= 0x1;
    if (sysInputIsKeyHeld(pad, 0x11)) r |= 0x8;
    if ((r & 0x1) != (gPauseButtons & 0x1)) r |= 0x2;
    if ((r & 0x8) != (gPauseButtons & 0x8)) r |= 0x10;
    gPauseButtons = r;
    return r;
    */
u32 Pause_ReadLeftRight(void) {
    u8 (**fp)(s32, s32);
    s32 pad;
    u32 r;
    u32 old;

    pad = (u8)gPausePlayer != 1;
    fp = &sysInputIsKeyHeld;
    r = (*fp)(pad, 0x10) != 0;
    if ((*fp)(pad, 0x11)) {
        r |= 8;
    }
    old = gPauseButtons;
    r = ((r & 1) != (old & 1)) ? r | 2 : r;
    r = ((r & 8) != (old & 8)) ? r | 0x10 : r;
    gPauseButtons = r;
    return r;
}

void Pause_SetPlayer(s8 arg0) {
    gPausePlayer = arg0;
}

void Pause_Quit(void) {
    Pause_RequestQuit();
}

void Pause_Restart(void) {
    Pause_RequestRestart();
}

void Pause_Advance(void) {
    Fsm_Trigger(&gPauseFsm, 0);
}

void Confirm_DrawYes(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseConfirmMenu, 1);
    PauseUi_EndFrame();
}

void Confirm_DrawNo(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseConfirmMenu, 0);
    PauseUi_EndFrame();
}

void Confirm_Animate(void) {
    PauseUi_Animate(gPauseConfirmMenu);
}

void Confirm_Open(void) {
    PauseUi_Open(gPauseConfirmMenu);
}

    /* TODO (Clean version):
    (&gPauseMainMenu)[i] accesses contiguous global variables gPauseMainMenu..gPauseConfirmMenu.
    Declare as an array:
    extern s32 gPauseMainMenu[5];
    PauseUi_Load(gPauseMainMenu[i]);
    */
void Pause_Load(void) {
    u8 i;

    PauseUi_ResetTextBuffer();
    for (i = 0; i < 5; i++) {
        PauseUi_Load((&gPauseMainMenu)[i]);
    }
    PauseUi_FadeIn();
}

void Pause_LoadNop(void) {

}

void Pause_Unload(void) {
    u8 i;

    for (i = 0; i < 5; i++) {
        PauseUi_Unload((&gPauseMainMenu)[i]);
    }
}


void Pause_UnloadNop(void) {

}

void Pause_ClearRequests(void) {
    D_800FA241 = 0;
    D_800FA240 = 0;
}

void PauseMain_Open(void) {
    PauseUi_Open(gPauseMainMenu);
}

void PauseMain_Animate(void) {
    PauseUi_Animate(gPauseMainMenu);
}

void PauseMain_OnContinue(void) {
    if (sysInputIsPad1Connected() != 0) {
        Fsm_Trigger(&gPauseFsm, 7);
    } else {
        Fsm_Trigger(&gPauseFsm, 8);
    }
}


void PauseMain_DrawContinue(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseMainMenu, 0);
    PauseUi_EndFrame();
}

void PauseMain_DrawRestart(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseMainMenu, 1);
    PauseUi_EndFrame();
}

void PauseMain_DrawOptions(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseMainMenu, 2);
    PauseUi_EndFrame();
}

void PauseMain_DrawQuit(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseMainMenu, 3);
    PauseUi_EndFrame();
}

void PauseOptions_Open(void) {
    PauseUi_Open(gPauseOptionsMenu);
}

void PauseOptions_Animate(void) {
    PauseUi_Animate(gPauseOptionsMenu);
}

void PauseOptions_DrawAudio(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseOptionsMenu, 0);
    PauseUi_EndFrame();
}

void PauseOptions_DrawJukebox(void) {
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseOptionsMenu, 1);
    PauseUi_EndFrame();
}

void PauseAudio_Open(void) {
    PauseUi_Open(gPauseAudioMenu);
}

void PauseAudio_Animate(void) {
    PauseUi_Animate(gPauseAudioMenu);
}

/* Same edge detection as Pause_ReadLeftRight; repeats buttons 0x10/0x11 as menu inputs 0xB/0xA. */
void Pause_RepeatLeftRight(void) {
    s32 pad;
    u32 r;
    u32 old;

    pad = (u8)gPausePlayer != 1;
    r = sysInputIsKeyHeld(pad, 0x10) != 0;
    if (sysInputIsKeyHeld(pad, 0x11)) {
        r |= 8;
    }
    old = gPauseButtons;
    r = ((r & 1) != (old & 1)) ? r | 2 : r;
    r = ((r & 8) != (old & 8)) ? r | 0x10 : r;
    gPauseButtons = r;
    if ((r & 9) == 1) {
        if (r & 2) {
            Fsm_Trigger(&gPauseFsm, 0xB);
            gPauseRepeatLeft = -8;
        } else if (gPauseRepeatLeft++ >= 3) {
            Fsm_Trigger(&gPauseFsm, 0xB);
            gPauseRepeatLeft = 0;
        }
    } else if ((r & 9) == 8) {
        if (r & 0x10) {
            Fsm_Trigger(&gPauseFsm, 0xA);
            gPauseRepeatRight = -8;
        } else if (gPauseRepeatRight++ >= 3) {
            Fsm_Trigger(&gPauseFsm, 0xA);
            gPauseRepeatRight = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_6C240", PauseAudio_DrawSfx);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", PauseAudio_DrawVoice);

INCLUDE_ASM("asm/nonmatchings/unit_6C240", PauseAudio_DrawMusic);

/* Raises a 0-255 level to the next 10% step; plays sound 0x31 on change. */
u8 Volume_StepUp(u8 arg0) {
    s32 v;
    s32 cur;
    s32 target;

    v = arg0;
    cur = v * 100 / 255;
    cur = cur / 10 * 10;
    if (cur >= 100) {
        return 255;
    }
    target = cur + 10;
    if (target >= 100) {
        audSfxPlay(0x31, 1, 1.0f, 1.0f);
        return 255;
    }
    while (cur < target) {
        v++;
        cur = v * 100 / 255;
        cur = cur / 10 * 10;
    }
    /* TODO (Bug check): 'v < 0' is dead code; 'v' starts as u8 and is only incremented. */
    if (v < 0) {
        return 0;
    }
    if (v > 255) {
        return 255;
    }
    audSfxPlay(0x31, 1, 1.0f, 1.0f);
    return v;
}

/* Lowers a 0-255 level to the previous 10% step; plays sound 0x31 on change. */
u8 Volume_StepDown(u8 arg0) {
    s32 v;
    s32 cur;
    s32 target;

    v = arg0;
    cur = v * 100 / 255;
    cur = cur / 10 * 10;
    if (cur <= 0) {
        return 0;
    }
    target = cur - 10;
    if (target <= 0) {
        audSfxPlay(0x31, 1, 1.0f, 1.0f);
        return 0;
    }
    while (target < cur) {
        v--;
        cur = v * 100 / 255;
        cur = cur / 10 * 10;
    }
    if (v <= 0) {
        return 0;
    }
    /* TODO (Bug check): 'v >= 255' is unreachable; 'v' is only decremented from arg0 <= 255. */
    if (v >= 255) {
        return 255;
    }
    audSfxPlay(0x31, 1, 1.0f, 1.0f);
    return v;
}

    /* TODO (Clean version):
    Offsets 0x87C, 0x87D, and 0x880 into Save_GetActiveProfile() represent audio settings.
    Define an options structure:
    typedef struct {
        u8 pad00[0x87C];
        u8 bgm_volume;    // 0x87C
        u8 sfx_volume;    // 0x87D
        u8 pad87E[2];
        u8 voice_volume;  // 0x880
    } GameSettings;
    GameSettings *opt = Save_GetActiveProfile();
    */
void PauseAudio_SfxUp(void) {
    u8 *opt = Save_GetActiveProfile();
    u8 cur = opt[0x87C];
    u8 next;

    if (cur != 0xFF) {
        next = Volume_StepUp(cur);
        opt[0x87C] = next;
        sysAudioSetSfxVolume(next);
    }
}

void PauseAudio_SfxDown(void) {
    u8 *opt = Save_GetActiveProfile();
    u8 cur = opt[0x87C];
    u8 next;

    if (cur != 0) {
        next = Volume_StepDown(cur);
        opt[0x87C] = next;
        sysAudioSetSfxVolume(next);
    }
}

void PauseAudio_VoiceUp(void) {
    u8 *opt = Save_GetActiveProfile();
    u8 cur = opt[0x880];
    u8 next;

    if (cur != 0xFF) {
        next = Volume_StepUp(cur);
        opt[0x880] = next;
        sysAudioSetVoiceVolume(next);
    }
}

void PauseAudio_VoiceDown(void) {
    u8 *opt = Save_GetActiveProfile();
    u8 cur = opt[0x880];
    u8 next;

    if (cur != 0) {
        next = Volume_StepDown(cur);
        opt[0x880] = next;
        sysAudioSetVoiceVolume(next);
    }
}

void PauseAudio_MusicUp(void) {
    u8 *opt = Save_GetActiveProfile();
    u8 cur = opt[0x87D];
    u8 next;

    if (cur != 0xFF) {
        next = Volume_StepUp(cur);
        opt[0x87D] = next;
        sysAudioSetMusicVolume(next);
    }
}

void PauseAudio_MusicDown(void) {
    u8 *opt = Save_GetActiveProfile();
    u8 cur = opt[0x87D];
    u8 next;

    if (cur != 0) {
        next = Volume_StepDown(cur);
        opt[0x87D] = next;
        sysAudioSetMusicVolume(next);
    }
}

void Jukebox_Open(void) {
    PauseUi_Open(gPauseJukeboxMenu);
    audMusicPlayTrack(audMusicGetTrack());
}

void Jukebox_Animate(void) {
    PauseUi_Animate(gPauseJukeboxMenu);
}

    /* TODO (Clean version):
    'fp' is a matching workaround to cache audMusicGetTrack in a register across calls:
    s32 val = audMusicGetTrack();
    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseJukeboxMenu, 0);
    PauseUi_DrawText(Jukebox_GetArtist(val), 160, 155, 255, 255, 255);
    PauseUi_DrawText(Jukebox_GetTitle(val), 160, 175, 255, 255, 255);
    PauseUi_EndFrame();
    */
void Jukebox_DrawPrev(void) {
    s32 (**fp)() = &audMusicGetTrack;

    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseJukeboxMenu, 0);
    PauseUi_DrawText(Jukebox_GetArtist((*fp)()), 160, 155, 255, 255, 255);
    PauseUi_DrawText(Jukebox_GetTitle((*fp)()), 160, 175, 255, 255, 255);
    PauseUi_EndFrame();
}


void Jukebox_DrawNext(void) {
    s32 (**fp)() = &audMusicGetTrack;

    PauseUi_BeginFrame();
    PauseUi_DrawMenu(gPauseJukeboxMenu, 1);
    PauseUi_DrawText(Jukebox_GetArtist((*fp)()), 160, 155, 255, 255, 255);
    PauseUi_DrawText(Jukebox_GetTitle((*fp)()), 160, 175, 255, 255, 255);
    PauseUi_EndFrame();
}


void Jukebox_StopMusic(void) {
    audMusicStop();
}

void Jukebox_NextTrack(void) {
    s32 next;

    next = audMusicGetTrack() + 1;
    /* TODO (Clean version): 'next &= -((u8)next < 7)' is branchless wrap-around: next = (next >= 7) ? 0 : next; */
    next &= -((u8)next < 7);
    audMusicStop();
    audMusicPlayTrack(next & 0xFF);
}

void Jukebox_PrevTrack(void) {
    s32 t;
    u8 r;

    t = audMusicGetTrack();
    if ((t & 0xFF) == 0) {
        r = 6;
    } else {
        r = t - 1;
    }
    audMusicStop();
    audMusicPlayTrack(r & 0xFF);
}
