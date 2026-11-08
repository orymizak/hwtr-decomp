#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } OSPfs_;
typedef struct { u16 inode_page[128]; } Inode_;
s32 __osCheckId(OSPfs_ *pfs);
s32 __osPfsRWInode(OSPfs_ *pfs, Inode_ *inode, u8 flag, u8 bank);



s32 osPfsFreeBlocks(OSPfs_ *pfs, s32 *bytes_not_used) {
    s32 j;
    s32 pages = 0;
    Inode_ inode;
    s32 ret = 0;
    u8 bank;
    s32 offset;

    if ((pfs->status & 1) == 0)
        return 5;
    ret = __osCheckId(pfs);
    if (ret != 0)
        return ret;
    for (bank = 0; bank < pfs->banks; bank++) {
        ret = __osPfsRWInode(pfs, &inode, 0, bank);
        if (ret != 0)
            return ret;
        offset = ((bank > 0) ? 1 : pfs->inode_start_page);
        for (j = offset; j < 128; j++) {
            if (inode.inode_page[j] == 3) {
                pages++;
            }
        }
    }
    *bytes_not_used = pages * 8 * 32;
    return 0;
}
