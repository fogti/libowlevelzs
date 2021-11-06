#include "llzs_config.h"
#include "zsig.h"
#include "io/fd.h"
#include "string/xcpy.h"

#include <errno.h>
#include <stdlib.h>      /* malloc */
#include <stdint.h>      /* intmax_t */
#include <string.h>      /* memcpy */
#include <signal.h>
#include <unistd.h>

zs_attrib(cold)
void my_signal(const int sig_nr, const sighandler_t signalhandler) {
#ifdef WIN32
/* win32 -- source: https://stackoverflow.com/questions/32389905/sigaction-and-porting-linux-code-to-windows */
  signal(signr, signalhandler);
#else
/* normal unix --
  source: http://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap08-004.htm */

  struct sigaction sig;
  sig.sa_handler = signalhandler;
  sig.sa_flags   = SA_RESTART;
  sigemptyset(&sig.sa_mask);
  sigaction(sig_nr, &sig, 0);
#endif
}

#ifndef WIN32
/* this function is currently unsupported on platforms without sigaction */
zs_attrib(cold)
void my_signal_norestart(const int sig_nr, const sighandler_t signalhandler) {
  struct sigaction sig;
  sig.sa_handler = signalhandler;
  sig.sa_flags   = 0;
  sigemptyset(&sig.sa_mask);
  sigaction(sig_nr, &sig, 0);
}
#endif

zs_attrib(hot)
void llzs_hash_combine(uintmax_t *seed, const uintmax_t o) {
  // source of rndst : https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes#comment54810251_27952689
  // rndst := inverse of golden ratio as a ... fixed point fraction
  // the following uses more entropy on 64bit (e.g. when available)
  const uintmax_t sedc = *seed;
  const uintmax_t rndst =
    (sizeof(uintmax_t) >= 8) ? 0x9e3779b97f4a7c15 : 0x9e3779b9;
  *seed ^= rndst + o + (sedc << 6) + (sedc >> 2);
}

int llzs_read_char(const int fd) noexcept {
  char tmp = 0;
  int r;
  while((r = read(fd, &tmp, 1)) == -1 && errno == EINTR) ;
  if(zs_unlikely(!r)) {
    // handle EOF
    errno = ECONNRESET;
    r = -1;
  }
  return (r == -1) ? -1 : tmp;
}

bool llzs_write_char(const int fd, const char x) noexcept {
  while(true) {
    if(zs_likely(write(fd, &x, 1) == 1)) return true;
    if(zs_unlikely(errno != EINTR)) return false;
  }
}


void llzs_strixcpy(char * restrict * restrict dstp, const char * restrict src, size_t n) {
  /* assert(strlen(src) == n && sizeof(dest) >= (n + 1)); */
  memcpy(*dstp, src, n);
  *dstp += n; **dstp = 0;
}

char * llzs_strxcpy(char * restrict dest, const char * restrict src, size_t n) {
  llzs_strixcpy(&dest, src, n);
  return dest;
}

char * llzs_strxdup(const char * str, size_t n) {
  if(zs_unlikely(!str)) return 0;
  /* assert(strlen(str) == n); */
  char *const ret = malloc(n + 1);
  if(zs_likely(ret)) {
    memcpy(ret, str, n);
    ret[n] = 0;
  }
  return ret;
}
