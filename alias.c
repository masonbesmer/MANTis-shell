#include "alias.h"

int add_alias(char **args) {
    
    char** args_buff = (char**) calloc(512, sizeof(char*));
    char *alias_name = strtok(args[1], "=");

    int num_args = get_args(args_buff, args);

    for (int i = 0; i < num_args; i++) {
        printf("args_buff[%d]: %s\n", i, args_buff[i]);
    }
    return 0; //return to avoid segfault below for now
    //print text between single quotes in args[]
    char *start = strchr(args[2], '\'');
    char *end = strrchr(args[2], '\'');
    printf("start: %s\n", start);
    printf("end: %s\n", end);
    if (start && end) {
        size_t len = end - start - 1;
        char *alias_value = malloc(len + 1);
        strncpy(alias_value, start + 1, len);
        alias_value[len] = '\0';
        printf("alias_value: %s\n", alias_value);
    }

    printf("Adding alias %s\n", alias_name);
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