#include "xcpy.h"
#include <stdlib.h>
#include <string.h>
#include <llzs_config.h>

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
  if(zs_unlikely(!str)) return 0;
  char *const ret = calloc(n + 1, 1);
  if(zs_likely(ret)) memcpy(ret, str, n);
  return ret;
}
