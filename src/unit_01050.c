#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
#include "car.h"
void func_800CE480(s32, s32);
void *Memory_Alloc(s32);
extern void *D_80101E70;
extern u8 D_80138AB0;
void func_8006B838(void);
u8 func_8008BE7C(s32);
void Particles_FreeAll(void);
void func_80078980(s32);
typedef struct Node64_ { u8 pad[0xC]; f32 v; u8 pad2[4]; struct Node64_ *next; u8 pad3[2]; u8 id; } Node64_;
extern Node64_ *D_800F0100;
void nuPiReadRom(u32 devAddr, void *dram, u32 size);
void func_800CE6D4(void *data, s32 a);
typedef struct { s32 a; s32 b; s32 c; } Vec3J_;
extern Vec3J_ D_800EFF14;
typedef struct { s32 a; s32 b; s32 c; } Vec3I_;
extern Vec3I_ D_800EFF08;
extern Vec3f D_800EFF20;
extern Vec3f D_800EFF2C;
typedef struct { f32 f0; } S_D_800F01B4;
extern S_D_800F01B4 D_800F01B4[];
typedef struct { f32 f0; u8 p4[0x48]; } S_D_800EFF54;
extern S_D_800EFF54 D_800EFF54[];
extern f64 D_800FE868;
typedef struct { s32 f0; } S_D_800EFC78;
typedef struct { u8 kind; u8 mode; u8 pad[2]; u8 flag; u8 pad2[0x47]; } Slot_;
typedef struct { s32 h[6]; f32 rpm; f32 f1C; f32 f20; f32 f24; f32 f28; u8 p2C[0x18]; u8 b44; u8 b45; u8 p46[6]; } Snd29_;
typedef struct Nd29_ { u8 p0[0x10]; s32 h; struct Nd29_ *next; } Nd29_;
typedef struct N64c_ { f32 x; f32 y; f32 z; f32 v; s32 f10; struct N64c_ *next; u8 b18; u8 b19; u8 id; u8 b1B; } N64c_;
typedef struct { s32 w; u8 p4[0x10]; } E14s_;
typedef struct { u32 f0; u8 p4[0x14]; u8 n18; u8 p19[3]; u32 f1C; u8 p20[0x10]; E14s_ e[1]; } Cfg9_;
f32 func_800649F8(void *pos, void *vel);
s32 func_80064B58(void *);
extern u8 D_80138AAE;
extern u8 D_80138AAF;
extern u8 D_80138AB1;
void func_8007F4D0(void);
void func_8007C080(void);
void func_80066C9C(s32, u8);
u8 Race_Init(Cfg9_ *);
extern s32 D_800F0108[];
extern s32 D_800F0120;
extern s32 D_800F0124[];
extern s32 D_800F013C;
extern s32 D_800F0140[];
extern s32 D_800F0158;
s32 Rand_Range(s32);
extern s32 D_800EFC64;
extern Vec3f D_800FE7B8;
extern u8 D_800F01B0;
extern f64 D_800FE848;
void func_800CE810(s32);
extern Snd29_ D_800EFF38[];
void func_800CE4E0(s32, s32);
extern Slot_ D_800EFF7C[];
extern u8 D_800F0104;
s32 func_800CE550(s32 id, f32 val);
extern s32 D_800F01A0;
extern s32 D_800F01A4;
extern s32 D_800EFC94[];
extern s32 D_8014C0C0;
extern s32 D_8014C0A8;
extern s32 D_8014C0AC;
extern s32 D_8014C0B0;
extern s32 D_8014C0C4;
extern s32 D_8014C0C8;
extern s32 D_8014C0CC;
extern s32 D_8014C0D0;
extern s32 D_8014C0D4;
extern s32 D_8014C0D8;
extern s32 D_8014C0DC;
extern s32 D_8014C0E0;
extern s32 D_8014C0E4;
extern s32 D_8014C0E8;
extern s32 D_8014C0EC;
extern s32 D_8014C0F0;
extern s32 D_8014C0F4;
extern s32 D_8014C0F8;
extern s32 D_8014C0FC;
extern s32 D_8014C100;
extern s32 D_8014C104;
extern s32 D_8014C108;
extern s32 D_8014C10C;
extern s32 D_8014C110;
extern s32 D_8014C114;
extern s32 D_8014C118;
extern s32 D_8014C11C;
extern s32 D_8014C120;
extern s32 D_8014C124;
extern s32 D_8014C12C;
extern s32 D_8014C130;
extern s32 D_8014C144;
extern s32 D_8014C150;
extern S_D_800EFC78 D_800EFC78[];
s32 func_800CD8CC(s32, s32,s32,s32, s32);
extern s32 D_800EFC68;
extern f32 D_800EFC6C;
extern u8 D_800EFF60;
extern u8 D_800EFF4C;
extern u8 D_800EFF48;
extern u8 D_800EFF5C;
extern u8 D_800EFF44;
extern u8 D_800EFF58;
extern u8 D_800EFF40;
extern u8 D_800EFF50;
s32 func_800CE228(s32,s32);
s32 func_8006495C(void);
s32 func_800CDDE4(s32, s32);
s32 func_800CE374(s32,s32);
s32 func_800CE428(s32);
extern s32 D_800F01A8;
extern s32 D_800F01AC;
extern f64 D_800FE8A8;
extern s32 D_8014102C;
extern void *D_80141698;
extern void *D_8014169C;
extern void *D_801416A8;
extern void *D_801416B4;
extern void *D_801416B8;
extern void *D_801416BC;
extern void *D_801416C4;
extern void *D_801416C8;
extern s32 D_801416CC;
extern void *D_801416E8;
extern void *D_801416EC;
extern void *D_801416F4;
extern void *D_801416FC;
extern void *D_80141700;
extern void *D_80141704;
extern void *D_80141718;
extern void *D_8014171C;
extern void *D_80141720;
extern void *D_80141724;
extern void *D_80141728;
extern void *D_8014172C;
extern void *D_80141730;
extern void *D_80141734;
extern void *D_80141738;
extern void *D_8014173C;
extern void *D_80141744;
extern void *D_80141748;
extern void *D_8014174C;
extern void *D_80141754;
extern void *D_80141758;
extern s32 D_80141768;
extern void *D_8014177C;
extern void *D_80141780;
extern void *D_80141784;
extern void *D_80141788;
extern void *D_8014178C;
extern void *D_801417A8;
extern void *D_801417AC;
extern void *D_801417B0;
extern void *D_801417B4;

