#include "common.h"

extern void *D_80140F90;
extern void *D_80140F94;
extern void *D_80140F98;
extern void *D_80140F9C;
extern void *D_80140FA8;
extern void *D_80140FAC;
extern void *D_80140FB0;
extern void *D_80140FB4;
extern void *D_80140FB8;
extern void *D_80140FBC;
extern void *D_80140FC0;
extern void *D_80140FC8;
extern void *D_80140FCC;
extern void *D_80140FD0;
extern void *D_80140FD4;
extern void *sysFileLoad;
extern void *sysFileGetSize;
extern void *D_80140FE8;
extern void *D_80140FEC;
extern void *sysSaveRead;
extern void *sysSaveWrite;
extern void *D_80140FFC;
extern void *sysFontLoad;
extern void *sysFontFree;
extern void *sysFontGetCharWidth;
extern void *sysFontDrawChar;
extern void *sysDraw2DSetColor;
extern void *sysScreenSetClearColor;
extern void *D_80141024;
extern void *D_80141028;
extern void *D_8014102C;
extern void *D_80141030;
extern void *sysCameraApply;
extern void *sysCameraSetPerspective;
extern void *D_8014103C;
extern void *D_80141040;
extern void *D_80141044;
extern void *sysTimeGetMillis;
extern void *sysRandRange;
extern void *sysAudioGetMusicVolume;
extern void *sysAudioSetMusicVolume;
extern void *D_80141058;
extern void *D_8014105C;
extern void *D_80141060;
extern void *D_80141064;
extern void *D_80141068;
extern void *D_80141074;
extern void *D_80141078;
extern void *D_8014107C;
extern void *sysIsPAL;
extern void *sysScreenAllocBg;
extern void *sysScreenFreeBg;
extern s32 osTvType; /* osTvType */

void func_8006681C();
void func_8006695C();
void func_80065514();
void func_80065584();
void func_80066B94();
void func_80066C64();
void func_80067950();
void func_80068270();
void File_Load();
void File_GetSize();
void func_8006E578();
void func_8006E8A4();
void Save_Read();
void Save_Write();
void func_8006EDD4();
void Font_Load();
void Font_Free();
void Font_GetCharWidth();
void Font_DrawChar2();
void Draw2D_SetColor();
void Screen_SetClearColor();
void func_8007815C();
void func_80078238();
void func_80078300();
void Camera_SetPerspective();
void Camera_Apply();
void Screen_AllocBg();
void Screen_FreeBg();
void func_8007BE54();
void func_8007BEBC();
void func_8007BF20();
void Time_GetMillis();
void Rand_Range();
void func_800609C8();
void func_80060AFC();
void func_80060B6C();
void Audio_GetMusicVolume();
void Audio_SetMusicVolume();
void func_80064798();
void func_8006476C();
void func_80064824();
void func_800647F8();
void func_8006185C();
void func_80064884();
u8 func_80078388(void);

s32 func_8007EFF8(void);
s32 func_8007F000(void);
s32 func_8007F008(void);
s32 func_8007F028(void);
s32 func_8007F030(void);
s32 func_8007F038(void);
s32 System_IsPAL(void);

s32 func_8007EBF0(void) {
    return 1;
}

    /* TODO (Clean version):
    Populating a massive list of global `void*` variables with raw function addresses 
    bypasses type safety. A clean implementation should use a properly typed 
    plugin interface struct or a static dispatch table with explicit function signatures:
    typedef struct {
        void (*initAudio)(void);
        void (*updateInput)(void);
        // ...
    } SystemInterface;
    */
