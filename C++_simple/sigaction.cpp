// % g++ sigaction.cpp && ./a.out
// zsh: user-defined signal 1  ./a.out
// ===================================

#include <signal.h>
#include <cstddef>

int main(void) {
  struct sigaction my_action;

  my_action.sa_handler = SIG_IGN; // SIG_IGN 	signal is ignored
  my_action.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &my_action, NULL);
  raise(SIGUSR1);  /* Ignored */

  my_action.sa_handler = SIG_DFL; // SIG_DFL 	default signal handling
  sigaction(SIGUSR1, &my_action, NULL);
  raise(SIGUSR1); /* Terminates */

  return 0;
}
