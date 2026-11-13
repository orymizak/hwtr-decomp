#include "common.h"
#include "include_asm.h"
typedef struct { f32 x; f32 y; f32 z; } V3f6_;
typedef struct { s32 a; s32 b; s32 c; } V12_;
typedef struct { s32 a; s32 b; s32 c; s32 d; } V16_;
typedef struct { V16_ q; V12_ v; } E1C_;
typedef struct { u8 pad[0x10]; E1C_ *p; } E14_;
typedef struct { s32 idx; s32 a; s32 b; s32 c; } Rec16_;
typedef struct { f32 x; f32 y; f32 z; } Vec3f_;
typedef struct { Vec3f_ v; f32 s; u8 pad[2]; char name[0xE]; } Ent20_;
extern void *D_80138170;
void func_800734F4(void *);
void func_8006AA34(void);
void func_8006DC28(void);
extern char D_800FE9F0[];
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
s32 File_Load(char *, s32);
extern void *D_80101E84;
void Memory_Free(void *);
typedef struct { s32 a; s32 b; s32 c; s32 d; } Vec4L_;
typedef struct { u8 p0[0x13C]; Vec4L_ v; u8 unk14C; } ObjL_;
u8 func_80066A2C(u8 a, s32 b, s32 c);
typedef struct { u16 unk0; s16 x; s16 y; s16 z; s32 unk8; s32 unkC; s32 unk10; s32 unk14; } Node_;
typedef struct { f32 x; f32 y; f32 z; } Vec3_;
typedef struct { s32 x; s32 y; s32 z; } Pos_;
typedef struct { Pos_ p; f32 m[9]; s32 d; f32 e; } Cam_;
typedef struct { s32 d[0x114 / 4]; } Big114_;
typedef struct { s32 d[0x40 / 4]; } Big40_;
typedef struct { u8 p0[0xFE]; s8 id; u8 pFF; u8 cur; u8 tgt; u8 r; u8 g; u8 b; u8 b105; u8 p106[7]; u8 b10D; u8 b10E; } Lt65_;
typedef struct { s32 cur[6]; s32 tgt[6]; s32 dt; } Fade65_;
typedef struct { f32 x; f32 y; f32 z; } V3f69_;
typedef struct { f32 f0; s32 i4; f32 f8; f32 fC; u8 p10[0x30]; V3f69_ pos; V3f69_ vel; f32 f58; f32 f5C; u8 p60[4]; s16 h64; u8 b66; u8 c[3]; u8 b6A; u8 p6B; u16 s6C; s16 s6E; } Pt94_;
typedef struct { f32 x, y, z; } V3_9A4;
typedef struct { u8 p0[0xC]; f32 x; u8 p10[0xC]; f32 y; u8 p20[0xC]; f32 z; u8 p30[8]; f32 r; } O_9A4;
void func_8007E694(V3_9A4, V3_9A4);
void func_8007E974(V3_9A4, V3_9A4);
extern u8 D_80138A6C[];
extern s32 D_80138A90[];
extern u8 D_800F20F6;
void func_8007C318(void *, Pt94_ *);
extern Fade65_ D_80138A78;
extern u8 D_80138AAC;
extern u8 D_80148088;
extern u8 D_80134FC0;
extern u8 D_80145488;
void func_8007D14C(void *);
void func_8006DAC4(s32);
void func_800738C0(void *, s32);
void func_80075600(s32 arg0, s8 arg1, s8 arg2, s8 arg3);
void func_800756A0(s32);
void func_80075720(s32);
void func_800674DC(void);
s32 rand(void);
f32 sqrtf(f32);
void func_8007A858(u8 *, u8 *, s32 *, s32);
void func_8007DE54(Vec3f_, u32, u8 *, Vec3f_);
s32 File_GetSize(char *);
extern char D_800FE964[];
s32 func_800682A4(s32 unused, char *name, Big114_ *a, Big40_ *b);
extern u8 D_80138150[];
extern u8 D_80138156[];
extern f32 D_8013815C;
extern f32 D_80138160;
extern f32 D_80138164;
extern f32 D_80138168;
extern s32 D_80101E88[];
extern s32 D_800F0218[];
u8 File_PollAsync(s32);
s32 func_80073AC8(s32);
void func_80067A00(u8, u8, f32, f32, f32);
s32 Memory_Alloc();
extern u8 D_80101E80;

