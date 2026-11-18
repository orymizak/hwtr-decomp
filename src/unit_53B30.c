#include "common.h"
#include "include_asm.h"
/* TODO(match-debt): empty loop needed to match (scheduling barrier). Assumed to be a debug hook
 * compiled out in release builds; verify against a real cause when one turns up. */
#define DEBUG_NOP() do { } while (0)

/* TODO (Clean version): Unify Plr53_ with the actual player-data struct at D_80124148.
 * Fields bits14/bits18 are a split 64-bit unlock mask; consider:
 *   typedef struct { u8 p0[0x14]; u64 unlockBits; s32 raceUnlocks; u8 sel; } Plr53_;
 * or use helper accessors:
 *   #define PLR_IS_UNLOCKED(p, id) \
 *       ((id) >= 32 ? ((p)->bits18 >> ((id)-32)) & 1 : ((p)->bits14 >> (id)) & 1)
 */
/* Player menu state: unlock bitmasks and the selected entry. */
typedef struct { u8 p0[0x14]; s32 bits14; s32 bits18; s32 bits1C; u8 sel; } Plr53_;
/* Per-player race result block (0x2C) inside the race state. */
typedef struct { s32 lapTime; u8 lap; u8 p5[3]; s32 raceTime; u8 pC[0x20]; } RaceRes_;
typedef struct { u8 p0[0x3C]; RaceRes_ res[2]; } RaceSt_;
typedef struct { s32 a; s32 b; s32 c; } V12_;
extern void (*D_80140FBC)(s32, char *, s32);
extern void (*D_80140FB8)(char *, void *, void *);
extern void (*D_80140FD8)(void);
void func_800C46C4(void);
extern void (*D_80141024)(s32);
extern u8 (*D_80140FFC)(u8);
extern s32 D_80124CE4;
extern s32 D_80124CE8;
extern s32 D_80124CEC;
extern s32 D_80124CF0;
extern s32 D_80124CF4;
extern s32 D_80124CF8;
extern u8 D_80124CFC;
extern s32 D_800F9FA0[][3];
extern void (*D_8014103C)(u8, s32);
s8 func_800B0FB0(void *);
extern s16 D_800F616C;
extern s16 D_800F61D0;
extern u8 D_800F6101;
extern void (*D_801455AC)(s32, s32);
u8 func_800B7EE8(u8 i);
typedef struct { u8 a; u8 pad; s16 b; s16 c; u8 pad2[4]; } E0A_;
typedef struct { u8 pad[2]; u8 n; u8 pad2; E0A_ *arr; u8 pad3[8]; u8 r; u8 g; u8 bl; } T39_;
extern void (*D_80141020)(s32, s32, s32);
extern void (*D_8014102C)(void);
extern void (*D_80141038)(f32, f32, f32, f32);
extern void (*D_8014105C)(s32);
extern u8 (*D_80141080)(void);
extern u8 (*D_801455A4)(void);
extern void (*D_8014556C)(s32);
extern u8 (*D_80145578)(s32, s32);
extern u8 (*D_80140F9C)(void *);
extern u8 (*D_80140FA8)(void);
extern void (*D_80140FAC)(s32, s32, f32, f32);
typedef struct { f32 x; f32 y; f32 z; } V3f3_;
extern V3f3_ D_80123E40[];
s32 strcmp(const char *, const char *);
u32 func_800B042C(u8);
extern u8 D_800F6102;
void func_800C0B68(void);
u8 func_800C1664(void);
extern u8 D_80124C7E;
extern u8 D_80124C7F;
extern u8 D_80124160;
extern u8 D_801249F0;
extern u8 D_80124B30;
void func_800B5294(void *);
void func_800B3264(void);
extern u8 D_80124C7C;
extern u8 D_80124C7D;
extern s32 D_80124C78;
void func_800B2A20(s32);
s32 func_800B97D8(u8 i);
s32 func_800B7D60(void);
extern u8 D_80123F48;
typedef struct { u8 pad[0x23]; u8 b23; s32 w24[6]; } Blk23_;
extern s32 D_800F60E8[];
u8 func_800AF910(void);
void func_800B9794(u8 i);
extern u8 D_800F6418;
extern u8 D_80100F88[];
typedef struct { u8 p0[0x10]; s32 unk10; f32 unk14; } ObjG1_;
typedef struct { s32 d[0x88C / 4]; } Big88C_;
typedef struct { u8 a; u8 pad; s16 x; s16 y; u8 pad2[4]; } E0A4_;
typedef struct { s32 d[0x50]; } Big140_;
typedef struct { char *name; u8 pad[0xC]; } N10_;
typedef struct { f32 x; f32 y; f32 z; f32 x2; f32 y2; u8 pad[0x1C]; u16 h30; u16 h32; } E34b_;
typedef struct { u8 pad[0x20]; u8 n; u8 p[3]; N10_ *names; E34b_ *objs; } TBL_;
typedef struct { u8 pad[8]; char name[0xC]; s32 w14; s32 w18; s32 w1C; u8 pad2[0x120]; } E140b_;
typedef struct { u16 idx; u8 pad[2]; s32 lo; s32 hi; s32 y; s32 reset; f32 x; } Scroll4E_;
typedef struct { u8 a; u8 b; u8 c; s8 lv; } Bars4_;
typedef struct { char name[0xC]; s32 f0C; u8 f10; u8 f11; u8 f12; u8 p13; } Pl7A_;
typedef struct { s32 flags; char name[9]; char sub[9]; u8 trk; u8 sb; u8 cnt; u8 f19; u8 f1A; u8 p1B; s32 f1C; s32 f20; Pl7A_ p[6]; u8 f9C; } Race7A_;
typedef struct { u8 b0; u8 p1[0x17]; u8 b18; u8 p19[0x13]; } It2C_;
typedef struct { u8 n; u8 p1[3]; It2C_ *it; } Ls2C_;
typedef struct { s32 a; s32 b; s32 c; } V12c_;
typedef struct { u8 *s; s16 n; s16 x6; s16 x8; s16 y; u8 pC[2]; u8 e; u8 f; u8 g; u8 p11[3]; } ItAA4_;
typedef struct { u8 p0[0x1C]; ItAA4_ *it; } MnAA4_;
typedef struct { f32 x; f32 y; f32 z; } V3B37_;
typedef struct { f32 a; f32 b; f32 c; f32 d; } V4B37_;
typedef struct { V3B37_ p; u8 pC[0xC]; f32 f18; f32 f1C; f32 f20; u8 p24[0xE]; s16 s32; } EnB37_;
typedef struct { s32 *a; u8 p4[0xC]; } RfB37_;
typedef struct { u8 p0[0x20]; u8 n; u8 p21[3]; RfB37_ *r; EnB37_ *e; } ObB37_;
typedef struct { u8 p0[0xC]; f32 fC; f32 f10; u8 p14[0x20]; } ElC05_;
typedef struct { char *name; u8 n; u8 p5[3]; s32 v[3]; } Nm5010_;
typedef struct { char *s; u8 p4[0xC]; } NmC0_;
typedef struct { f32 x; f32 y; u8 p8[4]; f32 xC; f32 x10; u8 p14[0x1C]; s16 h30; s16 h32; } ElC0_;
typedef struct { u8 p0[0x20]; u8 n; u8 p21[3]; NmC0_ *names; ElC0_ *elems; } MnC0_;
typedef struct { char *s; u8 p4[0xC]; } NmC3_;
typedef struct { f32 x; f32 y; u8 p8[4]; f32 xC; f32 x10; u8 p14[0x20]; } ElC3_;
typedef struct { u8 p0[0x20]; u8 n; u8 p21[3]; NmC3_ *names; ElC3_ *elems; } MnC3_;
extern u8 D_800F6474;
s32 *func_800C815C(void);
void func_800C8174(void);
void func_800BA584(s32 id);
extern u8 D_800F6475;
s32 *func_800C8168(void);
void func_800C81A0(void);
s32 Config_CheckCheatCode(char *code, void *cfg);
void func_800BA6F8(s32);
extern char D_80100FEC[];
void func_800C3560(s32 x, s32 y);
extern s32 D_800F5F70;
extern s32 D_800F5F74;
extern u8 D_800F63D0;
s32 func_800AFC40(u32 i);
void func_800C8110(void);
extern u8 D_801410A8[];
extern u8 D_800F6400;
extern char *D_800F649C[];
void func_800C0A20(u8 which, u8 name, s32 x, s32 y, u8 val);
extern u8 D_80100DC0;
extern u8 D_80100DC8;
u8 func_80086E40(void *);
u8 func_80086BE0(void *);
extern s32 D_801411CC[];
extern s32 D_801411D0;
extern s32 D_801411D4;
extern s32 D_801411D8;
extern s32 D_801411DC;
extern s32 D_801411E0;
extern s32 D_801411E4;
extern s32 D_8012406C;
extern s32 D_80124070;
extern s32 D_80124074;
extern s32 D_80124078;
extern s32 D_8012407C;
extern s32 D_80124080;
extern s32 D_80124084;
extern s32 D_80124088;
extern s32 D_80124110[];
extern Nm5010_ D_800F61F0[];
void func_800B337C(u8, V3B37_, f32, f32, f32, f32);
extern u8 D_800F648F;
extern u8 D_800F648E;
void func_800C4890(void);
extern u8 D_80100BD8;
void func_800B4DF8(s32 arg0);
s32 func_800B7DFC(u8 i, s32 p);
extern void (*D_80141044)(u8);
extern void (*D_80141000)(s32, void *);
extern u8 D_80100F94;
void func_800B8308(void);
extern s32 (*D_80141030)();
s32 func_800B32D0();
s32 func_800B3144(s32 *a);
extern Race7A_ D_80123EA8;
extern u8 D_80124D10[];
extern s32 D_800F9FD0[];
extern u8 D_800FA010[][3];
extern s32 D_80101044[];
extern s32 D_80101050[];
extern u8 D_8010105C;
extern u8 D_80101064;
extern s32 (*D_80140F98)(void *);
void *memset(void *, s32, u32);
void *memcpy(void *, const void *, u32);
extern u8 *D_801410E0[];
extern s32 (*D_80141008)(s32, u8);
s8 func_80086F80(s32, u8, u8);
s32 func_800B0454(u32 i, E140b_ *src);
extern char D_80100F70[];
extern s32 *D_80141268;
s32 *func_8008D460(s32);
s32 *func_8008D48C(s32);
void func_800BFC68(void);
extern u8 D_80100DF4;
extern s32 *D_80141480;
extern u8 D_80100E08;
extern ObjG1_ D_800F6358;
u8 func_800AFD1C(void);
void *func_800B027C(void);
s32 func_800B0364(s32);
extern u8 D_80100DAC;
extern u8 D_80100D94;
extern s32 *D_8014158C;
extern u8 D_80100D48;
extern u8 D_80100D58;
extern u8 D_80100D68;
extern u8 D_80100D7C;
extern s32 *D_80141464;
extern s32 *D_80141468;
extern s32 *D_8014146C;
extern u8 D_80100CC8;
extern u8 D_80100CF0;
extern u8 D_80100D08;
extern u8 D_80100D20;
extern u8 D_80100CAC;
extern s32 *D_8014124C;
extern s32 *D_80141250;
extern u8 D_80100D88;
extern s32 *D_80141470;
extern s32 *D_80141474;
extern s32 *D_80141478;
extern s32 *D_80141258;
extern s32 *D_8014125C;
extern u8 D_80100D38;
extern s32 *D_80141454;
extern s32 *D_80141458;
extern s32 *D_80141234;
extern s32 *D_80141238;
extern s32 *D_8014123C;
extern s32 *D_80141240;
extern s32 *D_80141244;
extern u8 D_80100FAC;
extern u8 D_80100FB4;
extern u8 D_80100FBC;
extern u8 D_80100FC4;
extern u8 D_80124CA0;
extern u8 D_80124CC0;
extern char *D_800F64A8[];
void func_800C4564(s32 i, s32 x, s32 y, s32 c);
extern char *D_800F6478[];
void func_800BB350(s32 i, s32 x, s32 y, s32 c);
extern char *D_800F646C[];
void func_800B983C(s32 i, s32 x, s32 y, s32 c);
extern s32 D_80141504;
extern s32 D_80141508;
extern s32 D_8014150C;
extern u8 D_80100CA0;
extern u8 D_80100EB4;
extern u8 D_80100F34;
extern u8 D_80100F48;
extern s32 *D_80141248;
extern s32 *D_80141254;
extern s32 D_80100DBC;
extern u8 D_80100E74;
extern ObjG1_ D_800F63A0;
extern Bars4_ D_801240E4;
extern u8 D_80100F0C;
extern u8 D_80100F14;
extern u8 D_80100F1C;
void func_800B4CC4(s32, s32, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern Bars4_ D_801240E0;
extern u8 D_80100FA4;
extern ObjG1_ D_800F63B8;
extern void (*D_801455A8)(s32, s32);
extern void (*D_80141054)(s32);
extern void (*D_80141064)(s32);
extern void (*D_8014107C)(void);
extern void (*D_80141088)(void);
void func_800C9130(void);
void func_800C9A00(void);
extern u32 D_800F6078[];
u8 func_800C8214(u8 i, s32 k);
extern void (*D_80141068)(s32);
void func_800C90DC(void);
void func_800C9978(void);
Big88C_ *func_800B02E4(s32);
void func_800C65AC(void *);
void func_800B8184(void);
extern u8 D_800F6424;
extern u8 D_800F6444;
s32 func_800B0F94(void *, s32);
void func_800B0F74(s32, s32 *);
void func_800B80B0(void);
extern s32 D_8014122C;
extern u8 D_80100CA4;
void func_800B51C0(void *arg0, s32 arg1);
void func_800C0BE0(void);
extern u8 D_800F648C;
extern u8 D_800F648D;
extern u8 D_80124C70;
extern u8 D_801240D5;
extern u8 D_800F60D0;
extern s8 D_800F60D4[][6];
void func_800BDF94(void);
void func_800BE288(void);
extern s32 D_801240F4;
extern u8 D_80124C95;
void func_800B4E64(ObjG1_ *o);
extern u8 D_800F6476;
s32 func_800C8134(s32, s32);
extern u8 D_800F6480;
extern u8 D_80124C94;
extern void (*D_8014100C)(s32, s32, s32, s32, s32);
extern void (*D_80141010)(s32, s32, s32);
extern void func_800C9F18(void *, s32);
extern u8 *D_800F641C;
extern u8 D_80123EBE;
extern u8 D_80123EBF;
extern u8 D_80123EC1;
s32 func_800B3EAC(s32, s32, s32, s32);
s32 func_800B463C(s8);
s32 func_800B4AA8(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_800B8DC4();
extern s32 D_801240E8;
extern s32 D_801240EC;
extern s32 D_801240F0;
extern u8 (*D_80140FC0)();
extern u8 D_801240DF;
extern u8 D_80100FE4;
extern void *D_800F5F10;
extern void *D_800F5F0C;
extern s16 D_800F610C[];
extern s16 D_800F6170[];
extern u8 D_801240D4;
void func_800B3C18(u8 i);
u8 func_80086D50(s32);
extern u8 D_80100FDC;
extern u8 D_801240D6;
extern s8 D_801240D7;
extern s32 D_801240D8;
extern u8 D_801240DC;
extern s8 D_801240DD;
extern s8 D_801240DE;
extern s8 D_800F610A;
extern void *D_80124CE0;
void * func_800B0310(u32, u32);
extern s32 D_80124150;
extern s32 D_80124154;
extern void *D_800F5F6C;
extern u8 D_801240C0;
extern u8 D_800F64A4;
extern s8 D_80124106;
extern s8 D_80124105;
s32 func_8008744C(void *);
extern u8 D_800F2FA0;
extern u8 D_800F2FC0;
extern s8 D_800F61D6;
extern u8 D_800F63E8;
extern u8 D_80100F80;
extern u8 D_80124C71;
extern u8 D_80124C72;
extern u8 D_800F6490;
extern u8 D_80100F60;
extern s8 D_80124C81;
extern s8 D_80124C9E;
s32 func_800B3E24(s32,s32,s32,s32);
extern u8 D_80100F58;
extern u8 D_80124C80;
extern s8 D_80124C9D;
extern u8 D_801240BB;
extern u8 D_801240BC;
extern u8 D_800F61D2;
extern u8 D_80124C8C;
void func_800B8DA8(void);
void func_800B8D8C(void);
void func_800B8D74(void);
s32 func_800C9E70(void);
extern s8 D_800F6466;
s32 func_8008D4B8(u32);
extern s8 D_800F6464;
extern u8 D_800F9F70;
extern u8 D_800FA0DC;
extern f32 D_80123E88;
extern f32 D_80123E8C;
extern f32 D_80123E90;
f32 func_800B1294(s32);
f32 func_800B12C4(s32);
f32 func_800B1308(s32);
extern f32 D_80123E74;
extern f32 D_80123E78;
extern f32 D_80123E7C;
void func_800B4578(void);
extern s32 D_80123E80;
extern f32 D_80123E84;
extern f32 D_80123E94;
extern s8 D_80123E98;
void func_800B3C74(u8 i);
/* Per-player camera zoom and field of view (current/target, radians). */
extern f32 D_800F60A0[2];
extern f32 D_800F60A8[2];
extern f32 D_800F60B0[2];
extern f32 D_800F60B8[2];
extern u8 D_800F60C0[2];

extern s32 (*D_80141028)(s32);
extern s8 D_800F61D5;
s32 func_800B2BAC(s32);
extern s32 D_800F5F18;
extern s32 D_800F5F1C;
extern s32 (*D_8014C0B4)();
extern s8 D_80124C73;
extern s32 D_800F6468;
extern s32 (*D_80141048)();
s32 func_800B2A84();
extern s32 (*D_80141084)();
s32 func_80086F78();
extern s32 (*D_80140F94)();
s32 func_800B2AD0(s32);
extern s32 D_800F5F08;
s32 func_800C8554(s32);
extern s32 D_80100DD0;
extern s8 D_80124144;
extern s8 D_80124145;
extern u8 D_80124146;
extern s32 D_800F5F20;
extern s32 D_80100DDC;
extern s32 D_80100DE4;
extern s32 D_800F5F24;
extern s32 D_80100DEC;
extern s8 D_80124C74;
extern s8 D_80124C75;
s32 func_800B2B28(s32);
extern s32 D_800F5F14;
extern s32 D_80124C84;
extern s32 D_80124C88;
extern s32 (*D_80140FC8)();
extern s32 (*D_80140FCC)();
extern s32 (*D_80140FB4)(s32);
extern s32 (*D_80140FDC)();
s32 func_800B1064(s32 *);
s32 func_800B10F4(s32 *);
extern s32 D_80100E10;
extern s32 (*D_80141040)(s32);
extern s32 D_800F5F28;
s32 func_800B2F20(s32);
extern s32 D_800F5F2C;
extern s8 D_800F6100;
extern s8 D_800F61D3;
s32 func_800B2AFC(s32);
extern s32 D_800F5F34;
extern s32 D_80100ED4;
extern s32 D_800F5F38;
extern s32 D_80100EFC;
extern s32 D_80124C98;
extern s32 D_800F5F3C;
extern s32 D_80100F04;
extern s32 D_800F5F30;
extern s8 D_80124C9C;
extern s32 D_800F6484;
extern s32 D_800F6488;
extern s32 D_800F5F40;
extern s32 D_800F5F44;
extern u8 D_801240B9;
extern s32 D_80124148;
extern u8 D_80123EA0;
extern u8 D_801240BA;
extern s32 D_8012414C;
extern s32 D_800F5F48;
extern s32 D_800F6370;
extern s32 D_80100F78;
extern s8 D_800F61D4;
extern s32 D_800F5F60;
extern s32 (*D_80141004)(s32);
extern u8 D_80124107;
extern u8 D_80124109;
extern u8 D_8012410B;
extern u8 D_80124108;
extern u8 D_8012410A;
extern u8 D_8012410C;
extern s32 D_800F5F4C;
extern s32 D_800F5F50;
extern s32 D_800F5F54;
extern s32 D_800F5F58;
extern s32 (*D_801455B0)(s32);
extern s32 D_800F5F5C;
s32 func_800C891C(s32);
s32 func_800AFB7C(u8);
extern u8 D_800F6420;
extern u8 D_801240B0;
extern u8 D_800F6421;
extern u8 D_801240B1;
extern s32 D_800F5F64;
extern s32 D_800F5F68;
extern s8 D_801240B3;
extern s8 D_801240B4;
extern s8 D_801240B6;
extern s8 D_801240B2;
extern u8 D_801240B5;
extern u8 D_801240B8;
extern u8 D_801240B7;
s32 func_800B0024();
s32 func_800B0548(u8, s32);
extern s32 D_800F5F7C;
extern s32 D_800F5F78;
extern s32 D_800F5F80;
extern u8 D_80124103;
extern u8 D_80124104;
extern u8 D_800F61D7;
extern s32 D_800F5F88;
extern s32 D_80101014;
extern s8 D_80124CFD;
extern s8 D_80124CFE;
extern s8 D_80124CFF;
extern s8 D_80124D00;
extern s8 D_80124D01;
s32 sprintf(s32 *, s32 *, s32);
extern s32 D_80101020;
extern s32 D_80124D04;
extern s8 D_80124D08;
extern s8 D_80124D09;
extern s32 (*D_8014104C)(s32);
s32 strcpy(s32 *, s32);
extern s32 D_801345B8;
extern s32 D_80138D20;

extern u32 D_800F6050[];
extern s32 (*D_80141048)();

/* TODO (Bug potential): Discontinuity in delta time calculation. In func_800B3024, delta > 500 clamps to 0.5f (d / 1000.0f).
 * Here, if delta > 100, ret is set to 0.1f instead of 1.0f (delta / 100.0f), causing an abrupt 10x drop during lag spikes.
 * Clean version:
 *   ret = (delta > 100) ? 1.0f : (f32)delta / 100.0f;
 */
f32 func_800B2F30(u8 arg0) {
    u32 now;
    u32 delta;
    f32 ret;

    if (D_800F6050[arg0] == 0) {
        ret = 0.15f;
        D_800F6050[arg0] = D_80141048();
    } else {
        now = D_80141048();
        delta = now - D_800F6050[arg0];
        if (delta > 100) {
            ret = 0.1f;
        } else {
            ret = (f32)delta / 100.0f;
        }
        D_800F6050[arg0] = now;
    }
    return ret;
}


f32 func_800B3024(u8 i) {
    f32 r;
    u32 now;
    u32 d;

    if (D_800F6078[i] == 0) {
        r = 0.15f;
        D_800F6078[i] = D_80141048();
    } else {
        now = D_80141048();
        d = now - D_800F6078[i];
        if (d > 500) {
            r = 0.5f;
        } else {
            r = d / 1000.0f;
        }
        D_800F6078[i] = now;
    }
    return r;
}


void func_800B3114(s8 a) {
    if (a >= 0) {
        D_80141044(a);
    }
}


s32 func_800B3144(s32 *a) {
    return D_80141028((s32)a);
}


void func_800B3168(void) {
    D_80141028(0);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B318C);

/* TODO (Clean version): Perspective projection setup with magic constants:
 *   nearClip = 48.0f, farClip = 5000.0f, fovY = 60.0 deg (1.0471967f rad = M_PI / 3.0f), aspect = 4:3 (1.3333334f).
 * Propose:
 *   #define FOV_60_DEGREES  1.0471967f
 *   #define ASPECT_4_3      (4.0f / 3.0f)
 *   D_80141038(48.0f, 5000.0f, FOV_60_DEGREES, ASPECT_4_3);
 */
void func_800B3264(void) {
    D_80141020(0, 0, 0);
    D_8014102C();
    D_80141038(48.0f, 5000.0f, 1.0471967f, 1.3333334f);
}


s32 func_800B32D0() {
    return D_80141030();
}


void func_800B32F4(u8 arg0, u8 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7) {
    s32 n = arg3 + 1;

    D_80141010(arg5, arg6, arg7);
    D_8014100C(arg1, arg0, arg2, n, arg4);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B337C);

void func_800B3720(ObB37_ *o) {
    u8 i;
    s32 r;

    for (i = 0; i < o->n; i++) {
        r = func_800B1064(o->r[i].a);
        func_800B337C(r, o->e[i].p, o->e[i].f18, o->e[i].f1C, o->e[i].f20, o->e[i].s32 / 100.0f);
    }
}

/* TODO (Clean version): Replace raw byte indexing on 't' with a typed UI component header:
 *   typedef struct {
 *       u8 pad0[0x18];
 *       u8 count;             // 0x18
 *       u8 pad19[3];
 *       E0A4_ *entries;       // 0x1C
 *       u8 pad20[8];
 *       u8 sheet;             // 0x28
 *       u8 r, g, b;           // 0x29, 0x2A, 0x2B
 *   } UIComponent_;
 */
void func_800B3814(u8 *t, u8 mode, u8 c) {
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    s32 i;

    if (mode == 0) {
        r = t[0x29];
        g = t[0x2A];
        b = t[0x2B];
    }
    for (i = 0; i < t[0x18]; i++) {
        E0A4_ *e = &(*(E0A4_ **)(t + 0x1C))[i];

        if (mode != 0) {
            func_800B32F4(e->a, t[0x28], e->x, e->y, mode, c, c, c);
        } else {
            func_800B32F4(e->a, t[0x28], e->x, e->y, 0, r, g, b);
        }
    }
}

void func_800B3914(T39_ *t) {
    s32 i;

    for (i = 0; i < t->n; i++) {
        E0A_ *e = (E0A_ *)((u8 *)t->arr + i * 10);

        func_800B32F4(e->a, 1, e->b, e->c, 0, t->r, t->g, t->bl);
    }
}


void func_800B39A0(u8 *t, u8 r, s32 g, s32 b, u8 mode, s32 unused, u8 c) {
    s32 one = 1;
    s32 i;

    for (i = 0; i < t[2]; i++) {
        E0A4_ *e = &(*(E0A4_ **)(t + 4))[i];

        if (mode != 0) {
            func_800B32F4(e->a, one, e->x, e->y, mode, c, c, c);
        } else {
            func_800B32F4(e->a, one, e->x, e->y, 0, r, g, b);
        }
    }
}


void func_800B3AA4(MnAA4_ *m, u8 idx, u8 col) {
    ItAA4_ *it;
    s32 i;
    s32 x;
    s32 y;

    it = &m->it[idx];
    i = 0;
    y = it->y;
    x = (it->x6 + it->x8) / 2 - (it->n * 15) / 2;
    for (; i < it->n; i++, x += 15) {
        func_800B32F4(it->s[i], 0, x, y, col, it->e, it->f, it->g);
    }
}

/* TODO (Clean version): Unify per-player camera arrays into a struct and define angle constant:
 *   #define CAM_DEFAULT_FOV_RAD 2.2689264f // 130.0 degrees in radians (130 * M_PI / 180)
 *   typedef struct {
 *       f32 zoomCur; // D_800F60A0
 *       f32 zoomTgt; // D_800F60A8
 *       f32 fovCur;  // D_800F60B0
 *       f32 fovTgt;  // D_800F60B8
 *       u8  active;  // D_800F60C0
 *   } PlayerCam;
 */
void func_800B3B88(u8 i) {
    D_800F60C0[i] = 0;
    D_800F60B0[i] = 2.2689264f;
    D_800F60B8[i] = 2.2689264f;
    D_800F60A0[i] = 1.0f;
    D_800F60A8[i] = 1.0f;
}

s32 func_800B3BE4(u8 i) {
    D_800F60B8[i] = 2.2689264f;
    D_800F60B0[i] = 2.2689264f;
    return 1;
}

void func_800B3C18(u8 i) {
    if (i < 2) {
        D_800F60A0[i] = 0.0f;
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.0f;
        D_800F60B8[i] = 2.2689264f;
    }
}

void func_800B3C74(u8 i) {
    if (i < 2) {
        D_800F60A8[i] = 0.0f;
        D_800F60C0[i] = 0;
    }
}

void func_800B3CA4(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.5f;
    }
}

void func_800B3CDC(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.0f;
        D_800F60B8[i] = 2.2689264f;
    }
}