s32 func_8007F514();
s32 func_8008C08C();
s32 func_8006C468();
s32 func_800CA1C0();
s32 func_8006C488();
s32 func_800CA1F8(s32);
s32 func_800CA230();
s32 Font_DrawChar(s32, s32, s32, s32, s32);
s32 func_800CD600(s32 *);
extern s32 D_800F015C;
extern u8 D_800F0105;
extern f32 D_800EFC70;
extern f64 D_800FE898;
extern f32 D_800EFC74;
extern f64 D_800FE8A0;

s32 func_80060450(void) {
    return 1;
}

void func_800655CC();
void func_8006586C();
extern void *D_801416F0;
void func_80065940();
void func_80065C5C();
extern void *D_801416F8;
void func_800664A4();
void func_800687D8();
void func_800687E8();
void func_800688F4();
extern void *D_80141708;
void func_80069214();
void func_8006A358();
void func_8006A368();
void func_800658C4();
void func_800658D4();
void func_800667F8();
void func_80066BD4();
extern void *D_80141740;
void func_80066ECC();
void CarInfo_LoadEx();
extern void *D_80141760;
void func_80068390();
extern void *D_80141764;
void func_80068504();
extern void *D_80141750;
void func_800685E0();
void func_80066EFC();
void func_80067284();
void func_80067EFC();
void func_80067F40();
extern void *D_8014175C;
void func_80068020();
extern void *D_8014176C;
void func_80066758();
extern void *D_80141770;
void func_8006673C();
extern void *D_80141774;
void func_800667A8();
extern void *D_80141778;
void func_80066778();
extern void *D_801416D0;
void Font_Load();
extern void *D_801416D4;
void Font_Free();
extern void *D_801416D8;
void Font_GetCharWidth();
extern void *D_801416DC;
s32 func_80060C00(s32, s32, s32, s32);
extern void *D_801416E0;
void Draw2D_SetColor();
extern void *D_801416A4;
void func_80078320();
void func_80078388();
void func_80078624();
void func_80078630();
void func_8007863C();
void Camera_Apply();
void func_80078828();
void Time_GetMillis();
void func_80060B8C();
void func_80060BB8();
void func_80060BE4();
void func_800698C0();
void func_8006A1E0();
void func_8006A1F0();
void func_8006A234();
void func_8006A2B8();
extern void *D_80141790;
void func_8006A400();
extern void *D_80141710;
void func_8006A460();
extern void *D_8014170C;
void func_8006A450();
extern void *D_80141714;
void func_8006A4D4();
s32 func_80060990(void);
s32 func_80060998(void);
s32 func_800609A0(void);
s32 func_800609A8(void);
extern void *D_8014179C;
void File_GetSize();
extern void *D_801417A0;
void File_Load();
void Draw2D_BeginPrim();

/* Fills the service table at D_80141698..D_801417B4 (font, file, timer and game callbacks);
 * func_80060840 clears it again. */
