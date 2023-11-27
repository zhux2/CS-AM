#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

void *malloc(size_t size) {
  size = (size_t)ROUNDUP(size, 8);
  char *old = (char *)addrspace.ptr;
  addrspace.ptr = addrspace.ptr + size;
  if (!((uintptr_t)heap.start <= (uintptr_t)addrspace.ptr && (uintptr_t)addrspace.ptr < (uintptr_t)heap.end))
    return NULL;
  for (char *p = old; p != (char *)addrspace.ptr; p++)
    *p = 0;
  return old;
}

void free(void *ptr) {
}

#endif
