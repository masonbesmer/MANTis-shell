// CSCE 3600 - major 2 - group 6
// parser.c
// author:  Nathan Jodoin
// date:    10APR23
// desc:    user command args parsing functionality

#include "parser.h"

#define MAX_CMD_LEN     512

// Populate supplied empty argument buffer with commands and args
// expects: empty argument buffer, userin from stdio or batchfile
// returns integer number of entries in argument buffer, or -1 on error
int get_args( char ** args_buff, char * userin ) {
  // parse line by \n or ;

  // populate argument buffer with commands
  return 0;
}

// read the args from a file into a supplied character buffer
// expects an empty args buffer and a filename
// returns integer number of entries in argument buffer, or -1 on error
int get_args_from_batch( char ** args_buffer, char * filename ) {
  // Open file for reading
  // Read file, delimit by neline or ;
  // Parse line by argument
  return 0;
}

// PARSE arument bufer into an array of buffers of commands and args
// to be processed by the shell.
// Each line must be parsed down to command arg arg arg
// must recognize redirect and pipe situations and call pipe/redir as needed
// returns 0 on success, -1 on failure
int parse_args( char ** args_buffer ) {
  //create a args array buffer args[][]
  //for each item in args_buffer, create an args[] and insert it into the array
  return 0;
}
