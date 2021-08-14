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

char **readCSVData(FILE *watchtowerCSV, char **watchtowerStringArray);
watchtowerData *readWatchtowerStringArray(char **watchtowerStringArray, watchtowerData *watchtowerStructArray);