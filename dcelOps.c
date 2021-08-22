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

    // read in each vertex for the initial polygon
    for (i=0; initPolygonStringArray[i]; i++) {

        // allocate more memory if needed, as for the initial polygon num edges = num vertices
        if (i == dcel->maxVertices) {
            dcel->maxVertices *= 2;
            dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->maxVertices);
            assert(dcel->verticesArray);
            dcel->maxEdges *= 2;
            dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->maxEdges);
            assert(dcel->edgesArray);
        }

        // set vertices
        dcel->verticesArray[dcel->numVertices].xCoord = strtod(strtok(initPolygonStringArray[i], COORD_DELIM), NULL);
        dcel->verticesArray[dcel->numVertices++].yCoord = strtod(strtok(NULL, COORD_DELIM), NULL);
    }

    // start with the half edge pointer in the face struct
    curr = &(dcel->facesArray[dcel->numFaces].halfEdge);

    // create new half edge each loop until all vertices linked
    for (i=0; i<dcel->numVertices; i++) {

        *curr = (halfEdge_t*)malloc(sizeof(halfEdge_t));
        assert(*curr);

        // set indices and opposite half edge pointer
        (*curr)->startVertIndex = i;
        (*curr)->endVertIndex = (i + 1) % dcel->numVertices;
        (*curr)->faceIndex = 0;
        (*curr)->edgeIndex = i;
        (*curr)->oppositeHalfEdge = NULL;

        // link between previous half edge
        if (prev != NULL) {
            (*curr)->prevHalfEdge = prev;
            (*curr)->prevHalfEdge->nextHalfEdge = *curr;
        }

        // set the edge struct pointer
        dcel->edgesArray[dcel->numEdges++].halfEdge = *curr;

        // set pointers for next loop
        prev = *curr;
        curr = &((*curr)->nextHalfEdge);
    }

    // link up first and last half edge
    *curr = dcel->facesArray[0].halfEdge;
    dcel->facesArray[dcel->numFaces++].halfEdge->prevHalfEdge = *curr;

    return dcel;
}

/* print all information in a DCEL */
void
printDcel(dcel_t *dcel) {

    int i;

    printf("\ndcel data:\n\n");

    // print number of faces, edges and vertices in the DCEL
    printf("# of faces: %d\n", dcel->numFaces);
    printf("# of edges: %d\n", dcel->numEdges);
    printf("# of vertices: %d\n", dcel->numVertices);

    // print info for every face, edge and vertex in the DCEL
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

    // print every half edge in a face
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

/* calculate the midpoint of en edge and return a vertex at that location */
vertex_t
getEdgeMidPoint(dcel_t *dcel, int edgeIndex) {

    vertex_t midPoint;
    halfEdge_t *halfEdge = dcel->edgesArray[edgeIndex].halfEdge;

    // get coords of vertices attached to edge
    double x1 = dcel->verticesArray[halfEdge->startVertIndex].xCoord;
    double y1 = dcel->verticesArray[halfEdge->startVertIndex].yCoord;
    double x2 = dcel->verticesArray[halfEdge->endVertIndex].xCoord;
    double y2 =  dcel->verticesArray[halfEdge->endVertIndex].yCoord;

    // set return vertex coords as midpoint
    midPoint.xCoord = (x1 + x2) / 2;
    midPoint.yCoord = (y1 + y2) / 2;

    return midPoint;
}

/* execute a split from startEdgeIndex to endEdgeIndex on the DCEL */
dcel_t*
edgeSplit(dcel_t *dcel, int startEdgeIndex, int endEdgeIndex) {

    // allocate more space in DCEL arrays if required
    if (dcel->maxVertices - dcel->numVertices < 2) {
        dcel->maxVertices *= 2;
        dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->maxVertices);
        assert(dcel->verticesArray);
    }

    // set new vertices
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

    // allocate memory for new half edge
    halfEdge_t *halfEdge = (halfEdge_t*)malloc(sizeof(halfEdge_t));
    assert(halfEdge);

    // set indices and pointers
    halfEdge->startVertIndex = startVertIndex;
    halfEdge->endVertIndex = endVertIndex;
    halfEdge->edgeIndex = edgeIndex;
    halfEdge->nextHalfEdge = NULL;
    halfEdge->prevHalfEdge = NULL;
    halfEdge->oppositeHalfEdge = NULL;

    return halfEdge;
}

/* execute all the splits in splitsStringArray */
dcel_t*
executeSplits(dcel_t *dcel, char **splitStringArray) {

    int i, startEdgeIndex, endEdgeIndex;

    // execute splits in array order
    for (i=0; splitStringArray[i] != NULL; i++) {

        // get indices from string array
        startEdgeIndex = atoi(strtok(splitStringArray[i], SPLIT_DELIM));
        endEdgeIndex = atoi(strtok(NULL, SPLIT_DELIM));

        // execute split about edges
        edgeSplit(dcel, startEdgeIndex, endEdgeIndex);
        printDcel(dcel);
    }
    return dcel;
}

void printVisual(dcel_t *dcel) {

    int i, initVertIndex;
    halfEdge_t *curr;

    for (i=0; i<dcel->numFaces; i++) {

        curr = dcel->facesArray[i].halfEdge;
        initVertIndex = curr->startVertIndex; printf("\n");
        do {

            printf("@E%d %d %lf %lf %lf %lf\n"
                   , curr->edgeIndex
                   , curr->faceIndex
                   , dcel->verticesArray[curr->startVertIndex].xCoord
                   , dcel->verticesArray[curr->startVertIndex].yCoord
                   , dcel->verticesArray[curr->endVertIndex].xCoord
                   , dcel->verticesArray[curr->endVertIndex].yCoord);
            curr = curr->nextHalfEdge;

        } while (curr->startVertIndex != initVertIndex);

    }

}

void freeDcel(dcel_t *dcel) {

    for (int i=0; i<dcel->numFaces; i++) {

        freeFace(dcel->facesArray[i].halfEdge);
    }

    free(dcel->verticesArray);
    free(dcel->edgesArray);
    free(dcel->facesArray);

}

void freeFace(halfEdge_t *currHalfEdge) {

    halfEdge_t *temp;
    currHalfEdge->prevHalfEdge->nextHalfEdge = NULL;

    while (currHalfEdge != NULL) {

        /*printf("prev: %d curr: %d next: %d\n",
               currHalfEdge->prevHalfEdge->edgeIndex,
               currHalfEdge->edgeIndex,
               currHalfEdge->nextHalfEdge->edgeIndex);*/
        temp = currHalfEdge;
        currHalfEdge = currHalfEdge->nextHalfEdge;
        free(temp);
    }

}
