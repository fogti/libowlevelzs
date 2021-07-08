/** lowlevelzs MMAPguard.hpp
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <stddef.h> // size_t

namespace llzs {
  class MMAPguard final {
    void * _addr;
    size_t _len;
    void _do_unmap() noexcept;

   public:
    MMAPguard(): _addr(0), _len(0) { }
    MMAPguard(const MMAPguard &o) = delete;
    ~MMAPguard()                { _do_unmap(); }

    bool assign(int fd, size_t len) noexcept;
    bool valid() const noexcept { return _addr && _len; }

    auto get() const noexcept   { return static_cast<const char*>(_addr); }
    auto size() const noexcept  { return _len; }
    auto begin() const noexcept { return get(); }
    auto end() const noexcept   { return get() + _len; }

    void advise(int adv) const noexcept;
  };
}
