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
        free(command);
    }
    return 0;
}
int pipe_redir(char** args)
{
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
        //if the redirection symbol is found, hold the output file name
        char* file = malloc(sizeof(char*) * 100);
        file = strncpy(file, args[i+1], strlen(args[i+1]));
        //split the command by the redirection symbol
        char** command = malloc(sizeof(char*) * 100);
        int j = 0;
        while (j < i){
            command[j] = args[j];
            j++;
        }
        command[j] = NULL;
        //split command by the pipe symbol
        char** command2 = malloc(sizeof(char*) * 100);
        int k = 0;
        while (command[k] != NULL){
            if (strcmp(command[k], "|") == 0){
                command[k] = NULL;
                k++;
                break;
            }
            k++;
        }
        int l = 0;
        while (command[k] != NULL){
            command2[l] = command[k];
            k++;
            l++;
        }
        command2[l] = NULL;
        //print both commands
        printf("command 1: ");
        int m = 0;
        while (command[m] != NULL){
            printf("args[%d]: %s \n", m,command[m]);
            m++;
        }
        printf("\n");
        printf("command 2: ");
        int n = 0;
        while (command2[n] != NULL){
            printf("args[%d]: %s \n", n,command2[n]);
            n++;
        }
        printf("\n");

        //pipe the two commands together then execute then write the output to the file
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(command[0], command);
        }
        else if (pid < 0){
            perror("Error forking for output redirection");
            return -1;
        }
        else {
            int status;
            waitpid(pid, &status, 0);
            close(fd[1]);
            pid_t pid2 = fork();
            if (pid2 == 0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                close(fd[1]);
                //reconstruct the command 
                char** command3 = malloc(sizeof(char*) * 100);
                int o = 0;
                while (command2[o] != NULL){
                    command3[o] = command2[o];
                    o++;
                }
                command3[o] = ">";
                command3[o+1] = file;
                command3[o+2] = NULL;
                redirection(command3);
                free(command3);
            }
            else if (pid2 < 0){
                perror("Error forking for output redirection");
                return -1;
            }
            else {
                int status;
                close(fd[0]);
                close(fd[1]);
                waitpid(pid2, &status, 0);
            }
        }
        close(fd[0]);
        close(fd[1]);
        //free the memory
        free(command);
        free(command2);
        free(file);
        //flush stdin
        fflush(stdin);
        return 0;
    }
    return 0;
}