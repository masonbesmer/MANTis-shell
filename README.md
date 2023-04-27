
# Major 2 - Basic Shell Interpreter

## Groups Names: Nathan, Alex, Mason, Tobi

# Organization of the Project

## The Interactive Mode (Nathan)

## The Batch Mode (Nathan) 


## Built-In Commands

### The CD command (Tobi)


The CD command function handles changing the current directory in a Unix-like shell using the "cd" command. It takes an array of string arguments as input and uses various conditional statements to determine which action to take. If the second argument is null, the function changes the current directory to the home directory. If the second argument is "..", the function changes the current directory to the parent directory. Finally, if the second argument is any other string, the function attempts to change the current directory to the specified directory.


The important structures in the CD command include the "if" statements that check the second argument to determine the appropriate action, the "chdir" function that changes the current directory, and the "printf" function that prints the current directory. Additionally, the "getcwd" function is used to retrieve the current working directory to verify the success of the change in the current directory. Overall, this command provides a basic implementation of the "cd" command.


### The EXIT command (Nathan)

### The PATH command (Nathan)

### The MYHISTORY command (Tobi)

The MYHISTORY command is related to a history command in a shell. It includes a global variable called ‘CommandHistory’, which is a struct that stores an array of strings representing previous commands entered by the user. The “struct” also includes a counter for the number of commands in the array.


The function ‘add_to_history()’ takes a command string as input and stores it in the command history. It shifts all previous commands by one position in the array to make room for the new command.


The function ‘clear_history()’ sets the command counter to zero and clears all previous commands from the command history array.


The function ‘execute_history()’ takes an integer representing a command history number as input and returns the corresponding command string from the array. It first checks if the first command in the history is "myhistory" and increments the input history number if it is. The function also checks if the history number is valid and returns an error message if it is not. If the history number is valid, the function returns the corresponding command string and prints a message indicating the command number and the command itself.


The function ‘print_history()’ prints all previous commands stored in the command history array, including their position in the array and whether they are the latest command entered by the user. It uses the ‘strtok()’ function to split each command string by newline character and only print the first part of the command.


## Extend Shell Commands

### Redirection (Alex)

### Pipelining (Nathan)

### Support Signal Handling and Terminal Control (Mason)

### The Alias command (Mason)

# Design Overview

# Complete Specification

# Known Bugs or Problems