#include "common.h"
#include "ultra64.h"
#include "include_asm.h"
typedef struct { void *pfs; s32 file_no; s32 error; s32 type; } PakFile73_;
void nuContPakJisToN64(u8 *, void *, s32);
void nuContPakFileOpen(u8 *, void *, void *, s32, s32);
s32 nuSiSendMesg(s32, void *);



void nuContPakRepairId(PakFile73_ *file) {
    file->error = nuSiSendMesg(0x208, file);
}

void nuContPakFileOpenJis(u8 *a0, u8 *a1, u8 *a2, s32 a3, s32 a4) {
    u8 name[0x20];
    u8 ext[4];
    s32 len;
    u8 *p;

    if (*(s32 *)(a0 + 0xC) == 0) {
        *(s32 *)(a0 + 8) = 1;
        return;
    }
    bzero(name, 0x20);
    bzero(ext, 4);
    len = 0;
    if (*a1 != 0) {
        p = a1;
        do {
            p++;
            len++;
        } while (*p != 0);
    }
    nuContPakJisToN64(a1, name, len);
    nuContPakJisToN64(a2, ext, 1);
    nuContPakFileOpen(a0, name, ext, a3, a4);
}
