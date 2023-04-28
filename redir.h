/* CSCE 3600 - major 2 - group 6
   redir.h
   author:  Alex Ho
   date:    04/11/2023
   desc:    Command header
*/

#ifndef REDIR_H
#define REDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include "signal.h"
#include "pipe.h"

int redirection(char **args);
int pipe_redir(char **args);


#endif