s32 func_80060458(void) {
    D_801416E8 = &func_800655CC;
    D_801416EC = &func_8006586C;
    D_801416F0 = &func_80065940;
    D_801416F4 = &func_80065C5C;
    D_801416F8 = &func_800664A4;
    D_801416FC = &func_800687D8;
    D_80141700 = &func_800687E8;
    D_80141704 = &func_800688F4;
    D_80141708 = &func_80069214;
    D_80141718 = &func_8006A358;
    D_8014171C = &func_8006A368;
    D_80141730 = &func_800658C4;
    D_80141734 = &func_800658D4;
    D_80141738 = &func_800667F8;
    D_8014173C = &func_80066BD4;
    D_80141740 = &func_80066ECC;
    D_80141744 = &CarInfo_LoadEx;
    D_80141760 = &func_80068390;
    D_80141764 = &func_80068504;
    D_80141750 = &func_800685E0;
    D_80141748 = &func_80066EFC;
    D_8014174C = &func_80067284;
    D_80141758 = &func_80067EFC;
    D_80141754 = &func_80067F40;
    D_8014175C = &func_80068020;
    D_8014176C = &func_80066758;
    D_80141770 = &func_8006673C;
    D_80141774 = &func_800667A8;
    D_80141778 = &func_80066778;
    D_801416D0 = &Font_Load;
    D_801416D4 = &Font_Free;
    D_801416D8 = &Font_GetCharWidth;
    D_801416DC = &func_80060C00;
    D_801416E0 = &Draw2D_SetColor;
    D_801416A4 = &func_80078320;
    D_801416A8 = &func_80078388;
    D_801416B4 = &func_80078624;
    D_801416B8 = &func_80078630;
    D_801416BC = &func_8007863C;
    D_801416C4 = &Camera_Apply;
    D_801416C8 = &func_80078828;
    D_80141698 = &Time_GetMillis;
    D_8014169C = &Rand_Range;
    D_80141780 = &func_80060B8C;
    D_80141784 = &func_80060BB8;
    D_80141788 = &func_80060BE4;
    D_8014177C = &func_800698C0;
    D_80141720 = &func_8006A1E0;
    D_80141724 = &func_8006A1F0;
    D_80141728 = &func_8006A234;
    D_8014172C = &func_8006A2B8;
    D_80141790 = &func_8006A400;
    D_80141710 = &func_8006A460;
    D_8014170C = &func_8006A450;
    D_80141714 = &func_8006A4D4;
    D_801417A8 = &func_80060990;
    D_801417AC = &func_80060998;
    D_801417B0 = &func_800609A0;
    D_801417B4 = &func_800609A8;
    D_8014179C = &File_GetSize;
    D_801417A0 = &File_Load;
    D_8014178C = &Draw2D_BeginPrim;
    return 1;
}

s32 func_80060840(void) {
    D_801416E8 = 0;
    D_801416EC = 0;
    D_801416F4 = 0;
    D_801416FC = 0;
    D_80141700 = 0;
    D_80141704 = 0;
    D_80141718 = 0;
    D_8014171C = 0;
    D_80141730 = 0;
    D_80141734 = 0;
    D_80141738 = 0;
    D_8014173C = 0;
    D_80141744 = 0;
    D_80141748 = 0;
    D_8014174C = 0;
    D_80141758 = 0;
    D_80141754 = 0;
    D_80141768 = 0;
    D_8014102C = 0;
    D_801416A8 = 0;
    D_801416B4 = 0;
    D_801416B8 = 0;
    D_801416BC = 0;
    D_801416C4 = 0;
    D_801416C8 = 0;
    D_801416CC = 0;
    D_80141698 = 0;
    D_8014169C = 0;
    D_801417A8 = 0;
    D_801417AC = 0;
    D_801417B0 = 0;
    D_801417B4 = 0;
    D_8014177C = 0;
    D_80141780 = 0;
    D_80141784 = 0;
    D_80141788 = 0;
    D_80141720 = 0;
    D_80141724 = 0;
    D_80141728 = 0;
    D_8014172C = 0;
    D_8014178C = 0;
    return 1;
}

s32 func_80060990(void) {
    return 1;
}

s32 func_80060998(void) {
    return 1;
}

s32 func_800609A0(void) {
    return 1;
}

s32 func_800609A8(void) {
    func_8007F514();
    return 1;
}

u8 Game_StartRace(Cfg9_ *c) {
    s32 n = 0;
    s32 i;

    func_8007F4D0();
    func_8007C080();
    for (i = 0; i < c->n18; i++) {
        if (c->e[i].w == 1) {
            n++;
        }
    }
    if (c->f1C & 8) {
        D_80138AAE = 1;
    } else {
        D_80138AAE = 0;
    }
    if (c->f1C & 1) {
        D_80138AAF = 1;
    } else {
        D_80138AAF = 0;
    }
    if (c->f1C & 0x20) {
        D_80138AB1 = 1;
    } else {
        D_80138AB1 = 0;
    }
    if (c->f0 & 0x40) {
        D_80138AB0 = 1;
        func_8006B838();
    } else {
        D_80138AB0 = 0;
    }
    func_80066C9C(n, c->n18);
    if (!(c->f0 & 0x10)) {
        func_80078980(0);
    } else {
        func_80078980(1);
    }
    return Race_Init(c);
}

s32 Music_Stop(void);
u8 Game_EndRace(s32 a) {
    u8 *p = &D_80138AB0;
    u8 r;

    func_8006C468();
    Music_Stop();
    if (*p != 0) {
        func_8006B838();
    }
    *p = 0;
    r = func_8008BE7C(a);
    Particles_FreeAll();
    func_80078980(0);
    return r;
}


s32 func_80060B6C(void) {
    return func_8008C08C() & 0xFF;
}

void func_80060B8C(void) {
    func_8006C468();
    func_8006495C();
    func_800CA1C0();
}

void func_80060BB8(s32 arg0) {
    func_8006C488();
    func_800CA1F8(arg0);
}

void func_80060BE4(void) {
    func_800CA230();
}

s32 func_80060C00(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return Font_DrawChar(arg0 & 0xFF, arg1 & 0xFF, arg2 & 0xFFFF, arg3 & 0xFFFF, 0) & 0xFFFF;
}

void func_80060C30(void) {
    func_800CD600(&D_800F015C);
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060C50);

