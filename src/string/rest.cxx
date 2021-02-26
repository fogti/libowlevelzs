#include "csarray.hpp"
#include "utils.hpp"
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>

using std::istringstream;
using std::move;
using std::string;
using std::string_view;
using std::vector;

namespace llzs {
  CStringArray::CStringArray(const vector<string> &v)
    : _v(new const char* [v.size()+1])
  {
    for(size_t j = 0; j < v.size(); ++j) _v[j] = v[j].c_str();
    _v[v.size()] = 0;
  }

  CStringArray::CStringArray(const vector<string_view> &v)
    : _v(new const char* [v.size()+1])
  {
    for(size_t j = 0; j < v.size(); ++j) _v[j] = v[j].data();
    _v[v.size()] = 0;
  }

  auto str_split(const string &s) -> vector<string> {
    vector<string> parts;
    istringstream ss(s);
    string tok;
    while((ss >> tok))
      parts.emplace_back(move(tok));
    return parts;
  }

  auto str_split(const string &s, const char delim) -> vector<string> {
    vector<string> parts;
    istringstream ss(s);
    string tok;
    while(getline(ss, tok, delim))
      parts.emplace_back(move(tok));
    return parts;
  }

  auto string_split_sh(const string &in) -> vector<string> {
    vector<string> ret;

    string tok;
    bool escape = false, quote = false, apf = false;

    for(auto &&i : in) {
      if(escape) {
        escape = false;
        char res = i;
        switch(i) {
          case 'n':
            res = '\n'; break;
          case 't':
            res = '\t'; break;
        }
        if(res) tok += res;
      } else {
        switch(i) {
          case '"':
            quote = !quote;
            apf = true;
            break;

          case '\\':
            escape = true;
            break;

          case ' ':
            if(quote) tok += i;
            else if(apf || !tok.empty()) {
              ret.emplace_back(move(tok));
              tok.clear();
              apf = false;
            }
            break;

          default:
            tok += i;
        }
      }
    }

    if(!tok.empty()) ret.emplace_back(move(tok));

    return ret;
  }

  void str_trim(string &s) {
    string_view ts = str_trimmed(string_view(s));
    if(ts.size() != s.size()) s = string(ts);
  }

  auto str_trimmed(string_view ts) -> string_view {
    static const string_view whitespace = "\t\n\v\f\r ";
    const size_t tmp = ts.find_last_not_of(whitespace);
    if(tmp != string::npos) ts.remove_suffix(ts.size() - tmp - 1);
    ts.remove_prefix(std::min(ts.find_first_not_of(whitespace), ts.size()));
    return ts;
  }
}
