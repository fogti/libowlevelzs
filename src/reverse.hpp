/** lowlevelzs reverse.hpp --- Reversed iterable
    (C) 2019 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#pragma once
#include <iterator>
namespace llzs {
  namespace intern {
    template<class T>
    struct reversion_wrapper { T& iterable; };

#define llzs__reverse_wr__ffn(X,Y,P) \
    template<class T> \
    auto X(reversion_wrapper<T> P w) { \
      using std::Y; \
      return Y(w.iterable); \
    }

    llzs__reverse_wr__ffn( begin,  rbegin,       )
    llzs__reverse_wr__ffn(   end,    rend,       )
    llzs__reverse_wr__ffn(cbegin, crbegin, const&)
    llzs__reverse_wr__ffn(  cend,   crend, const&)

#undef llzs__reverse_wr__ffn

    template<class T>
    reversion_wrapper<T> reverse(T&& iterable)
      { return { iterable }; }
  }

  using intern::reserve;
}
