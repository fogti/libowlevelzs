#define _ZS_PLUGIN__
#include "llzs_config.h"
#include <zs/ll/dtors4plugins.h>
#include <zs/ll/plg4plugins.h>
#include <zs/ll/string/replace.hpp>
#include <zs/ll/string/xcpy.h>

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
  RET_GDSA(ret, sizeof(spawn_handle_t), _Z10do_destroyP14spawn_handle_t);
}

zsplugin_t * init_spawn() {
  static zsplugin_t plg = {
    .data        = ZS_GDSA_NULL,
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

typedef struct {
  const size_t aaplen; /* strlen(s_pre) = aaplen >> 1; strlen(l_post) = aaplen >> 2; */
  const char * restrict s_pre, * restrict s_post;
} appenh_dat;

static bool zs_spap_appenh(spawn_handle_t * restrict sph, const size_t arglen,
              const char * restrict arg, const appenh_dat * restrict dat) {
  const size_t aaplen = dat->aaplen;
  zs_xxx_tosiz(&sph->cmd, &sph->cln, sph->rcln + arglen + aaplen);
  if(zs_unlikely(!sph->cmd)) return false;
  char * cmdptr = sph->cmd + sph->rcln;
  llzs_strixcpy(&cmdptr, dat->s_pre, aaplen >> 1);
  llzs_strixcpy(&cmdptr, arg, arglen);
  llzs_strixcpy(&cmdptr, dat->s_post, aaplen >> 2);
  sph->rcln = (cmdptr - sph->cmd);
  return true;
}

static bool zs_spap_simple(spawn_handle_t *sph, register const char *arg) {
  appenh_dat dat = { 2, " ", 0 };
  return zs_spap_appenh(sph, strlen(arg), arg, &dat);
}

static bool zs_spap_quote(spawn_handle_t *sph, register const char *arg) {
  size_t n = strlen(arg);
  const char * tmp;
  { /* only call llzs_streplace if needed */
    for(tmp = arg; tmp != arg + n; ++tmp)
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
  {
    const appenh_dat dat = { 4, " \"", "\"" };
    const bool ret = zs_spap_appenh(sph, n, tmp, &dat);
    if(tmp != arg) free((void*)tmp);
    return ret;
  }
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
    const size_t readlen = strlen(buf);
    if(!zs_caout_tosiz(sph, pos + readlen))
      { pclose(my_pipe); return -1; }
    llzs_strxcpy(sph->cmd + pos, buf, readlen);
    pos += readlen;
  }
  const int ret = pclose(my_pipe);
  if(!zs_caout_shtf(sph))                return -1;
  return ret;
}

static int zs_do_exec(spawn_handle_t *sph, const bool co) {
  const int ret = zs_do_exech(sph, co);
  if(ret == -1) return -1;
  return  WIFSIGNALED(ret)
    ? (256 + WTERMSIG(ret))
    :     WEXITSTATUS(ret);
}

/* (HANDLE) MEMBER FUNCTIONS */
#define SPH_FN(N) zsplg_gdsa_t spawn_h_##N(spawn_handle_t *sph, const size_t argc, const char *argv[])
SPH_FN(ap) {
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
  RET_GDSA("-", 2, 0);
}

SPH_FN(ga) {
  return llzs_strxdup_gdsa(sph->cmd, sph->cln);
}

SPH_FN(exec) {
  int * ret = malloc(sizeof(int));
  if(zs_likely(ret))
    *ret = zs_do_exec(sph, argc == 1 && argv[0][0] == '1' && !argv[0][1]);
  RET_GDSA(ret, sizeof(int), _Z10do_destroyPv);
}

SPH_FN(caout) {
  return llzs_strxdup_gdsa(sph->caout, sph->cosiz);
}
