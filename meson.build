project('Zscheile Lowlevel (utility) library', 'c', 'cpp',
  default_options : ['cpp_std=c++17'],
  version : '0.1.2',
  license : 'ISC',
  meson_version : '>=0.50')

install_include_middir = get_option('includedir')
install_library_middir = get_option('libdir')
llzs_include_subdir = join_paths('zs', 'll')

cc = meson.get_compiler('c', native : false)

is_win32 = host_machine.system() == 'windows'

configure_file(
  input  : 'llzs_config.h.in',
  output : 'llzs_config.h',
  configuration : {
    'HAVE_ATTRIBUTE' :
      cc.compiles('''
static int __attribute__((pure)) zs_st_pure_func(const int x) { return 2 * x; }
int main(void) { return zs_st_pure_func(0); }
''',
        name : '__attribute__((...))'),
    'HAVE_BUILTIN_EXPECT' :
      cc.compiles('int main(void) { return __builtin_expect(0, 1); }',
        name : '__builtin_expect(...)'),
    'WIN32' : is_win32,
  },
  install     : true,
  install_dir : join_paths('include', llzs_include_subdir))

subdir('src')

pkg_mod = import('pkgconfig')
cmake = import('cmake')
fs = import('fs')

pkg_mod.generate(
  libraries   : [libowlevelzs],
  version     : meson.project_version(),
  name        : 'libowlevelzs',
  filebase    : 'lowlevelzs',
  description : 'Zscheile Lowlevel (utility) library')

# backwards compatibility
cmake.configure_package_config_file(
  name        : 'LowlevelZS',
  input       : 'LowlevelZSConfig.cmake.in',
  install_dir : install_library_middir + '/cmake/LowlevelZS',
  configuration : configuration_data({
    'include_path' : '${PACKAGE_PREFIX_DIR}/' + install_include_middir,
    'library_path' : '${PACKAGE_PREFIX_DIR}/' + install_library_middir + '/' + fs.name(libowlevelzs.full_path()),
  }))
