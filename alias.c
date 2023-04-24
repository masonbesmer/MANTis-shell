#include "alias.h"

struct Alias_List list[MAX_ENTRIES];
int numAliases = 0;

int add_alias(char **iargs) {
    int result = store_alias(parse_alias(iargs));
    if (result != 0) {
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
    //printf("alias_name: %s\n", alias_name);
    iargs[0] = alias_name;
    iargs[1] = cmd_arg_1;

    for (int i = 0; i < 100; i++) {
        if (iargs[i+1] == NULL) {
            iargs[i] = strtok(iargs[i], "\'"); //remove last ' from last argument
            break;
        }
    }
    // for (int i = 0; i < 100; i++) {
    //     if (iargs[i] == NULL) {
    //         //printf("iargs[%d] is NULL", i);
    //         break;
    //     }
    //     printf("arr to pass to store = iargs[%d]: %s\n", i, iargs[i]);
    // }
    return iargs;
}

int store_alias(char **alias) {
    // for (int i = 0; i < numAliases; i++) {
    //     if (strcmp(*list[i].name, alias[0]) == 0) {
    //         break;
    //     }
    // }
    printf("Storing alias %s\n", alias[0]);
    strncpy(list[numAliases].name, alias[0], MAX_ALIAS_NAME_LEN - 1);
    if (alias[1] == NULL) {
        return -1;
    }
    //if (strcmp(alias[1], ""))
    for (int i = 1; i < 100; i++) {
        if (alias[i] == NULL) {
            break;
        }
        list[numAliases].command[i-1] = alias[i];
        printf("command stored: %s\n", list[numAliases].command[i-1]);
    }
    for (int i = 0; i < 100; i++) {
        if (list[numAliases].command[i] == NULL) {
            break;
        }
        printf("numAliases: %d\n", numAliases);
        printf("command stored: %s\n", list[numAliases].command[i]);
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
        printf("%s: ", list[i].name);
        for (int i = 0; i < 100; i++) {
            // if (list[numAliases].command[i] == NULL) {
            //     break;
            // }
            printf("numAliases: %d\n", numAliases);
            printf("command: %s", list[numAliases].command[i]);
        }
    }
    return 0;
}