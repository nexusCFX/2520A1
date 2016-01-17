#pragma once
#include <stdlib.h>
//Remove stdio if not needed once finished
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "calcutil.h"

void freeCalParam (CalParam *const param);
void freeCalProp (CalProp *const prop);
CalStatus makeCalStatus (CalError code, int linefrom, int lineto);

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
    readCallLine(NULL, NULL);
    //Potentially move this into another function
    //Initialize pcomp for use, call readcalLine with null ^^
    *pcomp = malloc(sizeof(CalComp));
    *pcomp->name = NULL;
    *pcomp->nprops = 0;
    *pcomp->prop = NULL;
    *pcomp->ncomps = 0;
    *pcomp->comp = NULL;
    
    readCalComp(const, *pcomp);
    //If error found at or after this point, call freecalcomp and return
    //otherwise
    
    //Go through components to do error checking
    //Probably best to put this in a function
    
    //Check version and product ID
    bool hasVersion = false;
    bool hasProdID = false;
    CalProp *traverseProps = *pcomp->prop;
    for (int i = 0; i < *pcomp->nprops; i++) {
        //Version checking
        if (strcmp(traverseProps->name,"VERSION") == 0) {
            //If there is already a version property, or if version != 2.0, BADVER
           if (hasVersion || strcmp(traverseProps->value,VCAL_VER) != 0) {
               return BADVER;
           } 
           hasVersion = true;
        }
        //PID checking
        if (strcmp(traverseProps->name,"PRODID") == 0) {
            //If it already exists, or if the value is null or empty
            if (traverseProps->value == NULL) {
                return NOPROD;
            }
            if (hasProdID || strcmp(traverseProps->value,"") == 0) {
                return NOPROD;
            }
            hasProdID = true;
        }
        traverseProps = prop->next;
    }
    
    //Check components
    bool startWithV = false;
    for (int i = 0; i < *pcomp->ncomps; i++) {
        if (*pcomp->comp[i]->name[0] == 'V') {
            startWithV = true;
        }    
    }
    //If none start with V, error
    if(!startWithV) {
        return NOCAL;
    }
    //Check EOF. If there are lines after END:VCALENDAR, error
    if (readCalLine() != EOF) {
        return AFTEND;
    }
    return OK;
}

CalStatus readCalComp( FILE *const ics, CalComp *const comp ) {
    
    return NULL;
}


CalStatus readCalLine( FILE *const ics, char **const pbuff ) {
    if (ics == NULL){
        //do reset things
        return makeCalStatus(OK,0,0);
    }
    return NULL;
}

CalError parseCalProp( char *const buff, CalProp *const prop ) {
    
    return NULL;
}
//Free params of props, free props of subcomps, free subcomps of comps, free comp
void freeCalComp( CalComp *const comp ) {
    
    //Recursively visit all components
    if (comp->comp) {
        freeCalComp(comp->comp);
    }
    
    //Go along list of sub components, moving to deeper sub components if needed
    //Can subcomponents have subcomponents? Doesn't hurt to check I guess
    for (int i = 0; i < comp->ncomps; i++) {
        
        //If a sub component has a sub component [inception]
        if (comp->comp[i]->comp) {
            freeCalComp(comp->comp[i]->comp);
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
    CalProp traverseProps = prop;
    CalProp nextProp;
            
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
    CalParam traverseParams = param;
    CalParam nextParam;
    
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


