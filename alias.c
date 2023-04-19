#include "alias.h"

struct Alias_List list[MAX_ENTRIES];
int numAliases = 0;

int add_alias(char **iargs) {
    if (store_alias(parse_alias(iargs)) != 0) {
        perror("parse/store_alias() failed");
        return -1;
    }
    if (numAliases-1 >= MAX_ENTRIES) {
        perror("Too many aliases.");
        return -1;
    }
    printf("Adding alias %s\n", iargs[0]);
    return 0;
}

char **parse_alias(char **iargs) {
    //print all of iargs
    // for (int i = 0; i < 100; i++) {
    //     if (iargs[i] == NULL) {
    //         break;
    //     }
    //     printf("iargs[%d]: %s\n", i, iargs[i]);
    // }

    char *alias_name = strtok(iargs[1], "="); // get the part before "="
    char *cmd_arg_1 = strtok(NULL, "\'"); //get token after first "'"
    //printf("alias_name: %s\n", alias_name);
    iargs[0] = alias_name;
    iargs[1] = cmd_arg_1;
    // strncpy(iargs[0], alias_name, MAX_ALIAS_NAME_LEN - 1);
    //strncpy(iargs[1], cmd_arg_1, MAX_ALIAS_LEN - 1);

    for (int i = 0; i < 100; i++) {
        if (iargs[i] == NULL) {
            //iargs[i] = strtok(iargs[i], "\'"); //remove last ' from last argument
            //strncpy(iargs[i], strtok(NULL, "\'"), MAX_ALIAS_LEN - 1);
            break;
        }
        size_t len = strlen(iargs[i]);
        if (len > 0 && iargs[i][len-1] == '\'') {
            iargs[i][len-1] = '\0';
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
    //printf("Storing alias %s\n", alias[0]);
    strncpy(list[numAliases].name, alias[0], MAX_ALIAS_NAME_LEN - 1);
    //strcpy(list[numAliases].name, alias[0]);
    if (alias[1] == NULL) {
        return -1;
    }
    //if (strcmp(alias[1], ""))
    for (int i = 1; i < 100; i++) {
        if (alias[i] == NULL) {
            break;
        }
        list[numAliases].command[i-1] = malloc(MAX_ALIAS_LEN * sizeof(char));
        strncpy(list[numAliases].command[i-1], alias[i], MAX_ALIAS_LEN - 1);
        //printf("command stored: %s\n", list[numAliases].command[i-1]);
    }
    // for (int i = 0; i < 100; i++) {
    //     if (list[numAliases].command[i] == NULL) {
    //         break;
    //     }
    //     printf("numAliases: %d\n", numAliases);
    //     printf("command stored: %s\n", list[numAliases].command[i]);
    // }
    numAliases++;
    return 0;
}

int remove_alias(char *name) {
    printf("Removing alias %s\n", name);
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(list[i].name, name) == 0) {
            memset(list[i].name, '\0', MAX_ALIAS_NAME_LEN - 1);
            for (int j = 0; j < 100; j++) {
                if (list[i].command[j] == NULL) {
                    break;
                }
                memset(list[i].command[j], '\0', MAX_ALIAS_LEN - 1);
            }
            return 0;
        }
    }
    perror("Alias not found");
    return -1;
}

int clear_aliases() {
    printf("Clearing all aliases\n");
    for (int i = 0; i < MAX_ENTRIES; i++) {
        memset(list[i].name, '\0', MAX_ALIAS_NAME_LEN - 1);
        for (int j = 0; j < 100; j++) {
            if (list[i].command[j] == NULL) {
                break;
            }
            memset(list[i].command[j], '\0', MAX_ALIAS_LEN - 1);
        }
    }
    return 0;
}

int list_aliases() {
    printf("Listing all aliases\n");
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(list[i].name, "") != 0 && list[i].name != NULL) {
            printf("Name: %s, expands to: \"", list[i].name);
            for (int j = 0; j < 100; j++) {
                if (list[i].command[j] == NULL) {
                    break;
                }
                printf("%s ", list[i].command[j]);
            }
            printf("\"\n");
        }
    }
    return 0;
}