#include "common.h"
#include "include_asm.h"
extern u8 D_800FACF8;
extern s32 D_800FB170;
extern s32 D_800FB174;
extern s32 D_8014C6D0;
extern s32 D_80127960;
s32 func_800CFE2C();
extern s32 D_8012795C;

extern s16 D_8012794C;
extern s16 D_8012794E;
s32 func_800CE7E8(s32);
s32 func_800D05EC();
extern void *D_80127958;
extern s32 D_80127968;
extern s32 D_8012796C;
extern s32 D_800FAEAC;
extern s32 D_80127970;
s32 func_800D23FC(s32);
extern s32 D_80127974;
extern s32 D_80127978;
extern s32 D_8012797C;
extern s32 D_80127980;
s32 func_800D4DD0(s32, s32);
s32 func_800D4E60(s32);
extern s32 D_80127938;
extern s32 D_80127954;
extern f64 D_801016A0;

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CDA10);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CDB5C);

void func_800CDDE4(s32 arg0, s16 arg1) {
    if (arg0 & 1) {
        D_8012794C = arg1;
    }
    if (arg0 & 2) {
        D_8012794E = arg1;
    }
}

s32 func_800CDE10(void) {
    s32 temp_v0;

    temp_v0 = func_800D05EC();
    func_800CE7E8(temp_v0);
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CDE40);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE090);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE110);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE228);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE2F4);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE374);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE428);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE480);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE4E0);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE550);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE5C0);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE64C);

void func_800CE6D4(s32 arg0) {
    func_800CFE2C();
    if (D_8012795C == 0) {
        D_8012795C = arg0;
    }
}

void *func_800CE710(void *arg0) {
    if ((arg0 != NULL) && (*(s32 *)((u8 *)arg0 + 0x10) < 0)) {
        D_80127958 = arg0;
    }
    return D_80127958;
}

void func_800CE738(s32 arg0) {
    if ((arg0 != 0) && ((*(s32 *)((u8 *)(arg0) + 0x10)) < 0)) {
        D_8012795C = arg0;
    }
}

s32 func_800CE75C(void) {
    return D_8012795C;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE768);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE7C4);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE7E8);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE810);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE840);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE89C);

s32 func_800CE920(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x4);
}

void func_800CE928(s32 arg0) {
    D_80127968 = arg0;
}

void func_800CE934(s32 arg0) {
    D_8012796C = arg0;
}

s32 func_800CE940(void) {
    return D_80127968;
}

void func_800CE94C(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x10) = arg1;
}

s32 func_800CE954(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x10);
}

void func_800CE95C(s32 arg0) {
    D_800FAEAC = arg0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE968);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE9CC);

void func_800CEA44(s32 arg0) {
    D_80127970 = arg0;
}

void func_800CEA50(s32 arg0) {
    s32 var_s0;

    var_s0 = arg0;
    if (var_s0 < 0x40) {
        var_s0 = 0x40;
    } else if (var_s0 >= 0x401) {
        var_s0 = 0x400;
    }
    D_80127980 = func_800D23FC(var_s0 * 8);
    D_8012797C = var_s0;
    D_80127978 = 0;
    D_80127974 = 0;
}

void func_800CEAB4(void *arg0) {
    u8 temp_v1;

    temp_v1 = (*(u8 *)((u8 *)(arg0) + 0x0));
    switch (temp_v1) {
    case 0:
        func_800D0858((u32) (*(s32 *)((u8 *)(arg0) + 0x4)), -2U, 1U);
        return;
    case 1:
        func_800D0858((u32) (*(s32 *)((u8 *)(arg0) + 0x4)), -2U, 0U);
        return;
    case 2:
        func_800D1A80((*(s32 *)((u8 *)(arg0) + 0x4)));
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CEB34);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CEBCC);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CEEBC);

void func_800CF408(void *arg0, s32 arg1) {
    if (*(u8 *)((u8 *)arg0 + 0xC9) != 0) {
        func_800D4E60(D_80127938 + (arg1 * 0x1C));
    }
    *(u8 *)((u8 *)arg0 + 0xC9) = 1U;
    func_800D4DD0(D_80127938 + (arg1 * 0x1C), *(s32 *)((u8 *)arg0 + 0x8));
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CF484);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CF5D8);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CF6E4);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CF754);

void func_800CF930(void *arg0) {
    *(u8 *)((u8 *)arg0 + 0xD9) = 0;
    *(s32 *)((u8 *)arg0 + 0x94) = (s32) *(s32 *)((u8 *)arg0 + 0x40);
    *(u8 *)((u8 *)arg0 + 0xDA) = (s8) (*(u8 *)((u8 *)arg0 + 0xBD) & 0x40);
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CF94C);

