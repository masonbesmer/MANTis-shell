#include "handle_exit.h"

pid_t shell_pgid;

int setup_exit() {
  //pid_t shell_pgid;
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

  tcsetpgrp(STDIN_FILENO, getpid());

  signal(SIGSTP, handle_exit);
  signal(SIGINT, handle_exit);
  //ctrl d
  return 0;
}
//c
void handle_exit(int signal) {
  printf("signal: %d\n", signal);
  if (tcgetpgrp(STDIN_FILENO) == getpgrp()) {
    //printf("in foreground, exiting");
    tcsetpgrp(STDIN_FILENO, shell_pgid);
    printf("parent put in fg now");
    exit(0);
  } else {
    // do nothing
  }
}

int cmd_fork_template() {
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);

  printf("PARENT: testing. we're about to fork\n");
  pid_t pid;
  pid = fork();
  if (pid == 0){
      // set child process group id to its own pid
      setpgid(0, 0);

      // respect signals in child
      // signal(SIGINT, SIG_DFL);
      // signal(SIGTSTP, SIG_DFL);
      // signal(SIGTTOU, SIG_DFL);
      do {} while(tcgetpgrp(STDIN_FILENO) != getpgrp()); // wait until we are in foreground
      // temp code to not terminate until i tell it to
      printf("CHILD: just forked, i am in foreground\n");
      for(int i = 0; i < 5; i++){
          printf("CHILD: child\n");
          fflush(stdout);
          sleep(1);
      }
      printf("CHILD: child done\n");

      exit(0);
  } else if (pid < 0){
      // fork failed
      perror("ERROR: fork failed");
      return -1;
  } else {
      //ignore term for writing while in background
      signal(SIGTTOU, SIG_IGN);
      //printf("PARENT: place parent in background\n");
      //tcsetpgrp(STDIN_FILENO, getpid());
      printf("PARENT: place child in foreground\n");
      tcsetpgrp(STDIN_FILENO, pid);

      printf("PARENT: wait for child to finish its stuff\n");
      int status;
      waitpid(pid, &status, WUNTRACED);

      printf("PARENT: place parent back in foreground\n");
      tcsetpgrp(STDIN_FILENO, getpgrp());
      signal(SIGTTOU, SIG_DFL);
      printf("PARENT: returning to shell\n");

      // return back to shell loop
      return 0;
  }
}
