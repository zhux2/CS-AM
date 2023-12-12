#include <amtest.h>

#define NAMEINIT(key)  [ AM_KEY_##key ] = #key,
static const char *names[] = {
  AM_KEYS(NAMEINIT)
};

static bool has_kbd;

static void drain_keys() {

  if (has_kbd) {
    while (1) {
      AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
      if (ev.keycode == AM_KEY_NONE) break;
      printf("Got  (kbd): %s (%d) %s\n", names[ev.keycode], ev.keycode, ev.keydown ? "DOWN" : "UP");
    }
  }
}

void keyboard_test() {
  printf("Try to press any key (uart or keyboard)...\n");
  has_kbd  = 1;
  while (1) {
    drain_keys();
  }
}
