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

enum exec_type{
    EXEC = 0,
    PIPE = 1,
    REDIRECT = 2,
    BOTH = 3
};

int shell_cmd(char **args,enum exec_type mode);

#endif
