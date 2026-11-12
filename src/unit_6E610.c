#include "common.h"
#include "include_asm.h"
typedef struct { u32 unk0; s32 unk4; u8 p8[8]; s32 unk10; u8 p14[8]; s32 unk1C; u8 p20[0x58]; s32 unk78; u8 p7C[0xC0]; } VoiceW_;
typedef struct { u32 unk0; u8 p4[0xC]; s32 unk10; u8 p14[8]; s32 unk1C; u8 p20[0x24]; s32 unk44; u8 p48[0xF4]; } VoiceX_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x34]; s32 unk7C; u8 p80[0xBC]; } VoiceY_;
typedef struct { u8 p0[0x44]; s32 unk44; s32 unk48; u8 p4C[0x2C]; void *unk78; u8 p7C[0x22]; s16 unk9E; u8 pA0[0x10]; s16 unkB0; } SeqY_;
typedef struct { u8 p0[0x7C]; s32 unk7C; } SeqY2_;
typedef struct { s32 unk0; u8 p4[0x70]; s32 unk74; u8 p78[0x24]; s16 unk9C; u8 p9E[0xA]; s16 unkA8; u8 pAA[0x92]; } VoiceZ_;
typedef struct { u8 p0[0x74]; s32 unk74; s32 unk78; u8 p7C[0x1C]; s16 unk98; u8 p9A[2]; s16 unk9C; } SeqZ_;
extern s32 D_80127944;
typedef struct { u8 p0[0xC]; s32 unkC; u8 p10[4]; s32 unk14; u8 p18[0x20]; u8 *unk38; u8 p3C[0x66]; u16 unkA2; u8 pA4[0x18]; u8 unkBC; } SeqP_;
typedef struct { u8 p0[0xC]; s32 unkC; u8 p10[0x84]; s32 unk94; u8 p98[0x25]; u8 unkBD; u8 pBE[0x16]; u8 unkD4; u8 pD5[4]; u8 unkD9; u8 unkDA; } SeqO_;
typedef struct { u8 p0[0x10]; s32 unk10; u8 p14[4]; struct { s32 a; s32 b; } entries[1]; } BankG_;
typedef struct { u8 p0[4]; s32 unk4; u8 p8[0x3C]; s32 unk44; s32 unk48; u8 p4C[0x2C]; BankG_ *unk78; s32 unk7C; s32 unk80; u8 p84[0x1A]; s16 unk9E; u8 pA0[6]; s16 unkA6; u8 pA8[8]; s16 unkB0; } VoiceG_;
extern s32 D_80127950;
void func_800D0058(VoiceG_ *v);
typedef struct { u8 p0[0x24]; f32 unk24; u8 p28[0xC]; s32 unk34; s32 unk38; u8 p3C[0x30]; f32 unk6C; f32 unk70; u8 p74[0x2E]; u16 unkA2; u16 unkA4; u8 pA6[0x16]; u8 unkBC; u8 pBD[0x1E]; u8 unkDB; u8 pDC[4]; u8 *unkE0[4]; s32 unkF0[4]; s32 unk100[4]; u16 unk110[4]; u16 unk118[4]; u8 unk120[4]; u8 unk124[4]; f32 unk128[4]; } SeqN_;
typedef struct { u8 b[8]; } AudMsgE_;
void func_800D247C(void *dst, void *src, s32 n);
s32 func_800CEB34(AudMsgE_ *msg);
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x50]; s16 unk98; u8 p9A[2]; s16 unk9C; u8 p9E[0xA]; u16 unkA8; u8 pAA[0x92]; } VoiceC_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x83]; u8 unkCB; u8 unkCC; u8 pCD[0x6F]; } VoiceB_;
typedef struct { u8 p0[0x20]; s32 unk20; } ObjA_;
typedef struct { u8 p0[0x14]; s32 unk14; } ObjB_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x2C]; ObjA_ *unk74; ObjB_ *unk78; u8 p7C[0xC0]; } Voice9_;
typedef struct { u8 p0[0x3C]; s32 unk3C; s32 unk40; u8 p44[0x10]; s32 unk54; u8 p58[0x42]; u16 unk9A; u8 p9C[0x16]; u16 unkB2; u16 unkB4; u8 pB6[9]; u8 unkBF; u8 unkC0; u8 pC1[2]; u8 unkC3; u8 unkC4; u8 unkC5; } SeqK_;
typedef struct { u8 p0[0x20]; f32 unk20; u8 p24[0x92]; u8 unkB6; u8 pB7[0x1E]; u8 unkD5; u8 pD6[6]; f32 unkDC; } SeqJ_;
typedef struct { s32 unk0; u8 p4[0x2C]; f32 unk30; u8 p34[0x10]; s32 unk44; u8 p48[0x48]; f32 unk90; u8 p94[0xA8]; } Voice8_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x68]; s16 unkB0; u8 pB2[0xC]; u8 unkBE; u8 pBF[0x7D]; } Voice7_;
typedef struct { u8 p0[0x34]; s32 unk34; s32 unk38; u8 p3C[0x34]; f32 unk70; u8 p74[0x2E]; u16 unkA2; u16 unkA4; u8 pA6[0x16]; u8 unkBC; u8 pBD[0x1E]; u8 unkDB; u8 pDC[4]; u8 *unkE0[4]; s32 unkF0[4]; s32 unk100[4]; u16 unk110[4]; u16 unk118[4]; u8 unk120[4]; u8 unk124[4]; f32 unk128[4]; } SeqH_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x56]; s16 unk9E; u8 pA0[0x9C]; } Voice5_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0x34]; s32 unk7C; u8 p80[0xBC]; } Voice4_;
typedef struct { u8 p0[0x30]; f32 unk30; u8 p34[0x5C]; f32 unk90; } SeqG_;
typedef struct { s32 unk0; u8 p4[0x40]; s32 unk44; u8 p48[0xF4]; } Voice_;
extern s32 D_80127934;
typedef struct { u8 p0[0xBD]; u8 unkBD; } SeqF_;
typedef struct { u8 p0[0xBC]; u8 unkBC; } SeqE_;
typedef struct { u8 p0[0xB9]; u8 unkB9; } SeqD_;
typedef struct { u8 p0[0x1C]; u8 *unk1C; } Bank_;
typedef struct { u8 p0[0x74]; Bank_ *unk74; u8 p78[0xC]; u8 *unk84; } SeqC_;
typedef struct { s32 unk0; s32 unk4; } Entry_;
typedef struct { u8 p0[0xAC]; u16 unkAC; } SeqB_;
typedef struct { u8 p0[0xAE]; u16 unkAE; } SeqA_;
typedef struct { u8 p0[0xCE]; u8 unkCE; u8 unkCF; u8 pD0[8]; u8 unkD8; } SeqChan3_;
typedef struct { u8 p0[0xB2]; u16 unkB2; u16 unkB4; } SeqChan2_;
extern s32 D_80127964;
void func_800CE7E8(s32 arg);
typedef struct { u8 type; u8 pad[3]; s32 arg; } AudMsg_;
typedef struct { u8 p0[0x2C]; f32 unk2C; u8 p30[0x20]; f32 unk50; u8 p54[0x64]; u8 unkB8; } SeqChan_;
extern s32 D_800FAE80;
extern u8 D_800FAE84;
f32 sinf(f32);
s32 func_800D0444(s32, s32, s16, s16, s32);
typedef struct { s32 f0; s32 f4; s32 f8; s32 fC; u16 f10; u16 f12; s32 f14; s32 f18; s32 f1C; s32 f20; s32 f24; s32 f28; s32 f2C; s32 f30; s32 f34; s32 f38; s32 f3C; s32 f40; s32 f44; u8 p48[0xC]; s32 f54; s32 f58; s32 f5C; s32 f60; s32 f64; s32 f68; s32 f6C; u8 p70[0x1C]; s32 f8C; u8 p90[0x4]; s32 f94; s32 f98; u8 p9C[0xA0]; } S_D_8012793C;
typedef struct { s32 a; s32 b; } Ent89_;
typedef struct { s32 n; u8 p4[8]; s32 flags; s32 unk10; s32 unk14; Ent89_ e[1]; } Bank89_;
typedef struct { u8 p0[0x58]; f32 unk58; f32 unk5C; f32 unk60; s32 unk64; u8 p68[0x57]; u8 unkBF; u8 unkC0; u8 unkC1; u8 unkC2; u8 pC3[3]; u8 unkC6; u8 unkC7; u8 unkC8; } SeqE9_;
void func_800D15B8();
extern S_D_8012793C D_8012793C[];
extern u8 D_800FACF8;
extern s32 D_800FB170;
extern s32 D_800FB174;
extern s32 D_8014C6D0;
extern s32 D_80127960;
s32 func_800CFE2C();
extern s32 D_8012795C;

