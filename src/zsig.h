/** zsparsell zsig.h
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "cxa_noexcept.h"
#include "llzs_config.h"
#ifdef __cplusplus
extern "C" {
#endif
  typedef void (*sighandler_t)(int);
  void my_signal(int sig_nr, sighandler_t signalhandler) noexcept;
#ifndef WIN32
  void my_signal_norestart(const int sig_nr, const sighandler_t signalhandler) noexcept;
#endif
#ifdef __cplusplus
}
#endif
