#include "common.h"
#include "include_asm.h"
typedef struct { u8 b[0x10]; } ClientW_;
typedef struct { u8 b[0x10]; } ClientR_;
typedef struct { u8 p0[0xC]; void *fb; } FrameR_;
s32 osCreateThread(void *,s32, void *,s32, void *, s32);
s32 osStartThread(void *);
extern u8 D_8012DBE0;
extern u8 D_8012FD90;
void func_800D5E00(void *arg);

extern s32 D_8014D304;
extern s32 D_8014D33C;
s32 osSetIntMask(s32);
extern s8 D_8014D97D;
extern u8 D_8014D97C;

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5560);

s32 *func_800D57AC(void) {
    return &D_8014D304;
}

s32 *func_800D57B8(void) {
    return &D_8014D33C;
}

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D57C4);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5910);

void func_800D59A0(void *arg0, s16 arg1) {
    s32 temp_a0;

    temp_a0 = osSetIntMask(1);
    *(s16 *)((u8 *)arg0 + 0x8) = arg1;
    osSetIntMask(temp_a0);
}

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D59E0);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5A5C);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5ABC);

INCLUDE_ASM("asm/nonmatchings/unit_76160", func_800D5C18);

void *osViGetCurrentFramebuffer(void);
void *osViGetNextFramebuffer(void);
void func_800D5910(ClientR_ *c, void *mq, s32 m);
void func_800D59E0(ClientR_ *c);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
void func_800D5D5C(FrameR_ *f) {
    u8 *base = &D_8014D97D;
    void *fb = f->fb;
    u8 *mq;
    ClientR_ c;

    if (*base == 1) {
        return;
    }
    mq = base - 0x561;
    while (osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == fb) {
        func_800D5910(&c, mq, 1);
        osRecvMesg(mq, 0, 1);
        func_800D59E0(&c);
    }
}

void func_800D5DE8(s8 arg0) {
    D_8014D97D = arg0;
}

u8 func_800D5DF4(void) {
    return D_8014D97C;
}

extern u8 D_80141598[];
extern void *D_8012DBC0[];
extern s32 D_80145484;
extern void (*D_800FB190)(s32);
extern void (*D_800FB194)(void);
void osCreateMesgQueue(void *mq, void **msg, s32 count);
void func_800D5E00(void *arg) {
    ClientW_ c;
    s16 *msg;

    osCreateMesgQueue(D_80141598, D_8012DBC0, 8);
    func_800D5910(&c, D_80141598, 3);
    while (1) {
        osRecvMesg(D_80141598, (void **)&msg, 1);
        switch (*msg) {
            case 1:
                if (D_800FB190 != 0) {
                    D_800FB190(D_80145484);
                }
                break;
            case 2:
                if (D_800FB194 != 0) {
                    D_800FB194();
                }
                break;
        }
    }
}

void func_800D5ED0(void) {
    osCreateThread(&D_8012DBE0, 4, &func_800D5E00, 0, &D_8012FD90, 0x32);
    osStartThread(&D_8012DBE0);
}
