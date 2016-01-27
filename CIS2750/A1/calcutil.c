#include <stdlib.h>
//Remove stdio if not needed once finished
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "calcutil.h"

/*
File: calcutil.c \\put description too
Author: Brandon Chester : 0877477
Contact: bchester@mail.uoguelph.ca
Created: 
Last modified:
*/

void freeCalParam (CalParam *const param);
void freeCalProp (CalProp *const prop);
bool checkEmptyString(const char *line);
CalStatus makeCalStatus (CalError code, int linefrom, int lineto);
bool hasCRLF (FILE *const ics, char inputLine[]);

int main () {
    /*FILE* file;
    file = fopen("events.ics","r");
    char* buffer;
    CalStatus returnValue;
    readCalLine(NULL,NULL);
    while (!feof(file)) {
        returnValue = readCalLine(file, &buffer);
        if (returnValue.code == OK) {
            printf("Lines %d to %d %s\n",returnValue.linefrom, returnValue.lineto, buffer);
        } else {
            printf("%u\n",returnValue.code);
            exit(0);
        }
        
    }*/
    CalProp *testProp = malloc(sizeof(*testProp));
    
    parseCalProp("TRIGGER;VALUE=DURATION;NIGGER=KFC;LAIN=CUTE:-PT30M", testProp);
    puts("\n\n");
    printf("\nName of prop:%s\nValue of prop:%s\n",testProp->name,testProp->value);
    if (testProp->nparams > 0) {
        printf("%d PARAMETERS\n",testProp->nparams);
        CalParam* temp = testProp->param;
        while (true) {
            printf("Name:%s\n",temp->name);
            printf("%d VALUES-\n",temp->nvalues);
            for (int j = 0; j < temp->nvalues; j++) { 
                printf("%s\n",temp->value[j]);
            }
            if (temp->next) {
                temp = temp->next;
            } else {
              break;   
            }
        }
    } else {
        puts("NO PARAMS");
    }
    
    return 1;
}

CalStatus makeCalStatus (CalError code, int linefrom, int lineto) {
    CalStatus returnStatus;
    returnStatus.code = code;
    returnStatus.linefrom = linefrom;
    returnStatus.lineto = lineto;
    return returnStatus;    
}

//pcomp points to var where address of newly allocated calcomp structure will be returned
//ics is file pointer. precondition is that it is open for reading, is at beginning of file

//function returns CalStatus struct with status OK if file is read to EOF
//Calstatus line numbers give the total lines read from file
//FREE PCOMP USING PCALFILE
CalStatus readCalFile( FILE *const ics, CalComp **const pcomp ) {
    readCalLine(NULL, NULL);
    //Potentially move this into another function
    //Initialize pcomp for use, call readcalLine with null ^^
    (*pcomp) = malloc(sizeof(CalComp));
    (*pcomp)->name = NULL;
    (*pcomp)->nprops = 0;
    (*pcomp)->prop = NULL;
    (*pcomp)->ncomps = 0;
    //May not be needed ((*pcomp))->comp[0] = NULL;
    
    //Read in components
    
    CalStatus readStatus;
    

    readStatus = readCalComp(ics, pcomp);
    //If error found at or after this point, call freecalcomp and return
    if (readStatus.code != OK) {
        freeCalComp(*pcomp);
    }
    
    //otherwise
    
    //Go through components to do error checking
    //Probably best to put this in a function
    
    //Check version and product ID
    bool hasVersion = false;
    bool hasProdID = false;
    CalProp *traverseProps = (*pcomp)->prop;
    
    for (int i = 0; i < (*pcomp)->nprops; i++) {
        //Version checking
        if (strcmp(traverseProps->name,"VERSION") == 0) {
            //If there is already a version property, or if version != 2.0, BADVER
           if (hasVersion || strcmp(traverseProps->value,VCAL_VER) != 0) {
               return makeCalStatus(BADVER,0,0); //change this
           } 
           hasVersion = true;
        }
        //PID checking
        if (strcmp(traverseProps->name,"PRODID") == 0) {
            //If it already exists, or if the value is null or empty
            if (traverseProps->value == NULL) {
                return makeCalStatus(NOPROD,0,0); //change this;
            }
            if (hasProdID || strcmp(traverseProps->value,"") == 0) {
                return makeCalStatus(NOPROD,0,0); //change this
            }
            hasProdID = true;
        }
        traverseProps = traverseProps->next;
    }
    
    //Check components
    bool startWithV = false;
    for (int i = 0; i < (*pcomp)->ncomps; i++) {
        if ((*pcomp)->comp[i]->name[0] == 'V') {
            startWithV = true;
        }    
    }
    //If none start with V, error
    if(!startWithV) {
        return makeCalStatus(NOCAL,0,0); //change this
    }
    //Check EOF. If there are lines after END:VCALENDAR, error
    if (!feof(ics)) {
       return makeCalStatus(AFTEND,0,0); //change this
    }
    return readStatus;
}