void func_800B3D2C(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 1.75f;
    }
}

void func_800B3D64(u8 i) {
    if (i < 2) {
        D_800F60C0[i] = 0;
        D_800F60A8[i] = 2.0f;
    }
}

void func_800B3D9C(u8 i, s32 a, s32 b, s32 c) {
    if (i < 2) {
        D_80123E40[i].x = func_800B1294(a);
        D_80123E40[i].y = func_800B12C4(b);
        D_80123E40[i].z = func_800B1308(c);
    }
}


extern V3f3_ D_80123E58[];
/* TODO (Bug potential): Missing return value in non-void function. Return type is s32 but control
 * reaches end without returning a value. Either change signature to void func_800B3E24(...) or return 0.
 */
s32 func_800B3E24(s32 arg0, s32 a, s32 b, s32 c) {
    u8 i = arg0;

    if (i < 2) {
        D_80123E58[i].x = func_800B1294(a);
        D_80123E58[i].y = func_800B12C4(b);
        D_80123E58[i].z = func_800B1308(c);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B3EAC);

void func_800B44F4(void) {
    D_80123E98 = 0;
    D_80123E94 = 0.75f;
}

void func_800B4510(void) {
    D_80123E98 = 0;
    D_80123E94 = 0.5f;
    D_80123E84 = 2.5307255f;
}

void func_800B4540(void) {
    D_80123E80 = 0;
    D_80123E98 = 0;
    D_80123E94 = 0.5f;
    D_80123E84 = 2.5307255f;
}

/* TODO (Clean version): Type punning integer store to clear an f32 variable (D_80123E94 is f32,
 * assigned 0.75f and 0.5f in func_800B44F4/510). Propose clean assignment:
 *   D_80123E94 = 0.0f;
 *   D_80123E98 = 0;
 */
void func_800B4578(void) {
    (*(s32 *)&D_80123E94) = 0;
    D_80123E98 = 0;
}

void func_800B458C(s32 arg0, s32 arg1, s32 arg2) {
    D_80123E74 = func_800B1294(arg0);
    D_80123E78 = func_800B12C4(arg1);
    D_80123E7C = func_800B1308(arg2);
}

void func_800B45E4(s32 arg0, s32 arg1, s32 arg2) {
    D_80123E88 = func_800B1294(arg0);
    D_80123E8C = func_800B12C4(arg1);
    D_80123E90 = func_800B1308(arg2);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B463C);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B4AA8);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B4CC4);

void func_800B4DF8(s32 arg0) {
    Ls2C_ *l = (Ls2C_ *)arg0;
    u8 i;
    u8 *src;

    i = 0;
    if (l->n != 0) {
        src = &D_80100BD8;
        do {
            l->it[i].b0 = *src;
            l->it[i].b18 = 0;
            i++;
        } while (i < l->n);
    }
}

void func_800B4E64(ObjG1_ *o) {
    o->unk14 = o->unk10;
}


void func_800B4E74(Scroll4E_ *t) {
    u8 *s;
    s32 x;
    s32 n;

    s = D_801410E0[t->idx];
    x = t->x;
    n = 0;
    for (; *s != 0; s++) {
        if (t->lo < x && x < t->hi) {
            func_800B32F4(*s, 1, x, t->y, 1, 0xF0, 0xF0, 0xF0);
            n++;
        }
        if (s[1] != 0) {
            x += D_80141008(0, *s) + func_80086F80(0, *s, s[1]);
        } else {
            x += D_80141008(0, *s);
        }
    }
    if (n == 0 && t->x < t->lo) {
        t->x = t->reset;
        return;
    }
    t->x -= func_800B3024(4) * 45.0f;
}

/* TODO (Clean version): Fix sprintf prototype and remove int-pointer casts:
 *   int sprintf(char *str, const char *format, ...);
 *   sprintf((char *)out, (const char *)&D_80100CA0, name);
 * Also replace magic bound 18 with ARRAY_COUNT(D_800F61F0).
 */
s32 func_800B5010(char *name, u8 *out) {
    u8 i;
    u8 j;

    for (i = 0; i < 18; i++) {
        if (strcmp(name, D_800F61F0[i].name) == 0) {
            for (j = 0; j < D_800F61F0[i].n; j++) {
                out[j] = D_800F61F0[i].v[j];
            }
            out[j] = 0;
            return 1;
        }
    }
    sprintf((s32 *)out, (s32 *)&D_80100CA0, (s32)name);
    return 0;
}

/* TODO (Clean version): D_800FA0DC is an array of 41 string pointers (char *table[41]).
 * Declare properly as:
 *   extern const char *D_800FA0DC[41];
 * And refactor accessors cleanly without raw byte pointer arithmetic:
 *   const char *func_800B5124(u8 id) { return D_800FA0DC[id]; }
 *   s8 func_800B513C(const char *s) {
 *       for (u8 i = 0; i < 41; i++) {
 *           if (D_800FA0DC[i] != NULL && strcmp(s, D_800FA0DC[i]) == 0) return i;
 *       }
 *       return -1;
 *   }
 */
s32 func_800B5124(u32 arg0) {
    return *(s32 *)((u8 *)&D_800FA0DC + ((arg0 & 0xFF) * 4));
}

s8 func_800B513C(char *s) {
    u8 i;

    for (i = 0; i < 41; i++) {
        if (((char **)&D_800FA0DC)[i] != NULL && strcmp(s, ((char **)&D_800FA0DC)[i]) == 0) {
            return i;
        }
    }
    return -1;
}


/* TODO (Clean version): Structure definition for 0x140-byte player profile / save record:
 *   typedef struct {
 *       u8 pad0[8];
 *       char name[12];           // 0x08 (default "Player %d")
 *       u32 magic;               // 0x14 (0xB48EE52E)
 *       s32 unlockMaskLo;        // 0x18 (default 0x10B)
 *       s32 unlockMaskHi;        // 0x1C (default 0xE3F)
 *       u8 defaultCar;           // 0x20 (default 0x1D)
 *       u8 selectedCourse;       // 0x21
 *       u8 pad22[2];
 *       s32 records[6];          // 0x24..0x38
 *       s32 totalTime;           // 0x3C
 *       u8 controllerCfg[0x100]; // 0x40
 *   } PlayerProfile; // sizeof = 0x140
 */
void func_800B51C0(void *arg0, s32 arg1) {
    u8 *o = arg0;
    s32 buf[4];
    u8 i = arg1;
    u8 j;

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, D_8014122C, i + 1);
    strcpy((s32 *)(o + 8), (s32)buf);
    *(u32 *)(o + 0x14) = 0xB48EE52E;
    *(s32 *)(o + 0x18) = 0x10B;
    *(s32 *)(o + 0x1C) = 0xE3F;
    D_801455B0(i);
    D_801455AC((s32)(o + 0x40), i);
    o[0x20] = 0x1D;
    o[0x21] = 0;
    o[0x22] = 0;
    o[0x23] = 0;
    for (j = 0; j < 6; j++) {
        ((s32 *)(o + 0x24))[j] = 0;
    }
    *(s32 *)(o + 0x3C) = 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B5294);

void func_800B5478(void) {
    if (func_800AF910() != 0) {
        func_800C9F18(&D_800F9F70, 0x19);
    } else {
        func_800C9F18(&D_800F9F70, 0x1A);
    }
}


void func_800B54C0(void) {
    if (D_800F6418 != 0) {
        func_800C9F18(&D_800F9F70, 0x2F);
    } else {
        func_800C9F18(&D_800F9F70, 0x30);
    }
}


void func_800B5504(void) {
    void (**fp)(s32, s32);

    func_800B5294(&D_80124160);
    func_800B51C0(&D_801249F0, 0);
    func_800B51C0(&D_80124B30, 1);
    if (func_800AFD1C() == 0) {
        func_800C9F18(&D_800F9F70, 0x23);
        return;
    }
    D_800F641C = func_800B027C();
    D_80124148 = func_800B0364(0);
    D_8012414C = func_800B0364(1);
    if (D_800F641C == NULL) {
        D_800F641C = &D_80124160;
        D_80124148 = (s32)&D_801249F0;
        D_8012414C = (s32)&D_80124B30;
        func_800C9F18(&D_800F9F70, 0x23);
        return;
    }
    if (D_80124148 == 0) {
        D_80124148 = (s32)&D_801249F0;
    }
    if (D_8012414C == 0) {
        D_8012414C = (s32)&D_80124B30;
    }
    D_8014105C(0xFF);
    fp = &D_801455A8;
    (*fp)(D_80124148 + 0x40, 0);
    (*fp)(D_8012414C + 0x40, 1);
    D_800F6418 = 1;
    func_800C9F18(&D_800F9F70, 0x22);
}

/* TODO (Clean version): Consolidate repeated 4-controller button polling with a helper macro:
 *   #define CHECK_ANY_CONTROLLER_BTN(btn) \
 *       (func_800C8214(0, (btn)) || func_800C8214(1, (btn)) || \
 *        func_800C8214(2, (btn)) || func_800C8214(3, (btn)))
 * And define named constants for button events (0xE, 0xF, 0x12) and SFX IDs (0x2E, 0x2F, 0x32).
 */
void func_800B5680(void) {
    void (**fp)(s32);

    fp = &D_8014556C;
    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if ((u8) D_800F61D5 == 4) {
        if (func_800C8214(0, 0xF) || func_800C8214(1, 0xF) || func_800C8214(2, 0xF) || func_800C8214(3, 0xF)) {
            D_80141068(0xFF);
            D_80140FAC(0x2F, 1, 1.0f, 1.0f);
            func_800C9F18(&D_800F9F70, 8);
        }
    } else if (func_800C8214(0, 0xE) || func_800C8214(1, 0xE) || func_800C8214(2, 0xE) || func_800C8214(3, 0xE)) {
        D_80141068(0xFF);
        D_80140FAC(0x2E, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 6);
    }
    if (func_800C8214(0, 0x12) || func_800C8214(1, 0x12) || func_800C8214(2, 0x12) || func_800C8214(3, 0x12)) {
        D_80141068(0xFF);
        D_80140FAC(0x32, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 2);
    }
}

