#include <zs/ll/plg.h>
#include <stdio.h>
#include <stdlib.h>

static void my_errmsg(const char *msg) {
  fprintf(stderr, "echo test: ERROR: %s\n", msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  zsplg_handle_t plg = zsplg_open(0, "spawn", false);
  if(plg.error_str) {
    zsplg_close(&plg);
    my_errmsg(plg.error_str);
  }

  char * my_argv[] = { "echo", 0 };
  void * handle = zsplg_h_create(&plg, 1, my_argv);
  void * got_ret;
  int retcode = 0;

  /* zsplg pseudo-code
    plg = plugin.open "spawn"
    handle = plg.create "echo"
    handle.ap @(argc,argv)
    return handle.exec
   */
  {
    zsplg_fncall_t hfna = {
      .plgh = &plg,
      .fn   = "ap",
      .argc = argc - 1,
      .argv = argv + 1
    };
    got_ret = zsplg_h_call(&hfna, handle);
    if(!got_ret) my_errmsg("spawn.ap failed");
    zsplg_free_ret(&plg, got_ret);
    hfna.fn = "exec";
    hfna.argc = 0;
    my_argv[0] = 0;
    hfna.argv = my_argv;
    got_ret = zsplg_h_call(&hfna, handle);
    if(!got_ret) my_errmsg("spawn.exec failed");
    retcode = *((int*)got_ret);
    zsplg_free_ret(&plg, got_ret);
  }

  zsplg_h_destroy(&plg, handle);
  zsplg_close(&plg);
  return retcode;
}
