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
# define ZS_GDSA(X, Y) ((zsplg_gdsa_t) { (X), (bool(*)(void*))(Y) })
# define RET_GDSA(X,Y) return ZS_GDSA((X), (Y))
# define RET_GDSA_NULL RET_GDSA(0, 0)
#endif
