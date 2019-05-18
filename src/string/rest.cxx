#include "csarray.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;

namespace llzs {
  CStringArray::CStringArray(const std::vector<std::string> &v)
    : _v(new const char* [v.size()+1])
  {
    for(size_t j = 0; j < v.size(); ++j) _v[j] = v[j].c_str();
    _v[v.size()] = 0;
  }

  CStringArray::CStringArray(const std::vector<intern::string_view> &v)
    : _v(new const char* [v.size()+1])
  {
    for(size_t j = 0; j < v.size(); ++j) _v[j] = v[j].data();
    _v[v.size()] = 0;
  }

  void str_trim(string &s) {
    static const intern::string_view whitespace = "\t\n\v\f\r ";
    intern::string_view ts = s;
    const size_t tmp = ts.find_last_not_of(whitespace);
    if(tmp != string::npos) ts.remove_suffix(ts.size() - tmp - 1);
    ts.remove_prefix(std::min(ts.find_first_not_of(whitespace), ts.size()));
    if(ts.size() != s.size()) s = string(ts);
  }

  auto str_split(const string &s) -> vector<string> {
    vector<string> parts;
    std::istringstream ss(s);
    string tok;
    while((ss >> tok))
      parts.emplace_back(std::move(tok));
    return parts;
  }

  auto str_split(const string &s, const char delim) -> vector<string> {
    vector<string> parts;
    std::istringstream ss(s);
    string tok;
    while(getline(ss, tok, delim))
      parts.emplace_back(std::move(tok));
    return parts;
  }
}
