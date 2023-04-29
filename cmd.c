// CSCE 3600 - major 2 - group 6
// cmd.c
// author:  Alex Ho
// date:    04/09/2023
// desc:    Command source file, launches a command in a child process
#include "cmd.h"
#include "handle_exit.h"
#include "handle_myhistory.h"
#include "path.h"
#include "alias.h"
#include "main.h"
#include "pipe.h"

int shell_cmd(char **args, int mode){
    if(mode == REDIR){
        printf("Redirection mode\n");
        return redirection(args);
    }
    if(mode == PIPE){
        return shell_pipe_cmd(args);
    }
    if(mode == BOTH){
        return pipe_redir(args);
    }
    if(strcmp(args[0], "cd") == 0){
        handle_cd(args);
        return 0;
    }
    else if(strcmp(args[0], "path") == 0){
        return shell_path(args);
    }
    else if(strcmp(args[0], "myhistory") == 0){
        if(args[1] == NULL){
            print_history();
        }
        else if(strcmp(args[1], "-c") == 0){
            clear_history();
        }
        else if(strcmp(args[1], "-e") == 0){
            char hist_cmd[MAX_ARG_LEN];
            int num_args;

            strcpy(hist_cmd, execute_history(atoi(args[2])));

            num_args = get_args(args, hist_cmd);

            if( parse_args(args, num_args, false) == -1 ) {
              perror("ERROR: Unable to parse hist arg. ");
              return -1;
            }
        }
        else {
            printf("ERROR: Invalid arguments for myhistory\n");
            return -1;
        }
        return 0;
    }
    else if(strcmp(args[0], "exit") == 0){
        // Eats exit haha
        return 0;
    }
    else if(strcmp(args[0], "alias") == 0 || strcmp(args[0], "uhlias") == 0){
        int i=0;
        while (args[i] != NULL) i++;
        if(i==1){
            list_aliases();
        }
        else if(i==2&&strcmp(args[1], "-c") == 0){
            clear_aliases();
        }
        else if(i == 3&&strcmp(args[1], "-r") == 0){
            if (args[2] == NULL) {
                printf("ERROR: No alias name provided\n");
                return -1;
            }
            remove_alias(args[2]);
        }
        else if(i==2 && strstr(args[1], "=")!=NULL){
            add_alias(args);
        } 
        else if(i==3 && strcmp(args[1], "-e") ==0 ) {
            char **expanded = expand_alias(args[2]);
            if (expanded == NULL) {
                printf("ERROR: Alias not found\n");
                return -1;
            }
            int j = 0;
            while (expanded[j] != NULL) {
                printf("%s ", expanded[j]);
                j++;
            }
            printf("\n");

        }
        else {
            printf("ERROR: Invalid alias command\n");
            return -1;
        }
        return 0;
    }
    //check for alias
    char** temp = check_alias(args[0]);
    if(temp != NULL){
        args = temp;
    }
    pid_t pid;
    pid = fork();
    if(pid == 0){ 
        setpgid(0, 0);
        do {} while(tcgetpgrp(STDIN_FILENO) != getpgrp());
        //executes command and checks for errors
        if(execvp(args[0], args) == -1){
            perror("Unable to execute");
            kill(getpid(), SIGTERM);
            return 1;
        }
    }else if(pid < 0){
        //error forking
        perror("ERROR: fork failed");
        return 1;
    }
    else{
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(STDIN_FILENO, pid);
        int status;
        //waits for child process to finish
        waitpid(pid, &status, WUNTRACED);
        tcsetpgrp(STDIN_FILENO, getpgrp());
        signal(SIGTTOU, SIG_DFL);
        return 0;
    }
    return 0;
}
