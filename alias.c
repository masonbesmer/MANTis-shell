#include "alias.h"
#include "parser.h"

struct Alias_List list[MAX_ENTRIES];
int numAliases = 0;
char **alias_arr;

int add_alias(char **iargs) {
    int result = store_alias(parse_alias(iargs));
    if (result != 0) {
        //perror("parse/store_alias() failed");
        return -1;
    }
    //free alias_arr
    // for (int i = 0; i < 100; i++) {
    //     if (alias_arr[i] == NULL) {
    //         break;
    //     }
    //     free(alias_arr[i]);
    // }
    // free(alias_arr);
    return 0;
}

char **parse_alias(char **iargs) {
    alias_arr = (char**)malloc(sizeof(char*) * 512);
    //copy iargs to alias_arr
    for (int i = 0; i < 100; i++) {
        if (iargs[i] == NULL) {
            break;
        }
        alias_arr[i] = iargs[i];
    }


    char *alias_name = strtok(alias_arr[1], "="); // get the part before =
    if (alias_name == NULL) {
        printf("ERROR: invalid syntax\n");
        return NULL;
    }
    char *alias_cmd = strtok(NULL, "="); //get token after =
    if (alias_cmd == NULL) {
        printf("ERROR: invalid syntax\n");
        return NULL;
    }
    alias_arr[0] = alias_name;
    alias_arr[1] = strtok(alias_cmd, " "); //set first arg
    int i=2;
    while ((alias_name = strtok(NULL, " ")) != NULL) { //rest of args
        alias_arr[i] = alias_name;
        i++;
    }
    //free(alias_name);
    //free(alias_cmd);
    return alias_arr;
}

int store_alias(char **alias) {
    if (alias == NULL) {
        return -1;
    }
    for (int i = 0; i < numAliases; i++) {
        if (strcmp(list[i].name, alias[0]) == 0) {
            printf("Alias %s already exists, overwriting...\n", alias[0]);
            memset(list[i].name, '\0', MAX_ALIAS_NAME_LEN - 1);
            for (int j = 0; j < 100; j++) {
                if (list[i].command[j] == NULL) {
                    break;
                }
                memset(list[i].command[j], '\0', MAX_ALIAS_LEN - 1);
            }
            break;
        }
    }
    printf("Storing alias %s\n", alias[0]);
    strncpy(list[numAliases].name, alias[0], MAX_ALIAS_NAME_LEN - 1);
    if (alias[1] == NULL) {
        return -1;
    }
    for (int i = 1; i < 100; i++) {
        if (alias[i] == NULL) {
            break;
        }
        //list[numAliases].command[i-1] = alias[i];
        list[numAliases].command[i-1] = (char*)malloc(sizeof(char) * MAX_ALIAS_LEN);
        strncpy(list[numAliases].command[i-1], alias[i], MAX_ALIAS_LEN - 1);
    }
    for (int i = 0; i < 100; i++) {
        if (list[numAliases].command[i] == NULL) {
            break;
        }
    }
    printf("Alias %s stored.\n", alias[0]);
    numAliases++;
    return 0;
}

int remove_alias(char *name) {
    if (name == NULL) {
        perror("Invalid syntax, no name provided");
        return -1;
    }
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
            printf("Alias %s removed.\n", name);
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
    printf("All aliases cleared.\n");
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
    printf("\nEnd of list\n");
    return 0;
}

char **expand_alias(char *name) {
    printf("Expanding alias %s: ", name);
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return list[i].command;
        }
    }
    perror("Alias not found");
    return NULL;
}