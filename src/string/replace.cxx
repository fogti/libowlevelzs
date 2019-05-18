#include "replace.hpp"
#include "xcpy.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#define restrict __restrict__

using namespace std;
using llzs::intern::string_view;

namespace llzs {
  // source: http://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
  void string_inreplace(string &subject, const string_view &search, const string_view &replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != string::npos) {
      subject.replace(pos, search.size(), replace);
      pos += replace.size();
    }
  }
  string string_replace(string subject, const string_view& search, const string_view& replace) {
    string_inreplace(subject, search, replace);
    return subject;
  }

  void string_inreplace(string &subject, const string& search, const string& replace) {
    string_inreplace(subject, string_view(search), string_view(replace));
  }
  void string_inreplace(string &subject, const char * restrict search, const char * restrict replace) {
    string_inreplace(subject, string_view(search), string_view(replace));
  }
  string string_replace(string subject, const string& search, const string& replace) {
    string_inreplace(subject, search, replace);
    return subject;
  }
}

extern "C" {
  bool llzs_stinreplace(char ** restrict stval, const char * restrict search, const char * restrict replace) noexcept {
    try {
      string tmp = *stval;
      llzs::string_inreplace(tmp, search, replace);
      char *xret = *stval = reinterpret_cast<char*>(realloc(*stval, tmp.size() + 1));
      if(zs_likely(xret)) llzs_strxcpy(xret, tmp.c_str(), tmp.size());
      return xret;
    } catch(...) {
      return false;
    }
  }

  char * llzs_streplace(const char * restrict stval, const char * restrict search, const char * restrict replace, size_t * restrict n) noexcept {
    try {
      string tmp(stval, *n);
      llzs::string_inreplace(tmp, search, replace);
      return llzs_strxdup(tmp.c_str(), (*n = tmp.size()));
    } catch(...) {
      return nullptr;
    }
  }
}
