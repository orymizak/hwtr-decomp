#include "common.h"
#include "include_asm.h"
typedef struct { u16 a; u16 b; u16 id; u8 state; u8 flags; u8 x; u8 y; } PakSlotQ_;
typedef struct { u16 a; u16 b; u8 p4[2]; u8 state; } PakInfoQ_;
typedef struct { u8 ch; u8 p1[7]; PakInfoQ_ *info; } ArgsQ_;
typedef struct { u8 p0[0xC]; ArgsQ_ *args; } ReqQ_;
typedef struct { u16 id; u8 state; u8 flags; u8 pad[6]; } PakSlotJ_;
extern PakSlotJ_ D_80140F6C[4];
void func_800D7450(void *a);
typedef struct { u8 b[0x68]; } PfsG_;
typedef struct { u8 p0[0xC]; u8 *args; } ReqG_;
extern u8 D_8014EB40[];
extern PfsG_ D_8014E998[];
s32 osMotorInit(void *mq, PfsG_ *pfs, s32 channel);
typedef struct { u8 *unk0; } ReqArgA_;
typedef struct { u8 p0[0xC]; u8 *args; } ReqA_;
extern u8 D_80140F6A[];

s32 func_800D74F0(s32 *);
extern s32 D_800FB210;
extern u8 D_80140F6F;

void func_800D69E0(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_80140F6C[i].state = 2;
        D_80140F6C[i].flags = 0;
        D_80140F6C[i].id = i;
    }
    func_800D7450(((u8 *)&D_800FB210));
}


void func_800D6A40(void) {
    func_800D74F0(&D_800FB210);
}

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6A60);

INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6C4C);

s32 func_800D6DCC(ReqG_ *r) {
    u8 ch = r->args[0];
    return osMotorInit(D_8014EB40, &D_8014E998[ch], ch);
}


INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6E14);

s32 func_800D6E5C(void) {
    u32 var_a0;
    u8 *var_v1;

    var_a0 = 0;
    var_v1 = &D_80140F6F;
    do {
        var_a0 += 1;
        *var_v1 &= 0x7F;
        var_v1 += 0xA;
    } while (var_a0 < 4U);
    return 0;
}

extern PakSlotQ_ D_80140F68[4];
s32 func_800D6E8C(ReqQ_ *r) {
    ArgsQ_ *a = r->args;
    PakInfoQ_ *info = a->info;

    D_80140F68[a->ch].state = info->state;
    D_80140F68[a->ch].b = info->b;
    D_80140F68[a->ch].a = info->a;
    D_80140F68[a->ch].id = 0;
    return 0;
}

s32 func_800D6F18(ReqA_ *r) {
    *(s16 *)(D_80140F6A + r->args[0] * 10) = 0;
    return 0;
}


INCLUDE_ASM("asm/nonmatchings/unit_775E0", func_800D6F40);
