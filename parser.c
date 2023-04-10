// CSCE 3600 - major 2 - group 6
// parser.c
// author:  Nathan Jodoin
// date:    10APR23
// desc:    user command args parsing functionality

#include "parser.h"
#include "path.h"
#include "main.h"

// Populate supplied unalloced argument buffer with commands and args
// expects: empty argument buffer, userin from stdio or batchfile
// returns integer number of entries in argument buffer, or -1 on error
int get_args( char ** args_buff, char * userin ) {

  args_buff = malloc(MAX_ARG_LEN * sizeof(char*));
  if (args_buff == NULL ){
    perror("EROR: Unable to malloc args_buff in get_args ");
    return -1;
  }

  char* token;
  int index = 0;
  token = strtok(userin, ";");

  if ( token == NULL ) {
    printf("NOTE: User input has no tokenizable content.\n");
  }

  while ( token != NULL ) {
    args_buff[index] = malloc((strlen(token) + 1));
    if( args_buff == NULL ) {
      perror("ERROR: malloc failed in args_buffer in get_args ");
      return -1;
    }
    args_buff[index] = token;
    token = strtok(userin, ";");
    index ++;
  }
  // This will return 0 if the user_args buffer is empty
  // and an index > 0 if it is not empty.
  return index;
}

// read the args from a file into a supplied character buffer
// expects an empty args buffer and a filename
// returns integer number of entries in argument buffer, or -1 on error
int get_args_from_batch( char ** args_buff, char * filename ) {

  // Open file for reading
  FILE* batchfile;
  char batch_buff[512];
  char* token;
  int index = 0;

  if ( (batchfile = fopen(filename, READ_ONLY)) == NULL ) {
    perror("ERROR: Unable to open batch file for reading, file DNE ");
    return -1;
  }

  args_buff = malloc(MAX_ARG_LEN * sizeof(char*));
  if (args_buff == NULL ){
    perror("ERROR: Unable to malloc args_buff in get_args ");
    return -1;
  }

  while ( (fgets(batch_buff, MAX_ARG_LEN, batchfile)) != NULL) {

    token = strtok(batch_buff, ";");
    if ( token == NULL ) {
      printf("NOTE: User input has no tokenizable content.\n");
    }

   while ( token != NULL ) {
      args_buff[index] = malloc((strlen(token) + 1));
      if( args_buff == NULL ) {
        perror("ERROR: malloc failed in args_buffer in get_args ");
        return -1;
      }
      args_buff[index] = token;
      token = strtok(batch_buff, ";");
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

// PARSE arument bufer into an array of buffers of commands and args
// to be processed by the shell.
// Each line must be parsed down to command arg arg arg
// must recognize redirect and pipe situations and call pipe/redir as needed
// returns 0 on success, -1 on failure
int parse_args( char ** args_buff, int num_args) {
  //create a args array buffer (args[][])
  //for each item in args_buffer, create an args[] and insert it into the array
  return 0;
}
