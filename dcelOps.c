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

void
printDcel(dcel_t *dcel) {

    int i;

    printf("\nDCEL Data:\n\n");
    printf("# of Faces: %d\n", dcel->numFaces);
    printf("# of Edges: %d\n", dcel->numEdges);
    printf("# of Vertices: %d\n", dcel->numVertices);

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

void printFace(dcel_t* dcel, int faceIndex) {

    halfEdge_t *curr = dcel->facesArray[faceIndex].halfEdge;
    int startVertIndex = curr->startVertIndex;
    printf("\nFace index: %d\n\n", faceIndex);

    do {
        printHalfEdge(dcel, curr);
        curr = curr->nextHalfEdge;
        printf("\n");
    } while (curr->startVertIndex != startVertIndex);
}

void printHalfEdge(dcel_t *dcel, halfEdge_t *halfEdge) {
    printf("Face index %d\n", halfEdge->faceIndex);
    printf("Edge index: %d\n", halfEdge->edgeIndex);
    printf("Starting "); printVertex(dcel, halfEdge->startVertIndex);
    printf("Ending "); printVertex(dcel, halfEdge->endVertIndex);
}

void printEdge(dcel_t *dcel, int edgeIndex) {
    printf("Edge at index %d has half edges:\n\n", edgeIndex);
    printHalfEdge(dcel, dcel->edgesArray[edgeIndex].halfEdge); printf("\n");
    if (dcel->edgesArray[edgeIndex].halfEdge->oppositeHalfEdge != NULL) {
        printHalfEdge(dcel, dcel->edgesArray[edgeIndex].halfEdge->oppositeHalfEdge);
    }
}

void printVertex(dcel_t* dcel, int vertexIndex) {
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
edgeSplit(dcel_t *dcel, int edgeIndex1, int edgeIndex2) {

    if (dcel->maxVertices - dcel->numVertices < 2) {
        dcel->maxVertices *= 2;
        dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->maxVertices);
        assert(dcel->verticesArray);
    }
    dcel->verticesArray[dcel->numVertices++] = getEdgeMidPoint(dcel, edgeIndex1);
    dcel->verticesArray[dcel->numVertices++] = getEdgeMidPoint(dcel, edgeIndex2);

    /* for adding whole edges after linkong all the half edges
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

