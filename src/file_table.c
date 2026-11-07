#include "common.h"

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

s32 FileTable_Init(void) {
    memset(&gFileSystem, 0, sizeof(FileSystem));
    gFileSystem.table = gFileTable;
    gFileSystem.count = 201;
    return 1;
}
