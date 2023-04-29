
# Major 2 - Basic Shell Interpreter

### Names: Nathan, Alex, Mason, Tobi

# Organization of the Project

## General Shell Operation (main() - Nathan)

When our shell starts, is first checks the arguments to determine which mode the
shell needs to start in. Then the shell attempts all necessary memory allocation
and errors if there is a failure.  

If the number of arguments provided is incorrect, the shell program will print a
usage message. If there are no arguments passed, the shell will enter the
"interactive mode". If one argument is passed, the shell will interpret this as a
filename, and attempt to process the contents of that file as a list of batch
arguments, or "batch mode."  

## The Interactive Mode (Nathan)

In interactive mode, the shell first asks the user if they would like to set a
custom prompt. (Extra Credit functionality).  

Then, the program loops infinitely, processing each line of user input until the
user passes the exit command, Ctrl-C (SIGINT), or Ctrl-Z (SIGSTOP).  

All user input is parsed by our shell's robust, per-character input parser. This
parser takes each line of user input and does two layers of parsing. First, the
parser accepts up to the maximum possible line length of 512 characters, it then
processes that text by separating it on every newline terminating character,
including linefeeds such as a carriage return, newline, vertical tab, etc. It
also breaks lines on semicolons and the null character. The parser enters each
command string into a sequential location on the heap in a string array.  

Then, the second half of the parser takes this array of command lines and
processes it down into a series of command arguments in a NULL terminated arrays
needed for exec family syscalls. The parser maintains whitespace and appropriate
nesting of quotes, e.g. single quotes in double quotes and double quotes within
single quotes. The parser also tracks whether quotes are terminated in pairs by
type and will throw an error if a statement is passed with mismatched or missing
quotations.  

This array of args arrays is then iterated over and passed to the shell_cmd()
fuction, with additional flags that help the shell_cmd function detemrine what 
type of operation is being done, e.g. piping, redirection, or both.  

## The Batch Mode (Nathan) 

The batch mode functions similarly to the interactive mode, except that it needs
a different pre-parser setup. The function of the preparser itself is similar to
the first stage parser in interactive. Then all the argument lines are sent to
parse_args() and processed by line of input identically to interactive mode,
until the end of the file or the exit is command is reached.  

## Built-In Commands

### The CD command (Tobi)

The CD command function handles changing the current directory in a Unix-like 
shell using the "cd" command. It takes an array of string arguments as input
and uses various conditional statements to determine which action to take. 
If the second argument is null, the function changes the current directory to
the home directory. If the second argument is "..", the function changes the 
current directory to the parent directory. Finally, if the second argument is 
any other string, the function attempts to change the current directory 
to the specified directory.  

The important structures in the CD command include the "if" statements that 
check the second argument to determine the appropriate action, the "chdir" 
function that changes the current directory, and the "printf" function that 
prints the current directory. Additionally, the "getcwd" function is used to
retrieve the current working directory to verify the success of the change in
the current directory. 
Overall, this command provides a basic implementation of the "cd" command.  

### The EXIT command (Nathan)

The exit command is detected during the second stage parsing. This sets a flag
in the interpreter's main function, and then after all commands on that line 
have been executed, the interactive or batch processing loops are halted and the
program exits gracefully.

### The PATH command (Nathan)

The path command accepts either 0 or 3 arguments. All other uses of the command
illicit a usage message.  

With 0 arguments, the path command prints the current
system path for the shell process.  

If 3 arguments are passed, then the command expects either + or - as the second
argument. If the second argument is +, then the contents of argument 3 are
appended to the path. If the second argument is -, then if the contents of 
argument 3 match the exact contents of one of the paths in the shell's path,
then the argument is removed from the shell path.

These changes are reverted to normal when the shell exits.

### The MYHISTORY command (Tobi)

The MYHISTORY command is related to a history command in a shell. 
It includes a global variable called ‘CommandHistory’, which is a struct that 
stores an array of strings representing previous commands entered by the user. 
The “struct” also includes a counter for the number of commands in the array.  

