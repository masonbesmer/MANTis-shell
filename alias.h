#ifndef ALIAS_H
#define ALIAS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

int add_alias(char **iargs);
int remove_alias(char *name);
int clear_aliases();
int list_aliases();

#endif