void func_800B58AC(void) {
    func_800B4CC4((&D_800F5F08)[4], 0, D_80141234, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, D_80141238, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B5A54(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CAC, (s32)D_80141234, D_80124C70);
    func_800B4CC4((&D_800F5F08)[4], 0, buf, 320, 150, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, D_8014124C, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_80141238, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141250, 320, 210, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141254, 320, 250, 0, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141248, 320, 270, 0, 0, 1, 1, 127, 127, 127);
}


void func_800B5C18(void) {
    func_800B4CC4((&D_800F5F08)[4], 0, D_80141454, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, D_80141458, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141254, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B5DC0);

void func_800B5FD4(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CAC, (s32)D_80141234, D_80124C70);
    func_800B4CC4((&D_800F5F08)[4], 0, buf, 320, 150, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, (s32 *)&D_80100D08, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, (s32 *)&D_80100D20, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, (s32 *)&D_80100CC8, 320, 210, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141244, 320, 250, 0, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, (s32 *)&D_80100CF0, 320, 270, 0, 0, 1, 1, 127, 127, 127);
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B6198);

void func_800B623C(void) {
    void (**fp)(s32);

    fp = &D_8014556C;
    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if (func_800C8214(0, 0x12) || func_800C8214(1, 0x12) || func_800C8214(2, 0x12) || func_800C8214(3, 0x12)) {
        D_80141068(0xFF);
        D_80140FAC(0x32, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 2);
    }
    if ((u8) D_800F61D5 < 5 && (func_800C8214(0, 0xF) || func_800C8214(1, 0xF) || func_800C8214(2, 0xF) || func_800C8214(3, 0xF))) {
        D_80141068(0xFF);
        D_80140FAC(0x2F, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 8);
    }
    if ((u8) D_800F61D5 >= 4 && (func_800C8214(0, 0xE) || func_800C8214(1, 0xE) || func_800C8214(2, 0xE) || func_800C8214(3, 0xE))) {
        D_80141068(0xFF);
        D_80140FAC(0x2E, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 6);
    }
}

void func_800B6478(void) {
    if ((u8)D_800F61D5 >= 4) {
        D_800F61D5 = (u8)D_800F61D5 - 1;
    }
}


void func_800B649C(void) {
    if ((u8)D_800F61D5 < 5) {
        D_800F61D5 = (u8)D_800F61D5 + 1;
    }
}


void func_800B64C0(void) {
    switch ((u8)D_800F61D5) {
        case 4:
            func_800C9F18(&D_800F9F70, 0x26);
            break;
        case 5:
            func_800C9F18(&D_800F9F70, 0x28);
            break;
        case 3:
        default:
            func_800C9F18(&D_800F9F70, 0x27);
            break;
    }
}


void func_800B6534(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, D_80124C70);
    func_800B4CC4((&D_800F5F08)[4], 0, buf, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, D_80141464, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_80141468, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_8014146C, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141248, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141244, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B66F8(void) {
    func_800B4CC4((&D_800F5F08)[4], 0, D_80141234, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, (s32 *)&D_80100D38, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141248, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141244, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B68A0(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, D_80124C70);
    func_800B4CC4((&D_800F5F08)[4], 0, buf, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, (s32 *)&D_80100D48, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, (s32 *)&D_80100D58, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, (s32 *)&D_80100D68, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, (s32 *)&D_80100D7C, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}




void func_800B6A64(void) {
    func_800B4CC4((&D_800F5F08)[4], 0, D_80141258, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, D_8014125C, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B6C0C(void) {
    func_800B4CC4((&D_800F5F08)[4], 0, D_80141470, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, D_80141474, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141478, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B6DB4(void) {
    func_800B4CC4((&D_800F5F08)[4], 0, D_80141234, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, (s32 *)&D_80100D88, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, D_8014123C, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, D_80141240, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_80141244, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, D_80141248, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B6F5C(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, D_80124C70);
    D_800F61D5 = 4;
    func_800B4CC4((&D_800F5F08)[4], 0, (s32 *)&D_80100CC8, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, buf, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, (s32 *)&D_80100D94, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, (s32 *)&D_80100CC8, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_8014158C, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, (s32 *)&D_80100CC8, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B7130(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100CA4, (s32)D_80141234, D_80124C70);
    D_800F61D5 = 4;
    func_800B4CC4((&D_800F5F08)[4], 0, (s32 *)&D_80100CC8, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 1, buf, 320, 180, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 2, (s32 *)&D_80100DAC, 320, 200, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 3, (s32 *)&D_80100CC8, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[4], 4, D_8014158C, 320, 250, 1, 0, 1, 1, 127, 127, 127);
    func_800B4CC4((&D_800F5F08)[4], 5, (s32 *)&D_80100CC8, 320, 270, 1, 0, 1, 1, 127, 127, 127);
}


void func_800B7304(void) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (func_800AFB7C(i) == 4) {
            if (D_80140FFC(i) != 0) {
                func_800C9F18(&D_800F9F70, 0x29);
            } else {
                func_800C9F18(&D_800F9F70, 0x2A);
            }
            return;
        }
    }
    func_800C9F18(&D_800F9F70, 0x11);
}


void func_800B73A8(void) {
    void (**fp)(s32) = &D_8014556C;

    (*fp)(0);
    (*fp)(1);
    (*fp)(2);
    (*fp)(3);
    if (func_800C8214(0, 18) != 0 || func_800C8214(1, 18) != 0 || func_800C8214(2, 18) != 0 ||
        func_800C8214(3, 18) != 0) {
        D_80141068(0xFF);
        D_80140FAC(0x2F, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 2);
    }
}


/* TODO (Clean version): Simplify 6 consecutive 4-iteration loops with a prioritized table lookup:
 *   static const struct { u8 status; u8 targetMenuState; } sStatusPriority[] = {
 *       { 4, 0x1F }, { 9, 0x20 }, { 3, 0x1E }, { 8, 0x25 }, { 2, 0x1D }, { 7, 0x24 }
 *   };
 * Also replace magic status integers with an enum (e.g., CONTROLLER_STATUS_REMOVED, MEMCARD_STATUS_CORRUPT).
 */
void func_800B7494(void) {
    s32 st[4];
    s32 i;
    s32 found;
    s32 first;

    first = 0xFF;
    found = 0;
    for (i = 0; (u8) i < 4; i++) {
        st[(u8) i] = func_800AFB7C(i);
        if (!(u8) found && st[(u8) i] != 1) {
            first = i;
            found = 1;
        }
    }
    if ((u8) first == 0xFF) {
        func_800C9F18(&D_800F9F70, 0x1C);
        return;
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 4) {
            D_80124C70 = i + 1;
            func_800C9F18(&D_800F9F70, 0x1f);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 9) {
            D_80124C70 = i + 1;
            func_800C9F18(&D_800F9F70, 0x20);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 3) {
            D_80124C70 = i + 1;
            func_800C9F18(&D_800F9F70, 0x1e);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 8) {
            D_80124C70 = i + 1;
            func_800C9F18(&D_800F9F70, 0x25);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 2) {
            D_80124C70 = i + 1;
            func_800C9F18(&D_800F9F70, 0x1d);
            return;
        }
    }
    for (i = 0; (u8) i < 4; i++) {
        if (st[(u8) i] == 7) {
            D_80124C70 = i + 1;
            func_800C9F18(&D_800F9F70, 0x24);
            return;
        }
    }
    func_800C9F18(&D_800F9F70, 0x21);
}

void func_800B7744(void) {
    D_800F61D5 = 4;
    func_800B3144(0);
    func_800B8DA8();
    func_800B2AD0(D_800F5F18);
}

void func_800B7780(void) {
    D_800F61D5 = 3;
}

void func_800B7790(void) {
    func_800B2BAC(D_800F5F18);
    func_800B2BAC(D_800F5F1C);
}

void func_800B77C0(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[4], D_800F61D5, -1, -1, -1, -1, -1, 0);
    func_800B32D0();
}

void func_800B781C(void) {
    D_800F61D5 = 4;
}

void func_800B782C(void) {
    D_800F61D5 = 5;
}

/* TODO (Clean version): Avoid raw offset indexing into game state D_800F641C.
 * Offsets 0x884 and 0x888 correspond to player profile IDs/hashes in the 0x88C save header:
 *   typedef struct {
 *       u8 data[0x884];
 *       s32 player1Id; // 0x884
 *       s32 player2Id; // 0x888
 *   } GameSaveData_;
 * Also replace ((s32 *)p)[1] with player2->id.
 */
void func_800B783C(void) {
    E140b_ buf;
    u8 found;
    u8 slot;
    u8 i;
    void *p;

    found = 0;
    slot = 0;
    for (i = 0; i < 4; i++) {
        if (func_800AFB7C(i) == 3) {
            found = func_800AFC40(i);
            slot = i;
            break;
        }
    }
    if (!found) {
        func_800C9F18(&D_800F9F70, 0x2B);
        return;
    }
    func_800AFD1C();
    D_800F641C = (u8 *)func_800B02E4(slot);
    func_800B5294(D_800F641C);
    func_800B51C0(&buf, 0);
    func_800B0454(slot, &buf);
    func_800B51C0(&buf, 1);
    func_800B0454(slot, &buf);
    D_80124148 = (s32)func_800B0310(slot, 0);
    p = func_800B0310(slot, 1);
    *(s32 *)(D_800F641C + 0x884) = *(s32 *)(D_80124148 + 4);
    D_8012414C = (s32)p;
    *(s32 *)(D_800F641C + 0x888) = ((s32 *)p)[1];
    func_800B0024();
    D_800F6418 = 1;
    func_800C8110();
}

void func_800B7994(void) {
    u8 found = 0xFF;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (func_800AFB7C(i) != 0 && func_800AFB7C(i) != 1) {
            found = i;
            break;
        }
    }
    D_80124C70 = found + 1;
    if (found != 0xFF && func_800AFB7C(i) == 7) {
        func_800C9F18(&D_800F9F70, 0x24);
    } else {
        func_800C9F18(&D_800F9F70, 0x2C);
    }
}

void func_800B7A5C(void) {
    D_800F6418 = 0;
    D_800F641C = &D_80124160;
    func_800B5294(&D_80124160);
    D_80124148 = (s32)&D_801249F0;
    func_800B51C0(&D_801249F0, 0);
    D_8012414C = (s32)&D_80124B30;
    func_800B51C0(&D_80124B30, 1);
    D_8014105C(0xFF);
}


void func_800B7ACC(void) {
    s32 x;

    D_800F6418 = 0;
    *(Big88C_ *)&D_80124160 = *(Big88C_ *)D_800F641C;
    *(Big140_ *)&D_801249F0 = *(Big140_ *)D_80124148;
    *(Big140_ *)&D_80124B30 = *(Big140_ *)D_8012414C;
    x = D_80100DBC;
    D_800F641C = &D_80124160;
    D_80124148 = (s32)&D_801249F0;
    D_8012414C = (s32)&D_80124B30;
    *(s32 *)&D_800F6424 = x;
    if (D_80123EA0 == 2) {
        *(s32 *)&D_800F6444 = x;
    }
}

void func_800B7C0C(void) {
    if ((*(u8 *)&D_800F61D5) == 4) {
        func_800C9F18(&D_800F9F70, 0x27);
    } else {
        func_800C9F18(&D_800F9F70, 0x26);
    }
}


void func_800B7C54(void) {
    if (((s32 *)D_80124148)[5] == 0) {
        func_800B51C0((void *)D_80124148, 0);
    }
    if (((s32 *)D_8012414C)[5] == 0) {
        func_800B51C0((void *)D_8012414C, 1);
    }
    if ((u8)func_800B0024() != 0) {
    if (((s32 *)D_80124148)[1] == 0) {
        func_800C9F18(&D_800F9F70, 0x31);
    } else {
        func_800B0F74(D_80124148, (s32 *)&D_800F6424);
    }
    if (D_80123EA0 == 2 && ((s32 *)D_8012414C)[1] == 0) {
        func_800C9F18(&D_800F9F70, 0x31);
    } else {
        func_800B0F74(D_8012414C, (s32 *)&D_800F6444);
    }
    func_800C9F18(&D_800F9F70, 0x33);
    } else {
        func_800C9F18(&D_800F9F70, 0x32);
    }
}

s32 func_800B7D60(void) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (func_800AFB7C(i) == 2) {
            return 1;
        }
    }
    return 0;
}


void func_800B7DB4(void) {
    if (func_800B7D60() & 0xFF) {
        func_800C9F18((s32) &D_800F9F70, 0x1D);
    }
    func_800C9F18((s32) &D_800F9F70, 0x21);
}

s32 func_800B7DFC(u8 i, s32 p) {
    if (((s32 *)&D_800FA0DC)[i] == 0) {
        return 0;
    }
    if (i >= 32) {
        return (*(u32 *)(p + 0x18) >> (i - 32)) & 1;
    }
    return (*(u32 *)(p + 0x14) >> i) & 1;
}

u8 func_800B7E48(u8 i) {
    u8 r;

    if (D_800F9FA0[(u8)(i / 3)][(u8)(i % 3)] == 0) {
        return 0;
    }
    r = 0;
    if (((((u32 *)D_80124148)[7] >> i) & 1) || ((((u32 *)D_8012414C)[7] >> i) & 1)) {
        r = 1;
    }
    return r;
}


u8 func_800B7EE8(u8 i) {
    u8 r;
    u32 w;

    switch (i) {
        case 0:
            return 1;
        case 1:
            w = ((u32 *)D_80124148)[7];
            r = 0;
            if ((w >> 7) & 1) {
                r = (w >> 8) & 1;
            }
            return r;
        case 2:
            return func_800B7DFC(0x26, D_80124148);
    }
    return 0;
}

/* TODO (Bug potential): Off-by-one bit shift. For id >= 32, this sets bit (id - 31) instead of (id - 32).
 * In func_800B7DFC, checking id >= 32 reads (bits18 >> (id - 32)) & 1.
 * As a result, when id == 32, bit 1 is set instead of bit 0, leaving entry 32 permanently locked
 * and shifting all upper unlocks by 1 bit. Furthermore, id == 62 shifts by 31, and id == 63 shifts by 32 (UB on 32-bit int).
 * Correct code:
 *   p->bits18 |= 1 << (id - 32);
 */
void func_800B7F74(u8 id, Plr53_ *p) {
    if (id >= 32) {
        p->bits18 |= 1 << (id - 31);
        return;
    }
    p->bits14 |= 1 << id;
}

void func_800B7FB4(s32 id, Plr53_ *p) {
    p->bits1C |= 1 << id;
}

void func_800B7FCC(u8 player) {
    Plr53_ *p;

    if (!player) {
        p = (Plr53_ *)D_80124148;
    } else {
        p = (Plr53_ *)D_8012414C;
    }
    do {
        if (p->sel < 40) {
            p->sel++;
        } else {
            p->sel = 0;
        }
    } while (!(u8)func_800B7DFC(p->sel, (s32)p));
}

void func_800B8038(u8 player) {
    Plr53_ *p;

    if (!player) {
        p = (Plr53_ *)D_80124148;
    } else {
        p = (Plr53_ *)D_8012414C;
    }
    do {
        if (p->sel != 0) {
            p->sel--;
        } else {
            p->sel = 40;
        }
    } while (!(u8)func_800B7DFC(p->sel, (s32)p));
}

/* TODO (Bug potential): Potential infinite loop if all tracks are locked or invalid.
 * If func_800B7E48() returns 0 for all 12 indices (0..11), this loop will never terminate.
 * Propose adding a loop counter / max iterations guard:
 *   for (u8 count = 0; count < 12; count++) { ... }
 */
void func_800B80B0(void) {
    u8 v;

    for (;;) {
        if (((u8 *)D_80124148)[0x21] < 11) {
            ((u8 *)D_80124148)[0x21]++;
        } else {
            ((u8 *)D_80124148)[0x21] = 0;
        }
        if (func_800B7E48(((u8 *)D_80124148)[0x21]) == 0) {
            continue;
        }
        v = ((u8 *)D_80124148)[0x21];
        if (D_800F9FA0[(u8)(v / 3)][(u8)(v % 3)] != 0) {
            break;
        }
    }
    ((u8 *)D_8012414C)[0x21] = v;
}


void func_800B8184(void) {
    u8 v;

    for (;;) {
        if (((u8 *)D_80124148)[0x21] != 0) {
            ((u8 *)D_80124148)[0x21]--;
        } else {
            ((u8 *)D_80124148)[0x21] = 11;
        }
        if (func_800B7E48(((u8 *)D_80124148)[0x21]) == 0) {
            continue;
        }
        v = ((u8 *)D_80124148)[0x21];
        if (D_800F9FA0[(u8)(v / 3)][(u8)(v % 3)] != 0) {
            break;
        }
    }
    ((u8 *)D_8012414C)[0x21] = v;
}


void func_800B8260(void) {
    *(u8 *)&D_800F6464 = D_8014104C(7);
}


void func_800B828C(void) {
    D_800F6464 = func_8008D4B8((u32) (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x21)));
}

void func_800B82B8(void) {
    if ((u8)D_800F6464 < 6) {
        D_800F6464 = (u8)D_800F6464 + 1;
    } else {
        D_800F6464 = 0;
    }
}


void func_800B82E8(void) {
    u8 v = D_800F6464;

    if (v == 0) {
        v = 6;
    } else {
        v--;
    }
    D_800F6464 = v;
}


extern void (*D_8014C0AC)(u8);

void func_800B8308(void) {
    D_8014C0AC(*(u8 *)&D_800F6464);
}


void func_800B8334(void) {
    D_8014C0B4();
}

void func_800B8358(void) {
    switch (D_80124C71) {
        case 0:
            func_800B8260();
            break;
        case 1:
            func_800B828C();
            break;
        case 2:
            break;
    }
    func_800B8308();
}


void func_800B83B0(void) {
    u8 v;

    if (D_80124C71 == 2) {
        D_80124C72 = *(u8 *)&D_800F6464;
        return;
    }
    v = D_8014104C(7);
    D_80124C72 = v;
    *(u8 *)&D_800F6464 = v;
}

extern void (*D_8014C0AC)(u8);
void func_800B8408(void) {
    u8 v = D_800F6466;

    if (v == 0) {
        D_800F6466 = 1;
        D_8014C0AC(D_80124C72);
    }
}

void func_800B8448(void) {
    D_800F6466 = 0;
    D_8014C0B4();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B8470);

void func_800B8520(void) {
    D_80124C73 = 0;
}

void func_800B852C(void) {
    D_80124C73 = 1;
}

void func_800B853C(void) {
    D_80124C73 = 2;
}

void func_800B854C(void) {
    D_800F6468 = D_80141048();
}

/* TODO (Clean version / Bug check): Idle timeout check (30,000 ms = 30 seconds).
 * Using (now > start + 30000) causes an early trigger or miss on 32-bit timer overflow (wraps around ~49.7 days).
 * Standard delta time check is overflow-safe:
 *   #define IDLE_TIMEOUT_MS 30000
 *   if ((u32)(D_80141048() - D_800F6468) > IDLE_TIMEOUT_MS) {
 *       func_800C9F18(&D_800F9F70, 0x38);
 *   }
 */
void func_800B8578(void) {
    if ((u32)D_80141048() > (u32)(D_800F6468 + 30000)) {
        func_800C9F18(&D_800F9F70, 0x38);
    }
}


void func_800B85C4(void) {
    u8 i;

    func_800B2A84();
    for (i = 0; i < 33; i++) {
        func_800B2A20((&D_800F5F08)[i]);
    }
}


void func_800B8620(void) {
    func_800B2A84();
}

void func_800B863C(void) {
    D_80141084();
}

/* TODO (Clean version): D_801411CC is a loaded configuration/calibration block.
 * Contiguous scalar globals D_8012406C..D_80124088 (8 words) and D_80124110 (13 words)
 * should be mapped to structured settings:
 *   typedef struct { s32 val[8]; } SystemSettings;
 *   typedef struct { s32 val[13]; } AudioCalibrationSettings;
 */
void func_800B8660(void) {
    u8 i;

    if (!func_80086E40(&D_80100DC0)) {
        func_800C9F18(&D_800F9F70, 0x12);
    }
    if (!func_80086BE0(&D_80100DC8)) {
        func_800C9F18(&D_800F9F70, 0x12);
    }
    D_8012406C = D_801411CC[0];
    D_80124070 = D_801411D0;
    D_80124074 = D_801411D4;
    D_80124078 = D_801411D8;
    D_8012407C = D_801411DC;
    D_80124080 = D_801411E0;
    D_80124084 = D_801411E4;
    D_80124088 = D_801411CC[0];
    for (i = 0; i < 13; i++) {
        D_80124110[i] = D_801411CC[i + 11];
    }
}

void func_800B877C(void) {
    if (!(func_800C9E70() & 0xFF)) {
        func_800C9F18((s32) &D_800F9F70, 0x12);
    }
}

void func_800B87B4(void) {
    func_80086F78();
}

void func_800B87D0(void) {

}

void func_800B87D8(void) {
    if (D_800F6418 == 0) {
        *func_800B02E4(0) = *(Big88C_ *)&D_80124160;
    }
    func_800B8448();
    D_80141054(D_800F641C[0x87D]);
    D_8014105C(D_800F641C[0x87C]);
    D_80141064(D_800F641C[0x880]);
    D_80141068(0xFF);
    func_800C9130();
    func_800C9A00();
    D_8014107C();
    D_80141088();
    func_800B8620();
}

void func_800B88F8(void) {
    func_800C90DC();
    func_800C9978();
    func_800B85C4();
    D_80141084();
    D_80141068(0);
    D_8014105C(0xFF);
    func_800B83B0();
    func_800B8408();
    if (D_800F6418 == 0) {
        *(Big88C_ *)&D_80124160 = *func_800B02E4(0);
    }
    func_800C65AC(&D_80123F48);
}


extern u8 (*D_80140F90)(void);
void func_800B89DC(void) {
    if (D_80140F90() == 0) {
        func_800C9F18(&D_800F9F70, 0x12);
    }
}

void func_800B8A1C(void) {
    D_80140F94();
}

/* TODO (Clean version): Millisecond time formatting helpers. Propose unifying into inline helpers or a struct:
 *   typedef struct { u16 hours; u8 minutes; u8 seconds; u8 centiseconds; } FormattedTime;
 *   static inline u16 Time_GetHours(u32 ms)        { return (ms / 3600000); }
 *   static inline u16 Time_GetTotalMinutes(u32 ms) { return (ms / 60000); }
 *   static inline u32 Time_GetSeconds(u32 ms)      { return (ms / 1000) % 60; }
 *   static inline u32 Time_GetCentiseconds(u32 ms) { return (ms % 1000) / 10; }
 */
u16 func_800B8A40(u32 ms) {
    u32 s = ms / 1000;
    u32 m = s / 60;
    return m / 60;
}


u16 func_800B8A84(u32 ms) {
    u32 s = ms / 1000;
    return s / 60;
}

u32 func_800B8AB8(u32 arg0) {
    return ((arg0 / 1000) % 60) & 0xFFFF;
}

u32 func_800B8AF8(u32 arg0) {
    return ((arg0 % 1000) / 10) & 0xFFFF;
}

void func_800B8B44(void) {
    if (D_80141080() != 0) {
        func_800C9F18(&D_800F9F70, 0x13);
    } else {
        func_800C9F18(&D_800F9F70, 0x14);
    }
}


void func_800B8B94(void) {
    func_800B3264();
    func_800B4AA8((&D_800F5F08)[1], -1, -1, -1, -1, -1, -1, 0);
    func_800B32D0();
}

void func_800B8BE4(void) {
    if (D_801455A4() != 0) {
        func_800C9F18(&D_800F9F70, 0x15);
    } else {
        func_800C9F18(&D_800F9F70, 0x16);
    }
}


void func_800B8C34(void) {
    func_800B3264();
    func_800B4AA8((&D_800F5F08)[2], -1, -1, -1, -1, -1, -1, 0);
    func_800B32D0();
}

void func_800B8C84(void) {
    func_800B2AD0(D_800F5F08);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B8CA8);

void func_800B8CF8(void) {
    func_800C8554(0x50);
}

void func_800B8D14(void) {
    func_800B3144(&D_80100DD0);
}

void func_800B8D34(void) {
    func_800B3264();
    func_800B32D0();
}

void func_800B8D58(void) {
    D_80124144 = 0;
    D_80124145 = 0;
    D_80124146 = 0;
}

void func_800B8D74(void) {
    D_80124144 = 1;
    D_80124146 = 0;
}

void func_800B8D8C(void) {
    D_80124144 = 2;
    D_80124146 = 1;
}

void func_800B8DA8(void) {
    D_80124144 = 3;
    D_80124146 = 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800B8DC4);

void func_800B8FD8(void) {
    func_800B8D74();
    func_800B3144(&D_80100DD0);
    func_800B2AD0(D_800F5F20);
}

void func_800B900C(void) {
    func_800B8D74();
    func_800B3144(&D_80100DDC);
    func_800B2AD0(D_800F5F20);
}

void func_800B9040(void) {
    func_800B8D74();
    func_800B3144(&D_80100DE4);
    func_800B2AD0(D_800F5F20);
}

void func_800B9074(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[6], -1, 0, -1, -1, -1, -1, D_80124146);
    func_800B32D0();
}

void func_800B90D4(void) {
    func_800B2844((*(void * *)&D_800F5F20), 6U);
}

void func_800B90F8(void) {
    func_800B8D8C();
}

void func_800B9114(void) {
    func_800B3144(&D_80100DEC);
    func_800B8D74();
    func_800B2AD0(D_800F5F24);
    D_80124C74 = 0;
    D_80124C75 = 0;
}

void func_800B915C(void) {
    func_800B2BAC(D_800F5F24);
}

void func_800B9180(void) {
    func_800B3264();
    func_800B8DC4();
    if (*(u8 *)&D_80124C75 == 0 && (u8)D_80124145 == 0xFF) {
        D_80124C75 = 1;
        D_80140FAC(0x37, 1, 1.0f, 1.0f);
    }
    func_800B4AA8((&D_800F5F08)[7], -1, 0, -1, -1, -1, -1, D_80124146);
    func_800B32D0();
}

void func_800B922C(void) {
    func_800C8CE4(0x50, 0);
}

void func_800B924C(void) {
    func_800B2844((*(void * *)&D_800F5F24), 7U);
}

void func_800B9270(void) {
    func_800B8D8C();
}

void func_800B928C(void) {
    D_80124C74 = 1;
}

void func_800B929C(void) {
    if ((*(u8 *)&D_80124C74) != 0) {
        func_800C9F18((s32) &D_800F9F70, 2);
    }
}

void func_800B92CC(void) {
    s32 a[8];
    s32 b[8];

    D_80124C7C = 0;
    D_80124C7D = 0;
    func_800B0F74(D_80124148, a);
    func_800B0F74(D_8012414C, b);
    if (strcmp((char *)&D_800F6424, (char *)&D_80100CC8) == 0) {
        strcpy((s32 *)&D_800F6424, (s32)a);
    }
    if (strcmp((char *)&D_800F6444, (char *)&D_80100CC8) == 0) {
        strcpy((s32 *)&D_800F6444, (s32)b);
    }
    if (strcmp((char *)&D_800F6424, (char *)a) != 0) {
        strcpy((s32 *)&D_800F6424, (s32)a);
        D_80124C7C = 1;
    }
    if (strcmp((char *)&D_800F6444, (char *)b) != 0) {
        strcpy((s32 *)&D_800F6444, (s32)b);
        D_80124C7D = 1;
    }
}

void func_800B93D8(void) {
    if (D_80124C7C != 0 || D_80124C7D != 0) {
        D_80124C78 = D_80141048();
        func_800C9F18(&D_800F9F70, 0x34);
    } else {
        func_800C9F18(&D_800F9F70, 0x35);
    }
}

void func_800B9440(void) {
    s32 a[8];
    s32 b[8];
    s32 c[8];
    u8 *src;

    func_800B2AD0(D_800F5F14);
    func_800B3144(NULL);
    func_800B8D74();
    if (D_80124C7C != 0) {
        sprintf(a, (s32 *)&D_80100DF4, D_80124148 + 8);
        src = &D_800F6424;
        D_80124C7C = 0;
        D_80124C7E = 1;
        D_80124C7F = 0;
    } else {
        sprintf(a, (s32 *)&D_80100DF4, D_8012414C + 8);
        src = &D_800F6444;
        D_80124C7D = 0;
        D_80124C7E = 0;
        D_80124C7F = 1;
    }
    strcpy(b, (s32)src);
    ((u8 *)b)[10] = 0;
    strcpy(c, (s32)(src + 10));
    ((u8 *)c)[10] = 0;
    func_800B4CC4((&D_800F5F08)[3], 0, D_80141480, 320, 170, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[3], 1, a, 320, 195, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[3], 2, b, 320, 220, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((&D_800F5F08)[3], 3, c, 320, 245, 0, 0, 1, 1, 255, 255, 255);
}

void func_800B9644(void) {
    func_800B2B28(D_800F5F14);
}

void func_800B9668(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[3], -1, -1, -1, -1, -1, -1, D_80124146);
    func_800B32D0();
}

void func_800B96C8(void) {
    if ((u32)D_80141048() > (u32)(D_80124C78 + 5000)) {
        if (D_80124C7E != 0) {
            func_800C8554(0x50);
        } else if (D_80124C7F != 0) {
            func_800C891C(0x50);
        }
    }
}

void func_800B973C(s32 arg0) {
    *(s32 *)((u8 *)&D_80124C8C + ((arg0 & 0xFF) * 4)) = D_80141048();
}

void func_800B977C(void) {
    D_80124C84 = 0;
}

void func_800B9788(void) {
    D_80124C88 = 0;
}

void func_800B9794(u8 i) {
    if (((u32 *)&D_80124C84)[i] == 0) {
        ((u32 *)&D_80124C84)[i] = 500;
    } else {
        ((u32 *)&D_80124C84)[i] >>= 1;
        if (((u32 *)&D_80124C84)[i] < 250) {
            ((u32 *)&D_80124C84)[i] = 250;
        }
    }
}


s32 func_800B97D8(u8 i) {
    u32 now = D_80141048();

    return ((u32 *)&D_80124C8C)[i] + ((u32 *)&D_80124C84)[i] < now;
}


void func_800B982C(void) {
    (*(s8 *)&D_80123EA0) = 1;
}

void func_800B983C(s32 i, s32 x, s32 y, s32 c) {
    TBL_ *t = (TBL_ *)D_800F5F28;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_800F646C[(u8)i]) == 0) {
            if ((x < 0) & (y < 0)) {
                t->objs[j].h30 = (u8)c;
                return;
            }
            t->objs[j].h30 = (u8)c;
            t->objs[j].x = func_800B1294(x);
            t->objs[j].y = func_800B12C4(y);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
            return;
        }
    }
}


