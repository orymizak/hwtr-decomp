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

s32 __osMotorAccess(PfsG_ *, s32);
extern PakSlotQ_ D_80140F68[4];
extern u32 D_800FB1F0;
s32 func_800D6A60(PakSlotQ_ *r, u32 ch) {
    s32 ret;
    u32 n;

    ret = 0;
    switch (r->state) {
    case 2:
        break;
    case 1:
        if (r->id != 0) {
            ret = __osMotorAccess(&D_8014E998[ch], 0);
        } else {
            r->state = 2;
        }
        r->id--;
        break;
    case 3:
        if (r->b != 0) {
            r->id += r->a;
            n = r->id >> 8;
            r->id &= 0xFF;
            if (n != 0) {
                ret = __osMotorAccess(&D_8014E998[ch], 1);
            } else {
                ret = __osMotorAccess(&D_8014E998[ch], 0);
            }
        } else {
            ret = __osMotorAccess(&D_8014E998[ch], 0);
            r->state = 1;
            r->id = 2;
        }
        r->b--;
        break;
    case 4:
        ret = osMotorInit(D_8014EB40, &D_8014E998[ch], ch);
        if (ret == 0) {
            __osMotorAccess(&D_8014E998[ch], 0);
        }
        r->state = 1;
        r->id = 2;
        break;
    }
    return ret;
}

s32 func_800D6C4C(void *mesg) {
    u32 i;
    PakSlotQ_ *r;

    for (i = 0; i < 4; i++) {
        r = &D_80140F68[i];
        switch (r->flags) {
        case 0:
            break;
        case 1:
            if (func_800D6A60(r, i)) {
                r->flags = 0;
            }
            break;
        case 2:
            if (!r->x) {
                if (!(r->id % D_800FB1F0)) {
                    if (!osMotorInit(D_8014EB40, &D_8014E998[i], i)) {
                        r->x = 1;
                        r->y = 2;
                    }
                }
                r->id++;
            } else {
                if (func_800D6A60(r, i)) {
                    r->id = i;
                    r->x = 0;
                    r->y = 0;
                }
            }
            break;
        case 0x81:
        case 0x82:
            if (r->y == 2) {
                func_800D6A60(r, i);
            }
            break;
        }
    }
    return 0;
}

s32 func_800D6DCC(ReqG_ *r) {
    u8 ch = r->args[0];
    return osMotorInit(D_8014EB40, &D_8014E998[ch], ch);
}


s32 func_800D6E14(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_80140F6C[i].state = 4;
        D_80140F6C[i].flags |= 0x80;
    }
    return 0;
}

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
