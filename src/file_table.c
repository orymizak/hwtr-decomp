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
    /* 0x00 */ FileEntry *table;
    /* 0x04 */ s32 count;
    /* 0x08 */ u8 unk08[0x14];
} FileSystem; /* size = 0x1C */

extern FileEntry gFileTable[];
extern FileSystem gFileSystem;
extern u8 D_80102220[];

s32 File_LoadLZ(s32 index, void *dst);
s32 File_LoadLZChunked(s32 index, u8 *dst);
void *func_80079024(u32 size);
void func_800790C4(void *ptr);
void func_800D6240(u32 romAddr, void *dst, u32 size);
void func_8006F0FC(u32 romAddr, void *dst, u32 size, void *mesg);
u8 func_8006F1AC(void *mesg);
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

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E224);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E3EC);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E4AC);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E578);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E8A4);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E954);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006E990);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006ECF0);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006EDD4);

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
        func_8006F0FC(gFileSystem.table[index].romStart + offset, buffer, chunkSize, mesg);
        while (!func_8006F1AC(mesg)) {
        }
        offset += chunkSize;
        chunkSize = *(u32 *)buffer;
        buffer += 8;
        out += LZ_Decompress(buffer, out);
    } while (chunkSize != 0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006EFB4);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F08C);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F0E4);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F0EC);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F0F4);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F0FC);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F188);

INCLUDE_ASM("asm/nonmatchings/file_table", func_8006F1AC);
