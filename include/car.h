#ifndef CAR_H
#define CAR_H

/* Car state (one per player/AI, array of 0x77C-byte entries, gCars).
 * Merged from the partial per-unit car types; fields named unkXXX/kXXX/vXXX/bXXX are not
 * identified yet. Every offset is fixed by the matching build. */

#include "math.h"

/* Wheel/contact point (0x70). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 w;
    /* 0x10 */ f32 h;
    /* 0x14 */ u8 flags;
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ Vec3f dir;
    /* 0x24 */ Vec3f world;
    /* 0x30 */ u8 contact;
    /* 0x31 */ u8 c31;
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ Vec3f n;
    /* 0x40 */ Vec3f pt;
    /* 0x4C */ Vec3f vel;
    /* 0x58 */ u8 pad58[8];
    /* 0x60 */ u8 b60;
    /* 0x61 */ u8 pad61[3];
    /* 0x64 */ f32 h64;
    /* 0x68 */ f32 ang;
    /* 0x6C */ f32 rate;
} CarPoint; /* size = 0x70 */

/* Room-system node and the object that links the car into rooms (see unit_39C90). */
typedef struct CarRoomNode {
    /* 0x0 */ s32 key;
    /* 0x4 */ struct CarRoomNode *prev;
    /* 0x8 */ struct CarRoomNode *next;
    /* 0xC */ u16 cnt;
} CarRoomNode;
typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ u16 *rooms;
    /* 0x58 */ u8 pad58[0x18];
    /* 0x70 */ CarRoomNode *list;
} CarRoomObj;

/* Value with limits (0x1C). */
typedef struct {
    /* 0x00 */ f32 cur;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ u8 b8;
    /* 0x09 */ u8 pad9[0xB];
    /* 0x14 */ f32 hi;
    /* 0x18 */ f32 lo;
} CarRange;

