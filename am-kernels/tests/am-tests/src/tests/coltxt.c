#include <amtest.h>

#define WRITE_LINE(x) io_write(AM_TXT_FBDRAW, 0, x, str, len)
void coltxt() {
    char str[] = "Hello World from AM";
    int len = sizeof(str) - 1;
    SET_COL(CYAN, WHITE);
    WRITE_LINE(0);
    SET_COL(BLUE, YELLOW);
    WRITE_LINE(1);
    SET_COL(MAGENTA, BLACK);
    WRITE_LINE(2);
    SET_COL(GREEN, WHITE);
    WRITE_LINE(3);
    SET_COL(RED, YELLOW);
    WRITE_LINE(4);
}