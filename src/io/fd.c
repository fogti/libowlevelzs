#include "fd.h"
#include <llzs_config.h>
#include <errno.h>
#include <unistd.h>

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
