/* second part of ../plgx.cxx */
#include <zs/ll/plg.h>

struct spawn_opaque { };

int main() {
  zsplg_handle_t plg = zsplg_open(0, "spawn", false);
  if(plg.error_str) return 1;
  {
    const char * my_argv[] = { "echo", 0 };
    zsplg_gdsa_t handle = zsplg_h_create(&plg, 1, my_argv);
    auto uptr = llzs::zsplg::gdsa2unique_ptr<spawn_opaque>(handle);
    // uptr is automatically free'd
  }
  zsplg_close(&plg);
  return 0;
}
