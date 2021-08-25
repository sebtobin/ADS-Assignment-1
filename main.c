/*
 * main program for COMP20003 assignment 1
 * contains functions that utilise contents of multiple header files
 */

#include <stdio.h>
#include "readInputData.h"
#include "watchtowerOps.h"
#include "dcelOps.h"
#include "main.h"

//  # defines

#define INIT_ARRAY_SIZE 4

// main function for the assignment
int
main(int argc, char *argv[]) {

    char **watchtowerStringArray = NULL, **initPolygonStringArray = NULL, **splitStringArray = NULL;
    FILE *CSVData, *initPolygonData, *outputFile;
    watchtower_t **watchtowerStructArray = NULL;
    dcel_t dcel;

    // read CSV data and store records as array of strings
    CSVData = fopen(argv[1], "r");
    watchtowerStringArray = readTextData(CSVData, watchtowerStringArray);
    fclose(CSVData);

    // store strings into watchtower structs and then free string array
    watchtowerStructArray = storeWatchtowerStructs(watchtowerStringArray, watchtowerStructArray);
    freeStringArray(&watchtowerStringArray);

    // read initial polygon data and store in array of strings
    initPolygonData = fopen(argv[2], "r");
    initPolygonStringArray = readTextData(initPolygonData, initPolygonStringArray);
    fclose(initPolygonData);

    // initialise DCEL and build initial polygon, then free polygon string array
    initialiseDcel(&dcel, INIT_ARRAY_SIZE);
    buildInitPolygon(initPolygonStringArray, &dcel);
    freeStringArray(&initPolygonStringArray);

    // read polygon splits into string array
    splitStringArray = readTextData(stdin, splitStringArray);

    // execute splits then free split string array
    executeSplits(&dcel, splitStringArray);
    freeStringArray(&splitStringArray);

    // create output file and print watchtower summaries to output file
    outputFile = fopen(argv[3], "w");
    printWatchtowerSummaries(&dcel, watchtowerStructArray, outputFile);
    fclose(outputFile);

    // freeing watchtower struct array and DCEL
    freeWatchtowerStructArray(&watchtowerStructArray);
    freeDcel(&dcel);

    printf("\nprogram finished\n\n");

    return 0;
}

// function prints summary of watchtower info for each face into outputFile
void
printWatchtowerSummaries(dcel_t *dcel, watchtower_t **watchtowerStructArray, FILE *outputFile) {

    int i, j, populations[dcel->numFaces];

    // iterate through each face
    for (i=0; i<dcel->numFaces; i++) {

        fprintf(outputFile, "%d\n", i);
        populations[i] = 0;

        for (j=0; watchtowerStructArray[j]; j++) {

            // ignore watchtowers that have already been categorised into a different face
            if (watchtowerStructArray[j]->isCategorised) {
                continue;
            }

            // check if uncategorised watchtowers are within current face
            else if (inFace(watchtowerStructArray[j], dcel, i)) {

                // sum up population, mark as categorised and then print watchtower info to outputFile
                populations[i] += watchtowerStructArray[j]->populationServed;
                watchtowerStructArray[j]->isCategorised = 1;
                printWatchtowerStruct(watchtowerStructArray[j], outputFile);

            }
        }
    }

    // print total population info for each face to outputFile
    for (i=0; i<dcel->numFaces; i++) {

        fprintf(outputFile, "Face %d population served: %d\n", i, populations[i]);

    }
}

// returns true if watchtower is inside face with index faceIndex, else returns false
int
inFace(watchtower_t *watchtower, dcel_t *dcel, int faceIndex) {

    halfEdge_t *curr = dcel->facesArray[faceIndex].halfEdge;
    int initVertIndex = curr->startVertIndex;

    // create vertex at watchtower location
    vertex_t watchtowerPoint = {watchtower->longitude, watchtower->latitude};

    // return false if vertex can't be inside the face of any half edge in the face
    do {
        if (!halfEdgeCheck(dcel, curr, watchtowerPoint)) {
            return 0;
        }
        curr = curr->nextHalfEdge;
    } while (curr->startVertIndex != initVertIndex);

    return 1;
}