extern void *D_8013816C;
extern s32 D_8013818C;
s32 GfxPools_Shutdown();
s32 func_800741D8(s32);
extern s32 D_80138174;
s32 func_80066D80(s32);
s32 func_800709CC(s32 *);
extern s32 D_800F2138;

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065450);

s32 func_80065514(void) {
    if ((*(s32 *)((u8 *)&D_80101E80 + 0x0)) == 0) {
        (*(s32 *)((u8 *)&D_80101E80 + 0x0)) = Memory_Alloc(0x12C00);
    }
    if ((*(s32 *)((u8 *)&D_80101E80 + 0x4)) == 0) {
        (*(s32 *)((u8 *)&D_80101E80 + 0x4)) = Memory_Alloc(0x12C00);
    }
    return ((*(s32 *)((u8 *)&D_80101E80 + 0x0)) == 0) + ((*(s32 *)((u8 *)&D_80101E80 + 0x4)) == 0);
}

s32 func_80065584(void) {
    void **p = (void **)&D_80101E80;

    Memory_Free(*p);
    Memory_Free(D_80101E84);
    *p = NULL;
    D_80101E84 = NULL;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800655CC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006572C);

s32 func_8006586C(void) {
    void **p = &D_8013816C;

    func_800734F4(*p);
    func_800734F4(D_80138170);
    func_8006AA34();
    func_8006DC28();
    *p = NULL;
    D_80138170 = NULL;
    return 1;
}

u16 func_800658C4(void) {
    return *(u16 *)((u8 *)D_8013816C + 0x10);
}

void func_800658D4(s32 i, Vec3f_ *pos, s32 *a, s32 *b, s32 *c) {
    Rec16_ *r = *(Rec16_ **)((*(u8 **)&D_8013816C) + 0x4C) + i;
    f32 *m = (f32 *)(*(u8 **)((*(u8 **)&D_8013816C) + 0x2C) + r->idx * 0x54);

    pos->x = m[3];
    pos->y = m[7];
    pos->z = m[11];
    *a = r->a;
    *b = r->b;
    *c = r->c;
}


void func_80065940(s32 dt) {
    u32 i;
    u32 k;
    Lt65_ *o;
    f32 c;
    f32 t;

    D_80138A78.dt = dt;
    if (dt != 0) {
        func_8007D14C(&D_80148088);
        func_8007D14C(&D_80134FC0);
        func_8007D14C(&D_80145488);
        func_8006DAC4(dt);
        func_800738C0(D_8013816C, dt);
        for (i = 0; i < 6; i++) {
            o = ((Lt65_ **)&D_80138174)[i];
            if (o == NULL) {
                continue;
            }
            func_800738C0(o, dt);
            if (o->b105 < 5) {
                o->b105 = 0;
            } else {
                o->b105 -= 5;
            }
            for (k = 0; k < 12; k++) {
                if (o->b10D < o->b10E) {
                    o->b10D++;
                } else if (o->b10E < o->b10D) {
                    o->b10D--;
                } else {
                    break;
                }
            }
            if (D_80138A78.cur[i] < D_80138A78.tgt[i]) {
                D_80138A78.cur[i] += 15;
                if (D_80138A78.cur[i] > D_80138A78.tgt[i]) {
                    D_80138A78.cur[i] = D_80138A78.tgt[i];
                }
            }
            if (D_80138A78.cur[i] > D_80138A78.tgt[i]) {
                D_80138A78.cur[i] -= 15;
                if (D_80138A78.cur[i] < D_80138A78.tgt[i]) {
                    D_80138A78.cur[i] = D_80138A78.tgt[i];
                }
            }
            if (o->cur != o->tgt) {
                c = o->cur / 255.0f;
                t = o->tgt / 255.0f;
                if (c < t) {
                    c += 0.05f;
                    if (t < c) {
                        c = t;
                    }
                } else {
                    c -= 0.05f;
                    if (c < t) {
                        c = t;
                    }
                }
                o->cur = c * 255.0f;
                if (c != 0.0f) {
                    func_80075600(o->id, c * o->r, c * o->g, c * o->b);
                    func_800756A0(o->id);
                } else {
                    func_80075720(o->id);
                }
            }
        }
        func_800674DC();
        D_80138AAC = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065C5C);

void func_800664A4(s32 arg0) {
    D_8013818C = arg0;
}

void func_800664B0(void) {
    s32 *var_s0;
    s32 temp_a0;
    u32 var_s1;

    GfxPools_Shutdown();
    var_s1 = 0;
    var_s0 = &D_80138174;
    do {
        temp_a0 = *var_s0;
        var_s1 += 1;
        if (temp_a0 != 0) {
            func_800741D8(temp_a0);
        }
        var_s0 += 1;
    } while (var_s1 < 6U);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066504);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665A4);

