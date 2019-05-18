/** lowlevelzs string/utils.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <string>
#include <vector>
namespace llzs {
  void str_trim(std::string &s);
  auto str_split(const std::string &s) -> std::vector<std::string>;
  auto str_split(const std::string &s, char delim) -> std::vector<std::string>;
}
