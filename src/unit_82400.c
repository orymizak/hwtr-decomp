#include "common.h"
#include "include_asm.h"
#include "PR/os_internal.h"
#include "PRinternal/controller.h"
#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PRinternal/controller.h"
#include "PRinternal/siint.h"

static s32 __osPfsCheckRamArea(OSPfs* pfs);


s32 osPfsNumFiles(OSPfs* pfs, s32* max_files, s32* files_used) {
    int j;
    s32 ret;
    __OSDir dir;
    int files = 0;

    PFS_CHECK_STATUS();
    ERRCK(__osCheckId(pfs));
    SET_ACTIVEBANK_TO_ZERO();

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, pfs->dir_table + j, (u8*)&dir));

        if (dir.company_code != 0 && dir.game_code != 0) {
            files++;
        }
    }
    *files_used = files;
    *max_files = pfs->dir_size;

    ret = __osPfsGetStatus(pfs->queue, pfs->channel);
    return ret;
}



s32 osPfsInitPak(OSMesgQueue* queue, OSPfs* pfs, int channel) {
    s32 ret = 0;
    u16 sum;
    u16 isum;
    u8 temp[BLOCKSIZE];
    __OSPackId* id;
    __OSPackId newid;

    __osSiGetAccess();

    ret = __osPfsGetStatus(queue, channel);

    __osSiRelAccess();

    if (ret != 0) {
        return ret;
    }

    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;

    ERRCK(__osPfsCheckRamArea(pfs));
    ERRCK(SELECT_BANK(pfs, 0));
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, temp));

    __osIdCheckSum((u16*)temp, &sum, &isum);
    id = (__OSPackId*)temp;

    if ((id->checksum != sum) || (id->inverted_checksum != isum)) {
        ret = __osCheckPackId(pfs, id);

        if (ret != 0) {
            pfs->status |= PFS_ID_BROKEN;
            return ret;
        }
        
    }

    if (!(id->deviceid & 1)) {
        ret = __osRepairPackId(pfs, id, &newid);

        if (ret != 0) {
            if (ret == PFS_ERR_ID_FATAL) {
                pfs->status |= PFS_ID_BROKEN;
            }
            return ret;
        }

        id = &newid;

        if (!(id->deviceid & 1)) {
            return PFS_ERR_DEVICE;
        }
    }

    bcopy(id, pfs->id, BLOCKSIZE);

    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE;
    pfs->inode_table = 1 * PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + (pfs->banks * PFS_ONE_PAGE);

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));

    ret = osPfsChecker(pfs);
    pfs->status |= PFS_INITIALIZED;

    return ret;
}


static s32 __osPfsCheckRamArea(OSPfs* pfs) {
    s32 i;
    s32 ret = 0;
    u8 temp1[BLOCKSIZE];
    u8 temp2[BLOCKSIZE];
    u8 save[BLOCKSIZE];

    ERRCK(SELECT_BANK(pfs, PFS_ID_BANK_256K));
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, save));

    for (i = 0; i < BLOCKSIZE; i++) {
        temp1[i] = i;
    }

    ERRCK(__osContRamWrite(pfs->queue, pfs->channel, 0, temp1, FALSE));
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, temp2));

    if (bcmp(temp1, temp2, BLOCKSIZE) != 0) {
        return PFS_ERR_DEVICE;
    }

    ret = __osContRamWrite(pfs->queue, pfs->channel, 0, save, FALSE);
    return ret;
}
