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
#define SPLIT_DELIM " "

/* initialise a new DCEL ready to be filled */
void
initialiseDcel(dcel_t *dcel, int initArraySize) {

    // initialise initial array sizes
    dcel->maxVertices = initArraySize;
    dcel->maxEdges = initArraySize;
    dcel->maxFaces = initArraySize;

    // initialise array n item buddy variables
    dcel->numVertices = 0;
    dcel->numEdges = 0;
    dcel->numFaces = 0;

    // allocate memory for each array
    dcel->verticesArray = (vertex_t*)malloc(sizeof(vertex_t) * dcel->maxVertices);
    assert(dcel->verticesArray);

    dcel->edgesArray = (edge_t*)malloc(sizeof(edge_t) * dcel->maxEdges);
    assert(dcel->edgesArray);

    dcel->facesArray = (face_t*)malloc(sizeof(face_t) * dcel->maxFaces);
    assert(dcel->facesArray);
}

/* build initial polygon into empty DCEL */
dcel_t*
buildInitPolygon(char **initPolygonStringArray, dcel_t *dcel) {

    int i;
    halfEdge_t **curr, *prev = NULL;

    for (i=0; initPolygonStringArray[i]; i++) {
        if (i == dcel->maxVertices) {
            dcel->maxVertices *= 2;
            dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->maxVertices);
            assert(dcel->verticesArray);
            dcel->maxEdges *= 2;
            dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->maxEdges);
            assert(dcel->edgesArray);
        }

        dcel->verticesArray[dcel->numVertices].xCoord = strtod(strtok(initPolygonStringArray[i], COORD_DELIM), NULL);
        dcel->verticesArray[dcel->numVertices++].yCoord = strtod(strtok(NULL, COORD_DELIM), NULL);
    }

    curr = &(dcel->facesArray[dcel->numFaces].halfEdge);

    for (i=0; i<dcel->numVertices; i++) {

        *curr = (halfEdge_t*)malloc(sizeof(halfEdge_t));
        assert(*curr);

        (*curr)->startVertIndex = i;
        (*curr)->endVertIndex = (i + 1) % dcel->numVertices;
        (*curr)->faceIndex = 0;
        (*curr)->edgeIndex = i;
        (*curr)->oppositeHalfEdge = NULL;

        if (prev != NULL) {
            (*curr)->prevHalfEdge = prev;
            (*curr)->prevHalfEdge->nextHalfEdge = *curr;
        }

        prev = *curr;
        dcel->edgesArray[dcel->numEdges++].halfEdge = *curr;
        curr = &((*curr)->nextHalfEdge);
    }

    *curr = dcel->facesArray[0].halfEdge;
    dcel->facesArray[dcel->numFaces++].halfEdge->prevHalfEdge = *curr;
    return dcel;
}

/* print all information in a DCEL */
void
printDcel(dcel_t *dcel) {

    int i;

    printf("\ndcel data:\n\n");
    printf("# of faces: %d\n", dcel->numFaces);
    printf("# of edges: %d\n", dcel->numEdges);
    printf("# of vertices: %d\n", dcel->numVertices);

    for (i=0; i<dcel->numFaces; i++) {
        printFace(dcel, i);
    }

    for (i=0; i<dcel->numEdges; i++) {
        printEdge(dcel, i);
    }

    for (i=0; i<dcel->numVertices; i++) {
        printVertex(dcel, i);
    }
}

/* print all the half edges in a face */
void
printFace(dcel_t* dcel, int faceIndex) {

    halfEdge_t *curr = dcel->facesArray[faceIndex].halfEdge;
    int startVertIndex = curr->startVertIndex;
    printf("\nface index: %d\n\n", faceIndex);

    do {
        printHalfEdge(dcel, curr);
        curr = curr->nextHalfEdge;
        printf("\n");
    } while (curr->startVertIndex != startVertIndex);
}

/* print all the information in a half edge */
void
printHalfEdge(dcel_t *dcel, halfEdge_t *halfEdge) {
    printf("face index %d\n", halfEdge->faceIndex);
    printf("edge index: %d\n", halfEdge->edgeIndex);
    printf("starting "); printVertex(dcel, halfEdge->startVertIndex);
    printf("ending "); printVertex(dcel, halfEdge->endVertIndex);
}

/* print all half edges at a given edge index */
void
printEdge(dcel_t *dcel, int edgeIndex) {
    printf("edge at index %d has half edges:\n\n", edgeIndex);
    printHalfEdge(dcel, dcel->edgesArray[edgeIndex].halfEdge); printf("\n");
    if (dcel->edgesArray[edgeIndex].halfEdge->oppositeHalfEdge != NULL) {
        printHalfEdge(dcel, dcel->edgesArray[edgeIndex].halfEdge->oppositeHalfEdge);
    }
}

/* print coordinates of a vertex */
void
printVertex(dcel_t* dcel, int vertexIndex) {
    printf("vertex x,y = %lf,%lf at index: %d\n",
           dcel->verticesArray[vertexIndex].xCoord,
           dcel->verticesArray[vertexIndex].yCoord,
           vertexIndex);
}

vertex_t
getEdgeMidPoint(dcel_t *dcel, int edgeIndex) {

    vertex_t midPoint;
    halfEdge_t *halfEdge = dcel->edgesArray[edgeIndex].halfEdge;
    double x1 = dcel->verticesArray[halfEdge->startVertIndex].xCoord;
    double y1 = dcel->verticesArray[halfEdge->startVertIndex].yCoord;
    double x2 = dcel->verticesArray[halfEdge->endVertIndex].xCoord;
    double y2 =  dcel->verticesArray[halfEdge->endVertIndex].yCoord;
    midPoint.xCoord = (x1 + x2) / 2;
    midPoint.yCoord = (y1 + y2) / 2;
    return midPoint;
}

dcel_t*
edgeSplit(dcel_t *dcel, int startEdgeIndex, int endEdgeIndex) {

    if (dcel->maxVertices - dcel->numVertices < 2) {
        dcel->maxVertices *= 2;
        dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->maxVertices);
        assert(dcel->verticesArray);
    }
    dcel->verticesArray[dcel->numVertices++] = getEdgeMidPoint(dcel, startEdgeIndex);
    dcel->verticesArray[dcel->numVertices++] = getEdgeMidPoint(dcel, endEdgeIndex);

    /* for adding whole edges after linking all the half edges
    if (dcel->maxEdges == dcel->numEdges) {
        dcel->maxEdges *= 2;
        dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->maxEdges);
        assert(dcel->edgesArray);
    }
    dcel->edgesArray[dcel->numEdges++].halfEdge
    */

    return dcel;
}

halfEdge_t*
getHalfEdge(int startVertIndex, int endVertIndex, int edgeIndex) {
    halfEdge_t *halfEdge = (halfEdge_t*)malloc(sizeof(halfEdge_t));
    assert(halfEdge);
    halfEdge->startVertIndex = startVertIndex;
    halfEdge->endVertIndex = endVertIndex;
    halfEdge->edgeIndex = edgeIndex;
    return halfEdge;
}

dcel_t*
executeSplits(dcel_t *dcel, char **splitStringArray) {

    int startEdgeIndex, endEdgeIndex;
    for (int i=0; splitStringArray[i] != NULL; i++) {
        startEdgeIndex = atoi(strtok(splitStringArray[i], SPLIT_DELIM));
        endEdgeIndex = atoi(strtok(NULL, SPLIT_DELIM));
        edgeSplit(dcel, startEdgeIndex, endEdgeIndex);
        printDcel(dcel);
    }
    return dcel;
}

