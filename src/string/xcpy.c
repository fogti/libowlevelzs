#include "xcpy.h"
#include <stdlib.h>
#include <string.h>

void llzs_strixcpy(char * restrict * restrict dstp, const char * restrict src, size_t n) noexcept {
  /* assert(strlen(src) == n && sizeof(dest) >= (n + 1)); */
  memcpy(*dstp, src, n);
  *dstp += n; **dstp = 0;
}

char * llzs_strxcpy(char * restrict dest, const char * restrict src, size_t n) noexcept {
  llzs_strixcpy(&dest, src, n);
  return dest;
}

char * llzs_strxdup(const char * str, size_t n) noexcept {
  /* assert(strlen(str) == n); */
  if(!str) return 0;
  char *const ret = malloc(n + 1);
  if(ret) llzs_strxcpy(ret, str, n);
  return ret;
}
