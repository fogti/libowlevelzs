#define _ZS_PLUGIN__
#define _GNU_SOURCE
#include <zs/ll/dtors4plugins.h>
#include <zs/ll/plg4plugins.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** 'file' plugin
   funcs:
     _init    initializes plugin
     :create  open file handle
     :destroy close handles, free retvals, cleanup plugin
     .getchar @ret { S, V, '\0' },
              S = 'e' --> EOF, 'd' --> data
              V = (char) fgetc(...)
     .getline @ret string, read with getline
     .is_open probably useless function
              @ret '1\0' or '0\0', depending on handle val
     .puts    @args
                c2: "(\")-(\")" "\"STR\"" -- put string without quotes
                c1: "STR" -- put string directly
              @ret { S, '\0' }
                S = 'i' --> invalid invocation
                    'e' --> fputs failed
                    '\0' --> OK
 */

static zsplg_gdsa_t h_create(void *data, size_t argc, const char *argv[]) {
  if(argc != 2) RET_GDSA_NULL;
  RET_GDSA(fopen(argv[0], argv[1]), _Z10do_destroyP8_IO_FILE);
}

static inline zsplg_gdsa_t pack_ret(char * const ret) {
  RET_GDSA(ret, ret ? _Z10do_destroyPv : 0);
}
#define RET_PACK return pack_ret(ret)

zsplg_gdsa_t file_h_getchar(FILE *fh, const size_t argc, const char *argv[]) {
  char * ret = malloc(3);
  if(ret) {
    const int nxc = fgetc(fh);
    ret[0] = (nxc == EOF) ? 'e' : 'd';
    ret[1] = nxc;
    ret[2] = 0;
  }
  RET_PACK;
}

zsplg_gdsa_t file_h_getline(FILE *fh, const size_t argc, const char *argv[]) {
  char * ret = 0;
  size_t n = 0;
  getline(&ret, &n, fh);
  RET_PACK;
}

zsplg_gdsa_t file_h_is_open(FILE *fh, const size_t argc, const char *argv[]) {
  char    *ret = calloc(2, 1);
  if(ret) *ret = '0' + (fh?1:0);
  RET_PACK;
}

zsplg_gdsa_t file_h_puts(FILE *fh, const size_t argc, const char *argv[]) {
  char * ret = calloc(2, 1);
  if(!ret) {
    // do nothing
  } else if(argc < 1) {
    // not enough arguments
    ret[0] = 'i';
  } else {
    const char *const tmp =
      (argc == 2 && (!strcmp(argv[0], "-") || !strcmp(argv[0], "\"-\"")) && argv[1][0])
      ?
        // unquote string before writing
        // remove last quote-mark, print with inc 1
        strndupa(&argv[1][1], strlen(argv[1]) - 2)
      :
        *argv;
    if(EOF == fputs(tmp, fh))
      ret[0] = 'e';
  }
  RET_PACK;
}

zsplugin_t * init_file() {
  static zsplugin_t plg = {
    .data        = ZS_GDSA(0, 0),
    .fn_h_create = &h_create,
  };
  return &plg;
}