extern s16 D_8012794C;
extern s16 D_8012794E;
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

s32 func_800CE090(s32 arg0) {
    s32 temp_v0;
    s32 var_a0;

    var_a0 = D_8012793C->f30;
    if (var_a0 == 0) {
        var_a0 = D_8012793C->f2C;
        if (var_a0 == 0) {
            D_8012793C->f1C = 0;
            return 0;
        }
        goto block_4;
    }
    D_8012793C->f30 = 0;
block_4:
    if (D_8012793C->f1C == 0) {
        D_8012793C->f1C = (*(s32 *)((u8 *)(var_a0) + 0x10));
    }
    temp_v0 = func_800D0444(var_a0, arg0, 0x80, 0x80, -1);
    D_8012793C->f1C = 0;
    return temp_v0;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CE110);

void func_800CE228(s32 flags, s32 val) {
    s32 i;
    s32 rate;
    VoiceW_ *v;

    rate = val;
    if (val == 0) {
        rate = 1;
    }
    v = *(VoiceW_ **)&D_8012793C;
    for (i = 0; i < D_80127934; i++, v++) {
        if (!((v->unk78 != 0) ? (flags & 1) : (flags & 2))) {
            continue;
        }
        if (v->unk4 == 0) {
            continue;
        }
        if (v->unk10 != -1) {
            continue;
        }
        if (v->unk0 & 1) {
            v->unk1C = 1;
            v->unk10 = 0;
            v->unk0 &= ~1;
        } else {
            v->unk1C = rate;
            v->unk10 = val;
        }
    }
}

