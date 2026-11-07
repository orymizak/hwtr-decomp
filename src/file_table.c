#include "common.h"
#include "include_asm.h"

typedef enum {
    FILE_TYPE_RAW,
    FILE_TYPE_LZ,
    FILE_TYPE_LZ_CHUNKED
} FileType;

typedef struct {
    /* 0x00 */ char *name;
    /* 0x04 */ u8 type;
    /* 0x08 */ u32 storedSize;
    /* 0x0C */ u32 size;
    /* 0x10 */ u32 firstChunkSize;
    /* 0x14 */ u32 romStart;
    /* 0x18 */ u32 romEnd;
} FileEntry; /* size = 0x1C */

typedef struct {
    /* 0x00 */ u8 unk00[8];
    /* 0x08 */ s32 status;
    /* 0x0C */ u8 unk0C[4];
} PakHandle; /* size = 0x10 */

typedef struct {
    /* 0x00 */ FileEntry *table;
    /* 0x04 */ s32 count;
    /* 0x08 */ PakHandle *handles[4];
    /* 0x18 */ u8 initialized[4];
} FileSystem; /* size = 0x1C */

typedef struct {
    /* 0x00 */ u8 data[0xDA0];
} SaveBlock; /* size = 0xDA0 */

typedef struct {
    /* 0x00 */ u8 ioMesg[0x18];
    /* 0x18 */ u8 queue[0x18];
    /* 0x30 */ void *msg;
    /* 0x34 */ u8 unk34[4];
    /* 0x38 */ void *buffer;
    /* 0x3C */ void *dst;
    /* 0x40 */ u8 unk40[4];
    /* 0x44 */ u8 active;
    /* 0x45 */ u8 type;
} AsyncRead; /* size = 0x48 */

extern FileEntry gFileTable[];
extern FileSystem gFileSystem;
extern AsyncRead D_800F1C08[2];
extern u8 D_800F1C98[];
extern char D_800FF710[];
extern char D_800FF720[];
extern u8 D_80102220[];

s32 File_LoadLZ(s32 index, void *dst);
s32 File_LoadLZChunked(s32 index, u8 *dst);
s32 File_LoadLZChunkedAsync();
void *func_80079024(u32 size);
void func_800790C4(void *ptr);
void func_800D6240(u32 romAddr, void *dst, u32 size);
void func_800D6770(PakHandle *handle);
void func_800D67A0(PakHandle *handle, char *name, char *ext, s32 create, s32 size);
void func_800D68D0(PakHandle *handle, s32 offset, s32 size, void *buffer, s32 write);
void osInvalDCache(void *addr, u32 size);
void osCreateMesgQueue(void *queue, void *msgs, s32 count);
s32 osRecvMesg(void *queue, void *msg, s32 flag);
void osPiStartDma(void *mesg, s32 pri, s32 direction, u32 romAddr, void *dst, u32 size, void *queue);
s32 func_8006E578(s32 port);
s32 File_LoadLZAsync(s32 index, void *dst, s32 slot);
s32 File_FinishLZAsync(s32 slot);
void Rom_ReadAsync(u32 romAddr, void *dst, u32 size, AsyncRead *read);
u8 Rom_PollAsync(AsyncRead *read);
s32 LZ_Decompress(u8 *src, u8 *dst);
s32 strcasecmp(const char *a, const char *b);

s32 FileTable_Init(void) {
    memset(&gFileSystem, 0, sizeof(FileSystem));
    gFileSystem.table = gFileTable;
    gFileSystem.count = 201;
    return 1;
}

s32 File_Load(char *name, void *dst) {
    s32 high = gFileSystem.count;
    s32 low = 0;
    s32 mid = 0;
    s32 cmp = -1;
    u32 size;

    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcasecmp(name, gFileSystem.table[mid].name);
        if (cmp == 0) {
            break;
        }
        if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (cmp != 0) {
        return 0;
    }

    switch (gFileSystem.table[mid].type) {
        case FILE_TYPE_LZ_CHUNKED:
            File_LoadLZChunked(mid, dst);
            break;
        case FILE_TYPE_LZ:
            File_LoadLZ(mid, dst);
            break;
        case FILE_TYPE_RAW:
        default:
            size = gFileSystem.table[mid].size;
            if (size & 1) {
                func_800D6240(gFileSystem.table[mid].romStart, dst, size + 1);
            } else {
                func_800D6240(gFileSystem.table[mid].romStart, dst, size);
            }
            break;
    }
    return gFileSystem.table[mid].size;
}

