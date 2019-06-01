/** lowlevelzs io/file.hpp
    (C) 2019 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <string>

namespace llzs {
  // post-condition: if this function returns false,
  //  'content' is empty after the function returns
  bool read_file2buffer(const std::string &file, std::string &content) noexcept;
}
