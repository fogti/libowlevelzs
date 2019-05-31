#include "random.hpp"
#include <stdlib.h>
#include <algorithm>

namespace llzs {
  using namespace std;
  auto gen_random(const size_t length) -> string {
    static const char charset[] =
      " !\"#$%&'()*+,-./:;<=>?@[\\]^_{|}~"
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

    string ret(length, 0);
    generate_n(ret.begin(), length, []() noexcept -> char {
      return charset[rand() % (sizeof(charset) - 1)];
    });
    return ret;
  }
}
