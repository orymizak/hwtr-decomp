#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } OSPfs_;
typedef union { struct { u8 bank; u8 page; } inode_t; u16 ipage; } InodeUnit3_;
typedef struct { InodeUnit3_ inode_page[128]; } Inode3_;
typedef struct { Inode3_ inode; u8 bank; u8 map[256]; } InodeCache_;
typedef struct { u32 game_code; u16 company_code; InodeUnit3_ start_page; u8 status; s8 reserved; u16 data_sum; u8 ext_name[4]; u8 game_name[16]; } Dir3_;
typedef struct { u8 dummy; u8 txsize; u8 rxsize; u8 cmd; u8 typeh; u8 typel; u8 status; u8 dummy1; } ReqFormat_;
extern u32 D_8014C73C;
extern u8 D_80145498;
void __osPfsRequestData(u8 cmd);
typedef struct { u16 type; u8 status; u8 errnum; } ContStatus_;
extern u8 D_8014C700;
extern u8 D_80141590;
void __osSiGetAccess(void);
void __osSiRelAccess(void);
s32 __osSiRawStartDma(s32 direction, void *dramAddr);
s32 osRecvMesg(void *mq, void **msg, s32 flag);
void func_800E5BB0(u8 *pattern, ContStatus_ *data);



s32 __osPfsRWInode(OSPfs_ *pfs, Inode3_ *inode, u8 flag, u8 bank);
s32 __osCheckId(OSPfs_ *pfs);
s32 __osGetId(OSPfs_ *pfs);
s32 __osPfsSelectBank(OSPfs_ *pfs, u8 bank);
s32 __osContRamRead(void *mq, s32 channel, u16 address, u8 *buffer);
s32 __osContRamWrite(void *mq, s32 channel, u16 address, u8 *buffer, s32 force);
void bzero(void *p, s32 n);
s32 corrupted_init(OSPfs_ *pfs, InodeCache_ *cache);
s32 corrupted(OSPfs_ *pfs, InodeUnit3_ fpage, InodeCache_ *cache);
s32 osPfsChecker(OSPfs_ *pfs) {
    s32 j;
    s32 ret;
    InodeUnit3_ next_page;
    Inode3_ checked_inode;
    Inode3_ tmp_inode;
    Dir3_ tmp_dir;
    InodeUnit3_ file_next_node[16];
    InodeCache_ cache;
    s32 fixed = 0;
    u8 bank;
    u8 oldbank = 254;
    s32 cc;
    s32 cl;
    s32 offset;

    ret = __osCheckId(pfs);
    if (ret == 2) {
        ret = __osGetId(pfs);
    }
    if (ret != 0) {
        return ret;
    }
    ret = corrupted_init(pfs, &cache);
    if (ret != 0)
        return ret;
    for (j = 0; j < pfs->dir_size; j++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir);
        if (ret != 0)
            return ret;
        if (tmp_dir.company_code != 0 || tmp_dir.game_code != 0) {
            if (tmp_dir.company_code == 0 || tmp_dir.game_code == 0) {
                cc = -1;
            } else {
                next_page = tmp_dir.start_page;
                cl = cc = 0;
                bank = 255;
                while ((next_page.ipage >= pfs->inode_start_page) && (next_page.inode_t.bank < pfs->banks) && (next_page.inode_t.page >= 0x01) && (next_page.inode_t.page < 0x80)) {
                    if (bank != next_page.inode_t.bank) {
                        bank = next_page.inode_t.bank;
                        if (oldbank != bank) {
                            ret = __osPfsRWInode(pfs, &tmp_inode, 0, bank);
                            oldbank = bank;
                        }
                        if (ret != 0 && ret != 3) {
                            return ret;
                        }
                    }
                    if ((cc = corrupted(pfs, next_page, &cache) - cl) != 0) {
                        break;
                    }
                    cl = 1;
                    next_page = tmp_inode.inode_page[next_page.inode_t.page];
                }
            }
            if (cc != 0 || next_page.ipage != 1) {
                bzero(&tmp_dir, sizeof(Dir3_));
                if (pfs->activebank != 0) {
                    ret = __osPfsSelectBank(pfs, 0);
                    if (ret != 0)
                        return ret;
                }
                ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, 0);
                if (ret != 0)
                    return ret;
                fixed++;
            }
        }
    }
    for (j = 0; j < pfs->dir_size; j++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir);
        if (ret != 0)
            return ret;
        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0 && tmp_dir.start_page.ipage >= (u16)pfs->inode_start_page) {
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        } else {
            file_next_node[j].ipage = 0;
        }
    }
    for (bank = 0; bank < pfs->banks; bank++) {
        ret = __osPfsRWInode(pfs, &tmp_inode, 0, bank);
        if (ret != 0 && ret != 3) {
            return ret;
        }
        offset = (bank > 0) ? 1 : pfs->inode_start_page;
        for (j = 0; j < offset; j++) {
            checked_inode.inode_page[j].ipage = tmp_inode.inode_page[j].ipage;
        }
        for (; j < 128; j++) {
            checked_inode.inode_page[j].ipage = 3;
        }
        for (j = 0; j < pfs->dir_size; j++) {
            while (file_next_node[j].inode_t.bank == bank && file_next_node[j].ipage >= (u16)pfs->inode_start_page) {
                u8 pp = file_next_node[j].inode_t.page;
                file_next_node[j] = checked_inode.inode_page[pp] = tmp_inode.inode_page[pp];
            }
        }
        ret = __osPfsRWInode(pfs, &checked_inode, 1, bank);
        if (ret != 0)
            return ret;
    }
    if (fixed) {
        pfs->status |= 2;
    } else {
        pfs->status &= ~2;
    }
    return 0;
}

