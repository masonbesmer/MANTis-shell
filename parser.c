// CSCE 3600 - major 2 - group 6
// parser.c
// author:  Nathan Jodoin
// date:    10APR23
// desc:    user command args parsing functionality, also handles exit builtin

#include "parser.h"
#include "path.h"
#include "main.h"
#include "cmd.h"

// Populate supplied allocated argument buffer with commands and args.
// Expects: empty argument buffer, userin from stdio or batchfile.
// Returns integer number of entries in argument buffer, or -1 on error.
int get_args( char* args_buff[], char* userin ) {

  char buff[512] = "";
  int buff_ind = 0, num_args = 0;
  char curr_quote = '\0';
  bool inquote = false;

  if ( strlen(userin)+1 > 512 ) {
    errno = EINVAL;
    perror("Error: User entered string longer than 512 characters. ");
    return -1;
  }

  for ( int i = 0; i < strlen(userin) + 1; i++ ) {

    switch (userin[i]) {
      case SQ: case DQ:
        {
          if ( !inquote ) {
            curr_quote = userin[i];
            inquote = true;
          }
          else {
            if ( userin[i] == curr_quote )
              inquote = false;
          }

          buff[buff_ind] = userin[i];
          buff_ind++;

          break;
        }

      case ';': case '\0': case '\n': case '\r': case '\v':
        {
          if ( !inquote ) {
            buff[buff_ind] = '\0';
            args_buff[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));
            if ( args_buff[num_args] == NULL ) {
              perror("ERROR: Bad malloc on parser.");
              return -1;
            }

            strcpy(args_buff[num_args], buff);
            num_args++;
            buff_ind = 0;
          }
          else {
            buff[buff_ind] = userin[i];
            buff_ind++;
          }
          break;
        }

      default:
        {
          buff[buff_ind] = userin[i];
          buff_ind++;

        }
    }
  }

  // catch mismatched quotes in user input
  if (inquote == true ) {
    printf("Invalid syntax, mismatched quotes.\n" );
    return -1;
  }

  // This will return 0 if the user_args buffer is empty
  // and an index > 0 if it is not empty.
  return num_args;
}

// Read args from a batchfile into a supplied allocated character buffer.
// Expects an empty args buffer reference and a filename.
// Returns integer number of entries in argument buffer, or -1 on error.
int get_args_from_batch( char* args_buff[], char * filename ) {

  // Open file for reading
  FILE* batchfile;
  char batch_buff[512];
  int num_args = 0;

  if ( (batchfile = fopen(filename, READ_ONLY)) == NULL ) {
    perror("ERROR: Unable to open batch file for reading, file DNE ");
    return -1;
  }

  while ( (fgets(batch_buff, MAX_ARG_LEN, batchfile)) != NULL) {

    args_buff[num_args] = (char*)malloc((strlen(batch_buff)+1) * sizeof(char));

    if ( args_buff[num_args] == NULL ) {
      perror("ERROR: Bad malloc in batch_args processing.");
      return -1;
    }

    strcpy(args_buff[num_args], batch_buff);
    num_args++;
  }

  if ( num_args == 0 ) {
    // Have not read anything at all from file.
    perror("WARNING: Batch file empty or is improperly formatted ");
  }
  // This will return 0 if the user_args buffer is empty
  // and an index > 0 if it is not empty.
  fclose(batchfile);
  return num_args;
}

