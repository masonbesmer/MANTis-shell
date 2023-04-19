/* CSCE 3600 - major 2 - group 6
   redir.h
   author:  Tobi Adeoye
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

int redirection(char **args);


#endif