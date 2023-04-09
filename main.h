// CSCE 3600 - major 2 - group 6
// main.h
// author:  Nathan Jodoin
// date:    08APR23
// desc:    main header

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define MAX_ARG_LEN     512

void print_help() {
    printf(
      "MANTis - an interactive shell by Mason, Alex, Nathan, and Tobi\n"
      "Usage: newshell [batchfile]\n"
    );
}

#endif
