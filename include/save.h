#ifndef SAVE_H
#define SAVE_H

/* Game save (0x88C bytes, gActiveSave): record tables, then the options and the ids of the
   two active player profiles. Only the tail is mapped so far. */
typedef struct {
    /* 0x000 */ u8 records[0x878];
    /* 0x878 */ s32 gameMode;   /* 0-4, selected with GameMode_Prev/Next */
    /* 0x87C */ u8 sfxVolume;
    /* 0x87D */ u8 musicVolume;
    /* 0x87E */ u8 option0;     /* first Options entry (0-255), copied to gRaceSetup.f9C */
    /* 0x87F */ u8 pad87F;
    /* 0x880 */ u8 voiceVolume;
    /* 0x881 */ u8 pad881[3];
    /* 0x884 */ s32 player1Id;
    /* 0x888 */ s32 player2Id;
} GameSave; /* size = 0x88C */

#endif
