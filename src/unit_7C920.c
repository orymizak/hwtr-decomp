#include "common.h"
#include "include_asm.h"



void func_800DBD20(void *arg0, void *arg1, s32 arg2) {
  s32 var_v1;
  u8 temp_v0;
  int new_var;
  void *var_a0;
  void *var_a1;
  var_a0 = arg0;
  var_a1 = arg1;
  var_v1 = 0;
  new_var = 0;
  if (arg2 > new_var) {
    do
    {
      temp_v0 = *((u8 *) var_a0);
      var_a0 += 1;
      var_v1 += 1;
      *((u8 *) var_a1) = temp_v0;
      var_a1 += 1;
    }
    while (var_v1 < arg2);
  }
}
