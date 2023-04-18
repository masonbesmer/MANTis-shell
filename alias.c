#include "alias.h"

int add_alias(char **iargs) {
    char *alias = strtok(iargs[1], "="); // split the string at "="
    char **command = (char**)malloc(sizeof(char*) * 100);
    int i = 0;
    char *token = strtok(NULL, "="); // get the part after "="
    while (token != NULL) {
        command[i] = token;
        printf("command[%d]: %s\n", i, command[i]);
        i++;
        token = strtok(NULL, "=");
    }
    printf("Alias: %s\n", alias);
    printf("Command: %s\n", command[0]);
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