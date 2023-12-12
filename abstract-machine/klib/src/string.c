#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len;
  for (len = 0; s[len] != '\0'; len++);
  return len;
}

char *strcpy(char *dst, const char *src) {
  size_t i = 0;
  do {
    dst[i] = src[i];
  } while (src[i++] != '\0');
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\n'; ++i) 
    dst[i] = src[i];
  for (; i < n; ++i)
    dst[i] = '\0';
  return dst;
}

char *strcat(char *dst, const char *src) {
  size_t dst_len = strlen(dst);
  size_t i;
  for (i = 0; src[i] != '\0'; ++i) 
    dst[dst_len + i] = src[i];
  dst[dst_len + i] = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  size_t i;
  for (i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i) {
    if (s1[i] != s2[i]) return s1[i] - s2[i];
  }
  if (s1[i] == s2[i]) return 0; //both s1[i] = s2[i] = '\0'
  return s1[i] - s2[i];
}

int strncmp(const char *s1, const char *s2, size_t n) {
  size_t i;
  for (i = 0; i < n && s1[i] != '\0' && s2[i] != '\0'; ++i) {
    if (s1[i] != s2[i]) return s1[i] - s2[i];
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  size_t i;
  for (i = 0; i < n; ++i) 
    *(char *)(s + i) = c;
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  if (dst == src) return dst;
  long i;
  if ((size_t)dst < (size_t)src) {
    for (i = 0; i < n; ++i)
      *(char *)(dst + i) = *(char *)(src + i);
  }
  else {
    for (i = n - 1; i >= 0; --i) {
      *(char *)(dst + i) = *(char *)(src + i);
    }
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  size_t i;
  for (i = 0; i < n; ++i)
    *(char *)(out + i) = *(char *)(in + i);
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  size_t i;
  for (i = 0; i < n; ++i) 
    if (*(char *)(s1 + i) != *(char *)(s2 + i)) 
      return *(char *)(s1 + i) - *(char *)(s2 + i);
  return 0;
}

char *strtok(char *src, char seperator, int *len) {
  char *original_src = src;
  char *dst;
  while (*src == seperator) src++;
  dst = src;
  while (*src && *src != seperator) src++;
  if (*src) { *src = '\0'; src++; }
  *len = src - original_src;
  return dst;
}

#endif