// Parse populated arument buffer into mult. arrays of exec args.
// Then exec on each array sequentially.
// Returns 0 on success, -1 on failure.
int parse_args( char* args_buff[], int num_args_in, bool* exit_flag) {
  // create a args array buffer (args[][])
  char* token;
  char* args[512];
  char  curr_quote  = '\0';
  bool  inquote     = false;
  int   mode        = EXEC;
  int  num_args;

  for ( int i = 0; i < num_args_in; i++ ) {

    char buff[512] = "";
    int buff_ind   = 0;
    num_args = 0;

    token = (char*) malloc( (strlen(args_buff[i])+1)*sizeof(char) );
    if ( token == NULL ) {
      perror("ERROR: Bad malloc in parse_args()");
      return -1;
    }

    strcpy(token, args_buff[i]);

    // eat empty args
    if ( token[0] == '\0' ) {
      continue;
    }

    // This loop processes a single entered argument line.
    for ( int j = 0; j < strlen(token)+1; j++ ) {

      switch (token[j]) {
        // single or double quotes
        case SQ: case DQ:
          {
            if ( !inquote ) {
              inquote = true;
              curr_quote = token[j];
            }
            else {
              if ( token[j] == curr_quote ) {
                curr_quote = '\0';
                inquote = false;
              }
              else {
                buff[buff_ind] = token[j];
                buff_ind++;
              }
            }
            break;
          }
        // pipes and delimeters, term buff & add arg, if pipe/redir add as arg
        // piping and redirection
        case '>': case '<':
          {
            if ( inquote ) {
              buff[buff_ind] = token[j];
              buff_ind++;
              break;
            }
            // set the mode
            if ( mode == EXEC )
              mode = REDIR;
            else if ( mode == PIPE )
              mode = BOTH;
            // copy the buffer up to this char exclusive as an arg
            buff[buff_ind] = '\0';

            if ( buff[0] != '\0' ) {
              args[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));
              if ( args[num_args] == NULL ) {
                perror("ERROR: Bad malloc on parser.");
                return -1;
              }

              strcpy(args[num_args], buff);
              buff_ind = 0;
              num_args++;
            }

            // add the redirection character as an arg
            buff[buff_ind] = token[j];
            buff[buff_ind+1] = '\0';
            args[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));
            if ( args[num_args] == NULL ) {
              perror("ERROR: Bad malloc on parser.");
              return -1;
            }
            strcpy(args[num_args], buff);
            num_args++;
            break;
          }
        case '|':
          {
            if ( inquote ) {
              buff[buff_ind] = token[j];
              buff_ind++;
              break;
            }
            if ( mode == EXEC )
              mode = PIPE;
            else if ( mode == REDIR )
              mode = BOTH;
            // copy the buffer up to this char exclusive as an arg
            buff[buff_ind] = '\0';

            if ( buff[0] != '\0' ) {
              args[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));

              if ( args[num_args] == NULL ) {
                perror("ERROR: Bad malloc on parser.");
                return -1;
              }

              strcpy(args[num_args], buff);
              buff_ind = 0;
              num_args++;
            }

            // add the pipe character as an arg
            buff[buff_ind] = token[j];
            buff[buff_ind+1] = '\0';
            args[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));

            if ( args[num_args] == NULL ) {
              perror("ERROR: Bad malloc on parser.");
              return -1;
            }
            strcpy(args[num_args], buff);
            num_args++;
            break;
          }

        // delimeters
        case ' ': case '\n': case '\r': case '\v': case '\t':
          {
            if ( inquote ) {
              buff[buff_ind] = token[j];
              buff_ind++;
              break;
            }

            // copy the buffer up to this char exclusive as an arg
            buff[buff_ind] = '\0';

            if ( buff[0] != '\0' ) {
              args[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));

              if ( args[num_args] == NULL ) {
                perror("ERROR: Bad malloc on parser.");
                return -1;
              }

              strcpy(args[num_args], buff);
              buff_ind = 0;
              num_args++;
            }

            break;
          }

        case '\0':
          {
            buff[buff_ind] = token[j];

            if ( buff[0] != '\0' ) {
              args[num_args] = (char*)malloc((strlen(buff)+1)*sizeof(char));
              if ( args[num_args] == NULL ) {
                perror("ERROR: Bad malloc on parser.");
                return -1;
              }

              strcpy(args[num_args], buff);
              buff_ind = 0;
              num_args++;
            }
            break;
          }
        // anything that doesn't require special parsing gets added to the buff
        default:
          {
            buff[buff_ind] = token[j];
            buff_ind++;
          }
      }
    }

    if ( token != NULL )
      free(token);

    // NULL terminate the args array
    args[num_args] = (char*) NULL;

    if ( strcmp(args[0], "exit") == 0 ) {
      printf("Exit flag thrown, line completed, exiting.\n");
      *exit_flag = true;
    }

    // Exec commands
    if ( shell_cmd(args, mode) == 1 ) {
      perror("shell_cmd() failed to exec ");
      return -1;
    }
    for(int i = 0; i < num_args; i++)
      free(args[i]);
  }
  // memory cleanup
  return 0;
}
