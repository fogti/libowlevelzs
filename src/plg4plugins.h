/** plg4plugins.h
 * (C) 2018 Erik Zscheile

  expected plugin functions
    zsplugin_t * MODNAME_init()
    void * MODNAME__XFN(void * data, size_t argc, char *argv[])
    void * MODNAME_h_XFN(void * handle, size_t argc, char *argv[])

 */
#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct zsplg_gdsa {
  /* destroy invoc: .destroy(.data) */
  void* data;
  bool (*destroy)(void *data);
} zsplg_gdsa_t;

typedef struct {
  zsplg_gdsa_t data;
  zsplg_gdsa_t (*fn_h_create)(void *data, size_t argc, char *argv[]);
} zsplugin_t;

#ifdef _ZS_PLUGIN__
# include "llzs_config.h"
# ifdef __cplusplus
extern "C" {
# endif
  extern const zsplg_gdsa_t zsplg_gdsa_null;
# ifdef __cplusplus
}
# endif
# define INLINE_GDSA(X, Y) ((zsplg_gdsa_t) { (X), (Y) })
zs_attrib(const) zs_attrib(hot)
static inline zsplg_gdsa_t zsplg_make_gdsa(void *data, bool (*destroy)(void *data)) {
  zsplg_gdsa_t ret = INLINE_GDSA(data, destroy);
  return ret;
}
# define RET_GDSA(X,Y) return zsplg_make_gdsa((X), (Y))
#endif
