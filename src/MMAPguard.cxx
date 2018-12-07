/** lowlevelzs MMAPguard.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "mman.h"
#include "MMAPguard.hpp"

namespace llzs {
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
}
