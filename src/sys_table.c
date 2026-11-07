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
extern void *D_80140FE0;
extern void *D_80140FE4;
extern void *D_80140FE8;
extern void *D_80140FEC;
extern void *D_80140FF0;
extern void *D_80140FF4;
extern void *D_80140FFC;
extern void *D_80141000;
extern void *D_80141004;
extern void *D_80141008;
extern void *D_8014100C;
extern void *D_80141010;
extern void *D_80141020;
extern void *D_80141024;
extern void *D_80141028;
extern void *D_8014102C;
extern void *D_80141030;
extern void *D_80141034;
extern void *D_80141038;
extern void *D_8014103C;
extern void *D_80141040;
extern void *D_80141044;
extern void *D_80141048;
extern void *D_8014104C;
extern void *D_80141050;
extern void *D_80141054;
extern void *D_80141058;
extern void *D_8014105C;
extern void *D_80141060;
extern void *D_80141064;
extern void *D_80141068;
extern void *D_80141074;
extern void *D_80141078;
extern void *D_8014107C;
extern void *D_80141080;
extern void *D_80141084;
extern void *D_80141088;
extern s32 D_80000300; /* osTvType */

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
void func_8006F218();
void func_8006F34C();
void func_8006F3F0();
void func_8006F714();
void func_80076BB0();
void func_80078140();
void func_8007815C();
void func_80078238();
void func_80078300();
void func_800787EC();
void func_80078684();
void func_800781BC();
void func_800781FC();
void func_8007BE54();
void func_8007BEBC();
void func_8007BF20();
void func_8007F448();
void func_8007F498();
void func_800609C8();
void func_80060AFC();
void func_80060B6C();
void func_8006470C();
void func_80064684();
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
s32 func_8007F040(void);

s32 func_8007EBF0(void) {
    return 1;
}

s32 func_8007EBF8(void) {
    D_80140FBC = &func_8006681C;
    D_80140FC0 = &func_8006695C;
    D_80140FC8 = &func_80065514;
    D_80140FCC = &func_80065584;
    D_80140FB4 = &func_80066B94;
    D_80140FD0 = &func_80066C64;
    D_80140FD4 = &func_80067950;
    D_80140FB8 = &func_80068270;
    D_80140FE0 = &File_Load;
    D_80140FE4 = &File_GetSize;
    D_80140FE8 = &func_8006E578;
    D_80140FEC = &func_8006E8A4;
    D_80140FF0 = &Save_Read;
    D_80140FF4 = &Save_Write;
    D_80140FFC = &func_8006EDD4;
    D_80141000 = &func_8006F218;
    D_80141004 = &func_8006F34C;
    D_80141008 = &func_8006F3F0;
    D_8014100C = &func_8006F714;
    D_80141010 = &func_80076BB0;
    D_80141020 = &func_80078140;
    D_80141024 = &func_8007815C;
    D_80141028 = &func_80078238;
    D_8014102C = &func_80078300;
    D_80141030 = &func_8007F008;
    D_80141038 = &func_800787EC;
    D_80141034 = &func_80078684;
    D_80141080 = &func_8007F040;
    D_80141084 = &func_800781BC;
    D_80141088 = &func_800781FC;
    D_8014103C = &func_8007BE54;
    D_80141040 = &func_8007BEBC;
    D_80141044 = &func_8007BF20;
    D_80141048 = &func_8007F448;
    D_8014104C = &func_8007F498;
    D_80140F90 = &func_8007EFF8;
    D_80140F94 = &func_8007F000;
    D_80140F98 = &func_800609C8;
    D_80140F9C = &func_80060AFC;
    D_80140FA8 = &func_80060B6C;
    D_80141050 = &func_8006470C;
    D_80141054 = &func_80064684;
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

s32 func_8007EF30(void) {
    D_80140FB0 = 0;
    D_80140FB4 = 0;
    D_80140FD0 = 0;
    D_80140FE0 = 0;
    D_80140FE4 = 0;
    D_80141080 = 0;
    D_80141020 = 0;
    D_80141028 = 0;
    D_8014102C = 0;
    D_80141030 = 0;
    D_80141034 = 0;
    D_80141038 = 0;
    D_8014103C = 0;
    D_80141040 = 0;
    D_80141044 = 0;
    D_80141048 = 0;
    D_8014104C = 0;
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

s32 func_8007F040(void) {
    return D_80000300 == 0;
}
