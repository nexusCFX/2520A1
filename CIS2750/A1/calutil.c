#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "calutil.h"

#define BUF_LEN 4000

/************************
calutil.c

Implementation of iCalendar utility functions in calutil.h

Note to future programmers: This file conforms to the syntax used for the LLVM
project and this should be maintained with all future modifications to ensure
consistency.

Author: Brandon Chester : 0877477
Contact: bchester@mail.uoguelph.ca
Created: Jan 14, 2016
Last modified: Jan 29, 2016
*************************/

/*************************
complexStringParse
Internal function which parses input strings with greater complexity than those
of the form NAME:VALUE. Should only be called from parseCalProp

Arguments:
buffCpy: A copy of the buffer sent from readCalLine, which can be safely
modified.

prop: The CalProp structure that will be populated with the information stored
in buffCpy once parsed.

Return Value: A CalError which will either be OK or SYNTAX depending on the
validity of the input string.
*************************/
CalError complexStringParse(char *buffCpy, CalProp *const prop);

/*************************
checkEmptyString
Internal function which checks if a string is zero length or all white space.

Arguments:
line: The input line to be examined.

Return Value: A bool which will be true if the string is empty, or false if it
is not.
*************************/
bool checkEmptyString(const char *line);

/*************************
freeCalParam
Internal function which frees parameters belonging to a CalProp struct.

Arguments:
param: The first parameter of the CalProp.

Return Value: Not applicable.
*************************/
void freeCalParam(CalParam *const param);

/*************************
freeCalProp
Internal function which frees properties belonging to a CalParam struct.

Arguments:
prop: The first property of the CalParam.

Return Value: Not applicable.
*************************/
void freeCalProp(CalProp *const prop);

/*************************
makeCalStatus
Internal function which returns a CalStatus structure.

Arguments:
code: The error code to be included in the struct.
linefrom: The first line parsed by readCalLine before being unfolded.
lineto: The last line parsed by readCalLine before the error occurred.

Return Value: A CalStatus structure populated by the variables passed in.
*************************/
CalStatus makeCalStatus(CalError code, int linefrom, int lineto);

/*************************
hasCRLF
Internal function which checks if a line has the correct return character
sequence.

Arguments:
ics: A copy of the input file pointer which points to the file being read from
inputLine: The char array populated by fgets when reading a line from the file.

Return Value: A bool which will be true if the string has the CRLF convention,
or if the file hit EOF before that. False if the line does not have the correct
ending.
*************************/
bool hasCRLF(FILE *const ics, char inputLine[]);

/*************************
makeUpperCase
Internal functions which converts a string to upper case.

Arguments:
buffer: The character array to be turned into uppercase form.

NOTE: This function will not alter strings that begin and end with " which
follows the specifications defined for managing iCalendar data.

Return Value: Not applicable.
*************************/
void makeUpperCase(char *buffer);

