/** plg.h
 * (C) 2018 Erik Zscheile

  zsplg functions
    zsplg_open : open's a plugin + init of plugin
      cleanup with zsplg_close even in case of error in zsplg_status
    zsplg_close: deallocates internal vars, deinit plugin, close's a plugin

 */
#pragma once
#include "llzs_config.h"
#include "plg4plugins.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
  ZSP_OK, ZSPE_DLOPN, ZSPE_DLCLOS,
  ZSPE_DLSYM, ZSPE_PLG
} zsplg_status;

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
  bool zsplg_close(zsplg_handle_t *handle);
  bool zsplg_destroy(zsplg_gdsa_t *gdsa);

  zs_attrib(pure)
  zsplg_gdsa_t zsplg_h_create(const zsplg_handle_t *base, size_t argc, char *argv[]);
  zsplg_gdsa_t zsplg_call_h(const zsplg_fncall_t *fndat, void *h_id);
#ifdef __cplusplus
}
#endif
#define zsplg_h_destroy(BASE,ID) zsplg_destroy(&(ID))
#define zsplg_free_ret(BASE,RETVAL) zsplg_destroy(&(RETVAL))
#define zsplg_call(FNDAT) zsplg_call_h((FNDAT), 0)
#define zsplg_gdsa_get(GDSA) ((GDSA).data)
#define zsplg_h_call(FNDAT,ID) zsplg_call_h((FNDAT), zsplg_gdsa_get(ID))
