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
    else if (strcmp(args[0], "test") == 0){
        printf("PARENT: testing. we're about to fork\n");
        pid_t pid;
        pid = fork();
        if (pid == 0){
            // set child process group id to its own pid
            setpgid(0, 0);

            // respect signals in child
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            // temp code to not terminate until i tell it to
            printf("CHILD: just forked\n");
            for(int i = 0; i < 5; i++){
                printf("CHILD: child\n");
                fflush(stdout);
                sleep(1);
            }
            printf("CHILD: child done\n");

            exit(0);
        } else if (pid < 0){
            // fork failed
            perror("ERROR: fork failed");
            return -1;
        } else {
            printf("PARENT: place child in foreground\n");
            tcsetpgrp(STDIN_FILENO, pid);

            //ignore term for writing while in background
            signal(SIGTTOU, SIG_IGN);
            printf("PARENT: place parent in background\n");
            tcsetpgrp(STDIN_FILENO, getpid());

            printf("PARENT: wait for child to finish its stuff\n");
            int status;
            waitpid(pid, &status, WUNTRACED);

            printf("PARENT: place parent back in foreground\n");
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTOU, SIG_DFL);
            printf("PARENT: returning to shell\n");

            // return back to shell loop
            return 0;
        }
    }

    pid_t pid;
    pid = fork();
    if(pid == 0){
        //add current working directory to environment variables
        //setenv("CWD",getcwd(current_working_directory,1024), 1);
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
    else{
        //parent process
        int status;
        waitpid(pid, &status, 0);
        return 0;
    }
    return 0;
}
