// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include "main.h"
#include "path.h"
#include "handle_exit.h"

void print_help() {
    printf(
      "MANTis - an interactive shell by Mason, Alex, Nathan, and Tobi\n"
      "Usage: newshell [batchfile]\n"
    );
}

int main( int cargs, char** argv ) {
  if (handle_exit_signal() == -1) {
    perror("ERROR: unable to handle exit signal ");
    return 1;
  }

  printf(
      // We should see if we can come up with a better name...
      "Welcome to MANTis, A Basic Interactive Shell Built for CSCE3600\n\n"
      );

  size_t userin_len = MAX_ARG_LEN;
  char* curr_dir = malloc(PATH_MAX); // PATH_MAX is a system defined macro
  char* userin = malloc(MAX_ARG_LEN);
  char* pathenv;

  if ( curr_dir == NULL || userin == NULL ) {
    perror("ERROR: unable to malloc during shell init ");
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
  if ( (pathenv = get_pathenv(SHELL_PATH)) == NULL ) {
    perror("ERROR: failure to retreive from ENV_PATH");
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

    printf("\nEnter \"quit\" to exit the shell. \n\n");

    // TODO: Implement exit()
    do {
      printf("--:> ");
      if ( getline(&userin, &userin_len, stdin) != -1 ) {
        printf("User entered: %s\n", userin);
      }
      else {
        perror("User input too long or error reading from stdin ");
      }
    } while (strcmp(userin, "quit\n")); //this exit condition is temporary
  }

  // batch mode
  else if ( cargs > 1 ) {
    printf("Batch mode.\n");
  }

  free(curr_dir);
  free(pathenv);
  free(userin);
  return 0;
}
