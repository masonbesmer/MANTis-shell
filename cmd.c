// CSCE 3600 - major 2 - group 6
// cmd.c
// author:  Alex Ho
// date:    04/09/2023
// desc:    Command source file, launches a command in a child process
#include "cmd.h"


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
        return 0;
    }

    pid_t pid;
    pid = fork();
    if(pid == 0){
        //add current working directory to environment variables
        setenv("CWD",getcwd(current_working_directory,1024), 1);
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
