/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readCSVData.h"

#define LINE_DELIM '\n'
#define CELL_DELIM ','
#define BUFFER_SIZE 512

char** readCSVData(FILE *watchtowerCSV) {

    int i, j;
    char lineBuffer[BUFFER_SIZE + 1];

    while(fscanf(watchtowerCSV, "%s\n", lineBuffer) == 1) {
        printf("line recorded")
    }
}

