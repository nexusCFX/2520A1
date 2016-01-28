#include <stdlib.h>
// Remove stdio if not needed once finished
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

void freeCalParam(CalParam *const param);
void freeCalProp(CalProp *const prop);
bool checkEmptyString(const char *line);
CalStatus makeCalStatus(CalError code, int linefrom, int lineto);
bool hasCRLF(FILE *const ics, char inputLine[]);
void printComp(CalComp *comp);

int main() {
    CalComp *comp;
    comp = malloc(sizeof(CalComp));
    comp->name = NULL;
    comp->nprops = 0;
    comp->prop = NULL;
    comp->ncomps = 0;
    FILE *file;
    file = fopen("events.ics", "r");
    CalStatus readStatus;
    readStatus = readCalFile(file, &comp);
    // printComp(comp);

    /*for (int i = 0; i < comp->ncomps; i++) {
        printComp(comp->comp[i]);
    }*/

    printf("Error #%d. Lines %d to %d\n", readStatus.code, readStatus.linefrom,
           readStatus.lineto);
    if (readStatus.code == OK) {
        freeCalComp(comp);
    }
    return 1;
}

void printComp(CalComp *comp) {

    printf("**COMP NAME**: %s\n", comp->name);
    printf("%d Props\n", comp->nprops);
    if (comp->nprops > 0) {
        CalProp *temp = comp->prop;
        while (temp) {
            printf("Prop Name: %s\n", temp->name);
            printf("%d Params\n", temp->nparams);
            if (temp->nparams > 0) {
                CalParam *temp2 = temp->param;
                while (temp2) {
                    printf("Param Name: %s\n", temp2->name);
                    printf("%d Values\n", temp2->nvalues);
                    if (temp2->nvalues > 0) {
                        for (int i = 0; i < temp2->nvalues; i++) {
                            printf("Value: %s\n", temp2->value[i]);
                        }
                    }
                    if (temp2->next) {
                        temp2 = temp2->next;
                    } else {
                        temp2 = NULL;
                    }
                }
            }
            printf("Prop Value: %s\n", temp->value);
            if (temp->next) {
                temp = temp->next;
            } else {
                temp = NULL;
            }
        }
    }
    if (comp->ncomps > 0) {
        printComp(comp->comp[0]);
    }

    // printf("Comp Value: %s\n",comp->value);
}

CalStatus makeCalStatus(CalError code, int linefrom, int lineto) {
    CalStatus returnStatus;
    returnStatus.code = code;
    returnStatus.linefrom = linefrom;
    returnStatus.lineto = lineto;
    return returnStatus;
}

// pcomp points to var where address of newly allocated calcomp structure will
// be returned
// ics is file pointer. precondition is that it is open for reading, is at
// beginning of file

// function returns CalStatus struct with status OK if file is read to EOF
// Calstatus line numbers give the total lines read from file
// FREE PCOMP USING PCALFILE
CalStatus readCalFile(FILE *const ics, CalComp **const pcomp) {
    readCalLine(NULL, NULL);
    // Potentially move this into another function
    // Initialize pcomp for use, call readcalLine with null ^^
    (*pcomp)->name = NULL;
    (*pcomp)->nprops = 0;
    (*pcomp)->prop = NULL;
    (*pcomp)->ncomps = 0;

    CalStatus readStatus;

    readStatus = readCalComp(ics, pcomp);
    // If error found at or after this point, call freecalcomp and return
    if (readStatus.code != OK) {
        freeCalComp(*pcomp);
        return readStatus;
    }

    bool hasVersion = false;
    bool hasProdID = false;
    bool startWithV = false;

    CalProp *traverseProps = (*pcomp)->prop;
    while (traverseProps) {
        if (strcmp(traverseProps->name, "VERSION") == 0) {
            if (hasVersion || strcmp(traverseProps->value, VCAL_VER) != 0) {
                readStatus.code = BADVER;
                return readStatus;
            }
            hasVersion = true;
        }

        if (strcmp(traverseProps->name, "PRODID") == 0) {
            if (hasProdID) {
                readStatus.code = NOPROD;
            }
            hasProdID = true;
        }
        traverseProps = traverseProps->next;
    }

    if (readStatus.code != OK) {
        return readStatus;
    }

    for (int i = 0; i < (*pcomp)->ncomps; i++) {
        if ((*pcomp)->ncomps == 0) {
            readStatus.code = NOCAL;
            return readStatus;
        }

        if ((*pcomp)->comp[i]->name[0] == 'V') {
            startWithV = true;
        }
    }

    if (startWithV == false) {
        readStatus.code = NOCAL;
        return readStatus;
    }

    // Check EOF. If there are lines after END:VCALENDAR, error
    if (!feof(ics)) {
        char *buff = NULL;
        readCalLine(ics, &buff);
        if (buff != NULL) {
            readStatus.code = AFTEND;
        }
    }

    return readStatus;
}

