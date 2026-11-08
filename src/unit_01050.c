#include "common.h"
#include "include_asm.h"
void func_800CE480(s32, s32);
void *Memory_Alloc(s32);
extern void *D_80101E70;
extern u8 D_80138AB0;
void func_8006B838(void);
u8 func_8008BE7C(s32);
void func_8007C224(void);
void func_80078980(s32);
typedef struct Node64_ { u8 pad[0xC]; f32 v; u8 pad2[4]; struct Node64_ *next; u8 pad3[2]; u8 id; } Node64_;
extern Node64_ *D_800F0100;
void func_800D6240(u32 devAddr, void *dram, u32 size);
void func_800CE6D4(void *data, s32 a);
typedef struct { s32 a; s32 b; s32 c; } Vec3J_;
extern Vec3J_ D_800EFF14;
typedef struct { s32 a; s32 b; s32 c; } Vec3I_;
extern Vec3I_ D_800EFF08;
typedef struct { f32 x; f32 y; f32 z; } Vec3_;
extern Vec3_ D_800EFF20;
extern Vec3_ D_800EFF2C;
typedef struct { f32 f0; } S_D_800F01B4;
extern S_D_800F01B4 D_800F01B4[];
typedef struct { f32 f0; u8 p4[0x48]; } S_D_800EFF54;
extern S_D_800EFF54 D_800EFF54[];
extern f64 D_800FE868;
typedef struct { s32 f0; } S_D_800EFC78;
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
extern s32 D_80141698;
extern s32 D_8014169C;
extern s32 D_801416A8;
extern s32 D_801416B4;
extern s32 D_801416B8;
extern s32 D_801416BC;
extern s32 D_801416C4;
extern s32 D_801416C8;
extern s32 D_801416CC;
extern s32 D_801416E8;
extern s32 D_801416EC;
extern s32 D_801416F4;
extern s32 D_801416FC;
extern s32 D_80141700;
extern s32 D_80141704;
extern s32 D_80141718;
extern s32 D_8014171C;
extern s32 D_80141720;
extern s32 D_80141724;
extern s32 D_80141728;
extern s32 D_8014172C;
extern s32 D_80141730;
extern s32 D_80141734;
extern s32 D_80141738;
extern s32 D_8014173C;
extern s32 D_80141744;
extern s32 D_80141748;
extern s32 D_8014174C;
extern s32 D_80141754;
extern s32 D_80141758;
extern s32 D_80141768;
extern s32 D_8014177C;
extern s32 D_80141780;
extern s32 D_80141784;
extern s32 D_80141788;
extern s32 D_8014178C;
extern s32 D_801417A8;
extern s32 D_801417AC;
extern s32 D_801417B0;
extern s32 D_801417B4;

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

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80060458);

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

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800609C8);

s32 func_80061B40(void);
u8 func_80060AFC(s32 a) {
    u8 *p = &D_80138AB0;
    u8 r;

    func_8006C468();
    func_80061B40();
    if (*p != 0) {
        func_8006B838();
    }
    *p = 0;
    r = func_8008BE7C(a);
    func_8007C224();
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

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800610DC);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061334);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061454);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800616A0);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006185C);

s32 func_80061A84(u32 arg0) {
    register s32 temp_s0;
    register s32 temp_s1;

    temp_s0 = D_800EFC78[arg0].f0;
    temp_s1 = (128.0f * D_800EFC6C);
    if (func_800CE428(D_800EFC68) != 0) {
        func_800CE374(D_800EFC68, 0);
        D_800EFC68 = 0;
    }
    D_800EFC68 = func_800CD8CC(temp_s0, temp_s1, 0x80, 0, 0xFF);
    (*(s8 *)&D_800F0105) = arg0;
    return 1;
}

s32 func_80061B40(void) {
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

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061B94);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80061C70);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80062950);

s32 func_80063434(Vec3I_ v) {
    D_800EFF08 = v;
    return 1;
}

s32 func_80063468(Vec3_ a, Vec3_ b) {
    D_800EFF20 = a;
    D_800EFF2C = b;
    return 1;
}

s32 func_800634C0(Vec3J_ v) {
    D_800EFF14 = v;
    return 1;
}


s32 func_800634F4(u8 i, Vec3_ v) {
    *(Vec3_ *)((u8 *)&D_800EFF54[i] + 0x10) = v;
    return 1;
}


s32 func_80063550(u8 i, Vec3_ v) {
    *(Vec3_ *)((u8 *)&D_800EFF54[i] + 0x1C) = v;
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800635AC);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_8006365C);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063938);

