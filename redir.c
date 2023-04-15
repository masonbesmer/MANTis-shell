/* CSCE 3600 - major 2 - group 6
   redir.c
   author:  Tobi Adeoye
   date:    04/11/2023
   desc:    This code is used implement stdin or stdout redirection into a file on the shell. 
*/


#include "redir.h"


void redir_stdout(char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(fd < 0){
        perror("Error could not open file for stdout redirection.");
        exit(1);
    }
    if(dup2(fd, STDOUT_FILENO) < 0){
        perror("Error in redirecting to the stdout.");
        exit(1);
    }
    close(fd);
}

void redir_stdin(char *filename){
    int fd = open(filename, O_RDONLY);

    if(fd < 0){
        perror("Error in opening the file to stdin redirection.");
        exit(1);
    }
    if(dup2(fd, STDIN_FILENO) < 0){
        perror("Error in redirecting to the stdin");
        exit(1);
    }
    close(fd);

}