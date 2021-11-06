/** lowlevelzs random.hpp
    (C) 2019 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <stddef.h>
#include <string>
namespace llzs {
  // pre-condition: srand() was called since start of program
  auto gen_random(size_t length) -> std::string;
}