void func_800B999C(void) {
    switch (D_800F61D2) {
        case 1:
            func_800B983C(0, 90, 490, 100);
            func_800B983C(1, 303, 490, 100);
            break;
        case 2:
            func_800B983C(0, 90, 585, 100);
            func_800B983C(1, 303, 585, 100);
            break;
        case 3:
            func_800B983C(0, 90, 682, 100);
            func_800B983C(1, 303, 682, 100);
            break;
        case 5:
            func_800B983C(0, 90, 897, 100);
            func_800B983C(1, 625, 899, 100);
            break;
        default:
            func_800B983C(0, -1, -1, 0);
            func_800B983C(1, -1, -1, 0);
            break;
    }
}

void func_800B9AB4(void) {

    func_800B8D74();
    func_800B3144((s32 *)&D_80100E08);
    func_800B4DF8((&D_800F5F08)[8]);
    func_800B2AD0((&D_800F5F08)[8]);
    D_80124C80 = 0;
    D_80124C81 = 0;
    func_800B3C18(0);
    func_800B3C18(1);
    func_800B3D9C(0, 500, 500, 0);
    func_800B3D9C(1, 500, 500, 0);
    if (D_800F61D2 == 1) {
        func_800B3CA4(0);
        func_800B3CA4(1);
        func_800B3E24(0, 460, 480, 0);
        func_800B3E24(1, 670, 480, 0);
    } else {
        func_800B3E24(0, 450, 450, 0);
        func_800B3E24(1, 760, 450, 0);
    }
    func_800B4540();
    func_800B458C(500, 500, 0);
    if (D_800F61D2 == 2) {
        func_800B44F4();
        func_800B45E4(625, 825, 0);
    } else {
        func_800B45E4(675, 900, 0);
    }
    func_800B999C();
    if (D_800F6418 == 0 && D_800F6101 == 1) {
        D_800F6101 = 0;
    }
    D_801240B8 = 0;
    func_800B4E64(&D_800F6358);
}

void func_800B9C3C(void) {
    if (D_80123EA0 == 1) {
        func_800C9F18((s32) &D_800F9F70, 0x39);
        return;
    }
    if (D_800F61D2 == 3) {
        (*(s8 *)&D_801240B8) = 1;
        func_800C9F18((s32) &D_800F9F70, 0x3A);
        return;
    }
    func_800C8110();
}

void func_800B9CB4(void) {
    (*(s8 *)&D_80123EA0) = 2;
}

void func_800B9CC4(void) {
    (*(s8 *)&D_80123EA0) = 1;
}

void func_800B9CD4(void) {
    D_80140FC8();
}

void func_800B9CF8(void) {
    D_80140FCC();
}

void func_800B9D1C(void) {
    u8 buf[0x40];
    u8 big[0x118];

    D_80140FBC(0, ((char **)&D_800FA0DC)[((u8 *)D_80124148)[0x20]], 0);
    D_80140FB8(((char **)&D_800FA0DC)[((u8 *)D_80124148)[0x20]], big, buf);
    D_80140FD8();
    *(V12_ *)&D_801240E8 = *(V12_ *)(big + 0x80);
    D_80124C94 = 0;
}

void func_800B9DC8(void) {
    u8 buf[0x40];
    u8 big[0x118];

    D_80140FBC(1, ((char **)&D_800FA0DC)[((u8 *)D_8012414C)[0x20]], 0);
    D_80140FB8(((char **)&D_800FA0DC)[((u8 *)D_8012414C)[0x20]], big, buf);
    D_80140FD8();
    *(V12_ *)&D_801240F4 = *(V12_ *)(big + 0x80);
    D_80124C95 = 0;
}


void func_800B9E74(void) {
    D_80140FDC();
    D_80140FB4(0);
}

void func_800B9EA8(void) {
    D_80140FDC();
    D_80140FB4(1);
}

void func_800B9EDC(void) {
    s8 k = func_800B0FB0(&D_80100E10);
    u8 v = ((u8 *)D_80124148)[0x21];

    if (k > 0) {
        D_8014103C(k, D_800F9FA0[(u8)(v / 3)][(u8)(v % 3)]);
    }
}


void func_800B9F78(void) {
    D_80141040(func_800B1064(&D_80100E10) & 0xFF);
    func_800B10F4(&D_80100E10);
}

void func_800B9FBC(void) {
    func_800B2B28(D_800F5F28);
}

void func_800B9FE0(void) {
    func_800B2BAC(D_800F5F28);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BA004);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BA3A8);

/* Checks the entered cheat code, then handles player 1's buttons 0x10/0x11 on this menu. */
void func_800BA584(s32 id) {
    if ((u8)func_800C8554((u16)id) && (u8)Config_CheckCheatCode((char *)func_800C815C(), (void *)D_80124148)) {
        D_80140FAC(0x34, 1, 1.0f, 1.0f);
        func_800C8174();
    }
    if (D_80124C94 && ((D_800F61D2 != 0 && D_800F61D2 < 4) || D_800F61D2 == 5)) {
        if ((u8)func_800C8134(0, 0x10)) {
            if ((u8)func_800B97D8(0)) {
                func_800C9F18(&D_800F9F70, 10);
                func_800B9794(0);
                func_800B973C(0);
                D_800F6474 = 1;
                D_80124C80++;
            }
        } else if ((u8)func_800C8134(0, 0x11)) {
            if ((u8)func_800B97D8(0)) {
                func_800C9F18(&D_800F9F70, 12);
                func_800B9794(0);
                func_800B973C(0);
                D_800F6474 = 1;
                D_80124C80++;
            }
        } else if (D_800F6474) {
            func_800B977C();
            D_800F6474 = 0;
            D_80124C80 = 0;
        }
    }
}

/* Checks the entered cheat code, then handles player 2's buttons 0x10/0x11 on this menu. */
void func_800BA6F8(s32 id) {
    if ((u8)func_800C891C((u16)id) && (u8)Config_CheckCheatCode((char *)func_800C8168(), (void *)D_8012414C)) {
        D_80140FAC(0x34, 1, 1.0f, 1.0f);
        func_800C81A0();
    }
    if (D_80124C95 && D_80123EA0 == 2 && D_800F61D2 == 1) {
        if ((u8)func_800C8134(1, 0x10)) {
            if ((u8)func_800B97D8(1)) {
                func_800C9F18(&D_800F9F70, 11);
                func_800B9794(1);
                func_800B973C(1);
                D_800F6475 = 1;
                D_80124C81++;
            }
        } else if ((u8)func_800C8134(1, 0x11)) {
            if ((u8)func_800B97D8(1)) {
                func_800C9F18(&D_800F9F70, 13);
                func_800B9794(1);
                func_800B973C(1);
                D_800F6475 = 1;
                D_80124C81++;
            }
        } else if (D_800F6475) {
            func_800B9788();
            D_800F6475 = 0;
            D_80124C81 = 0;
        }
    }
}

void func_800BA864(void) {
    u16 a = 0x43;
    u16 b = 0x20;

    if (D_80123EA0 == 1) {
        b = 0x50;
    }
    switch (D_800F61D2) {
    case 2:
    case 5:
        break;
    case 3:
        a |= 0x10;
        b |= 0x10;
        break;
    case 0:
    case 1:
    case 4:
        a |= 0x10;
        break;
    }
    if (D_800F6476 == 0) {
        func_800BA584(a);
        func_800BA6F8(b);
        D_800F6476 = 1;
    } else {
        func_800BA6F8(b);
        func_800BA584(a);
        D_800F6476 = 0;
    }
}

void func_800BA924(void) {
    func_800B2844((*(void * *)&D_800F5F28), 8U);
}

void func_800BA948(void) {
    if (D_800F61D2 == 1) {
        func_800B3CDC(0);
        func_800B3CDC(1);
        func_800B3E24(0, 450, 450, 0);
        func_800B3E24(1, 760, 450, 0);
    }
    if (D_800F61D2 == 2) {
        func_800B4510();
        func_800B45E4(675, 900, 0);
    }
    if (D_800F61D2 == 0) {
        D_800F61D2 = 5;
    } else {
        D_800F61D2 -= 1;
    }
    if (D_800F61D2 == 1) {
        func_800B3CA4(0);
        func_800B3CA4(1);
        func_800B3E24(0, 460, 480, 0);
        func_800B3E24(1, 670, 480, 0);
    }
    if (D_800F61D2 == 2) {
        func_800B44F4();
        func_800B45E4(625, 825, 0);
    }
    func_800B999C();
}


void func_800BAA6C(void) {
    if (D_800F61D2 == 1) {
        func_800B3CDC(0);
        func_800B3CDC(1);
        func_800B3E24(0, 0x1C2, 0x1C2, 0);
        func_800B3E24(1, 0x2F8, 0x1C2, 0);
    }
    if (D_800F61D2 == 2) {
        func_800B4510();
        func_800B45E4(0x2A3, 0x384, 0);
    }
    if ((u8)D_800F61D2 < 5) {
        D_800F61D2++;
    } else {
        D_800F61D2 = 0;
    }
    if (D_800F61D2 == 1) {
        func_800B3CA4(0);
        func_800B3CA4(1);
        func_800B3E24(0, 0x1CC, 0x1E0, 0);
        func_800B3E24(1, 0x29E, 0x1E0, 0);
    }
    if (D_800F61D2 == 2) {
        func_800B44F4();
        func_800B45E4(0x271, 0x339, 0);
    }
    func_800B999C();
}

void func_800BAB9C(void) {
    if (D_80124C80 < 0x7C) {
        D_80140FAC(0x30, 1, 1.0f, 1.0f);
        if (D_800F61D2 == 1 && D_80123EA0 == 2) {
            func_800B8038(1);
        }
    }
}


void func_800BAC10(void) {
    if (D_80124C80 < 0x7C) {
        D_80140FAC(0x31, 1, 1.0f, 1.0f);
        if (D_800F61D2 == 1 && D_80123EA0 == 2) {
            func_800B7FCC(1);
        }
    }
}


void func_800BAC84(void) {
    switch (D_800F61D2) {
        case 1:
            func_800C9F18(&D_800F9F70, 0x3B);
            break;
        case 2:
            func_800C9F18(&D_800F9F70, 0x3C);
            break;
        case 3:
            func_800C9F18(&D_800F9F70, 0x3E);
            break;
        case 5:
            func_800C9F18(&D_800F9F70, 0x3D);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x3F);
            break;
    }
}


void func_800BAD34(void) {
    if (D_80124C80 < 0x7C) {
        D_80140FAC(0x30, 1, 1.0f, 1.0f);
        func_800B8038(0);
    }
}


void func_800BAD84(void) {
    if (D_80124C80 < 0x7C) {
        D_80140FAC(0x31, 1, 1.0f, 1.0f);
        func_800B7FCC(0);
    }
}


void func_800BADD4(void) {
    s32 *p;

    D_80140FAC(0x30, 1, 1.0f, 1.0f);
    p = (s32 *)(D_800F641C + 0x878);
    if (*p == 0) {
        *p = 4;
    } else {
        *p -= 1;
    }
}


void func_800BAE28(void) {
    u32 *p;

    D_80140FAC(0x31, 1, 1.0f, 1.0f);
    p = (u32 *)(D_800F641C + 0x878);
    *p += 1;
    if (*p >= 5) {
        *p = 0;
    }
}


void func_800BAE80(void) {
    D_80140FAC(0x30, 1, 1.0f, 1.0f);
    if (D_800F6418 != 0) {
        if (D_800F6101 != 0) {
            D_800F6101 -= 1;
        } else {
            D_800F6101 = 2;
        }
    } else {
        do {
            if (D_800F6101 != 0) {
                D_800F6101 -= 1;
            } else {
                D_800F6101 = 2;
            }
        } while (D_800F6101 == 1);
    }
}


void func_800BAF2C(void) {
    D_80140FAC(0x31, 1, 1.0f, 1.0f);
    if (D_800F6418 != 0) {
        if (++D_800F6101 >= 3) {
            D_800F6101 = 0;
        }
    } else {
        do {
            if (++D_800F6101 >= 3) {
                D_800F6101 = 0;
            }
        } while (D_800F6101 == 1);
    }
}


void func_800BAFE0(void) {
    D_80140FAC(0x30, 1, 1.0f, 1.0f);
    func_800B8184();
}

void func_800BB020(void) {
    D_80140FAC(0x31, 1, 1.0f, 1.0f);
    func_800B80B0();
}


void func_800BB060(void) {
    switch (*(s32 *)(D_800F641C + 0x878)) {
    case 0:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x40);
        break;
    case 1:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x41);
        break;
    case 2:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x42);
        break;
    case 3:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x43);
        break;
    case 4:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x44);
        break;
    default:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x3F);
        break;
    }
}

void func_800BB104(void) {
    switch (D_800F61D2) {
    case 0:
        switch (*(s32 *)(D_800F641C + 0x878)) {
        case 0:
            func_800C9F18(&(*(s32 *)&D_800F9F70), 0x40);
            break;
        case 1:
            func_800C9F18(&(*(s32 *)&D_800F9F70), 0x41);
            break;
        case 2:
            func_800C9F18(&(*(s32 *)&D_800F9F70), 0x42);
            break;
        case 3:
            func_800C9F18(&(*(s32 *)&D_800F9F70), 0x43);
            break;
        case 4:
            func_800C9F18(&(*(s32 *)&D_800F9F70), 0x44);
            break;
        default:
            func_800C9F18(&(*(s32 *)&D_800F9F70), 0x3F);
            break;
        }
        break;
    case 1:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x46);
        break;
    case 3:
        (*(s8 *)&D_801240B8) = 0;
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x45);
        break;
    case 4:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x47);
        break;
    default:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x3F);
        break;
    }
}

void func_800BB224(void) {
    switch (D_800F6101) {
        case 0:
            func_800C9F18(&D_800F9F70, 0x49);
            break;
        case 1:
            D_80124150 = ((s32 *)D_80124148)[1];
            D_80124154 = ((s32 *)D_8012414C)[1];
            func_800C9F18(&D_800F9F70, 0x4A);
            break;
        case 2:
        default:
            func_800C9F18(&D_800F9F70, 0x4B);
            break;
    }
}


void func_800BB2B4(void) {
    func_800B8D8C();
    func_800B2AFC(D_800F5F28);
    func_800B3C74(0U);
    func_800B3C74(1U);
    func_800B3E24(0U, 0x190, 0x190, 0);
    func_800B3E24(1U, 0x2BC, 0x190, 0);
    func_800B4578();
    func_800B45E4(0x1F4, 0x1F4, 0);
}

void func_800BB32C(void) {
    func_800B2F20(D_800F5F28);
}

void func_800BB350(s32 i, s32 x, s32 y, s32 c) {
    TBL_ *t = (TBL_ *)D_800F5F2C;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_800F6478[(u8)i]) == 0) {
            if ((x < 0) & (y < 0)) {
                t->objs[j].h30 = (u8)c;
                return;
            }
            t->objs[j].h30 = (u8)c;
            t->objs[j].x = func_800B1294(x);
            t->objs[j].y = func_800B12C4(y);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
            return;
        }
    }
}


void func_800BB4B0(void) {
    switch ((u8)D_800F61D3) {
        case 2:
            func_800BB350(0, 100, 590, 100);
            func_800BB350(1, 303, 590, 100);
            break;
        case 3:
            func_800BB350(0, 100, 690, 100);
            func_800BB350(1, 303, 690, 100);
            break;
        case 5:
            func_800BB350(0, 100, 897, 100);
            func_800BB350(1, 620, 897, 100);
            break;
        default:
            func_800BB350(0, -1, -1, 0);
            func_800BB350(1, -1, -1, 0);
            break;
    }
}


/* Race position of a player (0 = first), by score with ties broken by index.
 * TODO(match-debt): the comma expression and the dead 'j = 0' only exist to match. */
s32 func_800BB598(s32 player) {
    s32 rank;
    s32 i;
    u32 p;
    u32 a;
    u32 b;
    u32 j;

    rank = 0;
    i = 0;
    p = player & 0xFF;
    j = 0;
    do {
        j = i & 0xFF;
        if (j != p && ((a = *(s32 *)((u8 *)(j * 4 + D_80124148) + 0x24),
                        b = *(s32 *)((u8 *)(p * 4 + D_80124148) + 0x24), b < a) ||
                       ((a == b) & (j < p)))) {
            rank++;
        }
        i++;
    } while ((u32)(i & 0xFF) < 6);
    return rank & 0xFF;
}

u8 func_800BB610(u8 n) {
    Blk23_ *p = (Blk23_ *)D_80124148;
    u8 r = 0;
    u8 i;

    for (i = 0; i < 6; i++) {
        if (i != n && (u32)p->w24[i] > (u32)p->w24[n]) {
            r++;
        }
    }
    return r;
}


