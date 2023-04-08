// CSCE 3600 - major 2 - group 6
// main.c
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main function definition and basic shell logic

#include "main.h"

int main( int cargs, char** argv ) {

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
