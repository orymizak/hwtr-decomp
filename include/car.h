#ifndef CAR_H
#define CAR_H

/* Car state (one per player/AI, array of 0x77C-byte entries at D_80118630).
 * Merged from the partial per-unit car types; fields named unkXXX/kXXX/vXXX/bXXX are not
 * identified yet. Every offset is fixed by the matching build. */

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 m[3][3]; } Mtx3f;

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

/* Value with limits (0x1C). */
typedef struct {
    /* 0x00 */ f32 cur;
    /* 0x04 */ u8 pad4[0x10];
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
    /* 0x02A */ u8 pad02A[0x4A];
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
    /* 0x0E4 */ u8 pad0E4[0x30];
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
    /* 0x430 */ u8 pad430[0x4B];
    /* 0x47B */ u8 unk47B;
    /* 0x47C */ Vec3f v47C;
    /* 0x488 */ Vec3f v488;
    /* 0x494 */ Vec3f v494;
    /* 0x4A0 */ s32 unk4A0;
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
    /* 0x534 */ u8 pad534[0x8];
    /* 0x53C */ f32 k53C;
    /* 0x540 */ u32 w540;
    /* 0x544 */ u8 pad544[0x98];
    /* 0x5DC */ Vec3f center;
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 unk5EC;
    /* 0x5F0 */ f32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0x18];
    /* 0x60C */ f32 unk60C;
    /* 0x610 */ f32 unk610;
    /* 0x614 */ f32 unk614;
    /* 0x618 */ f32 unk618;
    /* 0x61C */ f32 unk61C;
    /* 0x620 */ f32 unk620;
    /* 0x624 */ f32 unk624;
    /* 0x628 */ f32 unk628;
    /* 0x62C */ f32 unk62C;
    /* 0x630 */ s32 unk630;
    /* 0x634 */ u8 pad634[0x88];
    /* 0x6BC */ u8 pickupFx[5];
    /* 0x6C1 */ u8 unk6C1;
    /* 0x6C2 */ u8 pad6C2[0x1];
    /* 0x6C3 */ u8 unk6C3;
    /* 0x6C4 */ u8 pad6C4[0x8];
    /* 0x6CC */ u8 n6CC;
    /* 0x6CD */ u8 unk6CD;
    /* 0x6CE */ u8 pad6CE[0xA];
    /* 0x6D8 */ s32 state;
    /* 0x6DC */ u8 pad6DC[0x34];
    /* 0x710 */ u8 unk710;
    /* 0x711 */ u8 pad711[0x3];
    /* 0x714 */ Vec3f planePt;
    /* 0x720 */ Vec3f planeN;
    /* 0x72C */ f32 unk72C;
    /* 0x730 */ u8 pad730[0x14];
    /* 0x744 */ u8 b744;
    /* 0x745 */ u8 pad745[0x13];
    /* 0x758 */ void *roomObj;
    /* 0x75C */ u32 unk75C;
    /* 0x760 */ u32 unk760;
    /* 0x764 */ u32 unk764;
    /* 0x768 */ u8 unk768;
    /* 0x769 */ u8 unk769;
    /* 0x76A */ u8 pad76A[0x2];
    /* 0x76C */ s32 unk76C;
    /* 0x770 */ s32 unk770;
    /* 0x774 */ s32 unk774;
    /* 0x778 */ u8 b778;
    /* 0x779 */ u8 pad779[0x3];
} Car; /* size = 0x77C */

extern Car D_80118630[];

#endif
