// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include "main.h"

#define MAX_LENGTH    512

// PARSE User args into a list of commands to be processed by the shell.
int parse_uargs( char** user_args, int num_args ) {
  //TODO
}

int main( int cargs, char** argv ) {

  char* curr_dir = malloc(PATH_MAX);
  char* pathenv = malloc(MAX_LENGTH);

  //get current working directory
  if (curr_dir == NULL ) {
    perror("Error: unable to malloc curr_dir, ");
    return 1;
  }
  if ( getcwd(curr_dir, PATH_MAX) != NULL ) {
    printf("Current directory is: %s\n", curr_dir);
  }
  else {
    perror("Error: getcwd failed ");
    return 1;
  }

  // store current system PATH
  if ( pathenv == NULL ) {
    perror("Unable to malloc pathenv ");
    return 1;
  }
  else{
    pathenv = getenv("PATH");
    printf("Current env PATH is: %s\n", pathenv);
  }

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
