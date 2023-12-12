#include "common.h"

static char shift_on = 0;
static char capslock_on = 0;

#define MAX_BUFFER_LEN 60
char cmd_buffer[MAX_BUFFER_LEN];
int  cmd_len = 0;

#define f(key, c)  [ AM_KEY_##key ] = c,
const char key2char[83] = {
f(ESCAPE, 0x1b) f(F1, ' ') f(F2, ' ') f(F3, ' ') f(F4, ' ') f(F5, ' ') f(F6, ' ') f(F7, ' ') f(F8, ' ') f(F9, ' ') f(F10, ' ') f(F11, ' ') f(F12, ' ')
f(GRAVE, '`') f(1, '1') f(2, '2') f(3, '3') f(4, '4') f(5, '5') f(6, '6') f(7, '7') f(8, '8') f(9, '9') f(0, '0') f(MINUS, '-') f(EQUALS, '=') f(BACKSPACE, 0x08)
f(TAB, '\t') f(Q, 'q') f(W, 'w') f(E, 'e') f(R, 'r') f(T, 't') f(Y, 'y') f(U, 'u') f(I, 'i') f(O, 'o') f(P, 'p') f(LEFTBRACKET, '[') f(RIGHTBRACKET, ']') f(BACKSLASH, '\\')
f(CAPSLOCK, ' ') f(A, 'a') f(S, 's') f(D, 'd') f(F, 'f') f(G, 'g') f(H, 'h') f(J, 'j') f(K, 'k') f(L, 'l') f(SEMICOLON, ';') f(APOSTROPHE, 0x27) f(RETURN, 0x0a)
f(LSHIFT,' ') f(Z, 'z') f(X, 'x') f(C, 'c') f(V, 'v') f(B, 'b') f(N, 'n') f(M, 'm') f(COMMA, ',') f(PERIOD, '.') f(SLASH, '/') f(RSHIFT, ' ')
f(LCTRL, ' ') f(APPLICATION, ' ') f(LALT, ' ') f(SPACE, ' ') f(RALT, ' ') f(RCTRL, ' ')
f(UP, ' ') f(DOWN, ' ') f(LEFT, ' ') f(RIGHT, ' ') f(INSERT, ' ') f(DELETE, 0x7f) f(HOME, 0x01) f(END, 0x02) f(PAGEUP, ' ') f(PAGEDOWN, ' ')
};
const char key2cap[83] = {
f(ESCAPE, 0x1b) f(F1, ' ') f(F2, ' ') f(F3, ' ') f(F4, ' ') f(F5, ' ') f(F6, ' ') f(F7, ' ') f(F8, ' ') f(F9, ' ') f(F10, ' ') f(F11, ' ') f(F12, ' ')
f(GRAVE, '~') f(1, '!') f(2, '@') f(3, '#') f(4, '$') f(5, '%') f(6, '^') f(7, '&') f(8, '*') f(9, '(') f(0, ')') f(MINUS, '_') f(EQUALS, '+') f(BACKSPACE, 0x08)
f(TAB, '\t') f(Q, 'Q') f(W, 'W') f(E, 'E') f(R, 'R') f(T, 'T') f(Y, 'Y') f(U, 'U') f(I, 'I') f(O, 'O') f(P, 'P') f(LEFTBRACKET, '{') f(RIGHTBRACKET, '}') f(BACKSLASH, '|')
f(CAPSLOCK, ' ') f(A, 'A') f(S, 'S') f(D, 'D') f(F, 'F') f(G, 'G') f(H, 'H') f(J, 'J') f(K, 'K') f(L, 'L') f(SEMICOLON, ':') f(APOSTROPHE, '"') f(RETURN, 0x0a)
f(LSHIFT,' ') f(Z, 'Z') f(X, 'X') f(C, 'C') f(V, 'V') f(B, 'B') f(N, 'N') f(M, 'M') f(COMMA, '<') f(PERIOD, '>') f(SLASH, '?') f(RSHIFT, ' ')
f(LCTRL, ' ') f(APPLICATION, ' ') f(LALT, ' ') f(SPACE, ' ') f(RALT, ' ') f(RCTRL, ' ')
f(UP, ' ') f(DOWN, ' ') f(LEFT, ' ') f(RIGHT, ' ') f(INSERT, ' ') f(DELETE, 0x7f) f(HOME, 0x01) f(END, 0x02) f(PAGEUP, ' ') f(PAGEDOWN, ' ')
};

int main() {
    GLOBAL_INIT_TXT;
    NEW_LINE;
    cmd_main("logo");
    NEW_LINE; PUT_PROMPT;
    char ch;
    int sec = 1;
    while (1) {
        if (io_read(AM_TIMER_UPTIME).us / 524288 >= sec) {
            sec++;
            cursor_blink();
        }

        AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
        if (ev.keycode == AM_KEY_NONE) continue;
        if (!ev.keydown) {
            if (ev.keycode == AM_KEY_LSHIFT || ev.keycode == AM_KEY_RSHIFT) shift_on = 0;
            continue;
        }
        switch (ev.keycode) {
        case AM_KEY_BACKSPACE: cmd_len--; BACKSPACE_HANDLER; break;
        case AM_KEY_CAPSLOCK : capslock_on = capslock_on ^ 1; break;
        case AM_KEY_RETURN   : 
            if (cmd_len) {
                NEW_LINE;
                cmd_buffer[cmd_len] = '\0';
                cmd_main(cmd_buffer);
                cmd_len = 0; 
            }
            NEW_LINE; PUT_PROMPT;
            break;
        case AM_KEY_RSHIFT   :
        case AM_KEY_LSHIFT   : shift_on = 1; break;
        default:
            ch = (shift_on ^ capslock_on) ? key2cap[ev.keycode] : key2char[ev.keycode];
            cmd_buffer[cmd_len++] = ch;
            putch(ch);
            break;
        }
    }
    return 0;
}