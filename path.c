// CSCE 3600 - major 2 - group 6
// path.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    path built-in program source, allows user to change path env

#include "path.h"
#include "main.h"

// if path passed with no args, print the path
int print_path() {
  char* path;
  if ( (path = getenv("PATH")) == NULL) {
      perror("Unable to retrieve path.");
      return -1;
  }
  printf("Current PATH is: %s\n", path);
  return 0;
}

// append string to path
int append_to_path(char* str) {

  return 0;
}

// remove string from path
int remove_from_path(char* str) {

  return 0;
}

// The builtin path function for the shell.
// No args, print path. arg="+string", add :string to path.
// arg="-string", search for and remove ":string" from path.
// Returns 0 for a successful call, -1 for an error.
int shell_path(char* args[]) {
  if ( args[0] == NULL || strcmp(args[0], "path") != 0 ) {
    errno = EINVAL;
    perror("ERROR: Invalid/no arguments passed by shell_cmd.");
    return -1;
  }

  int num_args = 1;
  while ( args[num_args] != NULL )
    num_args++;

  if ( num_args == 1 ) {
    print_path();
    return 0;
  }
  else if (num_args > 3) {
    errno = EINVAL;
    perror("ERROR: Too many arguments for path. ");
    return -1;
  }

  return 0;
}
