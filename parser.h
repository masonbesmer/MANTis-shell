// CSCE 3600 - major 2 - group 6
// parser.h
// author:  Nathan Jodoin
// date:    08APR23
// desc:    parser functionality header

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int get_args( char* args_buff[], char* user_in );
int get_args_from_batch ( char* args_buff[], char * user_in );
int parse_args( char* args_buff[], int num_args);

#endif
