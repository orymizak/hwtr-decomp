#include "common.h"

#define FRAMEBUFFER ((s32 *)0x80300000)
#define FRAMEBUFFER_WORDS 0x25800

#define OS_TV_PAL 0
#define OS_TV_NTSC 1
#define OS_TV_MPAL 2

#define VI_MODE_SIZE 0x50

typedef struct {
    /* 0x00 */ u8 data[VI_MODE_SIZE];
} ViMode;

extern s32 D_80000300; /* osTvType */
extern ViMode D_800FB920[];
extern s32 D_801380B0;
extern s32 D_800F2190;

void *osViGetCurrentFramebuffer(void);
void osViSwapBuffer(void *framebuffer);
u32 osViGetCurrentMode(void);
void osViSetMode(ViMode *mode);
void osViBlack(u8 active);
s32 FileTable_Init(void);
void Memory_InitMain(void);
void func_80060C30(void);
void func_80060C50(void);
void func_80065450(void);
void func_8006B2A0(void);
void func_8006C468(void);
void func_8006DA88(void);
void func_8006DC90(void);
void func_8006F1E0(void);
void func_80072018(void);
void func_80077FB0(void);
void func_80077FD0(void);
void func_8007BE30(void);
void func_8007F420(void);
void func_80087720(void);
void func_8008777C(void);
void func_800D5F30(void (*callback)(s32));
void func_800D5F70(void (*callback)(void));
void func_800D61C0(void);
void func_800D61F0(void);

void func_80078BA4(s32 arg0);
void func_80078BD0(void);

inline void Video_Init(void) {
    u32 i;

    for (i = 0; i < FRAMEBUFFER_WORDS; i++) {
        FRAMEBUFFER[i] = 0;
    }
    osViGetCurrentFramebuffer();
    osViSwapBuffer(FRAMEBUFFER);
    osViSwapBuffer(FRAMEBUFFER);
    osViGetCurrentMode();
    osViSetMode(&D_800FB920[D_801380B0]);
    osViBlack(1);
}

void Game_Init(void) {
    func_80060C30();
    switch (D_80000300) {
        case OS_TV_NTSC:
            D_801380B0 = 0;
            break;
        case OS_TV_PAL:
            D_801380B0 = 14;
            break;
        case OS_TV_MPAL:
            D_801380B0 = 28;
            break;
    }
    Video_Init();
    func_8006B2A0();
    func_80077FB0();
    func_8007F420();
    Memory_InitMain();
    func_80072018();
    FileTable_Init();
    func_80060C50();
    func_80065450();
    func_8006F1E0();
    func_8007BE30();
    func_80077FD0();
    func_800D5F70(func_80078BD0);
    func_80087720();
    func_8006DA88();
    func_8006DC90();
    func_800D5F30(func_80078BA4);
    func_800D61F0();
}

void func_80078BA4(s32 arg0) {
    if (arg0 < D_800F2190) {
        func_8008777C();
    }
}

void func_80078BD0(void) {
    func_8006C468();
    func_800D61C0();
}
