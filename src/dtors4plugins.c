#include "dtors4plugins.h"

bool _Z10do_destroyP8_IO_FILE(FILE *f)
  { return 0 == fclose(f); }

bool _Z10do_destroyPv(void *ptr)
  { free(ptr); return true; }
