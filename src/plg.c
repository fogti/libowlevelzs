#include "plg.h"
#include "string/xcpy.h"
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

/* TODO: every plugin function should return a struct of type {
  void *ptr;
  void (*destruct)(void * ptr);
} */

static void zsplg_setstdl(zsplg_handle_t *const handle, const zsplg_status st) {
  handle->st        = st;
  handle->error_str = dlerror();
}

zsplg_handle_t zsplg_open(const char * restrict file, const char * restrict modname, const bool do_strcpy) {
  /* load plugin with dlopen */
  zsplg_handle_t ret = {
    .st         = ZSP_OK,
    .modname    = do_strcpy ? strdup(modname) : modname,
    .mnlen      = strlen(modname),
    .dlh        = file ? dlopen(file, RTLD_LAZY | RTLD_LOCAL) : RTLD_DEFAULT,
    .have_alloc = do_strcpy,
  };
  if(zs_unlikely(file && !ret.dlh)) {
    zsplg_setstdl(&ret, ZSPE_DLOPN);
    return ret;
  }

  /* init_fn_name = "init_" modname + "\0" */
  char init_fn_name[ret.mnlen + 6];
  memset(init_fn_name, 0, ret.mnlen + 6);
  llzs_strxcpy(init_fn_name, "init_", 5);
  llzs_strxcpy(init_fn_name + 5, ret.modname, ret.mnlen);

  /* initialize plugin */
  zsplugin_t* (*init_fn)() = dlsym(ret.dlh, init_fn_name);
  if(zs_unlikely(!init_fn)) {
    /* ERROR HERE*/
    zsplg_setstdl(&ret, ZSPE_DLOPN);
    /* cleanup dlh */
    if(file) {
      dlclose(ret.dlh);
      ret.dlh = 0;
    }
  } else {
    ret.plugin = *((*init_fn)());
  }
  return ret;
}

bool zsplg_destroy(zsplg_handle_t *const handle, void *data, const bool is_ret) {
  if(zs_unlikely(!handle)) return false;

  zsplg_status st = handle->st;
  zsplugin_t *const plgptr = &handle->plugin;

  if(st == ZSPE_DLOPN) {
    if(zs_unlikely(data)) {
      /* WTF -- where does data come from?? */
      return false;
    }
    /* plugin isn't fully initialized --> only free leftover mem */
  } else {
    void *x_data;
    zsplg_destr_t drt;

    if(data) {
      /* destroy handle / retval */
      x_data = data;
      drt = is_ret ? ZSPD_RET : ZSPD_HANDLE;
    } else {
      /* deinitialize plugin */
      x_data = plgptr->data;
      drt = ZSPD_PLG;
    }

    const bool ret = plgptr->fn_destroy(x_data, drt);
    if(!ret) st = ZSPE_PLG;
    if(data) {
      handle->st = st;
      return ret;
    }
    plgptr->data = 0;

    /* unload plugin */
    if(handle->dlh) {
      const int tmp = dlclose(handle->dlh);
      if(zs_unlikely(tmp)) {
        if(st == ZSP_OK) st = ZSPE_DLCLOS;
        handle->error_str = dlerror();
      } else {
        handle->dlh = 0;
      }
    }
  }

  if(handle->have_alloc) {
    free((void*)handle->modname);
    handle->modname = 0;
  }

  return ZSP_OK == (handle->st = st);
}

zs_attrib(pure)
void * zsplg_h_create(const zsplg_handle_t *const base, size_t argc, char *argv[]) {
  const zsplugin_t *const plgptr = &base->plugin;
  return plgptr->fn_h_create(plgptr->data, argc, argv);
}

static bool zsplg_upd_errstr(zsplg_handle_t *const handle) {
  const char *const tmp = dlerror();
  if(zs_unlikely(tmp)) handle->error_str = tmp;
  return tmp;
}

void * zsplg_h_call(const zsplg_fncall_t *const fndat, void *const h_id) {
  /* handle error conditions */
  if(zs_unlikely(!fndat))
    return 0;

  zsplg_handle_t *const handle = fndat->plgh;
  const char     *const fn     = fndat->fn;

  if(zs_unlikely(!handle || !fn || handle->st == ZSPE_DLOPN))
    return 0;

  /* construct function name */
  const size_t fnlen = strlen(fn);
  const size_t xfnlen = handle->mnlen + fnlen + 3 + (h_id ? 1 : 0);
  char xfn_name[xfnlen];
  memset(xfn_name, 0, xfnlen);
  {
    char *xnp = llzs_strxcpy(xfn_name, handle->modname, handle->mnlen);
             *(xnp++) = '_';
    if(h_id) *(xnp++) = 'h';
    llzs_strxcpy(stpcpy(xnp, "_"), fn, fnlen);
  }

  /* get function addr */
  zsplg_upd_errstr(handle);
  void * (*xfn_ptr)(void *, size_t, char *const*) = dlsym(handle->dlh, xfn_name);

  if(zs_unlikely(!xfn_ptr)) {
    if(zsplg_upd_errstr(handle))
      handle->st = ZSPE_DLSYM;
    return 0;
  }

  /* call function */
  return xfn_ptr(h_id ? h_id : handle->plugin.data, fndat->argc, fndat->argv);
}
