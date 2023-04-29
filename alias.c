// CSCE 3600 - Major 2 - Group 6
// alias.c
// author:  Mason Besmer
// date:    28APR23
// desc:    alias source file, handles alias creation, expansion, deletion, printing, and listing

#include "alias.h"
#include "parser.h"

struct Alias_List list[MAX_ENTRIES];
int numAliases = 0; // total ever, not current
char **alias_arr; // global so it can be freed later

/// @brief  Creates an alias from the given args
/// @param char** iargs 
/// @return 0 on success, -1 on failure
int add_alias(char **iargs) {
    if (store_alias(parse_alias(iargs)) != 0) {
        return -1;
    }
    return 0;
}

/// @brief Parse the given args into an alias
/// @param char** iargs
/// @return NULL if invalid syntax, otherwise the alias array to be stored
char **parse_alias(char **iargs) {
    alias_arr = (char**)malloc(sizeof(char*) * 512); // allocate alias_arr
    //copy iargs to alias_arr
    int i = 0;
    while (iargs[i] != NULL && i < 512) {
        alias_arr[i] = (char*) malloc(sizeof(char) * 512);
        strcpy(alias_arr[i], iargs[i]);
        i++;
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
    strcpy(alias_arr[0], alias_name); //copy alias name to alias_arr[0]
    strcpy(alias_arr[1], strtok(alias_cmd, " ")); //copy first arg to alias_arr[1]
    int j=2;
    while ((alias_name = strtok(NULL, " ")) != NULL) { // copy rest of args
        alias_arr[j] = (char*) malloc(sizeof(char) * 512);
        strcpy(alias_arr[j], alias_name); // copy
        j++;
    }
    return alias_arr; // return full alias array for storage
}

/// @brief Store the alias in the dictionary
/// @param char** alias
/// @return 0 on success, -1 on failure
int store_alias(char **alias) {
    if (alias == NULL) {
        return -1;
    }
    // check if alias already exists
    for (int i = 0; i < numAliases; i++) {
        if (strcmp(list[i].name, alias[0]) == 0) {
            printf("Alias %s already exists, overwriting...\n", alias[0]); // overwrite it if it does
            memset(list[i].name, '\0', MAX_ALIAS_NAME_LEN - 1); // clear previous alias
            for (int j = 0; j < 100; j++) {
                if (list[i].command[j] == NULL) {
                    break;
                }
                memset(list[i].command[j], '\0', MAX_ALIAS_LEN - 1);
            }
            break;
        }
    }
    //store it in the list struct
    printf("Storing alias %s\n", alias[0]);
    strncpy(list[numAliases].name, alias[0], MAX_ALIAS_NAME_LEN - 1);
    if (alias[1] == NULL) {
        return -1;
    }
    // copy over the commands
    for (int i = 1; i < 100; i++) {
        if (alias[i] == NULL) {
            break;
        }
        list[numAliases].command[i-1] = (char*)malloc(sizeof(char) * MAX_ALIAS_LEN); // allocate memory
        strncpy(list[numAliases].command[i-1], alias[i], MAX_ALIAS_LEN - 1); // copy over
    }
    printf("Alias %s stored.\n", alias[0]);
    numAliases++;
    return 0;
}

/// @brief Remove the specified alias
/// @param char* name
/// @return 0 on success, -1 on failure
int remove_alias(char *name) {
    if (name == NULL) {
        perror("Invalid syntax, no name provided");
        return -1;
    }
    printf("Removing alias %s\n", name);
    // search for the given alias
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(list[i].name, name) == 0) {
            memset(list[i].name, '\0', MAX_ALIAS_NAME_LEN - 1); // delete it
            for (int j = 0; j < 100; j++) {
                if (list[i].command[j] == NULL) {
                    break;
                }
                memset(list[i].command[j], '\0', MAX_ALIAS_LEN - 1); // delete it
            }
            printf("Alias %s removed.\n", name);
            return 0;
        }
    }
    printf("Alias not found\n");
    return -1;
}

/// @brief Clear all aliases from the list but dont decrement numAliases
/// @return 0 on success, -1 on failure
int clear_aliases() {
    for (int i = 0; i < MAX_ENTRIES; i++) {
        memset(list[i].name, '\0', MAX_ALIAS_NAME_LEN - 1); // annotated previously
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

/// @brief List all aliases
/// @return 0 on success, -1 on failure
int list_aliases() {
    printf("Listing all aliases\n");
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(list[i].name, "") != 0) {
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

/// @brief Expand given alias
/// @param char* name 
/// @return char** of expanded alias, NULL if not alias
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

/// @brief Check if given name is an alias
/// @param char* name
/// @return char** of expanded alias, NULL if not alias
char** check_alias(char *name){
    for(int i = 0; i < MAX_ENTRIES; i++){
        if(strcmp(list[i].name, name) == 0){
            return list[i].command;
        }
    }
    return NULL;
}

/// @brief Free all malloc'd memory
void alias_cleanup() {
    if (numAliases == 0) {
        return;
    }
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (strcmp(list[i].name, "") != 0 && list[i].name != NULL) {
            for (int j = 0; j < 100; j++) {
                if (list[i].command[j] == NULL) {
                    break;
                }
                free(list[i].command[j]); // go through entire alias dictionary and free all memory
            }
        }
    }
    for (int i = 0; i < 100; i++) {
        if (alias_arr[i] == NULL) {
            break;
        }
        free(alias_arr[i]); // free alias array from parser
    }
    free(alias_arr);
}