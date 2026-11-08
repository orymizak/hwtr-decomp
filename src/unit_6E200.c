#include "common.h"
#include "include_asm.h"
typedef struct { u8 p0[0x20]; void *ucode; u8 p24[4]; void *ucode_data; u8 p2C[0x14]; void *data_ptr; u32 data_size; } ScTaskI_;
extern ScTaskI_ D_80138AB8;
extern u8 D_8014D304[];
extern u8 D_800FABA1;
extern void (*D_800FABA4)(s32, u8);
s32 osSendMesg(void *mq, void *msg, s32 flag);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
s32 func_800CDB5C(s32);
s32 func_800CE95C(void *);
extern u8 D_800E7AA0;
extern u8 D_800E7B70;
extern s8 D_800FABA0;
extern u8 D_800FABA8;
extern s32 D_80138AC8;
extern void *D_80138AD0;
extern s32 D_80138AD4;
extern s32 D_80138AE4;
extern s32 D_80138AE8;
extern s32 D_80138AEC;
extern s32 D_80138AF0;
extern s32 D_80138AF4;
extern s32 D_80138B00;
extern s32 D_80138B04;
extern void *D_80138B08;
extern s32 D_80138B0C;
s32 func_800D5910(void *, void *,s32);
s32 osCreateMesgQueue(void *, void *,s32);
extern u8 D_80127900;
extern u8 D_80127910;
extern u8 D_80134780;
extern u8 D_80144450;
extern u8 D_80145528;



void func_800CD600(s32 arg0) {
  u8 *new_var;
  D_800FABA0 = 1;
  D_80138B08 = &D_80134780;
  D_80138AC8 = 2;
  new_var = &D_800E7AA0;
  D_80138AD4 = ((u8 *) (&D_800E7B70)) - new_var;
  D_80138B0C = 0;
  D_80138AD0 = new_var;
  D_80138AE4 = 0x800;
  D_80138AE8 = 0;
  D_80138AEC = 0;
  D_80138AF0 = 0;
  D_80138AF4 = 0;
  D_80138B00 = 0;
  D_80138B04 = 0;
  func_800CE95C(&D_800FABA8);
  func_800CDB5C(arg0);
}

void func_800CD6C4(void) {
    osCreateMesgQueue(&D_80145528, &D_80127900, 4);
    osCreateMesgQueue(&D_80134780, &D_80127910, 1);
    func_800D5910(&D_80144450, &D_80145528, 3);
}

INCLUDE_ASM("asm/nonmatchings/unit_6E200", func_800CD728);

void func_800CD7B8(u32 *a) {
    if ((*(u8 *)&D_800FABA0) == 1) {
        D_80138AB8.data_ptr = (void *)a[0];
        D_80138AB8.data_size = a[1];
        D_80138AB8.ucode = (void *)a[2];
        D_80138AB8.ucode_data = (void *)a[3];
        osSendMesg(D_8014D304, &D_80138AB8, 1);
        osRecvMesg(((u8 *)&D_80134780), 0, 1);
    }
    if (D_800FABA4 != 0 && D_800FABA1 != 0) {
        D_800FABA4(1, D_800FABA1);
        D_800FABA1++;
    }
}
