#include <am.h>
#include <nemu.h>

void __am_gpu_init() {
  vga_set_img();
//for testing
  // int i;
  // int w = 32 << 2;  // TODO: get the correct width
  // int h = 32 << 2;  // TODO: get the correct height
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = H_Pixels, .height = V_Pixels,
    .vmemsz = H_Pixels * V_Pixels * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t *rgb_data = (uint32_t *)(ctl->pixels);
  for (int i = ctl->y; i < (ctl->y) + (ctl->h); ++i) {
    for (int j = ctl->x; j < (ctl->x) + (ctl->w); ++j) {
      outl(FB_ADDR + (i * H_Pixels + j), *rgb_data);
      rgb_data++;
    }
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

#define FORWARD(x, y) { x++; if (x >= TXT_H) {x = 0; y++; } }
void __am_txt_fbdraw(AM_TXT_FBDRAW_T *ctl) {
  uint8_t *origin = (uint8_t *)(ctl->str);
  uint8_t *txt_data = origin;
  int off = 0;
  int x = ctl->x, y = ctl->y;
  while (txt_data  - origin < ctl->len) {
    off = y * TXT_H + x;
    outb(TXT_ADDR + off, *txt_data);
    outb(TXTCOL_ADDR + off, serialcol);
    txt_data++;
    FORWARD(x, y);
  }
}