s32 func_800CE2F4(s32 flags) {
    s32 i;
    s32 n;
    VoiceW_ *v;

    n = 0;
    v = *(VoiceW_ **)&D_8012793C;
    for (i = 0; i < D_80127934; i++, v++) {
        if (v->unk4 == 0) {
            continue;
        }
        if ((v->unk78 != 0) ? (flags & 1) : (flags & 2)) {
            n++;
        }
    }
    return n;
}

s32 func_800CE374(s32 id, s32 val) {
    s32 i;
    s32 n;
    s32 rate;
    VoiceX_ *v;

    if (id == 0) {
        return 0;
    }
    rate = val;
    if (val == 0) {
        rate = 1;
    }
    i = 0;
    v = *(VoiceX_ **)&D_8012793C;
    for (n = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id && v->unk10 == -1) {
            if (v->unk0 & 1) {
                v->unk1C = 1;
                v->unk10 = 0;
                v->unk0 &= ~1;
            } else {
                v->unk1C = rate;
                v->unk10 = val;
            }
            n++;
        }
    }
    return n;
}

s32 func_800CE428(s32 id) {
    s32 i;
    Voice_ *v;
    s32 n;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = *(Voice_ **)&D_8012793C;
    for (n = 0; i < D_80127934; i++, v++) {
        n += (v->unk44 == id);
    }
    return n;
}

s32 func_800CE480(s32 id, s16 val) {
    s32 i;
    s32 n;
    Voice5_ *v;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = (*(Voice5_ * *)&D_8012793C);
    for (n = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            v->unk9E = val;
            n++;
        }
    }
    return n;
}

