#include <zs/ll/plgmm.hpp>
#include <stdio.h>
#include <stdlib.h>

static void my_errmsg(const char *msg) {
  fprintf(stderr, "echo test: ERROR: %s\n", msg);
  exit(1);
}

using namespace llzs;

int main(int argc, const char *argv[]) {
  zsplg::plugin plg(0, "spawn", false);
  if(plg.error_str()) {
    fprintf(stderr, "echo test: ERROR: %s\n", plg.error_str());
    return 1;
  }

  zsplg::handle handle(plg, "echo");
  auto got_ret = handle.call_argv("ap", argc - 1, argv + 1);
  if(!got_ret.get()) my_errmsg("spawn.ap failed");

  got_ret = handle("exec", nullptr);
  if(!got_ret.get()) my_errmsg("spawn.exec failed");
  return *static_cast<int*>(got_ret.get());
}
