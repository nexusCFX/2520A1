/************************
calutil.h
 
Public interface for iCalendar utility functions in calutil.c
 
Note to future programmers: This file conforms to the syntax used for the LLVM
project and this should be maintained with all future modifications to ensure
consistency.
 
Author: Brandon Chester : 0877477
Contact: bchester@mail.uoguelph.ca
Created: Jan 14, 2016
Last modified: Feb 22, 2016
*************************/

#ifndef CALUTIL_H
#define CALUTIL_H A2

#include <stdio.h>

#define FOLD_LEN 75     // fold lines longer than this length (RFC 5545 3.1)
#define VCAL_VER "2.0"  // version of standard accepted


/* data structures for ICS file in memory */

typedef struct CalParam CalParam;
typedef struct CalParam {    // property's parameter
    char *name;         // uppercase
    CalParam *next;     // linked list of parameters (ends with NULL)
    int nvalues;        // no. of values
    char *value[];      // uppercase or "..." (flexible array member)
} CalParam;

typedef struct CalProp CalProp;
typedef struct CalProp {    // (sub)component's property (=contentline)
    char *name;         // uppercase
    char *value;
    int nparams;        // no. of parameters
    CalParam *param;    // -> first parameter (or NULL)
    CalProp *next;      // linked list of properties (ends with NULL)
} CalProp;

typedef struct CalComp CalComp;
typedef struct CalComp {    // calendar's (sub)component
    char *name;         // uppercase
    int nprops;         // no. of properties
    CalProp *prop;      // -> first property (or NULL)
    int ncomps;         // no. of subcomponents
    CalComp *comp[];    // component pointers (flexible array member)
} CalComp;


/* General status return from functions */

typedef enum { OK=0,
    AFTEND,     // more text found after end of calendar 
    BADVER,     // version missing or wrong
    BEGEND,     // BEGIN...END not found as expected
    IOERR,      // I/O error
    NOCAL,      // outer block not VCALENDAR, or no V components found
    NOCRNL,     // CRNL missing at end of line
    NODATA,     // nothing between BEGIN...END
    NOPROD,     // PRODID missing
    SUBCOM,     // subcomponent not allowed
    SYNTAX,     // property not in valid form
} CalError;
    
typedef struct {
    CalError code;          // error code
    int linefrom, lineto;   // line numbers where error occurred
} CalStatus;    


/* File I/O functions */

CalStatus readCalFile( FILE *const ics, CalComp **const pcomp );
CalStatus readCalComp( FILE *const ics, CalComp **const pcomp );
CalStatus readCalLine( FILE *const ics, char **const pbuff );
CalError parseCalProp( char *const buff, CalProp *const prop );
CalStatus writeCalComp( FILE *const ics, const CalComp *comp );
void freeCalComp( CalComp *const comp );

#endif
