#include "mman.h"
#include "MMAPguard.hpp"
#include "random.hpp"
#include "io/file.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

namespace llzs {
  using std::string;

  void MMAPguard::_do_unmap() noexcept {
    if(valid()) munmap(_addr, _len);
  }

  bool MMAPguard::assign(const int fd, const size_t len) noexcept {
    if(zs_unlikely(!len)) return false;
    void *const addr = mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);
    if(zs_unlikely(addr == reinterpret_cast<void*>(-1)))
      return false;
    _do_unmap(); _addr = addr; _len = len;
    return true;
  }

  void MMAPguard::advise(const int adv) const noexcept {
    if(zs_likely(valid())) madvise(_addr, _len, adv);
  }

  auto gen_random(const size_t length) -> string {
    static const char charset[] =
      " !\"#$%&'()*+,-./:;<=>?@[\\]^_{|}~"
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

    string ret(length, 0);
    std::generate_n(ret.begin(), length, []() noexcept -> char {
      return charset[rand() % (sizeof(charset) - 1)];
    });
    return ret;
  }

  bool read_file2buffer(const string &file, string &content) noexcept {
    content.clear();

    FILE * fh = fopen(file.c_str(), "r");
    if(!fh) return false;

    char buf[1024];
    bool ret = true;

    try {
      while(const size_t cnt = fread(buf, 1, sizeof(buf) - 1, fh))
        content.append(buf, cnt);
    } catch(...) {
      content.clear();
      ret = false;
    }

    fclose(fh);
    content.shrink_to_fit();
    return ret;
  }
}
