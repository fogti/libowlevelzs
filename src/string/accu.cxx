#include "accu.hpp"
#include <algorithm>
#include <functional>

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
  const auto ie = _own_buf.end();
  const auto it = find_if(_own_buf.begin(), ie,
    [&xh](const decltype(_own_buf)::value_type &y) { return xh == y.second; });

  // insert the element into the _own_buf, if it isn't there already
  const auto insr = (it != ie) ? (*it) : (_own_buf.emplace_back(std::move(x), xh));
  _sv_buf.emplace_back(insr.first);
}

void string_accu::append_ref(const intern::string_view &x) {
  if(x.empty()) return;

  if(!_sv_buf.empty()) {
    auto &prev = _sv_buf.back();
    if(prev.end() == x.begin()) {
      // merge two string_views
      prev = intern::string_view(prev.begin(), prev.size() + x.size());
      return;
    }
  }

  _sv_buf.emplace_back(x);
}

}
