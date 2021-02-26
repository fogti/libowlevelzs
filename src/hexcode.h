/** lowlevelzs hexcode.h
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <inttypes.h>
#include <stdbool.h>
#include <zs/ll/cxa_noexcept.h>

#ifdef __cplusplus
extern "C" {
#endif
  bool llzs_decode_hexnum(const char what, uint8_t *ret) noexcept;
#ifdef __cplusplus
}
namespace llzs {
  static inline bool decode_hexnum(const char what, uint8_t &ret) noexcept {
    return llzs_decode_hexnum(what, &ret);
  }
}
#endif
