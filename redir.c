/* CSCE 3600 - major 2 - group 6
   redir.c
   author:  Alex Ho
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
        free(command);
        free(file);
        close(fd);
    }
    if(input)
    {
        //open input redirection file
        int fd = open(args[i+1],O_RDONLY, 0600);
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
        //create a new process
        pid_t pid = fork();
        //if the process is valid, execute the command
        if (pid == 0){
            //redirect the input to the file
            dup2(fd, STDIN_FILENO);
            //execute the command
            execvp(command[0], command);
            //close the file descriptor
            close(fd);
        }
        else if (pid < 0){
            perror("Error forking for input redirection");
            return -1;
        }
        else {
            int status;
            //wait for the child process to finish
            waitpid(pid, &status, 0);
            return 0;
        }
        close(fd);
        //free the memory
        for(i = 0; i < j; i++){
            free(command[i]);
        }
        free(command);
    }
    return 0;
}
int pipe_redir(char** args)
{
    //count the number of pipes
    int count = 0;
    int i = 0;
    while (args[i] != NULL){
        if (strcmp(args[i], "|") == 0){
            count++;
        }
        i++;
    }
    if (count == 0 || count > 1){
        perror("Error: invalid number of pipes");
        return -1;
    }
    bool output = false;
    bool input = false;
    i = 0;
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
    if(output){
        int num_commands;
        char** commandbuff[MAX_ARG_LEN];
        //split the command by the redirection symbol
        char** command = malloc(sizeof(char*) * 100);
        int j = 0;
        while (j < i){
            command[j] = args[j];
            j++;
        }
        command[j] = NULL;
        //grab the output file name
        char* file = malloc(sizeof(char*) * 100);
        file = strncpy(file, args[i+1], strlen(args[i+1]));
        num_commands = parse_pipe_args(commandbuff,command);
        //open the output file
        int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if(num_commands != -1){
            //execute the commands and write the output to the file
            pid_t pid = fork();
            if(pid == 0){
                dup2(fd,STDOUT_FILENO);
                pipe_exec(commandbuff,0,STDIN_FILENO,STDOUT_FILENO);
                //close the file descriptor
                close(fd);
                exit(0); //NEEDS TO BE HERE DO NOT DELETE
            }
            else if(pid < 0){
                perror("Error forking for piping into output redirection");
                return -1;
            }
            else{
                int status;
                waitpid(pid, &status, 0);
                close(fd);
            }
        }
        free(command);
        free(file);
        for ( int i=0; i < num_commands; i++ ){
            int j=0;
            while(commandbuff[i][j] != NULL){
            free(commandbuff[i][j]);
            j++;
            }
            free(commandbuff[i]);
        }
        //free the memory
        return 0;
    }
    if(input){
        perror("Error: input redirection with pipes not supported");
        return -1;
    }
    return 0;
}