// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include "main.h"
#include "path.h"
#include "parser.h"
#include "handle_exit.h"


void print_help() {
    printf( "\n"
      "MANTis - an interactive shell by Mason, Alex, Nathan, and Tobi\n"
      "Usage: newshell [batchfile]\n"
    );
}

char* set_prompt() {
  char* prompt_in;
  size_t prompt_len;

  printf("Would you like to set a custom shell prompt?\n[Default -->:] Y/n: ");
  if ( getline(&prompt_in, &prompt_len, stdin) == -1 ) {
      perror("User input too long or error reading from stdin ");
      return NULL;
  }
  if ( strcmp(prompt_in, "y\n") == 0 ) {
    printf("Please enter your custom shell prompt\n(<= 10 chars): ");
    if ( getline(&prompt_in, &prompt_len, stdin) == -1 ) {
        perror("User input too long or error reading from stdin ");
        return NULL;
    }

    else if ( strcmp(prompt_in, "\n") == 0 || strlen(prompt_in) > 11) {
      printf("Prompt too long or empty. Setting default prompt.\n");
      free(prompt_in);
      prompt_in = "-->";
    }

  }
  else {
    free(prompt_in);
    prompt_in = "-->";
  }
  prompt_in = strtok(prompt_in, "\n");
  return prompt_in;
}

void prompt(const char* prompt) {
  printf("\n%s: ", prompt);
}

int main( int cargs, char** argv ) {

  if (setup_exit() == -1) {
    perror("ERROR: unable to setup signal handler. ");
    return 1;
  }

  printf(
      // We should see if we can come up with a better name...
      "\nWelcome to MANTis, A Basic Interactive Shell Built for CSCE3600\n\n"
      );

  char* cust_prompt = set_prompt();

  int num_args;
  size_t user_in_len = MAX_ARG_LEN;
  char* shell_dir = (char*) calloc(PATH_MAX, sizeof(char));

  // PATH_MAX is a system defined macro for the max filepath length.
  char* user_in = (char*) calloc(MAX_ARG_LEN, sizeof(char));
  char** args_buff = (char**) calloc(MAX_ARG_LEN, sizeof(char*));

  if ( shell_dir == NULL || args_buff == NULL || user_in == NULL ) {
    errno = ENOMEM;
    perror("ERROR: unable to malloc during shell init.");
    return 1;
  }

  //get current working directory so we always know where the PATH is
  if ( getcwd(shell_dir, PATH_MAX) == NULL ) {
    perror("ERROR: getcwd failed ");
    return 1;
  }

  // usage/help statement
  if ( cargs == 2 && strcmp(argv[1], "-help") == 0 ) {
    print_help();
    return 0;
  }

  // interactive mode
  else if ( cargs == 1 ) {

    // Interactive user input loop:
    printf(
        "\nBegin interactive mode...\n"
        "Enter \"quit\" to exit the shell. \n");
    prompt(cust_prompt);
    if ( getline(&user_in, &user_in_len, stdin) == -1 ) {
      perror("User input too long or error reading from stdin ");
      return 1;
    }

    while ( strcmp(user_in, "quit\n") ) { // TODO this exit cond is temporary

      if (args_buff == NULL ){
        perror("ERROR: Unable to malloc args_buff in get_args ");
        return 1;
      }

      num_args = get_args(args_buff, user_in);
      parse_args(args_buff, num_args);

      for ( int i = 0; i < num_args; i++) {
        free(args_buff[i]);
      }

      prompt(cust_prompt);
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

  free(shell_dir);
  free(args_buff);
  free(user_in);
  return 0;
}