void func_800BB66C(void) {
    if ((*(u8 *)&D_800F6100) != 0) {
        func_800C9F18(&D_800F9F70, 0x36);
    } else {
        func_800C9F18(&D_800F9F70, 0x37);
    }
}


void func_800BB6B0(void) {
    D_800F6100 = 1;
    func_800B3144((s32 *)&D_80100E74);
    func_800B8D74();
    D_80123EA0 = 1;
    D_80124C80 = 0;
    func_800B3C18(0);
    func_800B3D9C(0, 500, 500, 0);
    if ((u8)D_800F61D3 == 2) {
        func_800B3CA4(0);
        func_800B3E24(0, 460, 480, 0);
    } else {
        func_800B3E24(0, 450, 450, 0);
    }
    func_800B4540();
    func_800B458C(500, 500, 0);
    if ((u8)D_800F61D3 == 5) {
        func_800B44F4();
        func_800B45E4(625, 825, 0);
    } else {
        func_800B45E4(675, 900, 0);
    }
    if (D_800F6418 == 0 && (u8)D_800F6102 == 1) {
        D_800F6102 = 0;
    }
    func_800B2AD0(D_800F5F2C);
    func_800B4E64(&D_800F63A0);
    func_800BB4B0();
}

void func_800BB7E8(void) {
    u8 i;

    ((Blk23_ *)D_80124148)->b23 = 0;
    for (i = 0; i < 6; i++) {
        ((Blk23_ *)D_80124148)->w24[i] = 0;
        D_800F60E8[i] = 0;
    }
}

void func_800BB838(void) {
    if (((u8) (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x22)) >= 3U) || ((u8) (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x23)) >= 6U)) {
        (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x22)) = 0U;
        func_800BB7E8();
    }
}

void func_800BB87C(void) {
    s8 k = func_800B0FB0(&D_80100E10);
    u8 v = D_800F60D4[((u8 *)D_80124148)[0x22]][((u8 *)D_80124148)[0x23]];

    if (k > 0) {
        D_8014103C(k, D_800F9FA0[(u8)(v / 3)][(u8)(v % 3)]);
    }
}


void func_800BB93C(void) {
    func_800B2844((*(void * *)&D_800F5F2C), 9U);
}

void func_800BB960(void) {
    func_800B3264();
    func_800B8DC4();
    if (D_80124C94 == 0) {
        D_80124C94 = D_80140FC0(0);
    }
    switch ((u8)D_800F61D3) {
    case 3:
        func_800B4AA8((&D_800F5F08)[9], 6, -1, -1, -1, -1, -1, D_80124146);
        break;
    case 4:
        func_800B4AA8((&D_800F5F08)[9], -1, 3, -1, -1, -1, -1, D_80124146);
        break;
    case 5:
        func_800B4AA8((&D_800F5F08)[9], 5, -1, -1, -1, -1, -1, D_80124146);
        break;
    default:
        func_800B4AA8((&D_800F5F08)[9], -1, D_800F61D3, -1, -1, -1, -1, D_80124146);
        break;
    }
    if (D_80124C94 != 0) {
        func_800B3EAC(0, D_801240E8, D_801240EC, D_801240F0);
    }
    ((s32 (*)())func_800B463C)();
    if (D_80124146 == 0) {
        func_800B4E74(&(*(u8 *)&D_800F63A0));
    }
    func_800B32D0();
}

void func_800BBAB4(void) {
    u16 mask;

    if (D_80124C94 != 0) {
        mask = 0x63;
        switch ((*(u8 *)&D_800F61D3)) {
        case 0:
            mask |= 0x10;
            break;
        case 1:
            mask |= 0x10;
            break;
        case 2:
            mask |= 0x10;
            break;
        case 3:
            mask |= 0x10;
            break;
        case 4:
            mask |= 0x10;
            break;
        case 5:
            break;
        }
        func_800C8554(mask);
        if ((*(u8 *)&D_800F61D3) == 2 || (*(u8 *)&D_800F61D3) == 3
            || ((*(u8 *)&D_800F61D3) == 5 && (*(u8 * *)&D_80124148)[0x23] == 0 && ((func_800B7EE8(1) & 0xFF) || (func_800B7EE8(2) & 0xFF)))) {
            if (func_800C8134(0, 0x10) & 0xFF) {
                if (func_800B97D8(0) & 0xFF) {
                    func_800C9F18(&(*(s32 *)&D_800F9F70), 0xA);
                    func_800B9794(0);
                    func_800B973C(0);
                    D_800F6480 = 1;
                    D_80124C80 += 1;
                }
            } else if (func_800C8134(0, 0x11) & 0xFF) {
                if (func_800B97D8(0) & 0xFF) {
                    func_800C9F18(&(*(s32 *)&D_800F9F70), 0xC);
                    func_800B9794(0);
                    func_800B973C(0);
                    D_800F6480 = 1;
                    D_80124C80 += 1;
                }
            } else if (D_800F6480 != 0) {
                func_800B977C();
                D_800F6480 = 0;
                D_80124C80 = 0;
            }
        }
    }
}

void func_800BBC38(void) {
    (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x23)) = (u8) ((*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x23)) + 1);
}

void func_800BBC50(void) {

}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BBC58);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BBE54);

void func_800BC324(void) {
    func_800B2B28(D_800F5F2C);
}

void func_800BC348(void) {
    func_800B2BAC(D_800F5F2C);
}

void func_800BC36C(void) {
    switch ((u8)D_800F6102) {
        case 0:
            func_800C9F18(&D_800F9F70, 0x49);
            break;
        case 1:
            D_80124150 = ((s32 *)D_80124148)[1];
            func_800C9F18(&D_800F9F70, 0x4A);
            break;
        case 2:
        default:
            func_800C9F18(&D_800F9F70, 0x4B);
            break;
    }
}


void func_800BC3E8(void) {
    D_800F61D3 = 0;
    D_800F6100 = 0;
}

void func_800BC3FC(void) {

}

void func_800BC404(void) {
    func_800B2AFC(D_800F5F2C);
    func_800B3C74(0);
    func_800B4578();
    func_800B8D8C();
}

void func_800BC440(void) {
    func_800B2F20(D_800F5F2C);
}

void func_800BC464(void) {
    if ((u8)D_800F61D3 == 2) {
        func_800B3CDC(0);
        func_800B3E24(0, 450, 450, 0);
    }
    if ((u8)D_800F61D3 == 5) {
        func_800B4510();
        func_800B45E4(675, 900, 0);
    }
    {
        u8 t = D_800F61D3;
        D_800F61D3 = t != 0 ? t - 1 : 5;
    }
    if ((u8)D_800F61D3 == 2) {
        func_800B3CA4(0);
        func_800B3E24(0, 460, 480, 0);
    }
    if ((u8)D_800F61D3 == 5) {
        func_800B44F4();
        func_800B45E4(625, 825, 0);
    }
    func_800BB4B0();
}

void func_800BC550(void) {
    if ((u8)D_800F61D3 == 2) {
        func_800B3CDC(0);
        func_800B3E24(0, 450, 450, 0);
    }
    if ((u8)D_800F61D3 == 5) {
        func_800B4510();
        func_800B45E4(675, 900, 0);
    }
    {
        u8 t = D_800F61D3;
        if (t < 5) {
            D_800F61D3 = t + 1;
        } else {
            D_800F61D3 = 0;
        }
    }
    if ((u8)D_800F61D3 == 2) {
        func_800B3CA4(0);
        func_800B3E24(0, 460, 480, 0);
    }
    if ((u8)D_800F61D3 == 5) {
        func_800B44F4();
        func_800B45E4(625, 825, 0);
    }
    func_800BB4B0();
}

void func_800BC648(void) {
    switch ((*(u8 *)&D_800F61D3)) {
    case 0:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x4C);
        break;
    case 1:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x4E);
        break;
    case 2:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x46);
        break;
    case 3:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x45);
        break;
    case 4:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x47);
        break;
    default:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x3F);
        break;
    }
}

void func_800BC6E8(void) {
    switch ((u8)D_800F61D3) {
        case 2:
            func_800C9F18(&D_800F9F70, 0x3B);
            break;
        case 3:
            func_800C9F18(&D_800F9F70, 0x3E);
            break;
        case 5:
            func_800C9F18(&D_800F9F70, 0x50);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x3F);
            break;
    }
}


void func_800BC780(void) {
    if (((u8 *)D_80124148)[0x23] == 0) {
        D_80140FAC(0x30, 1, 1.0f, 1.0f);
        do {
            if (((u8 *)D_80124148)[0x22] != 0) {
                ((u8 *)D_80124148)[0x22]--;
            } else {
                ((u8 *)D_80124148)[0x22] = 2;
            }
        } while (func_800B7EE8(((u8 *)D_80124148)[0x22]) == 0);
        func_800BB7E8();
    }
}

void func_800BC818(void) {
    if (((u8 *)D_80124148)[0x23] == 0) {
        D_80140FAC(0x31, 1, 1.0f, 1.0f);
        do {
            if (((u8 *)D_80124148)[0x22] < 2) {
                ((u8 *)D_80124148)[0x22]++;
            } else {
                ((u8 *)D_80124148)[0x22] = 0;
            }
        } while (func_800B7EE8(((u8 *)D_80124148)[0x22]) == 0);
        func_800BB7E8();
    }
}


void func_800BC8A8(void) {
    if (D_80124C80 < 0x7C) {
        D_80140FAC(0x30, 1, 1.0f, 1.0f);
        func_800B8038(0);
    }
}


void func_800BC8F8(void) {
    if (D_80124C80 < 0x7C) {
        D_80140FAC(0x31, 1, 1.0f, 1.0f);
        func_800B7FCC(0);
    }
}


void func_800BC948(void) {
    D_80140FAC(0x30, 1, 1.0f, 1.0f);
    if (D_800F6418) {
        if (D_800F6102) {
            D_800F6102--;
        } else {
            D_800F6102 = 2;
        }
    } else {
        do {
            if (D_800F6102) {
                D_800F6102--;
            } else {
                D_800F6102 = 2;
            }
        } while (D_800F6102 == 1);
    }
}

void func_800BC9F4(void) {
    D_80140FAC(0x31, 1, 1.0f, 1.0f);
    if (D_800F6418) {
        if (++D_800F6102 >= 3) {
            D_800F6102 = 0;
        }
    } else {
        do {
            if (++D_800F6102 >= 3) {
                D_800F6102 = 0;
            }
        } while (D_800F6102 == 1);
    }
}

void func_800BCAA8(void) {
    func_800C8554(0x50);
}

void func_800BCAC4(void) {
    u8 *p = (u8 *)D_80124148;

    D_800F60D0 = 0;
    if (p[0x23] >= 6 || D_800F60D4[p[0x22]][p[0x23]] == -1) {
        if ((u8)func_800BB598(0) == 0) {
            func_800C9F18(&D_800F9F70, 0x52);
            D_800F60D0 = 1;
        } else {
            func_800C9F18(&D_800F9F70, 0x53);
        }
    } else {
        func_800C9F18(&D_800F9F70, 0x54);
    }
}


void func_800BCB7C(void) {
    func_800B3144(&D_80100ED4);
    func_800B2AD0(D_800F5F34);
    func_800B8D74();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BCBB4);

void func_800BD1A0(void) {
    func_800B2B28(D_800F5F34);
}

void func_800BD1C4(void) {
    func_800B2844((*(void * *)&D_800F5F34), 0xBU);
}

void func_800BD1E8(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[11], -1, -1, -1, -1, -1, -1, D_80124146);
    func_800B32D0();
}

void func_800BD248(void) {
    func_800B8D8C();
}

void func_800BD264(void) {

}

void func_800BD26C(void) {
    if ((u32)D_80141048() > (u32)(D_80124C98 + 5000)) {
        func_800C8554(0x50);
    }
}

void func_800BD2B0(void) {
    func_800B3144(&D_80100EFC);
    func_800B2AD0(D_800F5F38);
    func_800B8D74();
    D_80124C98 = D_80141048();
}

void func_800BD300(void) {
    s32 buf[16];
    s32 *fmt = (s32 *)&D_80100CA0;

    sprintf(buf, fmt, D_80141504);
    func_800B4CC4((&D_800F5F08)[12], 0, buf, 320, 210, 0, 2, 1, 1, 255, 255, 255);
    ((s32 (*)(s32 *, s32 *, s32, s32))sprintf)(buf, (s32 *)&D_80100EB4, D_80124148 + 8, D_80141508);
    func_800B4CC4((&D_800F5F08)[12], 1, buf, 320, 240, 0, 2, 1, 1, 255, 255, 255);
    sprintf(buf, fmt, D_8014150C);
    func_800B4CC4((&D_800F5F08)[12], 2, buf, 320, 270, 0, 2, 1, 1, 255, 255, 255);
}

void func_800BD44C(void) {
    func_800B2B28(D_800F5F38);
}

void func_800BD470(void) {
    func_800B2844((*(void * *)&D_800F5F38), 0xCU);
}

void func_800BD494(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[12], -1, -1, -1, -1, -1, -1, D_80124146);
    func_800B32D0();
}

void func_800BD4F4(void) {
    func_800B8D8C();
}

void func_800BD510(void) {

}

void func_800BD518(void) {
    if ((u32)D_80141048() > (u32)(D_80124C98 + 5000)) {
        func_800C8554(0x50);
    }
}


void func_800BD55C(void) {
    func_800B3144(&D_80100F04);
    func_800B2AD0(D_800F5F3C);
    func_800B8D74();
    D_80124C98 = D_80141048();
}

void func_800BD5AC(void) {
    s32 buf[16];

    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100F0C);
    func_800B4CC4((&D_800F5F08)[13], 0, buf, 320, 210, 0, 2, 1, 1, 255, 255, 255);
    sprintf(buf, (s32 *)&D_80100F14, D_80124148 + 8);
    func_800B4CC4((&D_800F5F08)[13], 1, buf, 320, 240, 0, 2, 1, 1, 255, 255, 255);
    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100F1C);
    func_800B4CC4((&D_800F5F08)[13], 2, buf, 320, 270, 0, 2, 1, 1, 255, 255, 255);
}


void func_800BD6D8(void) {
    func_800B2B28(D_800F5F3C);
}

void func_800BD6FC(void) {
    func_800B2844((*(void * *)&D_800F5F3C), 0xDU);
}

void func_800BD720(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[13], -1, -1, -1, -1, -1, -1, D_80124146);
    func_800B32D0();
}

void func_800BD780(void) {
    func_800B8D8C();
}

void func_800BD79C(void) {

}

void func_800BD7A4(void) {
    func_800B2B28(D_800F5F30);
}

void func_800BD7C8(void) {
    s32 *p = &D_800F5F30;

    D_80124C9C = 3;
    func_800B4CC4(*p, 0, (s32 *)&D_80100F34, 320, 160, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4(*p, 1, (s32 *)&D_80100F48, 320, 190, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4(*p, 2, D_80141248, 320, 235, 0, 0, 1, 1, 127, 127, 127);
    func_800B4CC4(*p, 3, D_80141254, 320, 265, 0, 0, 1, 1, 127, 127, 127);
    func_800B2AD0(*p);
}

void func_800BD910(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[10], D_80124C9C, -1, -1, -1, -1, -1, 0);
    func_800B32D0();
}

void func_800BD96C(void) {
    if ((u8)D_80124C9C == 3) {
        func_800B8D74();
        func_800C9F18(&D_800F9F70, 0x11);
    } else {
        func_800C9F18(&D_800F9F70, 0x4F);
    }
}


void func_800BD9BC(void) {
    D_80124C9C = 2;
}

void func_800BD9CC(void) {
    D_80124C9C = 3;
}

void func_800BD9DC(void) {
    if (*(u8 *)&D_80124C9C == 3) {
        func_800C8554(0x11);
    } else {
        func_800C8554(0x12);
    }
}


void func_800BDA10(Bars4_ s, s32 x) {
    u16 i;
    u8 r;
    u8 g;
    u8 b;

    for (i = 0; i < (u8) (s.a / 5); i++) {
        func_800B32F4(0x9A, 0, x + i * 3, 0x9D, 1, 0xF0, 0, 0x14);
    }
    for (i = 0; i < (u8) (s.b / 5); i++) {
        func_800B32F4(0x9A, 0, x + i * 3, 0xB3, 1, 0, 0xF0, 0x14);
    }
    for (i = 0; i < (u8) (s.c / 5); i++) {
        func_800B32F4(0x9A, 0, x + i * 3, 0xC8, 1, 0x14, 0, 0xF0);
    }
    if (s.lv >= 0) {
        switch (s.lv) {
            case 0:
                r = 0x4E;
                g = 0xB0;
                b = 0x53;
                break;
            case 1:
                r = 0xF6;
                g = 0xF6;
                b = 0x24;
                break;
            case 2:
                r = 0xF0;
                g = 0x28;
                b = 0x28;
                break;
            default:
                r = 0xF0;
                g = 0x46;
                b = 0xB4;
                break;
        }
        for (i = 0; i < (s.lv + 1) * 12; i++) {
            func_800B32F4(0x9A, 0, x + i * 3, 0xDF, 1, r, g, b);
        }
    }
}

/* Draws the stat bars of each player's car: one set in single-player mode, two in two-player mode. */
void func_800BDCF0(void) {
    if (D_80123EA0 == 1) {
        func_800BDA10(D_801240E0, 0x15C);
    } else {
        func_800BDA10(D_801240E0, 0x8A);
        if (D_80123EA0 == 2) {
            func_800BDA10(D_801240E4, 0x15C);
        }
    }
}

void func_800BDE74(void) {
    D_800F6484 = D_80141048();
}

void func_800BDEA0(void) {
    D_800F6488 = D_80141048();
}

void func_800BDECC(void) {
    s32 (**fp)() = &D_80141048;

    if ((u32)(*fp)() > (u32)(D_800F6484 + 3000)) {
        func_800C9F18(&D_800F9F70, 0x5A);
        D_800F648C = D_800F648C == 0;
    }
    if (D_80123EA0 == 2 && (u32)(*fp)() > (u32)(D_800F6488 + 3000)) {
        func_800C9F18(&D_800F9F70, 0x5B);
        D_800F648D = D_800F648D == 0;
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BDF94);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BE288);

void func_800BE57C(void) {
    if (((D_80123EA0 == 1) && (D_801240BB != 0)) || ((D_80123EA0 == 2) && (D_801240BB != 0) && (D_801240BC != 0))) {
        func_800C9F18((s32) &D_800F9F70, 0x59);
    }
}

void func_800BE5EC(void) {
    func_800B2B28(D_800F5F40);
    func_800B2B28(D_800F5F44);
}

void func_800BE61C(void) {
    func_800B2BAC(D_800F5F40);
    func_800B2BAC(D_800F5F44);
}

void func_800BE64C(void) {
    if (D_80123EA0 == 1 || *(u8 *)&D_800F6100 != 0) {
        func_800C9F18(&D_800F9F70, 0x57);
    } else {
        func_800C9F18(&D_800F9F70, 0x58);
    }
}


void func_800BE6A0(void) {
    func_800B3144(&D_80100F58);
    func_800B8D74();
    func_800B3C18(0);
    func_800B3D64(0);
    func_800B3E24(0, 0x1A9, 0x20D, 0);
    (*(s8 *)&D_801240BB) = 0;
    D_80124C9D = 1;
    D_80124C80 = 0;
    func_800B2AD0(D_800F5F40);
    D_801240B9 = (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x20));
}

void func_800BE72C(void) {
    func_800B3144(&D_80100F60);
    func_800B8D74();
    D_80124C80 = 0;
    D_80124C81 = 0;
    func_800B3C18(0);
    func_800B3D2C(0);
    func_800B3E24(0, 0x186, 0x262, 0);
    func_800B3C18(1);
    func_800B3D2C(1);
    func_800B3E24(1, 0x262, 0x262, 0);
    (*(s8 *)&D_801240BB) = 0;
    (*(s8 *)&D_801240BC) = 0;
    D_80124C9D = 1;
    D_80124C9E = 1;
    func_800B2AD0(D_800F5F40);
    func_800B2AD0(D_800F5F44);
    D_801240B9 = (*(u8 *)((u8 *)((*(void * *)&D_80124148)) + 0x20));
    D_801240BA = (*(u8 *)((u8 *)((*(void * *)&D_8012414C)) + 0x20));
}

void func_800BE814(u16 id) {
    if (D_80124C94) {
        func_800C8554(id);
        if ((u8)func_800C8134(0, 14)) {
            if ((u8)func_800B97D8(0)) {
                func_800C9F18(&D_800F9F70, 6);
            func_800B9794(0);
            func_800B973C(0);
            D_800F648E = 1;
            D_80124C80++;
            }
        } else if ((u8)func_800C8134(0, 15)) {
            if ((u8)func_800B97D8(0)) {
                func_800C9F18(&D_800F9F70, 8);
            func_800B9794(0);
            func_800B973C(0);
            D_800F648E = 1;
            D_80124C80++;
            }
        } else {
            func_800B977C();
            D_800F648E = 0;
            D_80124C80 = 0;
        }
    }
}

