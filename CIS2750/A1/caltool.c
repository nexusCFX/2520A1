#include "calutil.h"
#include "caltool.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
/************************
caltool.c
Implementation of FILL THIS BLANK
Note to future programmers: This file conforms to the syntax used for the LLVM
project and this should be maintained with all future modifications to ensure
consistency.

Author: Brandon Chester : 0877477
Contact: bchester@mail.uoguelph.ca
Created: Feb 13, 2016
Last modified: Feb 14, 2016
*************************/

int getdate_r(const char *string, struct tm *tp);

/*************************
unlinkProp
Internal function which unlinks properties from a CalProp struct
Intended use is in conjunction with calCombine to remove version and prodid
Arguments:
comp2: The comp2 struct from the calCombine function
type: A pointer to a character string. Should be "VERSION" or "PRODID"
Return Value: A pointer to the unlinked property for re-linking later
*************************/
CalProp *unlinkProp(char *type, CalComp *comp2);

/*************************
countElements
Internal function which counts the number of subcomponents and properties in a CalComp structure
Intended use is with calInfo function
Arguments:
temp: A pointer to a character string which has already been allocated memory. Temp will contain the number of properties and subcomponents in comp separated by a colon.
comp: The comp struct from the calInfo function
Return Value: N/A
Note: The calling function is responsible for allocating enough memory 
*************************/
void countElements(char *temp, const CalComp *comp);

/*************************
findTimeRange
Internal function which unlinks properties from a CalProp struct
Intended use is in conjunction with calCombine to remove version and prodid
Arguments:
comp2: The comp2 struct from the calCombine function
type: A pointer to a character string. Should be "VERSION" or "PRODID"
Return Value: A pointer to the unlinked property for re-linking later
*************************/
void findTimeRange(char *timeRange, const CalComp *comp);

/*************************
compareOrganizers
Internal function which compares two organizers by the first character, case insensitive
Used for comparing items with qsort function to sort organizers "alphabetically"
Arguments:
org1: A void pointer to a pointer to a character string 
org2: A void pointer to a pointer to a character string 
Return Value: -1 if org1 precedes org2, 0 if they are equal, 1 if org2 precedes org1
*************************/
int compareOrganizers(const void *org1, const void *org2);

/*************************
findOrganizers
Internal function which finds the values of all organizers properties in a calendar
Intended use is in conjunction with calInfo to remove version and prodid
Arguments:
organizers: An array of pointers to characters. Must be allocated and freed by the caller.
comp: The comp struct from the calInfo function
Return Value: The number of organizers found in the calendar
*************************/
int findOrganizers(char **organizers, const CalComp *comp);

/*************************
compareEvents
Internal function which compares two events based on the DTSTART property
Used for comparing items with qsort function to sort organizers based on time
Arguments:
org1: A void pointer to a pointer to a CalProp structure
org2: A void pointer to a pointer to a CalProp structure
Return Value: -1 if org1's DTSTART precedes org2, 0 if they are equal, 1 if org2's DTSTART precedes org1
*************************/
int compareEvents(const void *org1, const void *org2);

/*************************
compareXProps
Internal function which compares two X-props alphabetically
Used for comparing X-props with qsort function
Arguments:
org1: A void pointer to a pointer to a character string 
org2: A void pointer to a pointer to a character string 
Return Value: -1 if org1 precedes org2, 0 if they are equal, 1 if org2 precedes org1
*************************/
int compareXProps(const void *prop1, const void *prop2);
CalError extractXProps(char **xProps, const CalComp *comp);
CalStatus makeCalStatus(CalError code, int linefrom, int lineto);
time_t convertToTime_t(char arg[]);

