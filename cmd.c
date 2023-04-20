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
        printf("Pipe mode\n");
        return shell_pipe_cmd(args);
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
        else{
            char hist_cmd[MAX_ARG_LEN];
            strcpy(hist_cmd, execute_history(atoi(args[1])));
            int num_args = get_args(args, hist_cmd);
            parse_args(args, num_args, false);
        }
        return 0;
    }
    else if(strcmp(args[0], "exit") == 0){
        // Eats exit haha
        return 0;
    }
    else if(strcmp(args[0], "alias") == 0){
        if(args[1] == NULL){
            list_aliases();
        }
        else if(strcmp(args[1], "-c") == 0){
            clear_aliases();
        }
        else if(strcmp(args[1], "-r") == 0){
            remove_alias(args[2]);
        }
        else{
            add_alias(args);
        }
        return 0;
    } else if(strcmp(args[0], "test") == 0){
        return cmd_fork_template();
    }

    pid_t pid;
    pid = fork();
    if(pid == 0){
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
        int status;
        //waits for child process to finish
        waitpid(pid, &status, 0);
        return 0;
    }
    return 0;
}
