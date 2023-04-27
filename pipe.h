// CSCE 3600 - major 2 - group 6
// pipe.c
// author:  Nathan Jodoin
// date:    19APR23
// desc:    pipe functional definition

#ifndef _PIPE_H_
#define _PIPE_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "signal.h"

#include "main.h"

int parse_pipe_args ( char** cmd_buff[], char* input_args[]);
int shell_pipe_cmd( char* args[] );

#endif
