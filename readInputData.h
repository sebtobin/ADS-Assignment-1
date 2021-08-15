/*
 *
 */

#ifndef COMP20003_ASS1_READCSVDATA_H
#define COMP20003_ASS1_READCSVDATA_H

#endif //COMP20003_ASS1_READCSVDATA_H

typedef struct {
    char *watchtowerID;
    char *postcode;
    int populationServed;
    char *watchtowerName;
    double longitude;
    double latitude;
} watchtowerData;

char **readTextData(FILE *text, char **stringArray);
watchtowerData **readWatchtowerStringArray(char **watchtowerStringArray, watchtowerData **watchtowerStructArray);
void printWatchtowerStruct(watchtowerData *watchtowerStruct);
void freeStringArray(char ***stringArray);
void freeWatchtowerStructArray(watchtowerData ***watchtowerStructArray);