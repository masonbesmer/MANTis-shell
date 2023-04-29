// CSCE 3600 - Major 2 - Group 6
// handle_exit.c
// author:  Mason Besmer
// date:    28APR23
// desc:    Pass exit signals to foreground process

#include "handle_exit.h"

pid_t shell_pgid;

// @brief Setup the shell to handle signals, and set up process groups.
// @return 0 on success, -1 on failure
int setup_exit() {
  int shell_terminal;
  // put shell in to its own process group
  shell_pgid = getpid();
  if (setpgid(shell_pgid, shell_pgid) < 0) {
    perror("setpgid");
    return -1;
  }

  // get terminal process associated with the shell
  shell_terminal = STDIN_FILENO;
  // kill until in foreground
  while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp())) {
    kill(-shell_pgid, SIGTTIN);
  }

  // set shell to foreground (probably redundant)
  tcsetpgrp(STDIN_FILENO, getpid());
  // ignore respective signals, inherited by child so custom handler needed
  signal(SIGSTP, handle_exit); // ctrl z
  signal(SIGINT, handle_exit); // ctrl c
  signal(SIGQUIT, handle_exit); // ctrl d
  return 0;
}

// @brief Pass signal to foreground process.
void handle_exit(int sig_num) {
  printf("\nCaught signal %d\n", sig_num);
  // if this pgrp is in the foreground
  pid_t this_pgrp = getpgrp();
  pid_t fg_pgrp = tcgetpgrp(STDIN_FILENO);
  if (fg_pgrp == this_pgrp) {
    // reset handlers to default to stop infinite loop
    signal(SIGSTP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    // handle respective signal
    switch (sig_num) {
      case SIGSTP:
        printf("\nCaught ^Z (SIGSTP)\n");
        break;
      case SIGINT:
        printf("\nCaught ^C (SIGINT)\n");
        break;
      case SIGQUIT:
        printf("\nIgnoring ^D (SIGQUIT)\n");
        return;
      default:
        printf("\nCaught unhandled signal, doing nothing.\n");
        return;
    }
    fflush(stdout);
    kill(this_pgrp, sig_num); // send signal to this process group
    // if the foreground is not the shell and this is the shell pgrp
  } else if (fg_pgrp != shell_pgid && this_pgrp == shell_pgid && sig_num != SIGQUIT) {
    tcsetpgrp(STDIN_FILENO, shell_pgid); // set shell to foreground
  } else {
    printf("Unexpected signal behaviour. Doing nothing.\n");
    // do nothing
  }
}
