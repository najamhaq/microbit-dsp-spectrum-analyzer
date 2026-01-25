//
// Created by najamhaq on 2026-01-18.
//
#include <stddef.h>
#include <stdint.h>

void* memset(void* dst, int c, size_t n) {
  uint8_t* p = (uint8_t*)dst;
  while (n--) {
    *p++ = (uint8_t)c;
  }
  return dst;
}

void* memcpy(void* dst, const void* src, size_t n) {
  unsigned char* d = (unsigned char*)dst;
  const unsigned char* s = (const unsigned char*)src;

  for (size_t i = 0; i < n; ++i)
    d[i] = s[i];
  return dst;
}