s32 File_LoadAsync(char *name, void *dst) {
    s32 high;
    s32 low;
    s32 mid;
    s32 cmp;
    s32 slot;
    u32 size;

    low = 0;
    high = gFileSystem.count;
    mid = 0;
    cmp = -1;
    slot = 0;

    if (D_800F1C08[0].active) {
        if (D_800F1C08[1].active) {
            return -1;
        }
        slot = 1;
    }
    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcasecmp(name, gFileSystem.table[mid].name);
        if (cmp == 0) {
            break;
        }
        if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (cmp != 0) {
        return 0;
    }

    D_800F1C08[slot].type = gFileSystem.table[mid].type;
    D_800F1C08[slot].active = 1;
    switch (gFileSystem.table[mid].type) {
        case FILE_TYPE_LZ_CHUNKED:
            File_LoadLZChunkedAsync(mid, dst, slot);
            break;
        case FILE_TYPE_LZ:
            File_LoadLZAsync(mid, dst, slot);
            break;
        case FILE_TYPE_RAW:
            size = gFileSystem.table[mid].size;
            if (size & 1) {
                Rom_ReadAsync(gFileSystem.table[mid].romStart, dst, size + 1, &D_800F1C08[slot]);
            } else {
                Rom_ReadAsync(gFileSystem.table[mid].romStart, dst, size, &D_800F1C08[slot]);
            }
            break;
    }
    return slot;
}

INCLUDE_ASM("asm/nonmatchings/file_table", File_PollAsync);

s32 File_GetSize(char *name) {
    s32 high = gFileSystem.count;
    s32 low = 0;
    s32 mid = 0;
    s32 cmp = -1;

    while (low <= high) {
        mid = (low + high) / 2;
        cmp = strcasecmp(name, gFileSystem.table[mid].name);
        if (cmp == 0) {
            break;
        }
        if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (cmp != 0) {
        return 0;
    }
    return gFileSystem.table[mid].size;
}

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E578);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E8A4);

u8 Save_IsChecksumValid(SaveBlock *block) {
    s32 sum = 0;
    u8 *p = block->data;
    u16 i = 0;
    u32 size = 0xD90;

    for (; i < size; i++) {
        sum += *p++;
    }
    return *(s32 *)&block->data[0xD90] == sum;
}

s32 Save_Read(s32 port, SaveBlock *dst) {
    SaveBlock *blocks[2];
    u8 errors[2];
    PakHandle *handle;

    blocks[0] = func_80079024(sizeof(SaveBlock));
    blocks[1] = func_80079024(sizeof(SaveBlock));
    D_800F1C98[port] = 0;
    handle = gFileSystem.handles[port];
    if (handle != NULL && gFileSystem.initialized[port]) {
        func_800D68D0(handle, 0, sizeof(SaveBlock), blocks[0], 0);
        errors[0] = handle->status != 0;
        func_800D68D0(handle, sizeof(SaveBlock), sizeof(SaveBlock), blocks[1], 0);
        errors[1] = handle->status != 0;
        if (Save_IsChecksumValid(blocks[0]) && !errors[0]) {
            goto first_valid;
        }
        if (Save_IsChecksumValid(blocks[1]) && !errors[1]) {
            goto second_only;
        }
    }
    func_800790C4(blocks[0]);
    func_800790C4(blocks[1]);
    return 0;

second_only:
    func_800D68D0(handle, 0, sizeof(SaveBlock), blocks[1], 1);
    *dst = *blocks[1];
    goto done;

first_valid:
    if (!Save_IsChecksumValid(blocks[1]) || errors[1]) {
        func_800D68D0(handle, sizeof(SaveBlock), sizeof(SaveBlock), blocks[0], 1);
        *dst = *blocks[0];
    } else {
        *dst = *blocks[0];
    }

done:
    func_800790C4(blocks[0]);
    func_800790C4(blocks[1]);
    return 1;
}

