#ifndef RACE_H
#define RACE_H

/* One racer of a race setup (0x14). */
typedef struct {
    /* 0x00 */ char name[0xC]; /* car model name */
    /* 0x0C */ s32 kind;       /* 1 = human player, 0 = AI */
    /* 0x10 */ u8 car;         /* car index (gCarModelNames) */
    /* 0x11 */ u8 player;      /* human player index */
    /* 0x12 */ u8 grid;        /* starting position */
    /* 0x13 */ u8 p13;
} RacerSetup;

/* Race description built by the menus (gRaceSetup) and passed to Race_Init. */
typedef struct {
    /* 0x00 */ s32 flags;      /* bit 0: demo, bit 2: time limit, 0x10/0x40: cheats, 0x100: subtitle */
    /* 0x04 */ char name[9];   /* track name */
    /* 0x0D */ char sub[9];
    /* 0x16 */ u8 cup;
    /* 0x17 */ u8 track;       /* 1-3 within the cup */
    /* 0x18 */ u8 count;       /* number of racers */
    /* 0x19 */ u8 f19;
    /* 0x1A */ u8 f1A;
    /* 0x1B */ u8 p1B;
    /* 0x1C */ u32 cheats;     /* Profile.cheats of the player (bit 0x200: show FPS) */
    /* 0x20 */ s32 timeLimit;
    /* 0x24 */ RacerSetup p[6];
    /* 0x9C */ u8 f9C;         /* GameSave.option0, or 0xFF */
} RaceSetup;

/* Race result block filled by Race_Shutdown (0x124 bytes are cleared first). */
typedef struct {
    /* 0x00 */ u8 pad0[0x12];
    /* 0x12 */ u8 points;
    /* 0x13 */ u8 pad13[0x19];
} RacerResult; /* size = 0x2C */
typedef struct {
    /* 0x00 */ s32 outcome;    /* gRaceOutcome */
    /* 0x04 */ u8 pad4[0x2C];
    /* 0x30 */ RacerResult r[1]; /* gRacerCount entries */
} RaceResult;
#define RACE_RESULT_SIZE 0x124

#endif
