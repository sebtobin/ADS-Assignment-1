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
#define NUM_NEW_VERTICES 2
#define NUM_NEW_EDGES 3
#define NUM_NEW_FACES 1

/* initialise a new DCEL ready to be filled */
void
initialiseDcel(dcel_t *dcel, int initArraySize) {

    // initialise initial array sizes
    dcel->verticesArraySize = initArraySize;
    dcel->edgesArraySize = initArraySize;
    dcel->facesArraySize = initArraySize;

    // initialise array n item buddy variables
    dcel->numVertices = 0;
    dcel->numEdges = 0;
    dcel->numFaces = 0;

    // allocate memory for each array
    dcel->verticesArray = (vertex_t*)malloc(sizeof(vertex_t) * dcel->verticesArraySize);
    assert(dcel->verticesArray);

    dcel->edgesArray = (edge_t*)malloc(sizeof(edge_t) * dcel->edgesArraySize);
    assert(dcel->edgesArray);

    dcel->facesArray = (face_t*)malloc(sizeof(face_t) * dcel->facesArraySize);
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
        if (i == dcel->verticesArraySize) {
            dcel->verticesArraySize *= 2;
            dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->verticesArraySize);
            assert(dcel->verticesArray);
            dcel->edgesArraySize *= 2;
            dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->edgesArraySize);
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
            prev->nextHalfEdge = *curr;
        }

        // set the edge struct pointer
        dcel->edgesArray[dcel->numEdges++].halfEdge = *curr;

        // set pointers for next loop
        prev = *curr;
        curr = &((*curr)->nextHalfEdge);
    }

    // link up first and last half edge
    *curr = dcel->facesArray[dcel->numFaces].halfEdge;
    dcel->facesArray[dcel->numFaces++].halfEdge->prevHalfEdge = prev;

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
    int initVertIndex = curr->startVertIndex;
    printf("\nface index: %d\n\n", faceIndex);

    // print every half edge in a face
    do {
        printHalfEdge(dcel, curr);
        curr = curr->nextHalfEdge;
        printf("\n");
    } while (curr->startVertIndex != initVertIndex);
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
    printHalfEdge(dcel, dcel->edgesArray[edgeIndex].halfEdge);
    printf("\n");

    if (dcel->edgesArray[edgeIndex].halfEdge->oppositeHalfEdge != NULL) {

        printHalfEdge(dcel, dcel->edgesArray[edgeIndex].halfEdge->oppositeHalfEdge);
        printf("\n");

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
    double y2 = dcel->verticesArray[halfEdge->endVertIndex].yCoord;

    // set return vertex coords as midpoint
    midPoint.xCoord = (x1 + x2) / 2;
    midPoint.yCoord = (y1 + y2) / 2;

    return midPoint;
}

vertex_t
getVerticesMidPoint(vertex_t vertex1, vertex_t vertex2) {

    vertex_t midPoint;

    // set return vertex coords as midpoint
    midPoint.xCoord = (vertex1.xCoord + vertex2.xCoord) / 2;
    midPoint.yCoord = (vertex1.yCoord + vertex2.yCoord) / 2;

    return midPoint;
}

