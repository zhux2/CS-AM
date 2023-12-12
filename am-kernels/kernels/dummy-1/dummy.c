#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define LED_WR io_write(AM_LIGHT_LED, cur_led, tmp >> cur_led); cur_led++
#define LED_WR2 LED_WR; LED_WR;
#define LED_WR4 LED_WR2 LED_WR2
#define LED_WR8 LED_WR4 LED_WR4
#define LED_WR16 LED_WR8 LED_WR8

#define SEG_WR io_write(AM_LIGHT_SEG, cur_seg, tmp >> (cur_seg << 2)); cur_seg++
#define SEG_WR2 SEG_WR; SEG_WR;
#define SEG_WR4 SEG_WR2 SEG_WR2
#define SEG_WR8 SEG_WR4 SEG_WR4

int main() {
    uint32_t tmp = 0xac79142e;
    int cur_led = 0, cur_seg = 0;
    LED_WR16
    SEG_WR8
    return 0;
}