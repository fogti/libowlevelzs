/** lowlevelzs io/file.cxx
    (C) 2019 Erik Zscheile
    License: MIT
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
    content.shrink_to_fit();
    ret = false;
  }
  fclose(fh);

  return ret;
}

}
