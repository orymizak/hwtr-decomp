#include "common.h"
#include "include_asm.h"
typedef struct { u32 repaired; u32 random; u64 serial_mid; u64 serial_low; u16 deviceid; u8 banks; u8 version; u16 checksum; u16 inverted_checksum; } PackId_;
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } OSPfs_;
typedef struct { u32 game_code; u16 company_code; u16 start_page; u8 status; s8 reserved; u16 data_sum; u8 ext_name[4]; u8 game_name[16]; } PfsDir_;



s32 __osCheckId(OSPfs_ *pfs);
s32 __osPfsSelectBank(OSPfs_ *pfs, u8 bank);
s32 __osContRamRead(void *mq, s32 channel, u16 address, u8 *buffer);
s32 __osPfsGetStatus(void *queue, s32 channel);
s32 osPfsNumFiles(OSPfs_ *pfs, s32 *max_files, s32 *files_used) {
    s32 j;
    s32 ret;
    PfsDir_ dir;
    s32 files = 0;

    if ((pfs->status & 1) == 0)
        return 5;
    ret = __osCheckId(pfs);
    if (ret != 0)
        return ret;
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    for (j = 0; j < pfs->dir_size; j++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&dir);
        if (ret != 0)
            return ret;
        if (dir.company_code != 0 && dir.game_code != 0) {
            files++;
        }
    }
    *files_used = files;
    *max_files = pfs->dir_size;
    ret = __osPfsGetStatus(pfs->queue, pfs->channel);
    return ret;
}

void __osSiGetAccess(void);
void __osSiRelAccess(void);
s32 __osPfsCheckRamArea(OSPfs_ *pfs);
void __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum);
s32 __osCheckPackId(OSPfs_ *pfs, PackId_ *temp);
s32 __osRepairPackId(OSPfs_ *pfs, PackId_ *badid, PackId_ *newid);
void bcopy(const void *src, void *dst, s32 n);
s32 osPfsChecker(OSPfs_ *pfs);
s32 osPfsInitPak(void *queue, OSPfs_ *pfs, s32 channel) {
    s32 ret = 0;
    u16 sum;
    u16 isum;
    u8 temp[32];
    PackId_ *id;
    PackId_ newid;

    __osSiGetAccess();
    ret = __osPfsGetStatus(queue, channel);
    __osSiRelAccess();
    if (ret != 0) {
        return ret;
    }
    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;
    ret = __osPfsCheckRamArea(pfs);
    if (ret != 0)
        return ret;
    ret = __osPfsSelectBank(pfs, 0);
    if (ret != 0)
        return ret;
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, temp);
    if (ret != 0)
        return ret;
    __osIdCheckSum((u16 *)temp, &sum, &isum);
    id = (PackId_ *)temp;
    if ((id->checksum != sum) || (id->inverted_checksum != isum)) {
        ret = __osCheckPackId(pfs, id);
        if (ret != 0) {
            pfs->status |= 4;
            return ret;
        }
    }
    if (!(id->deviceid & 1)) {
        ret = __osRepairPackId(pfs, id, &newid);
        if (ret != 0) {
            if (ret == 10) {
                pfs->status |= 4;
            }
            return ret;
        }
        id = &newid;
        if (!(id->deviceid & 1)) {
            return 11;
        }
    }
    bcopy(id, pfs->id, 32);
    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + 2 + (2 * pfs->banks);
    pfs->dir_size = 2 * 8;
    pfs->inode_table = 1 * 8;
    pfs->minode_table = (1 + pfs->banks) * 8;
    pfs->dir_table = pfs->minode_table + (pfs->banks * 8);
    ret = __osContRamRead(pfs->queue, pfs->channel, 7, pfs->label);
    if (ret != 0)
        return ret;
    ret = osPfsChecker(pfs);
    pfs->status |= 1;
    return ret;
}

s32 __osContRamWrite(void *mq, s32 channel, u16 address, u8 *buffer, s32 force);
s32 func_800E40D0(void *v1, void *v2, u32 size);
s32 __osPfsCheckRamArea(OSPfs_ *pfs) {
    s32 i;
    s32 ret = 0;
    u8 temp1[32];
    u8 temp2[32];
    u8 save[32];

    ret = __osPfsSelectBank(pfs, 0);
    if (ret != 0)
        return ret;
    ret = __osContRamRead(pfs->queue, pfs->channel, 0, save);
    if (ret != 0)
        return ret;
    for (i = 0; i < 32; i++) {
        temp1[i] = i;
    }
    ret = __osContRamWrite(pfs->queue, pfs->channel, 0, temp1, 0);
    if (ret != 0)
        return ret;
    ret = __osContRamRead(pfs->queue, pfs->channel, 0, temp2);
    if (ret != 0)
        return ret;
    if (func_800E40D0(temp1, temp2, 32) != 0) {
        return 11;
    }
    ret = __osContRamWrite(pfs->queue, pfs->channel, 0, save, 0);
    return ret;
}
