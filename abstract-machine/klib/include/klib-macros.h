#ifndef KLIB_MACROS_H__
#define KLIB_MACROS_H__

#define ROUNDUP(a, sz)      ((((uintptr_t)a) + (sz) - 1) & ~((sz) - 1))
#define ROUNDDOWN(a, sz)    ((((uintptr_t)a)) & ~((sz) - 1))
#define LENGTH(arr)         (sizeof(arr) / sizeof((arr)[0]))
#define RANGE(st, ed)       (Area) { .start = (void *)(st), .end = (void *)(ed) }
#define IN_RANGE(ptr, area) ((area).start <= (ptr) && (ptr) < (area).end)

#define STRINGIFY(s)        #s
#define TOSTRING(s)         STRINGIFY(s)
#define _CONCAT(x, y)       x ## y
#define CONCAT(x, y)        _CONCAT(x, y)

#define putstr(s) \
  ({ for (const char *p = s; *p; p++) putch(*p); })

#define io_read(reg) \
  ({ reg##_T __io_param; \
    ioe_read(reg, &__io_param); \
    __io_param; })

#define io_write(reg, ...) \
  ({ reg##_T __io_param = (reg##_T) { __VA_ARGS__ }; \
    ioe_write(reg, &__io_param); })

#define static_assert(const_cond) \
  static char CONCAT(_static_assert_, __LINE__) [(const_cond) ? 1 : -1] __attribute__((unused))

#define panic_on(cond, s) \
  ({ if (cond) { \
      putstr("AM Panic: "); putstr(s); \
      putstr(" @ " __FILE__ ":" TOSTRING(__LINE__) "  \n"); \
      halt(1); \
    } })

#define panic(s) panic_on(1, s)

//--RV32M--
#define mulhu(x, y) ((uint32_t)(mul(x, y) >> 32))

#define ANSI_FG_BLACK   0x00
#define ANSI_FG_RED     0x10
#define ANSI_FG_GREEN   0x20
#define ANSI_FG_YELLOW  0x30
#define ANSI_FG_BLUE    0x40
#define ANSI_FG_MAGENTA 0x50
#define ANSI_FG_CYAN    0x60
#define ANSI_FG_WHITE   0x70
#define ANSI_BG_BLACK   0x00
#define ANSI_BG_RED     0x01
#define ANSI_BG_GREEN   0x02
#define ANSI_BG_YELLOW  0x03
#define ANSI_BG_BLUE    0x04
#define ANSI_BG_MAGENTA 0x05
#define ANSI_BG_CYAN    0x06
#define ANSI_BG_WHITE   0x07

#define COL_LMASK 0x0f
#define COL_HMASK 0xf0
#define SET_FG_COL(col) set_col((char)((serialcol & COL_LMASK) | ANSI_FG_##col))
#define SET_BG_COL(col) set_col((char)((serialcol & COL_HMASK) | ANSI_BG_##col))
#define SET_COL(fg, bg) set_col((char)(ANSI_FG_##fg | ANSI_BG_##bg))
#define RESET_COL       reset_col()

#endif
