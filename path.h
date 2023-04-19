// CSCE 3600 - major 2 - group 6
// path.h
// author:  Nathan Jodoin
// date:    08APR23
// desc:    path built-in header

#ifndef _PATH_H_
#define _PATH_H_

#include <stdlib.h>
#include <stdio.h>

#define MAX_PATH_LENGTH 512
#define SHELL_PATH     "/.shellpath"
#define READ_ONLY       "r"
#define TRUNC           "w"
#define APPEND          "a"

int print_path();
int append_to_path(char* str);
int remove_from_path(char* str);

int shell_path(char* args[]);

#endif