//This funciton called at start of a new BEGIN block

//Note. This implementation does not call parseCalProp if *pcomp->name == NULL
//It simply checks the buffer instead of deconstructing a CalProp
CalStatus readCalComp( FILE *const ics, CalComp **const pcomp ) {
    static int callDepth;
    char* pbuff = NULL;
    CalStatus returnStatus;
  //  bool endFound = false;
    
    
    
    //Might need more than 5
    
    while (!feof(ics)) {
        
         returnStatus = readCalLine(ics, &pbuff);
         
         //Check to make sure start is BEGIN:VCALENDAR
         if ((*pcomp)->name == NULL) {
             if (strcmp(pbuff,"BEGIN:VCALENDAR") == 0) {
                 (*pcomp)->name = malloc(9);
                 strcpy((*pcomp)->name,"VCALENDAR");
                 callDepth = 1;
                 continue;
             } else {
                 //return an error
             }
         }
         
         //Assuming it's not begin vcal
         CalProp *prop = malloc(sizeof(prop));
         parseCalProp(pbuff, prop);
         
         //At this point, we have our prop structure filled in with SOMETHING
         //CHECK IF IT'S A PROP OR A COMP
         
         //First case: another comp
         if (strcmp(prop->name,"BEGIN") == 0) {
             //Realloc to make space for next component
             //Define name of next component using value of prop struct (Should work...)
             //Call readCalComp again with this new comp
             (*pcomp) = realloc((*pcomp), (sizeof(*pcomp) + (*pcomp)->ncomps*sizeof(CalComp*)));
             
             (*pcomp)->comp[(*pcomp)->ncomps]->name = malloc(sizeof(prop->value));
             strcpy((*pcomp)->comp[(*pcomp)->ncomps]->name,prop->value);
             
             //Free the now unneeded property struct
             free(prop);
             
             callDepth++;
             
             
         } else if (strcmp(prop->name,"END") == 0) {
             //make sure it matches the current comp, i.e. it matches the begin
             //decrement call depth, free prop memory, return
         }
         
 

         //Malloc memory in pcomp array, do memcpy
    }
    free(pbuff);
    return returnStatus;
}

//This functions seems to be completed
CalStatus readCalLine( FILE *const ics, char **const pbuff ) {
    static int currentLine;
    static int difference;
    static char inputLine[500];
    
    if (ics == NULL) {
        //do reset things
        puts("Reset successful");
        currentLine = 0;
        difference = 0;
        inputLine[0] = '\0';
        return makeCalStatus(OK,0,0);
    }
    
    if (feof(ics)) {
        //EOF case
    }
    
    //Increment line number, reset diff
    if (difference != 0) {
        currentLine += difference;
    }
    difference = 0;
    currentLine++;
    
    
    if (difference == 0) {
        // '\0' defines the initial case
        if (inputLine[0] == '\0') {
            fgets(inputLine, 500, ics);
            puts("Grabbing input line");
            while (checkEmptyString(inputLine) == true) {
                fgets(inputLine, 500, ics);
            }
        }
        
        //CRLF check
        if (!hasCRLF(ics, inputLine)) {
            return makeCalStatus(NOCRNL, currentLine, currentLine+difference);
        }   
        strcpy(*pbuff,inputLine);
    }
    
    fgets(inputLine, 500, ics);
    
    //Manage line folding if next line has a space
    while (inputLine[0] == ' ') {
        if (checkEmptyString(inputLine) == false) {

            //CRLF check for folded lines
            if (!hasCRLF(ics, inputLine)) {
                return makeCalStatus(NOCRNL, currentLine, currentLine+difference);
            }
            
            //Then shift every element in inputLine over by 1 to remove leading space
            int tempLength = strlen(inputLine);
            for (int i = 0; i < tempLength; i++) {
                inputLine[i] = inputLine[i+1];
            }
            
            difference++;
            *pbuff = realloc(*pbuff,(strlen(*pbuff) + strlen(inputLine)+1));
            strcat(*pbuff, inputLine);   
        }
        fgets(inputLine, 500, ics);
    }
    
    
    //If the buffer is somehow empty, recursively call to get next line
    //Yes this is a hack solution that doesn't fix the actual bug, but there's a deadline
    if (strlen(*pbuff) == 0){
        return readCalLine(ics,pbuff);
    }
    return makeCalStatus(OK, currentLine, currentLine+difference);
}

