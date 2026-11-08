#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } PfsC_;
s32 __osPfsSelectBank(PfsC_ *, u8);
s32 __osContRamRead(void *, s32, u16, u8 *);
s32 func_800E40D0(void *, void *, s32);
typedef struct { u32 repaired; u32 random; u64 serial_mid; u64 serial_low; u16 deviceid; u8 banks; u8 version; u16 checksum; u16 inverted_checksum; } PackId_;
s32 __osContRamWrite(void *mq, s32 channel, u16 address, u8 *buffer, s32 force);
s32 __osRepairPackId(PfsC_ *pfs, PackId_ *badid, PackId_ *newid);
void bcopy(const void *src, void *dst, s32 n);
u32 osGetCount(void);
typedef struct { u8 bank; u8 page; } InodeUnit_;
typedef struct { InodeUnit_ inode_page[128]; } Inode2_;
extern s32 D_800FC7D0;
extern u8 D_800FC7D4;
extern Inode2_ D_8014C548;



u16 __osSumcalc(u8 *ptr, s32 length) {
    s32 i;
    u32 sum;
    u8 *tmp;

    sum = 0;
    tmp = ptr;
    for (i = 0; i < length; i++) {
        sum += *tmp;
        tmp++;
    }
    return sum & 0xFFFF;
}

s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data = 0;
    u32 j;

    *csum = *icsum = 0;
    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u32)ptr + j);
        *csum += data;
        *icsum += ~data;
    }
    return 0;
}

s32 __osRepairPackId(PfsC_ *pfs, PackId_ *badid, PackId_ *newid) {
    s32 ret = 0;
    u8 temp[32];
    u8 comp[32];
    u8 mask = 0;
    s32 i;
    s32 j;
    u16 index[4];

    j = 0;
    newid->repaired = -1;
    newid->random = osGetCount();
    newid->serial_mid = badid->serial_mid;
    newid->serial_low = badid->serial_low;
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    do {
        ret = __osPfsSelectBank(pfs, j);
        if (ret != 0)
            return ret;
        ret = __osContRamRead(pfs->queue, pfs->channel, 0, temp);
        if (ret != 0)
            return ret;
        temp[0] = j | 0x80;
        for (i = 1; i < 32; i++) {
            temp[i] = ~temp[i];
        }
        ret = __osContRamWrite(pfs->queue, pfs->channel, 0, temp, 0);
        if (ret != 0)
            return ret;
        ret = __osContRamRead(pfs->queue, pfs->channel, 0, comp);
        if (ret != 0)
            return ret;
        for (i = 0; i < 32; i++) {
            if (comp[i] != temp[i]) {
                break;
            }
        }
        if (i != 32) {
            break;
        }
        if (j > 0) {
            ret = __osPfsSelectBank(pfs, 0);
            if (ret != 0)
                return ret;
            ret = __osContRamRead(pfs->queue, pfs->channel, 0, (u8 *)temp);
            if (ret != 0)
                return ret;
            if (temp[0] != 0x80) {
                break;
            }
        }
        j++;
    } while (j < 62);
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    mask = (j > 0) ? 1 : 0;
    newid->deviceid = (badid->deviceid & (u16)~1) | mask;
    newid->banks = j;
    newid->version = badid->version;
    __osIdCheckSum((u16 *)newid, &newid->checksum, &newid->inverted_checksum);
    index[0] = 1;
    index[1] = 3;
    index[2] = 4;
    index[3] = 6;
    for (i = 0; i < 4; i++) {
        ret = __osContRamWrite(pfs->queue, pfs->channel, index[i], (u8 *)newid, 1);
        if (ret != 0)
            return ret;
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, (u8 *)temp);
    if (ret != 0)
        return ret;
    for (i = 0; i < 32; i++) {
        if (temp[i] != ((u8 *)newid)[i]) {
            return 11;
        }
    }
    return 0;
}

