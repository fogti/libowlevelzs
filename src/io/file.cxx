/** lowlevelzs io/file.cxx
    (C) 2019 - 2021 Alain Zscheile
    SPDX-License-Identifier: ISC
 **/
#include "file.hpp"
#include <stdio.h>

using std::string;

namespace llzs {

bool read_file2buffer(const string &file, string &content) noexcept {
  content.clear();

  FILE * fh = fopen(file.c_str(), "r");
  if(!fh) return false;

  char buf[1024];
  bool ret = true;

  try {
    while(const size_t cnt = fread(buf, 1, sizeof(buf) - 1, fh))
      content.append(buf, cnt);
  } catch(...) {
    content.clear();
    ret = false;
  }

  fclose(fh);
  content.shrink_to_fit();
  return ret;
}

}
