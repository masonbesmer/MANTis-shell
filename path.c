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
  char* temp_path;
  char path_buffer[MAX_PATH_LENGTH] = "";

  temp_path = getenv("PATH");
  if (temp_path == NULL){
    perror("Unable to retrieve path.");
    return -1;
  }
  strcpy(path_buffer, temp_path);
  strcat(path_buffer, ":");
  strcat(path_buffer, str);
  setenv("PATH", path_buffer, 1);
  printf("Appended \"%s\" to PATH.\n", str);

  return 0;
}

// remove string from path
int remove_from_path(char* str) {
  int found = 0;
  char* temp_path;
  char* token;
  char path_buffer[MAX_PATH_LENGTH] = "";

  temp_path = getenv("PATH");
  if (temp_path == NULL){
    perror("Unable to retrieve path.");
    return -1;
  }

  token = strtok(temp_path, ":");

  while ( token != NULL ) {

    if ( strcmp(token, str) != 0 ) {
      strcat(path_buffer, token);
      strcat(path_buffer, ":");
      token = strtok(NULL, ":");
    }
    else {
      found = 1;
      token = strtok(NULL, ":");
    }
  }
  // necessary for removing the trailing : caused by the above loop
  path_buffer[strlen(path_buffer) - 1] = '\0';

  if (setenv("PATH", path_buffer, 1) == -1) {
    perror("ERROR: Unable to set path in remove_from_path.");
    return -1;
  }

  printf("%s \"%s\" from PATH.\n", (found)?"Removed":"Could not remove", str);
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

  //count args
  int num_args = 1;
  while ( args[num_args] != NULL )
    num_args++;

  if ( num_args == 1 ) {
    print_path();
    return 0;
  }

  else if (num_args == 3 ) {
    if ( strcmp(args[1], "+") == 0 )
      append_to_path(args[2]);

    else if ( strcmp(args[1], "-") == 0 )
      remove_from_path(args[2]);

    else {
      errno = EINVAL;
      perror("ERROR: Invalid 2nd argument for path builtin. ");
      return -1;
    }

  }

  else if (num_args > 3 || num_args == 2) {
    errno = EINVAL;
    perror("ERROR: Invalid syntax and/or # of arguments for path. ");
    return -1;
  }

  return 0;
}
