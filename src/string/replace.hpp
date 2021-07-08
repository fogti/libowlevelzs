/** lowlevelzs string/replace.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <stddef.h>
#ifdef __cplusplus
# include <string>
# include <string_view>
namespace llzs {
  std::string string_replace(std::string subject, const std::string &search, const std::string &replace);
  std::string string_replace(std::string subject, const std::string_view &search, const std::string_view &replace);
  void string_inreplace(std::string &subject, const std::string_view &search, const std::string_view &replace);
  void string_inreplace(std::string &subject, const std::string &search, const std::string &replace);
  void string_inreplace(std::string &subject, const char * __restrict__ search, const char * __restrict__ replace);
}

extern "C" {
#else
# include <stdbool.h>
# include "../cxa_noexcept.h"
#endif
  bool llzs_stinreplace(char ** __restrict__ stval, const char * __restrict__ search, const char * __restrict__ replace) noexcept;
  char* llzs_streplace(const char * __restrict__ stval, const char * __restrict__ search,
                       const char * __restrict__ replace, size_t * __restrict__ n) noexcept;
#ifdef __cplusplus
}
#endif
