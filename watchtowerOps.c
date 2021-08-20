/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "watchtowerOps.h"

#define CELL_DELIM ","
#define INIT_ARRAY_SIZE 16

/* read an array of strings, with each string being a CSV record
 * and store each string in a watchtowerData_t struct
 */
watchtowerData_t**
storeWatchtowerStructs(char **watchtowerStringArray, watchtowerData_t **watchtowerStructArray) {

    int i;
    char *cellBuffer;
    size_t arraySize = INIT_ARRAY_SIZE;

    // allocate memory for array of pointers to csv records was structs
    watchtowerStructArray = (watchtowerData_t**)malloc(sizeof(watchtowerData_t*) * arraySize);
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
            watchtowerStructArray = (watchtowerData_t**)realloc(watchtowerStructArray, sizeof(watchtowerData_t*) * arraySize);
            assert(watchtowerStructArray);
        }

        // allocate memory to current watchtower struct pointer
        watchtowerStructArray[i] = (watchtowerData_t*)malloc(sizeof(watchtowerData_t));
        assert(watchtowerStructArray[i]);

        /* hardcode storing of each cell into corresponding struct variables;
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

/* print out the contents of the watchtowerData_t struct */
void
printWatchtowerStruct(watchtowerData_t *watchtowerStruct) {
    printf("%s\n%s\n%d\n%s\n%lf\n%lf\n\n", watchtowerStruct->watchtowerID
           , watchtowerStruct->postcode, watchtowerStruct->populationServed
           , watchtowerStruct->watchtowerName, watchtowerStruct->longitude
           , watchtowerStruct->latitude);
}

/* free all the strings in the watchtowerData_t structs in an array,
 * the structs and then the array itself
 */
void
freeWatchtowerStructArray(watchtowerData_t ***watchtowerStructArray) {

    // free each struct pointer and strings within
    for (int i=0; (*watchtowerStructArray)[i] != NULL; i++) {
        free((*watchtowerStructArray)[i]->watchtowerID);
        free((*watchtowerStructArray)[i]->postcode);
        free((*watchtowerStructArray)[i]->watchtowerName);
        free((*watchtowerStructArray)[i]);
    }

    // free struct array and set pointer to NULL
    free(*watchtowerStructArray);
    *watchtowerStructArray = NULL;
}

/* get the number of watchtowers in watchtower pointer array */
int
numWatchtowers(watchtowerData_t **watchtowersArray) {

    int i;

    // iterate through watchtower pointers until null pointers
    for (i=0; watchtowersArray[i] != NULL; i++) {
    }

    return i;
}