void func_80061454(s32 unused, u8 count, u8 *info);
s32 func_800616A0();
s32 Sfx_Play(u32 id, s32 mode, f32 vol, f32 pitch);
s32 func_80061A84(u32 track);
u8 func_80061B88(void);
s32 func_80061C70();
s32 func_80062950(void);
s32 func_80063434(Vec3I_ v);
s32 func_80063468(Vec3f a, Vec3f b);
s32 func_800634C0(Vec3J_ v);
s32 func_800634F4(u8 i, Vec3f v);
s32 func_80063550(u8 i, Vec3f v);
s32 func_800635AC(s32 a, f32 v);
s32 func_8006365C();
s32 func_80063938(u8 i);
s32 func_800639EC(s32 arg0, s32 arg1);
s32 func_80063A18(u8 car, f32 v);
s32 func_80063AC0();
s32 func_80063C18(u8 car);
s32 func_80063C88(s32 arg0, s32 arg1);
s32 func_80063CB4();
s32 func_80063E0C(u8 car);
s32 func_80063E7C(s32 arg0, s32 arg1);
s32 func_80063EA8(u8 i, s32 k, f32 lvl);
s32 func_80063FE8(u8 car);
s32 func_80064058(u8 i, s32 k);
s32 func_80064194(u8 car);
s32 func_80064204(s32 arg0, s32 arg1);
u8 func_80064230(void);
s32 func_80064364();
s32 func_80064418();
s32 func_800645BC(u8 id);
s32 func_80064638(u8 id, f32 v);
s32 func_80064308(u8 id, Vec3f v);
s32 func_80061B94(s32 kind);
extern s32 D_8014C13C;
extern s32 D_8014C140;
extern s32 D_8014C0B4;
extern s32 D_8014C128;
extern s32 D_8014C134;
/* Installs the sound driver entry points used by the rest of the game. */
s32 func_800610DC(void) {
    D_8014C13C = (s32)func_80061454;
    D_8014C140 = (s32)func_800616A0;
    D_8014C0A8 = (s32)Sfx_Play;
    D_8014C0AC = (s32)func_80061A84;
    D_8014C0B4 = (s32)Music_Stop;
    D_8014C0B0 = (s32)func_80061B88;
    D_8014C0C0 = (s32)func_80061C70;
    D_8014C144 = (s32)func_80062950;
    D_8014C0C4 = (s32)func_80063434;
    D_8014C0C8 = (s32)func_80063468;
    D_8014C0CC = (s32)func_800634C0;
    D_8014C0D0 = (s32)func_800634F4;
    D_8014C0D4 = (s32)func_80063550;
    D_8014C0D8 = (s32)func_800635AC;
    D_8014C0DC = (s32)func_8006365C;
    D_8014C0E0 = (s32)func_80063938;
    D_8014C0E4 = (s32)func_800639EC;
    D_8014C0E8 = (s32)func_80063A18;
    D_8014C0EC = (s32)func_80063AC0;
    D_8014C0F0 = (s32)func_80063C18;
    D_8014C0F4 = (s32)func_80063C88;
    D_8014C0F8 = (s32)func_80063CB4;
    D_8014C0FC = (s32)func_80063E0C;
    D_8014C100 = (s32)func_80063E7C;
    D_8014C10C = (s32)func_80063EA8;
    D_8014C110 = (s32)func_80063FE8;
    D_8014C104 = (s32)func_80063EA8;
    D_8014C108 = (s32)func_80063FE8;
    D_8014C114 = (s32)func_80064058;
    D_8014C118 = (s32)func_80064194;
    D_8014C11C = (s32)func_80064204;
    D_8014C120 = (s32)func_80064230;
    D_8014C128 = (s32)func_80064364;
    D_8014C12C = (s32)func_80064418;
    D_8014C130 = (s32)func_800645BC;
    D_8014C134 = (s32)func_80064638;
    D_8014C124 = (s32)func_80064308;
    D_8014C150 = (s32)func_80061B94;
    return 1;
}

s32 func_80061334(void) {
    s32 *p = &D_8014C0C0;

    *p = 0;
    D_8014C144 = 0;
    D_8014C0A8 = 0;
    D_8014C0AC = 0;
    D_8014C0B0 = 0;
    *p = 0;
    D_8014C0C4 = 0;
    D_8014C0C8 = 0;
    D_8014C0CC = 0;
    D_8014C0D0 = 0;
    D_8014C0D4 = 0;
    D_8014C0D8 = 0;
    D_8014C0DC = 0;
    D_8014C0E0 = 0;
    D_8014C0E4 = 0;
    D_8014C0E8 = 0;
    D_8014C0EC = 0;
    D_8014C0F0 = 0;
    D_8014C0F4 = 0;
    D_8014C0F8 = 0;
    D_8014C0FC = 0;
    D_8014C100 = 0;
    D_8014C10C = 0;
    D_8014C110 = 0;
    D_8014C104 = 0;
    D_8014C108 = 0;
    D_8014C114 = 0;
    D_8014C118 = 0;
    D_8014C11C = 0;
    D_8014C120 = 0;
    D_8014C12C = 0;
    D_8014C130 = 0;
    D_8014C11C = 0;
    D_8014C124 = 0;
    D_8014C150 = 0;
    return 1;
}


