#include <zs/ll/plg.h>
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

static bool do_destroy(void *data, const zsplg_destr_t dt) {
  switch(dt) {
    case ZSPD_HANDLE:
      fclose((FILE*)data);
      break;
    case ZSPD_RET:
      free(data);
    default:
      break;
  }
  return true;
}

static void* h_create(void *data, size_t argc, char *argv[]) {
  if(argc != 2) return 0;
  return fopen(argv[0], argv[1]);
}

char * file_h_getchar(FILE *fh, const size_t argc, char *argv[]) {
  char * ret = malloc(3);
  if(ret) {
    const int nxc = fgetc(fh);
    ret[0] = (nxc == EOF) ? 'e' : 'd';
    ret[1] = nxc;
    ret[2] = 0;
  }
  return ret;
}

char * file_h_getline(FILE *fh, const size_t argc, char *argv[]) {
  char * ret = 0;
  size_t n = 0;
  getline(&ret, &n, fh);
  return ret;
}

char * file_h_is_open(FILE *fh, const size_t argc, char *argv[]) {
  char * ret = malloc(2);
  if(ret) {
    ret[0] = (fh ? '1' : '0');
    ret[1] = 0;
  }
  return ret;
}

char * file_h_puts(FILE *fh, const size_t argc, char *argv[]) {
  int fpr = 1;
  char * ret = malloc(2);
  if(!ret) return ret;
  ret[0] = 0;
  ret[1] = 0;
  if(argc < 1) {
    // not enough arguments
    ret[0] = 'i';
  } else if(argc == 2 && (!strcmp(argv[0], "-") || !strcmp(argv[0], "\"-\""))) {
    // unquote string before writing
    // remove last quote-mark
    argv[1][strlen(argv[1]) - 1] = 0;
    // print with inc 1
    fpr = fputs(&argv[1][1], fh);
  } else {
    fpr = fputs(argv[0], fh);
  }
  if(fpr == EOF)
    ret[0] = 'e';
  return ret;
}

zsplugin_t * init_file() {
  static zsplugin_t plg = {
    .fn_destroy  = &do_destroy,
    .fn_h_create = &h_create,
    .data        = 0
  };
  return &plg;
}
