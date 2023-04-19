// CSCE 3600 - major 2 - group 6
// cmd.h
// author:  Alex Ho
// date:    09/04/2023
// desc:    Command header
#ifndef CMD_H
#define CMD_H

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"
#include "string.h"
#include "limits.h"
#include "parser.h"

int shell_cmd(char **args, int mode);

#endif
