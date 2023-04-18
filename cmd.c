// CSCE 3600 - major 2 - group 6
// cmd.c
// author:  Alex Ho
// date:    04/09/2023
// desc:    Command source file, launches a command in a child process
#include "cmd.h"
#include "handle_exit.h"
#include "alias.h"


int shell_cmd(char **args){
    if(strcmp(args[0], "cd") == 0){
        //INSERT CD FUNCTION CALL HERE
        return 0;
    }
    else if(strcmp(args[0], "path") == 0){
        //INSERT path FUNCTION CALL HERE
        return 0;
    }
    else if(strcmp(args[0], "myhistory") == 0){
        //INSERT myhistory FUNCTION CALL HERE
        return 0;
    }
    else if(strcmp(args[0], "exit") == 0){
        exit(0);
    }
    else if(strcmp(args[0], "alias") == 0){
        //INSERT alias FUNCTION CALL HERE
        if(args[1] == NULL) {
            // No arguments - print existing aliases
            list_aliases();
        } else if(strcmp(args[1], "-r") == 0 && args[2] != NULL) {
            // Remove an alias
            remove_alias(args[2]);
        } else if(strcmp(args[1], "-c") == 0) {
            // Clear all aliases
            clear_aliases();
        } else  {
            // Define a new alias
            add_alias(args);
        // } else {
        //     printf("Invalid alias command.\n");
        }
        return 0;
    } else if(strcmp(args[0], "test") == 0){
        return cmd_fork_template();
    }

    pid_t pid;
    pid = fork();
    if(pid == 0){
        //add current working directory to environment variables
  //      setenv("CWD",getcwd(current_working_directory,1024), 1);
        //executes command and checks for errors
        if(execvp(args[0], args) == -1){
            perror("ERROR: exec failed");
            kill(getpid(), SIGTERM);
            return 1;
        }

    }else if(pid < 0){
        //error forking
        perror("ERROR: fork failed");
        return 1;
    }
    return 0;
}
