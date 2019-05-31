/** lowlevelzs string/accu.hpp
    (C) 2019 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <zs/ll/llzs_config.h>
#include <stddef.h>
#include <deque>
#include <string>
#include <utility>
#include <vector>

namespace llzs {
  // a string accumulator class
  class string_accu final {
    std::vector<intern::string_view>
      _sv_buf;
    std::deque<std::pair<std::string, size_t>>
      _own_buf;

   public:
    auto size() const noexcept -> size_t;
    auto to_string(size_t prealloc = 0) const -> std::string;

    void reserve(size_t n)
      { _sv_buf.reserve(n); }

    void append(const char x);
    void append(std::string x);
    void append_ref(const intern::string_view &x);

    void append_ref(const char * const b, const char * const e)
      { append_ref(intern::string_view(b, e - b)); }

    // NOTE: the return value of pop_back() is invalidated after clear() is called
    auto pop_back() -> intern::string_view;
    void shrink_to_fit() noexcept;

    void clear() noexcept {
      _sv_buf.clear();
      _own_buf.clear();
    }
  };
}
