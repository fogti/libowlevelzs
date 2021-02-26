#include "split.hpp"
#include <sstream>
#include <utility>

using namespace std;

namespace llzs {

auto string_split_at(const string &in, const char delim) -> vector<string> {
  vector<string> ret;
  istringstream ss(in);
  string tok;

  while(getline(ss, tok, delim))
    ret.emplace_back(move(tok));

  return ret;
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

}
