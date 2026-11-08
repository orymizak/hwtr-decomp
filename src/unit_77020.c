#include "common.h"
#include "include_asm.h"
typedef struct { void *pfs; s32 file_no; } PakFileS_;
typedef struct { PakFileS_ *file; u8 *name; u8 *ext; s32 size; s32 create; } ArgsS_;
typedef struct { u8 p0[0xC]; ArgsS_ *args; } ReqS_;
typedef struct { void *pfs; s32 file_no; } PakFileB_;
typedef struct { PakFileB_ *file; s32 offset; s32 size; u8 flag; u8 *buffer; } ReqArgB_;
typedef struct { u8 p0[0xC]; ReqArgB_ *args; } ReqB_;
s32 osPfsRepairId();
s32 osPfsNumFiles();
s32 osPfsDeleteFile();
s32 bcopy(void *, s32,s32);
s32 func_800D7664();
s32 func_800D768C();
extern u8 D_80141090;

s32 func_800D7450(s32 *);
extern s32 D_800FB1E0;
s32 func_800D74F0(s32 *);
s32 osPfsInitPak(s32 *, void *, s32);
extern s32 D_8014EB40;
s32 osPfsFreeBlocks(s32, void *);
s32 osPfsReadWriteFile(s32, s32, u8, s32, s32, s32);
s32 osPfsAllocateFile(s32, u16, s32, s32, s32, s32, s32);
s32 osPfsFindFile(s32, u16, s32, s32, s32, s32);
extern u16 D_800FB1B0;
extern s32 D_800FB1B4;
s32 osPfsFileState(s32, s32, s32);

void func_800D6420(s32 arg0) {
    func_800D7664();
    bcopy(&D_80141090, arg0, 0x18);
    func_800D768C();
}

void func_800D6460(void) {
    func_800D7450(&D_800FB1E0);
}

void func_800D6480(void) {
    func_800D74F0(&D_800FB1E0);
}

s32 func_800D64A0(void *arg0) {
    s32 temp_v0;
    void *temp_a1;
    void *temp_s0;

    temp_s0 = *(s32 *)((u8 *)arg0 + 0xC);
    temp_a1 = *(s32 *)((u8 *)temp_s0 + 0x0);
    *(s32 *)((u8 *)temp_s0 + 0xC) = 0;
    temp_v0 = osPfsInitPak(&D_8014EB40, temp_a1, *(s32 *)((u8 *)temp_a1 + 0x8));
    if (temp_v0 == 0) {
        *(s32 *)((u8 *)temp_s0 + 0xC) = 1;
    }
    return temp_v0;
}

void func_800D64F0(s32 arg0) {
  void ***temp_a1;
  temp_a1 = *((s32 *) (((u8 *) arg0) + 0xC));
  osPfsFreeBlocks(*(*temp_a1), (*((s32 *) (((u8 *) arg0) + 0xC))) + 4);
}

s32 func_800D6518(ReqB_ *r) {
    ReqArgB_ *a = r->args;
    return ((s32 (*)(void *pfs, s32 file_no, u8 flag, s32 offset, s32 size, u8 *data_buffer))osPfsReadWriteFile)(a->file->pfs, a->file->file_no, a->flag, a->offset, a->size, a->buffer);
}


s32 func_800D6558(ReqS_ *r) {
    ArgsS_ *a = r->args;
    PakFileS_ *f = a->file;
    s32 ret;

    ret = osPfsFindFile(f->pfs, D_800FB1B0, D_800FB1B4, a->name, a->ext, &f->file_no);
    if (ret == 5 && a->create == 1) {
        ret = osPfsAllocateFile(f->pfs, D_800FB1B0, D_800FB1B4, a->name, a->ext, a->size, &f->file_no);
    }
    return ret;
}

void func_800D6610(void *arg0) {
    void *temp_v0;

    temp_v0 = (*(s32 *)((u8 *)(arg0) + 0xC));
    osPfsDeleteFile(*(s32 *)(*(s32 *)((u8 *)(temp_v0) + 0x0)), D_800FB1B0, D_800FB1B4, (*(s32 *)((u8 *)(temp_v0) + 0x4)), (*(s32 *)((u8 *)(temp_v0) + 0x8)));
}

void func_800D6650(void *arg0) {
    void *temp_v0;
    void *temp_v1;

    temp_v1 = *(s32 *)((u8 *)arg0 + 0xC);
    temp_v0 = *(s32 *)((u8 *)temp_v1 + 0x0);
    osPfsFileState(*(s32 *)((u8 *)temp_v0 + 0x0), *(s32 *)((u8 *)temp_v0 + 0x4), *(s32 *)((u8 *)temp_v1 + 0x4));
}

void func_800D667C(void *arg0) {
    void *temp_v0;

    temp_v0 = (*(s32 *)((u8 *)(arg0) + 0xC));
    osPfsNumFiles(*(s32 *)(*(s32 *)((u8 *)(temp_v0) + 0x0)), (*(s32 *)((u8 *)(temp_v0) + 0x4)), (*(s32 *)((u8 *)(temp_v0) + 0x8)));
}

void func_800D66A8(void *arg0) {
    osPfsRepairId(*(s32 *)(*(s32 *)((u8 *)(arg0) + 0xC)));
}
