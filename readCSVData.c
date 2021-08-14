/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "readCSVData.h"

#define CELL_DELIM ','
#define BUFFER_SIZE 512
#define INIT_ARRAY_SIZE 16

char **readCSVData(FILE *watchtowerCSV, char **watchtowerStringArray) {

    int i;
    char lineBuffer[BUFFER_SIZE + 1];
    size_t arraySize = INIT_ARRAY_SIZE;

    watchtowerStringArray = (char**)malloc(sizeof(char*) * arraySize);
    assert(watchtowerStringArray);

    // dumping the csv header
    fgets(lineBuffer, BUFFER_SIZE + 1, watchtowerCSV);

    // storing each record in the csv as a string
    for (i=0; fgets(lineBuffer, BUFFER_SIZE + 1, watchtowerCSV) != NULL; i++) {
        if (i == arraySize) {
            arraySize *= 2;
            watchtowerStringArray = (char**)realloc(watchtowerStringArray, sizeof(char*) * arraySize);
        }
        watchtowerStringArray[i] = (char*)malloc(sizeof(char) * (strlen(lineBuffer) + 1));
        assert(watchtowerStringArray[i]);
        strcpy(watchtowerStringArray[i], lineBuffer);
    }
    watchtowerStringArray[i] = NULL;

    return watchtowerStringArray;
}

watchtowerData *readWatchtowerStringArray(char **watchtowerStringArray, watchtowerData *watchtowerStructArray) {

    int i, j;
    size_t arraySize = INIT_ARRAY_SIZE;

    watchtowerStructArray = (watchtowerData*)malloc(sizeof(watchtowerData) * arraySize);
    for (i=0; watchtowerStringArray != NULL; i++) {
        if (i == arraySize) {
            arraySize *= 2;
            watchtowerStructArray = (watchtowerData*)realloc(watchtowerStructArray, sizeof(watchtowerData) * arraySize);
        }
        j = 0;



    }

    return watchtowerStructArray;
}
