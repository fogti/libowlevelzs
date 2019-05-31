/** lowlevelzs random.hpp
    (C) 2019 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <string>
namespace llzs {
  // pre-condition: srand() was called since start of program
  auto gen_random(size_t length) -> std::string;
}
