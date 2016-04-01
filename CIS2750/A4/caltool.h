/********
caltool.h -- Public interface for iCalendar tools in caltool.c
 
Note to future programmers: This file conforms to the syntax used for the LLVM
project and this should be maintained with all future modifications to ensure
consistency.
 
Author: Brandon Chester : 0877477
Contact: bchester@mail.uoguelph.ca
Created: Feb 13, 2016
Last modified: Feb 22, 2016
********/

#ifndef CALTOOL_H
#define CALTOOL_H A2_RevA

#define _GNU_SOURCE     // for getdate_r
#define _XOPEN_SOURCE   // for strptime
#include <time.h>
#include <stdio.h>
#include "calutil.h"

/* Symbols used to send options to command execution modules */

typedef enum {
    OEVENT,     // events
    OPROP,      // properties
    OTODO,      // to-do items
} CalOpt;

/* iCalendar tool functions */

CalStatus calInfo( const CalComp *comp, int lines, FILE *const txtfile );
CalStatus calExtract( const CalComp *comp, CalOpt kind, FILE *const txtfile );
CalStatus calFilter( const CalComp *comp, CalOpt content, time_t datefrom, time_t dateto, FILE *const icsfile );
CalStatus calCombine( const CalComp *comp1, const CalComp *comp2, FILE *const icsfile );

#endif