void func_80061454(s32 unused, u8 count, u8 *info) {
    s32 i;
    u32 type;

    for (i = 0; i < count; i++) {
        D_800EFF7C[i].flag = 0;
        type = Car_Get(i)->w540 & 0xFF000000;
        switch (type) {
        case 0x00000000:
            D_800EFF7C[i].kind = 4;
            D_800EFF7C[i].mode = 3;
            break;
        case 0x01000000:
            D_800EFF7C[i].kind = 6;
            D_800EFF7C[i].mode = 5;
            break;
        case 0x02000000:
            D_800EFF7C[i].kind = 8;
            D_800EFF7C[i].mode = 7;
            break;
        case 0x03000000:
            D_800EFF7C[i].kind = 10;
            D_800EFF7C[i].mode = 9;
            break;
        case 0x04000000:
            D_800EFF7C[i].kind = 12;
            D_800EFF7C[i].mode = 11;
            break;
        case 0x05000000:
            D_800EFF7C[i].kind = 14;
            D_800EFF7C[i].mode = 13;
            break;
        case 0x06000000:
            D_800EFF7C[i].kind = 16;
            D_800EFF7C[i].mode = 15;
            break;
        case 0x07000000:
            D_800EFF7C[i].kind = 18;
            D_800EFF7C[i].mode = 17;
            break;
        case 0x08000000:
            D_800EFF7C[i].kind = 20;
            D_800EFF7C[i].mode = 19;
            break;
        case 0x09000000:
            D_800EFF7C[i].kind = 2;
            D_800EFF7C[i].mode = 1;
            break;
        case 0x0A000000:
            D_800EFF7C[i].kind = 21;
            D_800EFF7C[i].mode = 22;
            break;
        default:
            D_800EFF7C[i].kind = 10;
            D_800EFF7C[i].mode = 9;
            break;
        }
    }
    D_800F0104 = info[0x16] * 3 + (info[0x17] - 1);
    D_800F0105 = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800616A0);

s32 Sfx_Play(u32 id, s32 mode, f32 vol, f32 pitch) {
    s32 snd;
    s32 v;
    s32 pri;
    s32 h;

    snd = D_800EFC94[id];
    v = D_800EFC70 * 128.0f * vol;
    if (snd == 0) {
        return 0;
    }
    if (mode == 0) {
        pri = 0x30;
    } else if (mode == 1) {
        pri = 0x60;
    } else {
        return 0;
    }
    if (pitch > 12.0) {
        pitch = 12.0f;
    }
    if (v == 0) {
        v = 1;
    }
    switch (id) {
        case 0x1:
            if (func_800CE428(D_800F01A0) == 0) {
                h = func_800CD8CC(snd, v, 0x80, 0, pri);
                func_800CE550(h, pitch);
                D_800F01A0 = h;
            }
            return 1;
        case 0xD:
            if (func_800CE428(D_800F01A4) == 0) {
                h = func_800CD8CC(snd, v, 0x80, 0, pri);
                func_800CE550(h, pitch);
                D_800F01A4 = h;
            }
            return 1;
        case 0x36:
        case 0x38:
            D_800F01AC = func_800CD8CC(snd, v, 0x80, 1, pri);
            return 1;
        case 0x37:
            D_800F01A8 = func_800CD8CC(snd, v, 0x80, 1, pri);
            return 1;
        default:
            func_800CE550(func_800CD8CC(snd, v, 0x80, 0, pri), pitch);
            return 1;
    }
}

s32 func_80061A84(u32 track) {
    register s32 song;
    register s32 vol;

    song = D_800EFC78[track].f0;
    vol = 128.0f * D_800EFC6C;
    if (func_800CE428(D_800EFC68) != 0) {
        func_800CE374(D_800EFC68, 0);
        D_800EFC68 = 0;
    }
    D_800EFC68 = func_800CD8CC(song, vol, 0x80, 0, 0xFF);
    *(s8 *)&D_800F0105 = track;
    return 1;
}

s32 Music_Stop(void) {
    s32 *p = &D_800EFC68;

    if (func_800CE428(*p) != 0) {
        func_800CE374(*p, 0);
        *p = 0;
    }
    return 1;
}


u8 func_80061B88(void) {
    return D_800F0105;
}

