#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ListInterface.h"

int main(void) {
    List list;
    FILE* fileRead = fopen("test.txt", "r");
    char inputLine[10] = ""
    char command[10] = ""
    char name[80] = "";
    int i;
    int tempPosition;
    
    Initialize(&list);
    printf("List is empty; list is not full; list is of size 0:\n");
    
    while (fgets(inputLine, 100, fileRead) != NULL) {
        strcpy(command, strtok(inputLine, " "));

        tempPosition = atoi(strtok(NULL," "));
        
        if (strcmp(command,"Insert") == 0) {
            Student tempStudent;

            strcpy(name, strtok(NULL, " "));
            
            InitializeStudent(name, atoi(strtok(NULL," ")), &tempStudent);
            Insert(tempStudent, tempPosition, &list);
        }
        else if (strcmp(command,"Remove") == 0) {
            Remove(tempPosition, &list);
        }
        
        (Empty(&list)) ? printf("List is empty; ") : printf("List is not empty; ");
        (Full(&list)) ? printf("List is full; ") : printf("List is not full; ");
        
        printf("list is of size %d:\n", list.size);
        
        for (i = 0; i < list.size; i++) {
            Student peekStudent;
            
            Peek(i, &list, &peekStudent);
            printf("%s %d%c\n",peekStudent.name, peekStudent.grade,'%');
        }
    }
    Destroy(&list);
    
    return EXIT_SUCCESS;
}
