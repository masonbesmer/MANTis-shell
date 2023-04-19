/* CSCE 3600 - major 2 - group 6
   redir.c
   author:  Tobi Adeoye
   date:    04/11/2023
   desc:    This code is used implement stdin or stdout redirection into a file on the shell. 
*/


#include "redir.h"

int redirection(char** args){
    //find where the redirection symbol is
    int i = 0;
    while (args[i] != NULL){
        if (strcmp(args[i], ">") == 0){
            break;
        }
        i++;
    }
    //if the redirection symbol is not found, return -1
    if (args[i] == NULL){
        return -1;
    }
    //if the redirection symbol is found, create a new array to hold the command
    char** command = malloc(sizeof(char*) * 100);
    int j = 0;
    while (j < i){
        command[j] = args[j];
        j++;
    }
    command[j] = NULL;
    //create a new array to hold the file name
    char** file = malloc(sizeof(char*) * 100);
    int k = 0;
    while (args[i+1] != NULL){
        file[k] = args[i+1];
        i++;
        k++;
    }
    file[k] = NULL;
    //create a new file descriptor
    int fd = open(file[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //if the file descriptor is not valid, return -1
    if (fd == -1){
        return -1;
    }
    //create a new process
    pid_t pid = fork();
    //if the process is not valid, return -1
    if (pid == -1){
        return -1;
    }
    //if the process is valid, execute the command
    if (pid == 0){
        //redirect the output to the file
        dup2(fd, 1);
        //execute the command
        execvp(command[0], command);
        //close the file descriptor
        close(fd);
        //exit the process
        exit(0);
    }
    else {
        int status;
        //wait for the child process to finish
        waitpid(pid, &status, 0);
    }
    //close the file descriptor
    close(fd);
    //free the memory
    free(command);
    free(file);
    //return 0
    return 0;
}