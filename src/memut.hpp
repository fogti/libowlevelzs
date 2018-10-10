/** zprd memut.hpp
 * a simple, mostly compile-time typesafe mem* function collection
 * (C) 2018 Erik Zscheile.
 * License: MIT
 **/
#pragma once
#include <string.h>
#include <algorithm>

template<typename T>
static inline void zeroify(T &obj) noexcept
  { memset(&obj, 0, sizeof(T)); }

template<typename T>
static inline void whole_memcpy(T *const __restrict__ dest, const T *const __restrict__ src) noexcept
  { memcpy(dest, src, sizeof(T)); }

template<typename T>
static inline void whole_memcpy_lazy(char dest[sizeof(T)], const T *const __restrict__ src) noexcept
  { memcpy(dest, src, sizeof(T)); }

// internal functions with instable API -- lesser code duplication
namespace llzs_intern {
  static inline void zeroify_offset(void *const __restrict__ ptr, const size_t whole, const size_t snip) noexcept
    { memset(static_cast<char*>(ptr) + snip, 0, whole - snip); }

  template<size_t WHOLE>
  static inline void partial_memcpy(void *const __restrict__ dest, const void *const __restrict__ src, const size_t snip) noexcept {
    static_assert(WHOLE != 1); // avoid misuse of function
    const size_t sanit_snip = std::min(WHOLE, snip);
    memcpy(dest, src, sanit_snip);
    zeroify_offset(dest, WHOLE, sanit_snip);
  }

  template<typename T>
  static inline void memcpy_fromto(void *const __restrict__ dest, const void *const __restrict__ src) noexcept {
    static_assert(sizeof(T) != 1); // avoid misuse of function
    memcpy(dest, src, sizeof(T));
  }
}

// === partial memcpy ===
template<typename T>
static inline void partial_memcpy(T *const __restrict__ dest, const T *const __restrict__ src, const size_t snip) noexcept {
  llzs_intern::partial_memcpy<sizeof(T)>(dest, src, snip);
}

template<size_t WHOLE>
static inline void partial_memcpy_bytes(char dest[WHOLE], const char *const __restrict__ src, const size_t snip) {
  llzs_intern::partial_memcpy<WHOLE>(dest, src, snip);
}

template<size_t WHOLE, typename T>
static inline void partial_memcpy_lazy(char (&dest)[WHOLE], const T *const __restrict__ src) noexcept {
  static_assert(WHOLE >= sizeof(T));
  llzs_intern::partial_memcpy<WHOLE>(dest, src, sizeof(T));
}

// use size of src as real size
template<typename T>
static inline void memcpy_from(void *const __restrict__ dest, const T *const __restrict__ src) noexcept
  { llzs_intern::memcpy_fromto<T>(dest, src); }

// use size of dest as real size
template<typename T>
static inline void memcpy_to(T *const __restrict__ dest, const void *const __restrict__ src) noexcept
  { llzs_intern::memcpy_fromto<T>(dest, src); }
