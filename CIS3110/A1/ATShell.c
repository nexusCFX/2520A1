#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

char **parseInput(char *input);
int executeCommand (char **args);
void freeArgs ();

int main (int argc, char* argv[]) {
    char *input = NULL;
    char curDir[500];
    getcwd(curDir, 500);
    char **args;
    int status;
    pid_t pid;
    while (1) {
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            printf ("Background process[%d] ended.\n",pid);
        }
        
        getcwd(curDir, 500);
        printf("%s>",curDir);
        size_t size = 0;
        getline(&input, &size, stdin);
        if (strlen(input) == 1 && input[0] == '\n') {
            continue;
        }
        
        args = parseInput(input);
        
        if (strcmp(args[0],"cd")==0) {
            chdir(args[1]);
        } else {
            status = executeCommand(args);
        }
    }
    return 1;
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
    int pipeIO[2];
    int pipePos = -1;
    char **args2 = malloc(sizeof(args));
    
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
        if (strcmp(args[i],"|")==0) {
            pipePos = i;
        }
        i++;
    }
    
    
    if ((i > 0) && strcmp("&",args[i-1]) == 0) {
        isBG = true;
        args[i-1] = NULL;
        i--;
    }
    
    pid_t childPID;
    int status = 1;
    
    childPID = fork();
    if (childPID >= 0) {
        //Fork successful
        if(childPID == 0) {
			//since we are the child, we want to perform exec and use up this process ID
            if (isBG) {
                printf ("Child process[%d]\n",childPID);
                setpgid(0,0);
            }
            //If we have >, we are outputting to a file
            if ((i > 1) && strcmp(args[i-2],">")==0) {
                FILE* output = fopen(args[i-1], "w");
                args[i-2] = NULL;
                args[i-1] = NULL;
                //Redirect STDOUT
                dup2(fileno(output), 1);
                fclose(output);
                status = execvp(args[0], args); 
                if (status == -1) {
                    perror("Execvp failure");
                }
            //If we have <, we are getting input from a file
            } else if ((i > 1) && strcmp(args[i-2],"<")==0) {
                FILE* input = fopen(args[i-1], "r");
                args[i-2] = NULL;
                args[i-1] = NULL;
                dup2(fileno(input), STDIN_FILENO);
                fclose(input);
                status = execvp(args[0], args); 
                if (status == -1) {
                    perror("Execvp failure");
                }
            //We have a pipe
            } else if (pipePos != -1) { 
                for (int j = pipePos+1; j < i; j++) {
                    args2[j-(pipePos+1)] = args[j];
                    args[j] = NULL;
                }
                args[pipePos] = NULL;
                   
                //Initialize pipe, fork again
                pipe(pipeIO);
                pid_t tempPid = fork();
                if (tempPid == 0) {
                   //We are child. Close one end of pipe
                   close(pipeIO[0]);
                   
                   //Redirect STDOUT
                   dup2(pipeIO[1], 1); 
                   close(pipeIO[1]);
                  status = execvp(args[0], args);
                  if (status == -1) {
                    perror("Execvp failure");
                }
                } else {
                   //Parent, close other end of pipe
                   close(pipeIO[1]);
                   //Redirect STDIN
                   dup2(pipeIO[0], 0); 
                   close(pipeIO[0]);
                   status = execvp(args2[0], args2);
                   if (status == -1) {
                    perror("Execvp failure");
                }
                }
            } else {
                status = execvp(args[0], args); 
                if (status == -1) {
                    perror("Execvp failure");
                }
            }
			exit(0);
		} else {
            waitpid(childPID, &status, 0);
		}
    } else {
        perror("Failed to fork");
    }
    return 1;   
}