The function ‘add_to_history()’ takes a command string as input and stores it
in the command history. It shifts all previous commands by one position in
the array to make room for the new command.  

The function ‘clear_history()’ sets the command counter to zero and clears 
all previous commands from the command history array.  

The function ‘execute_history()’ takes an integer representing a command 
history number as input and returns the corresponding command string 
from the array. It first checks if the first command in the history is 
"myhistory" and increments the input history number if it is. The function 
also checks if the history number is valid and returns an error message 
if it is not. If the history number is valid, the function returns the 
corresponding command string and prints a message indicating the command 
number and the command itself.  

The function ‘print_history()’ prints all previous commands stored in the 
command history array, including their position in the array and whether they 
are the latest command entered by the user. It uses the ‘strtok()’ function to 
split each command string by newline character and only print the first 
part of the command.  

## Extend Shell Commands

### Redirection (Alex)

### Pipelining (Nathan)

Pipelining was designed with the idea that it could be done with as many
commands sequentially as the user desires. It was actually much more cumbersome
to design a functin in which only 1 or 2 pipes were supported.  

If the argument parser detects a pipe, it tells shell_cmd to parse the input
with parse_pipe_args() and then execute those args with shell_pipe_cmd().  

The pipe arguments are parsed in such a way where each sequential command is an
array of arguments in an array of arguments arrays. The pipe_exec() function
is a recursive function which takes this array of args arrays and acceps an in
and out file descriptor. The fd's represent the beginning (or read) end of the
pipe chain, and the end (write end) of the pipe chain respectively. This allows
for pipes to be redirected to text output for the redirection command, and
theoretically recieve input redirection at the beggining of a pipe chain.
However file redireciton to the beginning of the pipe chain was taken off the
table as available functionality due to the complicated nature of the problem
and the fact that the guidelines said we don't have to do it.  

Each recursive layer of the pipe_exec() function takes the array of commands and
picks the current index in the array to execute, creates a new file descriptor
to pipe to the next recursive layer, forks, duplicated the write end of the FD 
to stdout and executes the command. Then, in the parent logic branch the 
function calls itself, passing the read end of the file descriptor to the next
function as the input for that function. This results in a pipe chain of
variable length.  

### Support Signal Handling and Terminal Control (Mason)

### The Alias command (Mason)

# Design Overview

The basic deisgn of the shell was to have a functional approach, where each part
of the program was a function which operated on some functionally relevant data.
No shared state exists between any of the functions in our program.  

There are no major data structures in our program. Most of the data that is
stored is stored on the heap and is freed when executed upon, with the exception
of the arguments atored for the alias and myhistory functions, which are simply
arrays of cstrings.  

The argument arrays used to process user input and execute commands are also
just arrays of strings, terminated with a null token. However these are freed
from memory after the command is executed.  

# Complete Specification

The specification requires a few specific clairifcations:

- User input over 512 characters is not accepted.  
- User input which has mismatched quotes is not accepted.  
- User input which has no content is skipped by the parser and not executed.  
- User input lines which have no content between semicolons is skipped and not
executed.  
- Quotes are allowed, as long as they are paired with a matching quote type.
- Quoted arguments are passed without quotes, but with all content between the
quotes preserved including spaces, as a single argument to the exec function.
- Semicolons and quotes of another type nested within a quoted argument are
ignored by the parser and treated as part of the content of the argument.
- There is no distinction between quoted arguments quoted with single quotes
and those quoted with double quotes, as there is no expansion of terms in our
shell.  
- Quotes of the same type cannot be nested within a quoted argument. You must
use single quotes in a double quoted argument and double quotes in a single
quoted argument, for instance.  
- Pipe chain output can be redirected to a file only at the end of the chain.
- Pipe chaining with file input redirection is not supported.  

# Known Bugs or Problems

Main loop: None
Custom Prompt: None
Parser: None
Exit: None
Path: None

