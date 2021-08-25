/*
 * file containing functions for reading text and storing as an
 * array of strings to be then stored in specialised structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "readInputData.h"

//  # defines

#define LINE_BUFFER_SIZE 512
#define INIT_ARRAY_SIZE 16
#define MIN_SPLIT_CHARS 3

// read a file and store each line as a string in an array
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

        // input files are assumed to be well formatted, however weird (bug?)
        // causes redirection of empty string, as in <<< "", to input a string
        // which is just a newline character, as such treat this as no string
        if (strlen(lineBuffer) < MIN_SPLIT_CHARS) {
            break;
        }

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

// free all the strings in an array, then the array itself
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

