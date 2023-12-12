#include "common.h"

#define NR_SPACE 80
static char space[NR_SPACE];
void init_txt() {
    RESET_COL;
    for (int y = 0; y < NR_INIT; y++)
        io_write(AM_TXT_FBDRAW, 0, y, init[y], TXT_H);
    for (int y = NR_INIT; y < TXT_V; y++)
        io_write(AM_TXT_FBDRAW, 0, y, space, TXT_H);
    set_cursor_position(0, NR_INIT);
    PUT_PROMPT;
}

int cmd_hello(char *arg) {
    printf("Hello World!");
    return 0;
}

int cmd_logo(char *arg) {
    PUT_LOGO;
    return 0;
}

int cmd_clear(char *arg) {
    init_txt();
    return 0;
}

int cmd_fib(char *arg) {
    int n;
    arg = strtok(arg, ' ', &n);
    if (*arg == '\0') {
        printf("Need argument.\n"); 
        return -1; 
    }
    n = atoi(arg);

    int f0 = 0, f1 = 1, tmp;
    for (int i = 1; i < n; ++i) {
        tmp = f1;
        f1 += f0;
        f0 = tmp;
    }
    printf("fib[%d] = %d", n, f1);
    return 0;
}

#define MAX_NR_A 40
static int a[MAX_NR_A];

#define SWAP_INT(x, y) {int tmp = x; x = y; y = tmp; }

static void quick_sort(int l, int r) {
    if (l < r) {
        int x = a[r];
        int i = l - 1;
        for (int j = l; j < r; ++j) {
            if (a[j] <= x) {
                i++;
                SWAP_INT(a[i], a[j]);
            }
        }
        SWAP_INT(a[i+1], a[r]);
        x = i + 1;
        quick_sort(l, x - 1);
        quick_sort(x + 1, r);
    }
}

int cmd_sort(char *arg) {
    int nr_a = 0;
    int arg_len;
    while (*arg) {
        a[nr_a++] = atoi(arg);
        strtok(arg, ' ', &arg_len);
        arg += arg_len;
    }
    quick_sort(0, nr_a - 1);
    for (int i = 0; i < nr_a; ++i) 
        printf("%d ", a[i]);
    return 0;
}

int cmd_dry(char *arg) {
    dry_main();
    return 0;
}

int cmd_led(char *arg) {
    int sec = io_read(AM_TIMER_UPTIME).us / 524288;
    for (int i = 15; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            if (j) io_write(AM_LIGHT_LED, j - 1, 0);
            io_write(AM_LIGHT_LED, j, 1);
            while (io_read(AM_TIMER_UPTIME).us / 524288 < sec);
            sec++;
        }
    }
    while (io_read(AM_TIMER_UPTIME).us / 524288 < sec);
    for (int i = 0; i < 16; ++i) 
        io_write(AM_LIGHT_LED, i, 0);
    return 0;
}

int cmd_seg(char *arg) {
    int n;
    arg = strtok(arg, ' ', &n);
    if (*arg == '\0') {
        printf("Need argument.\n"); 
        return -1; 
    }
    n = atoi(arg);

    int segid = 0;
    while (n && segid < 8) {
        io_write(AM_LIGHT_SEG, segid, modu(n, 10));
        n = divu(n, 10);
    }
    return 0;
}

int cmd_sw(char *arg) {
    printf("sw = %d", io_read(AM_INPUT_SW).sw);
    return 0;
}

int cmd_help(char *);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *); //a function pointer named handler
} cmd_table [] = {
    { "help" , "Display information about all supported commands.", cmd_help },
    { "hello", "Print [Hello World!].", cmd_hello },
    { "logo" , "Display logo.", cmd_logo},
    { "clear", "Clear the screen.", cmd_clear },
    { "fib"  , "Compute the n th fibonacci number.", cmd_fib },
    { "sort" , "Sort the numbers.", cmd_sort},
    { "dry"  , "Run bench-mark DHRYSTONE.", cmd_dry},
    { "led"  , "A led light show.", cmd_led},
    { "seg"  , "Display number on segment LED.", cmd_seg},
    { "sw"   , "Read switches.", cmd_sw},
};

#define NR_CMD (int)(sizeof(cmd_table) / sizeof(cmd_table[0]))

int cmd_help(char *arg) {
    for (int i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
    return 0;
}

void cmd_main(char *arg) {
    int len;
    char *cmd = strtok(arg, ' ', &len);
    arg += len;
    
    int i;
    for (i = 0; i < NR_CMD; ++i) {
        if (strcmp(cmd, cmd_table[i].name) == 0) {
            if (cmd_table[i].handler(arg) < 0) ERROR("error");
            break;
        } 
    }
    if (i == NR_CMD) 
        printf("Unknown command '%s'", cmd);
}