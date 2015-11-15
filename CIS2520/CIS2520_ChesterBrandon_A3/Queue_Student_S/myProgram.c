#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "QueueInterface.h"

int main(void) {
    Queue queue;
    FILE* fileRead = fopen("test.txt", "r");
    char inputLine[100] = "";
    char command[10] = "";
    char name[80] = "";
    int i;
    
    Initialize(&queue);
    printf("Queue is empty; queue is not full; queue is of size 0:\n");
    
    while (fgets(inputLine, 100, fileRead) != NULL) {
        strcpy(command, strtok(inputLine, " "));
        
        if (strcmp(command,"Enqueue") == 0) {
            Student tempStudent;
            
            strcpy(name, strtok(NULL, " "));
            
            InitializeStudent(name, atoi(strtok(NULL," ")), &tempStudent);
            Enqueue(tempStudent, &queue);
        } else {
            Dequeue(&queue);
        }
        
        (Empty(&queue)) ? printf("Queue is empty; ") : printf("Queue is not empty; ");
        (Full(&queue)) ? printf("queue is full; ") : printf("queue is not full; ");
        
        printf("queue is of size %d:\n", queue.size);
        
        for (i = queue.head; i < (queue.head+queue.size); i++) {
            if (i >= MAXQUEUESIZE) {
                printf("%s %d%c\n",queue.items[i-MAXQUEUESIZE].name, queue.items[i-MAXQUEUESIZE].grade,'%');
            } else {
                printf("%s %d%c\n",queue.items[i].name, queue.items[i].grade,'%');
            }
        }
    }
    fclose(fileRead);
    Destroy(&queue);
    
    return EXIT_SUCCESS;
}
