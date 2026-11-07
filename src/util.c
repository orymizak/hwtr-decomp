#include "common.h"
#include "include_asm.h"

typedef struct {
    /* 0x00 */ u8 width;
    /* 0x01 */ u8 height;
    /* 0x02 */ u8 format;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ void *data;
} Texture; /* size = 0x8 */

extern s32 D_800F2190;
extern s32 D_800F2194;
extern s32 D_800F2198;

u32 osGetCount(void);
u64 osGetTime(void);
void srand(u32 seed);
u32 rand(void);
void *Memory_Alloc(u32 size);
void Memory_EnableSecondary(void);
void Memory_DisableSecondary(void);
s32 File_GetSize(char *name);
s32 File_Load(char *name, void *dst);
void GfxPools_Shutdown(void);
void func_80071940(void);
void func_80071A40(void);

s32 func_8007F420(void) {
    srand(osGetCount());
    return 1;
}

u32 func_8007F448(void) {
    u64 time = osGetTime() * 64 / 3000;

    return time / 1000;
}

u32 func_8007F498(u32 max) {
    return rand() % max;
}

s32 func_8007F4D0(void) {
    D_800F2190 = D_800F2194;
    func_80071940();
    Memory_EnableSecondary();
    srand(osGetCount());
    return 1;
}

s32 func_8007F514(void) {
    D_800F2190 = D_800F2198;
    GfxPools_Shutdown();
    Memory_DisableSecondary();
    func_80071A40();
    return 1;
}

s32 func_8007F550(u32 value) {
    s32 log2 = 0;

    value >>= 1;
    while (value != 0) {
        value >>= 1;
        log2++;
    }
    return log2;
}

void func_8007F570(Texture *texture) {
    texture->data = texture + 1;
}

Texture *func_8007F57C(char *name) {
    Texture *texture = Memory_Alloc(File_GetSize(name));

    File_Load(name, texture);
    texture->data = texture + 1;
    return texture;
}

Texture *func_8007F5C4(s32 width, s32 height) {
    Texture *texture = Memory_Alloc(width * height * 2 + sizeof(Texture));

    texture->width = width;
    texture->height = height;
    texture->format = 0;
    texture->unk03 = 0;
    texture->data = texture + 1;
    return texture;
}

INCLUDE_ASM("asm/nonmatchings/util", func_8007F61C);

INCLUDE_ASM("asm/nonmatchings/util", func_8007FB38);
