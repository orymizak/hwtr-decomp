#include "common.h"
#include "include_asm.h"

typedef enum {
    FILE_TYPE_RAW,
    FILE_TYPE_LZ,
    FILE_TYPE_LZ_CHUNKED
} FileType;

typedef struct {
    /* 0x00 */ char *name;
    /* 0x04 */ u8 type;
    /* 0x08 */ u32 storedSize;
    /* 0x0C */ u32 size;
    /* 0x10 */ u32 firstChunkSize;
    /* 0x14 */ u32 romStart;
    /* 0x18 */ u32 romEnd;
} FileEntry; /* size = 0x1C */

typedef struct {
    /* 0x00 */ u8 unk00[8];
    /* 0x08 */ s32 status;
    /* 0x0C */ u8 unk0C[4];
} PakHandle; /* size = 0x10 */

typedef struct {
    /* 0x00 */ FileEntry *table;
    /* 0x04 */ s32 count;
    /* 0x08 */ PakHandle *handles[4];
    /* 0x18 */ u8 initialized[4];
} FileSystem; /* size = 0x1C */

typedef struct {
    /* 0x00 */ u8 data[0xDA0];
} SaveBlock; /* size = 0xDA0 */

typedef struct {
    /* 0x00 */ u8 ioMesg[0x18];
    /* 0x18 */ u8 queue[0x18];
    /* 0x30 */ void *msg;
    /* 0x34 */ u8 unk34[4];
    /* 0x38 */ void *buffer;
    /* 0x3C */ void *dst;
    /* 0x40 */ u8 unk40[4];
    /* 0x44 */ u8 active;
    /* 0x45 */ u8 type;
} AsyncRead; /* size = 0x48 */

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ u8 unk04[0x50];
} PakState; /* size = 0x54 */

extern PakState D_800F03F8[];
extern u8 D_80102220[];

