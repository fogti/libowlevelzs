subdir('io')
subdir('lexer')
subdir('string')

llzs_base_sources = files(
  'ctchk.cxx',
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

# we pass more restrictive flags to the C compiler,
# because we have no C functions which use callbacks
libowlevelzs = shared_library('owlevelzs',
  llzs_base_sources,
  llzs_io_sources,
  llzs_lexer_sources,
  llzs_string_sources,
  c_args : ['-fno-plt', '-fno-exceptions', '-fno-unwind-tables'],
  cpp_args : ['-fno-plt', '-fno-rtti'],
  include_directories : include_directories('.', '..'),
  install : true)
