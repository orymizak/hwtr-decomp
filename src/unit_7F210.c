#include "common.h"
#include "include_asm.h"
s32 __osSpGetStatus(void);



u32 osSpTaskYielded(s32 arg0) {
  u32 temp_v0;
  u32 temp_a0;
  temp_v0 = __osSpGetStatus();
  temp_a0 = (temp_v0 >> 3) >> 5;
  temp_a0 = temp_a0 & 1;
  if (temp_v0 & 0x80) {
    *((s32 *) (((u8 *) arg0) + 0x4)) = (s32) (((*((s32 *) (((u8 *) arg0) + 0x4))) | temp_a0) & (~2));
  }
  return temp_a0;
}
