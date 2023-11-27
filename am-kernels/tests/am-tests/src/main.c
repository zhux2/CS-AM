#include <amtest.h>

void (*entry)() = NULL; // mp entry

#define NR_TESTS 7
static const char *tests[NR_TESTS] = {
  [0] = "hello",
  [6] = "display this help message",
  [1] = "real-time clock test",
  [2] = "readkey test",
  [3] = "vedio display test",
  [4] = "text color test",
  [5] = "board io-port test",
};

#define NR_SPACE 2560
char space[NR_SPACE];
void init_txt() {
  RESET_COL;
  io_write(AM_TXT_FBDRAW, 0, 0, space, TXT_H * TXT_V);
}

void init_img() {
  for (int y = 0; y < V_Pixels; ++y)
    io_write(AM_GPU_FBDRAW, 0, y, space, H_Pixels, 1, 0);
}

#define INIT_TXT vga_set_txt(); init_txt()
#define INIT_IMG vga_set_img(); init_img()

int main() {
  AM_INPUT_SW_T arg = io_read(AM_INPUT_SW);
  switch (arg.sw) {
    CASE(0, hello, INIT_TXT);
    CASE(1, rtc_test, INIT_TXT);
    CASE(2, keyboard_test, INIT_TXT);
    CASE(3, video_test, INIT_IMG);
    CASE(4, coltxt, INIT_TXT);
    CASE(5, board_io_port, INIT_TXT);
    case 6:
    default:
      printf("Usage: make run mainargs=*\n");
      for (int ch = 0; ch < NR_TESTS; ch++) {
        if (tests[ch]) {
          printf("  %c: %s\n", ch, tests[ch]);
        }
      }
  }
  return 0;
}