s32 corrupted_init(OSPfs_ *pfs, InodeCache_ *cache) {
    s32 i;
    s32 n;
    s32 offset;
    u8 bank;
    InodeUnit3_ tpage;
    Inode3_ tmp_inode;
    s32 ret;

    for (i = 0; i < 256; i++) {
        cache->map[i] = 0;
    }
    cache->bank = -1;
    for (bank = 0; bank < pfs->banks; bank++) {
        offset = bank > 0 ? 1 : pfs->inode_start_page;
        ret = __osPfsRWInode(pfs, &tmp_inode, 0, bank);
        if (ret != 0 && ret != 3) {
            return ret;
        }
        for (i = offset; i < 128; i++) {
            tpage = tmp_inode.inode_page[i];
            if (tpage.ipage >= pfs->inode_start_page && tpage.inode_t.bank != bank) {
                n = ((tpage.inode_t.page & 0x7F) / 4) + ((tpage.inode_t.bank % 8) * 32);
                cache->map[n] |= 1 << (bank % 8);
            }
        }
    }
    return 0;
}

s32 corrupted(OSPfs_ *pfs, InodeUnit3_ fpage, InodeCache_ *cache) {
    s32 j;
    s32 n;
    s32 hit;
    u8 bank;
    s32 offset;
    s32 ret;

    hit = 0;
    ret = 0;
    n = (fpage.inode_t.page / 4) + (fpage.inode_t.bank % 8) * 32;
    for (bank = 0; bank < pfs->banks; bank++) {
        offset = bank > 0 ? 1 : pfs->inode_start_page;
        if (bank == fpage.inode_t.bank || cache->map[n] & (1 << (bank % 8))) {
            if (bank != cache->bank) {
                ret = __osPfsRWInode(pfs, &cache->inode, 0, bank);
                if (ret != 0 && ret != 3) {
                    return ret;
                }
                cache->bank = bank;
            }
            for (j = offset; hit < 2 && (j < 128); j++) {
                if (cache->inode.inode_page[j].ipage == fpage.ipage) {
                    hit++;
                }
            }
            if (hit >= 2) {
                return 2;
            }
        }
    }
    return hit;
}

s32 osPfsIsPlug(void *mq, u8 *pattern) {
    s32 ret = 0;
    void *msg;
    u8 bitpattern;
    ContStatus_ contData[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    __osSiGetAccess();
    do {
        __osPfsRequestData(0);
        ret = __osSiRawStartDma(1, &D_8014C700);
        osRecvMesg(mq, &msg, 1);
        ret = __osSiRawStartDma(0, &D_8014C700);
        osRecvMesg(mq, &msg, 1);
        func_800E5BB0(&bitpattern, &contData[0]);
        for (channel = 0; channel < D_80141590; channel++) {
            if ((contData[channel].status & 4) == 0) {
                crcErrorCount--;
                break;
            }
        }
        if (channel == D_80141590) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);
    for (channel = 0; channel < D_80141590; channel++) {
        if ((contData[channel].errnum == 0) && ((contData[channel].status & 1) != 0)) {
            bits |= (1 << channel);
        }
    }
    __osSiRelAccess();
    *pattern = bits;
    return ret;
}


void __osPfsRequestData(u8 cmd) {
    u8 *ptr = &D_8014C700;
    ReqFormat_ requestformat;
    s32 i;

    D_80145498 = cmd;
    D_8014C73C = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    for (i = 0; i < D_80141590; i++) {
        *((ReqFormat_ *)ptr) = requestformat;
        ptr += sizeof(ReqFormat_);
    }
    *ptr = 0xFE;
}


void func_800E5BB0(u8 *pattern, ContStatus_ *data) {
    u8 *ptr;
    ReqFormat_ requestformat;
    s32 i;
    u8 bits = 0;

    ptr = &D_8014C700;
    for (i = 0; i < D_80141590; i++, ptr += sizeof(requestformat), data++) {
        requestformat = *((ReqFormat_ *)ptr);
        data->errnum = (requestformat.rxsize & 0xC0) >> 4;
        if (data->errnum != 0) {
            continue;
        }
        data->type = ((requestformat.typel << 8) | requestformat.typeh);
        data->status = requestformat.status;
        bits |= (1 << i);
    }
    *pattern = bits;
}