void func_800665D0(u32 arg0, void *arg1, void *arg2, void *arg3) {
  u32 temp_a0;
  s8 new_var;
 do { temp_a0 = arg0 & 0xFFFF; } while (0);
  *((u8 *) arg1) = (s8) (temp_a0 >> 0xB);
  new_var = (s8) ((temp_a0 >> 6) & 0x1F);
  *((u8 *) arg2) = new_var;
  *((u8 *) arg3) = (s8) ((temp_a0 >> 1) & 0x1F);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800665F8);

void func_8006673C(s32 arg0) {
    (*(u8 *)((u8 *)((*(s32 *)(&(*(u8 *)&D_80138174) + ((arg0 & 0xFF) * 4)))) + 0x105)) = 0;
}

void func_80066758(s32 arg0) {
    (*(u8 *)((u8 *)((*(s32 *)(&(*(u8 *)&D_80138174) + ((arg0 & 0xFF) * 4)))) + 0x105)) = 0xFF;
}

void func_80066778(s32 arg0, s8 arg1) {
    void *temp_v0;

    temp_v0 = *(s32 *)(&(*(u8 *)&D_80138174) + ((arg0 & 0xFF) * 4));
    if (temp_v0 != NULL) {
        (*(u8 *)((u8 *)((*(s32 *)((u8 *)((*(s32 *)((u8 *)(temp_v0) + 0x2C))) + 0x3C))) + 0x9)) = arg1;
    }
}

void func_800667A8(s32 arg0, s32 arg1) {
    void *temp_v0;
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = *(s32 *)(&(*(u8 *)&D_80138174) + ((arg0 & 0xFF) * 4));
    if (temp_v1 != NULL) {
        if (arg1 & 0xFF) {
            temp_v1_2 = (*(s32 *)((u8 *)(temp_v1) + 0x2C));
            (*(s32 *)((u8 *)(temp_v1_2) + 0x30)) = (s32) ((*(s32 *)((u8 *)(temp_v1_2) + 0x30)) | 0x40);
            return;
        }
        temp_v0 = (*(s32 *)((u8 *)(temp_v1) + 0x2C));
        (*(s32 *)((u8 *)(temp_v0) + 0x30)) = (s32) ((*(s32 *)((u8 *)(temp_v0) + 0x30)) & ~0x40);
    }
}

u8 func_800667F8(u8 a, s32 b) {
    return func_80066A2C(a, b, 0);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006681C);

s32 func_8006695C(u8 i) {
    s32 j;

    if (File_PollAsync(D_80101E88[i]) == 0) {
        return 0;
    }
    D_800F0218[i] = 0;
    (&D_80138174)[i] = func_80073AC8(((s32 *)&D_80101E80)[i]);
    for (j = 0; j < ((u8 *)(&D_80138174)[i])[0xFC]; j++) {
        func_80067A00(i, j, 0.0f, 0.0f, 0.0f);
    }
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066A2C);

s32 func_80066B94(u32 arg0) {
  s32 *temp_s0;
  ;
  func_800741D8((void *) (*(&((s32 *)&D_80138174)[arg0 & 0xFF])));
  *(&((s32 *)&D_80138174)[arg0 & 0xFF]) = 0;
  return 1;
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066BD4);

void func_80066C64(s32 arg0) {
    func_800709CC(&D_800F2138);
    func_80066D80(arg0 & 0xFF);
}

