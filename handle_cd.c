/* CSCE 3600 - major 2 - group 6
   handle_cd.c
   author:  Tobi Adeoye
   date:    04/11/2023
   desc:    This code is used implement the cd command into the shell.
*/

#include "handle_cd.h"



int handle_cd(char *args[]) {
	char s[100];

// If we write no path (only 'cd'), then go to the home directory
    if(args[1] == NULL){
        chdir(getenv("HOME"));
	printf("%s\n",getcwd(s,100)); //to check the current directory
        return 1;
    }

// Else we change the directory to the one specified by the argument, if possible
    else{

        if(chdir(args[1]) == -1){
            printf("%s: no such directory\n", args[1]);
            return -1;
        }
    }
    return 0;
}