bool hasCRLF (FILE *const ics, char inputLine[]) {
    if (!feof(ics)) {
        if (inputLine[strlen(inputLine)-2] == '\r' && inputLine[strlen(inputLine)-1] == '\n') {
            inputLine[strlen(inputLine)-2] = '\0';           
            return true;
        } else {
            return false;
        }
    }
    return true;
}

bool checkEmptyString(const char *line) {
    int length = strlen(line);
    if (length == 0) {
        return true;
    }
    for (int i = 0; i < length; i++) {
        if (!isspace(line[i])) {
            return false;
        }
    }
    return true;
}

CalError parseCalProp( char *const buff, CalProp *const prop ) {
    prop->name = NULL;
    prop->value = NULL;
    prop->nparams = 0;
    prop->param = NULL;
    prop->next = NULL;
    char* buffCpy;
    char* name;
    char* propValue;
    
    buffCpy = malloc(strlen(buff)+1);
    strcpy(buffCpy,buff);
    
    //Implies zero length name
    if (buffCpy[0] == ':') {
        return SYNTAX;
    }
    
    int length = strlen(buffCpy);
    
    /*Simplestring
    2 = simple. No params. NAME:VALUE
    1 = complex. Do analysis.
    0 = Error with undefined state.
    */
    int simpleString = 0;
    for (int i = 0; i < length; i++) {
        if (buffCpy[i] == ':') {
            puts("SIMPLE");
            simpleString = 2;
            break;
        } else if (buffCpy[i] == ';') {
            simpleString = 1;
            puts("COMPLEX");
            break;
        }
    }
    
    //If simpleString = 0 something is horribly wrong
    //If simpleString = 2, simple case, easy parsing
    if (simpleString == 0) {
       return SYNTAX; 
   } else if (simpleString == 2) {
        name = strtok(buffCpy,":");
        prop->name = malloc(strlen(name)+1);
        strcpy(prop->name,name);
        
        propValue = strtok(NULL,":");
        if (propValue == NULL) {
            prop->value = malloc(1);
            strcpy(prop->value,"");
        } else {
            prop->value = malloc(strlen(propValue)+1);
            strcpy(prop->value,propValue);
        }
        return OK;
    } else {
        //Find the position of the actual : between params and prop value
       int propValueDelimeter = -1;
       bool preceedsSemiOrQuote = false;
       
       for (int i = 0; i < length; i++) {
           if (buffCpy[i] == ':') {
               preceedsSemiOrQuote = false;
               for (int j = i+1; j < length; j++) {
                   if (buffCpy[j] == '"' || buffCpy[j] == ';') {
                       preceedsSemiOrQuote = true;
                       break;
                   }
                }
                if (preceedsSemiOrQuote == false) {
                    propValueDelimeter = i;
                    break;
                }
           }
       }
       //No delimeter, syntax error
       if (propValueDelimeter == -1) {
           return SYNTAX;
       }
       
       //Build prop value using delimeter position
       //Hopefully this copies the NUL terminator
       prop->value = malloc(strlen(buffCpy)-propValueDelimeter+1);
       for (int i = propValueDelimeter+1; i < length+1; i++) {
           prop->value[i - propValueDelimeter - 1] = buffCpy[i]; 
       }
       buffCpy[propValueDelimeter] = '\0';
       printf("Buffer is %s\n",buffCpy);
       
       //Find length of name section
       int curPos = 0;
       length = strlen(buffCpy);
       for (int i = 0; i < length; i++) {
           curPos++;
           if (buffCpy[i] == ';') {
               break;
           }
       }
       
       //Copy over name of prop
       prop->name = malloc(curPos+1);
       for (int i = 0; i < curPos; i++) {
           prop->name[i] = buffCpy[i];
       }
       prop->name[curPos] = '\0';
       
       printf("Buffer now is ");
       for (int i = curPos; i < length; i++) {
           printf("%c",buffCpy[i]);
       }
       puts("");
       
       //Analyze the individual parameters to avoid errors
       //Go over the string, do case analysis for each ; found
       int lasPos = curPos;
       bool withinQuotes = false;
       for (int k = curPos; k < length; k++) {
           //If the next char is ", invert withinQuotes
           if (buffCpy[k+1] == '"') {
               withinQuotes = (!withinQuotes);
           }
           
           //If next char is ; and we arent in quotes, this is the end of the current param block
           if ((buffCpy[k+1] == ';' || buffCpy[k+1] == '\0') && withinQuotes == false) {
               puts("BEGINNING PARSE!");
               CalParam *newParam = malloc(sizeof(CalParam));
               //Copy param and value segment into a temporary string
               
               char param[100] = {0};
               for (int i = lasPos; i < k+1; i++) {
                   param[i-lasPos] = buffCpy[i];
               }
               param[strlen(param)] = '\0';
               printf("Param str: %s\n",param);
               
               //If we have no =, syntax failure
               if (strchr(param,'=') == NULL) {
                   return SYNTAX;
               } else {
                   //Split the parameter string into name and values
                   char* paramName = strtok(param,"=");
                   char* paramValues = strtok(NULL,"\0");
                   printf("Param values after strtok: %s\n",paramValues);
                   //Fixing strtok bug with zero length strings
                   if (paramValues == NULL) {
                       paramValues = "";
                   }
                   newParam->name = malloc(strlen(paramName)+1);
                   newParam->next = NULL;
                   strcpy(newParam->name,paramName);
                   newParam->nvalues = 1;

                   //If we have commas, it's possible that we have
                   //Multiple values for the parameter
                   if (strchr(paramValues,',')) {
                       //Do multi value analysis
                       bool valueInQuotes = false;
                       int length = strlen(paramValues);
                       int oldPos = 0;
                       for (int i = 0; i < length; i++) {
                           //Once you find end of a parameter value, add it
                           if (paramValues[i + 1] == ',' && valueInQuotes == false) {
                               char tempValue[i - oldPos + 1];
                               for (int j = 0; j < (i - oldPos); i++) {
                                   tempValue[j] = paramValues[j + oldPos];
                               }
                               newParam->nvalues++;
                               (newParam) = realloc((newParam), (sizeof(*newParam) + newParam->nvalues*sizeof(char*)));
                               (newParam)->value[newParam->nvalues-1] = malloc(strlen(paramValues)+1);
                               strcpy((newParam)->value[newParam->nvalues-1],paramValues);
                               
                               oldPos = i;
                           }
                       }
                   } else {
                       puts("other case");
                       //No commas, one value
                       (newParam) = realloc((newParam), (sizeof(*newParam) + sizeof(char*)));
                       ((newParam)->value[0]) = malloc(strlen(paramValues)+1);
                       strcpy((newParam)->value[0],paramValues);
                       printf("Param values: %s\n",paramValues);
                       printf("Copied buffer: %s\n",(newParam)->value[0]);
                   }
               }
               puts("PAST PARSING");
               //Add CalParam to list
               if (prop->param == NULL) {
                   prop->param = newParam;
               } else {
                   
                   CalParam* temp = prop->param;
                   while (temp->next != NULL) {
                       puts("YO");
                       temp = temp->next;
                   }
                   
                   temp->next = newParam;
                   if (prop->param->next != NULL) {
                        printf("TEMP NEXT %s",temp->next->name);
                   }
               }
               prop->nparams++;
               
           
           //Move up the last position    
           k+=2;
           lasPos = k;
           
           }
       }
    }
    free(buffCpy);
    return OK;
}

