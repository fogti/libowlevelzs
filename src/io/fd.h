/** lowlevelzs io/fd.h
    (C) 2019 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "cxa_noexcept.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
  int llzs_read_char(int fd) noexcept;
  bool llzs_write_char(int fd, char x) noexcept;
#ifdef __cplusplus
}
#endif
