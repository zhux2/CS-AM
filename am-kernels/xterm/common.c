#include <am.h>
#include <klib.h>
#include <klib-macros.h>

/********************************  Initialization ***************************************/
#define NR_INIT 2
char *init[NR_INIT] = {
    [0] = "------Xterminal----------------------------------[221240095]----[wzr]-----------",
    [1] = "Type [help] to learn more.                                                      "
};

/********************************  LOGO  ************************************************/
#define NR_LOGO 6
const char *logo[NR_LOGO] = {
    [0] = "     www    www    www         zzzzzzzzzzzz         RRRRRRR\n",
    [1] = "      www  ww ww  www                  zzz          RR    RR\n",
    [2] = "      www  ww ww  www                zzz            RR    RR\n",
    [3] = "       www w   w www               zzz              RRRRRRR\n",
    [4] = "        wwww   wwww              zzz                RR    RR\n",
    [5] = "         www   www             zzzzzzzzzzzz         RR     RRR\n"
};