s32 Save_Write(s32 port, SaveBlock *src) {
    PakHandle *handle;

    D_800F1C98[port] = 0;
    handle = gFileSystem.handles[port];
    if (handle == NULL) {
        return 0;
    }
    if (!gFileSystem.initialized[port]) {
        func_800D67A0(handle, D_800FF710, D_800FF720, 1, 0x1B40);
        if (handle->status != 0) {
            return 0;
        }
        gFileSystem.initialized[port] = 1;
    }
    func_800D68D0(handle, 0, sizeof(SaveBlock), src, 1);
    func_800D68D0(handle, sizeof(SaveBlock), sizeof(SaveBlock), src, 1);
    return handle->status == 0;
}

s32 func_8006EDD4(s32 port) {
    PakHandle *handle;

    D_800F1C98[port] = 0;
    handle = gFileSystem.handles[port];
    if (handle == NULL) {
        return 0;
    }
    func_800D6770(handle);
    if (handle->status == 0) {
        return func_8006E578(port) != 10;
    }
    return 0;
}

s32 File_LoadLZ(s32 index, void *dst) {
    void *buffer;
    u32 size;

    buffer = func_80079024(gFileSystem.table[index].storedSize);
    if (buffer == NULL) {
        return 0;
    }
    size = gFileSystem.table[index].storedSize;
    if (size & 1) {
        func_800D6240(gFileSystem.table[index].romStart, buffer, size + 1);
    } else {
        func_800D6240(gFileSystem.table[index].romStart, buffer, size);
    }
    LZ_Decompress(buffer, dst);
    func_800790C4(buffer);
    return 1;
}

s32 File_LoadLZChunked(s32 index, u8 *dst) {
    u8 mesg[0x38];
    u8 *buffer = D_80102220;
    u8 *out = dst;
    u32 offset = 0;
    u32 chunkSize = gFileSystem.table[index].firstChunkSize;

    do {
        Rom_ReadAsync(gFileSystem.table[index].romStart + offset, buffer, chunkSize, (AsyncRead *)mesg);
        while (!Rom_PollAsync((AsyncRead *)mesg)) {
        }
        offset += chunkSize;
        chunkSize = *(u32 *)buffer;
        buffer += 8;
        out += LZ_Decompress(buffer, out);
    } while (chunkSize != 0);
    return 1;
}

s32 File_LoadLZAsync(s32 index, void *dst, s32 slot) {
    void *buffer;
    u32 size;

    buffer = func_80079024(gFileSystem.table[index].storedSize);
    if (buffer == NULL) {
        return 0;
    }
    size = gFileSystem.table[index].storedSize;
    if (size & 1) {
        Rom_ReadAsync(gFileSystem.table[index].romStart, buffer, size + 1, &D_800F1C08[slot]);
    } else {
        Rom_ReadAsync(gFileSystem.table[index].romStart, buffer, size, &D_800F1C08[slot]);
    }
    D_800F1C08[slot].buffer = buffer;
    D_800F1C08[slot].dst = dst;
    return 1;
}

s32 File_FinishLZAsync(s32 slot) {
    LZ_Decompress(D_800F1C08[slot].buffer, D_800F1C08[slot].dst);
    func_800790C4(D_800F1C08[slot].buffer);
    return 1;
}

s32 File_LoadLZChunkedAsync() {
    return 0;
}

s32 func_8006F0EC(void) {
    return 0;
}

s32 func_8006F0F4(void) {
    return 0;
}

void Rom_ReadAsync(u32 romAddr, void *dst, u32 size, AsyncRead *read) {
    osInvalDCache(dst, size);
    osCreateMesgQueue(read->queue, &read->msg, 1);
    osPiStartDma(read, 0, 0, romAddr, dst, size, read->queue);
}

void Rom_WaitAsync(AsyncRead *read) {
    osRecvMesg(read->queue, NULL, 1);
}

u8 Rom_PollAsync(AsyncRead *read) {
    return osRecvMesg(read->queue, NULL, 0) == 0;
}