s32 func_800CE4E0(s32 id, s16 val) {
    s32 i;
    s32 n;
    Voice7_ *v;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = (*(Voice7_ * *)&D_8012793C);
    for (n = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            n++;
            v->unkB0 = val;
            v->unkBE = 0xFF;
        }
    }
    return n;
}


s32 func_800CE550(s32 id, f32 val) {
    s32 i;
    s32 n;
    Voice8_ *v;

    if (id == 0) {
        return 0;
    }
    i = 0;
    v = (*(Voice8_ * *)&D_8012793C);
    for (n = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            v->unk30 = val + v->unk90;
            n++;
        }
    }
    return n;
}


s32 func_800CE5C0(s32 id, s32 val) {
    s32 i;
    s32 n;
    VoiceC_ *v;

    if (id == 0) {
        return 0;
    }
    if (val <= 0) {
        val = 1;
    } else if (val > 256) {
        val = 256;
    }
    i = 0;
    v = (*(VoiceC_ * *)&D_8012793C);
    for (n = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            n++;
            v->unk98 = val;
            v->unk9C = (v->unkA8 * val) >> 7;
        }
    }
    return n;
}


s32 func_800CE64C(s32 id, s32 val) {
    s32 i;
    s32 n;
    VoiceB_ *v;

    if (id == 0) {
        return 0;
    }
    if (val < 0) {
        val = 0;
    } else if (val >= 128) {
        val = 127;
    }
    i = 0;
    v = (*(VoiceB_ * *)&D_8012793C);
    for (n = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            n++;
            v->unkCB = val;
            v->unkCC = 0xFF;
        }
    }
    return n;
}


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

s32 func_800CE768(s32 id) {
    s32 i;
    Voice4_ *v;

    if (id == 0) {
        return 0;
    }
    v = (*(Voice4_ * *)&D_8012793C);
    for (i = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            return v->unk7C;
        }
    }
    return 0;
}


void func_800CE7C4(s32 arg) {
    AudMsg_ msg;
    msg.type = 0;
    msg.arg = arg;
    func_800CEB34(&msg);
}


void func_800CE7E8(s32 arg) {
    AudMsg_ msg;
    msg.type = 1;
    msg.arg = arg;
    func_800CEB34(&msg);
}


void func_800CE810(s32 arg) {
    AudMsg_ msg;
    msg.type = 2;
    msg.arg = arg;
    D_80127964 = arg;
    func_800CEB34(&msg);
}


s32 func_800CE840(s32 a) {
    struct { u8 type; u8 p[3]; s32 val; } m;
    s32 r;

    r = 1;
    if (a == 0) {
        m.type = 2;
        m.val = D_80127964;
        D_80127964 = m.val;
        r = func_800CEB34((AudMsgE_ *)&m);
    }
    if (r != 0) {
        D_80127960 = a;
    }
    return r;
}

void func_800CE89C(u8 *base) {
    s32 i;
    Bank89_ *b;

    b = (Bank89_ *)base;
    if (b->flags & 1) {
        return;
    }
    if (D_80127968 == 0) {
        D_80127968 = (s32)base;
    }
    b->flags = 1;
    b->unk10 = 0;
    b->unk14 = (s32)base + b->unk14;
    for (i = 0; i < b->n; i++) {
        b->e[i].a = (s32)base + b->e[i].a;
    }
}


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

s32 func_800CE9CC(s32 id) {
    s32 i;
    Voice9_ *v;

    if (id == 0) {
        return 0;
    }
    v = (*(Voice9_ * *)&D_8012793C);
    for (i = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            if (v->unk74 != 0) {
                return v->unk74->unk20;
            }
            return v->unk78->unk14;
        }
    }
    return 0;
}


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

s32 func_800CEB34(AudMsgE_ *msg) {
    s32 next = (D_80127978 + 1) % D_8012797C;

    if (next == D_80127974) {
        return 0;
    }
    func_800D247C(&(*(AudMsgE_ * *)&D_80127980)[D_80127978], msg, 8);
    D_80127978 = next;
    return 1;
}


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