s32 func_800639EC(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF50 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

s32 func_80063A18(u32 arg0, f32 arg1) {
    register u8 temp_v1;
    u8 var_v0;

    temp_v1 = 0xFF & arg0;
    if ((D_800F01B4[temp_v1].f0 < arg1) || ((f64) arg1 == D_800FE868)) {
        var_v0 = 0xFF & arg0;
        D_800EFF54[temp_v1].f0 = 1.0f;
    } else {
        D_800EFF54[temp_v1].f0 = 0;
        var_v0 = arg0 & 0xFF;
    }
    D_800F01B4[var_v0].f0 = arg1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063AC0);

s32 func_80063C18(s32 arg0) {
    s32 temp_s0;

    temp_s0 = (arg0 & 0xFF) * 0x4C;
    if (func_800CE428(*(s32 *)(&D_800EFF40 + temp_s0)) != 0) {
        func_800CE374(*(s32 *)(&D_800EFF40 + temp_s0), 0);
        *(s32 *)(&D_800EFF40 + temp_s0) = 0;
    }
    return 1;
}

s32 func_80063C88(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF58 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063CB4);

s32 func_80063E0C(s32 arg0) {
    s32 temp_s0;

    temp_s0 = (arg0 & 0xFF) * 0x4C;
    if (func_800CE428(*(s32 *)(&D_800EFF44 + temp_s0)) != 0) {
        func_800CE374(*(s32 *)(&D_800EFF44 + temp_s0), 0);
        *(s32 *)(&D_800EFF44 + temp_s0) = 0;
    }
    return 1;
}

s32 func_80063E7C(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF5C + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80063EA8);

s32 func_80063FE8(s32 arg0) {
    s32 temp_s0;

    temp_s0 = (arg0 & 0xFF) * 0x4C;
    if (func_800CE428(*(s32 *)(&D_800EFF48 + temp_s0)) != 0) {
        func_800CE374(*(s32 *)(&D_800EFF48 + temp_s0), 0);
        *(s32 *)(&D_800EFF48 + temp_s0) = 0;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064058);

s32 func_80064194(s32 arg0) {
    s32 temp_s0;

    temp_s0 = (arg0 & 0xFF) * 0x4C;
    if (func_800CE428(*(s32 *)(&D_800EFF4C + temp_s0)) != 0) {
        func_800CE374(*(s32 *)(&D_800EFF4C + temp_s0), 0);
        *(s32 *)(&D_800EFF4C + temp_s0) = 0;
    }
    return 1;
}

s32 func_80064204(s32 arg0, s32 arg1) {
    *(s32 *)(&D_800EFF60 + ((arg0 & 0xFF) * 0x4C)) = arg1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064230);

s32 func_80064308(u8 id, Vec3_ v) {
    Node64_ *p = D_800F0100;

    while (p != NULL) {
        if (p->id == id) {
            ((Vec3_ *)p)->x = v.x;
            ((Vec3_ *)p)->y = v.y;
            ((Vec3_ *)p)->z = v.z;
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


void func_80064684(u8 v) {
    f32 *p = &D_800EFC6C;

    *p = v / 255.0;
    if (func_800CE428(D_800EFC68) != 0) {
        func_800CE480(D_800EFC68, *p * 128.0f);
    }
}


u8 func_8006470C(void) {
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


void func_80064884(s32 arg0) {
    s32 temp_s0;

    temp_s0 = arg0 & 0xFF;
    func_800CDDE4(1, (s32) ((f32) ((f64) temp_s0 / D_800FE8A8) * 32767.0f));
    if (temp_s0 == 0) {
        if ((D_800F01A8 != 0) && (func_800CE428(D_800F01A8) != 0)) {
            func_800CE374(D_800F01A8, 0);
            D_800F01A8 = 0;
        }
        if ((D_800F01AC != 0) && (func_800CE428(D_800F01AC) != 0)) {
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

f32 func_80064E68(void *arg0, void *arg1);
f32 func_80064988(void *a) {
    f32 d = func_80064E68(a, &D_800EFF08);

    d = sqrtf(d);

    if (d >= 2400.0) {
        return 0.0f;
    }
    return (2400.0 - d) / 2400.0;
}


INCLUDE_ASM("asm/nonmatchings/unit_01050", func_800649F8);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064B58);

INCLUDE_ASM("asm/nonmatchings/unit_01050", func_80064D3C);

f32 func_80064E68(void *arg0, void *arg1) {
    f32 temp_ft0;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_fv1 = *(f32 *)((u8 *)arg0 + 0x0) - *(f32 *)((u8 *)arg1 + 0x0);
    temp_ft0 = *(f32 *)((u8 *)arg0 + 0x4) - *(f32 *)((u8 *)arg1 + 0x4);
    temp_fv0 = *(f32 *)((u8 *)arg0 + 0x8) - *(f32 *)((u8 *)arg1 + 0x8);
    return (temp_fv1 * temp_fv1) + (temp_ft0 * temp_ft0) + (temp_fv0 * temp_fv0);
}

void func_80064EA4(u32 devAddr, u32 size, s32 a) {
    func_800D6240(devAddr, (void *)0x80010400, size);
    func_800CE6D4((void *)0x80010400, a);
}


void func_80064EE8(u32 a, u32 sizeA, u32 b, u32 sizeB) {
    func_800D6240(a, (void *)0x80010400, sizeA);
    D_80101E70 = Memory_Alloc(sizeB);
    func_800D6240(b, D_80101E70, sizeB);
    func_800CE6D4((void *)0x80010400, (s32)D_80101E70);
}


void func_80064F5C(s32 arg0, u32 arg1) {
    func_800D6240(arg0, 0x80020400, (arg1 & 1) + arg1);
}

void func_80064F84(s32 arg0, u32 arg1) {
    func_800D6240(arg0, 0x80000400, (arg1 & 1) + arg1);
    func_800CE89C((void *)0x80000400);
}
