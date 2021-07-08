project('Zscheile Lowlevel (utility) library', 'c', 'cpp',
  default_options : ['cpp_std=c++17'],
  version : '0.1.2',
  license : 'ISC')

llzs_include_subdir = join_paths('zs', 'll')

cc = meson.get_compiler('c', native : false)

is_win32 = host_machine.system() == 'windows'

configure_file(
  input : 'llzs_config.h.in',
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
  install: true,
  install_dir : join_paths('include', llzs_include_subdir))

subdir('src')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  libraries : [libowlevelzs],
  version : '0.1.2',
  name : 'libowlevelzs',
  filebase : 'lowlevelzs',
  description : 'Zscheile Lowlevel (utility) library')
