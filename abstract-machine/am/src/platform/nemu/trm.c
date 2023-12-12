#include <am.h>
#include <nemu.h>

extern char _heap_start;
extern char *malloc_space;
int main(const char *args);

Area heap = RANGE(&_heap_start, PMEM_END);
AddrSpace addrspace = {.pgsize = PGSIZE, .area = RANGE(&_heap_start, PMEM_END), .ptr = &_heap_start};
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;
#define ENTER '\n'
#define SPACE ' '
char serialcol = DEFAULT_COL;
int txt_x = 0;
int txt_y = 0;

void putch(char ch) {
  if (ch == ENTER) {
    txt_x = 0; txt_y++;
    return;
  }

  int off = txt_y * TXT_H + txt_x;
  outb(TXT_ADDR + off, ch);
  outb(TXTCOL_ADDR + off, serialcol);
  txt_x ++;
  if (txt_x >= TXT_H) { 
    txt_y++; txt_x = 0;
  }
}

void vga_set_txt() {
  outb(VGA_CONF_PORT, 0x01); 
}

void vga_set_img() {
  outb(VGA_CONF_PORT, 0x00);
}

void set_cursor_position(int x, int y) {
  txt_x = x;
  txt_y = y;
}

void halt(int code) {
  nemu_trap(code);

  // should not reach here
  while (1);
}

// static void malloc_init() {
//   malloc_space = (char *)ROUNDUP(heap.start, 8);
// }

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}
