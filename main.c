// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include "main.h"
#include "path.h"
// PARSE User args into a list of commands to be processed by the shell.
int parse_uargs( char** user_args, int num_args ) {
  //TODO
}

int main( int cargs, char** argv ) {

  char* curr_dir = malloc(PATH_MAX);
  char* pathenv = malloc(MAX_PATH_LENGTH);

  if ( curr_dir == NULL ) {
    perror("ERROR: unable to malloc curr_dir ");
    return 1;
  }

  //get current working directory
  if ( getcwd(curr_dir, PATH_MAX) != NULL ) {
    printf("Current directory is: %s\n", curr_dir);
  }
  else {
    perror("ERROR: getcwd failed ");
    return 1;
  }

  // store current system PATH in the ENV_PATH file
  pathenv = getenv("PATH");
  if ( pathenv == NULL ) {
    perror("ERROR: no match for PATH in process environment ");
    return 1;
  }
  if ( set_pathenv(SHELL_PATH, pathenv) == -1 ) {
    perror("ERROR: failure to write to ENV_PATH ");
    return 1;
  }

  // test path was written correctly by reading the path from file
  pathenv = get_pathenv(SHELL_PATH);
  if (pathenv == NULL ) {
    perror("ERROR: unable to retreive pathenv from file ");
    return 1;
  }
  printf("Current env PATH is: %s\n", pathenv);

  // usage/help statement
  if ( cargs == 2 && strcmp(argv[1], "-help") == 0 ) {
    print_help();
    return 0;
  }

  // interactive mode
  else if ( cargs == 1 ) {
    printf("Interactive mode.\n");
  }

  // batch mode
  else if ( cargs > 1 ) {
    printf("Batch mode.\n");
  }
  return 0;
}
