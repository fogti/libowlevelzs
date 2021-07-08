/** lowlevelzs io/file.hpp
    (C) 2019 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <string>

namespace llzs {
  // post-condition: if this function returns false,
  //  'content' is empty after the function returns
  bool read_file2buffer(const std::string &file, std::string &content) noexcept;
}
