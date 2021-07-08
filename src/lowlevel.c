#include "zsig.h"
#include <signal.h>
zs_attrib(cold)
void my_signal(const int sig_nr, const sighandler_t signalhandler) {
#ifdef WIN32
/* win32 -- source: https://stackoverflow.com/questions/32389905/sigaction-and-porting-linux-code-to-windows */
  signal(signr, signalhandler);
#else
/* normal unix --
  source: http://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap08-004.htm */

  struct sigaction sig;
  sig.sa_handler = signalhandler;
  sig.sa_flags   = SA_RESTART;
  sigemptyset(&sig.sa_mask);
  sigaction(sig_nr, &sig, 0);
#endif
}

#ifndef WIN32
/* this function is currently unsupported on platforms without sigaction */
zs_attrib(cold)
void my_signal_norestart(const int sig_nr, const sighandler_t signalhandler) {
  struct sigaction sig;
  sig.sa_handler = signalhandler;
  sig.sa_flags   = 0;
  sigemptyset(&sig.sa_mask);
  sigaction(sig_nr, &sig, 0);
}
#endif
