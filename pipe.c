// CSCE 3600 - major 2 - group 6
// pipe.c
// author:  Nathan Jodoin
// date:    19APR23
// desc:    pipe functional definition

#include "pipe.h"

int parse_pipe_args ( char** cmd_buff[], char* input_args[]) {

  char** arg_buff = (char**)malloc(MAX_ARG_LEN*sizeof(char**));

  int num_args = 0;
  int num_cmds = 0;

  if ( arg_buff == NULL ) {
    perror("ERROR: Bad malloc in shell_pipe_cmd() ");
    return -1;
  }
  if ( strcmp(input_args[0], "|") == 0) {
    perror("ERROR: Invalid pipe syntax, cannot start with a pipe.");
    return -1;
  }

  for (int i = 0; i < MAX_ARG_LEN; i++) {

    // handle last input_args[i]
    if ( input_args[i] == NULL) {
      arg_buff[num_args] = (char*) NULL;
      cmd_buff[num_cmds] = arg_buff;
      cmd_buff[num_cmds+1] = (char**) NULL;
      num_cmds++;
      break;
    }

    // handle args normally (curr arg is not pipe)
    if (strcmp(input_args[i], "|") != 0) {
      arg_buff[num_args] = (char*)malloc((strlen(input_args[i])+1)*sizeof(char));
      strcpy(arg_buff[num_args], input_args[i]);
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

// Recursive pipe command execution function.
// Expects: array of args arrays (NULL term for exec), index (start at 0)
// fd of input for 1st cmd, fd of output for redirection if needed
// Returns: an integer status
// of the last command, enabling further redirection if necessary, and
// -1 on error. fd_in and fd_out default to 0 and 1 respectively.
int pipe_exec( char** cmds[], int index, int in, int out ) {

  int status, pipe_fd[2];

  // Pipe config error.
  if ( pipe(pipe_fd) == -1 ) {
    perror("ERROR: Unable to create pipe.");
    return -1;
  }

  // Base Case
  if ( cmds[index] == NULL ) {
    return 0;
  }

  // Begin Forking...
  pid_t pid = fork();

  if ( pid < 0 ) { // Fork Syscall Error
    perror("ERROR: Bad fork.");
    return -1;
  }
  else if ( pid == 0 ) { // Child
    if (in != -1 && in != 0) {
      dup2(in, STDIN_FILENO);
      close(in);
    }
    close(pipe_fd[0]);
    if (cmds[index + 1] != NULL) {
      dup2(pipe_fd[1], STDOUT_FILENO);
    }
    else {
      dup2(out, STDOUT_FILENO);
    }
    close(pipe_fd[1]);

    execvp(cmds[index][0], cmds[index]);
    perror("ERROR: Exec error. ");
    return -1;
  }
  else // Parent
  {
    close(pipe_fd[1]);

    if (in != -1 && in != 0) {
      close(in);
    }
    if (pipe_exec(cmds, index + 1, pipe_fd[0], 1) == -1 ){
      return -1;
    }

    waitpid(pid, &status, 0);
    if ( WEXITSTATUS(status) != 0 ) {
      char message[1024];
      sprintf(message,
        "Pipe fail at \"%s\"; exit code %d.", cmds[index][0], status);
      perror(message);
      return -1;
    }
    return 0;
  }
  return 0;
}

int shell_pipe_cmd( char* args[] ) {

  char** cmd_buff[MAX_ARG_LEN];
  int num_cmds;

  num_cmds = parse_pipe_args(cmd_buff, args);
  if ( num_cmds != -1 ) {
    pipe_exec(cmd_buff, 0, STDIN_FILENO, STDOUT_FILENO);
  }
  else {
    perror("Unable to execute, bad syntax provided by user. ");
    return -1;
  }

  for ( int i=0; i < num_cmds; i++ ){
    int j=0;
    while(cmd_buff[i][j] != NULL){
      free(cmd_buff[i][j]);
      j++;
    }
    free(cmd_buff[i]);
  }

  return 0;
}