void func_800BE8F8(u16 id) {
    if (D_80124C95) {
        func_800C891C(id);
        if ((u8)func_800C8134(1, 14)) {
            if ((u8)func_800B97D8(1)) {
                func_800C9F18(&D_800F9F70, 7);
            func_800B9794(1);
            func_800B973C(1);
            D_800F648F = 1;
            D_80124C81++;
            }
        } else if ((u8)func_800C8134(1, 15)) {
            if ((u8)func_800B97D8(1)) {
                func_800C9F18(&D_800F9F70, 9);
            func_800B9794(1);
            func_800B973C(1);
            D_800F648F = 1;
            D_80124C81++;
            }
        } else {
            if (D_800F648F) {
                func_800B9788();
                D_800F648F = 0;
                D_80124C81 = 0;
            }
        }
    }
}

void func_800BE9EC(void) {
    s16 a;
    s16 b;

    if (D_80123EA0 == 2) {
        b = 0x10;
        /* TODO(match-debt): branchless form of (D_801240BB == 0 ? 0x50 : 0x20); the ternary does not match. */
        a = (-(D_801240BB == 0) & 0x30) | 0x20;
        if (D_801240BC != 0) {
            b = 0x20;
        }
        if (D_800F6490 == 0) {
            func_800BE814(a);
            func_800BE8F8(b);
            D_800F6490 = 1;
            return;
        }
        func_800BE8F8(b);
        func_800BE814(a);
        D_800F6490 = 0;
        return;
    }
    if (D_801240BB == 0) {
        func_800BE814(0x70);
    }
}

void func_800BEAB0(void) {
    func_800B2844((*(void * *)&D_800F5F40), 0xEU);
    if (D_80123EA0 == 2) {
        func_800B2844((*(void * *)&D_800F5F44), 0xFU);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BEAF8);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800BF064);

void func_800BF440(void) {
    if (D_801240BB != 0) {
        D_801240BB = 0;
        D_80124C9D = 1;
        func_800B3D2C(0U);
        func_800B3E24(0U, 0x186, 0x262, 0);
        func_800C8110();
        return;
    }
    func_800C9F18((s32) &D_800F9F70, 0x5C);
}

void func_800BF4B0(void) {
    if (D_801240BC != 0) {
        D_801240BC = 0;
        D_80124C9E = 1;
        func_800B3D2C(1U);
        func_800B3E24(1U, 0x262, 0x262, 0);
    }
    func_800C8110();
}

void func_800BF508(void) {
    if ((u8)func_800B7DFC(D_801240B9, D_80124148) != 0) {
        ((u8 *)D_80124148)[0x20] = D_801240B9;
        D_801240BB = 1;
        if (D_80123EA0 == 2) {
            func_800B3E24(0, 325, 650, 0);
            func_800B3CDC(0);
        }
    } else {
        D_80140FAC(0x35, 1, 1.0f, 1.0f);
        func_800BDF94();
    }
}

void func_800BF5BC(void) {
    if (D_80123EA0 != 1) {
        if ((u8)func_800B7DFC(D_801240BA, D_8012414C) != 0) {
            ((u8 *)D_8012414C)[0x20] = D_801240BA;
            D_801240BC = 1;
            func_800B3E24(1, 675, 650, 0);
            func_800B3CDC(1);
        } else {
            D_80140FAC(0x35, 1, 1.0f, 1.0f);
            func_800BE288();
        }
    }
}


void func_800BF66C(void) {
    u8 buf[0x40];
    u8 big[0x118];

    if ((u8)func_800B7DFC(D_801240B9, D_80124148) != 0) {
        D_80140FBC(0, ((char **)&D_800FA0DC)[D_801240B9], 1);
        D_80140FB8(((char **)&D_800FA0DC)[D_801240B9], big, buf);
        D_80140FD8();
        *(V12_ *)&D_801240E8 = *(V12_ *)(big + 0x80);
        D_80124C94 = 0;
        D_801240E0.a = big[0x110];
        D_801240E0.b = big[0x112];
        D_801240E0.c = big[0x111];
        D_801240E0.lv = big[0x113];
    } else {
        D_801240E0.lv = -1;
        D_801240E0.a = 0;
        D_801240E0.b = 0;
        D_801240E0.c = 0;
        D_80124C94 = 1;
    }
}

void func_800BF794(void) {
    if (func_800B7DFC(D_801240B9, D_80124148) & 0xFF) {
        D_80140FDC();
        D_80140FB4(0);
    }
}

void func_800BF7EC(void) {
    u8 buf[0x40];
    u8 big[0x118];

    if (D_80123EA0 != 2) {
        return;
    }
    if ((u8)func_800B7DFC(D_801240BA, D_8012414C) != 0) {
        D_80140FBC(1, ((char **)&D_800FA0DC)[D_801240BA], 1);
        D_80140FB8(((char **)&D_800FA0DC)[D_801240BA], big, buf);
        D_80140FD8();
        *(V12_ *)&D_801240F4 = *(V12_ *)(big + 0x80);
        D_80124C95 = 0;
        D_801240E4.a = big[0x110];
        D_801240E4.b = big[0x112];
        D_801240E4.c = big[0x111];
        D_801240E4.lv = big[0x113];
    } else {
        D_801240E4.lv = -1;
        D_801240E4.a = 0;
        D_801240E4.b = 0;
        D_801240E4.c = 0;
        D_80124C95 = 1;
    }
}


void func_800BF920(void) {
    if ((D_80123EA0 == 2) && (func_800B7DFC(D_801240BA, D_8012414C) & 0xFF)) {
        D_80140FDC();
        D_80140FB4(1);
    }
}

void func_800BF984(void) {
    if (D_801240BB == 0 && D_80124C80 < 0x7C) {
        if (++D_801240B9 > 40) {
            D_801240B9 = 0;
        }
        D_80124C9D = 1;
        D_80140FAC(0x2F, 1, 1.0f, 1.0f);
    }
}


void func_800BFA14(void) {
    if (D_801240BC == 0 && *(u8 *)&D_80124C81 < 0x7C) {
        if (++D_801240BA > 40) {
            D_801240BA = 0;
        }
        D_80124C9E = 1;
        D_80140FAC(0x2F, 1, 1.0f, 1.0f);
    }
}


void func_800BFAA4(void) {
    if (D_801240BB == 0 && D_80124C80 < 0x7C) {
        if (D_801240B9 == 0) {
            D_801240B9 = 40;
        } else {
            D_801240B9 -= 1;
        }
        D_80124C9D = 1;
        D_80140FAC(0x2E, 1, 1.0f, 1.0f);
    }
}


void func_800BFB24(void) {
    if (D_801240BC == 0 && *(u8 *)&D_80124C81 < 0x7C) {
        if (D_801240BA == 0) {
            D_801240BA = 40;
        } else {
            D_801240BA -= 1;
        }
        D_80124C9E = 1;
        D_80140FAC(0x2E, 1, 1.0f, 1.0f);
    }
}


void func_800BFBA4(void) {
    func_800B3E24(0U, 0x1F4, 0x1F4, 0);
    func_800B3C74(0U);
    func_800B2AFC(D_800F5F40);
    if (D_80123EA0 == 2) {
        func_800B3C74(1U);
        func_800B3E24(1U, 0x1F4, 0x1F4, 0);
        func_800B2AFC(D_800F5F44);
    }
    func_800B8D8C();
}

void func_800BFC24(void) {
    func_800B2F20(D_800F5F40);
    if (D_80123EA0 == 2) {
        func_800B2F20(D_800F5F44);
    }
}

void func_800BFC68(void) {
    TBL_ *t = (TBL_ *)D_800F5F48;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_80100F70) == 0) {
            t->objs[j].x = func_800B1294(D_80124C71 * 185 + 460);
            t->objs[j].y = func_800B12C4(725);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
        }
    }
    if (D_80124C71 == 2) {
        func_800B4CC4((s32)t, 0, D_80141268, 73, 418, 0, 1, 0, 1, 127, 127, 127);
    } else {
        func_800B4CC4((s32)t, 0, (s32 *)&D_80100CC8, 73, 414, 0, 1, 0, 1, 127, 127, 127);
    }
    func_800B4CC4((s32)t, 1, func_8008D460((u8)D_800F6464), 426, 385, 0, 0, 1, 1, 255, 255, 255);
    func_800B4CC4((s32)t, 2, func_8008D48C((u8)D_800F6464), 426, 410, 0, 0, 1, 1, 255, 255, 255);
}

void func_800BFE68(void) {
    func_800B3144(&D_80100F78);
    func_800B8D74();
    func_800B2AD0(D_800F5F48);
    func_800BFC68();
    func_800B4E64(&D_800F6370);
}

void func_800BFEB4(void) {
    u16 a = 0x63;

    switch ((u8)D_800F61D4) {
        case 4:
        case 5:
        case 6:
            a = 0x73;
            break;
        case 7:
            if (D_80124C71 != 0) {
                a = 0x67;
            }
            if (D_80124C71 < 2) {
                a |= 8;
            }
            break;
        case 8:
            a = 0x6F;
            break;
    }
    func_800C8554(a);
}


void func_800BFF30(void) {
    func_800B2844((*(void * *)&D_800F5F48), 0x10U);
}

void func_800BFF54(void) {
    u8 t;
    func_800B3264();
    func_800B8DC4();
    t = D_800F61D4;
    func_800B4AA8((&D_800F5F08)[16], -(t != 8), (s8)t, -1, -1, -1, -1, D_80124146);
    if (D_80124146 == 0) {
        func_800B4E74(&D_800F6370);
    }
    func_800B32D0();
}

void func_800BFFE4(void) {
    func_800B2BAC(D_800F5F48);
}

void func_800C0008(void) {
    func_800B2B28(D_800F5F48);
}

void func_800C002C(void) {
    if (*(u8 *)&D_800F61D4 < 4) {
        D_8014556C(0);
        if ((u8)func_800C8134(0, 0x10) != 0) {
            func_800C9F18(&D_800F9F70, 0x5D);
        } else if ((u8)func_800C8134(0, 0x11) != 0) {
            func_800C9F18(&D_800F9F70, 0x5E);
        } else {
            func_800C9F18(&D_800F9F70, 0x11);
        }
    }
}

void func_800C00BC(void) {
    switch ((u8)D_800F61D4) {
        case 2:
            D_80140FAC(0x38, 1, D_800F641C[0x87C] / 255.0f, 1.0f);
            break;
        case 3:
            D_80140FAC(0x37, 1, D_800F641C[0x880] / 255.0f, 1.0f);
            break;
        case 1:
            break;
    }
}


void func_800C0174(void) {
    switch ((*(u8 *)&D_800F61D4)) {
    case 7:
        if (D_80124C71 != 0) {
            D_80124C71 -= 1;
            func_800BFC68();
            return;
        }
        return;
    case 8:
        func_800B82E8();
        func_800BFC68();
        func_800B8448();
        D_80124C72 = (*(u8 *)&D_800F6464);
        func_800B8408();
        break;
    }
}

void func_800C01FC(void) {
    switch ((*(u8 *)&D_800F61D4)) {
    case 7:
        if ((u8) D_80124C71 < 2U) {
            D_80124C71 += 1;
            func_800BFC68();
            return;
        }
        return;
    case 8:
        func_800B82B8();
        func_800BFC68();
        func_800B8448();
        D_80124C72 = (*(u8 *)&D_800F6464);
        func_800B8408();
        break;
    }
}

void func_800C0288(void) {
    switch ((u8)D_800F61D4) {
    case 0:
        if (D_800F641C[0x87E] > 10) {
            D_800F641C[0x87E] -= 10;
            D_80140FAC(0x30, 1, 1.0f, 1.0f);
        } else {
            D_800F641C[0x87E] = 0;
        }
        break;
    case 1:
        if (D_800F641C[0x87D] > 10) {
            D_800F641C[0x87D] -= 10;
        } else {
            D_800F641C[0x87D] = 0;
        }
        D_80141054(D_800F641C[0x87D]);
        break;
    case 2:
        if (D_800F641C[0x87C] > 10) {
            D_800F641C[0x87C] -= 10;
        } else {
            D_800F641C[0x87C] = 0;
        }
        break;
    case 3:
        if (D_800F641C[0x880] > 10) {
            D_800F641C[0x880] -= 10;
        } else {
            D_800F641C[0x880] = 0;
        }
        break;
    }
}

void func_800C03B4(void) {
    switch ((u8)D_800F61D4) {
    case 0:
        if (D_800F641C[0x87E] < 245) {
            D_800F641C[0x87E] += 10;
            D_80140FAC(0x31, 1, 1.0f, 1.0f);
        } else {
            D_800F641C[0x87E] = 255;
        }
        break;
    case 1:
        if (D_800F641C[0x87D] < 245) {
            D_800F641C[0x87D] += 10;
        } else {
            D_800F641C[0x87D] = 255;
        }
        D_80141054(D_800F641C[0x87D]);
        break;
    case 2:
        if (D_800F641C[0x87C] < 245) {
            D_800F641C[0x87C] += 10;
        } else {
            D_800F641C[0x87C] = 255;
        }
        break;
    case 3:
        if (D_800F641C[0x880] < 245) {
            D_800F641C[0x880] += 10;
        } else {
            D_800F641C[0x880] = 255;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C04F0);

void func_800C0524(void) {
    if (D_80124C71 == 2) {
        if ((u8)D_800F61D4 < 8) {
            D_800F61D4++;
        } else {
            D_800F61D4 = 0;
        }
    } else {
        if ((u8)D_800F61D4 < 7) {
            D_800F61D4++;
        } else {
            D_800F61D4 = 0;
        }
    }
}


void func_800C0584(void) {
    D_800F61D4 = 0;
}

void func_800C0590(void) {
    u8 c[4];
    u8 i;

    c[0] = D_800F641C[0x87E];
    c[1] = D_800F641C[0x87D];
    c[2] = D_800F641C[0x87C];
    c[3] = D_800F641C[0x880];
    for (i = 0; i < 4; i++) {
        (*(ElC05_ **)(D_800F5F48 + 0x28))[i].fC = func_800B1294((s32)(c[i] * 2.1568627f) + 360);
        (*(ElC05_ **)(D_800F5F48 + 0x28))[i].f10 = func_800B12C4(i * 0x54 + 0x136);
    }
}

void func_800C06A0(void) {
    u8 v = D_800F61D4;

    if (v == 4) {
        func_800C9F18(&D_800F9F70, 0x5F);
    } else if (v == 5) {
        func_800C9F18(&D_800F9F70, 0x61);
    } else if (v == 6) {
        func_800C9F18(&D_800F9F70, 0x60);
    } else {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}


void func_800C0718(void) {
    func_800B2AFC(D_800F5F48);
    func_800B8D8C();
}

void func_800C0744(void) {
    func_800B2F20(D_800F5F48);
}

void func_800C0768(void) {
    func_800B3144(&D_80100F80);
    func_800B8D74();
    func_800B2AD0(D_800F5F60);
    D_800F61D6 = 0;
    func_8008744C(&D_800F2FA0);
    func_8008744C(&D_800F2FC0);
    func_800B4E64(&D_800F63E8);
}

void func_800C07CC(void) {
    if (*(u8 *)&D_800F61D6 == 0) {
        func_800C8554(0x62);
    } else {
        func_800C8554(0x61);
    }
}

void func_800C07FC(void) {
    func_800B2844((*(void * *)&D_800F5F60), 0x16U);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C0820);

void func_800C0910(void) {
    if ((u8)D_800F61D6 == 1) {
        D_800F61D6 = 0;
        func_800B3144(((u8 *)&D_80100F80));
    }
}


void func_800C0948(void) {
    u8 v = D_800F61D6;

    if (v == 0) {
        D_800F61D6 = 1;
        func_800B3144(D_80100F88);
    }
}


void func_800C0980(void) {
    func_800B2AFC(D_800F5F60);
    func_800B8D8C();
}

void func_800C09AC(void) {
    func_800B2F20(D_800F5F60);
}

void func_800C09D0(void) {
    D_80141000(2, &D_80100F94);
}


void func_800C09FC(void) {
    D_80141004(2);
}

void func_800C0A20(u8 which, u8 name, s32 x, s32 y, u8 val) {
    MnC0_ *m;
    u8 i;

    if (which == 0) {
        m = (MnC0_ *)D_800F5F4C;
    } else {
        m = (MnC0_ *)D_800F5F50;
    }
    for (i = 0; i < m->n; i++) {
        if (strcmp(m->names[i].s, D_800F649C[name]) == 0) {
            m->elems[i].h30 = val;
            m->elems[i].h32 = val;
            m->elems[i].x = func_800B1294(x);
            m->elems[i].y = func_800B12C4(y);
            m->elems[i].xC = m->elems[i].x;
            m->elems[i].x10 = m->elems[i].y;
            return;
        }
    }
}

void func_800C0B68(void) {
    func_800C0A20(0, 0, 200, 310, D_80124107 ? 120 : 0);
    func_800C0A20(0, 1, 250, 870, D_80124109 < 13 ? 100 : 0);
}


void func_800C0BE0(void) {
    if (D_80123EA0 == 2) {
        func_800C0A20(1, 0, 775, 310, D_80124108 ? 120 : 0);
        if (D_8012410A < 13) {
            func_800C0A20(1, 1, 825, 870, 100);
        } else {
            func_800C0A20(1, 1, 825, 870, 0);
        }
    } else {
        func_800C0A20(1, 0, 700, 310, 0);
        func_800C0A20(1, 1, 750, 870, 0);
    }
}


void func_800C0CAC(void) {
    void (**fp)(s32, s32);

    func_800B3144((s32 *)&D_80100FA4);
    func_800B8D74();
    fp = &D_801455A8;
    (*fp)(D_80124148 + 0x40, 0);
    (*fp)(D_8012414C + 0x40, 1);
    D_80124105 = 0;
    D_80124106 = 0;
    func_800B2AD0((&D_800F5F08)[17]);
    func_800B2AD0(D_800F5F50);
    func_800B2AD0(D_800F5F54);
    func_800B2AD0(D_800F5F58);
    func_800B4DF8((&D_800F5F08)[17]);
    func_800B4DF8(D_800F5F54);
    func_800B4DF8(D_800F5F50);
    func_800B4DF8(D_800F5F58);
    D_8012410B = 0;
    D_8012410C = 0;
    D_80124107 = 0;
    D_80124108 = 0;
    D_80124109 = 5;
    D_8012410A = 5;
    func_800C0B68();
    func_800C0BE0();
    func_800B4E64(&D_800F63B8);
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C0DD0);

void func_800C1160(void) {
    func_800B2844((*(void * *)&D_800F5F4C), 0x11U);
    func_800B2844((*(void * *)&D_800F5F54), 0x13U);
    func_800B2844((*(void * *)&D_800F5F50), 0x12U);
    func_800B2844((*(void * *)&D_800F5F58), 0x14U);
}

/* Draws the four option rows of this menu page (values taken modulo 5). */
void func_800C11B4(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[17], (s8)(D_8012410B % 5), -1, -1, -1, -1, -1, D_80124146);
    func_800B4AA8((&D_800F5F08)[19], (s8)(D_8012410B % 5), -1, -1, -1, -1, -1, D_80124146);
    func_800B4AA8((&D_800F5F08)[18], (s8)(D_8012410C % 5), -1, -1, -1, -1, -1, D_80124146);
    func_800B4AA8((&D_800F5F08)[20], (s8)(D_8012410C % 5), -1, -1, -1, -1, -1, D_80124146);
    if (D_80124146 == 0) {
        func_800B4E74(&D_800F63B8);
    }
    func_800B32D0();
}

void func_800C1360(void) {
    if (D_8012410B != 0) {
        D_8012410B -= 1;
    } else if (D_80124107 != 0) {
        D_80124107 -= 1;
        D_80124109 -= 1;
    }
    func_800C0B68();
}

void func_800C13C4(void) {
    if ((u8) D_8012410B < 4U) {
        D_8012410B += 1;
    } else if ((u8) D_80124109 < 0xDU) {
        D_80124109 += 1;
        D_80124107 += 1;
    }
    func_800C0B68();
}

void func_800C1430(void) {
    if (D_8012410C != 0) {
        D_8012410C -= 1;
    } else if (D_80124108 != 0) {
        D_80124108 -= 1;
        D_8012410A -= 1;
    }
    func_800C0BE0();
}

void func_800C1494(void) {
    if ((u8) D_8012410C < 4U) {
        D_8012410C += 1;
    } else if ((u8) D_8012410A < 0xDU) {
        D_8012410A += 1;
        D_80124108 += 1;
    }
    func_800C0BE0();
}

void func_800C1500(void) {
    void (**fp)(s32, s32);

    func_800B2AFC(D_800F5F4C);
    func_800B2AFC(D_800F5F50);
    func_800B2AFC(D_800F5F54);
    func_800B2AFC(D_800F5F58);
    func_800B8D8C();
    fp = &D_801455AC;
    (*fp)(D_80124148 + 0x40, 0);
    (*fp)(D_8012414C + 0x40, 1);
}


void func_800C158C(void) {
    func_800B2F20(D_800F5F4C);
    func_800B2F20(D_800F5F50);
    func_800B2F20(D_800F5F54);
    func_800B2F20(D_800F5F58);
}

void func_800C15D4(void) {
    func_800B2BAC(D_800F5F4C);
    func_800B2BAC(D_800F5F50);
    func_800B2BAC(D_800F5F54);
    func_800B2BAC(D_800F5F58);
}

void func_800C161C(void) {
    func_800B2B28(D_800F5F4C);
    func_800B2B28(D_800F5F50);
    func_800B2B28(D_800F5F54);
    func_800B2B28(D_800F5F58);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C1664);

void func_800C1824(void) {
    if (func_800C1664() != 0) {
        D_80140FAC(0x35, 1, 1.0f, 1.0f);
        func_800B8DA8();
        func_800C9F18(&D_800F9F70, 0x65);
    } else {
        func_800C9F18(&D_800F9F70, 0x66);
    }
}


void func_800C1898(void) {
    D_80124105 = 1;
}

void func_800C18A8(void) {
    if (D_80123EA0 == 1) {
        D_80123EA0 = 2;
        func_800C0BE0();
        return;
    }
    D_80124106 = 1;
}

void func_800C18F0(void) {
    D_801455B0(0);
}

void func_800C1914(void) {
    D_801455B0(1);
}

void func_800C1938(void) {
    D_8014556C(0);
    if (D_80145578(0, D_80124107 + D_8012410B) != 0) {
        D_80124105 = 0;
        func_800C9F18(&D_800F9F70, 0x64);
    }
}


void func_800C19A4(void) {
    D_8014556C(1);
    if (D_80145578(1, D_80124108 + D_8012410C) != 0) {
        D_80124106 = 0;
        func_800C9F18(&D_800F9F70, 0x64);
    }
}


void func_800C1A10(void) {
    s32 screen;

    if ((*(u8 *)&D_80124105) == 0) {
        screen = 0x70;
        if ((D_80124107 != 0) || (D_8012410B != 0)) {
            screen = 0x71;
        }
        if (((u8) D_80124109 < 0xDU) || ((u8) D_8012410B < 4U)) {
            screen |= 2;
        }
        func_800C8554(screen & 0xFFFF);
        return;
    }
    func_800C1938();
}

void func_800C1A94(void) {
    s32 screen;

    if ((*(u8 *)&D_80124106) == 0) {
        if (D_80123EA0 == 1) {
            screen = 0x50;
        } else {
            screen = 0x10;
            if ((D_80124108 != 0) || (D_8012410C != 0)) {
                screen = 0x11;
            }
            if (((u8) D_8012410A < 0xDU) || ((u8) D_8012410C < 4U)) {
                screen |= 2;
            }
        }
        func_800C891C(screen & 0xFFFF);
        return;
    }
    func_800C19A4();
}

void func_800C1B34(void) {
    if (D_800F64A4 == 0) {
        D_800F64A4 = 1;
        func_800C1A10();
        func_800C1A94();
        return;
    }
    D_800F64A4 = 0;
    func_800C1A94();
    func_800C1A10();
}

void func_800C1B88(void) {
    s32 buf[8];

    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100FAC);
    func_800B4CC4((&D_800F5F08)[21], 0, buf, 320, 165, 0, 0, 1, 1, 255, 255, 255);
    sprintf(buf, (s32 *)&D_80100FB4, (s32)&D_80124CA0);
    func_800B4CC4((&D_800F5F08)[21], 1, buf, 320, 195, 0, 0, 1, 1, 255, 255, 255);
    sprintf(buf, (s32 *)&D_80100FBC, (s32)&D_80124CC0);
    func_800B4CC4((&D_800F5F08)[21], 2, buf, 320, 225, 0, 0, 1, 1, 255, 255, 255);
    ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80100FC4);
    func_800B4CC4((&D_800F5F08)[21], 3, buf, 320, 255, 0, 0, 1, 1, 255, 255, 255);
    func_800B2AD0((&D_800F5F08)[21]);
}

