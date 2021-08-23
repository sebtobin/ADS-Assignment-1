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

    int startEdgeEndVertexIndex, endEdgeStartVertexIndex, areAdjacentEdges;
    halfEdge_t *temp, *startEdgeNext, *endEdgePrev;

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

    // allocate more space in DCEL arrays if required
    if (dcel->verticesArraySize - dcel->numVertices < 2) {
        dcel->verticesArraySize *= 2;
        dcel->verticesArray = (vertex_t*)realloc(dcel->verticesArray, sizeof(vertex_t) * dcel->verticesArraySize);
        assert(dcel->verticesArray);
    }

    // store half edge pointers from starting and ending half edges
    startEdgeNext = dcel->edgesArray[startEdgeIndex].halfEdge->nextHalfEdge;
    startEdgeEndVertexIndex = dcel->edgesArray[startEdgeIndex].halfEdge->endVertIndex;
    endEdgePrev = dcel->edgesArray[endEdgeIndex].halfEdge->prevHalfEdge;
    endEdgeStartVertexIndex = dcel->edgesArray[endEdgeIndex].halfEdge->startVertIndex;

    // set new vertices
    dcel->verticesArray[dcel->numVertices] = startEdgeMidPoint;
    dcel->verticesArray[dcel->numVertices + 1] = endEdgeMidPoint;
    dcel->edgesArray[startEdgeIndex].halfEdge->endVertIndex = dcel->numVertices;
    if ((temp = dcel->edgesArray[startEdgeIndex].halfEdge->oppositeHalfEdge) != NULL) {
        temp->endVertIndex = dcel->numVertices;
    }
    dcel->numVertices++;
    dcel->edgesArray[endEdgeIndex].halfEdge->startVertIndex = dcel->numVertices;
    if ((temp = dcel->edgesArray[endEdgeIndex].halfEdge->oppositeHalfEdge) != NULL) {
        temp->startVertIndex = dcel->numVertices;
    }
    dcel->numVertices++;

    // create bisecting half edge for already existing face
    temp = getHalfEdge(dcel->edgesArray[startEdgeIndex].halfEdge->endVertIndex,
                dcel->edgesArray[endEdgeIndex].halfEdge->startVertIndex,
                dcel->edgesArray[startEdgeIndex].halfEdge,
                dcel->edgesArray[endEdgeIndex].halfEdge);

    // link to starting and ending half edges in already existing face
    dcel->edgesArray[startEdgeIndex].halfEdge->nextHalfEdge = temp;
    dcel->edgesArray[endEdgeIndex].halfEdge->prevHalfEdge = temp;

    // allocate space for more edges
    if (dcel->edgesArraySize - dcel->numEdges < 3) {
        dcel->edgesArraySize *= 2;
        dcel->edgesArray = (edge_t*)realloc(dcel->edgesArray, sizeof(edge_t) * dcel->edgesArraySize);
        assert(dcel->edgesArray);
    }

    // store new edge in edge array, set new half edge's edge and face indices
    dcel->edgesArray[dcel->numEdges].halfEdge = temp;
    dcel->facesArray[dcel->numFaces - 1].halfEdge = temp;
    temp->edgeIndex = dcel->numEdges;
    temp->faceIndex = temp->prevHalfEdge->faceIndex;

    // get twin of bisecting half edge
    temp = getHalfEdge(temp->endVertIndex, temp->startVertIndex, NULL, NULL);
    dcel->facesArray[dcel->numFaces].halfEdge = temp;
    temp->edgeIndex = dcel->numEdges++;

    // setup new half edges
    temp->nextHalfEdge = getHalfEdge(temp->endVertIndex, startEdgeEndVertexIndex, temp, startEdgeNext);
    startEdgeNext->prevHalfEdge = temp->nextHalfEdge;
    temp->nextHalfEdge->edgeIndex = dcel->numEdges;
    dcel->edgesArray[dcel->numEdges++].halfEdge = temp->nextHalfEdge;

    temp->prevHalfEdge = getHalfEdge(endEdgeStartVertexIndex, temp->startVertIndex, endEdgePrev, temp);
    endEdgePrev->nextHalfEdge = temp->prevHalfEdge;
    temp->prevHalfEdge->edgeIndex = dcel->numEdges;
    dcel->edgesArray[dcel->numEdges++].halfEdge = temp->prevHalfEdge;

    setFaceIndex(dcel, temp, dcel->numFaces++);

    return dcel;
}

void
setFaceIndex(dcel_t *dcel, halfEdge_t *halfEdge, int faceIndex) {

    int initVertIndex = halfEdge->startVertIndex;
    printf("\n\ninit vert index: %d\n", initVertIndex);

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

    printf("%lf\n", y2);
    printf("%lf\n", y1);
    printf("%lf\n", x2);
    printf("%lf\n", x1);
    printf("%lf\n", m);
    printf("%lf\n", c);
    printf("%lf\n",yPredicted);

    if ((y2 > y1 && midPoint.yCoord > yPredicted) || (y1 > y2 && yPredicted > midPoint.yCoord)) {
        return 1;
    }
    return 0;
}
