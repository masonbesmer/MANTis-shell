#ifndef ALIAS_H
#define ALIAS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

#define MAX_ALIAS_LEN 200
#define MAX_ALIAS_NAME_LEN 50
#define MAX_ENTRIES 100

struct Alias_List {
    char name[MAX_ALIAS_NAME_LEN];
    char *command[MAX_ALIAS_LEN];
};

int add_alias(char **iargs);
int remove_alias(char *name);
char **parse_alias(char **iargs);
int clear_aliases();
int list_aliases();
int store_alias(char **iargs);
char **expand_alias(char *name);

#endif