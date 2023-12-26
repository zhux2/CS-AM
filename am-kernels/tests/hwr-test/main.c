#include <am.h>
#include <klib.h>
#include <klib-macros.h>

 #define SMALL
// #define MIDDLE
//#define HUGE

/*
 * SMALL  is for low    efficiency. e.g. fpga
 * MIDDLE is for middle efficiency. e.g. nemu 
 * HUGE   is for high   efficiency. e.g. native
 */

uint64_t get_time() {
  return io_read(AM_TIMER_UPTIME).us;
}

void print_time(const char *name, uint64_t us) {
  printf("[%s]:\n     %d.%06d sec\n", name, divu(us, 1000000), modu(us, 1000000));
}

void run_test(const char *name, void (*func)(void)) {
  uint64_t t1 = get_time();
  func();
  uint64_t t2 = get_time();
  print_time(name, t2 - t1);
}

/* test bench 1: fact */

#if defined(SMALL)
#define FACT_N ((int)1e5)
#elif defined(MIDDLE)
#define FACT_N ((int)1e7)
#elif defined(HUGE)
#define FACT_N ((int)1e9)
#endif


static __attribute__ ((noinline))
void fact1() {
  volatile unsigned fact __attribute__((unused)) = 1;
  for (int i = 1; i < FACT_N; i++)
    fact = mul(fact, i);
}

__attribute__ ((noinline))
void fact2() {
  volatile unsigned fact0 __attribute__((unused)) = 1;
  volatile unsigned fact1 __attribute__((unused)) = 1;
  volatile unsigned fact2 __attribute__((unused)) = 1;
  volatile unsigned fact3 __attribute__((unused)) = 1;
  for (int i = 1; i < FACT_N; i += 4) {
    fact0 = mul(fact0, i);
    fact1 = mul(fact1, i + 1);
    fact2 = mul(fact2, i + 2);
    fact3 = mul(fact3, i + 3);
  }
}

__attribute__ ((noinline))
void fact3() {
  volatile unsigned fact __attribute__((unused)) = 1;
  for (int i = 1; i < FACT_N; i += 4) {
    fact = mul(fact, i);
    fact = mul(fact, i+1);
    fact = mul(fact, i+2);
    fact = mul(fact, i+3);
  }
}
static void test_fact() {
  run_test("fact1(no optimization)", fact1);
  run_test("fact2(loop expansion)", fact2);
  run_test("fact3(loop expansion but data conflict)", fact3);
}

/* test bench 2: fact */

#if defined(SMALL)
#define TIMES ((int)1e2)
#elif defined(MIDDLE)
#define TIMES ((int)1e3)
#elif defined(HUGE)
#define TIMES ((int)1e4)
#endif
#define SIZE (0x1000)
static volatile int value[SIZE];

static __attribute__ ((noinline))
void count1() {
  int T = TIMES;
  while (T--) {
    for (int i = 0; i < SIZE; i++) {
      value[i] = rand();
    }
    volatile int cnt __attribute__((unused)) = 0;
    for (int i = 0; i < SIZE; i++)
      if (value[i] > 500)
        cnt++;
  }
}

static __attribute__ ((noinline))
void count2() {
  int T = TIMES;
  while (T--) {
    for (int i = 0; i < SIZE; i++) {
      value[i] = i;
    }
    volatile int cnt __attribute__((unused)) = 0;
    for (int i = 0; i < SIZE; i++)
      if (value[i] > 500)
        cnt++;
  }
}

static void test_count() {
  run_test("count1(random value)", count1);
  run_test("count2(sorted value)", count2);
}

#define NR_SPACE 80
static char space[NR_SPACE];
void init_txt() {
  vga_set_txt();
    RESET_COL;
    for (int y = 0; y < TXT_V; y++)
        io_write(AM_TXT_FBDRAW, 0, y, space, TXT_H);
    set_cursor_position(0, 0);
}

int main() {
  init_txt();
  printf("begin\n");
  srand(get_time());
  test_fact();
  test_count();
  return 0;
}
