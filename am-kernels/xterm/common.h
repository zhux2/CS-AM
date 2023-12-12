#include <am.h>
#include <klib.h>
#include <klib-macros.h>

/********************************  Initialization ***************************************/
#define NR_INIT 2
extern char *init[NR_INIT];

void init_txt();
#define GLOBAL_INIT_TXT vga_set_txt(); init_txt()

/********************************  LOGO  ************************************************/
#define NR_LOGO 6
extern const char *logo[NR_LOGO];
#define PUT_LOGO do {\
    SET_COL(GREEN, BLACK);\
    for (int i = 0; i < NR_LOGO; ++i)\
        putstr(logo[i]);\
    RESET_COL;\
    } while(0)

/********************************  prompt  **********************************************/
#define prompt "wzr@nju-cpu$ "
#define PUT_PROMPT SET_COL(CYAN, BLACK); printf(prompt); RESET_COL


/********************************  tools  ***********************************************/
#define CLEAR_CURSOR io_write(AM_TXT_FBDRAW, txt_x, txt_y, " ", 1)
#define NEW_LINE { CLEAR_CURSOR; txt_x = 0; txt_y++; }

#define ERROR(fmt, ...) { SET_COL(RED, BLACK); printf(fmt, ## __VA_ARGS__); RESET_COL; }

#define BACKSPACE_HANDLER do {\
    CLEAR_CURSOR;\
    if (txt_x == 0) { txt_x = TXT_H; txt_y--; }\
    else { txt_x--; }\
    io_write(AM_TXT_FBDRAW, txt_x, txt_y, " ", 1);\
} while(0)

/********************************  cmd_handler  *****************************************/
void cmd_main(char *arg);

/********************************  cursor  **********************************************/
void cursor_blink();

/********************************  benchmark  *******************************************/
int  dry_main ();