void func_800CF6E4(SeqK_ *s) {
    if (s->unk9A != 0x7FFF) {
        if (s->unkB2 != 0) {
            s->unk54 = s->unk40 + (s->unkB2 << 8);
        } else {
            s->unk54 = s->unk3C - (s->unkB4 << 8);
        }
    } else {
        s->unk54 = s->unk40 + 0x7FFFFFFF;
    }
    s->unkC3 = 1;
    s->unkC4 = s->unkC0;
    s->unkC5 = s->unkBF;
}


INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800CF754);

void func_800CF930(void *arg0) {
    *(u8 *)((u8 *)arg0 + 0xD9) = 0;
    *(s32 *)((u8 *)arg0 + 0x94) = (s32) *(s32 *)((u8 *)arg0 + 0x40);
    *(u8 *)((u8 *)arg0 + 0xDA) = (s8) (*(u8 *)((u8 *)arg0 + 0xBD) & 0x40);
}

void func_800CF94C(SeqO_ *s) {
    u32 t;

    do {
        s->unk94 += 0x100;
        t = s->unkD9 + s->unkD4;
        if (t < 0x40) {
            s->unkD9 = t;
        } else {
            s->unkD9 = t & 0x3F;
            t >>= 6;
            if (s->unkDA == 0) {
                s->unkBD += t;
                if (s->unkBD >= 0x80) {
                    s->unkBD = 0x7F;
                    s->unkDA = 1;
                }
            } else {
                s->unkBD -= t;
                if (s->unkBD >= 0x80 || s->unkBD == 0) {
                    s->unkBD = 0;
                    s->unkDA = 0;
                }
            }
        }
    } while (s->unk94 - s->unkC < 0);
}


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

f32 func_800CFA5C(s32 arg0) {
  f32 temp_fv0;
  s32 temp_v1;
  temp_v1 = (*((u16 *) (((u8 *) arg0) + 0xAA))) - (*((u8 *) (((u8 *) arg0) + 0xB6)));
  if (temp_v1 <= 0) {
    return 0.0f;
    return temp_fv0;
  }
  temp_fv0 = sinf(((f32) temp_v1) * (*((f32 *) (((u8 *) arg0) + 0xDC)))) * (*((f32 *) (((u8 *) arg0) + 0x20)));
  *((f32 *) (((u8 *) arg0) + 0x68)) = temp_fv0;
}

void func_800CFAC0(SeqP_ *s) {
    u8 b;
    u16 v;

    do {
        s->unk14 += 0x100;
        if (--s->unkA2 == 0) {
            b = *s->unk38++;
            if (b >= 0x80) {
                s->unkBC = b & 0x7F;
                b = *s->unk38++;
                if (b >= 0x80) {
                    v = (b & 0x7F) << 8;
                    s->unkA2 = v;
                    v += 2;
                    s->unkA2 = *s->unk38++ + v;
                } else {
                    s->unkA2 = b + 2;
                }
            } else {
                s->unkBC = b;
                s->unkA2 = 1;
            }
        }
    } while (s->unk14 - s->unkC < 0);
}

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

void func_800D035C(void *arg0, s32 arg1, s32 arg2) {
  s32 temp_v0;
  s32 var_v1;
  int new_var;
  void *var_a0;
  var_a0 = arg0;
  new_var = 0;
  var_v1 = 0;
  if (arg2 > new_var) {
    do
    {
      temp_v0 = *((s32 *) var_a0);
      var_v1 += 1;
      if (temp_v0 != 0) {
        *((s32 *) var_a0) = (s32) (temp_v0 + arg1);
      }
      var_a0 += 4;
    }
    while (var_v1 < arg2);
  }
}

s32 func_800D0394(VoiceG_ *v, BankG_ *b, s32 idx, s32 a3, s32 a4, s32 a5) {
    func_800D0058(v);
    v->unkA6 = idx;
    v->unk78 = b;
    v->unk9E = a3;
    v->unkB0 = a4;
    v->unk44 = D_80127950;
    v->unk48 = a5;
    D_80127950++;
    if (b->unk10 != 0) {
        v->unk7C = b->unk10;
    }
    v->unk4 = v->unk80 = b->entries[idx].a;
    return v->unk44;
}

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D0444);

INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D05EC);

void func_800D0858(s32 id, s32 mask, s32 bits) {
    s32 i;
    Voice_ *v = (*(Voice_ * *)&D_8012793C);

    for (i = 0; i < D_80127934; i++, v++) {
        if (v->unk44 == id) {
            v->unk0 = (v->unk0 & mask) | bits;
        }
    }
}


s32 func_800D08AC(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x38) = 0;
    *(s32 *)((u8 *)arg0 + 0x34) = 0;
    *(s32 *)((u8 *)arg0 + 0x74) = 0;
    *(s32 *)((u8 *)arg0 + 0x78) = 0;
    *(s32 *)((u8 *)arg0 + 0x44) = 0;
    *(s32 *)((u8 *)arg0 + 0x8) = 0;
    return 0;
}

u8 *func_800D08CC(SeqA_ *s, u8 *p) {
    s32 v = *p++;
    if (v & 0x80) {
        v = *p++ | ((v & 0x7F) << 8);
    }
    s->unkAE = v;
    return p;
}


u8 *func_800D08FC(SeqChan_ *s, u8 *p) {
    u8 v = *p++;
    s->unkB8 = v;
    if (v) {
        s->unk50 = s->unk2C;
    }
    return p;
}


s32 func_800D091C(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xB8) = 0;
    return arg1;
}

u8 *func_800D0928(SeqE9_ *s, u8 *p) {
    u8 n;
    u8 c;

    n = *p++;
    if (n == 0) {
        n = 1;
    }
    s->unkBF = n;
    s->unk64 = 1024 / n;
    s->unkC0 = *p++;
    n = *p++;
    s->unkC6 = n;
    c = *p++;
    s->unk58 = (1.0 / (f32)n) * (f32)(c - s->unkC0);
    s->unkC1 = c;
    n = *p++;
    s->unkC7 = n;
    c = *p++;
    s->unk5C = (1.0 / (f32)n) * (f32)(c - s->unkC1);
    s->unkC2 = c;
    n = *p++;
    s->unkC8 = n;
    s->unk60 = 1.0 / (f32)n;
    return p;
}

u8 *func_800D0A50(SeqZ_ *s, u8 *p) {
    s32 tempo = ((*p * 0x6000) / 120) / D_80127944;
    s32 scaled;
    s32 i;
    VoiceZ_ *v;

    p++;
    scaled = (tempo * s->unk98) >> 7;
    if (s->unk78 != 0) {
        s->unk9C = tempo;
    } else {
        v = (*(VoiceZ_ * *)&D_8012793C);
        for (i = 0; i < D_80127934; i++, v++) {
            if (v->unk74 == s->unk74) {
                v->unkA8 = tempo;
                v->unk9C = scaled;
            }
        }
    }
    return p;
}

void *func_800D0B34(s32 arg0, void *arg1) {
  s16 new_var;
  new_var = (s16) (*((u8 *) arg1));
  *((s16 *) (((u8 *) arg0) + 0xB2)) = 0;
  *((s16 *) (((u8 *) arg0) + 0xB4)) = new_var;
  return arg1 + 1;
}

u8 *func_800D0B48(SeqChan2_ *s, u8 *p) {
    s32 hi = *p++;
    s32 lo = *p++;
    s->unkB4 = 0;
    s->unkB2 = lo | (hi << 8);
    return p;
}


u8 *func_800D0B6C(SeqJ_ *s, u8 *p) {
    s->unkB6 = *p++;
    s->unkD5 = *p++;
    s->unk20 = (f32)*p / 50.0;
    s->unkDC = 6.2831852 / (f32)s->unkD5;
    return p + 1;
}

u8 *func_800D0BDC(SeqJ_ *s, u8 *p) {
    f32 a;
    f32 b;

    s->unkB6 = *p++;
    s->unkD5 = *p++;
    a = 6.2831852 / (f32)*(volatile u8 *)&s->unkD5;
    b = -(f32)*p++ / 50.0;
    s->unkDC = a;
    s->unk20 = b;
    return p;
}

