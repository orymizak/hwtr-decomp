#include "common.h"
#include "include_asm.h"
#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PRinternal/controller.h"

s32 osPfsDeleteFile(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name) {
    s32 file_no;
#if BUILD_VERSION < VERSION_J
    int k;
#endif
    s32 ret;
    __OSInode inode;
    __OSDir dir;
#if BUILD_VERSION < VERSION_J
    u16 sum = 0;
#endif
    __OSInodeUnit last_page;
    u8 startpage;
    u8 bank;

    if (company_code == 0 || game_code == 0) {
        return PFS_ERR_INVALID;
    }

#if BUILD_VERSION >= VERSION_J
    ERRCK(osPfsFindFile(pfs, company_code, game_code, game_name, ext_name, &file_no));
    SET_ACTIVEBANK_TO_ZERO();
#else
    PFS_CHECK_STATUS();
    PFS_CHECK_ID();
    SET_ACTIVEBANK_TO_ZERO();
    ERRCK(osPfsFindFile(pfs, company_code, game_code, game_name, ext_name, &file_no));
    
    if (file_no == -1) {
        return PFS_ERR_INVALID;
    }
#endif
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8*)&dir));

    startpage = dir.start_page.inode_t.page;

    for (bank = dir.start_page.inode_t.bank; bank < pfs->banks;) {
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_READ, bank));
#if BUILD_VERSION >= VERSION_J
        ERRCK(__osPfsReleasePages(pfs, &inode, startpage, bank, &last_page));
#else
        ERRCK(__osPfsReleasePages(pfs, &inode, startpage, &sum, bank, &last_page, TRUE));
#endif
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_WRITE, bank));

        if (last_page.ipage == PFS_EOF) {
            break;
        }

        bank = last_page.inode_t.bank;
        startpage = last_page.inode_t.page;
    }

    if (bank >= pfs->banks) {
        return PFS_ERR_INCONSISTENT;
    }

#if BUILD_VERSION >= VERSION_J
    bzero(&dir, sizeof(__OSDir));
#else
    dir.game_code = 0;
    dir.company_code = 0;
    dir.start_page.ipage = 0;
    dir.data_sum = 0;
    for (k = 0; k < ARRLEN(dir.game_name); k++)
    {
        dir.game_name[k] = 0;
    }
    for (k = 0; k < ARRLEN(dir.ext_name); k++)
    {
        dir.ext_name[k] = 0;
    }
    dir.status = DIR_STATUS_EMPTY;
#endif

    ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->dir_table + file_no, (u8*)&dir, FALSE);

    return ret;
}

s32 __osPfsReleasePages(OSPfs* pfs, __OSInode* inode, u8 start_page, u8 bank, __OSInodeUnit* last_page) {
    __OSInodeUnit next_page;
    __OSInodeUnit old_page;
    s32 ret = 0;

    next_page.ipage = (bank << 8) + start_page;

    do {
        old_page = next_page;
        next_page = inode->inode_page[next_page.inode_t.page];
        inode->inode_page[old_page.inode_t.page].ipage = PFS_PAGE_NOT_USED;
    } while (next_page.ipage >= pfs->inode_start_page && next_page.inode_t.bank == bank);

    *last_page = next_page;

    return ret;
}
