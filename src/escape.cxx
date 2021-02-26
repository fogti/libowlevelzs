// original source: https://github.com/zserik/zxtw/src/escape.cxx
#include "escape.h"
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

namespace llzs {
   // impl for chars

# define CRT(F,T) case F: return T;
  char escape(const char x) noexcept {
    switch(x) {
      CRT('\a', 'a')
      CRT('\b', 'b')
      CRT('\033', 'e')
      CRT('\f', 'f')
      CRT('\n', 'n')
      CRT('\r', 'r')
      CRT('\t', 't')
      CRT('\v', 'v')
      CRT('"' , '"')
      CRT('\\', '\\')
      default: return 0;
    }
  }

  char unescape(const char x) noexcept {
    switch(x) {
      CRT('a', '\a')
      CRT('b', '\b')
      CRT('e', '\033')
      CRT('f', '\f')
      CRT('n', '\n')
      CRT('r', '\r')
      CRT('t', '\t')
      CRT('v', '\v')
      // '"' and '\\' are handled implicit
      default:  return x;
    }
  }

  // impl for c-strings

  typedef size_t (*escape_cstr_fnt)(const char *s, char *ret);
  static char * escape_wrap(const char * const s, bool x2, escape_cstr_fnt fn) noexcept {
    char * tmp = static_cast<char*>(calloc((1 + x2) * strlen(s) + 1, 1));
    if(!tmp) return nullptr;
    char * ret = static_cast<char*>(realloc(tmp, fn(s, tmp) + 1));
    return ret ? ret : tmp;
  }

  static size_t escape_intern(const char * const s, char * const ret) noexcept {
    size_t pos = 0;
    for(const char *i = s; *i; ++i) {
      char tmp = escape(*i);
      if(tmp) ret[pos++] = '\\';
      else    tmp = *i;
      ret[pos++] = tmp;
    }
    return pos;
  }

  static size_t unescape_intern(const char * const s, char * const ret) noexcept {
    size_t pos = 0;
    for(const char *i = s; *i; ++i) {
      char tmp = *i;
      if(*i == '\\') {
        if(!*(++i)) break;
        tmp = unescape(*i);
      }
      ret[pos++] = tmp;
    }
    return pos;
  }

  auto escape(const char * const s) noexcept -> char *
    { return escape_wrap(s, true, escape_intern); }

  auto unescape(const char * const s) noexcept -> char *
    { return escape_wrap(s, false, unescape_intern); }

  // impl for c++ strings

  auto escape(const string &s) -> string {
    string ret;
    ret.reserve(s.size());

    for(const auto i : s) {
      // NOTE: prevent creation of too many temporaries
      if(const char tmp = escape(i)) {
        ret += '\\';
        ret += tmp;
      } else {
        ret += i;
      }
    }

    return ret;
  }

  auto unescape(const string &s) -> string {
    string ret;
    ret.reserve(s.size());

    bool is_esc = false;
    for(const auto i : s) {
      const char tmp = ([&] {
        if(is_esc) {
          is_esc = !is_esc;
          return unescape(i);
        }
        if(i == '\\') return '\0';
        return i;
      })();
      if(tmp)
        ret += tmp;
    }

    return ret;
  }
}

extern "C" char * llzs_escape(const char * const s) noexcept
  { return llzs::escape_wrap(s, true, llzs::escape_intern); }

extern "C" char * llzs_unescape(const char * const s) noexcept
  { return llzs::escape_wrap(s, false, llzs::unescape_intern); }
