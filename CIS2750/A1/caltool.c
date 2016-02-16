#include "calutil.h"
#include "caltool.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
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
CalProp *unlinkProp(CalComp *comp2, char *type);
void countElements(char *temp, const CalComp *comp);
void findTimeRange(char *timeRange, const CalComp *comp);
int compareOrganizers(const void *org1, const void *org2);
time_t convertToTime_t(char arg[]);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    setenv("DATEMSK", "datemsk.txt", 1);
    CalComp *comp1 = NULL;
    CalComp *comp2 = NULL;
    CalStatus readStatus;

    readStatus = readCalFile(stdin, &comp1);
    // if readstatus error handle it
    if (readStatus.code != 0) {
        // print error code message
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-info") == 0) {
        if (argc != 2) {
            fprintf(stderr,
                    "Invalid command line arguments. Syntax: caltool -info\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
        calInfo(comp1, readStatus.lineto, stdout);
    } else if (strcmp(argv[1], "-extract") == 0) {
        CalOpt opt;
        if (argc != 3) {
            fprintf(stderr, "Invalid command line arguments. Syntax: caltool "
                            "-extract kind\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
        if (argv[2][0] == 'x') {
            opt = 1;
        } else if (argv[2][0] == 'e') {
            opt = 0;
        } else {
            fprintf(stderr, "Invalid command line arguments. Syntax: caltool "
                            "-extract kind\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
    } else if (strcmp(argv[1], "-filter") == 0) {
        CalOpt opt;
        if (argv[2][0] == 't') {
            opt = 2;
        } else if (argv[2][0] == 'e') {
            opt = 0;
        } else {
            fprintf(stderr, "Invalid command line arguments. Syntax: caltool "
                            "-filter content [from date] [to date]\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }

        if (argc == 3) {
            calFilter(comp1, opt, 0, 0, stdout);
        } else if (argc == 5) {
            time_t oneDate_t = convertToTime_t(argv[4]);
            if (oneDate_t == -404) {
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            if (strcmp(argv[3], "from") == 0) {
                calFilter(comp1, opt, oneDate_t, 0, stdout);
            } else if (strcmp(argv[3], "to") == 0) {
                calFilter(comp1, opt, 0, oneDate_t, stdout);
            } else {
                fprintf(stderr, "Invalid command line arguments. Syntax: "
                                "caltool -filter content [from date] [to "
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
                fprintf(stderr, "Invalid command line arguments. Syntax: "
                                "caltool -filter content [from date] [to "
                                "date]\n");
                freeCalComp(comp1);
                return EXIT_FAILURE;
            }
            calFilter(comp1, opt, fromDate_t, toDate_t, stdout);
        } else {
            fprintf(stderr, "Invalid command line arguments. Syntax: caltool "
                            "-filter content [from date] [to date]\n");
            freeCalComp(comp1);
            return EXIT_FAILURE;
        }
    } else if (strcmp(argv[1], "-combine") == 0) {
        FILE *ics = fopen(argv[2], "r");
        if (ics == NULL) {
            fprintf(stderr, "Problem opening file %s\n", argv[2]);
            return EXIT_FAILURE;
        }
        readStatus = readCalFile(ics, &comp2);

        fclose(ics);
        if (readStatus.code != 0) {
	        printf("%d\n",readStatus.code); 
            return EXIT_FAILURE;
        }

        calCombine(comp1, comp2, stdout);
        freeCalComp(comp2);
    } else {
        // Error
    }

    freeCalComp(comp1);
    return EXIT_SUCCESS;
}

time_t convertToTime_t(char arg[]) {
    time_t argDate_t;
    struct tm *lc = malloc(sizeof(*lc));
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
    if (ret != 0) {
        fprintf(stderr,
                "Problem with DATEMSK environment variable or template file");
        return -404;
    }
    lc->tm_sec = 0;
    lc->tm_min = 0;
    lc->tm_hour = 0;
    argDate_t = mktime(lc);
    free(lc);
    return argDate_t;
}

// Countelements and findtimerange require that temp is already allocated!
// Caller must free temp when done with its value
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

int findOrganizers(char **organizers, const CalComp *comp) {
    static int numOrganizers = 0;
    if (comp->nprops > 0) {
        CalProp *traverseProps = comp->prop;

        // Traverse properties
        while (traverseProps) {
            if (strcmp(traverseProps->name, "ORGANIZER") == 0) {
                CalParam *traverseParams = traverseProps->param;
                while (traverseParams) {
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

                char propTime[strlen(traverseProps->value) + 1];
                strcpy(propTime, traverseProps->value);
                if (propTime[strlen(traverseProps->value) - 1] == 'Z') {
                    propTime[strlen(traverseProps->value) - 1] = '\0';
                }

                // Remove the stupid T
                for (int i = 8; i < strlen(propTime); i++) {
                    propTime[i] = propTime[i + 1];
                }

                // Convert parsed date into struct tm
                // Should also check for error return

                strptime(propTime, "%Y%m%d%H%M%S", &propDate);

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

CalStatus calInfo(const CalComp *comp, int lines, FILE *const txtfile) {

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
    countElements(temp, comp);
    numSubcom = atoi(strtok(temp, ":")) - comp->ncomps;
    numProps = atoi(strtok(NULL, "\0"));
    free(temp);

    fprintf(txtfile, "%d line", lines);
    if (lines != 1) {
        fprintf(txtfile, "s");
    }
    fprintf(txtfile, "\n%d component", comp->ncomps);
    if (comp->ncomps != 1) {
        fprintf(txtfile, "s");
    }
    fprintf(txtfile, ": %d event", numEvents);
    if (numEvents != 1) {
        fprintf(txtfile, "s");
    }
    fprintf(txtfile, ", %d todo", numTodo);
    if (numTodo != 1) {
        fprintf(txtfile, "s");
    }
    fprintf(txtfile, ", %d other", (comp->ncomps - numEvents - numTodo));
    if ((comp->ncomps - numEvents - numTodo) != 1) {
        fprintf(txtfile, "s");
    }

    fprintf(txtfile, "\n%d subcomponent", numSubcom);
    if (numSubcom != 1) {
        fprintf(txtfile, "s");
    }
    fprintf(txtfile, "\n%d propert", numProps);
    if (numProps != 1) {
        fprintf(txtfile, "ies");
    } else {
        fprintf(txtfile, "y");
    }
    fprintf(txtfile, "\n");
    char *dateRange = malloc(100);
    findTimeRange(dateRange, comp);

    time_t lowDate = atoi(strtok(dateRange, ":"));
    time_t highDate = atoi(strtok(NULL, "\0"));
    free(dateRange);
    // strptime(propTime, "%Y%m%d%H%M%S", &propDate);
    if (lowDate == INT_MAX || highDate == INT_MIN) {
        fprintf(txtfile, "No dates\n");
    } else {
        struct tm *lc = localtime(&lowDate);
        char str[100];
        strftime(str, 99, "%Y-%b-%d", lc);
        fprintf(txtfile, "From %s ", str);
        lc = localtime(&highDate);
        strftime(str, 99, "%Y-%b-%d", lc);
        fprintf(txtfile, "to %s\n", str);
    }

    char **organizers = malloc(numProps * sizeof(char *));
    int numOrganizers = findOrganizers(organizers, comp);
    if (numOrganizers == 0) {
        fprintf(txtfile, "No organizers");
    } else {

        fprintf(txtfile, "Organizers:\n");

        qsort(organizers, numOrganizers, sizeof(char *), compareOrganizers);
        for (int i = 0; i < numOrganizers; i++) {
            if (i == 0 ||
                (i > 0 && strcmp(organizers[i], organizers[i - 1]) != 0)) {
                fprintf(txtfile, "%s\n", organizers[i]);
            }
        }
        free(organizers);
    }

    CalStatus ret;
    ret.linefrom = 0;
    ret.lineto = 0;
    ret.code = OK;
    return ret;
}

int compareOrganizers(const void *org1, const void *org2) {
    char **a = (char **)org1;
    char **b = (char **)org2;
    if (toupper(**a) <= toupper(**b)) {
        return -1;
    } else if (toupper(**a) > toupper(**b)) {
        return 1;
    } else {
        return 0;
    }
}

CalStatus calExtract(const CalComp *comp, CalOpt kind, FILE *const txtfile) {
    // return strcmp(*(char* const*) p1, *(char* const*) p2);
    // Use the above for qsort compare for x-props
    CalStatus ret;
    ret.linefrom = 0;
    ret.lineto = 0;
    ret.code = OK;
    return ret;
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

                        char propTime[strlen(traverseProps->value) + 1];
                        strcpy(propTime, traverseProps->value);
                        if (propTime[strlen(traverseProps->value) - 1] == 'Z') {
                            propTime[strlen(traverseProps->value) - 1] = '\0';
                        }

                        // Remove the stupid T
                        for (int i = 8; i < strlen(propTime); i++) {
                            propTime[i] = propTime[i + 1];
                        }

                        // Convert parsed date into struct tm
                        // Should also check for error return
                        strptime(propTime, "%Y%m%d%H%M%S", &propDate);

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

    writeCalComp(stdout, temp);

    free(temp);
    CalStatus ret;
    ret.linefrom = 0;
    ret.lineto = 0;
    ret.code = OK;
    return ret;
}

CalStatus calCombine(const CalComp *comp1, CalComp *comp2,
                     FILE *const icsfile) {

    CalComp *temp = malloc(sizeof(CalComp) +
                           sizeof(CalComp *) * (comp1->ncomps + comp2->ncomps));

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

    CalProp *unlinkedProdID = unlinkProp(comp2, "PRODID");
    CalProp *unlinkedVersion = unlinkProp(comp2, "VERSION");

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

CalProp *unlinkProp(CalComp *comp2, char *type) {
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
