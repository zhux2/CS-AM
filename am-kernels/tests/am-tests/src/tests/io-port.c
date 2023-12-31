#include <amtest.h>

char njuid[9] = {2, 2, 1, 2, 4, 0, 0, 9, 5};

void board_io_port() {
    printf("Look at development board.");
    int pre_led = NR_LED - 1, cur_led = 0;
    int cur_seg = 0;
    int sec = 1;
    while (1) {
        //write led
        io_write(AM_LIGHT_LED, cur_led, 1);
        io_write(AM_LIGHT_LED, pre_led, 0);
        //write seg
        io_write(AM_LIGHT_SEG, 7, njuid[cur_seg]);
        io_write(AM_LIGHT_SEG, 6, njuid[modu(cur_seg + 1, 9)]);
        io_write(AM_LIGHT_SEG, 5, njuid[modu(cur_seg + 2, 9)]);
        io_write(AM_LIGHT_SEG, 4, njuid[modu(cur_seg + 3, 9)]);
        io_write(AM_LIGHT_SEG, 3, njuid[modu(cur_seg + 4, 9)]);
        io_write(AM_LIGHT_SEG, 2, njuid[modu(cur_seg + 5, 9)]);
        io_write(AM_LIGHT_SEG, 1, njuid[modu(cur_seg + 6, 9)]);
        io_write(AM_LIGHT_SEG, 0, njuid[modu(cur_seg + 7, 9)]);
        //update
        pre_led = cur_led;
        if (cur_led == NR_LED - 1) { cur_led = 0; }
        else { cur_led++; }
        if (cur_seg == sizeof(njuid) - 1) { cur_seg = 0; }
        else { cur_seg++; }
        while(io_read(AM_TIMER_UPTIME).us / 1000000 < sec) ;
        sec++;
    }
}