s32 func_800D0C50(void *arg0, s32 arg1) {
    *(u8 *)((u8 *)arg0 + 0xD5) = 0;
    *(s32 *)((u8 *)arg0 + 0x68) = 0;
    return arg1;
}

u8 *func_800D0C60(SeqB_ *s, u8 *p) {
    s32 v = *p++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *p++;
    }
    s->unkAC = v;
    return p;
}


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

u8 *func_800D0CC0(SeqG_ *s, u8 *p) {
    s32 v = *p++;
    f32 f;

    if (v & 0x80) {
        v |= -0x100;
    }
    f = (f32)v / 100.0;
    s->unk30 = s->unk30 - s->unk90 + f;
    s->unk90 = f;
    return p;
}

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

u8 *func_800D0D50(SeqH_ *s, u8 *p) {
    s32 i = s->unkDB;

    s->unk120[i] = *p++;
    s->unkE0[i] = p;
    s->unkF0[i] = s->unk38;
    s->unk100[i] = s->unk34;
    s->unk124[i] = s->unkBC;
    s->unk128[i] = s->unk70;
    s->unk110[i] = s->unkA2;
    s->unk118[i] = s->unkA4;
    s->unkDB++;
    return p;
}

u8 *func_800D0DBC(SeqN_ *s, u8 *p) {
    s32 i = s->unkDB - 1;

    if (s->unk120[i] != 0xFF) {
        if (--s->unk120[i] == 0) {
            s->unkDB = i;
            i = -1;
        }
    }
    if (i >= 0) {
        p = s->unkE0[i];
        s->unk38 = s->unkF0[i];
        s->unk34 = s->unk100[i];
        s->unkBC = s->unk124[i];
        s->unk70 = s->unk128[i];
        s->unkA2 = s->unk110[i];
        s->unkA4 = s->unk118[i];
        s->unk24 = s->unk70 * s->unk6C;
    }
    return p;
}

u8 *func_800D0E58(SeqChan3_ *s, u8 *p) {
    s->unkD8 = *p++;
    s->unkCE = *p++;
    s->unkCF = *p;
    return p + 1;
}

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

void *func_800D0EA8(s32 arg0, void *arg1) {
  short new_var2;
  void *new_var;
  new_var = arg1;
  new_var2 = (u8) (*((u8 *) new_var));
  *((u8 *) (((u8 *) arg0) + 0xD2)) = 0;
  *((u8 *) (((u8 *) arg0) + 0xD3)) = new_var2;
  return arg1 + 1;
}

void *func_800D0EBC(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xBD) = (s8) ((u8) *arg1 >> 1);
    return arg1 + 1;
}

s32 func_800D0ED0(s32 arg0, s32 arg1) {
    return arg1 + 2;
}

u8 *func_800D0ED8(SeqC_ *s, u8 *p) {
    s32 v = *p++;
    if (v >= 0x80) {
        v &= 0x7F;
        v <<= 8;
        v |= *p++;
    }
    s->unk84 = s->unk74->unk1C + v * 6;
    return p;
}


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

u8 *func_800D0FB4(SeqD_ *s, u8 *p) {
    s->unkB9 = ((u8 (*)(u8))func_800CFFC4)(*p++);
    s->unkB9 += *p++;
    return p;
}

u8 *func_800D1004(SeqE_ *s, u8 *p) {
    s->unkBC = ((u8 (*)(u8))func_800CFFC4)(*p++);
    s->unkBC += *p++;
    return p;
}


u8 *func_800D1054(SeqF_ *s, u8 *p) {
    s->unkBD = ((u8 (*)(u8))func_800CFFC4)(*p++);
    s->unkBD += *p++;
    return p;
}


void *func_800D10A4(void *arg0, u8 *arg1) {
    *(u8 *)((u8 *)arg0 + 0xBC) = (u8) *arg1;
    return arg1 + 1;
}