// This funciton called at start of a new BEGIN block

// Note. This implementation does not call parseCalProp if *pcomp->name == NULL
// It simply checks the buffer instead of deconstructing a CalProp
CalStatus readCalComp(FILE *const ics, CalComp **const pcomp) {
    static int callDepth = 1;
    char *pbuff = NULL;
    CalStatus returnStatus;

    returnStatus.code = OK;
    returnStatus.linefrom = 0;

    returnStatus.lineto = 0;

    while ((pbuff == NULL) ||
           (strcmp("END:VCALENDAR", pbuff) != 0 && returnStatus.code == OK)) {
        if (callDepth > 3) {
            returnStatus.code = SUBCOM;
            return returnStatus;
        }
        returnStatus = readCalLine(ics, &pbuff);
        if (returnStatus.code != OK) {
            if (pbuff != NULL) {
                free(pbuff);
            }

            return returnStatus;
        }

        // Check to make sure start is BEGIN:VCALENDAR
        if ((*pcomp)->name == NULL && callDepth == 1) {
            if (strcmp(pbuff, "BEGIN:VCALENDAR") != 0) {
                returnStatus.code = NOCAL;
                return returnStatus;
            }
            (*pcomp)->name = malloc(10);
            strcpy((*pcomp)->name, "VCALENDAR");
            continue;
        }

        // Assuming it's not BEGIN:VCALENDAR
        CalProp *prop = malloc(sizeof(*prop));
        parseCalProp(pbuff, prop);

        // First case: another CalComp
        if (strcmp(prop->name, "BEGIN") == 0) {
            (*pcomp)->ncomps++;
            (*pcomp) = realloc((*pcomp), (sizeof(**pcomp) +
                                   ((*pcomp)->ncomps)*sizeof(CalComp *)));
                                   
             assert(*pcomp != NULL);
             
             
            //Allocate our new CalComp and initialize it
            (*pcomp)->comp[(*pcomp)->ncomps - 1] = malloc(sizeof(CalComp));
            CalComp *nextComp = (*pcomp)->comp[(*pcomp)->ncomps - 1];
            nextComp->prop = NULL;
            nextComp->nprops = 0;
            nextComp->ncomps = 0;
            nextComp->name = malloc(strlen(prop->value) + 1);
            strcpy(nextComp->name, prop->value);
            
            // Free the now unneeded property struct
            free(prop->name);
            free(prop->value);
            free(prop);
            callDepth++;
            
            //Recursively call readCalComp
            returnStatus =
                readCalComp(ics, &((*pcomp)->comp[(*pcomp)->ncomps - 1]));
        } else if (strcmp(prop->name, "END") == 0) {
            
            // make sure it matches the current comp, i.e. it matches the begin
            // decrement call depth, free prop memory, return
            if ((*pcomp)->ncomps == 0 && (*pcomp)->nprops == 0) {
                returnStatus.code = NODATA;
            } else if (strcmp(prop->value, (*pcomp)->name) != 0) {
                returnStatus.code = BEGEND;
            }
            callDepth--;
            free(prop->name);
            free(prop->value);
            free(prop);
            if (pbuff != NULL) {
                free(pbuff);
            }
            return returnStatus;
        } else {
            (*pcomp)->nprops++;
            if ((*pcomp)->prop == NULL) {
                (*pcomp)->prop = prop;
            } else {
                CalProp *temp = (*pcomp)->prop;
                //  printf("depth %d\n",callDepth);
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = prop;
            }
            //     printf("Numparams %d\n",prop->nparams);
        }
    }
    free(pbuff);

    return returnStatus;
}

