/** lowlevelzs escape.hpp
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include "cxa_noexcept.h"
#ifdef __cplusplus
#include <string>
namespace llzs {
  /* escape_single
   * escapes a single character
   * if there is a valid escape sequence, it returns the escape char,
   *  which should be pretended with a '\\' (backslash)
   * otherwise it returns '\0'
   */
  char escape(char x) noexcept;

  auto escape(const char *s) noexcept -> char*;
  auto escape(const std::string &s) -> std::string;

  /* unescape_single
   * unescapes a single character
   * @param x = character after '\\' (backslash)
   * @return x <or> real character
   */
  char unescape(char x) noexcept;

  auto unescape(const char *s) noexcept -> char*;
  auto unescape(const std::string &s) -> std::string;
}
extern "C" {
#endif
  char * llzs_escape(const char *s) noexcept;
  char * llzs_unescape(const char *s) noexcept;
#ifdef __cplusplus
}
#endif
