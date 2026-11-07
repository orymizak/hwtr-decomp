#include "common.h"
#include "include_asm.h"
s32 func_80079140(void *, s32 *, s32,s32, s32);
s32 func_80079BD4(void *, s32);
s32 func_80079E48(void *);
s32 func_80079E68(void *);
extern s32 D_3000000;

s32 File_GetSize();
s32 File_Load(s32, s32);
s32 Memory_Alloc(s32);
s32 Memory_Free(s32);
s32 func_80073530();
s32 func_80073740(s32);
s32 func_800741D8();
extern s32 D_80134798;
extern s8 D_800F1FA0;
extern s8 D_800F1FA1;
extern s8 D_800F1FA2;

void func_80072890(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;

    temp_v1 = *(s32 *)((u8 *)arg0 + 0x3C);
    if (temp_v1 != -1) {
        *(s32 *)((u8 *)arg0 + 0x3C) = (s32) (arg2 + (temp_v1 * 0x34));
    } else {
        *(s32 *)((u8 *)arg0 + 0x3C) = 0;
    }
    temp_v1_2 = *(s32 *)((u8 *)arg0 + 0x4C);
    if (temp_v1_2 != -1) {
        *(s32 *)((u8 *)arg0 + 0x4C) = (s32) (arg1 + (temp_v1_2 * 0x54));
    } else {
        *(s32 *)((u8 *)arg0 + 0x4C) = 0;
    }
    temp_v1_3 = *(s32 *)((u8 *)arg0 + 0x50);
    if (temp_v1_3 != -1) {
        *(s32 *)((u8 *)arg0 + 0x50) = (s32) (arg1 + (temp_v1_3 * 0x54));
        return;
    }
    *(s32 *)((u8 *)arg0 + 0x50) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80072928);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80072BAC);

void func_80072D48(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x30) = (s32) (*(s32 *)((u8 *)arg0 + 0x30) & ~0x40);
}

void func_80072D5C(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x30) = (s32) (*(s32 *)((u8 *)arg0 + 0x30) | 0x40);
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80072D6C);

void func_80072E38(void *arg0, s32 arg1) {
    s32 temp_v0;
    void *temp_a0;
    void *temp_v1;

    if ((*(s32 *)((u8 *)(arg0) + 0x40)) == 0) {
        temp_v0 = func_80079E68((*(s32 *)((u8 *)(arg0) + 0x3C)));
        (*(s32 *)((u8 *)(arg0) + 0x40)) = temp_v0;
        if (temp_v0 != 0) {
            func_80079BD4((*(s32 *)((u8 *)(arg0) + 0x3C)), temp_v0);
            goto block_3;
        }
    } else {
block_3:
        temp_a0 = (*(s32 *)((u8 *)(arg0) + 0x3C));
        if ((*(s32 *)((u8 *)(temp_a0) + 0x30)) == 0) {
            (*(s32 *)((u8 *)((*(s32 *)((u8 *)(arg0) + 0x3C))) + 0x30)) = func_80079E48(temp_a0);
            temp_v1 = (*(s32 *)((u8 *)(arg0) + 0x3C));
            if ((*(s32 *)((u8 *)(temp_v1) + 0x30)) != 0) {
                func_80079140((*(s32 *)((u8 *)(arg0) + 0x3C)), &D_3000000, (*(s32 *)((u8 *)(temp_v1) + 0x30)), arg1, (*(s32 *)((u8 *)(arg0) + 0x30)) & 0x4000);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80072EE0);

s32 func_8007319C(s32 arg0) {
    s32 temp_v0;

    temp_v0 = Memory_Alloc(File_GetSize());
    File_Load(arg0, temp_v0);
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800731DC);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800734AC);

void func_800734F4(s32 arg0) {
    if (arg0 != 0) {
        func_80073530();
        func_80073740(arg0);
        Memory_Free(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80073530);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800735E4);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80073740);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800737C8);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800738C0);

s32 func_80073A48(s32 arg0) {
    s32 temp_v0;

    temp_v0 = Memory_Alloc(0x12C00);
    File_Load(arg0, temp_v0);
    return temp_v0;
}

s32 func_80073A88(s32 arg0) {
    s32 temp_v0;

    temp_v0 = Memory_Alloc(File_GetSize());
    File_Load(arg0, temp_v0);
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80073AC8);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800740DC);

void func_800741AC(s32 arg0) {
    func_800741D8();
    Memory_Free(arg0);
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800741D8);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80074274);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075084);

u8 func_80075564(void *arg0) {
    u8 var_v0;

    var_v0 = 0xFF;
    if (arg0 != NULL) {
        var_v0 = (*(u8 *)((u8 *)((*(s32 *)((u8 *)((*(s32 *)((u8 *)(arg0) + 0x2C))) + 0x3C))) + 0x8));
    }
    return var_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075580);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800755B4);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075600);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075640);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800756A0);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075720);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800757A4);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800757E8);

void func_80075858(void) {
    s32 temp_v0;

    temp_v0 = D_80134798 + 1;
    D_80134798 = temp_v0;
    if (temp_v0 >= 2) {
        D_80134798 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075888);

void func_800759F0(void *arg0) {
    s32 temp_a0;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0x44);
    if (temp_a0 != 0) {
        Memory_Free(temp_a0);
        *(s32 *)((u8 *)arg0 + 0x44) = 0;
        *(s32 *)((u8 *)arg0 + 0x30) = (s32) (*(s32 *)((u8 *)arg0 + 0x30) & ~0x100);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075A38);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075ABC);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076020);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800761D4);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076470);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076578);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800766FC);

void func_80076BB0(s8 arg0, s8 arg1, s8 arg2) {
    D_800F1FA0 = arg0;
    D_800F1FA1 = arg1;
    D_800F1FA2 = arg2;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076BCC);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076CF8);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076D6C);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80077228);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800772F4);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800778A4);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80077970);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80077E5C);
