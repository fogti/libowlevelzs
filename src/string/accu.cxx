#include "accu.hpp"
#include <algorithm>
#include <functional>
// DEBUG
#include <iostream>
#include <iomanip>

using namespace std;

namespace llzs {

[[gnu::hot]]
auto string_accu::size() const noexcept -> size_t {
  size_t ret = 0;
  for(const auto &x : _sv_buf)
    ret += x.size();
  return ret;
}

auto string_accu::to_string(const size_t prealloc) const -> string {
  string ret;
  ret.reserve(prealloc ? prealloc : size());
  for(const auto &x : _sv_buf)
    ret += x;
  return ret;
}

auto string_accu::pop_back() -> intern::string_view {
  if(_sv_buf.empty()) return {};
  const auto sv = _sv_buf.back();
  _sv_buf.pop_back();
  return sv;
}

void string_accu::append(const char x) {
  append(string(1, x));
}

void string_accu::append(string x) {
  const auto xh = hash<string>()(x);

  // check if we need to insert the element into _own_buf
  {
    const auto ie = _own_buf.end();
    const auto it = find_if(_own_buf.begin(), ie,
      [&xh](const decltype(_own_buf)::value_type &y) { return xh == y.second; });

    if(it != ie) {
      _sv_buf.emplace_back(it->first);
      return;
    }
  }

  // insert the element into the _own_buf
  const size_t old_sv_buf_siz = _sv_buf.size();
  const auto insr = _own_buf.emplace_back(std::move(x), xh);

  // DEBUG
  cerr << "DEBUG: _own_buf = [";
  for(const auto &i : _own_buf)
    cerr << ' ' << quoted(i.first) << "[" << i.second << "]";
  cerr << " ]\n";
  cerr << "DEBUG: _sv_buf = [";
  for(const auto &i : _sv_buf)
    cerr << ' ' << quoted(i);
  cerr << " ]\n";

  try {
    _sv_buf.emplace_back(insr.first);
  } catch(...) {
    if(_sv_buf.size() != old_sv_buf_siz) {
      // this probably shouldn't happen, but catch it, since it might break our invariants
      _sv_buf.pop_back();
    }
    _own_buf.pop_back();
    shrink_to_fit();
    throw;
  }
}

void string_accu::append_ref(const intern::string_view &x) {
  if(x.empty()) return;

  if(!_sv_buf.empty()) {
    auto &prev = _sv_buf.back();
    if((prev.data() + prev.size()) == x.data()) {
      // merge two string_views
      prev = intern::string_view(prev.data(), prev.size() + x.size());
      return;
    }
  }

  _sv_buf.emplace_back(x);
}

void string_accu::shrink_to_fit() noexcept {
  _sv_buf.shrink_to_fit();
  if(_sv_buf.empty())
    _own_buf.clear();
  if(_own_buf.empty()) // only shrink own_buf if no string can reference it
    _own_buf.shrink_to_fit();
}

}