s32 func_80061B94(s32 kind) {
    s32 vol = D_800EFC74 * 255.0f;
    s32 snd = 0;

    switch ((u32)kind) {
    case 0:
        snd = D_800F0108[Rand_Range(D_800F0120)];
        break;
    case 1:
        snd = D_800F0140[Rand_Range(D_800F0158)];
        break;
    case 2:
        snd = D_800F0124[Rand_Range(D_800F013C)];
        break;
    }
    if (snd != 0) {
        return func_800CD8CC(snd, vol, 0x80, 0, 0x70);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061C70);

s32 func_80062950(void) {
    s32 p0;
    s32 p1;
    s32 p2;
    s32 p3;
    s32 p4;
    s32 p5;
    s32 i;
    s32 v;
    s32 pan;
    f32 vol;
    f32 x;
    f32 zero;
    f32 pitch;
    Nd29_ *n;

    n = (*(Nd29_ * *)&D_800F0100);
    i = 0;
    p1 = func_800CE428(D_800EFF38[i].h[1]);
    p0 = func_800CE428(D_800EFF38[i].h[0]);
    p2 = func_800CE428(D_800EFF38[i].h[2]);
    p3 = func_800CE428(D_800EFF38[i].h[3]);
    p4 = func_800CE428(D_800EFF38[i].h[4]);
    p5 = func_800CE428(D_800EFF38[i].h[5]);
    if (p0 || p2 || p3 || p4 || p5 || p1) {
        vol = D_800EFC70;
        x = D_800EFF38[i].f1C;
        zero = 0.0f;
        pan = 0x80;
        if (p1) {
            pitch = (D_800EFF38[i].b45 != 13) ? (D_800EFF38[i].rpm - 1000.0f) * 0.0021818182f + -17.0f : 0.0f;
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f * (1.0 - x);
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[1], v);
            func_800CE4E0(D_800EFF38[i].h[1], pan);
            func_800CE550(D_800EFF38[i].h[1], pitch);
        }
        if (p0) {
            pitch = (D_800EFF38[i].rpm - 1000.0f) * 0.0021818182f + -17.0f;
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f * x;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[0], v);
            func_800CE4E0(D_800EFF38[i].h[0], pan);
            func_800CE550(D_800EFF38[i].h[0], pitch);
        }
        if (p2) {
            v = D_800EFF38[i].f20 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[2], v);
            func_800CE4E0(D_800EFF38[i].h[2], pan);
            func_800CE550(D_800EFF38[i].h[2], zero);
        }
        if (p3) {
            v = D_800EFF38[i].f24 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[3], v);
            func_800CE4E0(D_800EFF38[i].h[3], pan);
            func_800CE550(D_800EFF38[i].h[3], zero);
        }
        if (p4) {
            v = vol * 128.0f;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[4], v);
            func_800CE4E0(D_800EFF38[i].h[4], pan);
            func_800CE550(D_800EFF38[i].h[4], zero);
        }
        if (p5) {
            v = D_800EFF38[i].f28 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[5], v);
            func_800CE4E0(D_800EFF38[i].h[5], pan);
            func_800CE550(D_800EFF38[i].h[5], zero);
        }
    }
    i++;
    p1 = func_800CE428(D_800EFF38[i].h[1]);
    p0 = func_800CE428(D_800EFF38[i].h[0]);
    p2 = func_800CE428(D_800EFF38[i].h[2]);
    p3 = func_800CE428(D_800EFF38[i].h[3]);
    p4 = func_800CE428(D_800EFF38[i].h[4]);
    p5 = func_800CE428(D_800EFF38[i].h[5]);
    if (p0 || p2 || p3 || p4 || p5 || p1) {
        vol = D_800EFC70;
        x = D_800EFF38[i].f1C;
        zero = 0.0f;
        pan = 0x80;
        if (p1) {
            pitch = (D_800EFF38[i].b45 != 13) ? (D_800EFF38[i].rpm - 1000.0f) * 0.0021818182f + -17.0f : 0.0f;
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f * (1.0 - x);
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[1], v);
            func_800CE4E0(D_800EFF38[i].h[1], pan);
            func_800CE550(D_800EFF38[i].h[1], pitch);
        }
        if (p0) {
            if (D_800EFF38[i].b44 != 14) {
                pitch = (D_800EFF38[i].rpm - 1000.0f) * 0.0021818182f + -17.0f;
            } else {
                pitch = 0.0f;
            }
            pitch = pitch + zero;
            if (pitch > 12.0) {
                pitch = 12.0f;
            }
            v = vol * 128.0f;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[0], v);
            func_800CE4E0(D_800EFF38[i].h[0], pan);
            func_800CE550(D_800EFF38[i].h[0], pitch);
        }
        if (p2) {
            v = D_800EFF38[i].f20 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[2], v);
            func_800CE4E0(D_800EFF38[i].h[2], pan);
            func_800CE550(D_800EFF38[i].h[2], zero);
        }
        if (p3) {
            v = D_800EFF38[i].f24 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[3], v);
            func_800CE4E0(D_800EFF38[i].h[3], pan);
            func_800CE550(D_800EFF38[i].h[3], zero);
        }
        if (p4) {
            v = vol * 128.0f;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[4], v);
            func_800CE4E0(D_800EFF38[i].h[4], pan);
            func_800CE550(D_800EFF38[i].h[4], zero);
        }
        if (p5) {
            v = D_800EFF38[i].f28 * 128.0f * vol;
            if (v == 0) {
                v = 1;
            }
            func_800CE480(D_800EFF38[i].h[5], v);
            func_800CE4E0(D_800EFF38[i].h[5], pan);
            func_800CE550(D_800EFF38[i].h[5], zero);
        }
    }
    while (n != NULL) {
        if (n->h != 0) {
            func_800CE374(n->h, 1);
            n->h = 0;
        }
        n = n->next;
    }
    return 1;
}

s32 func_80063434(Vec3I_ v) {
    D_800EFF08 = v;
    return 1;
}

s32 func_80063468(Vec3f a, Vec3f b) {
    D_800EFF20 = a;
    D_800EFF2C = b;
    return 1;
}

s32 func_800634C0(Vec3J_ v) {
    D_800EFF14 = v;
    return 1;
}


s32 func_800634F4(u8 i, Vec3f v) {
    *(Vec3f *)((u8 *)&D_800EFF54[i] + 0x10) = v;
    return 1;
}


s32 func_80063550(u8 i, Vec3f v) {
    *(Vec3f *)((u8 *)&D_800EFF54[i] + 0x1C) = v;
    return 1;
}


s32 func_800635AC(s32 a, f32 v) {
    u8 f = D_800F01B0;
    s16 ok = 1;

    if ((v == D_800FE848) != f) {
        func_800CE810(v == 1.0f ? 5 : 2);
        if ((u8)ok) {
            D_800F01B0 = D_800F01B0 == 0;
        }
    }
    return ok;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006365C);