/* execute a split from startEdgeIndex to endEdgeIndex on the DCEL */
dcel_t*
edgeSplit(dcel_t *dcel, int startEdgeIndex, int endEdgeIndex) {

    int startEdgeEndVertIndex, endEdgeStartVertexIndex, areAdjacentEdges = 0;
    halfEdge_t *temp = NULL, *twin = NULL, *startEdgeNext = NULL, *endEdgePrev = NULL;

    // get new vertices and midpoint for bisecting edge
    vertex_t startEdgeMidPoint = getEdgeMidPoint(dcel, startEdgeIndex);
    vertex_t endEdgeMidPoint = getEdgeMidPoint(dcel, endEdgeIndex);
    vertex_t verticesMidPoint = getVerticesMidPoint(startEdgeMidPoint, endEdgeMidPoint);

    // set half edge pointer in edge structs so they belong to face being split, exit if inside edge is NULL
    if (!halfEdgeCheck(dcel, dcel->edgesArray[startEdgeIndex].halfEdge, verticesMidPoint)) {
        dcel->edgesArray[startEdgeIndex].halfEdge = dcel->edgesArray[startEdgeIndex].halfEdge->oppositeHalfEdge;
        assert(dcel->edgesArray[startEdgeIndex].halfEdge);
    }
    if (!halfEdgeCheck(dcel, dcel->edgesArray[endEdgeIndex].halfEdge, verticesMidPoint)) {
        dcel->edgesArray[endEdgeIndex].halfEdge = dcel->edgesArray[endEdgeIndex].halfEdge->oppositeHalfEdge;
        assert(dcel->edgesArray[endEdgeIndex].halfEdge);
    }

    // allocate space for more vertices if required
    if (dcel->verticesArraySize - dcel->numVertices < NUM_NEW_VERTICES) {
        dcel->verticesArraySize *= 2;
        dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->verticesArraySize);
        assert(dcel->verticesArray);
    }

    // allocate space for more edges if required
    if (dcel->edgesArraySize - dcel->numEdges < NUM_NEW_EDGES) {
        dcel->edgesArraySize *= 2;
        dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->edgesArraySize);
        assert(dcel->edgesArray);
    }

    // allocate space for more faces if required
    if (dcel->facesArraySize - dcel->numFaces < NUM_NEW_FACES) {
        dcel->facesArraySize *= 2;
        dcel->facesArray = (face_t*)realloc(dcel->facesArray, sizeof(face_t) * dcel->facesArraySize);
        assert(dcel->facesArray);
    }

    // store vertex indices of old edges
    startEdgeEndVertIndex = dcel->edgesArray[startEdgeIndex].halfEdge->endVertIndex;
    endEdgeStartVertexIndex = dcel->edgesArray[endEdgeIndex].halfEdge->startVertIndex;

    if (dcel->edgesArray[startEdgeIndex].halfEdge->nextHalfEdge == dcel->edgesArray[endEdgeIndex].halfEdge) {
        areAdjacentEdges = 1;
    }
    else {
        startEdgeNext = dcel->edgesArray[startEdgeIndex].halfEdge->nextHalfEdge;
        endEdgePrev = dcel->edgesArray[endEdgeIndex].halfEdge->prevHalfEdge;
    }

    // set new vertices at the midpoint between split edges
    dcel->verticesArray[dcel->numVertices] = startEdgeMidPoint;
    dcel->verticesArray[dcel->numVertices + 1] = endEdgeMidPoint;

    // set new start/ending vertices for split edges
    dcel->edgesArray[startEdgeIndex].halfEdge->endVertIndex = dcel->numVertices++;
    dcel->edgesArray[endEdgeIndex].halfEdge->startVertIndex = dcel->numVertices++;

    // create bisecting half edge for already existing face, linked to split edges
    temp = getHalfEdge(dcel->edgesArray[startEdgeIndex].halfEdge->endVertIndex,
                dcel->edgesArray[endEdgeIndex].halfEdge->startVertIndex,
                dcel->edgesArray[startEdgeIndex].halfEdge,
                dcel->edgesArray[endEdgeIndex].halfEdge);

    // link split edges to new bisecting edge
    dcel->edgesArray[startEdgeIndex].halfEdge->nextHalfEdge = temp;
    dcel->edgesArray[endEdgeIndex].halfEdge->prevHalfEdge = temp;

    // store new edge in edge array, set new half edge's edge and face indices
    temp->edgeIndex = dcel->numEdges;
    temp->faceIndex = temp->prevHalfEdge->faceIndex;
    dcel->edgesArray[dcel->numEdges].halfEdge = temp;
    dcel->facesArray[temp->faceIndex].halfEdge = temp;

    /* get twin of bisecting half edge, set new face half edge pointer
     * to this half edge, set edge index to the same index as twin
     */
    temp->oppositeHalfEdge = getHalfEdge(temp->endVertIndex, temp->startVertIndex, NULL, NULL);
    temp->oppositeHalfEdge->oppositeHalfEdge = temp;
    temp = temp->oppositeHalfEdge;
    dcel->facesArray[dcel->numFaces].halfEdge = temp;
    temp->edgeIndex = dcel->numEdges++;

    /* setup new half edges on either side of twin bisecting half edge,
     * these and linked half edges make up the new face, for each set edge index
     * and set new edge's half edge pointer, needed for twin half edge's opposite
     * half edge pointer, if a twin half edge exists
     */
    temp->nextHalfEdge = getHalfEdge(temp->endVertIndex, startEdgeEndVertIndex, temp, startEdgeNext);
    temp->nextHalfEdge->edgeIndex = dcel->numEdges;
    dcel->edgesArray[dcel->numEdges].halfEdge = temp->nextHalfEdge;

    // if starting edge has twin edge, create and link up new half edge
    if ((twin = dcel->edgesArray[startEdgeIndex].halfEdge->oppositeHalfEdge) != NULL) {

        // store previous next half edge pointer and end vertex index
        halfEdge_t *startEdgePrev = twin->prevHalfEdge;
        int startEdgeStartVertIndex = twin->startVertIndex;

        // create new half edge and link to twin split edge
        twin->startVertIndex = twin->oppositeHalfEdge->endVertIndex;
        twin->prevHalfEdge = getHalfEdge(startEdgeStartVertIndex, twin->startVertIndex, startEdgePrev, twin);
        startEdgePrev->nextHalfEdge = twin->prevHalfEdge;

        // assign new half edge's indices and set opposite half edge pointer
        twin->prevHalfEdge->edgeIndex = dcel->numEdges;
        twin->prevHalfEdge->faceIndex = twin->faceIndex;
        twin->prevHalfEdge->oppositeHalfEdge = dcel->edgesArray[dcel->numEdges].halfEdge;

    }
    dcel->numEdges++;

    temp->prevHalfEdge = getHalfEdge(endEdgeStartVertexIndex, temp->startVertIndex, endEdgePrev, temp);
    temp->prevHalfEdge->edgeIndex = dcel->numEdges;
    dcel->edgesArray[dcel->numEdges].halfEdge = temp->prevHalfEdge;

    // if ending edge has twin edge, create and link up new half edge
    if ((twin = dcel->edgesArray[endEdgeIndex].halfEdge->oppositeHalfEdge) != NULL) {

        // store previous next half edge pointer and end vertex index
        halfEdge_t *endEdgeNext = twin->nextHalfEdge;
        int endEdgeEndVertIndex = twin->endVertIndex;

        // create new half edge and link to twin split edge
        twin->endVertIndex = twin->oppositeHalfEdge->startVertIndex;
        twin->nextHalfEdge = getHalfEdge(twin->endVertIndex, endEdgeEndVertIndex, twin, endEdgeNext);
        endEdgeNext->prevHalfEdge = twin->nextHalfEdge;

        // assign new half edge's indices and set opposite half edge pointer
        twin->nextHalfEdge->edgeIndex = dcel->numEdges;
        twin->nextHalfEdge->faceIndex = twin->faceIndex;
        twin->nextHalfEdge->oppositeHalfEdge = dcel->edgesArray[dcel->numEdges].halfEdge;

    }
    dcel->numEdges++;

    /* if the starting edge's next half edge is the ending edge, new half edges
     * won't have a preexisting half edge to link to, so they must link to each other
     * else, link preexisting half edge(s) to new half edges
     */
    if (areAdjacentEdges) {
        temp->nextHalfEdge->nextHalfEdge = temp->prevHalfEdge;
        temp->prevHalfEdge->prevHalfEdge = temp->nextHalfEdge;
    }
    else {
        startEdgeNext->prevHalfEdge = temp->nextHalfEdge;
        endEdgePrev->nextHalfEdge = temp->prevHalfEdge;
    }

    // set the face indices of all half edges in new face
    setFaceIndex(dcel, temp, dcel->numFaces++);

    return dcel;
}

