#include "InputSlice.hpp"

namespace llzs {

auto InputSlice::get_line(const LexPos::lineno line_given, LexPos::bound pos_)
    const noexcept -> InputSlice
{
  // line start and line end
  LexPos::single lst, lnd;

  if(line_given == _line) {
    lnd = lst = pos_;

    if(pos_ == _begin) // done with search for lst
      goto contlnd;

    /* handle the case pos_ -> '\n'
     *  if pos_ -> '\n' -> EOL, go 1 backwards
     *  else            -> normal char, go 1 backwards doesn't matter
     */
    --lst;
    do {
      if(*lst == '\n') {
        // start of line found
        //  if pos_ -> '\n' && (pos_ - 1) -> '\n' -> slice empty
        ++lst;
        break;
      }
      --lst;
    } while(lst != _begin);
  } else {
    lst = _begin;

    // skip to line
    for(size_t i = 0; i < line_given; ++i) {
      while(lst != _end && *lst != '\n') ++lst;
      if(lst == _end) break;
      ++lst;
    }
    lnd = lst;
  }

 contlnd:
  while(lnd != _end && *lnd != '\n') ++lnd;
  return InputSlice{line_given, lst, lnd};
}

}
