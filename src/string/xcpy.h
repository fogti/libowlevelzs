/** lowlevelzs string/xcpy.h
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#else
# include "../cxa_noexcept.h"
#endif
  char * llzs_strxcpy(char * __restrict__ dest, const char * __restrict__ src, size_t n) noexcept;
  void llzs_strixcpy(char * __restrict__ * __restrict__ dstp, const char * __restrict__ src, size_t n) noexcept;
  char * llzs_strxdup(const char * str, size_t n) noexcept;
#ifdef __cplusplus
}
#endif
