/*
 * header file for watchtower.c function prototypes and type definitions
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
} watchtower_t;

watchtower_t **storeWatchtowerStructs(char **watchtowerStringArray, watchtower_t **watchtowerStructArray);
void freeWatchtowerStructArray(watchtower_t ***watchtowerStructArray);

// DEBUG FUNCTION PROTOTYPES

void printWatchtowerStruct(watchtower_t *watchtowerStruct, FILE *file);