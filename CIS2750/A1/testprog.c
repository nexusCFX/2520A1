#include "calutil.h"
#include <stdlib.h>
#include <stdio.h>
int main() {
   // CalComp *comp;
    CalProp *prop = malloc(sizeof(*prop));
    CalError readStatus;
    FILE *file;
    file = fopen("test.txt", "r");
    readStatus = parseCalProp("NAMELIST;NAME=Bill,John:NA", prop);   
    
    printf("Name %s\n",prop->name);
    CalParam *travPar = prop->param;
    while (travPar) {
        printf("%s\n",travPar->name);
        printf("%d values\n",travPar->nvalues);
        for (int j = 0; j < travPar->nvalues; j++) {
            printf("%s\n",travPar->value[j]);
        }
        travPar = travPar->next;
    }
    printf("Value %s\n",prop->value);
      //   if (readStatus.code == OK) {
           //  freeCalComp(comp);
        // }
    fclose(file);

    return 1;
}