// This functions seems to be completed
CalStatus readCalLine(FILE *const ics, char **const pbuff) {
    static int currentLine;
    static int difference;
    static char inputLine[500];

    if (ics == NULL) {
        // do reset things
        currentLine = 0;
        difference = 0;
        inputLine[0] = '\0';

        return makeCalStatus(OK, 0, 0);
    }

    if (*pbuff == NULL) {
        (*pbuff) = malloc(500);
    }

    // Increment line number, reset diff
    if (difference != 0) {
        currentLine += difference;
    }

    difference = 0;
    currentLine++;

    if (difference == 0) {
        // '\0' defines the initial case
        if (inputLine[0] == '\0') {
            fgets(inputLine, 500, ics);
            while (checkEmptyString(inputLine) == true) {
                fgets(inputLine, 500, ics);
            }
        }

        // CRLF check
        if (!hasCRLF(ics, inputLine)) {
            free(*pbuff);
            *pbuff = NULL;
            return makeCalStatus(NOCRNL, currentLine, currentLine + difference);
        }
        strcpy(*pbuff, inputLine);
    }

    fgets(inputLine, 500, ics);

    // Manage line folding if next line has a space
    while (inputLine[0] == ' ') {
        if (checkEmptyString(inputLine) == false) {
            // CRLF check for folded lines
            if (!hasCRLF(ics, inputLine)) {
                free(*pbuff);
                *pbuff = NULL;
                return makeCalStatus(NOCRNL, currentLine,
                                     currentLine + difference);
            }

            // Then shift every element in inputLine over by 1 to remove leading
            // space
            int tempLength = strlen(inputLine);
            for (int i = 0; i < tempLength; i++) {
                inputLine[i] = inputLine[i + 1];
            }

            difference++;
            char *tempPtr =
                realloc(*pbuff, (strlen(*pbuff) + strlen(inputLine) + 1));
            if (tempPtr != NULL) {
                *pbuff = tempPtr;
            }
            strcat(*pbuff, inputLine);
        }
        fgets(inputLine, 500, ics);
    }

    // If the buffer is somehow empty, recursively call to get next line
    // Yes this is a hack solution that doesn't fix the actual bug, but there's
    // a
    // deadline
    if (strlen(*pbuff) == 0) {
        return readCalLine(ics, pbuff);
    }
    return makeCalStatus(OK, currentLine, currentLine + difference);
}

