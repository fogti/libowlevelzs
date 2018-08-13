#define _ZS_PLUGIN__
#include <zs/ll/dtors4plugins.h>
#include <zs/ll/plg4plugins.h>
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
  size_t rcln, cln, cosiz;
} spawn_handle_t;

static bool _Z10do_destroyP14spawn_handle_t(spawn_handle_t *const handle) {
  free(handle->cmd);
  free(handle->caout);
  free(handle);
  return true;
}

static zsplg_gdsa_t h_create(void *data, size_t argc, const char *argv[]) {
  if(argc != 1) RET_GDSA_NULL;
  spawn_handle_t * ret = calloc(1, sizeof(spawn_handle_t));
  ret->cln = 1 + (ret->rcln = strlen(argv[0]));
  ret->cmd = llzs_strxdup(argv[0], ret->rcln);
  if(zs_unlikely(!ret->cmd)) {
    free(ret);
    ret = 0;
  }
  RET_GDSA(ret, _Z10do_destroyP14spawn_handle_t);
}

zsplugin_t * init_spawn() {
  static zsplugin_t plg = {
    .data        = ZS_GDSA(0, 0),
    .fn_h_create = &h_create,
  };
  return &plg;
}

/* HELPER FUNCTIONS */
static bool zs_xxx_tosiz(char ** restrict stptr, size_t * stsiz, const size_t trgsiz) {
  if(*stsiz <= trgsiz || !*stptr) {
    const size_t olsiz = *stsiz;
    const size_t nwsiz = (olsiz ? (olsiz * 2) : 1) + trgsiz;
    if(zs_unlikely(!(*stptr = realloc(*stptr, nwsiz)))) {
      *stsiz = 0;
      return false;
    }
    *stsiz = nwsiz;
  }
  (*stptr)[trgsiz] = 0;
  return true;
}

typedef void (*zs_spap_aph_fnt)(char ** restrict cmpp, const char * restrict arg, size_t arglen);
static bool zs_spap_appenh(spawn_handle_t * restrict sph, const size_t arglen, const size_t aaplen,
              const char * restrict arg, const zs_spap_aph_fnt fn) {
  zs_xxx_tosiz(&sph->cmd, &sph->cln, sph->rcln + arglen + aaplen);
  if(zs_unlikely(!sph->cmd)) return false;
  char * cmdptr = sph->cmd + sph->rcln;
  fn(&cmdptr, arg, arglen);
  sph->rcln = (cmdptr - sph->cmd);
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
  return zs_spap_appenh(sph, strlen(arg), 2, arg, &zs__spsh);
}

static bool zs_spap_quote(spawn_handle_t *sph, const char *arg) {
  size_t n = strlen(arg);
  const char * tmp;
  { /* only call llzs_streplace if needed */
    const char * const eoas = arg + n;
    for(tmp = arg; tmp != eoas; ++tmp)
      if(*tmp == '\'') {
        tmp = llzs_streplace(arg, "'", "\\\\'", &n);
        goto cont_tmp;
      }
  }
  tmp = arg;

 cont_tmp:
  if(!tmp) return false;
  /* 4 = strlen(" \"\"\0")
     CMD += "\""+TMP+"\"" */
  const bool ret = zs_spap_appenh(sph, n, 4, tmp, &zs__spqh);
  if(tmp != arg) free((void*)tmp);
  return ret;
}

#define zs_caout_tosiz(SPH,TS) zs_xxx_tosiz(&SPH->caout, &SPH->cosiz, (TS))

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

static int zs_do_exech(spawn_handle_t *sph, const bool co) {
  if(!co)                                return system(sph->cmd);
  if(!zs_caout_tosiz(sph, 0))            return -1;
  FILE *my_pipe = popen(sph->cmd, "r");
  if(!my_pipe)                           return -1;

  char buf[1024];
  size_t pos = 0;
  while(fgets(buf, sizeof(buf), my_pipe)) {
    const size_t readlen = strlen(buf), enpos = pos + readlen;
    if(!zs_caout_tosiz(sph, enpos))      { pclose(my_pipe); return -1; }
    llzs_strxcpy(sph->cmd + pos, buf, readlen);
    pos = enpos;
  }
  const int ret = pclose(my_pipe);
  if(!zs_caout_shtf(sph))                return -1;
  return ret;
}

static int zs_do_exec(spawn_handle_t *sph, const bool co) {
  const int ret = zs_do_exech(sph, co);
  if(ret == -1) return ret;
  return (WIFSIGNALED(ret))
    ? (256 + WTERMSIG(ret))
    :     WEXITSTATUS(ret);
}

/* (HANDLE) MEMBER FUNCTIONS */
zsplg_gdsa_t spawn_h_ap(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  size_t argi = 0;
  bool do_quote = true;
  if(argc >= 2 && argv[0][0] == '\\' && argv[0][1] && !argv[0][2])
    switch(argv[0][1]) {
      case '-': do_quote = false;
      case 'q': ++argi;
      default : break;
    }

  for(; argi < argc; ++argi)
    if(!((do_quote ? zs_spap_quote : zs_spap_simple)(sph, argv[argi])))
      RET_GDSA_NULL;
  RET_GDSA("-", 0);
}

zsplg_gdsa_t spawn_h_ga(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  RET_GDSA(llzs_strxdup(sph->cmd, sph->cln), _Z10do_destroyPv);
}

zsplg_gdsa_t spawn_h_exec(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  const bool co = (argc == 1 && !strcmp(argv[0], "1"));
  int * ret = malloc(sizeof(int));
  if(zs_likely(ret)) *ret = zs_do_exec(sph, co);
  RET_GDSA(ret, _Z10do_destroyPv);
}

zsplg_gdsa_t spawn_h_caout(spawn_handle_t *sph, const size_t argc, const char *argv[]) {
  RET_GDSA(llzs_strxdup(sph->caout, sph->cosiz), _Z10do_destroyPv);
}
