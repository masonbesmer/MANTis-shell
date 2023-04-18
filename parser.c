// CSCE 3600 - major 2 - group 6
// parser.c
// author:  Nathan Jodoin
// date:    10APR23
// desc:    user command args parsing functionality

#include "parser.h"
#include "path.h"
#include "main.h"
#include "cmd.h"

// Populate supplied allocated argument buffer with commands and args.
// Expects: empty argument buffer, userin from stdio or batchfile.
// Returns integer number of entries in argument buffer, or -1 on error.
int get_args( char* args_buff[], char* userin ) {

  char* token, *input;
  input = (char*) malloc((strlen(userin) + 1) * sizeof(char));
  int index = 0;
  strcpy(input, userin);
  token = strtok(input, ";");

  if ( token == NULL ) {
    printf("NOTE: User input has no tokenizable content.\n");
  }

  while ( token != NULL ) {
    args_buff[index] = (char*)malloc((strlen(token) + 1) * sizeof(char));
    if( args_buff[index] == NULL ) {
      perror("ERROR: malloc failed in args_buffer in get_args ");
      return -1;
    }
    strcpy(args_buff[index], token);
    token = strtok(NULL, ";");
    index ++;
  }
  // This will return 0 if the user_args buffer is empty
  // and an index > 0 if it is not empty.
  free(input);
  return index;
}

// Read args from a batchfile into a supplied allocated character buffer.
// Expects an empty args buffer reference and a filename.
// Returns integer number of entries in argument buffer, or -1 on error.
int get_args_from_batch( char * args_buff[], char * filename ) {

  // Open file for reading
  FILE* batchfile;
  char batch_buff[512];
  char* token;
  int index = 0;

  if ( (batchfile = fopen(filename, READ_ONLY)) == NULL ) {
    perror("ERROR: Unable to open batch file for reading, file DNE ");
    return -1;
  }

  while ( (fgets(batch_buff, MAX_ARG_LEN, batchfile)) != NULL) {

    token = strtok(batch_buff, "\n;");
    if ( token == NULL ) {
      printf("NOTE: User input has no tokenizable content.\n");
    }

   while ( token != NULL ) {
      args_buff[index] = (char*) malloc((strlen(token) + 1));
      if( args_buff[index] == NULL ) {
        perror("ERROR: malloc failed in args_buffer in get_args ");
        return -1;
      }
      strcpy(args_buff[index], token);
      token = strtok(NULL, ";");
      index ++;
    }
  }
  if ( index == 0 ) {
    // Have not read anything at all from file.
    perror("WARNING: Batch file empty or is improperly formatted ");
  }
  // This will return 0 if the user_args buffer is empty
  // and an index > 0 if it is not empty.
  fclose(batchfile);
  return index;
}

// Parse populated arument buffer into mult. arrays of exec args.
// Then exec on each array sequentially. TODO Must handle exit() calls here.
// Returns 0 on success, -1 on failure.
int parse_args( char* args_buff[], int num_args) {
  // create a args array buffer (args[][])
  char* token;
  int j_args;
  char* args[512];
  char delims[] = " \t\v\r\n";
  enum exec_type mode = EXEC;
  // in a loop for each command to be processed
  // for each item in args_buffer, create an args[] and insert it into the array
  for ( int i = 0; i < num_args; i++ ) {

    token = strtok(args_buff[i], delims);
    // strtok returns NULL token if nothing is read before EOF
    if ( token == NULL ) {
      // this eats empty args
      continue;
    }

    j_args = 0;

    while ( token != NULL ) {
      // catch pipe and redirection in the args
      switch ( mode ) {
        case REDIR: {
          if ( strstr(token, "|") != NULL )
            mode = BOTH;

          break;
        }

        case PIPE: {
          if ( strstr(token, "<") != NULL || strstr(token, ">") != NULL)
            mode = REDIR;

          break;
        }

        case EXEC: {
          // Set exec type for redirection
          if ( strstr(token, "<") != NULL || strstr(token, ">") != NULL)
            mode = REDIR;
          // Set exec mode for piping
          if ( strstr(token, "|") != NULL )
            mode = PIPE;

          break;
        }
        default: {}
      }

      // parse <, >, and | into a separate arg token
      if ( mode != EXEC ) {
        // if current argument contains a pipe, <, or > parge argument again
        // into subtokens and add to args array
        char* copy = NULL;
        char subtoken[strlen(copy)+1];
        int sublen = 0;
        strcpy(copy, token);

        for ( int i = 0; i < strlen(copy) - 1; i++) {
          // terminate subtoken with null char and push to args arr
          if ( sublen > 0 && copy[i] == '\0' ) {
            subtoken[sublen] = '\0';
            args[j_args] = subtoken;
            j_args ++;
            sublen = 0;
          }

          switch(copy[i]) {

            case '<': case '>': case '|':
            {
              // if sublen > 0, terminate subtoken and add to arr
              if ( sublen > 0 ) {
                subtoken[sublen] = '\0';
                args[j_args] = subtoken;
                j_args ++;
                sublen = 0;
              }
              subtoken[sublen] = copy[i];
              subtoken[sublen+1] = '\0';
              args[j_args] = subtoken;
              j_args ++;

              break;
            }

            default:
            {
              sublen ++;
            }

          }

        }
      }
      else{
        args[j_args] = token;
        j_args++;
      }

      token = strtok(NULL, delims);
    }

    args[j_args] = (char*) NULL;

    if ( mode == BOTH )
      printf("Exec mode: BOTH");
    else if ( mode != EXEC )
      printf("Exec mode: %s\n",  (mode == PIPE)?"PIPE":"REDIR");
    else
     printf("Exec mode : EXEC");

    if ( shell_cmd(args, mode) == 1 ) {
      perror("shell_cmd() failed to exec ");
      for(int i = 0; i < j_args; i++) {
        free(args[i]);
      }
      return -1;
    }

  }
  return 0;
}
