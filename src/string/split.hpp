/** lowlevelzs string/split.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <string>
#include <vector>
namespace llzs {
  auto string_split_at(const std::string &in, const char delim) -> std::vector<std::string>;
  auto string_split_sh(const std::string &in) -> std::vector<std::string>;
}
