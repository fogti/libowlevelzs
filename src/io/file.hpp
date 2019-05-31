/** lowlevelzs io/file.hpp
    (C) 2019 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <string>

namespace llzs {
  bool read_file2buffer(const std::string &file, std::string &content) noexcept;
}
