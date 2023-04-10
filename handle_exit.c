#include "handle_exit.h"


int handle_exit_signal() {
  pid_t shell_pgid;
  int shell_terminal;
  //put shell in to its own process group
  shell_pgid = getpid();
  if (setpgid(shell_pgid, shell_pgid) < 0) {
    perror("setpgid");
    return -1;
  }

  //get terminal process associated with the shell
  shell_terminal = STDIN_FILENO;
  while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp())) {
    kill(-shell_pgid, SIGTTIN);
  }

  signal(SIGSTP, SIG_IGN);
  signal(SIGINT, SIG_IGN);
  return 0;
}
