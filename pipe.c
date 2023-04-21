// CSCE 3600 - major 2 - group 6
// pipe.c
// author:  Nathan Jodoin
// date:    19APR23
// desc:    pipe functional definition

#include "pipe.h"

int parse_pipe_args ( char** cmd_buff[], char* input_args[]) {
  char* argument = (char*)malloc(MAX_ARG_LEN*sizeof(char));
  char** arg_buff = (char**)malloc(MAX_ARG_LEN*sizeof(char**));

  int num_args = 0;
  int num_cmds = 0;

  if ( argument == NULL ) {
    perror("ERROR: Bad malloc in shell_pipe_cmd() ");
    return -1;
  }


  if ( strcmp(input_args[0], "|") == 0) {
    perror("ERROR: Invalid pipe syntax, cannot start with a pipe.");
    return -1;
  }

  for (int i = 0; i < MAX_ARG_LEN; i++) {
    // handle last argument
    if ( input_args[i] != NULL )
      strcpy(argument, input_args[i]);
    else {
      arg_buff[num_args] = (char*) NULL;
      cmd_buff[num_cmds] = arg_buff;
      num_cmds++;
      break;
    }

    // handle args
    if (strcmp(argument, "|") != 0) {
      arg_buff[num_args] = (char*)malloc(MAX_ARG_LEN*sizeof(char));
      strcpy(arg_buff[num_args], argument);
      num_args++;
    }
    else { // split commands on pipe
      arg_buff[num_args] = (char*) NULL;
      cmd_buff[num_cmds] = arg_buff;
      arg_buff = (char**)malloc(MAX_ARG_LEN*sizeof(char**));
      num_args = 0;
      num_cmds++;
    }

  }
  return num_cmds;
}

// Recursively forks and pipes output until no more commands needs be executed.
// RETURNS 0 on SUCCESS and -1 on FAILURE
int pipe_exec( char** pipe_commands[], int num_cmds, int curr, int fd_in ) {

  int fd[2], wstatus, pid = -1;

  if ( pipe(fd) == -1 ) {
    perror("ERROR: Unable to assign pipe in pipe_exec. ");
    return -1;
  }
  pid = fork();
  // child
  if ( pid == 0 ) {
    // if fd passed in
    if ( fd_in != 0 && fd_in != -1 ) {
      // dup fd of pipe read to stdin
      if ( dup2(fd_in, 0) == -1 ) {
        perror("Unable to duplicate file descriptor.");
        close(fd_in);
        close(fd[0]);
        close(fd[1]);
        return -1;
      }
    }
    else if ( fd_in == -1) {
      perror("ERROR: Bad fd passed to pipe_exec. ");
      return -1;
    }

    // dup pipe write over stdout
    if (dup2(fd[1], 1) == -1) {
      // pipe err
      perror("Unable to duplicate file descriptor.");
      close(fd_in);
      close(fd[0]);
      close(fd[1]);
      return -1;
    }

    // close unused fd in child
    close(fd[0]);
    close(fd[1]);

    execvp(*pipe_commands[curr], pipe_commands[curr]);
  }

  // parent
  else if (pid > 0) {

    if ( curr < num_cmds ) {
      pipe_exec(pipe_commands, num_cmds, curr+1, fd[0]);
    }

    waitpid(pid, &wstatus, 0);

    close(fd[0]);
    close(fd[1]);

    if ( !WEXITSTATUS(wstatus) ){
      perror("Warning: One of the piped commands exited with an error. ");
      return -1;
    }
    return 0;
  }

  else { // fork error
    close(fd[0]);
    close(fd[1]);
    perror("ERROR: Unable for fork successfully in pipe_exec. ");
    return -1;
  }
  return 0;
}

int shell_pipe_cmd( char* args[] ) {

  char** cmd_buff[MAX_ARG_LEN];
  int num_cmds;

  num_cmds = parse_pipe_args(cmd_buff, args);

  if ( num_cmds != -1 ) {
    for ( int i = 0; i < num_cmds; i++) {
      for (int j = 0; cmd_buff[i][j] != NULL; j++ ) {
        printf("Cmd[%d]::Arg[%d]: %s\n", i, j, cmd_buff[i][j]);
      }
    }
    pipe_exec(cmd_buff, num_cmds, 0, 0);
  }
  else {
    perror("Unable to execute, bad syntax provided by user. ");
    return -1;
  }

  return 0;
}
