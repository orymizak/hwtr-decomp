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
void func_80067A00(u8, u8, f32, f32, f32);
s32 Memory_Alloc();
extern u8 D_80101E80;
s32 func_800682A4(s32, s32,s32,s32);

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


INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80065940);

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

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006695C);

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

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80066C9C);

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

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800682A4);

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

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_800689A4);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80068E98);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80069214);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_8006936C);

INCLUDE_ASM("asm/nonmatchings/unit_06050", func_80069494);

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
