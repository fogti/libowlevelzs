/** lowlevelzs string/utils.hpp
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <string>
#include <string_view>
#include <vector>
namespace llzs {
  // str_split can't be trivially implemented for string_view,
  // as the C++ STL has no high-level support for string_view in stringstream
  auto str_split(const std::string &s) -> std::vector<std::string>;
  auto str_split(const std::string &s, char delim) -> std::vector<std::string>;

  // str_split_sh can't be trivially implemented for string_view, as it unescapes
  // returned tokens
  auto str_split_sh(const std::string &in) -> std::vector<std::string>;

  void str_trim(std::string &s);
  auto str_trimmed(std::string_view s) -> std::string_view;
}
