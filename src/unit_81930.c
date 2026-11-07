#include "common.h"
#include "include_asm.h"
typedef union { struct { u8 bank; u8 page; } t; u16 ipage; } InodeUnit_;
typedef struct { u8 pad[0x60]; s32 inode_start_page; } Pfs_;
typedef struct { InodeUnit_ page[128]; } Inode_;



INCLUDE_ASM("asm/nonmatchings/unit_81930", func_800E0D30);

s32 func_800E0EDC(Pfs_ *pfs, Inode_ *inode, u8 start_page, u8 bank, InodeUnit_ *final_page) {
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
