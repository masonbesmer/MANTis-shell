// CSCE 3600 - Major 2 - Group 6
// handle_exit.h
// author:  Mason Besmer
// date:    28APR23
// desc:    exit header file

#ifndef HANDLE_EXIT_H
#define HANDLE_EXIT_H

// basic includes
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "sys/wait.h"

#define SIGSTP 20 // ctrl z

// function prototypes/declarations
int setup_exit();
void handle_exit(int);
int cmd_fork_template();

#endif