s32 Sys_InitTable(void) {
    D_80140FBC = &func_8006681C;
    D_80140FC0 = &func_8006695C;
    D_80140FC8 = &func_80065514;
    D_80140FCC = &func_80065584;
    D_80140FB4 = &func_80066B94;
    D_80140FD0 = &func_80066C64;
    D_80140FD4 = &func_80067950;
    D_80140FB8 = &func_80068270;
    sysFileLoad = &File_Load;
    sysFileGetSize = &File_GetSize;
    D_80140FE8 = &func_8006E578;
    D_80140FEC = &func_8006E8A4;
    sysSaveRead = &Save_Read;
    sysSaveWrite = &Save_Write;
    D_80140FFC = &func_8006EDD4;
    sysFontLoad = &Font_Load;
    sysFontFree = &Font_Free;
    sysFontGetCharWidth = &Font_GetCharWidth;
    sysFontDrawChar = &Font_DrawChar2;
    sysDraw2DSetColor = &Draw2D_SetColor;
    sysScreenSetClearColor = &Screen_SetClearColor;
    D_80141024 = &func_8007815C;
    D_80141028 = &func_80078238;
    D_8014102C = &func_80078300;
    D_80141030 = &func_8007F008;
    sysCameraSetPerspective = &Camera_SetPerspective;
    sysCameraApply = &Camera_Apply;
    sysIsPAL = &System_IsPAL;
    sysScreenAllocBg = &Screen_AllocBg;
    sysScreenFreeBg = &Screen_FreeBg;
    D_8014103C = &func_8007BE54;
    D_80141040 = &func_8007BEBC;
    D_80141044 = &func_8007BF20;
    sysTimeGetMillis = &Time_GetMillis;
    sysRandRange = &Rand_Range;
    D_80140F90 = &func_8007EFF8;
    D_80140F94 = &func_8007F000;
    D_80140F98 = &func_800609C8;
    D_80140F9C = &func_80060AFC;
    D_80140FA8 = &func_80060B6C;
    sysAudioGetMusicVolume = &Audio_GetMusicVolume;
    sysAudioSetMusicVolume = &Audio_SetMusicVolume;
    D_80141058 = &func_80064798;
    D_8014105C = &func_8006476C;
    D_80141060 = &func_80064824;
    D_80141064 = &func_800647F8;
    D_80141074 = &func_8007F028;
    D_80141078 = &func_8007F030;
    D_8014107C = &func_8007F038;
    D_80140FAC = &func_8006185C;
    D_80141068 = &func_80064884;
    return 1;
}

    /* TODO (Clean version):
    Manually zeroing out dozens of individual global function pointers during teardown 
    is error-prone and verbose. This should be handled by resetting a registered 
    subsystem interface instance (e.g., `memset(&sysInterface, 0, sizeof(sysInterface))`).
    */
s32 Sys_ClearTable(void) {
    D_80140FB0 = 0;
    D_80140FB4 = 0;
    D_80140FD0 = 0;
    sysFileLoad = 0;
    sysFileGetSize = 0;
    sysIsPAL = 0;
    sysScreenSetClearColor = 0;
    D_80141028 = 0;
    D_8014102C = 0;
    D_80141030 = 0;
    sysCameraApply = 0;
    sysCameraSetPerspective = 0;
    D_8014103C = 0;
    D_80141040 = 0;
    D_80141044 = 0;
    sysTimeGetMillis = 0;
    sysRandRange = 0;
    D_80140F98 = 0;
    D_80140F9C = 0;
    D_80140FA8 = 0;
    D_80141074 = 0;
    D_80141078 = 0;
    D_8014107C = 0;
    D_80141068 = 0;
    return 1;
}

s32 func_8007EFF8(void) {
    return 1;
}

s32 func_8007F000(void) {
    return 1;
}

s32 func_8007F008(void) {
    return func_80078388();
}

s32 func_8007F028(void) {
    return 1;
}

s32 func_8007F030(void) {
    return 1;
}

s32 func_8007F038(void) {
    return 1;
}

    /* TODO (Clean version):
    Comparing `osTvType` against raw `0` should use the established TV type macro 
    (`OS_TV_PAL`) for clarity and consistency across modules:
    `return D_0000300 == OS_TV_PAL;`
    */
s32 System_IsPAL(void) {
    return osTvType == 0;
}
