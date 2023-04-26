/* CSCE 3600 - major 2 - group 6
   redir.c
   author:  Tobi Adeoye
   date:    04/11/2023
   desc:    This code is used implement stdin or stdout redirection into a file on the shell. 
*/


#include "redir.h"

int redirection(char** args){
    //find where the redirection symbol is
    bool output = false;
    bool input = false;
    int i = 0;
    while (args[i] != NULL){
        if (strcmp(args[i], ">") == 0){
            output = true;
            break;
        }
        if(strcmp(args[i], "<") == 0){
            input = true;
            break;
        }
        i++;
    }
    if (output && input){
        perror("Error: cannot have both input and output redirection");
        return -1;
    }
    printf("output boolean: %s\n", output ? "true" : "false");
    if(output){
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
            perror("Error opening file for output redirection");
            return -1;
        }
        //create a new process
        pid_t pid = fork();
        //if the process is not valid, return -1
        if (pid == -1){
            perror("Error forking for output redirection");
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
    }
    if(input)
    {
        //open input redirection file
        int fd = open(args[i+1],O_RDONLY, 0600);
        printf("input file: %s\n", args[i+1]);
        //if the file descriptor is not valid, return -1
        if (fd == -1){
            perror("Error opening file for input redirection");
            return -1;
        }
        //create a new array to hold the command
        char** command = malloc(sizeof(char*) * 100);
        int j = 0;
        while (j < i){
            command[j] = args[j];
            j++;
        }
        command[j] = NULL;
        //print command 
        printf("command: %s\n", command[0]);
        //create a new process
        pid_t pid = fork();
        //if the process is not valid, return -1
        if (pid == -1){
            perror("Error forking for input redirection");
            return -1;
        }
        //if the process is valid, execute the command
        if (pid == 0){
            //redirect the input to the file
            dup2(fd, STDIN_FILENO);
            //execute the command
            execvp(command[0], command);
            //close the file descriptor
            close(fd);
        }
        else {
            int status;
            //wait for the child process to finish
            waitpid(pid, &status, 0);
            return 0;
        }
        close(fd);
        //free the memory
        free(command);
    }
    return 0;
}