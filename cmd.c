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
        printf("testing. we're about to fork\n");
        pid_t pid;
        pid = fork();
        if (pid == 0){
            // set child process group id to its own pid
            setpgid(0, 0);

            // respect signals in child
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            // temp code to not terminate until i tell it to
            printf("just forked, this is the child talking\n");
            for(int i = 0; i < 5; i++){
                printf("child\n");
                fflush(stdout);
                sleep(1);
            }

            // exit child normally (it wont get here) (probably)
            exit(0);
        } else if (pid < 0){
            // fork failed
            perror("ERROR: fork failed");
            return -1;
        } else {
            printf("place child in foreground, move parent to background\n");
            tcsetpgrp(STDIN_FILENO, pid);

            printf("wait for child to finish its stuff, then return to shell\n");
            waitpid(pid, NULL, 0);

            printf("place parent back in foreground after child is done\n");
            tcsetpgrp(STDIN_FILENO, getpgid(0));
            printf("child is done, parent is back in foreground, returning to shell\n");

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
