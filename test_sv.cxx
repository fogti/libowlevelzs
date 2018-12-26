/** libowlevelzs test_sv.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/

#include <stdio.h>

#if __has_include(<string_view>) && __cplusplus > 201402L
# include <string_view>
# define SV_HNAM "string_view"
# define SV_NS std
# define SV_NSS "std"
#elif __has_include(<experimental/string_view>) && __cplusplus > 201402L
# include <experimental/string_view>
# define SV_HNAM "experimental/string_view"
# define SV_NS std::experimental
# define SV_NSS "std::experimental"
#endif

#ifdef SV_NS
  using zs_sv = SV_NS::string_view;
#endif

int main(void) {
#ifndef SV_HNAM
  return 1;
#else
  puts("#ifdef __cplusplus");
  puts("#include <" SV_HNAM ">");
  puts("#define LIBOWLEVELZS_SUPPORT_STRING_VIEW");
#endif

#ifdef SV_NSS
  puts("namespace llzs { namespace intern { using string_view = " SV_NSS "::string_view; } }");
#endif
  puts("#endif /* C++ */");

  return 0;
}