typedef struct Car {
    /* 0x000 */ u8 id; /* player/car index */
    /* 0x001 */ u8 pad001[0x3];
    /* 0x004 */ u32 flags;
    /* 0x008 */ u32 flags2;
    /* 0x00C */ u8 unk00C;
    /* 0x00D */ u8 pad00D[0x3];
    /* 0x010 */ f32 yaw;
    /* 0x014 */ f32 unk014;
    /* 0x018 */ f32 unk018;
    /* 0x01C */ u8 pad01C[0x8];
    /* 0x024 */ u8 unk024;
    /* 0x025 */ u8 unk025;
    /* 0x026 */ u8 unk026;
    /* 0x027 */ u8 unk027;
    /* 0x028 */ u8 unk028;
    /* 0x029 */ u8 unk029;
    /* 0x02A */ u8 pad02A[0x2];
    /* 0x02C */ Mtx3f m2C;
    /* 0x050 */ u8 pad050[0x24];
    /* 0x074 */ f32 scale;
    /* 0x078 */ f32 invScale;
    /* 0x07C */ f32 topSpeed;
    /* 0x080 */ Vec3f up;
    /* 0x08C */ Vec3f v8C;
    /* 0x098 */ Vec3f pos;
    /* 0x0A4 */ Vec3f impulse;
    /* 0x0B0 */ Vec3f vel;
    /* 0x0BC */ f32 fBC;
    /* 0x0C0 */ Mtx3f rot;
    /* 0x0E4 */ Vec3f vE4;
    /* 0x0F0 */ u8 pad0F0[0x24];
    /* 0x114 */ Vec3f angVel;
    /* 0x120 */ u8 pad120[0x4];
    /* 0x124 */ Vec3f force;
    /* 0x130 */ Vec3f torque;
    /* 0x13C */ u8 b13C;
    /* 0x13D */ u8 pad13D[0x7];
    /* 0x144 */ CarPoint points[6];
    /* 0x3E4 */ u8 nPoints;
    /* 0x3E5 */ u8 pad3E5[0x2];
    /* 0x3E7 */ u8 cnt3E7;
    /* 0x3E8 */ u8 cnt3E8;
    /* 0x3E9 */ u8 pad3E9[0x3];
    /* 0x3EC */ CarRange r3EC;
    /* 0x408 */ u8 pad408[0x24];
    /* 0x42C */ f32 k42C;
    /* 0x430 */ u8 pad430[0x10];
    /* 0x440 */ s32 unk440;
    /* 0x444 */ u8 checkpoints[12]; /* passed flags per checkpoint */
    /* 0x450 */ u32 lapStart[8]; /* race time at the start of each lap */
    /* 0x470 */ u32 bestLap;
    /* 0x474 */ u32 unk474;
    /* 0x478 */ u8 lap;
    /* 0x479 */ u8 checkpoint; /* next checkpoint index */
    /* 0x47A */ u8 lapsDone; /* completed laps shown on the HUD counter */
    /* 0x47B */ u8 finished; /* set when the last lap is completed */
    /* 0x47C */ Vec3f v47C;
    /* 0x488 */ Vec3f v488;
    /* 0x494 */ Vec3f v494;
    /* 0x4A0 */ u32 unk4A0;
    /* 0x4A4 */ s32 unk4A4;
    /* 0x4A8 */ s32 timerLast;
    /* 0x4AC */ u32 timerAcc;
    /* 0x4B0 */ s32 unk4B0;
    /* 0x4B4 */ u8 timerFlag;
    /* 0x4B5 */ u8 pad4B5[0x3];
    /* 0x4B8 */ u8 b4B8;
    /* 0x4B9 */ u8 unk4B9;
    /* 0x4BA */ u8 pad4BA[0x2];
    /* 0x4BC */ u32 unk4BC;
    /* 0x4C0 */ u8 b4C0;
    /* 0x4C1 */ u8 pad4C1[0x3];
    /* 0x4C4 */ f32 f4C4;
    /* 0x4C8 */ u8 col[4];
    /* 0x4CC */ f32 k4CC;
    /* 0x4D0 */ f32 k[24];
    /* 0x530 */ f32 k530;
    /* 0x534 */ f32 k534;
    /* 0x538 */ f32 k538;
    /* 0x53C */ f32 k53C;
    /* 0x540 */ u32 w540;
    /* 0x544 */ u8 pad544[0x98];
    /* 0x5DC */ Vec3f center;
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 unk5EC;
    /* 0x5F0 */ f32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0xC];
    /* 0x600 */ Vec3f savedPos; /* respawn snapshot */
    /* 0x60C */ Mtx3f savedRot;
    /* 0x630 */ s32 savedRoom;
    /* 0x634 */ u8 savedState[0x88];
    /* 0x6BC */ u8 pickupFx[5];
    /* 0x6C1 */ u8 unk6C1;
    /* 0x6C2 */ u8 pad6C2[0x1];
    /* 0x6C3 */ u8 unk6C3;
    /* 0x6C4 */ u8 wrongWay; /* set after driving the wrong way for 500 ms */
    /* 0x6C5 */ u8 pad6C5[0x3];
    /* 0x6C8 */ u32 wrongWayTime;
    /* 0x6CC */ u8 n6CC;
    /* 0x6CD */ u8 unk6CD;
    /* 0x6CE */ u8 pad6CE[0xA];
    /* 0x6D8 */ s32 state;
    /* 0x6DC */ s32 unk6DC;
    /* 0x6E0 */ u8 pad6E0[0x30];
    /* 0x710 */ u8 unk710;
    /* 0x711 */ u8 pad711[0x3];
    /* 0x714 */ Vec3f planePt;
    /* 0x720 */ Vec3f planeN;
    /* 0x72C */ f32 unk72C;
    /* 0x730 */ u8 pad730[0x14];
    /* 0x744 */ u8 b744;
    /* 0x745 */ u8 pad745[0x3];
    /* 0x748 */ f32 unk748;
    /* 0x74C */ f32 unk74C;
    /* 0x750 */ f32 unk750;
    /* 0x754 */ u8 pad754[0x4];
    /* 0x758 */ CarRoomObj *roomObj;
    /* 0x75C */ u32 unk75C;
    /* 0x760 */ u32 unk760;
    /* 0x764 */ u32 unk764;
    /* 0x768 */ u8 unk768;
    /* 0x769 */ u8 unk769;
    /* 0x76A */ u8 pad76A[0x2];
    /* 0x76C */ u32 unk76C;
    /* 0x770 */ s32 unk770;
    /* 0x774 */ s32 unk774;
    /* 0x778 */ u8 b778;
    /* 0x779 */ u8 pad779[0x3];
} Car; /* size = 0x77C */

extern Car gCars[];

Car *Car_Get(u8 i);

#endif
