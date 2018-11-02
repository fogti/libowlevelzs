#include "csarray.hpp"
#include "utils.hpp"
#include <string_view>
#include <algorithm>

using std::string;
using std::string_view;

namespace llzs {
  CStringArray::CStringArray(const std::vector<std::string> &v)
    : _v(new const char* [v.size()+1])
  {
    for(size_t j = 0; j < v.size(); ++j) _v[j] = v[j].c_str();
    _v[v.size()] = 0;
  }

  void str_trim(string &s) {
    static const string_view whitespace = "\t\n\v\f\r ";
    string_view tsv(s);
    const size_t tmp = tsv.find_last_not_of(whitespace);
    if(tmp != string::npos) tsv.remove_suffix(tsv.size() - tmp - 1);
    tsv.remove_prefix(std::min(tsv.find_first_not_of(whitespace), tsv.size()));
    if(tsv.size() != s.size()) s = string(tsv);
  }
}
