// CSCE 3600 - major 2 - group 6
// pipe.c
// author:  Nathan Jodoin
// date:    19APR23
// desc:    pipe functional definition

#include <stdlib.h>
#include "main.h"

int syntax(int args, int pipes) {
  if (args < pipes ){
    return -1;
  }
  else if (args == 2 & pipes != 1) {
    return -1;
  }
}

int shell_pipe_cmd( char* args[] ) {
  int num_pipes = 0;
  int num_args = 0;
  char* argument = (char*)malloc(MAX_ARG_LEN*sizeof(char));
  char** argbuff1[MAX_ARG_LEN];
  char** argbuff2[MAX_ARG_LEN];

  if ( argument == NULL ) {
    perror("ERROR: Bad malloc in shell_pipe_cmd() ");
    return -1;
  }
  // iterator over args array, validate syntax and count number of pipes

  strcpy(argument, args[num_args]);

  while ( argument != NULL ) {
    num_args++;
    strcpy(argument, args[num_args]);
    // if pipe command must follow, else ERROR


  }

  // if syntax valid iteratively exec with stdout of cmd1 to std in of cmd2

  return 0;
}