int main(int argc, char *argv[]) {
    const char *calErrors[] = {"OK", "AFTEND", "BADVER", "BEGEND", "IOERR",
                               "NOCAL",  "NOCRNL", "NODATA", "NOPROD",
                               "SUBCOM", "SYNTAX"};
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    setenv("DATEMSK", "datemsk.txt", 1);
    CalComp *comp1 = NULL;
    CalComp *comp2 = NULL;
    CalStatus readStatus;

    readStatus = readCalFile(stdin, &comp1);

    if (readStatus.code != 0) {
        fprintf(stderr, "Calendar error: %s\n", calErrors[readStatus.code]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-info") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Invalid arguments:\nUsage: caltool -info\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
        readStatus = calInfo(comp1, readStatus.lineto, stdout);
    } else if (strcmp(argv[1], "-extract") == 0) {
        CalOpt opt;
        if (argc != 3) {
            fprintf(stderr, "Invalid arguments:\nUsage: caltool "
                            "-extract [ex]\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }

        // Set correct CalOpt type
        if (argv[2][0] == 'x') {
            opt = 1;
        } else if (argv[2][0] == 'e') {
            opt = 0;
        } else {
            fprintf(stderr, "Invalid arguments:\nUsage: caltool "
                            "-extract [ex]\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
        readStatus = calExtract(comp1, opt, stdout);
    } else if (strcmp(argv[1], "-filter") == 0) {
        CalOpt opt;

        // Set correct CalOpt type
        if (argv[2][0] == 't') {
            opt = 2;
        } else if (argv[2][0] == 'e') {
            opt = 0;
        } else {
            fprintf(stderr, "Invalid arguments:\nUsage: caltool "
                            "-filter [et] [from date] [to date]\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }

        if (argc == 3) {
            readStatus = calFilter(comp1, opt, 0, 0, stdout);
        } else if (argc == 5) {
            time_t oneDate_t = convertToTime_t(argv[4]);
            if (oneDate_t == -404) {
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            if (strcmp(argv[3], "from") == 0) {
                readStatus = calFilter(comp1, opt, oneDate_t, 0, stdout);
            } else if (strcmp(argv[3], "to") == 0) {
                readStatus = calFilter(comp1, opt, 0, oneDate_t, stdout);
            } else {
                fprintf(stderr, "Invalid arguments:\nUsage: "
                                "caltool -filter [et] [from date] [to "
                                "date]\n");
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
        } else if (argc == 7) {
            time_t fromDate_t = convertToTime_t(argv[4]);
            if (fromDate_t == -404) {
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            time_t toDate_t = convertToTime_t(argv[6]);
            if (toDate_t == -404) {
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            if (strcmp(argv[3], "from") != 0 || strcmp(argv[5], "to") != 0) {
                fprintf(stderr, "Invalid arguments:\nUsage: "
                                "caltool -filter [et] [from date] [to "
                                "date]\n");
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            if (fromDate_t > toDate_t) {
                fprintf(stderr, "From date must occur earlier than to date\n");
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            readStatus = calFilter(comp1, opt, fromDate_t, toDate_t, stdout);
        } else {
            fprintf(stderr, "Invalid arguments:\nUsage: caltool "
                            "-filter [et] [from date] [to date]\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
    } else if (strcmp(argv[1], "-combine") == 0) {
        FILE *ics = fopen(argv[2], "r");
        if (ics == NULL) {
            fprintf(stderr, "Problem opening file %s\n", argv[2]);
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
        readStatus = readCalFile(ics, &comp2);

        fclose(ics);
        if (readStatus.code != 0) {
            fprintf(stderr, "Calendar error: %s with file %s\n",
                    calErrors[readStatus.code], argv[2]);
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }

        calCombine(comp1, comp2, stdout);
        freeCalComp(comp2);
    } else {
        fprintf(stderr, "Command not found\n");
        freeCalComp(comp1);
        return EXIT_FAILURE;
    }
    
        printf("Error %s, printed %d lines\n", calErrors[readStatus.code],
           readStatus.lineto);
    
    freeCalComp(comp1);
    return EXIT_SUCCESS;
}

CalStatus calInfo(const CalComp *comp, int lines, FILE *const txtfile) {
    int linesPrinted = 0;
    int numEvents = 0;
    int numTodo = 0;
    int numSubcom;
    int numProps;

    for (int i = 0; i < comp->ncomps; i++) {
        if (strcmp(comp->comp[i]->name, "VEVENT") == 0) {
            numEvents++;
        } else if (strcmp(comp->comp[i]->name, "VTODO") == 0) {
            numTodo++;
        }
    }
    char *temp = malloc(100);
    assert(temp != NULL);
    countElements(temp, comp);
    numSubcom = atoi(strtok(temp, ":")) - comp->ncomps;
    numProps = atoi(strtok(NULL, "\0"));
    free(temp);

    if (fprintf(txtfile, "%d line", lines) < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    if (lines != 1) {
        if (fprintf(txtfile, "s") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }

    if (fprintf(txtfile, "\n%d component", comp->ncomps) < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    linesPrinted++;
    if (comp->ncomps != 1) {

        if (fprintf(txtfile, "s") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }
    if (fprintf(txtfile, ": %d event", numEvents) < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    if (numEvents != 1) {
        if (fprintf(txtfile, "s") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }
    if (fprintf(txtfile, ", %d todo", numTodo) < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    if (numTodo != 1) {
        if (fprintf(txtfile, "s") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }
    if (fprintf(txtfile, ", %d other", (comp->ncomps - numEvents - numTodo)) <
        0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    if ((comp->ncomps - numEvents - numTodo) != 1) {
        if (fprintf(txtfile, "s") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }

    if (fprintf(txtfile, "\n%d subcomponent", numSubcom) < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    linesPrinted++;
    if (numSubcom != 1) {
        if (fprintf(txtfile, "s") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }
    if (fprintf(txtfile, "\n%d propert", numProps) < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    linesPrinted++;
    if (numProps != 1) {
        if (fprintf(txtfile, "ies") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    } else {
        if (fprintf(txtfile, "y") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }
    if (fprintf(txtfile, "\n") < 0) {
        return makeCalStatus(IOERR, linesPrinted, linesPrinted);
    }
    linesPrinted++;
    char *dateRange = malloc(100);
    assert(dateRange != NULL);
    findTimeRange(dateRange, comp);

    time_t lowDate = atoi(strtok(dateRange, ":"));
    time_t highDate = atoi(strtok(NULL, "\0"));
    free(dateRange);
    // strptime(propTime, "%Y%m%d%H%M%S", &propDate);
    if (lowDate == INT_MAX || highDate == INT_MIN) {
        if (fprintf(txtfile, "No dates\n") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    } else {
        struct tm *lc = localtime(&lowDate);
        char str[100];
        strftime(str, 99, "%Y-%b-%d", lc);
        if (fprintf(txtfile, "From %s ", str) < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
        lc = localtime(&highDate);
        strftime(str, 99, "%Y-%b-%d", lc);
        if (fprintf(txtfile, "to %s\n", str) < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
    }
    linesPrinted++;

    char **organizers = malloc(numProps * sizeof(char *));
    assert(organizers != NULL);
    int numOrganizers = findOrganizers(organizers, comp);
    if (numOrganizers == 0) {
        if (fprintf(txtfile, "No organizers\n") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
        linesPrinted++;
    } else {

        if (fprintf(txtfile, "Organizers:\n") < 0) {
            return makeCalStatus(IOERR, linesPrinted, linesPrinted);
        }
        linesPrinted++;
        qsort(organizers, numOrganizers, sizeof(char *), compareOrganizers);
        for (int i = 0; i < numOrganizers; i++) {
            if (i == 0 ||
                (i > 0 && strcmp(organizers[i], organizers[i - 1]) != 0)) {
                if (fprintf(txtfile, "%s\n", organizers[i]) < 0) {
                    return makeCalStatus(IOERR, linesPrinted, linesPrinted);
                }
                linesPrinted++;
            }
        }
        free(organizers);
    }
    return makeCalStatus(OK, linesPrinted, linesPrinted);
}

CalStatus calExtract(const CalComp *comp, CalOpt kind, FILE *const txtfile) {
    int linesPrinted = 0;
    if (kind == 0) {
        char str[100];
        char *val;
        bool foundSummary = false;
        bool foundTime = false;
        int numEvents = 0;
        CalComp **events = malloc(sizeof(CalComp *) * comp->ncomps);
        assert(events != NULL);
        for (int i = 0; i < comp->ncomps; i++) {
            if (strcmp(comp->comp[i]->name, "VEVENT") == 0) {
                events[numEvents] = comp->comp[i];
                numEvents++;
            }
        }

        qsort(events, numEvents, sizeof(events), compareEvents);

        for (int i = 0; i < numEvents; i++) {
            CalProp *traverseProps = events[i]->prop;
            while (traverseProps && (!foundSummary || !foundTime)) {
                if (strcmp("DTSTART", traverseProps->name) == 0) {
                    foundTime = true;
                    char propTime[9];
                    strncpy(propTime, traverseProps->value, 8);
                    propTime[8] = '\0';
                    struct tm propDate = {0};
                    strptime(propTime, "%Y%m%d", &propDate);
                    strftime(str, 99, "%Y-%b-%d", &propDate);
                } else if (strcmp("SUMMARY", traverseProps->name) == 0) {
                    val = malloc(strlen(traverseProps->value) + 1);
                    assert(val != NULL);
                    strcpy(val, traverseProps->value);
                    foundSummary = true;
                }
                traverseProps = traverseProps->next;
            }
            if (fprintf(txtfile, "%s: %s\n", str, val) < 0) {
                free(events);
                return makeCalStatus(IOERR, linesPrinted, linesPrinted);
            }
            linesPrinted++;
            free(val);
            foundSummary = false;
            foundTime = false;
        }
        free(events);
    } else {
        char **xProps = malloc(2000 * sizeof(char *));
        assert(xProps != NULL);
        int numXProps = extractXProps(xProps, comp);
        qsort(xProps, numXProps, sizeof(char *), compareXProps);
        for (int i = 0; i < numXProps; i++) {
            if (i == 0 || (i > 0 && strcmp(xProps[i], xProps[i - 1]) != 0)) {
                if (fprintf(txtfile, "%s\n", xProps[i]) < 0) {
                    free(xProps);
                    return makeCalStatus(IOERR, linesPrinted, linesPrinted);
                }
                linesPrinted++;
            }
        }
        free(xProps);
    }
    return makeCalStatus(OK, linesPrinted, linesPrinted);
}

CalStatus calFilter(const CalComp *comp, CalOpt content, time_t datefrom,
                    time_t dateto, FILE *const icsfile) {
    struct tm propDate = {0};
    char filterType[7];
    if (content == OEVENT) {
        strcpy(filterType, "VEVENT");
    } else if (content == OTODO) {
        strcpy(filterType, "VTODO");
    }

    // Shallow copy
    CalComp *temp =
        malloc(sizeof(CalComp) + sizeof(CalComp *) * (comp->ncomps));
    assert(temp != NULL);

    temp->name = comp->name;
    temp->nprops = comp->nprops;
    temp->prop = comp->prop;
    temp->ncomps = 0;

    for (int i = 0; i < comp->ncomps; i++) {
        if (strcmp(comp->comp[i]->name, filterType) == 0) {
            // Do date check
            if (datefrom != 0 || dateto != 0) {
                CalProp *traverseProps = comp->comp[i]->prop;
                while (traverseProps) {
                    // If it matches one we're looking for
                    if (strcmp(traverseProps->name, "COMPLETED") == 0 ||
                        strcmp(traverseProps->name, "DTEND") == 0 ||
                        strcmp(traverseProps->name, "DUE") == 0 ||
                        strcmp(traverseProps->name, "DTSTART") == 0 ||
                        strcmp(traverseProps->name, "CREATED") == 0 ||
                        strcmp(traverseProps->name, "DTSTAMP") == 0 ||
                        strcmp(traverseProps->name, "LAST-MODIFIED") == 0) {
                            
                            
                        char propTime[9];
                        strncpy(propTime, traverseProps->value, 8);
                        propTime[8] = '\0';
                        strptime(propTime, "%Y%m%d", &propDate);
                        time_t propDate_t = mktime(&propDate);

                        if (datefrom == 0 && dateto != 0) {
                            if (propDate_t <= dateto) {
                                temp->comp[temp->ncomps] = comp->comp[i];
                                temp->ncomps++;
                                break;
                            }
                        } else if (datefrom != 0 && dateto == 0) {
                            if (propDate_t >= datefrom) {

                                temp->comp[temp->ncomps] = comp->comp[i];
                                temp->ncomps++;
                                break;
                            }
                        } else if (datefrom != 0 && dateto != 0) {
                            if (propDate_t <= dateto &&
                                propDate_t >= datefrom) {
                                temp->comp[temp->ncomps] = comp->comp[i];
                                temp->ncomps++;
                                break;
                            }
                        }
                    }
                    traverseProps = traverseProps->next;
                }
            } else {
                temp->comp[temp->ncomps] = comp->comp[i];
                temp->ncomps++;
            }
        }
    }

    if (temp->ncomps == 0) {
        free(temp);
        return makeCalStatus(NOCAL, 0, 0);
    }
    CalStatus ret = writeCalComp(stdout, temp);

    free(temp);
    return ret;
}

CalStatus calCombine(const CalComp *comp1, CalComp *comp2,
                     FILE *const icsfile) {

    CalComp *temp = malloc(sizeof(CalComp) +
                           sizeof(CalComp *) * (comp1->ncomps + comp2->ncomps));
    assert(temp != NULL);

    temp->name = comp1->name;
    temp->nprops = comp1->nprops;
    temp->prop = comp1->prop;
    temp->ncomps = comp1->ncomps;

    for (int i = 0; i < comp1->ncomps; i++) {
        temp->comp[i] = comp1->comp[i];
    }

    CalProp *traverseProps = temp->prop;

    while (traverseProps->next != NULL) {
        traverseProps = traverseProps->next;
    }

    CalProp *unlinkedProdID = unlinkProp("PRODID", comp2);
    CalProp *unlinkedVersion = unlinkProp("VERSION", comp2);

    traverseProps->next = comp2->prop;

    for (int i = 0; i < comp2->ncomps; i++) {
        temp->comp[temp->ncomps + i] = comp2->comp[i];
    }
    temp->ncomps = (temp->ncomps) + (comp2->ncomps);

    writeCalComp(icsfile, temp);

    traverseProps->next = NULL;
    traverseProps = comp2->prop;
    if (traverseProps == NULL) {
        traverseProps = unlinkedProdID;
    } else {
        while (traverseProps->next != NULL) {
            traverseProps = traverseProps->next;
        }

        traverseProps->next = unlinkedProdID;
    }

    unlinkedProdID->next = unlinkedVersion;
    unlinkedVersion->next = NULL;
    free(temp);

    CalStatus ret;
    ret.linefrom = 0;
    ret.lineto = 0;
    ret.code = OK;
    return ret;
}

/*Helper functions ordered alphabetically*/

int compareEvents(const void *comp1, const void *comp2) {
    CalComp *a = *(CalComp **)comp1;
    CalComp *b = *(CalComp **)comp2;
    char time[9];
    time_t timeA = 0;
    time_t timeB = 0;
    CalProp *traverseProps = a->prop;
    struct tm propDate = {0};
    while (traverseProps) {
        if (strcmp("DTSTART", traverseProps->name) == 0) {
                    strncpy(time, traverseProps->value, 8);
                    time[8] = '\0';
            strptime(time, "%Y%m%d", &propDate);
            timeA = mktime(&propDate);
            break;
        }
        traverseProps = traverseProps->next;
    }

    traverseProps = b->prop;
    while (traverseProps) {
        if (strcmp("DTSTART", traverseProps->name) == 0) {
            strncpy(time, traverseProps->value, 8);
                    time[8] = '\0';
            strptime(time, "%Y%m%d", &propDate);
            timeB = mktime(&propDate);
            break;
        }
        traverseProps = traverseProps->next;
    }

    if (timeA < timeB) {
        return -1;
    } else if (timeA == timeB) {
        return 0;
    } else {
        return 1;
    }
}

int compareOrganizers(const void *org1, const void *org2) {
    char *a = *(char **)org1;
    char *b = *(char **)org2;
    if (toupper(a[0]) <= toupper(b[0])) {
        return -1;
    } else if (toupper(a[0]) > toupper(b[0])) {
        return 1;
    } else {
        return 0;
    }
    return 0;
}

int compareXProps(const void *prop1, const void *prop2) {
    return strcmp(*(char *const *)prop1, *(char *const *)prop2);
}

time_t convertToTime_t(char arg[]) {
    time_t argDate_t;
    struct tm *lc = malloc(sizeof(*lc));
    assert(lc != NULL);
    if (strcmp(arg, "today") == 0) {
        free(lc);
        
        argDate_t = time(NULL);
        lc = localtime(&argDate_t);
        argDate_t = mktime(lc);
        lc->tm_sec = 0;
        lc->tm_min = 0;
        lc->tm_hour = 0;
        return argDate_t;
    }
    int ret = getdate_r(arg, lc);
    assert(ret != 6);
    if (ret != 0) {
        if (ret < 7) {
            fprintf(
                stderr,
                "Problem with DATEMSK environment variable or template file\n");
        } else {
            fprintf(stderr, "Date \" %s \" could not be interpreted\n", arg);
        }
        return -404;
    }
    lc->tm_sec = 0;
    lc->tm_min = 0;
    lc->tm_hour = 0;
    argDate_t = mktime(lc);
    free(lc);
    return argDate_t;
}

void countElements(char *temp, const CalComp *comp) {
    static int subComps = 0;
    static int props = 0;
    char ret[100];
    subComps += comp->ncomps;
    props += comp->nprops;
    for (int i = 0; i < comp->ncomps; i++) {
        countElements(temp, comp->comp[i]);
    }
    sprintf(ret, "%d:%d", subComps, props);
    strcpy(temp, ret);
}

CalError extractXProps(char **xProps, const CalComp *comp) {
    static int numXProps = 0;
    if (comp->nprops > 0) {
        CalProp *traverseProps = comp->prop;
        // Traverse properties
        while (traverseProps) {
            if (traverseProps->name[0] == 'X' &&
                traverseProps->name[1] == '-') {
                if ((numXProps % 2000) == 0 && numXProps != 0) {
                    xProps = realloc(xProps, (numXProps * 2) * sizeof(char *));
                    assert(xProps != NULL);
                }
                xProps[numXProps] = traverseProps->name;
                numXProps++;
            }
            traverseProps = traverseProps->next;
        }
    }
    for (int i = 0; i < comp->ncomps; i++) {
        extractXProps(xProps, comp->comp[i]);
    }
    return numXProps;
}

int findOrganizers(char **organizers, const CalComp *comp) {
    static int numOrganizers = 0;
    if (comp->nprops > 0) {
        CalProp *traverseProps = comp->prop;
        // Traverse properties
        while (traverseProps) {
            //If we have organizer, point a pointer at the value strings
            if (strcmp(traverseProps->name, "ORGANIZER") == 0) {
                CalParam *traverseParams = traverseProps->param;
                while (traverseParams) {
                    //While I don't believe it will have multiple values it doesn't hurt to check
                    if (strcmp(traverseParams->name, "CN") == 0) {
                        for (int i = 0; i < traverseParams->nvalues; i++) {
                            organizers[numOrganizers] =
                                traverseParams->value[i];
                            numOrganizers++;
                        }
                    }
                    break;
                }
            }
            traverseProps = traverseProps->next;
        }
    }
    for (int i = 0; i < comp->ncomps; i++) {
        findOrganizers(organizers, comp->comp[i]);
    }
    return numOrganizers;
}

void findTimeRange(char *timeRange, const CalComp *comp) {
    static time_t lowestTime = INT_MAX;
    static time_t highestTime = INT_MIN;
    char ret[100];
    struct tm propDate = {0};
    if (comp->nprops > 0) {

        CalProp *traverseProps = comp->prop;

        // Traverse properties
        while (traverseProps) {
            if (strcmp(traverseProps->name, "COMPLETED") == 0 ||
                strcmp(traverseProps->name, "DTEND") == 0 ||
                strcmp(traverseProps->name, "DUE") == 0 ||
                strcmp(traverseProps->name, "DTSTART") == 0 ||
                strcmp(traverseProps->name, "CREATED") == 0 ||
                strcmp(traverseProps->name, "DTSTAMP") == 0 ||
                strcmp(traverseProps->name, "LAST-MODIFIED") == 0) {

                char time[9];
                
                strncpy(time, traverseProps->value, 8);
                time[8] = '\0';
                strptime(time, "%Y%m%d", &propDate);

                time_t propDate_t = mktime(&propDate);

                if (propDate_t > highestTime) {
                    highestTime = propDate_t;
                }
                if (propDate_t < lowestTime) {
                    lowestTime = propDate_t;
                }
            }
            traverseProps = traverseProps->next;
        }
    }
    for (int i = 0; i < comp->ncomps; i++) {
        findTimeRange(timeRange, comp->comp[i]);
    }
    sprintf(ret, "%ld:%ld", lowestTime, highestTime);
    strcpy(timeRange, ret);
}

CalProp *unlinkProp(char *type, CalComp *comp2) {
    CalProp *prev = NULL;
    CalProp *curr = NULL;
    CalProp *unlinked = NULL;

    curr = comp2->prop;
    while (strcmp(curr->name, type) != 0) {
        prev = curr;
        curr = curr->next;
    }
    unlinked = curr;

    if (prev == NULL) {
        comp2->prop = curr->next;
    } else {
        prev->next = curr->next;
    }
    return unlinked;
}
