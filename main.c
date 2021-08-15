/*
 * Main program for COMP20003 Assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "readCSVData.h"

int main(int argc, char *argv[]) {

    FILE *CSVData;
    char **watchtowerStringArray = NULL;
    watchtowerData **watchtowerStructArray = NULL;

    // read CSV and store watchtower data into array of structs
    CSVData = fopen(argv[1], "r");
    watchtowerStringArray = readCSVData(CSVData, watchtowerStringArray);
    watchtowerStructArray = readWatchtowerStringArray(watchtowerStringArray, watchtowerStructArray);
    freeStringArray(&watchtowerStringArray);

    for (int i=0; watchtowerStructArray[i] != NULL; i++) {
        printWatchtowerStruct(watchtowerStructArray[i]);
    }

    printf("program finished");
    return 0;
}




