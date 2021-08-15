/*
 * file containing functions for reading CSV with watchtower data and
 * storing in an array of watchtower data structs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "readInputData.h"

#define CELL_DELIM ","
#define LINE_BUFFER_SIZE 512
#define INIT_ARRAY_SIZE 16

char**
readTextData(FILE *text, char **stringArray) {

    int i;
    char lineBuffer[LINE_BUFFER_SIZE + 1];
    size_t arraySize = INIT_ARRAY_SIZE;

    // allocate memory for array of strings
    stringArray = (char**)malloc(sizeof(char*) * arraySize);
    assert(stringArray);

    // storing each line in the text as a string
    for (i=0; fgets(lineBuffer, LINE_BUFFER_SIZE + 1, text) != NULL; i++) {

        // allocate more memory if needed
        if (i == arraySize - 1) {
            arraySize *= 2;
            stringArray = (char**)realloc(stringArray, sizeof(char*) * arraySize);
        }

        // dynamically allocate memory for each string and store in array
        stringArray[i] = (char*)malloc(sizeof(char) * (strlen(lineBuffer) + 1));
        assert(stringArray[i]);
        strcpy(stringArray[i], lineBuffer);
    }

    // set NULL pointer to signify end of array
    stringArray[i] = NULL;

    return stringArray;
}

watchtowerData**
readWatchtowerStringArray(char **watchtowerStringArray, watchtowerData **watchtowerStructArray) {

    int i;
    char *cellBuffer;
    size_t arraySize = INIT_ARRAY_SIZE;

    // allocate memory for array of pointers to csv records as structs
    watchtowerStructArray = (watchtowerData**)malloc(sizeof(watchtowerData*) * arraySize);
    assert(watchtowerStructArray);

    // if string array is not empty, skip CSV header string
    // increment limited to local scope, so full string array can be accessed and freed later
    if (watchtowerStringArray[0] != NULL) {
        watchtowerStringArray++;
    }

    // storing each record as a struct, ignoring the CSV header string
    for (i=0; watchtowerStringArray[i] != NULL; i++) {

        // allocate more memory if needed to array of struct pointers
        if (i == arraySize) {
            arraySize *= 2;
            watchtowerStructArray = (watchtowerData**)realloc(watchtowerStructArray, sizeof(watchtowerData*) * arraySize);
        }

        // allocate memory to current watchtower struct pointer
        watchtowerStructArray[i] = (watchtowerData*)malloc(sizeof(watchtowerData));
        assert(watchtowerStructArray[i]);

        /*
         * hardcode storing of each cell into corresponding struct variables;
         * seems as though some level of hard coding is required to store cell
         * data into the struct, without significantly more complex code such as
         * a 2D matrix and array of dictionaries with constant keys
        */

        cellBuffer = strtok(watchtowerStringArray[i], CELL_DELIM);
        watchtowerStructArray[i]->watchtowerID = (char*)malloc(sizeof(char) * (strlen(cellBuffer) + 1));
        assert(watchtowerStructArray[i]->watchtowerID);
        strcpy(watchtowerStructArray[i]->watchtowerID, cellBuffer);

        cellBuffer = strtok(NULL, CELL_DELIM);
        watchtowerStructArray[i]->postcode = (char*)malloc(sizeof(char) * (strlen(cellBuffer) + 1));
        assert(watchtowerStructArray[i]->postcode);
        strcpy(watchtowerStructArray[i]->postcode, cellBuffer);

        cellBuffer = strtok(NULL, CELL_DELIM);
        watchtowerStructArray[i]->populationServed = atoi(cellBuffer);

        cellBuffer = strtok(NULL, CELL_DELIM);
        watchtowerStructArray[i]->watchtowerName = (char*)malloc(sizeof(char) * (strlen(cellBuffer) + 1));
        assert(watchtowerStructArray[i]->watchtowerName);
        strcpy(watchtowerStructArray[i]->watchtowerName, cellBuffer);

        cellBuffer = strtok(NULL, CELL_DELIM);
        watchtowerStructArray[i]->longitude = strtod(cellBuffer, NULL);

        cellBuffer = strtok(NULL, CELL_DELIM);
        watchtowerStructArray[i]->latitude = strtod(cellBuffer, NULL);
    }

    // add null pointer to signify end of array
    watchtowerStructArray[i] = NULL;

    return watchtowerStructArray;
}

void
printWatchtowerStruct(watchtowerData *watchtowerStruct) {
    printf("%s\n%s\n%d\n%s\n%lf\n%lf\n\n", watchtowerStruct->watchtowerID
           , watchtowerStruct->postcode, watchtowerStruct->populationServed
           , watchtowerStruct->watchtowerName, watchtowerStruct->longitude
           , watchtowerStruct->latitude);
}

void
freeStringArray(char ***stringArray) {

    int i;

    // free each string in array
    for (i=0; (*stringArray)[i] != NULL; i++) {
        free((*stringArray)[i]);
    }

    // free string array and set pointer to NULL
    free(*stringArray);
    *stringArray = NULL;
}

void
freeWatchtowerStructArray(watchtowerData ***watchtowerStructArray) {

    int i;

    // free each struct pointer and strings within
    for (i=0; (*watchtowerStructArray)[i] != NULL; i++) {
        free((*watchtowerStructArray)[i]->watchtowerID);
        free((*watchtowerStructArray)[i]->postcode);
        free((*watchtowerStructArray)[i]->watchtowerName);
        free((*watchtowerStructArray)[i]);
    }

    // free struct array and set pointer to NULL
    free(*watchtowerStructArray);
    *watchtowerStructArray = NULL;
}