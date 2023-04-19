/* CSCE 3600 - major 2 - group 6
   handle_myhistory.h
   author:  Tobi Adeoye
   date:    04/11/2023
   desc:    Command header
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HANDLE_MYHISTORY_H
#define HANDLE_MYHISTORY_H
#define MAX_HISTORY 20
#define MAX_COMMAND_LENGTH 256


// Data structure to store command history

typedef struct {
	char commands[MAX_HISTORY][MAX_COMMAND_LENGTH];
	int count;
} CommandHistory;


void add_to_history(char* command);
void clear_history();
void print_history();
char* execute_history(int history_num);

#endif
