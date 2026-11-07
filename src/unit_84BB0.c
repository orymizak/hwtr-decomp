#include "common.h"
#include "include_asm.h"



s32 __osAiDeviceBusy(void) {
  int new_var;
  new_var = 0;
  return (*((volatile s32 *) 0xA450000C)) < new_var;
}
