/** plg.h
 * (C) 2018 Erik Zscheile

  expected plugin functions
    zsplugin_t * MODNAME_init()
    void * MODNAME__XFN(void * data, size_t argc, char *argv[])
    void * MODNAME_h_XFN(void * handle, size_t argc, char *argv[])

  zsplg functions

    zsplg_open : open's a plugin + init of plugin
      cleanup with zsplg_close even in case of error in zsplg_status
    zsplg_close: deallocates internal vars, deinit plugin, close's a plugin

 */
#pragma once
#include "llzs_config.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
  ZSP_OK, ZSPE_DLOPN, ZSPE_DLCLOS,
  ZSPE_DLSYM, ZSPE_PLG
} zsplg_status;

typedef enum {
  ZSPD_PLG, ZSPD_HANDLE, ZSPD_RET
} zsplg_destr_t;

typedef struct {
  bool  (*fn_destroy)(void *data, zsplg_destr_t dt);
  void* (*fn_h_create)(void *data, size_t argc, char *argv[]);
  void* data;
} zsplugin_t;

typedef struct {
  zsplugin_t plugin;
  zsplg_status st;
  size_t mnlen;
  const char *modname, *error_str;
  void *dlh;
  bool have_alloc;
} zsplg_handle_t;

typedef struct {
  zsplg_handle_t *plgh;
  const char *fn;
  size_t argc;
  char *const *argv;
} zsplg_fncall_t;

#ifdef __cplusplus
extern "C" {
#endif
  zsplg_handle_t zsplg_open(const char * __restrict__ file, const char * __restrict__ modname, bool do_strcpy);
  bool zsplg_destroy(zsplg_handle_t *handle, void *data, bool is_ret);

  zs_attrib(pure)
  void * zsplg_h_create(const zsplg_handle_t *base, size_t argc, char *argv[]);
  void * zsplg_h_call(const zsplg_fncall_t *fndat, void *h_id);
#ifdef __cplusplus
}
#endif
#define zsplg_close(HANDLE) zsplg_destroy((HANDLE), 0, false)
#define zsplg_h_destroy(BASE,ID) zsplg_destroy((BASE), (ID), false)
#define zsplg_free_ret(BASE,RETVAL) zsplg_destroy((BASE), (RETVAL), true)
#define zsplg_call(FNDAT) zsplg_h_call((FNDAT), 0)
