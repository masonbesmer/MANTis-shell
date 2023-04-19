#include "alias.h"

struct Alias_List list[MAX_ENTRIES];
int numAliases = 0;

int add_alias(char **iargs) {
    if (store_alias(parse_alias(iargs)) != 0) {
        perror("parse/store_alias() failed");
        return -1;
    }
    printf("Adding alias %s\n", iargs[0]);
    return 0;
}

char **parse_alias(char **iargs) {
    //print all of iargs
    for (int i = 0; i < 100; i++) {
        if (iargs[i] == NULL) {
            break;
        }
        printf("iargs[%d]: %s\n", i, iargs[i]);
    }

    char *alias_name = strtok(iargs[1], "="); // get the part before "="
    char *cmd_arg_1 = strtok(NULL, "\'"); //get token after first "'"
    iargs[0] = alias_name;
    iargs[1] = cmd_arg_1;

    for (int i = 0; i < 100; i++) {
        if (iargs[i+1] == NULL) {
            iargs[i] = strtok(iargs[i], "\'"); //remove last ' from last argument
            break;
        }
    }
    for (int i = 0; i < 100; i++) {
        if (iargs[i] == NULL) {
            //printf("iargs[%d] is NULL", i);
            break;
        }
        //printf("arr to pass to store = iargs[%d]: %s\n", i, iargs[i]);
    }
    return iargs;
}

int store_alias(char **alias) {
    printf("Storing alias %s\n", alias[0]);
    strncpy(list[numAliases].name, alias[0], MAX_ALIAS_NAME_LEN - 1);
    printf("name stored");
    if (alias[1] == NULL) {
        return -1;
    }
    for (int i = 1; i < 100; i++) {
        if (alias[i] == NULL) {
            break;
        }
        list[numAliases].command[i] = &alias[i];
    }
    numAliases++;
    return 0;
}

int remove_alias(char *name) {
    printf("Removing alias %s\n", name);
    return 0;
}

int clear_aliases() {
    printf("Clearing all aliases\n");
    return 0;
}

int list_aliases() {
    printf("Listing all aliases\n");
    for (int i = 0; i < numAliases; i++) {
        printf("%s: %s\n", list[i].name, list[i].command);
    }
    return 0;
}