void func_800C1CFC(void) {
    func_800B2B28(D_800F5F5C);
}

void func_800C1D20(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[21], -1, -1, -1, -1, -1, -1, 0);
    func_800B32D0();
}

void func_800C1D78(void) {
    func_800C8554(0x50);
    if (D_80123EA0 == 2) {
        func_800C891C(0x50);
    }
}

void func_800C1DB0(void) {
    func_800B8D74();
}

void func_800C1DCC(void) {
    u8 i;

    for (i = 0; i < 20; i++) {
        (&D_801240C0)[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C1DFC);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C2034);

void func_800C245C(void) {
    if (D_801240B0 == 0) {
        do {
            D_800F6420 = (D_800F6420 - 1) & 3;
        } while (func_800AFB7C(D_800F6420) != 0);
    }
}

void func_800C24A0(void) {
    if (D_801240B0 == 0) {
        do {
            D_800F6420 = (D_800F6420 + 1) & 3;
        } while (func_800AFB7C(D_800F6420) != 0);
    }
}

void func_800C24E4(void) {
    if (D_801240B1 == 0 && D_80123EA0 == 2) {
        do {
            D_800F6421 = (D_800F6421 - 1) & 3;
        } while (func_800AFB7C(D_800F6421) != 0);
    }
}

void func_800C253C(void) {
    if (D_801240B1 == 0 && D_80123EA0 == 2) {
        do {
            D_800F6421 = (D_800F6421 + 1) & 3;
        } while (func_800AFB7C(D_800F6421) != 0);
    }
}

void func_800C2594(void) {
    func_800B2BAC(D_800F5F64);
    func_800B2BAC(D_800F5F68);
}

void func_800C25C4(void) {
    func_800B2844((*(void * *)&D_800F5F64), 0x17U);
    if (D_80123EA0 == 2) {
        func_800B2844((*(void * *)&D_800F5F68), 0x18U);
    }
    if ((*(u8 *)&D_801240B2) != 0) {
        func_800B2844(D_800F5F6C, 0x19U);
    }
}

void func_800C262C(void) {
    func_800B3264();
    func_800B8DC4();
    if (*(u8 *)&D_801240B2) {
        func_800B4AA8((s32)D_800F5F6C, -1, D_801240B3, -1, -1, -1, -1, 0);
    } else if (*(u8 *)&D_801240B4) {
        if (*(u8 *)&D_801240B6) {
            func_800B4AA8(D_800F5F74, -1, *(s8 *)&D_801240B7, -1, -1, -1, -1, 0);
        } else {
            func_800B4AA8(D_800F5F70, -1, *(s8 *)&D_801240B5, -1, -1, -1, -1, 0);
        }
    } else {
        func_800B4AA8(D_800F5F64, *(s8 *)&D_801240B0, -1, -1, -1, -1, -1, D_80124146);
        if (D_80123EA0 == 2) {
            func_800B4AA8(D_800F5F68, *(s8 *)&D_801240B1, -1, -1, -1, -1, -1, D_80124146);
        }
        if (D_80124146 == 0) {
            func_800B4E74(&D_800F63D0);
        }
    }
    func_800B32D0();
}

void func_800C2784(void) {
    if (D_801240B0 != 0) {
        D_801240B0 -= 1;
    }
}

void func_800C27A4(void) {
    if ((u8) D_801240B0 < 4U) {
        D_801240B0 += 1;
    }
}

void func_800C27C8(void) {
    if (D_801240B1 != 0) {
        D_801240B1 -= 1;
    }
}

void func_800C27E8(void) {
    if ((u8) D_801240B1 < 4U) {
        D_801240B1 += 1;
    }
}

void func_800C280C(void) {
    u32 screen;

    screen = 0x31;
    if ((*(u8 *)&D_801240B3) == 0) {
        screen = 0x32;
    }
    if (D_801240B8 == 0) {
        func_800C8554(screen);
        return;
    }
    func_800C891C(screen);
}

void func_800C285C(void) {
    D_801240B3 = 0;
}

void func_800C2868(void) {
    D_801240B3 = 1;
}

void func_800C2878(void) {
    if ((*(u8 *)&D_801240B3) == 0) {
        func_800C1DCC();
        func_800C9F18(&D_800F9F70, 0x6E);
    } else {
        func_800C9F18(&D_800F9F70, 0x6F);
    }
}


/* TODO(match-debt): the local pointer to D_801240B1 is only needed to match (forces a reload on one path). */
void func_800C28C4(void) {
    u8 *sel2;
    s32 a;
    s32 b;

    a = 0x2C;
    if (D_801240B0 != 0) {
        a = 0x31;
    }
    if (D_801240B0 < 4) {
        a |= 2;
    }
    func_800C8554(a & 0xFFFF);
    sel2 = &D_801240B1;
    b = 0x50;
    if (D_80123EA0 != 1) {
        b = 0x11;
        if (*sel2 == 0) {
            b = 0xC;
        }
        if (D_801240B1 < 4) {
            b |= 2;
        }
    }
    func_800C891C(b & 0xFFFF);
}

void func_800C2944(void) {
    u8 n = D_801240B0;

    if (n == 0) {
        func_800C9F18(&D_800F9F70, 0x69);
    } else if (func_800B042C(D_800F6420) < n) {
        D_801240B8 = 0;
        func_800C9F18(&D_800F9F70, 0x6C);
    } else {
        D_801240B8 = 0;
        func_800C9F18(&D_800F9F70, 0x6B);
    }
}


void func_800C29C4(void) {
    u8 n;

    if (D_80123EA0 == 1) {
        D_80123EA0 = 2;
        func_800C9F18(&D_800F9F70, 0x6D);
        return;
    }
    n = D_801240B1;
    if (n == 5) {
        func_800C9F18(&D_800F9F70, 0x6A);
    } else if (n == 0) {
        func_800C9F18(&D_800F9F70, 0x69);
    } else if (func_800B042C(D_800F6421) < n) {
        D_801240B8 = 1;
        func_800C9F18(&D_800F9F70, 0x6C);
    } else {
        D_801240B8 = 1;
        func_800C9F18(&D_800F9F70, 0x6B);
    }
}


void func_800C2A8C(void) {
    func_800B8DA8();
    D_801240B4 = 1;
}

void func_800C2AB4(void) {
    func_800B8DA8();
    D_801240B6 = 1;
}

void func_800C2ADC(void) {
    func_800B8DA8();
    D_801240B2 = 1;
}

void func_800C2B04(void) {
    if (D_801240B8 == 0) {
        D_80124150 = (*(s32 *)((u8 *)(func_800B0310((u32) D_800F6420, D_801240B0 - 1)) + 0x4));
    } else {
        D_80124154 = (*(s32 *)((u8 *)(func_800B0310((u32) D_800F6421, D_801240B1 - 1)) + 0x4));
    }
    func_800C8110();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C2B78);

void func_800C2EA4(void) {
    func_800B2AFC(D_800F5F64);
    func_800B2AFC(D_800F5F68);
    func_800B8D8C();
}

void func_800C2EDC(void) {
    func_800B2F20(D_800F5F64);
    func_800B2F20(D_800F5F68);
}

void func_800C2F0C(void) {
    s32 base;
    s32 screen;

    base = -(D_801240B5 != 0) & 0x31;
    screen = base | 0x30;
    if ((u8) D_801240B5 < 2U) {
        screen = base | 0x32;
    }
    if (D_801240B8 == 0) {
        func_800C8554(screen);
        return;
    }
    func_800C891C(screen);
}

void func_800C2F6C(void) {
    if (D_801240B5 != 0) {
        D_801240B5 -= 1;
    }
}

void func_800C2F8C(void) {
    if ((u8) D_801240B5 < 2U) {
        D_801240B5 += 1;
    }
}

void func_800C2FB0(void) {
    D_80141024(0xFF);
    switch (D_801240B5) {
        case 0:
            func_800C9F18(&D_800F9F70, 0x70);
            break;
        case 1:
            func_800C9F18(&D_800F9F70, 0x72);
            break;
        case 2:
            func_800C9F18(&D_800F9F70, 0x71);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x69);
            break;
    }
}


void func_800C3054(void) {
    u8 pak;
    u8 entry;
    void *rec;

    if (D_801240B8 == 0) {
        entry = D_801240B0;
        pak = D_800F6420;
    } else {
        entry = D_801240B1;
        pak = D_800F6421;
    }
    rec = func_800B0310((u32) pak, entry - 1);
    D_80124CE0 = rec;
    strcpy(&D_801240C0, rec + 8);
    D_800F610A = 1;
}

void func_800C30C4(void) {
    if ((*(u8 *)&D_800F610A) != 0) {
        func_800C9F18(&D_800F9F70, 0x67);
    } else {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}


void func_800C3108(void) {
    DEBUG_NOP();
    strcpy(*(s32 *)&D_80124CE0 + 8, &D_801240C0);
    if (func_800B0024() & 0xFF) {
        func_800C8110();
        return;
    }
    func_800C9F18((s32)&D_800F9F70, 0x68);
}

void func_800C3168(void) {
    (*(s8 *)&D_801240B7) = 0;
}

void func_800C3174(void) {
    (*(s8 *)&D_801240B7) = 1;
}

void func_800C3184(void) {
    s32 screen;

    screen = 0x31;
    if (D_801240B7 == 0) {
        screen = 0x32;
    }
    if (D_801240B8 == 0) {
        func_800C8554(screen);
        return;
    }
    func_800C891C(screen);
}

void func_800C31D4(void) {
    if (D_801240B7 == 0) {
        func_800C9F18(&D_800F9F70, 0x73);
    } else {
        func_800C9F18(&D_800F9F70, 0x74);
    }
}


void func_800C3218(void) {
    u8 pak;
    u8 entry;

    if (D_801240B8 == 0) {
        entry = D_801240B0;
        pak = D_800F6420;
    } else {
        entry = D_801240B1;
        pak = D_800F6421;
    }
    func_800B0548(pak, entry - 1);
    func_800B0024();
}

void func_800C326C(void) {
    s32 player;
    u8 n;

    n = func_80086D50((s32)&D_80100FDC);
    D_801240D6 = n;
    if (!n) {
        func_800C9F18((s32)&D_800F9F70, 0x12);
    }
    func_800C1DCC();
    if (D_801240B8 == 0) {
        player = D_80124148;
    } else {
        player = D_8012414C;
    }
    strcpy(&D_801240C0, player + 8);
    D_801240D7 = 10;
    D_801240D8 = 0;
    D_801240DD = D_801240D6 + 1;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 2;
    D_801240D6 += 3;
}

void func_800C3338(void) {
    u8 n;

    n = func_80086D50((s32)&D_80100FDC);
    D_801240D6 = n;
    if (!n) {
        func_800C9F18((s32)&D_800F9F70, 0x12);
    }
    D_801240D7 = 10;
    D_801240D8 = 2;
    D_801240DD = D_801240D6 + 1;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 2;
    D_801240D6 += 3;
}

void func_800C33CC(void) {
    u8 n;

    n = func_80086D50((s32)&D_80100FE4);
    D_801240D6 = n;
    if (!n) {
        func_800C9F18((s32)&D_800F9F70, 0x12);
    }
    func_800C1DCC();
    D_801240D7 = 20;
    D_801240D8 = 3;
    *(u8 *)&D_801240DD = 0xFF;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 1;
    D_801240D6 += 2;
}

void func_800C3468(void) {
    u8 n;

    n = func_80086D50((s32)&D_80100FDC);
    D_801240D6 = n;
    if (!n) {
        func_800C9F18((s32)&D_800F9F70, 0x12);
    }
    func_800C1DCC();
    D_801240D7 = 10;
    D_801240D8 = 1;
    D_801240DD = D_801240D6 + 1;
    D_801240DC = D_801240D6;
    D_801240DE = D_801240D6 + 2;
    D_801240D6 += 3;
}

void func_800C3504(void) {
    u8 r = 10;
    u8 i;

    for (i = 0; i < 10; i++) {
        if ((&D_801240C0)[i] != 0x20) {
            r = i;
        }
    }
    if (r < 8) {
        (&D_801240C0)[r + 1] = 0;
    }
}


void func_800C3560(s32 x, s32 y) {
    MnC3_ *m;
    u8 i;

    m = (MnC3_ *)D_800F5F7C;
    for (i = 0; i < m->n; i++) {
        if (strcmp(m->names[i].s, D_80100FEC) == 0) {
            m->elems[i].xC = func_800B1294((x + 5) / 640.0f * 1000.0f);
            m->elems[i].x10 = func_800B12C4((y + 10) / 480.0f * 1000.0f);
            m->elems[i].x = m->elems[i].xC;
            m->elems[i].y = m->elems[i].x10;
            return;
        }
    }
}

void func_800C36B8(void) {
    func_800B2BAC(D_800F5F7C);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C36DC);

void func_800C3954(void) {
    E140b_ l;
    s32 p;

    switch ((u32)D_801240D8) {
        case 0:
            if (D_801240B8 == 0) {
                p = D_80124148;
            } else {
                p = D_8012414C;
            }
            strcpy((s32 *)(p + 8), (s32)&D_801240C0);
            func_800C9F18(&D_800F9F70, 0x75);
            break;
        case 1:
            if (D_801240B8 == 0) {
                func_800B51C0(&l, 0);
                l.w14 |= ((E140b_ *)D_80124148)->w14;
                l.w18 |= ((E140b_ *)D_80124148)->w18;
                l.w1C |= ((E140b_ *)D_80124148)->w1C;
                strcpy((s32 *)l.name, (s32)&D_801240C0);
                func_800B0454(D_800F6420, &l);
            } else {
                func_800B51C0(&l, 0);
                l.w14 |= ((E140b_ *)D_8012414C)->w14;
                l.w18 |= ((E140b_ *)D_8012414C)->w18;
                l.w1C |= ((E140b_ *)D_8012414C)->w1C;
                strcpy((s32 *)l.name, (s32)&D_801240C0);
                func_800B0454(D_800F6421, &l);
            }
            if ((u8)func_800B0024() != 0) {
                func_800C9F18(&D_800F9F70, 0x76);
            } else {
                func_800B8DA8();
                func_800C9F18(&D_800F9F70, 0x68);
            }
            break;
        case 2:
            func_800C9F18(&D_800F9F70, 0x76);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x77);
            break;
    }
}

void func_800C3AF8(void) {
    switch ((u32)D_801240D8) {
        case 1:
            func_800C9F18(&D_800F9F70, 0x76);
            break;
        case 2:
            D_800F610A = 0;
            func_800C9F18(&D_800F9F70, 0x76);
            break;
        case 0:
        default:
            func_800C9F18(&D_800F9F70, 0x75);
            break;
    }
}

void func_800C3B70(void) {
    u16 f = 0x60;
    u8 n;

    if (D_801240D5 < *(u8 *)&D_801240D7 || D_801240D4 == D_801240DC || D_801240D4 == D_801240D6 - 1) {
        f |= 0x10;
    }
    if (D_801240D4 != 0) {
        f |= 4;
    }
    if (D_801240D4 < D_801240D6 - 1) {
        f |= 8;
    }
    if (D_801240D4 != D_801240D6 - 1) {
        f |= 2;
    }
    if (D_801240D4 >= 8) {
        f |= 1;
    }
    if (D_801240B8 == 0) {
        func_800C8554(f);
    } else {
        func_800C891C(f);
    }
}


void func_800C3C34(void) {
    func_800B2844((*(void * *)&D_800F5F7C), 0x1DU);
}

void func_800C3C58(void) {
    u8 i;

    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[29], -1, -1, -1, -1, -1, -1, D_80124146);
    if (D_80124146 == 0) {
        for (i = 0; i < D_801240D6 - 1; i++) {
            if (i == D_801240DC) {
                D_8014100C(1, 0x9B, D_800F610C[i], D_800F6170[i], i == D_801240D4);
            } else if (i == *(u8 *)&D_801240DD) {
                D_8014100C(1, 0x9A, D_800F610C[i], D_800F6170[i], i == D_801240D4);
            } else {
                D_8014100C(1, D_801410A8[i], D_800F610C[i], D_800F6170[i], i == D_801240D4);
            }
        }
        func_800B4E74(&D_800F6400);
    }
    func_800B32D0();
}

void func_800C3DA8(void) {
    if (D_801240D4 != 0) {
        D_801240D4--;
        func_800C3560(D_800F610C[D_801240D4], D_800F6170[D_801240D4]);
    }
}

void func_800C3DF8(void) {
    u8 i = D_801240D4;
    u8 n = D_801240D6;

    if (i == n - 2) {
        D_801240D4 = i + 1;
        func_800C3560(D_800F616C, D_800F61D0);
    } else if (i < n - 1) {
        i++;
        D_801240D4 = i;
        func_800C3560(D_800F610C[i], D_800F6170[i]);
    }
}


void func_800C3E88(void) {
    if (D_801240D4 == D_801240D6 - 1) {
        D_801240D4 = D_801240DF;
    } else if (D_801240D4 - 8 >= 0) {
        D_801240D4 -= 8;
    }
    func_800C3560(D_800F610C[D_801240D4], D_800F6170[D_801240D4]);
}

void func_800C3F00(void) {
    u8 i = D_801240D4;
    u8 n = D_801240D6;

    if (i + 8 >= n - 1) {
        D_801240DF = i;
        D_801240D4 = n - 1;
        func_800C3560(D_800F616C, D_800F61D0);
    } else if (i + 8 < n) {
        i += 8;
        D_801240D4 = i;
        func_800C3560(D_800F610C[i], D_800F6170[i]);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C3F9C);

void func_800C4380(void) {
    func_800B8D8C();
    func_800B2F20(D_800F5F7C);
}

void func_800C43A8(void) {
    s32 buf[6];

    if (func_800B0F94(&D_801240C0, D_801240B8 == 0 ? D_80124148 : D_8012414C) == 0) {
        if (D_801240B8 == 0) {
            func_800B0F74(D_80124148, buf);
            strcpy((s32 *)&D_800F6424, (s32)buf);
        } else {
            func_800B0F74(D_8012414C, buf);
            strcpy((s32 *)&D_800F6444, (s32)buf);
        }
        func_800C9F18(&D_800F9F70, 0x7A);
    } else {
        D_80140FAC(0x35, 1, 1.0f, 1.0f);
        func_800C9F18(&D_800F9F70, 0x7B);
    }
}

void func_800C4480(void) {
    func_800B8DA8();
    func_800B2AD0(D_800F5F78);
}

void func_800C44A8(void) {
    s32 m;
    s32 *q;
    q = &D_800F5F78;
    func_800B3264();
    m = -1;
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[29], -1, -1, -1, m, m, m, 1);
    func_800B4AA8(*q, -1, -1, -1, m, m, m, 0);
    func_800B32D0();
}

