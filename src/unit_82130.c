#include "common.h"
#include "include_asm.h"
typedef struct { s32 status; void *queue; s32 channel; u8 id[32]; u8 label[32]; s32 version; s32 dir_size; s32 inode_table; s32 minode_table; s32 dir_table; s32 inode_start_page; u8 banks; u8 activebank; } Pfs_;
typedef struct { u32 game_code; u16 company_code; u16 start_page; u8 status; s8 reserved; u16 data_sum; u8 ext_name[4]; u8 game_name[16]; } PfsDir_;
s32 __osCheckId(Pfs_ *);
s32 __osContRamRead(void *, s32, u16, u8 *);
s32 __osPfsGetStatus(void *, s32);



s32 osPfsFindFile(Pfs_ *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no) {
    s32 j, i;
    PfsDir_ dir;
    s32 ret = 0;
    s32 fail;

    if ((pfs->status & 1) == 0) {
        return 5;
    }
    ret = __osCheckId(pfs);
    if (ret != 0) {
        return ret;
    }
    for (j = 0; j < pfs->dir_size; j++) {
        ret = __osContRamRead(pfs->queue, pfs->channel, (u16)(pfs->dir_table + j), (u8 *)&dir);
        if (ret != 0) {
            return ret;
        }
        ret = __osPfsGetStatus(pfs->queue, pfs->channel);
        if (ret != 0) {
            return ret;
        }
        if ((dir.company_code == company_code) && (dir.game_code == game_code)) {
            fail = 0;
            if (game_name != 0) {
                for (i = 0; i < 16; i++) {
                    if (dir.game_name[i] != game_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if ((ext_name != 0) && (fail == 0)) {
                for (i = 0; i < 4; i++) {
                    if (dir.ext_name[i] != ext_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if (fail == 0) {
                *file_no = j;
                return ret;
            }
        }
    }
    *file_no = -1;
    return 5;
}
