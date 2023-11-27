#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  int key = (int)inl(KBD_ADDR);
  int iskeydown = key & KEYDOWN_MASK;
  kbd->keydown = iskeydown;
  kbd->keycode = key ^ iskeydown;
}

void __am_input_sw(AM_INPUT_SW_T *sw_val) {
  sw_val->sw = inl(SW_ADDR);
}
