/*********************************************************************
 * FILE NAME: myProgram.c
 * PURPOSE: Test program for Student List implementations.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 1)
 * DATE: 21/09/2015
 * NOTES: This program uses test data recorded in a text file
 *        (see test.txt). 
 *********************************************************************/

#include "StackInterface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    Stack s1;
    Stack s2;
    Stack s3;
    Item X;
    Item X2;
    Item sum;
    int carry = 0;
    FILE* fileRead = fopen("test.txt", "r");
    char* inputLine = malloc(sizeof(char)*101);
    int i;
    
    Initialize(&s1);
    Initialize(&s2);
    Initialize(&s3);
    
    fgets(inputLine, 100, fileRead);
    
    i = 0;
    while (inputLine[i] != '\0') {
        if (inputLine[i] == '\n') {
            break;
        }
        Push((inputLine[i] - '0'),&s1);
        i++;
    }
    
    fgets(inputLine, 100, fileRead);
    
    i = 0;
    while (inputLine[i] != '\0') {
        if (inputLine[i] == '\n') {
            break;
        }
        
        Push((inputLine[i] - '0'),&s2);
        i++;
    }
    
    fclose(fileRead);
    free(inputLine);
    
    i = 0;
    while (s1.first->next && s2.first->next) {
        Top(&s1, &X);
        Top(&s2, &X2);
        
        sum = X + X2 + carry;
        if (sum >= 10) {
            carry = sum / 10;
            sum = sum % 10;
        } else {
            carry = 0;
        }
        
        Push(sum,&s3);
        
        Pop(&s1);
        Pop(&s2);
        i++;
    }
    
    if (carry > 0) {
        X = carry;
        Push(X, &s3);
        carry = 0;
    }

    while (s1.first->next) {
        Top(&s1, &X);
        sum = X + carry;
        if (sum >= 10) {
            carry = sum / 10;
            sum = sum % 10;
        } else {
            carry = 0;
        }

        Push(sum, &s3);
        Pop(&s1);
        i++;
    }
    
    if (carry > 0) {
        X = carry;
        Push(X, &s3);
        carry = 0;
    }
    
    while (s2.first->next) {
        Top(&s2, &X);
        sum = X + carry;
        if (sum >= 10) {
            carry = sum / 10;
            sum = sum % 10;
        } else {
            carry = 0;
        }

        Push(sum, &s3);
        Pop(&s2);
        i++;
    }
    
    if (carry > 0) {
        X = carry;
        Push(X, &s3);
        carry = 0;
    }
    
    while (s3.first->next) {
        Top(&s3, &X);
        printf("%d",X);
        Pop(&s3);
    }
    printf("\n");
    
    Destroy(&s1);
    Destroy(&s2);
    Destroy(&s3);
    
    return EXIT_SUCCESS;
    
}

