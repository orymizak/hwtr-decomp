#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } PfsD_;
typedef union { struct { u8 bank; u8 page; } inode_t; u16 ipage; } InodeUnitD_;
typedef struct { u32 game_code; u16 company_code; InodeUnitD_ start_page; u8 status; s8 reserved; u16 data_sum; u8 ext_name[4]; u8 game_name[16]; } PfsDirD_;
typedef struct { InodeUnitD_ inode_page[128]; } InodeD_;
void bzero(void *, s32);
s32 osPfsFindFile(PfsD_ *, u16, u32, u8 *, u8 *, s32 *);
s32 __osPfsSelectBank(PfsD_ *, s32);
s32 __osContRamRead(void *, s32, u16, u8 *);
s32 __osPfsRWInode(PfsD_ *, InodeD_ *, s32, s32);
s32 __osContRamWrite(void *, s32, u16, u8 *, s32);
typedef union { struct { u8 bank; u8 page; } t; u16 ipage; } InodeUnit_;
typedef struct { u8 pad[0x60]; s32 inode_start_page; } Pfs_;
typedef struct { InodeUnit_ page[128]; } Inode_;



s32 osPfsDeleteFile(PfsD_ *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name) {
    s32 file_no;
    s32 ret;
    InodeD_ inode;
    PfsDirD_ dir;
    InodeUnitD_ last_page;
    u8 startpage;
    u8 bank;

    if ((company_code == 0) || (game_code == 0)) {
        return 5;
    }
    if ((ret = osPfsFindFile(pfs, company_code, game_code, game_name, ext_name, &file_no)) != 0) {
        return ret;
    }
    if (pfs->activebank != 0 && (ret = __osPfsSelectBank(pfs, 0)) != 0) {
        return ret;
    }
    if ((ret = __osContRamRead(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir)) != 0) {
        return ret;
    }
    startpage = dir.start_page.inode_t.page;
    for (bank = dir.start_page.inode_t.bank; bank < pfs->banks;) {
        if ((ret = __osPfsRWInode(pfs, &inode, 0, bank)) != 0) {
            return ret;
        }
        if ((ret = __osPfsReleasePages((Pfs_ *)pfs, (Inode_ *)&inode, startpage, bank, (InodeUnit_ *)&last_page)) != 0) {
            return ret;
        }
        if ((ret = __osPfsRWInode(pfs, &inode, 1, bank)) != 0) {
            return ret;
        }
        if (last_page.ipage == 1) {
            break;
        }
        bank = last_page.inode_t.bank;
        startpage = last_page.inode_t.page;
    }
    if (bank >= pfs->banks) {
        return 3;
    }
    bzero(&dir, sizeof(PfsDirD_));
    ret = __osContRamWrite(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir, 0);
    return ret;
}

s32 __osPfsReleasePages(Pfs_ *pfs, Inode_ *inode, u8 start_page, u8 bank, InodeUnit_ *final_page) {
    InodeUnit_ next;
    InodeUnit_ prev;

    next.ipage = (u16)((bank << 8) + start_page);
    do {
        prev = next;
        next = inode->page[prev.t.page];
        inode->page[prev.t.page].ipage = 3;
    } while (next.ipage >= pfs->inode_start_page && next.t.bank == bank);
    *final_page = next;
    return 0;
}