/*bool isValidToken(char *token) {
    if (token == NULL) {
        return false;
    } else if (strlen(token == 0)) {
        return false;
    } else {
        return true;
    }
}*/

//Free params of props, free props of subcomps, free subcomps of comps, free comp
//This function seems to be completed
void freeCalComp( CalComp *const comp ) {
    
    //Go along list of sub components, moving to deeper sub components if needed
    //Can subcomponents have subcomponents? Doesn't hurt to check I guess
    for (int i = 0; i < comp->ncomps; i++) {
        
        //If a sub component has a sub component [inception]
        if (comp->comp[i]->ncomps > 0) {
            freeCalComp(comp->comp[i]);
        }
            
        //If a component has properties
        if (comp->comp[i]->prop) {
            freeCalProp(comp->prop);
        }
        free(comp->comp[i]->name);
        free(comp->comp[i]->comp);
    }
    free(comp->name);
    free(comp);

}

void freeCalProp (CalProp *const prop) {
    CalProp *traverseProps = prop;
    CalProp *nextProp;
            
    //Traverse properties
    while (traverseProps) {
        nextProp = traverseProps->next;
        
        //Free name and value of property
        free(traverseProps->name);
        free(traverseProps->value);
        
        //If the property has parameters
        if(traverseProps->param) {
            freeCalParam(traverseProps->param);
        }
        free(traverseProps);
        traverseProps = nextProp;
    }
}

void freeCalParam (CalParam *const param) { 
    CalParam *traverseParams = param;
    CalParam *nextParam;
    
    //Traverse parameters of properties
    while (traverseParams) {
        nextParam = traverseParams->next;
        
        //Free name of parameter
        free(traverseParams->name);
        
        //Free values of parameter
        for (int i = 0; i < traverseParams->nvalues; i++) {
            free(traverseParams->value[i]);
        }
        free(traverseParams);
        traverseParams = nextParam;
    }
}


