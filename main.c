/*
 * Main program for COMP20003 Assignment 1
 */

#include <stdio.h>
#include "readInputData.h"
#include "watchtowerOps.h"
#include "dcelOps.h"

#define INIT_ARRAY_SIZE 16

/* main function for the assignment */
int
main(int argc, char *argv[]) {

    int i;
    char **watchtowerStringArray = NULL, **initPolygonStringArray = NULL, **splitStringArray = NULL;
    FILE *CSVData, *initPolygonData, *splits;
    watchtowerData_t **watchtowerStructArray = NULL;
    dcel_t dcel;

    /*// print command line args
    printf("\ncommand line arguments:\n\n");
    for (i=0; i<argc; i++) {
        printf("%s\n", argv[i]);
    } printf("\n"); */

    // read CSV and store watchtower data into array of structs
    CSVData = fopen(argv[1], "r");
    watchtowerStringArray = readTextData(CSVData, watchtowerStringArray);
    fclose(CSVData);

    /*// print strings in watchtowerStringsArray
    printf("watchtower strings:\n\n");
    for (i=0; watchtowerStringArray[i]; i++) {
        printf("%s", watchtowerStringArray[i]);
    }*/

    // store strings into watchtower structs and then free string array
    watchtowerStructArray = storeWatchtowerStructs(watchtowerStringArray, watchtowerStructArray);
    freeStringArray(&watchtowerStringArray);

    /*// print struct variables of structs in watchtowerStructArray
    printf("\nwatchtower structs:\n\n");
    for (i=0; watchtowerStructArray[i] != NULL; i++) {
        printWatchtowerStruct(watchtowerStructArray[i]);
    }
    printf("number of watchtowers: %d\n\n", numWatchtowers(watchtowerStructArray)); */

    // read initial polygon data and store in array of strings
    initPolygonData = fopen(argv[2], "r");
    initPolygonStringArray = readTextData(initPolygonData, initPolygonStringArray);
    fclose(initPolygonData);

    /*
    // print strings in initPolygonStringArray
    printf("initial polygon strings:\n\n");
    for (i=0; initPolygonStringArray[i]; i++) {
        printf("%s", initPolygonStringArray[i]);
    }*/

    // initialise DCEL and build initial polygon, then free polygon string array
    initialiseDcel(&dcel, INIT_ARRAY_SIZE);
    buildInitPolygon(initPolygonStringArray, &dcel);
    freeStringArray(&initPolygonStringArray);

    //printDcel(&dcel);

    // read polygon splits into string array
    splits = fopen("data/square_1split.txt", "r");
    splitStringArray = readTextData(splits, splitStringArray);

    // print strings in splitStringArray
    printf("\n\nsplit strings:\n\n");
    for (i=0; splitStringArray[i]; i++) {
        printf("%s", splitStringArray[i]);
    }

    // execute splits then free split string array
    executeSplits(&dcel, splitStringArray);
    freeStringArray(&splitStringArray);

    printVisual(&dcel);

    // freeing watchtower struct array and DCEL
    freeWatchtowerStructArray(&watchtowerStructArray);
    freeDcel(&dcel);

    printf("\nprogram finished\n\n");

    return 0;
}




