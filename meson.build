project('Zscheile Lowlevel (utility) library', 'c', 'cpp',
  default_options : ['c_std=c11', 'cpp_std=c++17'],
  version : '0.1.2',
  license : 'MIT')

llzs_include_subdir = join_paths('zs', 'll')

compiler = meson.get_compiler('c')

configure_file(
  input : 'llzs_config.h.in',
  output : 'llzs_config.h',
  configuration : {
    'HAVE_ATTRIBUTE' :
      compiler.compiles('''
static int __attribute__((pure)) zs_st_pure_func(const int x) { return 2 * x; }
int main(void) { return zs_st_pure_func(0); }
''',
        name : 'check for __attribute__(())'),
    'HAVE_BUILTIN_EXPECT' :
      compiler.compiles('int main(void) { return __builtin_expect(0, 1); }',
        name : 'check for __builtin_expect'),
    'WIN32' : host_machine.system() == 'windows',
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
