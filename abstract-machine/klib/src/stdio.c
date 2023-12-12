#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define PUTC_BY_MODE(c) \
  if (maxlen > 0) {\
  maxlen--;\
  switch (mode) {\
    case 0: *out = c; out++; break;\
    case 1: putch(c); out++; break;\
  } }\
// b is short for base, mode decide it's sprintf or printf, 
//signed decide to treat x as signed or unsigned
static int int2str(unsigned x, char *out, int b, bool mode, bool _signed_, 
                   bool l_align, int align, char filler, int maxlen, int *nr_char) { 
  char int_buf[10];
  int i = 0;
  char *origin_out = out;
  int origin_nr_char = *nr_char;
  if (_signed_ && (x & (1 << 31))) { 
    x = (~x) + 1;
    int_buf[i++] = '-';
  }
  do {
    unsigned tmp = modu(x, b);
    int_buf[i++] = (tmp > 9) ? ('a' + tmp - 10) : ('0' + tmp);
    x = divu(x, b); 
  } while(x > 0);
  
  while ((!l_align) && align > i) {
    align--; PUTC_BY_MODE(filler); (*nr_char)++;
  } 
  i--;
  while (i >= 0) {
    PUTC_BY_MODE(int_buf[i]); i--; (*nr_char)++;
  }
  while (l_align && align > ((*nr_char) - origin_nr_char)) {
    PUTC_BY_MODE(' '); (*nr_char)++;
  }
  return out - origin_out;
}

static int str2int(const char *s, int *res) {
  *res = 0;
  const char *origin_s = s;
  while (*s >= '0' && *s <= '9') {
    *res = (*res) * 10 + (*s - '0');
    s++;
  }
  return s - origin_s;
}

static int printstr(char *s, char *out, bool mode, bool l_align,
                    int align, int maxlen, int *nr_char) {
  char *origin_out = out;
  int origin_nr_char = *nr_char;
  int len = strlen(s);
  while ((!l_align) && align > len) {
    align--; PUTC_BY_MODE(' '); (*nr_char)++;
  }  
  while (*s) { PUTC_BY_MODE(*s); s++; (*nr_char)++; }
  while (l_align && align > ((*nr_char) - origin_nr_char)) {
    PUTC_BY_MODE(' '); (*nr_char)++;
  }
  return out - origin_out;
}

#define PUTC_PRINTF(c, n)  if (LEN_PRINT < n) { putch(c); out++; }
#define PUTC_SPRINTF(c, n) if (LEN_PRINT < n) { *out = c; out++; }
#define PRINTF_MODE 1
#define SPRINTF_MODE 0

#define LEN_PRINT (out - origin_out)

#define RESET do{ state = 0; filler = ' '; align = 0; l_align = false; rst = 0; } while(0)

#define PRINT_TEMPLATE(X, n) do {\
  int state = 0, rst = 0;\
  char filler = ' '; bool l_align = false; int align = 0;\
  for (const char *p = fmt; *p != '\0'; p++) {\
    if (state) {\
      switch (*p) {\
        case '-': l_align = true; break;\
        case '0': filler = '0';   break;\
        case '1': case '2': case '3': case '4': case '5': \
        case '6': case '7': case '8': case '9': \
          p += str2int(p, &align) - 1; break;\
        case '*': align = va_arg(ap, int); break;\
        case 'x': \
          out += int2str(va_arg(ap, int), out, 16, X##_MODE, 0, \
                         l_align, align, filler, n - LEN_PRINT, &nr_char); \
          rst = 1; break;\
        case 'd': \
          out += int2str(va_arg(ap, int), out, 10, X##_MODE, 1, \
                         l_align, align, filler, n - LEN_PRINT, &nr_char); \
          rst = 1; break;\
        case 's': case 'c': \
          out += printstr(va_arg(ap, char *), out, X##_MODE, \
                          l_align, align, n - LEN_PRINT, &nr_char);\
          rst = 1; break;\
        case '%': PUTC_##X('%', n); nr_char++; rst = 1; break;\
        default : PUTC_##X('%', n); PUTC_##X(*p, n); nr_char += 2; rst = 1; break;\
      }\
      if (rst) { RESET; }\
    }\
    else if (*p == '%') { state = 1; }\
    else { PUTC_##X(*p, n); nr_char++; }\
  } } while(0)

int printf(const char *fmt, ...) {
  int nr_char = 0;
  char *out = 0, *origin_out = 0;
  va_list ap;
  va_start(ap, fmt);
  PRINT_TEMPLATE(PRINTF, 0xfffffff);
  va_end(ap);
  return nr_char;
}

//only support %s and %d
int vsprintf(char *out, const char *fmt, va_list ap) { 
  char *origin_out = out;
  int nr_char = 0;
  PRINT_TEMPLATE(SPRINTF, 0xfffffff);
  *out = '\0';
  return nr_char;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int nr_char = vsprintf(out, fmt, args);
  va_end(args);
  return nr_char;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int nr_char = vsnprintf(out, n, fmt, args);
  va_end(args);
  return nr_char;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  char *origin_out = out;
  int nr_char = 0;
  n--;
  PRINT_TEMPLATE(SPRINTF, n);
  *out = '\0';
  return nr_char;
}

#endif
