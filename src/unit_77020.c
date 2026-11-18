#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
/* nusys Controller Pak manager types (NUSiCommonMesg / NUContPakFile). */
typedef struct { s32 status; void *queue; s32 channel; } Pfs7_;
typedef struct { Pfs7_ *pfs; s32 file_no; s32 error; s32 type; } NuPakFile_;
typedef struct { u8 p0[0xC]; void *data; } NuSiMesg_;
typedef struct { void *pfs; s32 file_no; } PakFileS_;
typedef struct { PakFileS_ *file; u8 *name; u8 *ext; s32 size; s32 create; } ArgsS_;
typedef struct { u8 p0[0xC]; ArgsS_ *args; } ReqS_;
typedef struct { void *pfs; s32 file_no; } PakFileB_;
typedef struct { PakFileB_ *file; s32 offset; s32 size; u8 flag; u8 *buffer; } ReqArgB_;
typedef struct { u8 p0[0xC]; ReqArgB_ *args; } ReqB_;
s32 nuContDataLock();
s32 nuContDataUnLock();
extern u8 nuContData;

s32 nuSiCallBackAdd(s32 *);
extern s32 nuContPakCallBack;
s32 nuSiCallBackRemove(s32 *);
extern s32 D_8014EB40;
extern u16 D_800FB1B0;
extern s32 D_800FB1B4;

void nuContDataGetAll(s32 arg0) {
    nuContDataLock();
    bcopy(&nuContData, arg0, 0x18);
    nuContDataUnLock();
}

void nuContPakMgrInit(void) {
    nuSiCallBackAdd(&nuContPakCallBack);
}

void nuContPakMgrRemove(void) {
    nuSiCallBackRemove(&nuContPakCallBack);
}

s32 contPakOpen(NuSiMesg_ *mesg) {
    NuPakFile_ *file = mesg->data;
    s32 ret;

    file->type = 0;
    ret = osPfsInitPak(&D_8014EB40, file->pfs, file->pfs->channel);
    if (ret == 0) {
        file->type = 1;
    }
    return ret;
}

void contPakFree(NuSiMesg_ *mesg) {
    osPfsFreeBlocks(*(s32 *)((NuPakFile_ *)mesg->data)->pfs, &((NuPakFile_ *)mesg->data)->file_no);
}

s32 contPakFileReadWrite(ReqB_ *r) {
    ReqArgB_ *a = r->args;
    return ((s32 (*)(void *pfs, s32 file_no, u8 flag, s32 offset, s32 size, u8 *data_buffer))osPfsReadWriteFile)(a->file->pfs, a->file->file_no, a->flag, a->offset, a->size, a->buffer);
}


s32 contPakFileOpen(ReqS_ *r) {
    ArgsS_ *a = r->args;
    PakFileS_ *f = a->file;
    s32 ret;

    ret = osPfsFindFile(f->pfs, D_800FB1B0, D_800FB1B4, a->name, a->ext, &f->file_no);
    if (ret == 5 && a->create == 1) {
        ret = osPfsAllocateFile(f->pfs, D_800FB1B0, D_800FB1B4, a->name, a->ext, a->size, &f->file_no);
    }
    return ret;
}

void contPakFileDelete(NuSiMesg_ *mesg) {
    ArgsS_ *a = mesg->data;

    osPfsDeleteFile(a->file->pfs, D_800FB1B0, D_800FB1B4, a->name, a->ext);
}

void contPakFileState(NuSiMesg_ *mesg) {
    ArgsS_ *a = mesg->data;

    osPfsFileState((s32)a->file->pfs, a->file->file_no, (s32)a->name);
}

void contPakFileNum(NuSiMesg_ *mesg) {
    ArgsS_ *a = mesg->data;

    osPfsNumFiles(a->file->pfs, a->name, a->ext);
}

void contPakRepairId(NuSiMesg_ *mesg) {
    osPfsRepairId(((NuPakFile_ *)mesg->data)->pfs);
}