u8 *func_800D10B4(SeqY_ *s, u8 *p) {
    s32 v = *p++;
    s32 id;
    s32 i;
    VoiceY_ *vc;

    if (v >= 0x80) {
        v = ((v & 0x7F) << 8) + *p++;
    }
    s->unk48++;
    id = ((s32 (*)(void *bank, s32 idx, s32 a, s32 b, s32 c))func_800D0444)(s->unk78, v, s->unk9E, s->unkB0, s->unk48);
    s->unk48--;
    if (id != 0) {
        vc = (*(VoiceY_ * *)&D_8012793C);
        for (i = 0; i < D_80127934; i++, vc++) {
            if (vc->unk44 == id) {
                vc->unk44 = s->unk44;
                vc->unk7C = ((SeqY2_ *)s)->unk7C;
            }
        }
    }
    return p;
}

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

s32 func_800D1568(s32 arg0) {
    s32 temp_s0;
    int new_var;

    temp_s0 = ((s32)(arg0 << 0x10)) >> 0xE;
    func_800D15B8((*((s32 *)(((u8 *)D_800FB174) + 0x34))) + (temp_s0 + (new_var = 0x24)));
    return *((s32 *)((temp_s0 + (*((s32 *)(((u8 *)D_800FB174) + 0x34)))) + 0x24));
}


s32 alHeapDBAlloc(s32, s32, void *, s32, s32);
s32 alFxParamHdl();
extern s32 D_8014C6E0;
extern s32 D_8014C6E4;
extern s32 D_8014C6E8;
extern s32 D_8014C6EC;
extern s32 D_8014C6F0;
extern s32 D_8014C6F4;

void func_800D15B8(s32 *rp, u8 *c, void *hp) {
    s32 r;

    r = alHeapDBAlloc(0, 0, hp, 1, 0x2C);
    *rp = r;
    *(void **)(r + 0x28) = alFxParamHdl;
    D_8014C6E0 = alHeapDBAlloc(0, 0, hp, func_800D19F8(), 0x28);
    D_8014C6E4 = alHeapDBAlloc(0, 0, hp, func_800D1A3C(), 2);
    D_8014C6E8 = alHeapDBAlloc(0, 0, hp, 1, 0x34);
    D_8014C6EC = alHeapDBAlloc(0, 0, hp, 1, 0x20);
    D_8014C6F0 = alHeapDBAlloc(0, 0, hp, 1, 0x30);
    D_8014C6F4 = alHeapDBAlloc(0, 0, hp, 1, 8);
    func_800D1700(*(s32 *)(c + 0x20));
}


INCLUDE_ASM("asm/nonmatchings/unit_6E610", func_800D1700);

void func_800D19E0(s32 arg0) {
    (*(u8 *)((u8 *)(arg0) + 0x1C)) = 6;
    (*(s32 *)((u8 *)(arg0) + 0x20)) = &D_800FACF8;
}

s32 func_800D19F8(void) {
    s32 i;
    s32 max = 0;

    for (i = 0; ((Entry_ * *)&D_800FAE84)[i] != 0; i++) {
        if (max < ((Entry_ * *)&D_800FAE84)[i]->unk0) {
            max = ((Entry_ * *)&D_800FAE84)[i]->unk0;
        }
    }
    return max;
}


s32 func_800D1A3C(void) {
    s32 i;
    s32 max = 0;

    for (i = 0; ((Entry_ * *)&D_800FAE84)[i] != 0; i++) {
        if (max < ((Entry_ * *)&D_800FAE84)[i]->unk4) {
            max = ((Entry_ * *)&D_800FAE84)[i]->unk4;
        }
    }
    return max;
}


s32 func_800D1A80(s32 arg0) {
  u8 *new_var;
  void *temp_a2;
  arg0 = arg0;
  if (arg0 < D_800FAE80) {
    func_800D1700(*((s32 *) (((u8 *) (&D_800FAE84)) + (arg0 * 4))));
    new_var = (u8 *) (*(void * *)&D_8014C6D0);
    temp_a2 = *((s32 *) (new_var + 0x34));
    alFxParam(*((s32 *) (((u8 *) temp_a2) + 0x20)), 1, (s32) temp_a2);
    return 0;
  }
  if (arg0) {
    return 1;
  }
  else
  {
    return 1;
  }
}