FileEntry gFileTable[] = {
    { "4X4.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xA2A70, 0xA2B20 },
    { "ACTNFNT.OVL", FILE_TYPE_LZ, 0x2937, 0xE350, 0x0, 0xA2B20, 0xA5460 },
    { "ACTNOVL1.OVL", FILE_TYPE_LZ, 0x33C5, 0x9CA8, 0x0, 0xA5460, 0xA8830 },
    { "ACTNOVL2.OVL", FILE_TYPE_LZ, 0x3118, 0x89B0, 0x0, 0xA8830, 0xAB950 },
    { "AIGRP.PRM", FILE_TYPE_RAW, 0x8, 0x8, 0x0, 0xAB950, 0xAB960 },
    { "ARROWD.OBJ", FILE_TYPE_LZ, 0xA6, 0x134, 0x0, 0xAB960, 0xABA10 },
    { "ARROWL.OBJ", FILE_TYPE_LZ, 0xA4, 0x134, 0x0, 0xABA10, 0xABAC0 },
    { "ARROWR.OBJ", FILE_TYPE_LZ, 0xA5, 0x134, 0x0, 0xABAC0, 0xABB70 },
    { "ARROWU.OBJ", FILE_TYPE_LZ, 0xAC, 0x134, 0x0, 0xABB70, 0xABC20 },
    { "BISECTOR.CAR", FILE_TYPE_LZ, 0x7D79, 0xEA58, 0x0, 0xABC20, 0xB39A0 },
    { "BISECTOR.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB39A0, 0xB3B00 },
    { "BLACK.OVL", FILE_TYPE_LZ, 0x32DB, 0xB560, 0x0, 0xB3B00, 0xB6DE0 },
    { "BRAKES.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xB6DE0, 0xB6E90 },
    { "BUTTONS.OVL", FILE_TYPE_LZ, 0x376D, 0xF1D8, 0x0, 0xB6E90, 0xBA600 },
    { "CATAPULT.CAR", FILE_TYPE_LZ, 0x7CB5, 0xD3A8, 0x0, 0xBA600, 0xC22C0 },
    { "CATAPULT.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xC22C0, 0xC2420 },
    { "CCCSND.BFX", FILE_TYPE_RAW, 0xFAC, 0xFAC, 0x0, 0xC2420, 0xC33D0 },
    { "CCCSND.BIN", FILE_TYPE_RAW, 0x1, 0x1, 0x0, 0xC33D0, 0xC33E0 },
    { "CCCSND.PTR", FILE_TYPE_RAW, 0x5C78, 0x5C78, 0x0, 0xC33E0, 0xC9060 },
    { "CCCSND.WBK", FILE_TYPE_RAW, 0x2CAB9E, 0x2CAB9E, 0x0, 0xC9060, 0x393C00 },
    { "CLOUD00.TEX", FILE_TYPE_RAW, 0x200, 0x200, 0x0, 0x393C00, 0x393E00 },
    { "CLOUD01.TEX", FILE_TYPE_RAW, 0x200, 0x200, 0x0, 0x393E00, 0x394000 },
    { "CLOUD02.TEX", FILE_TYPE_RAW, 0x200, 0x200, 0x0, 0x394000, 0x394200 },
    { "CLOUD03.TEX", FILE_TYPE_RAW, 0x200, 0x200, 0x0, 0x394200, 0x394400 },
    { "CLOUD04.TEX", FILE_TYPE_RAW, 0x200, 0x200, 0x0, 0x394400, 0x394600 },
    { "DEORA.CAR", FILE_TYPE_LZ, 0x903B, 0xFEA8, 0x0, 0x394600, 0x39D640 },
    { "DEORA.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x39D640, 0x39D7A0 },
    { "DESERT1.BLD", FILE_TYPE_RAW, 0x44D1, 0x44D1, 0x0, 0x39D7A0, 0x3A1C80 },
    { "DESERT1.OBJ", FILE_TYPE_LZ, 0xFEE, 0x12F8, 0x0, 0x3A1C80, 0x3A2C70 },
    { "DESERT1.SCN", FILE_TYPE_LZ, 0x11483, 0x16218, 0x0, 0x3A2C70, 0x3B4100 },
    { "DESERT1.SKY", FILE_TYPE_LZ, 0x62FA, 0x1069C, 0x0, 0x3B4100, 0x3BA400 },
    { "DESERT1.WLD", FILE_TYPE_LZ, 0x47DE6, 0x66528, 0x0, 0x3BA400, 0x4021F0 },
    { "DESERT2.BLD", FILE_TYPE_RAW, 0x4F82, 0x4F82, 0x0, 0x4021F0, 0x407180 },
    { "DESERT2.OBJ", FILE_TYPE_LZ, 0x10DD, 0x1448, 0x0, 0x407180, 0x408260 },
    { "DESERT2.SCN", FILE_TYPE_LZ, 0xEF0F, 0x13088, 0x0, 0x408260, 0x417170 },
    { "DESERT2.SKY", FILE_TYPE_LZ, 0x42BF, 0x1069C, 0x0, 0x417170, 0x41B430 },
    { "DESERT2.WLD", FILE_TYPE_LZ, 0x437E5, 0x63C68, 0x0, 0x41B430, 0x45EC20 },
    { "DESERT3.BLD", FILE_TYPE_RAW, 0x3460, 0x3460, 0x0, 0x45EC20, 0x462080 },
    { "DESERT3.OBJ", FILE_TYPE_LZ, 0xCAB, 0xF9C, 0x0, 0x462080, 0x462D30 },
    { "DESERT3.SCN", FILE_TYPE_LZ, 0xCF75, 0x112C0, 0x0, 0x462D30, 0x46FCB0 },
    { "DESERT3.SKY", FILE_TYPE_LZ, 0x6D59, 0x1069C, 0x0, 0x46FCB0, 0x476A10 },
    { "DESERT3.WLD", FILE_TYPE_LZ, 0x42304, 0x639C0, 0x0, 0x476A10, 0x4B8D20 },
    { "DOUBLEV.CAR", FILE_TYPE_LZ, 0x97AC, 0xFE88, 0x0, 0x4B8D20, 0x4C24D0 },
    { "DOUBLEV.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x4C24D0, 0x4C2630 },
    { "DRAGSTER.CAR", FILE_TYPE_LZ, 0x7F33, 0xE720, 0x0, 0x4C2630, 0x4CA570 },
    { "DRAGSTER.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x4CA570, 0x4CA6D0 },
    { "EFFECTS.TEX", FILE_TYPE_RAW, 0x148F0, 0x148F0, 0x0, 0x4CA6D0, 0x4DEFC0 },
    { "ENGCARS.CDT", FILE_TYPE_RAW, 0xB21, 0xB21, 0x0, 0x4DEFC0, 0x4DFAF0 },
    { "ENGLISH.HWT", FILE_TYPE_RAW, 0x1635, 0x1635, 0x0, 0x4DFAF0, 0x4E1130 },
    { "ENGNAME.CHM", FILE_TYPE_RAW, 0x6C, 0x6C, 0x0, 0x4E1130, 0x4E11A0 },
    { "ENGPWD.CHM", FILE_TYPE_RAW, 0x5E, 0x5E, 0x0, 0x4E11A0, 0x4E1200 },
    { "ENVMAP1.TEX", FILE_TYPE_RAW, 0x808, 0x808, 0x0, 0x4E1200, 0x4E1A10 },
    { "EVILWEVL.CAR", FILE_TYPE_LZ, 0xAFBB, 0x10AA0, 0x0, 0x4E1A10, 0x4EC9D0 },
    { "EVILWEVL.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x4EC9D0, 0x4ECB30 },
    { "FORM5G.CAR", FILE_TYPE_LZ, 0x6F4C, 0xB3B0, 0x0, 0x4ECB30, 0x4F3A80 },
    { "FORM5G.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x4F3A80, 0x4F3BE0 },
    { "GAUGEB.OBJ", FILE_TYPE_LZ, 0xB0, 0x140, 0x0, 0x4F3BE0, 0x4F3C90 },
    { "GAUGEDB.OBJ", FILE_TYPE_LZ, 0xAD, 0x140, 0x0, 0x4F3C90, 0x4F3D40 },
    { "GAUGER.OBJ", FILE_TYPE_LZ, 0xB0, 0x140, 0x0, 0x4F3D40, 0x4F3DF0 },
    { "GAUGEY.OBJ", FILE_TYPE_LZ, 0xAF, 0x140, 0x0, 0x4F3DF0, 0x4F3EA0 },
    { "GLACIAL1.BLD", FILE_TYPE_RAW, 0x7EA7, 0x7EA7, 0x0, 0x4F3EA0, 0x4FBD50 },
    { "GLACIAL1.OBJ", FILE_TYPE_LZ, 0xEDE, 0x11E8, 0x0, 0x4FBD50, 0x4FCC30 },
    { "GLACIAL1.SCN", FILE_TYPE_LZ, 0x1302A, 0x18E20, 0x0, 0x4FCC30, 0x50FC60 },
    { "GLACIAL1.SKY", FILE_TYPE_LZ, 0x4BCC, 0x1069C, 0x0, 0x50FC60, 0x514830 },
    { "GLACIAL1.WLD", FILE_TYPE_LZ, 0x3E6C5, 0x60390, 0x0, 0x514830, 0x552F00 },
    { "GLACIAL2.BLD", FILE_TYPE_RAW, 0x7139, 0x7139, 0x0, 0x552F00, 0x55A040 },
    { "GLACIAL2.OBJ", FILE_TYPE_LZ, 0xEF4, 0x1294, 0x0, 0x55A040, 0x55AF40 },
    { "GLACIAL2.SCN", FILE_TYPE_LZ, 0x1038E, 0x16480, 0x0, 0x55AF40, 0x56B2D0 },
    { "GLACIAL2.SKY", FILE_TYPE_LZ, 0x4AFF, 0x1069C, 0x0, 0x56B2D0, 0x56FDD0 },
    { "GLACIAL2.WLD", FILE_TYPE_LZ, 0x36D80, 0x57840, 0x0, 0x56FDD0, 0x5A6B50 },
    { "GLACIAL3.BLD", FILE_TYPE_RAW, 0x6F54, 0x6F54, 0x0, 0x5A6B50, 0x5ADAB0 },
    { "GLACIAL3.OBJ", FILE_TYPE_LZ, 0xC2F, 0xFBC, 0x0, 0x5ADAB0, 0x5AE6E0 },
    { "GLACIAL3.SCN", FILE_TYPE_LZ, 0x9E91, 0xC5C8, 0x0, 0x5AE6E0, 0x5B8580 },
    { "GLACIAL3.SKY", FILE_TYPE_LZ, 0x4AFF, 0x1069C, 0x0, 0x5B8580, 0x5BD080 },
    { "GLACIAL3.WLD", FILE_TYPE_LZ, 0x24CD8, 0x420E8, 0x0, 0x5BD080, 0x5E1D60 },
    { "GOCART.CAR", FILE_TYPE_LZ, 0x91E2, 0xE9F8, 0x0, 0x5E1D60, 0x5EAF50 },
    { "GOCART.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x5EAF50, 0x5EB0B0 },
    { "GULCHSTP.CAR", FILE_TYPE_LZ, 0x8E29, 0xF250, 0x0, 0x5EB0B0, 0x5F3EE0 },
    { "GULCHSTP.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x5F3EE0, 0x5F4040 },
    { "GYRO.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0x5F4040, 0x5F40F0 },
    { "HANDLING.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0x5F40F0, 0x5F41A0 },
    { "HAUNTED1.OBJ", FILE_TYPE_LZ, 0xADE, 0xDAC, 0x0, 0x5F41A0, 0x5F4C80 },
    { "HAUNTED2.BLD", FILE_TYPE_RAW, 0x445C, 0x445C, 0x0, 0x5F4C80, 0x5F90E0 },
    { "HAUNTED2.OBJ", FILE_TYPE_LZ, 0x106B, 0x1410, 0x0, 0x5F90E0, 0x5FA150 },
    { "HAUNTED2.SCN", FILE_TYPE_LZ, 0x10097, 0x16890, 0x0, 0x5FA150, 0x60A1F0 },
    { "HAUNTED2.SKY", FILE_TYPE_LZ, 0x5FF6, 0x10658, 0x0, 0x60A1F0, 0x6101F0 },
    { "HAUNTED2.WLD", FILE_TYPE_LZ, 0x490CC, 0x71C08, 0x0, 0x6101F0, 0x6592C0 },
    { "HAUNTED3.BLD", FILE_TYPE_RAW, 0x550F, 0x550F, 0x0, 0x6592C0, 0x65E7D0 },
    { "HAUNTED3.OBJ", FILE_TYPE_LZ, 0xFC3, 0x1380, 0x0, 0x65E7D0, 0x65F7A0 },
    { "HAUNTED3.SCN", FILE_TYPE_LZ, 0x975B, 0xC6EC, 0x0, 0x65F7A0, 0x668F00 },
    { "HAUNTED3.SKY", FILE_TYPE_LZ, 0x5F96, 0x10694, 0x0, 0x668F00, 0x66EEA0 },
    { "HAUNTED3.WLD", FILE_TYPE_LZ, 0x3677A, 0x5F0A8, 0x0, 0x66EEA0, 0x6A5620 },
    { "HW500.CAR", FILE_TYPE_LZ, 0x99D3, 0xF798, 0x0, 0x6A5620, 0x6AF000 },
    { "HW500.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x6AF000, 0x6AF160 },
    { "JETHREAT.CAR", FILE_TYPE_LZ, 0xA027, 0xFA98, 0x0, 0x6AF160, 0x6B9190 },
    { "JETHREAT.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x6B9190, 0x6B92F0 },
    { "KYLE.CAR", FILE_TYPE_LZ, 0x8FF9, 0xECA0, 0x0, 0x6B92F0, 0x6C22F0 },
    { "KYLE.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x6C22F0, 0x6C2450 },
    { "LAKESTER.CAR", FILE_TYPE_LZ, 0x8188, 0xE670, 0x0, 0x6C2450, 0x6CA5E0 },
    { "LAKESTER.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x6CA5E0, 0x6CA740 },
    { "MONGOOSE.CAR", FILE_TYPE_LZ, 0x935C, 0xFD50, 0x0, 0x6CA740, 0x6D3AA0 },
    { "MONGOOSE.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0x6D3AA0, 0x6D3C00 },
    { "MYSTERY.OBJ", FILE_TYPE_LZ, 0x8D5, 0xB24, 0x0, 0x6D3C00, 0x6D44E0 },
    { "N64CPRES.BG", FILE_TYPE_LZ_CHUNKED, 0x283E0, 0x4B200, 0x1D98, 0x6D44E0, 0x6FC8C0 },
    { "N64CRED.BG", FILE_TYPE_LZ_CHUNKED, 0x24FE0, 0x4B200, 0x1EA8, 0x6FC8C0, 0x7218A0 },
    { "N64CRED2.BG", FILE_TYPE_LZ_CHUNKED, 0x26030, 0x4B200, 0x1D38, 0x7218A0, 0x7478D0 },
    { "N64CTRL.BG", FILE_TYPE_LZ_CHUNKED, 0x281A8, 0x4B200, 0x1C98, 0x7478D0, 0x76FA80 },
    { "N64EA.BG", FILE_TYPE_LZ_CHUNKED, 0x13880, 0x4B200, 0xCB8, 0x76FA80, 0x783300 },
    { "N64GAR.BG", FILE_TYPE_LZ_CHUNKED, 0x22280, 0x4B200, 0x16A8, 0x783300, 0x7A5580 },
    { "N64GAR1.BG", FILE_TYPE_LZ_CHUNKED, 0x1A940, 0x4B200, 0xCB0, 0x7A5580, 0x7BFEC0 },
    { "N64HELP0.BG", FILE_TYPE_LZ_CHUNKED, 0x249F0, 0x4B200, 0xD08, 0x7BFEC0, 0x7E48B0 },
    { "N64HELP1.BG", FILE_TYPE_LZ_CHUNKED, 0x22158, 0x4B200, 0x1EB0, 0x7E48B0, 0x806A10 },
    { "N64HELP2.BG", FILE_TYPE_LZ_CHUNKED, 0x29848, 0x4B200, 0x16B0, 0x806A10, 0x830260 },
    { "N64HELP3.BG", FILE_TYPE_LZ_CHUNKED, 0x21708, 0x4B200, 0x1828, 0x830260, 0x851970 },
    { "N64HELP4.BG", FILE_TYPE_LZ_CHUNKED, 0x2D630, 0x4B200, 0xC50, 0x851970, 0x87EFA0 },
    { "N64HELP5.BG", FILE_TYPE_LZ_CHUNKED, 0x29210, 0x4B200, 0x2690, 0x87EFA0, 0x8A81B0 },
    { "N64HELP6.BG", FILE_TYPE_LZ_CHUNKED, 0x16BD0, 0x4B200, 0x418, 0x8A81B0, 0x8BED80 },
    { "N64HELP7.BG", FILE_TYPE_LZ_CHUNKED, 0x26A98, 0x4B200, 0x600, 0x8BED80, 0x8E5820 },
    { "N64HELP8.BG", FILE_TYPE_LZ_CHUNKED, 0x29D88, 0x4B200, 0xB18, 0x8E5820, 0x90F5B0 },
    { "N64HELP9.BG", FILE_TYPE_LZ_CHUNKED, 0x26928, 0x4B200, 0x1050, 0x90F5B0, 0x935EE0 },
    { "N64HIGH.BG", FILE_TYPE_LZ_CHUNKED, 0x28830, 0x4B200, 0x1C00, 0x935EE0, 0x95E710 },
    { "N64HWC.BG", FILE_TYPE_LZ_CHUNKED, 0x22080, 0x4B200, 0x1FF0, 0x95E710, 0x980790 },
    { "N64LEGAL.BG", FILE_TYPE_LZ_CHUNKED, 0x70C0, 0x4B200, 0x130, 0x980790, 0x987850 },
    { "N64LOAD.BG", FILE_TYPE_LZ_CHUNKED, 0x271F0, 0x4B200, 0x1A70, 0x987850, 0x9AEA40 },
    { "N64LOSE.BG", FILE_TYPE_LZ_CHUNKED, 0x2A2F0, 0x4B200, 0x1DD0, 0x9AEA40, 0x9D8D30 },
    { "N64MAIN.BG", FILE_TYPE_LZ_CHUNKED, 0x21F68, 0x4B200, 0x2170, 0x9D8D30, 0x9FACA0 },
    { "N64OPT.BG", FILE_TYPE_LZ_CHUNKED, 0x26718, 0x4B200, 0x1EA8, 0x9FACA0, 0xA213C0 },
    { "N64REG.BG", FILE_TYPE_LZ_CHUNKED, 0x281F8, 0x4B200, 0x1AA8, 0xA213C0, 0xA495C0 },
    { "N64SFS.BG", FILE_TYPE_LZ_CHUNKED, 0xDD10, 0x4B200, 0x348, 0xA495C0, 0xA572D0 },
    { "N64SPL.BG", FILE_TYPE_LZ_CHUNKED, 0x23DC8, 0x4B200, 0x23D8, 0xA572D0, 0xA7B0A0 },
    { "N64UNLOK.BG", FILE_TYPE_LZ_CHUNKED, 0x24C30, 0x4B200, 0x1D90, 0xA7B0A0, 0xA9FCD0 },
    { "N64WIN.BG", FILE_TYPE_LZ_CHUNKED, 0x2DB90, 0x4B200, 0x1C10, 0xA9FCD0, 0xACD860 },
    { "POP1.OBJ", FILE_TYPE_LZ, 0x5A6, 0xE70, 0x0, 0xACD860, 0xACDE10 },
    { "PPASSION.CAR", FILE_TYPE_LZ, 0x7E87, 0xDB78, 0x0, 0xACDE10, 0xAD5CA0 },
    { "PPASSION.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xAD5CA0, 0xAD5E00 },
    { "PTEST1.OBJ", FILE_TYPE_LZ, 0x7B8, 0x964, 0x0, 0xAD5E00, 0xAD65C0 },
    { "PWRPIPES.CAR", FILE_TYPE_LZ, 0x82C4, 0xD620, 0x0, 0xAD65C0, 0xADE890 },
    { "PWRPIPES.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xADE890, 0xADE9F0 },
    { "PWRPSTN.CAR", FILE_TYPE_LZ, 0x9C36, 0xF8D0, 0x0, 0xADE9F0, 0xAE8630 },
    { "PWRPSTN.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xAE8630, 0xAE8790 },
    { "RASH1.CAR", FILE_TYPE_LZ, 0x9087, 0xF2C8, 0x0, 0xAE8790, 0xAF1820 },
    { "RASH1.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xAF1820, 0xAF1980 },
    { "RDROCKET.CAR", FILE_TYPE_LZ, 0x9215, 0xE8A8, 0x0, 0xAF1980, 0xAFABA0 },
    { "RDROCKET.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xAFABA0, 0xAFAD00 },
    { "REDBARON.CAR", FILE_TYPE_LZ, 0x71CB, 0xBDA0, 0x0, 0xAFAD00, 0xB01ED0 },
    { "REDBARON.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB01ED0, 0xB02030 },
    { "RIGOMTR.CAR", FILE_TYPE_LZ, 0xABA6, 0x110B8, 0x0, 0xB02030, 0xB0CBE0 },
    { "RIGOMTR.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB0CBE0, 0xB0CD40 },
    { "ROCKET.CAR", FILE_TYPE_LZ, 0x9F14, 0xF7E8, 0x0, 0xB0CD40, 0xB16C60 },
    { "ROCKET.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB16C60, 0xB16DC0 },
    { "RODDER.CAR", FILE_TYPE_LZ, 0x9F15, 0x104E0, 0x0, 0xB16DC0, 0xB20CE0 },
    { "RODDER.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB20CE0, 0xB20E40 },
    { "ROKBUSTR.CAR", FILE_TYPE_LZ, 0xA1CD, 0xFC08, 0x0, 0xB20E40, 0xB2B010 },
    { "ROKBUSTR.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB2B010, 0xB2B170 },
    { "RUBBER.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xB2B170, 0xB2B220 },
    { "SCCUP1.BLD", FILE_TYPE_RAW, 0x410B, 0x410B, 0x0, 0xB2B220, 0xB2F330 },
    { "SCCUP2.BLD", FILE_TYPE_RAW, 0x5026, 0x5026, 0x0, 0xB2F330, 0xB34360 },
    { "SHDOWJET.CAR", FILE_TYPE_LZ, 0x84C4, 0xEF70, 0x0, 0xB34360, 0xB3C830 },
    { "SHDOWJET.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB3C830, 0xB3C990 },
    { "SIGNCURS.OBJ", FILE_TYPE_LZ, 0x20D, 0x360, 0x0, 0xB3C990, 0xB3CBA0 },
    { "SILHOUET.CAR", FILE_TYPE_LZ, 0x8B70, 0xEC30, 0x0, 0xB3CBA0, 0xB45710 },
    { "SILHOUET.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB45710, 0xB45870 },
    { "SLIDEBUT.OBJ", FILE_TYPE_LZ, 0x5FF, 0x990, 0x0, 0xB45870, 0xB45E70 },
    { "SLIDEOUT.CAR", FILE_TYPE_LZ, 0x8500, 0xDB38, 0x0, 0xB45E70, 0xB4E370 },
    { "SLIDEOUT.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB4E370, 0xB4E4D0 },
    { "SNAKE.CAR", FILE_TYPE_LZ, 0x924C, 0xF058, 0x0, 0xB4E4D0, 0xB57720 },
    { "SNAKE.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB57720, 0xB57880 },
    { "SOLAIRE.CAR", FILE_TYPE_LZ, 0x85B3, 0xE480, 0x0, 0xB57880, 0xB5FE40 },
    { "SOLAIRE.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB5FE40, 0xB5FFA0 },
    { "SPDBLSTR.CAR", FILE_TYPE_LZ, 0x8079, 0xCCC0, 0x0, 0xB5FFA0, 0xB68020 },
    { "SPDBLSTR.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB68020, 0xB68180 },
    { "SPLTIMG.CAR", FILE_TYPE_LZ, 0x8F4A, 0xFA60, 0x0, 0xB68180, 0xB710D0 },
    { "SPLTIMG.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB710D0, 0xB71230 },
    { "STEALTH.CAR", FILE_TYPE_LZ, 0x771F, 0xBA70, 0x0, 0xB71230, 0xB78950 },
    { "STEALTH.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB78950, 0xB78AB0 },
    { "STEEL.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xB78AB0, 0xB78B60 },
    { "STGFRT.CAR", FILE_TYPE_LZ, 0x834B, 0xC190, 0x0, 0xB78B60, 0xB80EB0 },
    { "STGFRT.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB80EB0, 0xB81010 },
    { "STICKY.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xB81010, 0xB810C0 },
    { "STRIP_T.CAR", FILE_TYPE_LZ, 0xA25A, 0xFA68, 0x0, 0xB810C0, 0xB8B320 },
    { "STRIP_T.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB8B320, 0xB8B480 },
    { "SUPERVAN.CAR", FILE_TYPE_LZ, 0x77CF, 0xE7C0, 0x0, 0xB8B480, 0xB92C50 },
    { "SUPERVAN.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB92C50, 0xB92DB0 },
    { "SWT16.CAR", FILE_TYPE_LZ, 0x9FE1, 0xF2F8, 0x0, 0xB92DB0, 0xB9CDA0 },
    { "SWT16.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xB9CDA0, 0xB9CF00 },
    { "TCUP.BLD", FILE_TYPE_RAW, 0x2202, 0x2202, 0x0, 0xB9CF00, 0xB9F110 },
    { "THUNDER.CAR", FILE_TYPE_LZ, 0x8D7E, 0xF228, 0x0, 0xB9F110, 0xBA7E90 },
    { "THUNDER.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xBA7E90, 0xBA7FF0 },
    { "TOWJAM.CAR", FILE_TYPE_LZ, 0x74CD, 0xDA60, 0x0, 0xBA7FF0, 0xBAF4C0 },
    { "TOWJAM.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xBAF4C0, 0xBAF620 },
    { "TUNING.PRM", FILE_TYPE_RAW, 0x100, 0x100, 0x0, 0xBAF620, 0xBAF720 },
    { "TURBO.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xBAF720, 0xBAF7D0 },
    { "TWINMIL2.CAR", FILE_TYPE_LZ, 0x89F1, 0xF780, 0x0, 0xBAF7D0, 0xBB81D0 },
    { "TWINMIL2.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xBB81D0, 0xBB8330 },
    { "TWINMILL.CAR", FILE_TYPE_LZ, 0xA0A8, 0xF3E0, 0x0, 0xBB8330, 0xBC23E0 },
    { "TWINMILL.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xBC23E0, 0xBC2540 },
    { "UNCAR1.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xBC2540, 0xBC25F0 },
    { "UNCAR2.DUP", FILE_TYPE_RAW, 0xAD, 0xAD, 0x0, 0xBC25F0, 0xBC26A0 },
    { "WAY2FAST.CAR", FILE_TYPE_LZ, 0x8E06, 0xFFD0, 0x0, 0xBC26A0, 0xBCB4B0 },
    { "WAY2FAST.CWH", FILE_TYPE_RAW, 0x158, 0x158, 0x0, 0xBCB4B0, 0xBCB610 },
    { "YELLOW.OVL", FILE_TYPE_LZ, 0x44AD, 0xCE48, 0x0, 0xBCB610, 0xBCFAC0 },
};
FileSystem gFileSystem = { 0 };
AsyncRead gAsyncReads[2] = { 0 };
u8 D_800F1C98[4] = { 0 };
s32 D_800F1C9C[5] = { 0 };

s32 File_LoadLZ(s32 index, void *dst);
s32 File_LoadLZChunked(s32 index, u8 *dst);
s32 File_LoadLZChunkedAsync();
void *Memory_Alloc(u32 size);
void Memory_Free(void *ptr);
void func_800D6240(u32 romAddr, void *dst, u32 size);
void func_800D66D0(PakHandle *handle, s32 port);
void func_800D6770(PakHandle *handle);
void func_800D7030(s32 port, s32 arg1);
u8 func_8006C4BC(u8 port);
void osSyncPrintf(const char *fmt, ...);
void func_800D67A0(PakHandle *handle, const char *name, const char *ext, s32 create, s32 size);
void func_800D68D0(PakHandle *handle, s32 offset, s32 size, void *buffer, s32 write);
void osInvalDCache(void *addr, u32 size);
void osCreateMesgQueue(void *queue, void *msgs, s32 count);
s32 osRecvMesg(void *queue, void *msg, s32 flag);
void osPiStartDma(void *mesg, s32 pri, s32 direction, u32 romAddr, void *dst, u32 size, void *queue);
s32 func_8006E578(s32 port);
s32 File_LoadLZAsync(s32 index, void *dst, s32 slot);
s32 File_FinishLZAsync(s32 slot);
void Rom_ReadAsync(u32 romAddr, void *dst, u32 size, AsyncRead *read);
u8 Rom_PollAsync(AsyncRead *read);
s32 LZ_Decompress(u8 *src, u8 *dst);
s32 strcasecmp(const char *a, const char *b);

s32 FileTable_Init(void) {
    memset(&gFileSystem, 0, sizeof(FileSystem));
    gFileSystem.table = gFileTable;
    gFileSystem.count = 201;
    return 1;
}

s32 File_Load(char *name, void *dst) {
    s32 high = gFileSystem.count;
    s32 low = 0;
    s32 mid = 0;
    s32 cmp = -1;
    u32 size;

    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcasecmp(name, gFileSystem.table[mid].name);
        if (cmp == 0) {
            break;
        }
        if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (cmp != 0) {
        return 0;
    }

    switch (gFileSystem.table[mid].type) {
        case FILE_TYPE_LZ_CHUNKED:
            File_LoadLZChunked(mid, dst);
            break;
        case FILE_TYPE_LZ:
            File_LoadLZ(mid, dst);
            break;
        case FILE_TYPE_RAW:
        default:
            size = gFileSystem.table[mid].size;
            if (size & 1) {
                func_800D6240(gFileSystem.table[mid].romStart, dst, size + 1);
            } else {
                func_800D6240(gFileSystem.table[mid].romStart, dst, size);
            }
            break;
    }
    return gFileSystem.table[mid].size;
}

s32 File_LoadAsync(char *name, void *dst) {
    s32 high;
    s32 low;
    s32 mid;
    s32 cmp;
    s32 slot;
    u32 size;

    low = 0;
    high = gFileSystem.count;
    mid = 0;
    cmp = -1;
    slot = 0;

    if (gAsyncReads[0].active) {
        if (gAsyncReads[1].active) {
            return -1;
        }
        slot = 1;
    }
    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcasecmp(name, gFileSystem.table[mid].name);
        if (cmp == 0) {
            break;
        }
        if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (cmp != 0) {
        return 0;
    }

    gAsyncReads[slot].type = gFileSystem.table[mid].type;
    gAsyncReads[slot].active = 1;
    switch (gFileSystem.table[mid].type) {
        case FILE_TYPE_LZ_CHUNKED:
            File_LoadLZChunkedAsync(mid, dst, slot);
            break;
        case FILE_TYPE_LZ:
            File_LoadLZAsync(mid, dst, slot);
            break;
        case FILE_TYPE_RAW:
            size = gFileSystem.table[mid].size;
            if (size & 1) {
                Rom_ReadAsync(gFileSystem.table[mid].romStart, dst, size + 1, &gAsyncReads[slot]);
            } else {
                Rom_ReadAsync(gFileSystem.table[mid].romStart, dst, size, &gAsyncReads[slot]);
            }
            break;
    }
    return slot;
}

INCLUDE_ASM("asm/nonmatchings/file_table", File_PollAsync);

s32 File_GetSize(char *name) {
    s32 high = gFileSystem.count;
    s32 low = 0;
    s32 mid = 0;
    s32 cmp = -1;

    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcasecmp(name, gFileSystem.table[mid].name);
        if (cmp == 0) {
            break;
        }
        if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (cmp != 0) {
        return 0;
    }
    return gFileSystem.table[mid].size;
}

const char sPakStatusFormat[] = "%d\n";
const char sPakGameName[16] = "Hot Wheels";
const char sPakExtName[8] = "";

s32 func_8006E578(s32 port) {
    PakHandle *handle;

    D_800F1C98[port] = 1;
    gFileSystem.initialized[port] = 0;
    if (gFileSystem.handles[port] != NULL) {
        Memory_Free(gFileSystem.handles[port]);
        gFileSystem.handles[port] = NULL;
    }
    handle = Memory_Alloc(sizeof(PakHandle));
    memset(handle, 0, sizeof(PakHandle));
    func_800D66D0(handle, port);
    if (port == 0) {
        osSyncPrintf(sPakStatusFormat, handle->status);
    }
    if (handle->status != 0) {
        gFileSystem.handles[port] = handle;
        switch (handle->status) {
            case 1:
                Memory_Free(handle);
                gFileSystem.handles[port] = NULL;
                D_800F1C9C[port] = 1;
                return 1;
            case 11:
                Memory_Free(handle);
                gFileSystem.handles[port] = NULL;
                if (func_8006C4BC(port)) {
                    D_800F1C9C[port] = 1;
                } else {
                    D_800F1C9C[port] = 0x80;
                }
                return D_800F1C9C[port];
            case 2:
                D_800F1C9C[port] = 2;
                return 2;
            case 10:
                D_800F03F8[port].unk00 = 1;
                D_800F1C9C[port] = 0x20;
                return 0x20;
            case 3:
            default:
                D_800F1C9C[port] = 1;
                return 1;
        }
    }
    if (D_800F03F8[port].unk00 == 2) {
        func_800D7030(port, 0);
    }
    func_800D67A0(handle, sPakGameName, sPakExtName, 0, 0);
    if (handle->status != 0) {
        gFileSystem.handles[port] = handle;
        switch (handle->status) {
            case 1:
                Memory_Free(handle);
                gFileSystem.handles[port] = NULL;
                D_800F03F8[port].unk00 = 0;
                D_800F1C9C[port] = 1;
                return 1;
            case 2:
                D_800F1C9C[port] = 2;
                return 2;
            case 5:
                D_800F1C9C[port] = 4;
                return 4;
            case 4:
                D_800F1C9C[port] = 1;
                return 1;
        }
    }
    gFileSystem.initialized[port] = 1;
    gFileSystem.handles[port] = handle;
    D_800F1C9C[port] = 0;
    return 0;
}

s32 func_8006E8A4(s32 port) {
    PakHandle *handle;

    if (D_800F1C98[port]) {
        return D_800F1C9C[port];
    }
    handle = gFileSystem.handles[port];
    if (handle == NULL) {
        return 1;
    }
    if (!gFileSystem.initialized[port]) {
        if (handle->status == 7 || handle->status == 8) {
            return 8;
        }
        return 1;
    }
    switch (handle->status) {
        case 0:
            return 0;
        case 2:
            return 2;
        case 7:
        case 8:
            return 8;
        case 9:
        case 10:
            return 0x20;
        case 13:
        default:
            return 1;
    }
}

u8 Save_IsChecksumValid(SaveBlock *block) {
    s32 sum = 0;
    u8 *p = block->data;
    u16 i = 0;
    u32 size = 0xD90;

    for (; i < size; i++) {
        sum += *p++;
    }
    return *(s32 *)&block->data[0xD90] == sum;
}

s32 Save_Read(s32 port, SaveBlock *dst) {
    SaveBlock *blocks[2];
    u8 errors[2];
    PakHandle *handle;

    blocks[0] = Memory_Alloc(sizeof(SaveBlock));
    blocks[1] = Memory_Alloc(sizeof(SaveBlock));
    D_800F1C98[port] = 0;
    handle = gFileSystem.handles[port];
    if (handle != NULL && gFileSystem.initialized[port]) {
        func_800D68D0(handle, 0, sizeof(SaveBlock), blocks[0], 0);
        errors[0] = handle->status != 0;
        func_800D68D0(handle, sizeof(SaveBlock), sizeof(SaveBlock), blocks[1], 0);
        errors[1] = handle->status != 0;
        if (Save_IsChecksumValid(blocks[0]) && !errors[0]) {
            goto first_valid;
        }
        if (Save_IsChecksumValid(blocks[1]) && !errors[1]) {
            goto second_only;
        }
    }
    Memory_Free(blocks[0]);
    Memory_Free(blocks[1]);
    return 0;

second_only:
    func_800D68D0(handle, 0, sizeof(SaveBlock), blocks[1], 1);
    *dst = *blocks[1];
    goto done;

first_valid:
    if (!Save_IsChecksumValid(blocks[1]) || errors[1]) {
        func_800D68D0(handle, sizeof(SaveBlock), sizeof(SaveBlock), blocks[0], 1);
        *dst = *blocks[0];
    } else {
        *dst = *blocks[0];
    }

done:
    Memory_Free(blocks[0]);
    Memory_Free(blocks[1]);
    return 1;
}

s32 Save_Write(s32 port, SaveBlock *src) {
    PakHandle *handle;

    D_800F1C98[port] = 0;
    handle = gFileSystem.handles[port];
    if (handle == NULL) {
        return 0;
    }
    if (!gFileSystem.initialized[port]) {
        func_800D67A0(handle, sPakGameName, sPakExtName, 1, 0x1B40);
        if (handle->status != 0) {
            return 0;
        }
        gFileSystem.initialized[port] = 1;
    }
    func_800D68D0(handle, 0, sizeof(SaveBlock), src, 1);
    func_800D68D0(handle, sizeof(SaveBlock), sizeof(SaveBlock), src, 1);
    return handle->status == 0;
}

s32 func_8006EDD4(s32 port) {
    PakHandle *handle;

    D_800F1C98[port] = 0;
    handle = gFileSystem.handles[port];
    if (handle == NULL) {
        return 0;
    }
    func_800D6770(handle);
    if (handle->status == 0) {
        return func_8006E578(port) != 10;
    }
    return 0;
}

s32 File_LoadLZ(s32 index, void *dst) {
    void *buffer;
    u32 size;

    buffer = Memory_Alloc(gFileSystem.table[index].storedSize);
    if (buffer == NULL) {
        return 0;
    }
    size = gFileSystem.table[index].storedSize;
    if (size & 1) {
        func_800D6240(gFileSystem.table[index].romStart, buffer, size + 1);
    } else {
        func_800D6240(gFileSystem.table[index].romStart, buffer, size);
    }
    LZ_Decompress(buffer, dst);
    Memory_Free(buffer);
    return 1;
}

s32 File_LoadLZChunked(s32 index, u8 *dst) {
    u8 mesg[0x38];
    u8 *buffer = D_80102220;
    u8 *out = dst;
    u32 offset = 0;
    u32 chunkSize = gFileSystem.table[index].firstChunkSize;

    do {
        Rom_ReadAsync(gFileSystem.table[index].romStart + offset, buffer, chunkSize, (AsyncRead *)mesg);
        while (!Rom_PollAsync((AsyncRead *)mesg)) {
        }
        offset += chunkSize;
        chunkSize = *(u32 *)buffer;
        buffer += 8;
        out += LZ_Decompress(buffer, out);
    } while (chunkSize != 0);
    return 1;
}

s32 File_LoadLZAsync(s32 index, void *dst, s32 slot) {
    void *buffer;
    u32 size;

    buffer = Memory_Alloc(gFileSystem.table[index].storedSize);
    if (buffer == NULL) {
        return 0;
    }
    size = gFileSystem.table[index].storedSize;
    if (size & 1) {
        Rom_ReadAsync(gFileSystem.table[index].romStart, buffer, size + 1, &gAsyncReads[slot]);
    } else {
        Rom_ReadAsync(gFileSystem.table[index].romStart, buffer, size, &gAsyncReads[slot]);
    }
    gAsyncReads[slot].buffer = buffer;
    gAsyncReads[slot].dst = dst;
    return 1;
}

s32 File_FinishLZAsync(s32 slot) {
    LZ_Decompress(gAsyncReads[slot].buffer, gAsyncReads[slot].dst);
    Memory_Free(gAsyncReads[slot].buffer);
    return 1;
}

s32 File_LoadLZChunkedAsync() {
    return 0;
}

s32 func_8006F0EC(void) {
    return 0;
}

s32 func_8006F0F4(void) {
    return 0;
}

void Rom_ReadAsync(u32 romAddr, void *dst, u32 size, AsyncRead *read) {
    osInvalDCache(dst, size);
    osCreateMesgQueue(read->queue, &read->msg, 1);
    osPiStartDma(read, 0, 0, romAddr, dst, size, read->queue);
}

void Rom_WaitAsync(AsyncRead *read) {
    osRecvMesg(read->queue, NULL, 1);
}

u8 Rom_PollAsync(AsyncRead *read) {
    return osRecvMesg(read->queue, NULL, 0) == 0;
}
