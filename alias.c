#include "alias.h"

int add_alias(char **iargs) {
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
    //copy iargs to alias_def
    char **alias_def = (char**)malloc(sizeof(char*) * 100);
    for (int i = 0; i < 100; i++) {
        if (iargs[i] == NULL) {
            break;
        }
        alias_def[i] = iargs[i];
    }
    //shift alias_def to the left by 1 and remove the first element and store in iargs
    // for (int i = 0; i < 100; i++) {
    //     if (alias_def[i+1] == NULL) {
    //         break;
    //     }
    //     iargs[i] = alias_def[i+1];
    // }
    for (int i = 0; i < 100; i++) {
        if (alias_def[i] == NULL) {
            //printf("alias_def[%d] is NULL\n", i);
            break;
        }
        printf("arr to store for alias def = alias_def[%d]: %s\n", i, alias_def[i]);
    }
    printf("gets here?\n");
    for (int i = 0; i < 100; i++) {
        if (iargs[i] == NULL) {
            //printf("iargs[%d] is NULL", i);
            break;
        }
        printf("arr to pass to exec = iargs[%d]: %s\n", i, iargs[i]);
    }
    printf("return");
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
    return 0;
}