void
setFaceIndex(dcel_t *dcel, halfEdge_t *halfEdge, int faceIndex) {

    int initVertIndex = halfEdge->startVertIndex;

    do {
        halfEdge->faceIndex = faceIndex;
        halfEdge = halfEdge->nextHalfEdge;
    } while (halfEdge->startVertIndex != initVertIndex);

}

halfEdge_t*
getHalfEdge(int startVertIndex, int endVertIndex, halfEdge_t *prev, halfEdge_t *next) {

    // allocate memory for new half edge
    halfEdge_t *halfEdge = (halfEdge_t*)malloc(sizeof(halfEdge_t));
    assert(halfEdge);

    // set indices and pointers
    halfEdge->startVertIndex = startVertIndex;
    halfEdge->endVertIndex = endVertIndex;
    halfEdge->nextHalfEdge = next;
    halfEdge->prevHalfEdge = prev;
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

        temp = currHalfEdge;
        currHalfEdge = currHalfEdge->nextHalfEdge;
        free(temp);
    }

}

int
halfEdgeCheck(dcel_t* dcel, halfEdge_t *halfEdge, vertex_t midPoint) {

    double y2 = dcel->verticesArray[halfEdge->endVertIndex].yCoord;
    double y1 = dcel->verticesArray[halfEdge->startVertIndex].yCoord;
    double x2 = dcel->verticesArray[halfEdge->endVertIndex].xCoord;
    double x1 = dcel->verticesArray[halfEdge->startVertIndex].xCoord;

    // check the special case for x1 == x2
    if (x1 == x2) {

        if (y2 > y1 && midPoint.xCoord > x1) {
            return 1;
        }
        else if (y1 > y2 && midPoint.xCoord < x1) {
            return 1;
        }
        else {
            return 0;
        }

    }

    // check the special case for y1 == y2
    if (y1 == y2) {

        if (x2 > x1 && midPoint.yCoord < y1) {
            return 1;
        }
        else if (x1 > x2 && midPoint.yCoord > y1) {
            return 1;
        }
        else {
            return 0;
        }

    }

    // if x1 != x2 and y1 != y2 use the gradient to check if midpoint is in the face
    double m = (y2 - y1) / (x2 - x1);
    double c = y2 - m * x2;
    double yPredicted = m * midPoint.xCoord + c;

    if ((x1 > x2 && midPoint.yCoord > yPredicted) || (x2 > x1 && yPredicted > midPoint.yCoord)) {
        return 1;
    }
    return 0;
}
