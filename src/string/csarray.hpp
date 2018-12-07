/** lowlevelzs string/csarray.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "llzs_config.h"
#include <string>
#include <vector>
namespace llzs {
  class CStringArray {
    const char** _v;
   public:
    explicit CStringArray(const std::vector<std::string> &);
    explicit CStringArray(const std::vector<std::string> &&) = delete;
#ifdef LIBOWLEVELZS_SUPPORT_STRING_VIEW
    explicit CStringArray(const std::vector<intern::string_view> &);
#endif
    ~CStringArray() noexcept { delete[] _v; }
    char ** data()  const    { return const_cast<char **>(_v); }
  };
}
