subdir('io')
subdir('lexer')
subdir('string')

llzs_base_sources = files(
  'escape.cxx',
  'hash.c', 'hexcode.c',
  'lowlevel.c',
  'MMAPguard.cxx',
  'Parser.cxx',
  'random.cxx',
)

install_headers(
  'cxa_noexcept.h',
  'escape.h',
  'hash.hpp', 'hexcode.h',
  'memut.hpp', 'mman.h',
  'MMAPguard.hpp',
  'Parser.hpp',
  'random.hpp', 'reverse.hpp',
  'zsig.h',
  subdir : llzs_include_subdir)

extra_srcs = []

if is_win32
  extra_srcs += ['cond/win32_mman.c']
endif

# make sure that some headers contain correct C++ code
static_library('ctchk', 'hash.hpp', 'memut.hpp',
  cpp_args : ['-fno-plt', '-fno-rtti'])

# we pass more restrictive flags to the C compiler,
# because we have no C functions which use callbacks
libowlevelzs = shared_library('owlevelzs',
  llzs_base_sources,
  llzs_io_sources,
  llzs_lexer_sources,
  llzs_string_sources,
  extra_srcs,
  version : '2.0.0',
  c_args : ['-fno-plt', '-fno-exceptions', '-fno-unwind-tables'],
  cpp_args : ['-fno-plt', '-fno-rtti'],
  include_directories : include_directories('.', '..'),
  install : true)
