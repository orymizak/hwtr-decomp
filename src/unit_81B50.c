#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } OSPfs_;
typedef union { struct { u8 bank; u8 page; } inode_t; u16 ipage; } InodeUnit3_;
typedef struct { InodeUnit3_ inode_page[128]; } Inode3_;
typedef struct { u32 game_code; u16 company_code; InodeUnit3_ start_page; u8 status; s8 reserved; u16 data_sum; u8 ext_name[4]; u8 game_name[16]; } Dir3_;
typedef struct { u32 file_size; u32 game_code; u16 company_code; char ext_name[4]; char game_name[16]; } PfsState_;
s32 __osCheckId(OSPfs_ *pfs);
s32 __osPfsSelectBank(OSPfs_ *pfs, u8 bank);
s32 __osContRamRead(void *mq, s32 channel, u16 address, u8 *buffer);
s32 __osContRamWrite(void *mq, s32 channel, u16 address, u8 *buffer, s32 force);
s32 __osPfsRWInode(OSPfs_ *pfs, Inode3_ *inode, u8 flag, u8 bank);
s32 __osPfsGetStatus(void *queue, s32 channel);
void bcopy(const void *src, void *dst, s32 n);

#define CHECK_IPAGE(p) (((p).ipage >= pfs->inode_start_page) && ((p).inode_t.bank < pfs->banks) && ((p).inode_t.page >= 0x01) && ((p).inode_t.page < 0x80))

static s32 __osPfsGetNextPage(OSPfs_ *pfs, u8 *bank, Inode3_ *inode, InodeUnit3_ *page) {
    s32 ret;

    if (page->inode_t.bank != *bank) {
        *bank = page->inode_t.bank;
        ret = __osPfsRWInode(pfs, inode, 0, *bank);
        if (ret != 0)
            return ret;
    }
    *page = inode->inode_page[page->inode_t.page];
    if (!CHECK_IPAGE(*page)) {
        if (page->ipage == 1) {
            return 5;
        }
        return 3;
    }
    return 0;
}

s32 osPfsReadWriteFile(OSPfs_ *pfs, s32 file_no, u8 flag, s32 offset, s32 size_in_bytes, u8 *data_buffer) {
    s32 ret;
    Dir3_ dir;
    Inode3_ inode;
    InodeUnit3_ cur_page;
    s32 cur_block;
    s32 siz_block;
    u8 *buffer;
    u8 bank;
    u16 blockno;

    if ((file_no >= (s32)pfs->dir_size) || (file_no < 0)) {
        return 5;
    }
    if ((size_in_bytes <= 0) || ((size_in_bytes % 32) != 0)) {
        return 5;
    }
    if ((offset < 0) || ((offset % 32) != 0)) {
        return 5;
    }
    if ((pfs->status & 1) == 0)
        return 5;
    if (__osCheckId(pfs) == 2)
        return 2;
    if (pfs->activebank != 0) {
        ret = __osPfsSelectBank(pfs, 0);
        if (ret != 0)
            return ret;
    }
    ret = __osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8 *)&dir);
    if (ret != 0)
        return ret;
    if (dir.company_code == 0 || dir.game_code == 0) {
        return 5;
    }
    if (!CHECK_IPAGE(dir.start_page)) {
        if ((dir.start_page.ipage == 1)) {
            return 5;
        }
        return 3;
    }
    if (flag == 0 && (dir.status & 2) == 0) {
        return 6;
    }
    bank = -1;
    cur_block = offset / 32;
    cur_page = dir.start_page;
    while (cur_block >= 8) {
        ret = __osPfsGetNextPage(pfs, &bank, &inode, &cur_page);
        if (ret != 0)
            return ret;
        cur_block -= 8;
    }
    siz_block = size_in_bytes / 32;
    buffer = data_buffer;
    while (siz_block > 0) {
        if (cur_block == 8) {
            ret = __osPfsGetNextPage(pfs, &bank, &inode, &cur_page);
            if (ret != 0)
                return ret;
            cur_block = 0;
        }
        if (pfs->activebank != cur_page.inode_t.bank) {
            ret = __osPfsSelectBank(pfs, cur_page.inode_t.bank);
            if (ret != 0)
                return ret;
        }
        blockno = cur_page.inode_t.page * 8 + cur_block;
        if (flag == 0) {
            ret = __osContRamRead(pfs->queue, pfs->channel, blockno, buffer);
        } else {
            ret = __osContRamWrite(pfs->queue, pfs->channel, blockno, buffer, 0);
        }
        if (ret != 0) {
            return ret;
        }
        buffer += 32;
        cur_block++;
        siz_block--;
    }
    if (flag == 1 && (dir.status & 2) == 0) {
        dir.status |= 2;
        if (pfs->activebank != 0) {
            ret = __osPfsSelectBank(pfs, 0);
            if (ret != 0)
                return ret;
        }
        ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8 *)&dir, 0);
        if (ret != 0)
            return ret;
    }
    ret = __osPfsGetStatus(pfs->queue, pfs->channel);
    return ret;
}

s32 osPfsFileState(OSPfs_ *pfs, s32 file_no, PfsState_ *state) {
    s32 ret;
    s32 pages;
    Inode3_ inode;
    Dir3_ dir;
    InodeUnit3_ next_page;
    u8 bank;

    if (file_no >= pfs->dir_size || file_no < 0) {
        return 5;
    }
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
    ret = __osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8 *)&dir);
    if (ret != 0)
        return ret;
    if (dir.company_code == 0 || dir.game_code == 0) {
        return 5;
    }
    pages = 0;
    next_page = dir.start_page;
    bank = 0xFF;
    while (1) {
        if (next_page.ipage < pfs->inode_start_page) {
            break;
        } else if (next_page.inode_t.bank != bank) {
            bank = next_page.inode_t.bank;
            ret = __osPfsRWInode(pfs, &inode, 0, bank);
            if (ret != 0)
                return ret;
        }
        pages++;
        next_page = inode.inode_page[next_page.inode_t.page];
    }
    if (next_page.ipage != 1) {
        return 3;
    }
    state->file_size = pages * (8 * 32);
    state->company_code = dir.company_code;
    state->game_code = dir.game_code;
    bcopy(&dir.game_name, state->game_name, 16);
    bcopy(&dir.ext_name, state->ext_name, 4);
    ret = __osPfsGetStatus(pfs->queue, pfs->channel);
    return ret;
}