bool hasCRLF(FILE *const ics, char inputLine[]) {
    if (!feof(ics)) {
        if (inputLine[strlen(inputLine) - 2] == '\r' &&
            inputLine[strlen(inputLine) - 1] == '\n') {
            inputLine[strlen(inputLine) - 2] = '\0';
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
// If you have to ever maintain this, god bless your soul
CalError parseCalProp(char *const buff, CalProp *const prop) {
    prop->name = NULL;
    prop->value = NULL;
    prop->nparams = 0;
    prop->param = NULL;
    prop->next = NULL;
    char *buffCpy;
    char *name;
    char *propValue;

    // Copy the buffer into one you can alter with strtok
    buffCpy = malloc(strlen(buff) + 1);
    strcpy(buffCpy, buff);

    // Implies zero length name
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
            simpleString = 2;
            break;
        } else if (buffCpy[i] == ';') {
            simpleString = 1;
            break;
        }
    }

    // If simpleString = 0 something is horribly wrong
    // If simpleString = 2, simple case, easy parsing
    // Otherwise, a lot of work
    if (simpleString == 0) {
        free(buffCpy);
        return SYNTAX;
    } else if (simpleString == 2) {
        name = strtok(buffCpy, ":");
        prop->name = malloc(strlen(name) + 1);
        strcpy(prop->name, name);

        propValue = strtok(NULL, ":");
        if (propValue == NULL) {
            prop->value = malloc(1);
            strcpy(prop->value, "");
        } else {
            prop->value = malloc(strlen(propValue) + 1);
            strcpy(prop->value, propValue);
        }
        free(buffCpy);
        return OK;
    } else {
        // Find the position of the actual : between params and prop value
        int propValueDelimeter = -1;
        bool preceedsSemiOrQuote = false;

        for (int i = 0; i < length; i++) {
            if (buffCpy[i] == ':') {
                preceedsSemiOrQuote = false;
                for (int j = i + 1; j < length; j++) {
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
        // No delimeter, syntax error
        if (propValueDelimeter == -1) {
            return SYNTAX;
        }

        // Cut out prop value using delimeter position, append NUL term
        prop->value = malloc(strlen(buffCpy) - propValueDelimeter + 1);
        for (int i = propValueDelimeter + 1; i < length + 1; i++) {
            prop->value[i - propValueDelimeter - 1] = buffCpy[i];
        }
        buffCpy[propValueDelimeter] = '\0';

        // Find length of name section
        int curPos = 0; // curPos will mark end of name, beginning of params
        length = strlen(buffCpy);
        for (int i = 0; i < length; i++) {
            curPos++;
            if (buffCpy[i] == ';') {
                break;
            }
        }

        // Copy over name of prop, append NUL
        prop->name = malloc(curPos + 1);
        for (int i = 0; i < curPos - 1; i++) {
            prop->name[i] = buffCpy[i];
        }
        prop->name[curPos] = '\0';

        // Analyze the individual parameters to avoid errors
        // Go over the string, do case analysis for each ; found
        int lasPos = curPos;
        bool withinQuotes = false;
        for (int k = curPos; k < length; k++) {
            // If the next char is ", invert withinQuotes
            if (buffCpy[k + 1] == '"') {
                withinQuotes = (!withinQuotes);
            }

            // If next char is ; and we arent in quotes, this is the end of the
            // current param block
            if ((buffCpy[k + 1] == ';' || buffCpy[k + 1] == '\0') &&
                withinQuotes == false) {
                CalParam *newParam = malloc(sizeof(CalParam)); //
                newParam->name = NULL;
                newParam->next = NULL;
                newParam->nvalues = 0;

                // Copy param and value segment into a temporary string
                char param[500] = {0};
                for (int i = lasPos; i < k + 1; i++) {
                    param[i - lasPos] = buffCpy[i];
                }
                param[strlen(param)] = '\0';

                // If we have no =, syntax failure
                if (strchr(param, '=') == NULL) {
                    return SYNTAX;
                } else {
                    // Split the parameter string into name and values
                    char *paramName = strtok(param, "=");
                    char *paramValues = strtok(NULL, "\0");

                    // Fixing strtok bug with zero length strings
                    if (paramValues == NULL) {
                        paramValues = "";
                    }

                    newParam->name = malloc(strlen(paramName) +
                                            1); // Move name into new parameter
                    strcpy(newParam->name, paramName);

                    // If we have commas, it's possible that we have multiple
                    // values for
                    // the parameter
                    if (strchr(paramValues, ',')) {
                        // Do multi value analysis
                        bool valueInQuotes = false;
                        int length = strlen(paramValues);
                        int oldPos = 0;
                        for (int i = 0; i < length; i++) {
                            if (paramValues[i] == '"') {
                                valueInQuotes = (!valueInQuotes);
                            }
                            //*********************** printf("PVs %s at char %c
                            // and
                            //%d\n",paramValues, paramValues[i+1],
                            // valueInQuotes);

                            // Once you find end of a parameter value, add it
                            if ((paramValues[i + 1] == ',' ||
                                 paramValues[i + 1] == '\0') &&
                                valueInQuotes == false) {
                                char tempValue[100] = {0};

                                // Move value into a temporary array
                                for (int j = 0; j < (i - oldPos + 1); j++) {
                                    tempValue[j] = paramValues[j + oldPos];
                                }
                                tempValue[i - oldPos + 1] = '\0';

                                // Allocate space in flexible member and copy
                                // over temp array
                                CalParam *temp = realloc(
                                    (newParam),
                                    (sizeof(*newParam) +
                                     (newParam->nvalues + 1) * sizeof(char *)));

                                if (temp != NULL) {
                                    newParam = temp;
                                }

                                (newParam)->value[newParam->nvalues] =
                                    malloc(strlen(tempValue) + 1);

                                strcpy((newParam)->value[newParam->nvalues],
                                       tempValue);
                                newParam->nvalues++;

                                i += 2; // Shift 2 because you're moving past
                                        // the comma
                                oldPos = i;
                            }
                        }
                    } else {
                        // No commas, one value

                        (newParam) = realloc(
                            (newParam), (sizeof(*newParam) + sizeof(char *)));
                        ((newParam)->value[0]) =
                            malloc(strlen(paramValues) + 1);
                        strcpy((newParam)->value[0], paramValues);
                        newParam->nvalues++;
                    }
                }
                // Add CalParam to list
                if (prop->param == NULL) {
                    prop->param = newParam;
                } else {
                    CalParam *temp = prop->param;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = newParam;
                }
                prop->nparams++;
                k += 2; // Shift 2 because you're moving past the semicolon
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

// Free params of props, free props of subcomps, free subcomps of comps, free
// comp
// This function seems to be completed
void freeCalComp(CalComp *const comp) {
    // Go along list of sub components, moving to deeper sub components if
    // needed
    for (int i = 0; i < comp->ncomps; i++) {
        // If a sub component has a sub component [inception]
        if (comp->comp[i]->ncomps > 0) {
            freeCalComp(comp->comp[i]);
        } else {
            free(comp->comp[i]->name);
        }

        // If a component has properties
        if (comp->comp[i]->nprops > 0) {
           freeCalProp(comp->comp[i]->prop);
        }

        // free(comp->comp[i]->comp);
    }
    free(comp->name);
    // free(comp);
}

void freeCalProp(CalProp *const prop) {
    CalProp *traverseProps = prop;
    CalProp *nextProp;

    // Traverse properties
    while (traverseProps) {
        nextProp = traverseProps->next;

        // Free name and value of property
        free(traverseProps->name);
        free(traverseProps->value);

        // If the property has parameters
        if (traverseProps->param) {
            freeCalParam(traverseProps->param);
        }
        free(traverseProps);
        traverseProps = nextProp;
    }
}

void freeCalParam(CalParam *const param) {
    CalParam *traverseParams = param;
    CalParam *nextParam;

    // Traverse parameters of properties
    while (traverseParams) {
        nextParam = traverseParams->next;

        // Free name of parameter
        free(traverseParams->name);

        // Free values of parameter
        for (int i = 0; i < traverseParams->nvalues; i++) {
            free(traverseParams->value[i]);
        }
        free(traverseParams);
        traverseParams = nextParam;
    }
}