s32 __osCheckPackId(PfsC_ *pfs, PackId_ *temp) {
    u16 index[4];
    s32 ret = 0;
    u16 sum;
    u16 isum;
    s32 i;
    s32 j;

    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    index[0] = 1;
    index[1] = 3;
    index[2] = 4;
    index[3] = 6;
    for (i = 1; i < 4; i++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, index[i], (u8 *)temp);
        if (ret != 0)
            return ret;
        __osIdCheckSum((u16 *)temp, &sum, &isum);
        if (temp->checksum == sum && temp->inverted_checksum == isum) {
            break;
        }
    }
    if (i == 4) {
        return 10;
    }
    for (j = 0; j < 4; j++) {
        if (j != i) {
            ret = __osContRamWrite(pfs->queue, pfs->channel, index[j], (u8 *)temp, 1);
            if (ret != 0)
                return ret;
        }
    }
    return 0;
}

s32 __osGetId(PfsC_ *pfs) {
    u16 sum;
    u16 isum;
    u8 temp[32];
    PackId_ newid;
    s32 ret;
    PackId_ *id;

    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, (u8 *)temp);
    if (ret != 0)
        return ret;
    __osIdCheckSum((u16 *)temp, &sum, &isum);
    id = (PackId_ *)temp;
    if (id->checksum != sum || id->inverted_checksum != isum) {
        ret = __osCheckPackId(pfs, id);
        if (ret == 10) {
            ret = __osRepairPackId(pfs, id, &newid);
            if (ret != 0)
                return ret;
            id = &newid;
        } else if (ret != 0) {
            return ret;
        }
    }
    if ((id->deviceid & 1) == 0) {
        ret = __osRepairPackId(pfs, id, &newid);
        if (ret != 0)
            return ret;
        id = &newid;
        if ((id->deviceid & 1) == 0) {
            return 11;
        }
    }
    bcopy(id, pfs->id, 32);
    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + 2 + (2 * pfs->banks);
    pfs->dir_size = 16;
    pfs->inode_table = 8;
    pfs->minode_table = (1 + pfs->banks) * 8;
    pfs->dir_table = pfs->minode_table + pfs->banks * 8;
    ret = __osContRamRead(pfs->queue, pfs->channel, 7, pfs->label);
    if (ret != 0)
        return ret;
    return 0;
}

s32 __osCheckId(PfsC_ *pfs) {
    u8 temp[32];
    s32 ret;

    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret == 2) {
            ret = __osPfsSelectBank(pfs, 0);
        }
        if (ret != 0) {
            return ret;
        }
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, temp);
    if (ret != 0) {
        if (ret != 2) {
            return ret;
        }
        ret = __osContRamRead(pfs->queue, pfs->channel, 1, temp);
        if (ret != 0) {
            return ret;
        }
    }
    if (func_800E40D0(pfs->id, temp, 32) != 0) {
        return 2;
    }
    return 0;
}

s32 __osPfsRWInode(PfsC_ *pfs, Inode2_ *inode, u8 flag, u8 bank) {
    u8 sum;
    s32 j;
    s32 ret;
    s32 offset;
    u8 *addr;

    if (flag == 0 && bank == D_800FC7D4 && (pfs->channel == D_800FC7D0)) {
        bcopy(&D_8014C548, inode, sizeof(Inode2_));
        return 0;
    }
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    offset = (bank > 0) ? 1 : pfs->inode_start_page;
    if (flag == 1) {
        inode->inode_page[0].page = __osSumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
    }
    for (j = 0; j < 8; j++) {
        addr = ((u8 *)inode->inode_page + j * 32);
        if (flag == 1) {
            ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr, 0);
            ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr, 0);
        } else {
            ret = __osContRamRead(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr);
        }
        if (ret != 0) {
            return ret;
        }
    }
    if (flag == 0) {
        sum = __osSumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
        if (sum != inode->inode_page[0].page) {
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = __osContRamRead(pfs->queue, pfs->channel, pfs->minode_table + bank * 8 + j, addr);
            }
            sum = __osSumcalc((u8 *)&inode->inode_page[offset], (128 - offset) * 2);
            if (sum != inode->inode_page[0].page) {
                return 3;
            }
            for (j = 0; j < 8; j++) {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * 8 + j, addr, 0);
            }
        }
    }
    D_800FC7D4 = bank;
    bcopy(inode, &D_8014C548, sizeof(Inode2_));
    D_800FC7D0 = pfs->channel;
    return 0;
}