void func_80066C9C(s32 a, s32 b) {
    u8 y = 1;
    u8 x = 24;
    f32 *p;
    s32 i;

    if ((a == 0) & (b == 0)) {
        x = 0;
        y = 0;
    }
    for (i = 0; i < 6; i++) {
        D_80138150[i] = x;
        D_80138156[i] = y;
    }
    p = &D_8013815C;
    *p = 700.0f;
    D_80138160 = 1400.0f;
    D_80138164 = 2800.0f;
    D_80138168 = 4800.0f;
    if (a != 1) {
        *p = 490.0f;
        D_80138160 = 980.0f;
        D_80138164 = 1960.0f;
        D_80138168 = 3360.0f;
    } else {
        D_80138156[0] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066D80);

void func_80066ECC(s32 arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    void *temp_v1;

    temp_v1 = *(s32 *)(&(*(u8 *)&D_80138174) + ((arg0 & 0xFF) * 4));
    *arg1 = (*(u8 *)((u8 *)(temp_v1) + 0x14D));
    *arg2 = (*(u8 *)((u8 *)(temp_v1) + 0x14E));
    *arg3 = (*(u8 *)((u8 *)(temp_v1) + 0x14F));
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066EFC);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067284);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067310);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800674DC);

void func_80067950(u8 i, f32 v) {
    u32 j;

    for (j = 0; j < ((u8 *)(&D_80138174)[i])[0xFC]; j++) {
        func_80067A00(i, j, 0.0f, 0.0f, v);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067A00);

void func_80067EFC(u8 i, u8 val, Vec4L_ *src) {
    ((ObjL_ * *)&D_80138174)[i]->unk14C = val;
    ((ObjL_ * *)&D_80138174)[i]->v = *src;
}


void func_80067F40(u8 i, f32 v) {
    ((u8 **)&D_80138174)[i][0x10E] = v * 240.0f;
}


INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80067FA4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068020);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006810C);

s32 func_80068270(s32 arg0, s32 arg1, s32 arg2) {
    return func_800682A4(0, arg0, arg1, arg2) & 0xFF;
}

s32 func_800682A4(s32 unused, char *name, Big114_ *a, Big40_ *b) {
    char path[32];
    u8 *buf;

    strcpy(path, name);
    strcat(path, D_800FE964);
    buf = (u8 *)Memory_Alloc(File_GetSize(path));
    File_Load(path, (s32)buf);
    *a = *(Big114_ *)(buf + 4);
    *b = *(Big40_ *)(buf + 0x118);
    Memory_Free(buf);
    return 1;
}


INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068390);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068504);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800685E0);

u16 func_800687D8(void) {
    return *(u16 *)((u8 *)D_8013816C + 0x6);
}

void func_800687E8(s32 i, f32 *pos, f32 *rot, f32 *a3, s32 *a4, s32 *a5, s32 *a6, s32 *a7) {
    Node_ *n = (Node_ *)*(u8 **)((*(u8 * *)&D_8013816C) + 0x38) + i;
    f32 *m = (f32 *)(*(u8 **)((*(u8 * *)&D_8013816C) + 0x2C) + n->unk0 * 0x54);

    rot[0] = m[0];
    rot[1] = m[1];
    rot[2] = m[2];
    rot[3] = m[4];
    rot[4] = m[5];
    rot[5] = m[6];
    rot[6] = m[8];
    rot[7] = m[9];
    rot[8] = m[10];
    pos[0] = m[3];
    pos[1] = m[7];
    pos[2] = m[11];
    a3[0] = n->x;
    a3[1] = n->y;
    a3[2] = n->z;
    *a4 = n->unkC;
    *a5 = n->unk8;
    *a6 = n->unk10;
    *a7 = n->unk14;
}

void func_800688F4(s32 idx, Vec3_ pos, f32 *m) {
    f32 *q = (f32 *)(*(u8 **)((*(u8 * *)&D_8013816C) + 0x2C) + *(u16 *)(*(u8 **)((*(u8 * *)&D_8013816C) + 0x38) + idx * 0x18) * 0x54);

    q[0] = m[0];
    q[1] = m[1];
    q[2] = m[2];
    q[4] = m[3];
    q[5] = m[4];
    q[6] = m[5];
    q[8] = m[6];
    q[9] = m[7];
    q[10] = m[8];
    q[3] = pos.x;
    q[7] = pos.y;
    q[11] = pos.z;
}

