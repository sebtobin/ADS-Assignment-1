/*
 *
 */

#ifndef COMP20003_ASS1_WATCHTOWEROPS_H
#define COMP20003_ASS1_WATCHTOWEROPS_H

#endif //COMP20003_ASS1_WATCHTOWEROPS_H

typedef struct {
    char *watchtowerID;
    char *postcode;
    int populationServed;
    char *watchtowerName;
    double longitude;
    double latitude;
} watchtowerData_t;

watchtowerData_t **readWatchtowerStringArray(char **watchtowerStringArray, watchtowerData_t **watchtowerStructArray);
void printWatchtowerStruct(watchtowerData_t *watchtowerStruct);
void freeWatchtowerStructArray(watchtowerData_t ***watchtowerStructArray);