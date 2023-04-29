// CSCE 3600 - Major 2 - Group 6
// alias.h
// author:  Mason Besmer
// date:    28APR23
// desc:    alias header file, handles alias creation, expansion, deletion, printing, and listing

#ifndef ALIAS_H
#define ALIAS_H

// basic includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

// define constants for aliases
#define MAX_ALIAS_LEN 20
#define MAX_ALIAS_NAME_LEN 20
#define MAX_ENTRIES 100

// struct for alias list
struct Alias_List {
    char name[MAX_ALIAS_NAME_LEN];
    char *command[MAX_ALIAS_LEN];
};

// function prototypes/declarations
int add_alias(char **iargs);
int remove_alias(char *name);
char **parse_alias(char **iargs);
int clear_aliases();
int list_aliases();
int store_alias(char **iargs);
char **expand_alias(char *name);
char** check_alias(char *name);
void alias_cleanup();

#endif