/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dcelOps.h"

#define INIT_ARRAY_SIZE 16
#define COORD_DELIM " "

void
initialiseDcel(dcel_t *dcel, int initArraySize) {

    // initialise initial array sizes
    dcel->maxVertices = initArraySize;
    dcel->maxEdges = initArraySize;
    dcel->maxFaces = initArraySize;

    // allocate memory for each array
    dcel->verticesArray = (vertex_t*)malloc(sizeof(vertex_t) * dcel->maxVertices);
    assert(dcel->verticesArray);

    dcel->edgesArray = (edge_t*)malloc(sizeof(edge_t) * dcel->maxEdges);
    assert(dcel->edgesArray);

    dcel->facesArray = (face_t*)malloc(sizeof(face_t) * dcel->maxFaces);
    assert(dcel->facesArray);
}

dcel_t*
buildInitPolygon(char **initPolygonStringArray, dcel_t *dcel) {

    int i, numVertices;
    halfEdge_t **curr, *prev = NULL;

    initialiseDcel(dcel, INIT_ARRAY_SIZE);

    for (i=0; initPolygonStringArray[i]; i++) {
        if (i == dcel->maxVertices) {
            dcel->maxVertices *= 2;
            dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->maxVertices);
            dcel->maxEdges *= 2;
            dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->maxEdges);
        }
        dcel->verticesArray[i].xCoord = strtod(strtok(initPolygonStringArray[i], COORD_DELIM), NULL);
        dcel->verticesArray[i].yCoord = strtod(strtok(NULL, COORD_DELIM), NULL);

    }

    numVertices = i;
    curr = &(dcel->facesArray[0].halfEdge);

    for (i=0; i<numVertices; i++) {

        *curr = (halfEdge_t*)malloc(sizeof(halfEdge_t));
        assert(*curr);

        (*curr)->startVertIndex = i;
        (*curr)->endVertIndex = (i + 1) % numVertices;
        (*curr)->faceIndex = 0;
        (*curr)->edgeIndex = i;
        (*curr)->oppositeHalfEdge = NULL;

        if (prev != NULL) {
            (*curr)->prevHalfEdge = prev;
            (*curr)->prevHalfEdge->nextHalfEdge = *curr;
        }

        prev = *curr;
        dcel->edgesArray[i].halfEdge = *curr;
        curr = &((*curr)->nextHalfEdge);
    }

    *curr = dcel->facesArray[0].halfEdge;
    dcel->facesArray[0].halfEdge->prevHalfEdge = *curr;
    dcel->facesArray[1].halfEdge = NULL;

    return dcel;
}

void
printDcel(dcel_t *dcel) {

    int i, startVertIndex;
    halfEdge_t *curr;

    printf("\nDCEL Data:\n");

    for (i=0; dcel->facesArray[i].halfEdge != NULL; i++) {

        curr = dcel->facesArray[i].halfEdge;
        startVertIndex = curr->startVertIndex;
        printf("\nFace index: %d\n\n", i);

        do {
            printf("Edge index: %d\n", curr->edgeIndex);
            printf("Starting vertex index: %d\n", curr->startVertIndex);
            printf("Ending vertex index: %d\n", curr->endVertIndex);
            curr = curr->nextHalfEdge;
        } while (curr->startVertIndex != startVertIndex);
    }
}