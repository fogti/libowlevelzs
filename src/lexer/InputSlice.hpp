/** lowlevelzs lexer/InputSlice.hpp
    (C) 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include "LexPos.hpp"
#include <string>

namespace llzs {
  class InputSlice final {
    LexPos::lineno _line;
    LexPos::single _begin, _end;

   public:
    InputSlice(const LexPos::lineno line, LexPos::bound begos, LexPos::bound endos)
      : _line(line), _begin(begos), _end(endos) { }

    auto to_string() const { return std::string(_begin, _end); }

    size_t size() const noexcept { return _end - _begin;  }
    bool empty()  const noexcept { return _begin == _end; }
    auto line()   const noexcept { return _line; }

    auto begin()  const noexcept { return _begin;      }
    auto end()    const noexcept { return _end;        }
    auto front()  const noexcept { return *_begin;     }
    auto back()   const noexcept { return *(_end - 1); }

    /* get_line: get line from source
     *
     * @param this { _line, 0, _begin, _end }
     * @param line_  current line
     * @param pos    current position
     */
    auto get_line(const LexPos::lineno line_, LexPos::bound pos)
      const noexcept -> InputSlice;
  };
}
