#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

char **parseInput(char *input);
int executeCommand (char **args);
void freeArgs ();

int main (int argc, char* argv[]) {
    char *input = NULL;
    char **args;
    int error;
    do {
        printf("> ");
        size_t size = 0;
        getline(&input, &size, stdin);
        args = parseInput(input);
        error = executeCommand(args);
    } while (error);
    
    
    return 1;
}

void freeArgs () {
    
}

char **parseInput(char *input) {
    int numArgs = 0;
    
    char **args = malloc(sizeof(char*)*strlen(input));
    assert(args != NULL && "Out of memory");
    
    char* token = strtok(input," \n");
    while (token != NULL) {
        args[numArgs] = token;
        token = strtok(NULL," \n");
        numArgs++;
    }

    args[numArgs] = NULL;
    
    
    args = realloc(args, (numArgs+1)*sizeof(char*));
    assert(args != NULL && "Out of memory");
    
    return args;
}

int executeCommand (char **args) {
    if (args[0] == NULL) {
        return 1;
    }
    
    if (strcmp(args[0],"exit") == 0) {
        puts("JIHAD THE INFIDELS");
        exit(0);
    }
    
    pid_t childPID;
    int status;
    bool background;
    
    childPID = fork();
    if (childPID >= 0) {
        //Fork successful
        if(childPID == 0) {
			//this is the child
		//	printf("This is the child process.  PID is: %d\n", getpid());
            
			//since we are the child, we want to perform exec and use up this process ID
		    int status = execvp(args[0], args);
            if (status == -1) {
                perror("ATShell");
            }
			exit(0);
		} else {
			//this is the parent
		//	printf("Parent process.  PID is: %d\n", getpid());
			waitpid(childPID, &status, 0);
		}
    } else {
        puts("Failed to fork");
    }
    return 1;
    
}