#ifndef HANDLE_EXIT_H
#define HANDLE_EXIT_H

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define SIGSTP 20

int handle_exit_signal();

#endif
