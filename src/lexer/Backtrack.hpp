/** lowlevelzs lexer/Backtrack.hpp
    (C) 2018 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include "LexPos.hpp"
namespace llzs {
  class Backtrack {
   protected:
    LexPos::single _begin;
    LexPos::ref    _pos;

   public:
    explicit Backtrack(LexPos::ref pos)
      : _begin(pos), _pos(pos) { }

    Backtrack(Backtrack&&) = default;
    Backtrack(const Backtrack&) = delete;
    Backtrack& operator=(const Backtrack&) = delete;

    ~Backtrack() noexcept
      { if(_begin) _pos = _begin; }

    void disable() noexcept
      { _begin = 0; }
  };
}
