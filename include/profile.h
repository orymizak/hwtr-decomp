#ifndef PROFILE_H
#define PROFILE_H

/* Player profile (0x140 bytes; gProfileP1/gProfileP2 point at one). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 id;             /* copied to GameSave.player1Id/player2Id */
    /* 0x08 */ char name[12];      /* default "Player %d" */
    /* 0x14 */ u32 carUnlocks[2];  /* one bit per car (gCarModelNames); default 0xB48EE52E, 0x10B */
    /* 0x1C */ u32 trackUnlocks;   /* one bit per track; default 0xE3F */
    /* 0x20 */ u8 car;             /* selected car, default 0x1D */
    /* 0x21 */ u8 track;           /* selected track */
    /* 0x22 */ u8 cup;             /* championship: current cup (0-2) */
    /* 0x23 */ u8 cupRace;         /* championship: race within the cup (0-5) */
    /* 0x24 */ s32 cupPoints[6];   /* championship points of the six racers */
    /* 0x3C */ u32 cheats;         /* cheat bits (Config_CheckCheatCode, Cheat_Get*Flag) */
    /* 0x40 */ u8 bindings[0x100]; /* Input_SaveBindings */
} Profile; /* size = 0x140 */

#endif