s32 func_80063938(u8 i) {
    D_800EFF38[i].p46[2] = 0;
    if (func_800CE428(D_800EFF38[i].h[0]) != 0) {
        func_800CE374(D_800EFF38[i].h[0], 0);
        D_800EFF38[i].h[0] = 0;
    }
    if (func_800CE428(D_800EFF38[i].h[1]) != 0) {
        func_800CE374(D_800EFF38[i].h[1], 0);
        D_800EFF38[i].h[1] = 0;
    }
    return 1;
}

s32 func_800639EC(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF50 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

s32 func_80063A18(u8 car, f32 v) {
    if (D_800F01B4[car].f0 < v || v == D_800FE868) {
        D_800EFF54[car].f0 = 1.0f;
    } else {
        D_800EFF54[car].f0 = 0;
    }
    D_800F01B4[car].f0 = v;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063AC0);

s32 func_80063C18(u8 car) {
    if (func_800CE428(D_800EFF38[car].h[2]) != 0) {
        func_800CE374(D_800EFF38[car].h[2], 0);
        D_800EFF38[car].h[2] = 0;
    }
    return 1;
}

s32 func_80063C88(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF58 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063CB4);

s32 func_80063E0C(u8 car) {
    if (func_800CE428(D_800EFF38[car].h[3]) != 0) {
        func_800CE374(D_800EFF38[car].h[3], 0);
        D_800EFF38[car].h[3] = 0;
    }
    return 1;
}

s32 func_80063E7C(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF5C + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

f32 func_80064988(void *a);
s32 func_80063EA8(u8 i, s32 k, f32 lvl) {
    s32 snd = D_800EFC94[k];
    s32 off;
    u8 *pos;
    f32 vol;
    s32 pan;
    f32 pitch;
    s32 busy;
    s32 v;

    if (snd == 0) {
        return 0;
    }
    off = i * 0x4C;
    pos = (u8 *)D_800EFC94 + 0x2D0 + off;
    vol = func_80064988(pos) * D_800EFC70;
    pan = func_80064B58(pos);
    pitch = func_800649F8(pos, (u8 *)D_800EFC94 + 0x2DC + off);
    busy = func_800CE428(*(s32 *)(&D_800EFF48 + off));
    v = lvl * 128.0f * vol;
    if (v == 0) {
        v = 1;
    }
    if (busy == 0) {
        *(s32 *)(&D_800EFF48 + off) = func_800CD8CC(snd, v, pan, 0, 0x40);
        func_800CE550(*(s32 *)(&D_800EFF48 + off), pitch);
        return 1;
    }
    return 0;
}

s32 func_80063FE8(u8 car) {
    if (func_800CE428(D_800EFF38[car].h[4]) != 0) {
        func_800CE374(D_800EFF38[car].h[4], 0);
        D_800EFF38[car].h[4] = 0;
    }
    return 1;
}

f32 func_80064988(void *a);
s32 func_80064058(u8 i, s32 k) {
    s32 snd = D_800EFC94[k];
    s32 off;
    u8 *pos;
    f32 vol;
    s32 pan;
    f32 pitch;
    s32 busy;
    s32 v;

    if (snd == 0) {
        return 0;
    }
    off = i * 0x4C;
    pos = (u8 *)D_800EFC94 + 0x2D0 + off;
    vol = func_80064988(pos) * D_800EFC70;
    pan = func_80064B58(pos);
    pitch = func_800649F8(pos, (u8 *)D_800EFC94 + 0x2DC + off);
    busy = func_800CE428(*(s32 *)(&D_800EFF4C + off));
    v = *(f32 *)(&D_800EFF60 + off) * 128.0f * vol;
    if (v == 0) {
        v = 1;
    }
    if (busy == 0) {
        *(s32 *)(&D_800EFF4C + off) = func_800CD8CC(snd, v, pan, 0, 0x40);
        func_800CE550(*(s32 *)(&D_800EFF4C + off), pitch);
        return 1;
    }
    return 0;
}

s32 func_80064194(u8 car) {
    if (func_800CE428(D_800EFF38[car].h[5]) != 0) {
        func_800CE374(D_800EFF38[car].h[5], 0);
        D_800EFF38[car].h[5] = 0;
    }
    return 1;
}

s32 func_80064204(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF60 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

u8 func_80064230(void) {
    Vec3f v = D_800FE7B8;
    N64c_ *n = Memory_Alloc(0x1C);
    N64c_ *p;
    N64c_ **head;
    s32 *cnt;

    n->x = v.x;
    n->y = v.y;
    n->z = v.z;
    n->v = 1.0f;
    n->f10 = 0;
    n->next = NULL;
    n->b19 = 0;
    n->b1B = 0;
    head = &(*(N64c_ * *)&D_800F0100);
    if (*head == NULL) {
        *head = n;
        n->id = 1;
    } else {
        p = *head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = n;
        n->id = p->id + 1;
    }
    cnt = &D_800EFC64;
    (*cnt)++;
    return n->id;
}

s32 func_80064308(u8 id, Vec3f v) {
    Node64_ *p = D_800F0100;

    while (p != NULL) {
        if (p->id == id) {
            ((Vec3f *)p)->x = v.x;
            ((Vec3f *)p)->y = v.y;
            ((Vec3f *)p)->z = v.z;
            return 1;
        }
        p = p->next;
    }
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064364);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064418);

s32 func_800645BC(u8 id) {
    Node64_ *p = D_800F0100;

    while (p != NULL) {
        if (p->id == id) {
            break;
        }
        p = p->next;
    }
    if (p == NULL) {
        return 0;
    }
    p->pad3[1] = 0;
    if (func_800CE428(*(s32 *)p->pad2) != 0) {
        func_800CE374(*(s32 *)p->pad2, 0);
        *(s32 *)p->pad2 = 0;
    }
    return 1;
}


s32 func_80064638(u8 id, f32 v) {
    Node64_ *p = D_800F0100;

    while (p != NULL) {
        if (p->id == id) {
            break;
        }
        p = p->next;
    }
    if (p == NULL) {
        return 0;
    }
    p->v = v;
    return 1;
}


void Audio_SetMusicVolume(u8 v) {
    f32 *p = &D_800EFC6C;

    *p = v / 255.0;
    if (func_800CE428(D_800EFC68) != 0) {
        func_800CE480(D_800EFC68, *p * 128.0f);
    }
}


u8 Audio_GetMusicVolume(void) {
    return D_800EFC6C * 255.0f;
}


void func_8006476C(s32 arg0) {
    D_800EFC70 = (f32) ((f64) (arg0 & 0xFF) / D_800FE898);
}

u8 func_80064798(void) {
    return D_800EFC70 * 255.0f;
}


void func_800647F8(s32 arg0) {
    D_800EFC74 = (f32) ((f64) (arg0 & 0xFF) / D_800FE8A0);
}

u8 func_80064824(void) {
    return D_800EFC74 * 255.0f;
}


void func_80064884(u8 vol) {
    func_800CDDE4(1, (f32)(vol / D_800FE8A8) * 32767.0f);
    if (vol == 0) {
        if (D_800F01A8 != 0 && func_800CE428(D_800F01A8) != 0) {
            func_800CE374(D_800F01A8, 0);
            D_800F01A8 = 0;
        }
        if (D_800F01AC != 0 && func_800CE428(D_800F01AC) != 0) {
            func_800CE374(D_800F01AC, 0);
            D_800F01AC = 0;
        }
    }
}

s32 func_8006495C(void) {
    func_800CE228(1, 1);
    return 1;
}

s32 func_80064980(void) {
    return 1;
}

f32 func_80064E68(Vec3f *a, Vec3f *b);
f32 func_80064988(void *a) {
    f32 d = func_80064E68(a, &D_800EFF08);

    d = sqrtf(d);

    if (d >= 2400.0) {
        return 0.0f;
    }
    return (2400.0 - d) / 2400.0;
}


f32 func_80064D3C(Vec3f *a, Vec3f *off, Vec3f *b);
/* Doppler pitch offset (clamped to +-7) for a source at pos moving at vel. */
f32 func_800649F8(void *pos, void *vel) {
    f32 a;
    f32 b;
    f32 ratio;

    /* The listener velocity (D_800EFF14) directly follows its position (D_800EFF08). */
    a = func_80064D3C(pos, &((Vec3f *)&D_800EFF08)[1], (Vec3f *)&D_800EFF08);
    b = func_80064D3C((Vec3f *)&D_800EFF08, vel, pos);
    if (b == 1.0f) {
        return 0.0f;
    }
    ratio = (a / 13500.0 + 1.0) / (1.0 - b / 13500.0);
    if (ratio < 1.0f) {
        ratio = 1.0f / ratio;
        if (ratio <= 2.0f) {
            return -(ratio - 1.0f) * 7.0f;
        }
        return -7.0f;
    }
    if (ratio > 1.0f) {
        if (ratio <= 2.0f) {
            return (ratio - 1.0f) * 7.0f;
        }
        return 7.0f;
    }
    return 0.0f;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064B58);

f32 func_80064D3C(Vec3f *a, Vec3f *off, Vec3f *b) {
    f32 d[3];
    f32 len;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 da;

    d[0] = a->x - b->x;
    d[1] = a->y - b->y;
    d[2] = a->z - b->z;
    len = sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    if (len == 0.0f) {
        return 0.0f;
    }
    nx = d[0] / len;
    ny = d[1] / len;
    nz = d[2] / len;
    da = -(nx * a->x + ny * a->y + nz * a->z);
    return len + (nx * (b->x + off->x) + ny * (b->y + off->y) + nz * (b->z + off->z) + da);
}

f32 func_80064E68(Vec3f *a, Vec3f *b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;

    return dx * dx + dy * dy + dz * dz;
}

void func_80064EA4(u32 devAddr, u32 size, s32 a) {
    nuPiReadRom(devAddr, (void *)0x80010400, size);
    func_800CE6D4((void *)0x80010400, a);
}


void func_80064EE8(u32 a, u32 sizeA, u32 b, u32 sizeB) {
    nuPiReadRom(a, (void *)0x80010400, sizeA);
    D_80101E70 = Memory_Alloc(sizeB);
    nuPiReadRom(b, D_80101E70, sizeB);
    func_800CE6D4((void *)0x80010400, (s32)D_80101E70);
}


void func_80064F5C(s32 arg0, u32 arg1) {
    nuPiReadRom(arg0, 0x80020400, (arg1 & 1) + arg1);
}

void func_80064F84(s32 arg0, u32 arg1) {
    nuPiReadRom(arg0, 0x80000400, (arg1 & 1) + arg1);
    func_800CE89C((void *)0x80000400);
}
