/** lowlevelzs string/csarray.hpp
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <string>
#include <string_view>
#include <vector>
namespace llzs {
  class CStringArray {
    const char** _v;
   public:
    explicit CStringArray(const std::vector<std::string> &);
    explicit CStringArray(const std::vector<std::string> &&) = delete;
    explicit CStringArray(const std::vector<std::string_view> &);
    ~CStringArray() noexcept { delete[] _v; }
    char ** data()  const    { return const_cast<char **>(_v); }
  };
}
