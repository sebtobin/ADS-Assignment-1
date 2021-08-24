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
    int isCategorised;
} watchtowerData_t;

watchtowerData_t **storeWatchtowerStructs(char **watchtowerStringArray, watchtowerData_t **watchtowerStructArray);
void printWatchtowerStruct(watchtowerData_t *watchtowerStruct, FILE *file);
void freeWatchtowerStructArray(watchtowerData_t ***watchtowerStructArray);
int numWatchtowers(watchtowerData_t **watchtowersArray);