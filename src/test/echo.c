#include <zs/ll/plg.h>
#include <stdio.h>
#include <stdlib.h>

static void my_errmsg(const char *msg) {
  fprintf(stderr, "echo test: ERROR: %s\n", msg);
  exit(1);
}

int main(int argc, const char *argv[]) {
  zsplg_handle_t plg = zsplg_open(0, "spawn", false);
  if(plg.error_str) {
    zsplg_close(&plg);
    my_errmsg(plg.error_str);
  }

  const char * my_argv[] = { "echo", 0 };
  zsplg_gdsa_t handle = zsplg_h_create(&plg, 1, my_argv);
  zsplg_gdsa_t got_ret;
  int retcode = 0;

  /* zsplg pseudo-code
    plg = plugin.open "spawn"
    handle = plg.create "echo"
    handle.ap @(argc,argv)
    return handle.exec
   */
  {
    got_ret = zsplg_h_call(&plg, handle, "ap", argc - 1, argv + 1);
    if(!zsplg_gdsa_get(got_ret)) my_errmsg("spawn.ap failed");
    zsplg_destroy(&got_ret);
    my_argv[0] = 0;
    got_ret = zsplg_h_call(&plg, handle, "exec", 0, my_argv);
    if(!zsplg_gdsa_get(got_ret)) my_errmsg("spawn.exec failed");
    retcode = *((int*)zsplg_gdsa_get(got_ret));
    zsplg_destroy(&got_ret);
  }

  zsplg_destroy(&handle);
  zsplg_close(&plg);
  return retcode;
}
