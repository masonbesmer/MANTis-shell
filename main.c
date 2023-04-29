// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include <ctype.h>

#include "main.h"
#include "path.h"
#include "parser.h"
#include "handle_exit.h"
#include "handle_myhistory.h"

void print_help() {
    printf( "\n"
      "MANTis - an interactive shell by Mason, Alex, Nathan, and Tobi\n"
      "Usage: newshell [batchfile]\n"
    );
}

// mallocs an uninitialized char* and sets it to a custom prompt
char* set_prompt(char * prompt) {
  prompt = (char*)malloc(sizeof(char)*11);
  size_t prompt_len = sizeof(char)*11;

  if ( prompt == NULL ) {
    perror("ERROR: Unable to malloc prompt vars in set_prompt. " );
    return NULL;
  }

  printf("Would you like to set a custom shell prompt?\n[Default -->:] Y/n: ");
  if ( getline(&prompt, &prompt_len, stdin) == -1 ) {
      perror("User input too long or error reading from stdin ");
      return NULL;
  }
  for ( int i=0; i < strlen(prompt); i++ )
    prompt[i] = tolower(prompt[i]);

  if ( strcmp(prompt, "y\n") == 0 || strcmp(prompt, "\n") == 0 ) {
    printf("Please enter your custom shell prompt\n(<= 10 chars): ");
    if ( getline(&prompt, &prompt_len, stdin) == -1 ) {
        perror("User input too long or error reading from stdin ");
        return NULL;
    }
    else if ( strlen(prompt) > 11 || strcmp(prompt, "\n") == 0 ) {
      printf("Prompt too long or empty. Setting default prompt.\n");
      strcpy(prompt, "-->");
    }
  }
  else {
    strcpy(prompt, "-->");
  }
  prompt = strtok(prompt, "\n");
  return prompt;
}

void prompt(const char* prompt) {
  printf("\n%s: ", prompt);
}

int main( int cargs, char** argv ) {

  int num_args;
  char* cust_prompt = "";
  bool exit_flag = false;
  size_t user_in_len = MAX_ARG_LEN;
  char* shell_dir = (char*) calloc(PATH_MAX, sizeof(char));

  // PATH_MAX is a system defined macro for the max filepath length.
  char* user_in = (char*) calloc(MAX_ARG_LEN, sizeof(char));
  char** args_buff = (char**) calloc(MAX_ARG_LEN, sizeof(char*));

  if (setup_exit() == -1) {
    perror("ERROR: unable to setup signal handler. ");
    return 1;
  }

  printf(
      // We should see if we can come up with a better name...
      "\nWelcome to MANTis, A Basic Interactive Shell Built for CSCE3600\n\n"
      );

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

    cust_prompt = set_prompt(cust_prompt);
    // Interactive user input loop:
    printf(
        "\nBegin interactive mode...\n"
        "Enter \"exit\" to exit the shell. \n");

    do {

      prompt(cust_prompt);

      if ( getline(&user_in, &user_in_len, stdin) == -1 ) {
        perror("User input too long or error reading from stdin ");
        continue;
      }

      if ( strlen(user_in)+1 > 512 ) {
        errno = EINVAL;
        perror("ERROR: User entered arguments > max length (512 characters). ");
        continue;
      }

      num_args = get_args(args_buff, user_in);

      if ( num_args > 0 ) {

        // Parse the user input into args[] for execution.
        // Also sets the exit flag if exit was on this line.
        parse_args(args_buff, num_args, &exit_flag);

        // Free the memory after execution.
        for ( int i = 0; i < num_args; i++) {
          free(args_buff[i]);
        }

        if ( exit_flag ) {
          break;
        }

        // Necessary for Tobi's functionality to work.
        add_to_history(user_in);

      }
    } while (true);

    free(cust_prompt);
  }

  // batch mode
  else if ( cargs == 2 ) {

    char* batch_args[1024];
    int num_batch_args = 0;

    printf("Batch mode.\n\n");

    num_batch_args = get_args_from_batch(batch_args, argv[1]);

    if ( num_batch_args < 0 ) {
      perror("Error retrieving args from file.");
      return -1;
    }

    for( int i = 0; i < num_batch_args; i++) {

      if ( exit_flag ) {
        break;
      }

      num_args = get_args(args_buff, batch_args[i]);

      parse_args(args_buff, num_args, &exit_flag);
    }

    for (int i = 0; i < num_args+1; i++) {
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
