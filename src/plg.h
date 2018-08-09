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
#ifdef __cplusplus
# include <memory>
#endif

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

  zsplg_gdsa_t zsplg_h_create(const zsplg_handle_t *base, size_t argc, char *argv[]);
  zsplg_gdsa_t zsplg_call_h(const zsplg_fncall_t *fndat, void *h_id);
#ifdef __cplusplus
}

namespace llzs {
namespace zsplg {
  class gdsa_helper_t {
   public:
    typedef bool (*destroyer_t)(void*);
   private:
    const destroyer_t _destroyer;
   public:
    gdsa_helper_t(const destroyer_t destroyer)
      : _destroyer(destroyer) { }
    void operator()(void *const ptr) const noexcept
      { _destroyer(ptr); }
  };

  /* zsplg_gdsa2ptr transfer's ownership
     from gdsa to '@return of zsplg_gdsa2ptr',
     which should be a smart pointer

     @template_param T = expected underlying data type
     */
  template<typename T>
  auto gdsa2unique_ptr(const zsplg_gdsa_t &gdsa) {
    return std::unique_ptr<T, void(*)(void*)>(gdsa.data, gdsa_helper_t(gdsa.destroy));
  }
}
}
#endif
#define zsplg_h_destroy(BASE,ID) zsplg_destroy(&(ID))
#define zsplg_free_ret(BASE,RETVAL) zsplg_destroy(&(RETVAL))
#define zsplg_call(FNDAT) zsplg_call_h((FNDAT), 0)
#define zsplg_gdsa_get(GDSA) ((GDSA).data)
#define zsplg_h_call(FNDAT,ID) zsplg_call_h((FNDAT), zsplg_gdsa_get(ID))
