CIS3110 Assignment 1

Author: Brandon Chester (0877477)

Mini Shell:

This shell allows for the execution of commands, with and without arguments, along with execution of programs in the background. Typing exit will terminate the shell. This fulfills the requirements for section 1 functions.

The shell allows for the use of output and input redirection from files, along with a single level of piping between two programs. This fulfills the requirements for section 2 functions.

The shell also supports using the cd command to change directories.

Limitations / Issues:

I have encountered situations where piping the output of ls to sort causes a “Bad Address” error. I have been unable to track down the source of this bug, and it may or may not present itself during usage.

References: 

Forking code skeleton (lines 111-170 but not every line) : process1.c code snippet on Courselink. Roughly 5% of code.

Piping concept code (lines 142-149) : shown on whiteboard in Friday lab session. 4% of code.

Line 43 to avoid blocking with background processing : my friend Dylan at UC Berkeley.


