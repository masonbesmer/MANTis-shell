// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include "main.h"
#include "path.h"
#include "parser.h"
#include "handle_exit.h"
<<<<<<< Updated upstream
=======
#include <ctype.h>
#include "handle_myhistory.h"
>>>>>>> Stashed changes

void print_help() {
    printf(
      "MANTis - an interactive shell by Mason, Alex, Nathan, and Tobi\n"
      "Usage: newshell [batchfile]\n"
    );
}

int main( int cargs, char** argv ) {
  if (setup_exit() == -1) {
    perror("ERROR: unable to setup signal handler. ");
    return 1;
  }

  printf(
      // We should see if we can come up with a better name...
      "Welcome to MANTis, A Basic Interactive Shell Built for CSCE3600\n\n"
      );

  char* pathenv;
  int num_args;
  size_t user_in_len = MAX_ARG_LEN;
  // PATH_MAX is a system defined macro for the max filepath length.
  char* curr_dir = (char*) calloc(PATH_MAX, sizeof(char));
  char* user_in = (char*) calloc(MAX_ARG_LEN, sizeof(char));
  char** args_buff = (char**) calloc(MAX_ARG_LEN, sizeof(char*));

  if ( curr_dir == NULL || args_buff == NULL ) {
    errno = ENOMEM;
    perror("ERROR: unable to malloc during shell init.");
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


    // Interactive user input loop:
    printf( "\nEnter \"quit\" to exit the shell. \n"
            "--:> " );
    if ( getline(&user_in, &user_in_len, stdin) == -1 ) {
      perror("User input too long or error reading from stdin ");
      return 1;
    }

    while ( strcmp(user_in, "quit\n") ) { // TODO this exit cond is temporary

      if (args_buff == NULL ){
        perror("ERROR: Unable to malloc args_buff in get_args ");
        return 1;
      }

      add_to_history(user_in);
      num_args = get_args(args_buff, user_in);
      parse_args(args_buff, num_args);

      for ( int i = 0; i < num_args; i++) {
        free(args_buff[i]);
      }

      printf("--:> ");
      if ( getline(&user_in, &user_in_len, stdin) == -1 ) {
        perror("User input too long or error reading from stdin ");
        return 1;
      }
    }
  }


  // batch mode
  else if ( cargs == 2 ) {
    printf("Batch mode.\n\n");

    if (args_buff == NULL ){
      perror("ERROR: Unable to malloc args_buff in get_args ");
      return 1;
    }
    num_args = get_args_from_batch(args_buff, argv[1]);

    parse_args(args_buff, num_args);
    for ( int i = 0; i < num_args; i++) {
      free(args_buff[i]);
    }
  }
  else if ( cargs > 2 || cargs < 1 ) {
    errno = EINVAL;
    perror("Incorrect number of arguments. ");
    return 1;
  }

  free(curr_dir);
  free(pathenv);
  free(args_buff);
  free(user_in);
  return 0;
}
