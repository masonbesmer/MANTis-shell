
# Major 2 - Basic Shell Interpreter

# Groups Names: Nathan, Alex, Mason, Tobi

# Organization of the Project

## The Interactive Mode (Nathan)

## The Batch Mode (Nathan) 


## Built-In Commands

### The CD command (Tobi)


The code is a function that handles changing the current directory in a Unix-like shell using the "cd" command. It takes an array of string arguments as input and uses various conditional statements to determine which action to take. If the second argument is null, the function changes the current directory to the home directory. If the second argument is "..", the function changes the current directory to the parent directory. Finally, if the second argument is any other string, the function attempts to change the current directory to the specified directory.


The important structures in this code include the "if" statements that check the second argument to determine the appropriate action, the "chdir" function that changes the current directory, and the "printf" function that prints the current directory. Additionally, the "getcwd" function is used to retrieve the current working directory to verify the success of the change in the current directory. Overall, this code provides a basic implementation of the "cd" command and demonstrates the use of various functions and conditional statements in C programming.


### The EXIT command (Nathan)

### The PATH command (Nathan)

### The MYHISTORY command (Tobi)


## Extend Shell Commands

### Redirection (Alex)

### Pipelining (Nathan)

### Support Signal Handling and Terminal Control (Mason)

### The Alias command (Mason)

# Design Overview

# Complete Specification

# Known Bugs or Problems