f32 func_800CFA04(s32 arg0) {
    u8 temp_v0;

    temp_v0 = (*(u8 *)((u8 *)(arg0) + 0xD0)) - 1;
    (*(u8 *)((u8 *)(arg0) + 0xD0)) = temp_v0;
    if (!(temp_v0 & 0xFF)) {
        if ((*(s8 *)((u8 *)(arg0) + 0xD1)) == 0) {
            (*(s8 *)((u8 *)(arg0) + 0xD1)) = (s8) (*(u8 *)((u8 *)(arg0) + 0xD8));
            (*(u8 *)((u8 *)(arg0) + 0xD0)) = (u8) (*(u8 *)((u8 *)(arg0) + 0xCE));
        } else {
            (*(s8 *)((u8 *)(arg0) + 0xD1)) = 0;
            (*(u8 *)((u8 *)(arg0) + 0xD0)) = (u8) (*(u8 *)((u8 *)(arg0) + 0xCF));
        }
    }
    return (f32) (*(s8 *)((u8 *)(arg0) + 0xD1));
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CFA5C);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CFAC0);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CFB84);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CFC98);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CFE2C);

s32 func_800CFFC4(s32 arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    do {
        temp_a1 = D_80127954 * 2;
        temp_v0 = D_80127954 & 0x48000000;
        temp_v1 = (temp_v0 == 0x48000000) | (temp_v0 == 0x08000000);
        D_80127954 = temp_a1;
        var_a2 += 1;
        if (temp_v1 != 0) {
            D_80127954 = temp_a1 | 1;
        }
    } while (var_a2 < 8);
    return (s32) ((f32) arg0 * (((f32) D_80127954 / 65536.0f) / 65536.0f));
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0058);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0194);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D035C);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0394);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0444);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D05EC);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0858);

s32 func_800D08AC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x38) = 0;
    *(s32 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x74) = 0;
    *(s32 *)((u8 *)arg0 + 0x78) = 0;
    *(s32 *)((u8 *)arg0 + 0x44) = 0;
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D08CC);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D08FC);

s32 func_800D091C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xB8) = 0;
    return arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0928);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0A50);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0B34);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0B48);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0B6C);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0BDC);

s32 func_800D0C50(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD5) = 0;
    *(s32 *)((u8 *)arg0 + 0x68) = 0;
    return arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0C60);

s32 func_800D0C90(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xB7) = 1;
    return arg1;
}

void *func_800D0CA0(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xB9) = (u8) *arg1;
    return arg1 + 1;
}

s32 func_800D0CB0(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xBA) = 1;
    return arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0CC0);

s32 func_800D0D18(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD6) = 1;
    return arg1;
}

s32 func_800D0D28(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD6) = 0;
    return arg1;
}

s32 func_800D0D34(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD7) = 1;
    return arg1;
}

s32 func_800D0D44(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD7) = 0;
    return arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0D50);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0DBC);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0E58);

s32 func_800D0E80(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xCE) = 0;
    return arg1;
}

s32 func_800D0E8C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD2) = 1;
    return arg1;
}

s32 func_800D0E9C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD2) = 0;
    return arg1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0EA8);

void *func_800D0EBC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xBD) = (s8) ((u8) *arg1 >> 1);
    return arg1 + 1;
}

s32 func_800D0ED0(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0ED8);

s32 func_800D0F20(void *arg0, s32 arg1) {
    *(s32 *)((u8 *)arg0 + 0x84) = 0;
    return arg1;
}

s32 func_800D0F2C(s32 arg0, s32 arg1) {
    return arg1 + 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0F34);

void *func_800D0FA4(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xCA) = (u8) *arg1;
    return arg1 + 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0FB4);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1004);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1054);

void *func_800D10A4(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xBC) = (u8) *arg1;
    return arg1 + 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D10B4);

void *func_800D1184(void *arg0, u8 *arg1) {
    f32 temp_fv0;

    temp_fv0 = (f32) ((f64) (f32) *arg1 * D_801016A0);
    *(f32 *)((u8 *)arg0 + 0x6C) = temp_fv0;
    *(f32 *)((u8 *)arg0 + 0x24) = (f32) (*(f32 *)((u8 *)arg0 + 0x70) * temp_fv0);
    return arg1 + 1;
}

void *func_800D11C0(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xD4) = (u8) *arg1;
    return arg1 + 1;
}

void *func_800D11D0(s32 arg0, void *arg1) {
    u8 temp_a0;
    void *var_s0;

    var_s0 = arg1;
    temp_a0 = (*(u8 *)var_s0);
    var_s0 += 1;
    if (D_80127960 == 1) {
        func_800D1A80((s32) temp_a0);
    }
    return var_s0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1214);

s32 func_800D1284(void *arg0, s32 arg1) {
    *(s16 *)((u8 *)arg0 + 0xAC) = 0;
    return arg1;
}

void func_800D1290(s32 arg0, s32 arg1) {
    if (D_800FB170 == 0) {
        D_800FB170 = arg0;
        if (D_800FB174 == 0) {
            func_800D19E0(arg1);
            D_8014C6D0 = D_800FB170;
            D_800FB174 = D_800FB170;
            func_800D12F8(arg1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D12F8);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1568);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D15B8);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1700);

void func_800D19E0(s32 arg0) {
    (*(u8 *)((u8 *)(arg0) + 0x1C)) = 6;
    (*(s32 *)((u8 *)(arg0) + 0x20)) = &D_800FACF8;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D19F8);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1A3C);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1A80);
