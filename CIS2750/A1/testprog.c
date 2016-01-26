CalStatus readCalLine( FILE *const ics, char **const pbuff ) {
    static int currentLine;
    static int difference;
    static char inputLine[500];
    CalStatus returnStatus;
    
    if (ics == NULL) {
        //do reset things
        puts("Reset successful");
        currentLine = 1;
        difference = 0;
        inputLine[0] = '\0';
        returnStatus = makeCalStatus(OK,0,0);
        return returnStatus;
    }
    
    if (difference == 0) {
        if (inputLine[0] == '\0') {
            puts("Allocating memory");
            *pbuff = malloc(500);
            fgets(inputLine, 500, ics);
            puts("Grabbing input line");
            while (checkEmptyString(inputLine) == true) {
                fgets(inputLine, 500, ics);
            }
        }
        
        //Put this FEOF check into a function, because you use it twice
        if (!feof(ics)) {
            if (!(inputLine[strlen(inputLine)-2] == '\r' && inputLine[strlen(inputLine)-1] == '\n')) {
                returnStatus = makeCalStatus(NOCRNL,currentLine,currentLine+difference);
                exit(0);
            }
        }
        //Copy buffer to temporary array and remove \r\n
        //Then put temp back into buffer
        inputLine[strlen(inputLine)-2] = '\0';
        
        strcpy(*pbuff,inputLine);
    }
    
    fgets(inputLine, 500, ics);
    
    while (inputLine[0] == ' ') {
        if (checkEmptyString(inputLine) == false) {
            //Put this FEOF check into a function, because you use it twice
            //Include the CRLF removal code too
            if (!feof(ics)) {
                if (!(inputLine[strlen(inputLine)-2] == '\r' && inputLine[strlen(inputLine)-1] == '\n')) {
                    returnStatus = makeCalStatus(NOCRNL,currentLine,currentLine+difference);
                    exit(0);
                }
            }
            //Copy buffer to temporary array and remove \r\n
            //Then put temp back into buffer
            char temp[strlen(*pbuff)];
            strcpy(temp,*pbuff);
            temp[strlen(temp)-2] = '\0';
            strcpy(*pbuff,temp);
            
            //Then shift every element in inputLine over by 1 to remove leading space
            inputLine[strlen(inputLine)-2] = '\0';
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
    
    difference = 0;
    int k = strlen(*pbuff);
    for (int i = 0; i < k; i++) {
        if((*pbuff)[i] == '\n') {
            puts("yes");
        } else {
            puts("no");
        }
    }
    
    //printf("%s\n",*pbuff);
    return returnStatus;
}

CalStatus readCalLine( FILE *const ics, char **const pbuff ) {
    static int currentLine;
    static int difference;
    static char inputLine[500];
    CalStatus returnStatus;
    
    if (ics == NULL) {
        //do reset things
        puts("Reset successful");
        currentLine = 1;
        difference = 0;
        inputLine[0] = '\0';
        returnStatus = makeCalStatus(OK,0,0);
        return returnStatus;
    }
    
    if (difference == 0) {
        if (inputLine[0] == '\0') {
            puts("Allocating memory");
            *pbuff = malloc(500);
            fgets(inputLine, 500, ics);
            puts("Grabbing input line");
            while (checkEmptyString(inputLine) == true) {
                fgets(inputLine, 500, ics);
            }
        }
        
        //Put this FEOF check into a function, because you use it twice
        if (!feof(ics)) {
            if (!(inputLine[strlen(inputLine)-2] == '\r' && inputLine[strlen(inputLine)-1] == '\n')) {
                returnStatus = makeCalStatus(NOCRNL,currentLine,currentLine+difference);
                exit(0);
            }
        }
        //Copy buffer to temporary array and remove \r\n
        //Then put temp back into buffer
        inputLine[strlen(inputLine)-2] = '\0';
        
        strcpy(*pbuff,inputLine);
    }
    
    fgets(inputLine, 500, ics);
    
    while (inputLine[0] == ' ') {
        if (checkEmptyString(inputLine) == false) {
            //Put this FEOF check into a function, because you use it twice
            //Include the CRLF removal code too
            if (!feof(ics)) {
                if (!(inputLine[strlen(inputLine)-2] == '\r' && inputLine[strlen(inputLine)-1] == '\n')) {
                    returnStatus = makeCalStatus(NOCRNL,currentLine,currentLine+difference);
                    exit(0);
                }
            }
            //Copy buffer to temporary array and remove \r\n
            //Then put temp back into buffer
            char temp[strlen(*pbuff)];
            strcpy(temp,*pbuff);
            temp[strlen(temp)-2] = '\0';
            strcpy(*pbuff,temp);
            
            //Then shift every element in inputLine over by 1 to remove leading space
            inputLine[strlen(inputLine)-2] = '\0';
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
    
    difference = 0;
    int k = strlen(*pbuff);
    for (int i = 0; i < k; i++) {
        if((*pbuff)[i] == '\n') {
            puts("yes");
        } else {
            puts("no");
        }
    }
    
    //printf("%s\n",*pbuff);
    return returnStatus;
}