#ifndef HANDLE_EXIT_H
#define HANDLE_EXIT_H

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
//include for until loop

#include "sys/wait.h"

#define SIGSTP 20

int setup_exit();
void handle_exit(int);
int cmd_fork_template();

#endif
