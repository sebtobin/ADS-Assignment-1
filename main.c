/*
 * Main program for COMP20003 Assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "readInputData.h"

int main(int argc, char *argv[]) {

    int i;
    FILE *CSVData;
    FILE *initPolygonData;
    char **watchtowerStringArray = NULL;
    watchtowerData **watchtowerStructArray = NULL;
    char **initPolygonStringArray = NULL;

    // print command line args
    printf("\nCommand Line Arguments:\n\n");
    for (i=0; argv[i]; i++) {
        printf("%s\n", argv[i]);
    } printf("\n");

    // read CSV and store watchtower data into array of structs
    CSVData = fopen(argv[1], "r");
    watchtowerStringArray = readTextData(CSVData, watchtowerStringArray);
    fclose(CSVData);

    // print strings in watchtowerStringsArray
    printf("Watchtower Strings:\n\n");
    for (i=0; watchtowerStringArray[i]; i++) {
        printf("%s", watchtowerStringArray[i]);
    } printf("\n");

    watchtowerStructArray = readWatchtowerStringArray(watchtowerStringArray, watchtowerStructArray);

    // freeing strings and string array
    freeStringArray(&watchtowerStringArray);

    initPolygonData = fopen(argv[2], "r");
    initPolygonStringArray = readTextData(initPolygonData, initPolygonStringArray);

    // print struct variables of structs in watchtowerStructArray
    printf("Watchtower Structs:\n\n");
    for (i=0; watchtowerStructArray[i] != NULL; i++) {
        printWatchtowerStruct(watchtowerStructArray[i]);
    }

    // print strings in initPolygonStringArray
    printf("Initial Polygon Strings:\n\n");
    for (i=0; initPolygonStringArray[i]; i++) {
        printf("%s", initPolygonStringArray[i]);
    } printf("\n");

    // freeing struct array
    freeWatchtowerStructArray(&watchtowerStructArray);

    printf("\nprogram finished\n\n");

    return 0;
}




