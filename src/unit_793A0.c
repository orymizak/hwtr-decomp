#include "common.h"
#include "include_asm.h"
s32 osPiGetCmdQueue(void);
extern s32 D_800FB2A0;



s32 osPiStartDma(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
  s16 var_v0;
  s32 *new_var;
  if (D_800FB2A0 == 0) {
    return -1;
  }
  var_v0 = 0xC;
  if (arg2 == 0) {
    var_v0 = 0xB;
  }
  *((s16 *) (((u8 *) arg0) + 0x0)) = var_v0;
  *((u8 *) (((u8 *) arg0) + 0x2)) = (s8) arg1;
  new_var = (s32 *) (((u8 *) arg0) - -0x4);
  *new_var = arg6;
  *((s32 *) (((u8 *) arg0) + 0x8)) = arg4;
  *((s32 *) (((u8 *) arg0) + 0xC)) = arg3;
  *((s32 *) (((u8 *) arg0) + 0x10)) = arg5;
  *((s32 *) (((u8 *) arg0) + 0x14)) = 0;
  if (arg1 == 1) {
    return osJamMesg(osPiGetCmdQueue(), (s32) arg0, 0);
  }
  if (arg3 || arg0) {
    return osSendMesg(osPiGetCmdQueue(), (s32) arg0, 0);
  }
  else
  {
    return osSendMesg(osPiGetCmdQueue(), (s32) arg0, 0);
  }
}
