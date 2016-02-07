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
    int status = 1;
    do {
        
        printf("> ");
        size_t size = 0;
        getline(&input, &size, stdin);
        if (strlen(input) == 1 && input[0] == '\n') {
            continue;
        }
        args = parseInput(input);
        status = executeCommand(args);
    } while (status);
    return 1;
}

void freeArgs () {
   //Maybe free stuff, or maybe just let the OS clean it up 
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
    args[numArgs-1][strlen(args[numArgs-1])] = '\0';
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
        printf("logout\n\n[Process completed]\n");
        exit(0);
    }
    
    int i = 0;
    bool isBG = false;
    while (args[i] != NULL) {
        i++;
    }
    
    if (strcmp("&",args[i-1]) == 0) {
        isBG = true;
    }
    
    pid_t childPID;
    int status;
    bool background;
    
    childPID = fork();
    if (childPID >= 0) {
        //Fork successful
        if(childPID == 0) {
			//this is the childz
            //use sigaction
            if (isBG) {
		      setpgid(0, 0);
            }
			//since we are the child, we want to perform exec and use up this process ID
		    int status = execvp(args[0], args);
            if (status == -1) {
                perror("ATShell");
            }
			exit(0);
		} else {
            if (!isBG) {
              waitpid(childPID, &status, 0);
            } else {
              waitpid(-1, &status,WNOHANG);
            }
		}
    } else {
        puts("Failed to fork");
    }
    return 1;
    
}