void func_800C452C(void) {
    if (D_801240B8 == 0) {
        func_800C8554(0x30);
        return;
    }
    func_800C891C(0x30);
}

void func_800C4564(s32 i, s32 x, s32 y, s32 c) {
    TBL_ *t = (TBL_ *)D_800F5F80;
    u8 j;

    for (j = 0; j < t->n; j++) {
        if (strcmp(t->names[j].name, D_800F64A8[(u8)i]) == 0) {
            if ((x < 0) & (y < 0)) {
                t->objs[j].h30 = (u8)c;
                return;
            }
            t->objs[j].h30 = (u8)c;
            t->objs[j].x = func_800B1294(x);
            t->objs[j].y = func_800B12C4(y);
            t->objs[j].x2 = t->objs[j].x;
            t->objs[j].y2 = t->objs[j].y;
            return;
        }
    }
}


void func_800C46C4(void) {
    switch (D_800F61D7) {
        case 0:
            func_800C4564(0, 80, 390, 100);
            func_800C4564(1, 425, 387, 100);
            break;
        case 1:
            func_800C4564(0, 80, 480, 100);
            func_800C4564(1, 425, 477, 100);
            break;
        default:
            func_800C4564(0, -1, -1, 0);
            func_800C4564(1, -1, -1, 0);
            break;
    }
}

extern u8 D_80101000;
extern u8 D_80124100;
extern u8 D_80124101;
extern u8 D_80124102;
extern ObjG1_ D_800F6388;
void func_800B4DF8(s32);
void func_800C4768(void) {

    func_800B3144((s32 *)&D_80101000);
    func_800B8D74();
    func_800B2AD0((&D_800F5F08)[30]);
    func_800B4DF8((&D_800F5F08)[30]);
    D_80124103 = 0;
    D_80124104 = 0;
    D_800F61D7 = 0;
    D_80124100 = 0;
    D_80124101 = 0;
    D_80124102 = 0;
    func_800C46C4();
    func_800B4E64(&D_800F6388);
}

void func_800C47F0(void) {
    func_800B2B28(D_800F5F80);
}

void func_800C4814(void) {
    func_800B2844((*(void * *)&D_800F5F80), 0x1EU);
}

void func_800C4838(void) {
    if (D_80124103 != 0) {
        func_800C82B4();
    }
}

void func_800C4860(void) {
    if (D_800F61D7 == 0) {
        func_800C8554(0x6E);
    } else {
        func_800C8554(0x6D);
    }
}

void func_800C4890(void) {
    s32 t;

    D_80124100 = D_8014104C(3);
    t = D_80124100;
    if (t >= 0) {
        if (t < 2) {
            do {
            } while (!func_800B7E48(D_80124101 = D_8014104C(12)));
            return;
        }
    }
    do {
    } while (!func_800B7EE8(D_80124102 = D_8014104C(3)));
}

void func_800C4930(void) {
    if (D_80124103 == 0) {
        func_800C4890();
        D_80124103 = 1;
        (*(s8 *)&D_80124104) = 0;
    }
}

void func_800C496C(void) {
    (*(s8 *)&D_80124103) = 0;
}

void func_800C4978(void) {
    if (D_80124103 != 0) {
        if (++D_80124104 >= 20) {
            func_800C4890();
            D_80124104 = 0;
        }
    }
}

void func_800C49CC(void) {
    if (D_800F61D7 != 0) {
        D_800F61D7 -= 1;
    }
    func_800C46C4();
}

void func_800C4A00(void) {
    if (D_800F61D7 == 0) {
        D_800F61D7 += 1;
    }
    func_800C46C4();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C4A34);

/* Moves the cursor of menu page D_800F61D7 to the next selectable entry, wrapping around. */
void func_800C4BA4(void) {
    s32 m;

    switch (D_800F61D7) {
        case 0:
            if (D_80124100 < 2) {
                D_80124100++;
            } else {
                D_80124100 = 0;
            }
            break;
        case 1:
            /* TODO(match-debt): the redundant m >= 0 test on a u8 is needed for the bltz/slti pair. */
            m = D_80124100;
            if (m >= 0) {
                if (m < 2) {
                    do {
                        do {
                            if (D_80124101 < 11) {
                                D_80124101++;
                            } else {
                                D_80124101 = 0;
                            }
                        } while (!func_800B7E48(D_80124101));
                    } while (D_800F9FA0[(u8)(D_80124101 / 3)][(u8)(D_80124101 % 3)] == 0);
                    return;
                }
            }
            do {
                if (D_80124102 < 2) {
                    D_80124102++;
                } else {
                    D_80124102 = 0;
                }
            } while (!func_800B7EE8(D_80124102));
            break;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C4D10);

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C5148);

void func_800C51C8(void) {
    func_800B2AFC(D_800F5F80);
    func_800B8D8C();
}

void func_800C51F4(void) {
    func_800B2F20(D_800F5F80);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C5218);

void func_800C53E0(void) {
    if (D_80124CE4 != 0 || D_80124CE8 != 0 || D_80124CEC != 0 || D_80124CF0 != 0 || D_80124CF4 != 0 ||
        D_80124CF8 != 0 || D_80124CFC != 0) {
        func_800C9F18(&D_800F9F70, 0x55);
    } else {
        func_800C9F18(&D_800F9F70, 0x56);
    }
}


s32 func_800C5480(u32 *m) {
    u32 v = *m;
    u8 i = 0;

    while (!(v & 1) && i < 32) {
        v >>= 1;
        i++;
    }
    if (i < 32) {
        *m &= ~(1 << i);
        return (s8)i;
    }
    return -1;
}


void func_800C54F8(void) {
    func_800B2AD0(D_800F5F88);
    func_800B3144(&D_80101014);
    func_800B8D74();
}

void func_800C5530(void) {
    if ((D_80124CFD != -1) || (D_80124CFF != D_80124CFD)) {
        D_80140FB4(0);
        return;
    }
    if ((D_80124CFE != D_80124CFF) || (D_80124D00 != D_80124CFE)) {
        D_80141040(func_800B1064(&D_80100E10) & 0xFF);
        func_800B10F4(&D_80100E10);
    }
}

void func_800C55CC(u8 i) {
    u8 buf[0x40];
    u8 big[0x118];

    D_80140FBC(0, ((char **)&D_800FA0DC)[i], 1);
    D_80140FB8(((char **)&D_800FA0DC)[i], big, buf);
    D_80140FD8();
    *(V12c_ *)&D_801240E8 = *(V12c_ *)(big + 0x80);
    D_80124C94 = 0;
    func_800B3C18(0);
    func_800B3D64(0);
    func_800B3D9C(0, 500, 500, 0);
    func_800B3E24(0, 510, 525, 0);
}

void func_800C569C(u8 i) {
    D_8014103C(func_800B0FB0(&D_80100E10), D_800F9FA0[(u8)(i / 3)][(u8)(i % 3)]);
    func_800B4540();
    func_800B44F4();
    func_800B458C(500, 500, 0);
    func_800B45E4(525, 425, 0);
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C5758);

void func_800C5DDC(void) {
    func_800B2B28(D_800F5F88);
}

void func_800C5E00(void) {
    func_800B2844((*(void * *)&D_800F5F88), 0x20U);
}

void func_800C5E24(void) {
    func_800B3264();
    func_800B8DC4();
    func_800B4AA8((&D_800F5F08)[32], -1, -1, -1, -1, -1, -1, D_80124146);
    if (D_80124CFD != -1 || D_80124CFF != D_80124CFD) {
        if (D_80124C94 == 0) {
            D_80124C94 = D_80140FC0(0);
        } else {
            func_800B3EAC(0, D_801240E8, D_801240EC, D_801240F0);
        }
    } else if (D_80124CFE != D_80124CFF || D_80124D00 != D_80124CFE) {
        func_800B463C(D_80124CFE);
    }
    func_800B32D0();
}

void func_800C5F20(void) {
    if ((D_80124CFD != -1) || (D_80124CFE != D_80124CFD) || (D_80124D01 != D_80124CFE)) {
        func_800C8554(0x50);
        return;
    }
    if ((D_80124CFF != D_80124D01) || (D_80124D00 != D_80124CFF)) {
        func_800C891C(0x50);
    }
}

void func_800C5F9C(void) {
    func_800B2AFC(D_800F5F88);
    func_800B8D8C();
}

void func_800C5FC8(void) {

}

typedef struct { char name[12]; u32 value; } SaveRecord;
typedef struct { SaveRecord e[5]; } SaveRecTable;
/* Save data (D_800F641C): per-track record tables, 12 tracks. */
typedef struct { u8 p0[4]; SaveRecTable highA[12]; SaveRecTable times[12]; SaveRecTable highB[12]; } SaveData;
#define SAVE_DATA ((SaveData *)D_800F641C)
extern char *D_80141230;
/* Inserts (name, value) into track's high-value record table (highA); an entry with the placeholder name is overwritten. */
void func_800C5FD0(char *name, u32 value, u8 track) {
    SaveRecTable tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        if (strcmp(SAVE_DATA->highA[track].e[i].name, D_80141230) == 0) {
            strcpy((s32 *)SAVE_DATA->highA[track].e[i].name, (s32)name);
            SAVE_DATA->highA[track].e[i].value = value;
            return;
        }
        if (SAVE_DATA->highA[track].e[i].value < value) {
            tmp = SAVE_DATA->highA[track];
            memcpy(&SAVE_DATA->highA[track].e[i + 1], &tmp.e[i], (4 - i) * sizeof(SaveRecord));
            strcpy((s32 *)SAVE_DATA->highA[track].e[i].name, (s32)name);
            SAVE_DATA->highA[track].e[i].value = value;
            return;
        }
    }
}

/* Inserts (name, value) into track's best-time table (times, lower is better); an entry with the placeholder name is overwritten. */
void func_800C615C(char *name, u32 value, u8 track) {
    SaveRecTable tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        if (strcmp(SAVE_DATA->times[track].e[i].name, D_80141230) == 0) {
            strcpy((s32 *)SAVE_DATA->times[track].e[i].name, (s32)name);
            SAVE_DATA->times[track].e[i].value = value;
            return;
        }
        if (value < SAVE_DATA->times[track].e[i].value) {
            tmp = SAVE_DATA->times[track];
            memcpy(&SAVE_DATA->times[track].e[i + 1], &tmp.e[i], (4 - i) * sizeof(SaveRecord));
            strcpy((s32 *)SAVE_DATA->times[track].e[i].name, (s32)name);
            SAVE_DATA->times[track].e[i].value = value;
            return;
        }
    }
}

/* Inserts (name, value) into track's second high-value record table (highB); an entry with the placeholder name is overwritten. */
void func_800C62E8(char *name, u8 track, u32 value) {
    SaveRecTable tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        if (strcmp(SAVE_DATA->highB[track].e[i].name, D_80141230) == 0) {
            strcpy((s32 *)SAVE_DATA->highB[track].e[i].name, (s32)name);
            SAVE_DATA->highB[track].e[i].value = value;
            return;
        }
        if (SAVE_DATA->highB[track].e[i].value < value) {
            tmp = SAVE_DATA->highB[track];
            memcpy(&SAVE_DATA->highB[track].e[i + 1], &tmp.e[i], (4 - i) * sizeof(SaveRecord));
            strcpy((s32 *)SAVE_DATA->highB[track].e[i].name, (s32)name);
            SAVE_DATA->highB[track].e[i].value = value;
            return;
        }
    }
}

void func_800C6474(RaceSt_ *rs) {
    u8 slot;

    slot = D_80123EBF + D_80123EBE * 3 - 1;
    func_800C5FD0(D_80124148 + 8, rs->res[0].raceTime, slot);
    if (D_80123EA0 == 2) {
        func_800C5FD0(D_8012414C + 8, rs->res[1].raceTime, slot);
    }
}

void func_800C6500(RaceSt_ *rs) {
    u8 slot;

    slot = D_80123EBF + D_80123EBE * 3 - 1;
    if (rs->res[0].lap == D_80123EC1) {
        func_800C615C(D_80124148 + 8, rs->res[0].lapTime, slot);
    }
    if (D_80123EA0 == 2 && rs->res[1].lap == D_80123EC1) {
        func_800C615C(D_8012414C + 8, rs->res[1].lapTime, slot);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C65AC);

void func_800C6700(void) {
    s32 buf[8];

    sprintf(buf, &D_80101020, D_8014104C(10));
    func_800B3144(buf);
    func_800B8D74();
    D_80124D04 = D_80141048();
    D_80124D08 = 0;
    D_80124D09 = 0;
}


void func_800C6770(void) {
    func_800B3264();
    func_800B8DC4();
    if ((u8)D_80124145 == 0xFF && *(u8 *)&D_80124D08 == 0) {
        D_80124D08 = 1;
        D_80140FAC(0x36, 1, 1.0f, 1.0f);
    }
    func_800B32D0();
}


void func_800C67EC(void) {
    func_800B8D8C();
    D_80124D09 = 1;
}

void func_800C6814(void) {
    if ((u32)D_80141048() > (u32)(D_80124D04 + 10000)) {
        func_800C9F18(&D_800F9F70, 0x7D);
    }
}


void func_800C6860(void) {
    if (*(u8 *)&D_80124D09 == 0 && (u32)D_80141048() > (u32)(D_80124D04 + 3000)) {
        func_800C8554(0x50);
        if (D_80123EA0 == 2) {
            func_800C891C(0x50);
        }
    }
}


void func_800C68CC(void) {
    switch (*(s32 *)(D_800F641C + 0x878)) {
    case 0:
    case 1:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x7E);
        break;
    case 2:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x7F);
        break;
    case 3:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x80);
        break;
    case 4:
    default:
        func_800C9F18(&(*(s32 *)&D_800F9F70), 0x81);
        break;
    }
}

void func_800C6950(void) {
    func_800C8554(0x50);
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C696C);

void func_800C6BEC(void) {
    if (D_80140F9C(&D_80123F48) == 0) {
        func_800C9F18(&D_800F9F70, 0x82);
    }
    func_800B8334();
}


void func_800C6C3C(void) {
    if (D_80140FA8() != 0) {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}


void func_800C6C7C(void) {
    strcpy(&D_80138D20, D_80124148 + 8);
    strcpy(&D_801345B8, D_8012414C + 8);
}

s32 func_800C6CC0(u32 arg0) {
    return (arg0 >> 3) & 0x10;
}

s32 func_800C6CCC(u32 arg0) {
    return (arg0 >> 2) & 0x40;
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C6CD8);

void func_800C6FB8(void) {
    u32 *p = (u32 *)&D_80123F48;

    D_80140F9C(p);
    switch (*p) {
        case 1:
            func_800C9F18(&D_800F9F70, 0x83);
            break;
        case 2:
            func_800C9F18(&D_800F9F70, 0x84);
            break;
        case 3:
            func_800C9F18(&D_800F9F70, 0x85);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x82);
            break;
    }
    func_800B8334();
    ((s32 *)D_80124148)[15] = 0;
    ((s32 *)D_8012414C)[15] = 0;
}


void func_800C708C(void) {
    if (D_80140FA8() != 0) {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C70CC);

void func_800C756C(void) {
    u32 *p = (u32 *)&D_80123F48;

    D_80140F9C(p);
    switch (*p) {
        case 1:
            func_800C9F18(&D_800F9F70, 0x83);
            break;
        case 2:
            func_800C9F18(&D_800F9F70, 0x84);
            break;
        case 3:
            func_800C9F18(&D_800F9F70, 0x85);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x82);
            break;
    }
    func_800B8334();
    ((s32 *)D_80124148)[15] = 0;
    ((s32 *)D_8012414C)[15] = 0;
}


void func_800C7640(void) {
    if (D_80140FA8() != 0) {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}


INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C7680);

void func_800C795C(void) {
    u32 *p = (u32 *)&D_80123F48;

    D_80140F9C(p);
    switch (*p) {
        case 1:
            func_800C9F18(&D_800F9F70, 0x83);
            break;
        case 2:
            func_800C9F18(&D_800F9F70, 0x84);
            break;
        case 3:
            func_800C9F18(&D_800F9F70, 0x85);
            break;
        default:
            func_800C9F18(&D_800F9F70, 0x82);
            break;
    }
    func_800B8334();
    ((s32 *)D_80124148)[15] = 0;
    ((s32 *)D_8012414C)[15] = 0;
}


void func_800C7A30(void) {
    if (D_80140FA8() != 0) {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}


u32 func_800C7A70(u32 arg0) {
    return ((arg0 & 0xFF) % 41) & 0xFF;
}

void func_800C7AA4(void) {
    s32 buf[4];
    u8 i;
    u8 r;
    u8 c;
    s32 a;
    s32 b;
    s32 d;
    Race7A_ *g;

    func_800B3168();
    func_800C6C7C();
    memset(D_80124D10, 1, 0x29);
    strcpy((s32 *)D_80123EA8.p[0].name, ((s32 *)&D_800FA0DC)[((u8 *)D_80124148)[0x20]]);
    D_80123EA8.p[0].f10 = ((u8 *)D_80124148)[0x20];
    D_80123EA8.p[0].f0C = 1;
    D_80123EA8.p[0].f11 = 0;
    D_80123EA8.p[0].f12 = func_800BB598(0);
    D_80124D10[((u8 *)D_80124148)[0x20]] = 0;
    if (((u8 *)D_80124148)[0x22] == 2) {
        D_80123EA8.cnt = 3;
        D_80123EA8.f9C = 0xFF;
        D_80123EA8.p[0].f12 = 2;
        memcpy(&D_80123EA8.p[1], D_80101044, 9);
        D_80123EA8.p[1].f10 = 0x26;
        D_80123EA8.p[1].f0C = 0;
        D_80123EA8.p[1].f11 = 0;
        D_80123EA8.p[1].f12 = 0;
        memcpy(&D_80123EA8.p[2], D_80101050, 9);
        D_80123EA8.p[2].f10 = 0x27;
        D_80123EA8.p[2].f0C = 0;
        D_80123EA8.p[2].f11 = 0;
        D_80123EA8.p[2].f12 = 1;
    } else {
        D_80123EA8.cnt = 6;
        D_80123EA8.f9C = D_800F641C[0x87E];
        for (i = 1; i < 6; i++) {
            r = (u8)func_800C7A70(((u8 *)D_80124148 + i)[7]);
            do {
                r++;
                if (r >= 0x29) {
                    r = 0;
                }
            } while ((u8)func_800B7DFC(r, D_80124148) == 0 || D_80124D10[r] == 0);
            D_80124D10[r] = 0;
            strcpy((s32 *)D_80123EA8.p[i].name, ((s32 *)&D_800FA0DC)[r]);
            D_80123EA8.p[i].f10 = r;
            D_80123EA8.p[i].f0C = 0;
            D_80123EA8.p[i].f11 = 0;
            D_80123EA8.p[i].f12 = func_800BB598(i);
        }
    }
    c = D_800F60D4[((u8 *)D_80124148)[0x22]][((u8 *)D_80124148)[0x23]];
    D_80123EA8.trk = c / 3;
    D_80123EA8.sb = c % 3 + 1;
    strcpy((s32 *)D_80123EA8.name, D_800F9FD0[D_80123EA8.trk]);
    D_80123EA8.f19 = 6;
    D_80123EA8.f1A = D_800FA010[D_80123EA8.trk][D_80123EA8.sb - 1];
    D_80123EA8.f20 = 0;
    if (*(s32 *)(D_80124148 + 0x3C) != 0) {
        D_80123EA8.f1C = *(s32 *)(D_80124148 + 0x3C);
    } else if (*(s32 *)(D_8012414C + 0x3C) != 0) {
        D_80123EA8.f1C = *(s32 *)(D_8012414C + 0x3C);
    } else {
        D_80123EA8.f1C = 0;
    }
    g = &D_80123EA8;
    g->flags = 0x20;
    a = func_800C6CC0(*(s32 *)(D_80124148 + 0x3C));
    b = func_800C6CC0(*(s32 *)(D_8012414C + 0x3C));
    d = func_800C6CCC(*(s32 *)(D_80124148 + 0x3C));
    g->flags |= a | b | d | func_800C6CCC(*(s32 *)(D_8012414C + 0x3C));
    if (((u8 *)D_80124148)[0x22] == 1) {
        g->flags |= 0x100;
        sprintf(buf, (s32 *)&D_8010105C, ((u8 *)D_80124148)[0x23] + 1);
        strcpy((s32 *)g->sub, (s32)buf);
    } else if (((u8 *)D_80124148)[0x22] == 2) {
        g->flags |= 0x100;
        ((s32 (*)(s32 *, s32 *))sprintf)(buf, (s32 *)&D_80101064);
        strcpy((s32 *)g->sub, (s32)buf);
    }
    if ((u8)D_80140F98(&D_80123EA8) == 0) {
        func_800C9F18(&D_800F9F70, 0x82);
    }
    func_800B8358();
}

INCLUDE_ASM("asm/nonmatchings/unit_53B30", func_800C7F88);

void func_800C80C8(void) {
    if (D_80140FA8() != 0) {
        func_800C9F18(&D_800F9F70, 0x11);
    }
}

