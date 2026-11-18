#ifndef PAK_H
#define PAK_H

/* Low-level pak states returned by Pak_Open and Pak_GetState (sysPakGetState). */
#define PAK_STATE_OK 0
#define PAK_STATE_NONE 1       /* no pak, or an unusable one */
#define PAK_STATE_CHANGED 2    /* a different pak was inserted */
#define PAK_STATE_NO_FILE 4    /* pak is fine but has no save file */
#define PAK_STATE_FULL 8       /* no free pages or directory entries */
#define PAK_STATE_UNK10 0x10   /* tested by Pak_GetStatus, never returned */
#define PAK_STATE_DAMAGED 0x20 /* corrupt id area; Pak_Repair can fix it */
#define PAK_STATE_UNK40 0x40   /* tested by Pak_GetStatus, never returned */
#define PAK_STATE_RUMBLE 0x80  /* a Rumble Pak is inserted */

/* Menu-level pak status returned by Pak_GetStatus (one per controller port). */
#define PAK_STATUS_OK 0
#define PAK_STATUS_NONE 1
#define PAK_STATUS_CHANGED 2
#define PAK_STATUS_NO_FILE 3
#define PAK_STATUS_DAMAGED 4 /* also for a bad port or an unknown state */
#define PAK_STATUS_UNK6 6
#define PAK_STATUS_FULL 7
#define PAK_STATUS_UNK8 8    /* gPakStatus bit 0x40, or PAK_STATE_UNK40 */
#define PAK_STATUS_RUMBLE 9

#endif
