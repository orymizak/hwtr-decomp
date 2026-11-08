#include "common.h"
#include "include_asm.h"
typedef struct { u8 pad[0x10]; s32 p10; } Sub14_;
typedef struct { u8 pad[0x14]; Sub14_ *sub; u8 pad2[0x18]; s32 p30; } Item34_;
typedef struct { u16 a; u16 count; u8 pad[0x2C]; Item34_ *items; } ItemList34_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[0xC]; s32 p40; u8 pad3[0x10]; } Item54b_;
typedef struct { u16 count; u8 pad[0x2A]; Item54b_ *items; } ItemListB_;
void func_80073740(ItemListB_ *l);
typedef struct { u32 pad0 : 30; u32 b1 : 1; u32 b0 : 1; u8 pad[0x14]; } Slot18_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[0x20]; } Item54_;
typedef struct { u16 count; u8 pad[0x2A]; Item54_ *items; } ItemList_;
typedef struct { u32 flags; u8 pad[0x14]; } SlotM_;
s32 func_80070BA4(void *);
s32 func_800717C8(s32, s32, s32);
void func_80072EE0(u8 *, void *, s32, s32, s32);
void func_8007AFD4(s32, s32, void *);
void func_80076D6C(s32, s32, s32, s32, s32, f32, f32);
extern u16 D_800F20FA;
extern u16 D_800F20FC;
extern u8 D_800F2090;
typedef struct { s32 f0; s32 f4; u8 p8[0x208]; s32 f210; s32 f214; u8 p218[0xFC]; s32 f314; s32 f318; u8 p31C[0x4]; } S_D_8014C740;
typedef struct { u8 pad[8]; u8 b8; u8 pad2[0xB]; s32 *p14; u8 pad3[0x18]; s32 w30; } Obj74_;
typedef struct { u8 pad[0x30]; s32 flags; u8 pad2[8]; Obj74_ *p3C; u8 pad3[0x14]; } Item74_;
typedef struct { u16 count; u8 pad[0x2A]; Item74_ *items; u8 pad2[4]; s32 w34; } List74_;
void func_8007A004(s32 *, s32, u8);
extern S_D_8014C740 D_8014C740[];
extern u8 D_800F2060;
extern s32 D_8014C540;
extern s32 D_8014EC78;
extern u8 D_8014D184;
extern u8 D_8014D188;
extern u8 D_8014D18C;
extern u8 D_8014D190;
extern u8 D_8014D194;
extern u8 D_8014D195;
extern u8 D_8014D196;
extern s32 D_8014C64C;
extern u8 D_8014D180;
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

s32 func_80072D6C(u8 *a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    u8 buf[0x30];

    if ((func_800717C8(a1, a2, *(s32 *)(a0 + 0x38)) & 0xFF) != 1) {
        if (*(s32 *)(a0 + 0x34) != a3) {
            *(s32 *)(a0 + 0x30) &= ~0x80;
        }
        return 0;
    }
    if (*(s32 *)(a0 + 0x3C) != 0) {
        func_8007AFD4(a1, a2, buf);
        func_80072EE0(a0, buf, func_80070BA4(buf), a3, a4);
    }
    return 1;
}

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

ItemList_ *func_800734AC(ItemList_ *l) {
    u32 i;

    for (i = 0; i < l->count; i++) {
        l->items[i].flags |= 0x4000;
    }
    return l;
}


void func_800734F4(s32 arg0) {
    if (arg0 != 0) {
        func_80073530();
        func_80073740(arg0);
        Memory_Free(arg0);
    }
}

