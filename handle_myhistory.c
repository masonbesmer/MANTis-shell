/* CSCE 3600 - major 2 - group 6
   handle_cd.c
   author:  Tobi Adeoye
   date:    04/11/2023
   desc:    This code is used implement the myhistory command into the shell.
*/


#include "handle_myhistory.h"



CommandHistory history;// a global variable to store command history


//To store commands to the history.
void add_to_history(char* command){

	//To move commands in history the to make room for  new commands.
	for(int i = MAX_HISTORY - 1; i > 0; i--) {
		strncpy(history.commands[i], history.commands[i-1], MAX_COMMAND_LENGTH);
	}
	strncpy(history.commands[0], command, MAX_COMMAND_LENGTH);
	history.count++;
}

//To clear the history.
void clear_history() {

	history.count = 0;
	for (int i = 0; i < MAX_HISTORY; i++) {
		memset(history.commands[i], 0, MAX_COMMAND_LENGTH);
	}
}

//To execute the history by its number.
char* execute_history(int history_num) {

	char* exe;
  // Added this so that blindly executing the history doesn't execute badly.
  if( strstr(history.commands[0], "myhistory") != NULL )
    history_num++;

  if(history_num > MAX_HISTORY) {
    printf("Invalid history command number\n");
    return NULL;
  }
	if(history_num <= 0 || history_num > history.count) {
		printf("Invalid history command number\n");
		return NULL;
	}
	else {
		exe = history.commands[history_num - 1];
		printf("Executing history command #%d: %s\n", history_num, exe);
		return exe;
	}
}

//To print all command history.
void print_history() {

	printf("History of previous commands:\n");
	for(int i = 0; i < history.count; i++) {
		printf("%d: %s %s \n",
        i+1, strtok(history.commands[i],"\n"), (i==0)?"<-(Latest)":"");
	}
}


