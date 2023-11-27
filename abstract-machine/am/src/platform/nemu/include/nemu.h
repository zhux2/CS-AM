#ifndef NEMU_H__
#define NEMU_H__

#include <klib-macros.h>

#include ISA_H // the macro `ISA_H` is defined in CFLAGS
               // it will be expanded as "x86/x86.h", "mips/mips32.h", ...

#if defined(__ISA_X86__)
# define nemu_trap(code) asm volatile ("int3" : :"a"(code))
#elif defined(__ISA_MIPS32__)
# define nemu_trap(code) asm volatile ("move $v0, %0; sdbbp" : :"r"(code))
#elif defined(__riscv)
# define nemu_trap(code) asm volatile("mv a0, %0; ebreak" : :"r"(code))
#elif defined(__ISA_LOONGARCH32R__)
# define nemu_trap(code) asm volatile("move $a0, %0; break 0" : :"r"(code))
#elif
# error unsupported ISA __ISA__
#endif

#if defined(__ARCH_X86_NEMU)
# define DEVICE_BASE 0x0
#else
# define DEVICE_BASE 0xa0000000
#endif

#define MMIO_BASE 0xa0000000

#define TXT_ADDR        0x20000000
#define TXTCOL_ADDR     0x30000000
#define KBD_ADDR        0x40000000
#define RTC_ADDR        0x50000000
#define FB_ADDR         0x60000000
#define LED_ADDR        0x70000000
#define SEG_ADDR        0x80000000
#define SW_ADDR         0x90000000
#define VGA_CONF_PORT   0xa0000000

extern char _pmem_start;
#define PMEM_SIZE (4 * 65536)
#define PMEM_END  ((uintptr_t)&_pmem_start + PMEM_SIZE)
//only used in vme
#define NEMU_PADDR_SPACE \
  RANGE(&_pmem_start, PMEM_END), \
  RANGE(FB_ADDR, FB_ADDR + 0x200000), \
  RANGE(MMIO_BASE, MMIO_BASE + 0x1000) /* serial, rtc, screen, keyboard */

typedef uintptr_t PTE;

#define PGSIZE    4096

#endif