void func_800689A4(O_9A4 *o, V3_9A4 v) {
    V3_9A4 d;
    V3_9A4 p;
    u32 s;
    f32 len;
    u32 i, n;

    if (o == NULL) {
        return;
    }
    s = o->r / 2.5f;
    v.x /= 14.0f;
    v.y /= 14.0f;
    v.z /= 14.0f;
    len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    n = (rand() & 7) + 12;
    for (i = 0; i < n; i++) {
        p.x = o->x;
        p.y = o->y;
        p.z = o->z;
        p.x += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.y += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.z += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        d.x = v.x + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        d.y = v.y + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        d.z = v.z + ((f32)(rand() & 0x7F) - 64.0f) / 64.0f * len;
        func_8007E694(p, d);
    }
    n = (rand() & 7) + 12;
    v.x /= 3.0f;
    v.y /= 3.0f;
    v.z /= 3.0f;
    for (i = 0; i < n; i++) {
        p.x = o->x;
        p.y = o->y;
        p.z = o->z;
        p.x += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.y += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        p.z += (f32)((rand() & 0x7F) - 64) / 128.0f * (f32)s;
        d.x = v.x + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        d.y = v.y + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        d.z = v.z + ((f32)(rand() & 0x7F) - 64.0f) / 127.0f * len;
        func_8007E974(p, d);
    }
}

void func_80068E98(u8 *o, Vec3f_ vel, u8 *verts) {
    Vec3f_ v;
    Vec3f_ p;
    s32 idx[16];
    u8 *m;
    u32 n;
    u32 sz;
    u32 i;
    f32 sp;

    if (o == NULL) {
        return;
    }
    m = *(u8 **)(o + 0x3C);
    if (m != NULL) {
        n = *(u16 *)(m + 2) >> 1;
        if (n > 16) {
            n = 16;
        }
        if (n < 6) {
            n = 6;
        }
    } else {
        n = (rand() & 7) + 7;
    }
    func_8007A858(m, verts, idx, 16);
    sz = *(f32 *)(o + 0x38) / 2.5f;
    vel.x /= 14.0f;
    vel.y /= 14.0f;
    vel.z /= 14.0f;
    sp = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    for (i = 0; i < n; i++) {
        p.x = *(f32 *)(o + 0xC);
        p.y = *(f32 *)(o + 0x1C);
        p.z = *(f32 *)(o + 0x2C);
        p.x += ((rand() & 0x7F) - 64) / 128.0f * sz;
        p.y += ((rand() & 0x7F) - 64) / 128.0f * sz;
        p.z += ((rand() & 0x7F) - 64) / 128.0f * sz;
        v.x = vel.x + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        v.y = vel.y + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        v.z = vel.z + ((rand() & 0x7F) - 64.0f) / 127.0f * sp;
        func_8007DE54(p, sz, verts + idx[rand() & 0xF] * 8, v);
    }
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80069214);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006936C);

void func_80069494(u8 **car, f32 speed, V3f69_ pos, V3f69_ vel) {
    Pt94_ pt;
    u8 *o;
    f32 r;

    if (car == NULL || *car == NULL || *(s32 *)(*car + 0x5C) != 3) {
        return;
    }
    o = ((u8 **)&D_80138174)[**(u8 **)(*car + 0x64)];
    if (o == NULL) {
        return;
    }
    if (speed < 200.0f) {
        return;
    }
    pt.pos = pos;
    pt.vel = vel;
    pt.vel.x /= 15.0f;
    pt.vel.y /= 15.0f;
    pt.vel.z /= 15.0f;
    pt.vel.x += rand() & 3;
    pt.vel.y += rand() & 3;
    pt.vel.z += (rand() & 3) + 20;
    pt.b66 = 0x41;
    pt.i4 = 0;
    pt.f58 = -2.0f;
    pt.f0 = 20.0f;
    pt.f8 = rand();
    pt.fC = ((rand() & 0xFF) - 128.0f) / 1000.0f;
    pt.h64 = 1000;
    pt.b6A = 0;
    pt.s6C = 0xFF00;
    pt.s6E = 0;
    r = (rand() % 21) / 100.0;
    pt.c[2] = (u8)((0.8 - r) * o[0x14F]) >> D_800F20F6;
    pt.c[1] = (u8)((0.8 - r) * o[0x14E]) >> D_800F20F6;
    pt.c[0] = (u8)((0.8 - r) * o[0x14D]) >> D_800F20F6;
    if (D_80138A6C[**(u8 **)(*car + 0x64)] == 1) {
        pt.c[0] >>= 4;
        pt.c[2] >>= 4;
        pt.c[1] >>= 4;
    }
    if (D_80138A90[**(u8 **)(*car + 0x64)] != 100) {
        pt.c[2] >>= 2;
        pt.c[1] >>= 2;
        pt.c[0] >>= 2;
    }
    pt.f5C = 100.0f;
    func_8007C318(&D_80145488, &pt);
}

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800698C0);

