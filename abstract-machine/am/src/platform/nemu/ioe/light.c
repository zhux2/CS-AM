#include <am.h>
#include <nemu.h>

void __am_light_led(AM_LIGHT_LED_T *led) {
    outb(LED_ADDR + led->x, led->data);
}

void __am_light_seg(AM_LIGHT_SEG_T *seg) {
    outb(SEG_ADDR + seg->x, seg->data);
}