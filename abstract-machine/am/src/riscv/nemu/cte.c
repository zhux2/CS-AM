#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

#define CASE2(x) case ((x) - 1): case (x):
#define CASE4(x) CASE2((x) - 2) CASE2(x)
#define CASE8(x) CASE4((x) - 4) CASE4(x)
#define CASE16(x) CASE8((x) - 8) CASE8(x)
//#define NR_SYSCALL_TYPE 19
#define CASE_SYSCALL CASE16(19) CASE4(3)

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 0xffffffff: ev.event = EVENT_YIELD; c->mepc += 4; break;
      CASE_SYSCALL ev.event = EVENT_SYSCALL; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
