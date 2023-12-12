#include <am.h>
#include <klib.h>
#include <klib-macros.h>

static char cursor_on = 0;

void cursor_blink() {
    if (cursor_on) 
        io_write(AM_TXT_FBDRAW, txt_x, txt_y, " ", 1);
    else 
        io_write(AM_TXT_FBDRAW, txt_x, txt_y, "|", 1);
        
    cursor_on = cursor_on ^ 1;
}