CalStatus readCalFile(FILE *const ics, CalComp **const pcomp) {
    readCalLine(NULL, NULL);

    (*pcomp) = malloc(sizeof(CalComp));
    assert((*pcomp) != NULL);

    (*pcomp)->name = NULL;
    (*pcomp)->nprops = 0;
    (*pcomp)->prop = NULL;
    (*pcomp)->ncomps = 0;

    CalStatus readStatus = readCalComp(ics, pcomp);

    // If error found at or after this point, call freecalcomp and return
    if (readStatus.code != OK) {
        freeCalComp(*pcomp);
        return readStatus;
    }

    bool hasVersion = false;
    bool hasProdID = false;

    // Check to make sure we have "valid" VERSION and PRODID properties
    CalProp *traverseProps = (*pcomp)->prop;
    while (traverseProps) {
        if (strcmp(traverseProps->name, "VERSION") == 0) {
            if (hasVersion || strcmp(traverseProps->value, VCAL_VER) != 0) {
                readStatus.code = BADVER;
                freeCalComp(*pcomp);
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

    if (hasVersion == false) {
        readStatus.code = BADVER;
    } else if (hasProdID == false) {
        readStatus.code = NOPROD;
    }

    if (readStatus.code != OK) {
        freeCalComp(*pcomp);
        return readStatus;
    }

    // Make sure that at least one component starts with V
    bool startWithV = false;
    for (int i = 0; i < (*pcomp)->ncomps; i++) {
        if ((*pcomp)->ncomps == 0) {
            readStatus.code = NOCAL;
            freeCalComp(*pcomp);
            return readStatus;
        }

        if ((*pcomp)->comp[i]->name[0] == 'V') {
            startWithV = true;
        }
    }

    if (startWithV == false) {
        readStatus.code = NOCAL;
        freeCalComp(*pcomp);
        return readStatus;
    }

    // Check EOF. If there are lines after END:VCALENDAR, error
    if (!feof(ics)) {
        char *buff = NULL;
        readCalLine(ics, &buff);
        if (buff != NULL) {
            readStatus.code = AFTEND;
            readStatus.lineto++;
            readStatus.linefrom++;
            freeCalComp(*pcomp);
        }
    }
    return readStatus;
}

CalStatus readCalComp(FILE *const ics, CalComp **const pcomp) {
    static int callDepth = 1;
    char *pbuff = NULL;
    static CalStatus returnStatus;

    while ((!feof(ics)) && ((pbuff == NULL) ||
           (strcmp("END:VCALENDAR", pbuff) != 0 && returnStatus.code == OK))) {

        if (callDepth > 3) {
            returnStatus.code = SUBCOM;
            free(pbuff);
            return returnStatus;
        }

        returnStatus = readCalLine(ics, &pbuff);
        
        if (feof(ics)) {
            for (int i = 0; i < strlen(pbuff); i++) {
                pbuff[i] = toupper(pbuff[i]);
            }
            if (strcmp("END:VCALENDAR", pbuff) != 0) {
                returnStatus.code = BEGEND;
            }    
            
        }

        if (returnStatus.code != OK) {
            free(pbuff);
            return returnStatus;
        }

        // Check to make sure start is BEGIN:VCALENDAR
        if ((*pcomp)->name == NULL && callDepth == 1) {
            for (int i = 0; i < strlen(pbuff); i++) {
                pbuff[i] = toupper(pbuff[i]);
            }
            if (strcmp(pbuff, "BEGIN:VCALENDAR") != 0) {
                free(pbuff);
                returnStatus.code = NOCAL;
                return returnStatus;
            }
            (*pcomp)->name = malloc(10);
            assert((*pcomp)->name != NULL);
            strcpy((*pcomp)->name, "VCALENDAR");
            continue;
        }

        // Assuming it's not BEGIN:VCALENDAR
        CalProp *prop = malloc(sizeof(*prop));
        assert(prop != NULL);
        returnStatus.code = parseCalProp(pbuff, prop);

        if (returnStatus.code != OK) {
            free(pbuff);
            return returnStatus;
        }

        // First case: another CalComp
        if (strcmp(prop->name, "BEGIN") == 0) {
            (*pcomp)->ncomps++;

            (*pcomp) =
                realloc((*pcomp), (sizeof(**pcomp) +
                                   ((*pcomp)->ncomps) * sizeof(CalComp *)));

            assert(*pcomp != NULL);

            // Allocate our new CalComp and initialize it
            (*pcomp)->comp[(*pcomp)->ncomps - 1] = malloc(sizeof(CalComp));

            // Make a temporary pointer with a shorter name than the mess above
            CalComp *nextComp = (*pcomp)->comp[(*pcomp)->ncomps - 1];
            assert(nextComp != NULL);
            nextComp->prop = NULL;
            nextComp->nprops = 0;
            nextComp->ncomps = 0;
            nextComp->name = malloc(strlen(prop->value) + 1);
            assert(nextComp->name != NULL);
            makeUpperCase(prop->value);
            strcpy(nextComp->name, prop->value);

            // Free the now unneeded property struct
            free(prop->name);
            free(prop->value);
            free(prop);
            callDepth++;

            // Call readCalComp to add new properties to new component
            returnStatus =
                readCalComp(ics, &((*pcomp)->comp[(*pcomp)->ncomps - 1]));
        } else if (strcmp(prop->name, "END") == 0) {
            // make sure the END matches the original begin, free memory
            makeUpperCase(prop->value);
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
            // Add to property list
            if (returnStatus.code != OK) {
                free(pbuff);
                return returnStatus;
            }
            (*pcomp)->nprops++;
            if ((*pcomp)->prop == NULL) {
                (*pcomp)->prop = prop;
            } else {
                CalProp *temp = (*pcomp)->prop;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = prop;
            }
        }
        if (feof(ics) && callDepth > 1) {
            returnStatus.code = BEGEND;
            free(pbuff);
            return returnStatus;
        }
    }
    free(pbuff);

    return returnStatus;
}

CalStatus readCalLine(FILE *const ics, char **const pbuff) {
    static int currentLine;
    static int difference;
    static char inputLine[BUF_LEN];

    if (ics == NULL) {
        // Reset function. Set input line to symbolic "empty"
        currentLine = 0;
        difference = 0;
        inputLine[0] = '\0';

        return makeCalStatus(OK, 0, 0);
    }

    if ((*pbuff) == NULL) {
        *pbuff = malloc(BUF_LEN);
        assert(*pbuff != NULL);
    }

    // Increment line number, reset diff
    if (difference != 0) {
        currentLine += difference;
    }

    difference = 0;
    currentLine++;

    if (difference == 0) {
        // \0 defines the initial case
        if (inputLine[0] == '\0') {
            if (fgets(inputLine, BUF_LEN, ics) == NULL) {
                free(*pbuff);
                *pbuff = NULL;
                return makeCalStatus(NOCAL, 0, 0);
            }
            while (checkEmptyString(inputLine) == true) {
                fgets(inputLine, BUF_LEN, ics);
            }
            if (feof(ics)) {
                if (inputLine[0] != '\0') {
                    strcpy(*pbuff, inputLine);
                } else {
                   free(*pbuff);
                   *pbuff = NULL; 
                }
                
                return makeCalStatus(OK, currentLine, currentLine + difference);
            }
        }

        if (!hasCRLF(ics, inputLine)) {
            free(*pbuff);
            *pbuff = NULL;
            return makeCalStatus(NOCRNL, currentLine, currentLine + difference);
        }
        strcpy(*pbuff, inputLine);
    }

    fgets(inputLine, BUF_LEN, ics);

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

            // Shift every element in inputLine over to remove leading space
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
        fgets(inputLine, BUF_LEN, ics);
    }

    // If the buffer is somehow empty, recursively call to get next line
    if (strlen(*pbuff) == 0) {
        return readCalLine(ics, pbuff);
    }
    return makeCalStatus(OK, currentLine, currentLine + difference);
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

CalStatus makeCalStatus(CalError code, int linefrom, int lineto) {
    CalStatus returnStatus;
    returnStatus.code = code;
    returnStatus.linefrom = linefrom;
    returnStatus.lineto = lineto;
    return returnStatus;
}

CalError parseCalProp(char *const buff, CalProp *const prop) {
    char *buffCpy;
    char *name;
    char *propValue;

    prop->name = NULL;
    prop->value = NULL;
    prop->nparams = 0;
    prop->param = NULL;
    prop->next = NULL;

    // Copy the buffer into one you can alter with strtok
    buffCpy = malloc(strlen(buff) + 1);
    assert(buffCpy != NULL);
    strcpy(buffCpy, buff);

    // Implies invalid zero length name
    if (buffCpy[0] == ':') {
        free(buffCpy);
        return SYNTAX;
    }

    int length = strlen(buffCpy);

    /*String cases for simpleString
    2 = Complex. Do analysis.
    1 = Simple. No params. NAME:VALUE
    0 = Error with undefined state.*/
    int simpleString = 0;
    for (int i = 0; i < length; i++) {
        if (buffCpy[i] == ':') {
            simpleString = 1;
            break;
        } else if (buffCpy[i] == ';') {
            simpleString = 2;
            break;
        }
    }

    // Parse string based on its form
    if (simpleString == 0) {
        free(buffCpy);
        return SYNTAX;
    } else if (simpleString == 1) {
        name = strtok(buffCpy, ":");
        prop->name = malloc(strlen(name) + 1);
        assert(prop->name != NULL);
        makeUpperCase(name);
        strcpy(prop->name, name);
        propValue = strtok(NULL, "\0");

        if (propValue == NULL) {
            prop->value = malloc(1);
            assert(prop->value != NULL);
            strcpy(prop->value, "");
        } else {
            prop->value = malloc(strlen(propValue) + 1);
            assert(prop->value != NULL);
            strcpy(prop->value, propValue);
        }
        free(buffCpy);
        return OK;
    } else {
        CalError parseCheck = complexStringParse(buffCpy, prop);
        free(buffCpy);
        return parseCheck;
    }
}

void makeUpperCase(char *buffer) {
    if (buffer != NULL) {
        int length = strlen(buffer);
        if (!(buffer[0] == '"' && buffer[length - 1] == '"')) {
            for (int i = 0; i < length; i++) {
                buffer[i] = toupper(buffer[i]);
            }
        }
    }
}

CalError complexStringParse(char *buffCpy, CalProp *const prop) {
    int length = strlen(buffCpy);
    int propValueDelimeter = -1;
    bool preceedsSemiOrQuote = false;

    // Find the position of the actual : between params and prop value
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

    // -1 means no delimeter, syntax error
    if (propValueDelimeter == -1) {
        return SYNTAX;
    }

    // Cut out prop value using delimeter position, append NUL term
    prop->value = malloc(strlen(buffCpy) - propValueDelimeter + 1);
    assert(prop->value != NULL);
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
    prop->name = malloc(curPos);
    assert(prop->name != NULL);
    for (int i = 0; i < curPos - 1; i++) {
        prop->name[i] = toupper(buffCpy[i]);
    }
    prop->name[curPos-1] = '\0';

    // Analyze the individual parameters to avoid errors
    // Go over the string, do case analysis for each ; found
    int lasPos = curPos;
    bool withinQuotes = false;
    for (int k = curPos; k < length; k++) {

        // If the next char is " invert withinQuotes
        if (buffCpy[k + 1] == '"') {
            withinQuotes = (!withinQuotes);
        }

        // If next char is ; and we arent in quotes, this is the end of the
        // current param block
        if ((buffCpy[k + 1] == ';' || buffCpy[k + 1] == '\0') &&
            withinQuotes == false) {
            CalParam *newParam = malloc(sizeof(CalParam));
            assert(newParam != NULL);
            newParam->name = NULL;
            newParam->next = NULL;
            newParam->nvalues = 0;

            // Copy param and value segment into a temporary string
            char param[BUF_LEN] = {0};
            for (int i = lasPos; i < k + 1; i++) {
                param[i - lasPos] = buffCpy[i];
            }
            param[strlen(param)] = '\0';

            // If we have no =, syntax failure
            if (strchr(param, '=') == NULL) {
                return SYNTAX;
            } else { // Split the parameter string into name and values
                char *paramName = strtok(param, "=");
                for (int i = 0; i < strlen(paramName); i++) {
                    if (isspace(paramName[i])) {
                        return SYNTAX;
                    }
                }
                char *paramValues = strtok(NULL, "\0");

                // Fixing strtok behavior with zero length strings
                if (paramValues == NULL) {
                    paramValues = "";
                }

                // Move name into new parameter
                newParam->name = malloc(strlen(paramName) + 1);
                assert(newParam->name != NULL);
                makeUpperCase(paramName);
                strcpy(newParam->name, paramName);

                // If we have commas, it's possible that we have multiple
                // values for the parameter
                if (strchr(paramValues, ',')) { // Do multi value analysis
                    bool valueInQuotes = false;
                    int length = strlen(paramValues);
                    int oldPos = 0;
                    for (int i = 0; i < length; i++) {
                        if (paramValues[i] == '"') {
                            valueInQuotes = (!valueInQuotes);
                        }

                        // Once you find end of a parameter value, add to array
                        if ((paramValues[i + 1] == ',' ||
                             paramValues[i + 1] == '\0') &&
                            valueInQuotes == false) {

                            // Move value into a temporary array
                            char tempValue[100] = {0};
                            for (int j = 0; j < (i - oldPos + 1); j++) {
                                tempValue[j] = paramValues[j + oldPos];
                            }
                            tempValue[i - oldPos + 1] = '\0';
                            makeUpperCase(tempValue);

                            // Allocate space in flexible member and copy
                            // over temp array
                            CalParam *temp =
                                realloc((newParam), (sizeof(*newParam) +
                                                     (newParam->nvalues + 1) *
                                                         sizeof(char *)));

                            if (temp != NULL) {
                                newParam = temp;
                            }

                            (newParam)->value[newParam->nvalues] =
                                malloc(strlen(tempValue) + 1);

                            assert((newParam)->value[newParam->nvalues] !=
                                   NULL);

                            strcpy((newParam)->value[newParam->nvalues],
                                   tempValue);

                            newParam->nvalues++;

                            if ((i + 2 <= strlen(paramValues)) &&
                                paramValues[i + 2] == '"') {
                                valueInQuotes = (!valueInQuotes);
                            }
                            i += 2; // Shift 2 because you're moving past
                                    // the comma
                            oldPos = i;
                        }
                    }
                } else { // No lone commas means only one value
                    (newParam) = realloc((newParam),
                                         (sizeof(*newParam) + sizeof(char *)));

                    ((newParam)->value[0]) = malloc(strlen(paramValues) + 1);

                    assert(((newParam)->value[0]) != NULL);

                    makeUpperCase(paramValues);
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
    return OK;
}

void freeCalComp(CalComp *const comp) {
    // Go along list of sub components, go to subcomponents if needed
    if (comp->name == NULL) {
        free(comp); // Only happens with NOCAL
        return;
    }

    for (int i = 0; i < comp->ncomps; i++) {
        if (comp->comp[i]->ncomps > 0) {
            freeCalComp(comp->comp[i]);
        }
        free(comp->comp[i]->name);

        // If a component has properties free them
        if (comp->comp[i]->nprops > 0) {
            freeCalProp(comp->comp[i]->prop);
        }

        free(comp->comp[i]);
    }
    // Free the root CalComp
    if (strcmp(comp->name, "VCALENDAR") == 0) {
        if (comp->prop) {
            freeCalProp(comp->prop);
        }
        free(comp->name);
        free(comp);
    }
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
