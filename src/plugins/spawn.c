#include <zs/ll/plg.h>
#include <zs/ll/string/replace.hpp>
#include <zs/ll/string/xcpy.h>
#include <llzs_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** 'spawn' plugin
  .ap        append arguments to cmdline
      '\\q': quote following args
      '\\-': don't quote following args
  .ga        get current cmdline
  .caout     get catched output
  .exec      execute command
      @args  { #X }
             X .. do_caout = 0 || 1
      @ret   (int*) return value
 */

typedef struct {
  char * cmd, * caout;
  size_t cln, cosiz;
} spawn_handle_t;

static bool do_destroy(void *data, const zsplg_destr_t dt) {
  switch(dt) {
    case ZSPD_HANDLE:
      {
        spawn_handle_t * handle = data;
        free(handle->cmd);
        free(handle->caout);
      }
    case ZSPD_RET:
      free(data);
    default:
      return true;
  }
}

static void *h_create(void *data, size_t argc, char *argv[]) {
  if(argc != 1) return 0;
  spawn_handle_t * ret = calloc(1, sizeof(spawn_handle_t));
  ret->cln = strlen(argv[0]);
  ret->cmd = llzs_strxdup(argv[0], ret->cln);
  if(!ret->cmd) {
    free(ret);
    ret = 0;
  }
  ret->caout = 0;
  ret->cosiz = 0;
  return ret;
}

zsplugin_t * init_spawn() {
  static zsplugin_t plg = {
    .fn_destroy  = &do_destroy,
    .fn_h_create = &h_create,
    .data        = 0
  };
  return &plg;
}

/* HELPER FUNCTIONS */
typedef void (*zs_spap_aph_fnt)(char ** restrict cmpp, const char * restrict arg, size_t arglen);
static bool zs_spap_appenh(spawn_handle_t * restrict sph, const size_t arglen, const size_t aaplen,
              const char * restrict arg, const zs_spap_aph_fnt fn) {
  sph->cmd = realloc(sph->cmd, sph->cln + arglen + aaplen);
  if(!sph->cmd) return false;
  char * cmdptr = sph->cmd + sph->cln;
  fn(&cmdptr, arg, arglen);
  sph->cln = (cmdptr - sph->cmd);
  return true;
}

static void zs__spsh(char ** restrict cmpp, const char * restrict arg, const size_t arglen) {
  llzs_strixcpy(cmpp, " ", 1);
  llzs_strixcpy(cmpp, arg, arglen);
}

static void zs__spqh(char ** restrict cmpp, const char * restrict arg, const size_t arglen) {
  llzs_strixcpy(cmpp, " \"", 2);
  llzs_strixcpy(cmpp, arg, arglen);
  llzs_strixcpy(cmpp, "\"", 1);
}

static bool zs_spap_simple(spawn_handle_t *sph, const char *arg) {
  const size_t arglen = strlen(arg);
  return zs_spap_appenh(sph, arglen, 2, arg, &zs__spsh);
}

static bool zs_spap_quote(spawn_handle_t *sph, const char *arg) {
  size_t n = 0;
  char * tmp = llzs_streplace(arg, "'", "\\\\'", &n);
  if(tmp) {
    /* 4 = strlen(" \"\"\0")
       CMD += "\""+TMP+        */
    if(zs_spap_appenh(sph, n, 4, tmp, &zs__spqh))
      return true;
    else
      free(tmp);
  }

  return false;
}

static bool zs_spap(spawn_handle_t *sph, const char *arg, const bool do_quote) {
  return do_quote
    ? zs_spap_quote(sph, arg)
    : zs_spap_simple(sph, arg);
}

static bool zs_caout_tosiz(spawn_handle_t *sph, const size_t trgsiz) {
  char ** const coptr = &sph->caout;
  if(sph->cosiz <= trgsiz) {
    const size_t nwsiz = sph->cosiz + trgsiz + 1;
    *coptr = realloc(*coptr, nwsiz);
    sph->cosiz = (*coptr) ? nwsiz : 0;
  }
  if(!*coptr) return false;
  (*coptr)[trgsiz] = 0;
  return true;
}

static bool zs_caout_shtf(spawn_handle_t *sph) {
  char ** const coptr = &sph->caout;
  if(!*coptr) return true;
  const size_t trgsiz = strlen(*coptr) + 1;
  if(sph->cosiz > trgsiz) {
    *coptr = realloc(*coptr, trgsiz);
    sph->cosiz = (*coptr) ? trgsiz : 0;
  }
  return *coptr;
}

static int zs_do_exec(spawn_handle_t *sph, const bool co) {
#define ZS_ERROR return -1
  int ret;
  if(!co) {
    ret = system(sph->cmd);
  } else {
    char buf[1024];
    size_t pos = 0;
    if(!zs_caout_tosiz(sph, 0))            ZS_ERROR;
    FILE *my_pipe = popen(sph->cmd, "r");
    if(!my_pipe)                           ZS_ERROR;

    while(fgets(buf, sizeof(buf), my_pipe)) {
      const size_t readlen = strlen(buf), enpos = pos + readlen;
      if(!zs_caout_tosiz(sph, enpos))
        { pclose(my_pipe); ZS_ERROR; }
      llzs_strxcpy(sph->cmd + pos, buf, readlen);
      pos = enpos;
    }
    ret = pclose(my_pipe);
    if(!zs_caout_shtf(sph))                ZS_ERROR;
  }
  return (WIFSIGNALED(ret))
    ? (256 + WTERMSIG(ret))
    :     WEXITSTATUS(ret);
#undef ZS_ERROR
}

/* (HANDLE) MEMBER FUNCTIONS */
const char * spawn_h_ap(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  size_t argi = 0;
  bool do_quote = false;
  if(argc >= 2 && argv[0][0] == '\\' && argv[0][1] && !argv[0][2])
    switch(argv[0][1]) {
      case 'q': do_quote = true;
      case '-': ++argi;
      default : break;
    }

  for(; argi < argc; ++argi)
    if(!zs_spap(sph, argv[argi], do_quote))
      return 0;
  return strdup("-");
}

const char * spawn_h_ga(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  return llzs_strxdup(sph->cmd, sph->cln);
}

int * spawn_h_exec(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  const bool co = (argc == 1 && !strcmp(argv[0], "1"));
  int * ret = malloc(sizeof(int));
  if(ret) *ret = zs_do_exec(sph, co);
  return ret;
}

int * _Z7spawn_hPvjPPKc(spawn_handle_t *sph, const size_t argc, const char *argv[])
  zs_attrib(alias("spawn_h_exec"));

const char * spawn_h_caout(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  return llzs_strxdup(sph->caout, sph->cosiz);
}