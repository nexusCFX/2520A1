Dine:

This program implements a solution for the dining philosophers problem using semaphores and a mutex lock. It can be run by first compiling it using the included makefile by typing “make dine”. It can be invoked from the command line by typing ./dine [NUM] [NUM] where the first number is the number of philosophers, and the second is the number of times they should eat.

Holes:
This program implements a memory management simulator. In order it simulates managing memory for processes using the first fit, worst fit, best fit, and next fit algorithms. To compile the program, type “make holes”, and invoke it by typing ./holes [FILE], where FILE is an input file of process ID characters and memory sizes separated by spaces, one per line.