/** lowlevelzs hexcode.h
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <inttypes.h>
#include <stdbool.h>
#include "cxa_noexcept.h"

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
