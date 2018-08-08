#include "csarray.hpp"

namespace llzs {
  CStringArray::CStringArray(const std::vector<std::string> &v)
    : _v(new const char* [v.size()+1])
  {
    for(size_t j = 0; j < v.size(); ++j) _v[j] = v[j].c_str();
    _v[v.size()] = 0;
  }
}