u16 func_8006A1E0(void) {
    return *(u16 *)((u8 *)D_8013816C + 0xC);
}

void func_8006A1F0(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    void *temp_v1;

    temp_v1 = *(s32 *)((u8 *)D_8013816C + 0x44) + (arg0 * 0x14);
    *arg1 = *(s32 *)((u8 *)temp_v1 + 0x0);
    *arg2 = (s32) *(u16 *)((u8 *)temp_v1 + 0x6);
    *arg3 = *(s32 *)((u8 *)temp_v1 + 0x8);
    *arg4 = *(s32 *)((u8 *)temp_v1 + 0xC);
}

void func_8006A234(s32 a, s32 b, V12_ *v, V16_ *q) {
    *v = (*(E14_ **)((u8 *)D_8013816C + 0x44))[a].p[b].v;
    *q = (*(E14_ **)((u8 *)D_8013816C + 0x44))[a].p[b].q;
}

void func_8006A2B8(s32 i, V3f6_ *t, f32 *m) {
    f32 *d = (f32 *)(*(u8 **)((u8 *)D_8013816C + 0x2C) +
                     *(u16 *)(*(u8 **)((u8 *)D_8013816C + 0x44) + i * 0x14 + 4) * 0x54);

    d[0] = m[0];
    d[1] = m[1];
    d[2] = m[2];
    d[3] = t->x;
    d[4] = m[3];
    d[5] = m[4];
    d[6] = m[5];
    d[7] = t->y;
    d[8] = m[6];
    d[9] = m[7];
    d[10] = m[8];
    d[11] = t->z;
}

u8 func_8006A358(void) {
    return *(u8 *)((u8 *)D_8013816C + 0xB);
}

void func_8006A368(u8 i, Pos_ *a1, f32 *a2, s32 *a3, f32 *a4) {
    Cam_ *c = (Cam_ *)*(u8 **)((*(u8 * *)&D_8013816C) + 0x40) + i;

    *a1 = c->p;
    *a3 = c->d;
    *a4 = c->e;
    a2[0] = c->m[0];
    a2[1] = c->m[1];
    a2[2] = c->m[2];
    a2[3] = c->m[3];
    a2[4] = c->m[4];
    a2[5] = c->m[5];
    a2[6] = c->m[6];
    a2[7] = c->m[7];
    a2[8] = c->m[8];
}

s32 func_8006A400(char *name, s32 dst) {
    char buf[64];

    strcpy(buf, name);
    strcat(buf, D_800FE9F0);
    File_Load(buf, dst);
    return 1;
}

u16 func_8006A450(void) {
    return *(u16 *)((u8 *)D_8013816C + 0xE);
}

void func_8006A460(s32 i, Vec3f_ *v, f32 *s, char *name) {
    *v = (*(Ent20_ **)((u8 *)D_8013816C + 0x48))[i].v;
    *s = (*(Ent20_ **)((u8 *)D_8013816C + 0x48))[i].s;
    strcpy(name, (*(Ent20_ **)((u8 *)D_8013816C + 0x48))[i].name);
}

void func_8006A4D4(s32 arg0, s8 arg1) {
    s32 temp_a0;

    temp_a0 = (*(s32 *)((u8 *)(D_8013816C) + 0x2C)) + ((*(u16 *)((u8 *)(((arg0 << 5) + (*(s32 *)((u8 *)(D_8013816C) + 0x48)))) + 0x10)) * 0x54);
    if (!(arg1 & 0xFF)) {
        func_80072D48(temp_a0);
        return;
    }
    func_80072D5C(temp_a0);
}
