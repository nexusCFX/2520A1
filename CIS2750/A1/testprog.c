#include "calutil.h"

int main() {
    CalComp *comp;
    CalStatus readStatus;
    FILE *file;
    file = fopen("test.txt", "r");
    readStatus = readCalFile(file, &comp);   
    printf("Error #%d. Lines %d to %d\n", readStatus.code, readStatus.linefrom,
         readStatus.lineto);
         if (readStatus.code == OK) {
             freeCalComp(comp);
         }
    fclose(file);

    return 1;
}