s32 func_80073530(ItemList34_ *l) {
    s32 i;

    for (i = 0; i < l->count; i++) {
        if (l->items[i].p30 != 0) {
            Memory_Free(l->items[i].p30);
            l->items[i].p30 = 0;
        }
        if (l->items[i].sub != NULL && l->items[i].sub->p10 != 0) {
            Memory_Free(l->items[i].sub->p10);
            l->items[i].sub->p10 = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800735E4);

void func_800759F0(void *arg0);
void func_80073740(ItemListB_ *l) {
    s32 i;

    for (i = 0; i < l->count; i++) {
        if (l->items[i].p40 != 0) {
            Memory_Free(l->items[i].p40);
            l->items[i].p40 = 0;
            ((void (*)(Item54b_ *))func_800759F0)(&l->items[i]);
        }
    }
}


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

void func_800740DC(List74_ *l, u8 v) {
    u32 i;
    Obj74_ *o;

    if (l != NULL) {
        for (i = 0; i < l->count; i++) {
            o = l->items[i].p3C;
            o->b8 = v;
            if (o->w30 != 0) {
                ((s32 (*)(void *, s32 *, s32, s32, s32))func_80079140)(o, &D_3000000, o->w30, l->w34, l->items->flags & 0x4000);
            }
            if (o->p14 != NULL && o->p14[4] != 0) {
                func_8007A004(o->p14, l->w34, o->b8);
            }
        }
    }
}

void func_800741AC(s32 arg0) {
    func_800741D8();
    Memory_Free(arg0);
}

void func_800757E8(s32);
s32 func_800741D8(ItemListB_ *l) {
    s32 i;

    if (l != NULL) {
        func_800757E8(((s8 *)l)[0xFE]);
        func_80073530(l);
        for (i = 0; i < l->count; i++) {
            if (l->items[i].p40 != 0) {
                Memory_Free(l->items[i].p40);
                l->items[i].p40 = 0;
                func_800759F0(&l->items[i]);
            }
        }
    }
}

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

void func_80075580(void) {
    s32 var_v0;

    var_v0 = 0x168;
    D_80134798 = 0;
    D_8014C64C = 0;
    do {
        *(s32 *)(&D_8014D180 + var_v0) = 0;
        var_v0 -= 0x18;
    } while (var_v0 >= 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800755B4);

void func_80075600(s32 arg0, s8 arg1, s8 arg2, s8 arg3) {
    s32 temp_v0;

    if (arg0 != -1) {
        temp_v0 = arg0 * 0x18;
        *(u8 *)(&D_8014D194 + temp_v0) = arg1;
        *(u8 *)(&D_8014D195 + temp_v0) = arg2;
        *(u8 *)(&D_8014D196 + temp_v0) = arg3;
    }
}

void func_80075640(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 temp_v0;

    if (arg0 != -1) {
        temp_v0 = arg0 * 0x18;
        *(f32 *)(&D_8014D184 + temp_v0) = arg1;
        *(f32 *)(&D_8014D188 + temp_v0) = arg2;
        *(f32 *)(&D_8014D18C + temp_v0) = arg3;
        *(f32 *)(&D_8014D190 + temp_v0) = arg4;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800756A0);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80075720);

s32 func_800757A4(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (!(((SlotM_ *)&D_8014D180)[i].flags & 2)) {
            ((SlotM_ *)&D_8014D180)[i].flags = 2;
            return i;
        }
    }
    return -1;
}


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

void func_80076CF8(s32 a, s32 b, s32 c, s32 d, s32 e) {
    if (d >= -a && e >= -b && a < D_800F20FA && b < D_800F20FC) {
        func_80076D6C(a, b, c, d, e, 1.0f, 1.0f);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80076D6C);

void func_80077228(void) {
    s32 temp_v1;

    if ((u32) (D_8014EC78 + 8) >= (u32) D_8014C740[D_8014C540].f4) {
        D_8014C740[D_8014C540].f0 = D_8014EC78;
        GfxPool_Grow(&D_8014C740[D_8014C540], 1U);
        D_8014EC78 = D_8014C740[D_8014C540].f0;
    }
    temp_v1 = D_8014EC78;
    D_8014EC78 = temp_v1 + 8;
    (*(s32 *)((u8 *)(temp_v1) + 0x0)) = 0xDE000000;
    (*(s32 *)((u8 *)(temp_v1) + 0x4)) = &D_800F2060;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_800772F4);

void func_800778A4(void) {
    s32 temp_v1;

    if ((u32) (D_8014EC78 + 8) >= (u32) D_8014C740[D_8014C540].f4) {
        D_8014C740[D_8014C540].f0 = D_8014EC78;
        GfxPool_Grow(&D_8014C740[D_8014C540], 1U);
        D_8014EC78 = D_8014C740[D_8014C540].f0;
    }
    temp_v1 = D_8014EC78;
    D_8014EC78 = temp_v1 + 8;
    (*(s32 *)((u8 *)(temp_v1) + 0x0)) = 0xDE000000;
    (*(s32 *)((u8 *)(temp_v1) + 0x4)) = &D_800F2090;
}

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80077970);

INCLUDE_ASM("asm/nonmatchings/unit_13490", func_80077E5C);
