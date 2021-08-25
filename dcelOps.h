/*
 * header file for dcelOps.c function prototypes and type definitions
 */

#ifndef COMP20003_ASS1_DCELOPS_H
#define COMP20003_ASS1_DCELOPS_H

#endif //COMP20003_ASS1_DCELOPS_H

typedef struct halfEdge {
    int endVertIndex;
    int startVertIndex;
    struct halfEdge *nextHalfEdge;
    struct halfEdge *prevHalfEdge;
    struct halfEdge *oppositeHalfEdge;
    int faceIndex;
    int edgeIndex;
} halfEdge_t;

typedef struct {
    double xCoord;
    double yCoord;
} vertex_t;

typedef struct {
    halfEdge_t *halfEdge;
} edge_t;

typedef struct {
    halfEdge_t *halfEdge;
} face_t;

typedef struct {
    vertex_t *verticesArray;
    edge_t *edgesArray;
    face_t *facesArray;
    int numVertices;
    int numEdges;
    int numFaces;
    size_t verticesArraySize;
    size_t edgesArraySize;
    size_t facesArraySize;
} dcel_t;

dcel_t *buildInitPolygon(char **initPolygonStringArray, dcel_t *dcel);
dcel_t* executeSplits(dcel_t *dcel, char **splitStringArray);
dcel_t* edgeSplit(dcel_t *dcel, int startEdgeIndex, int endEdgeIndex);
halfEdge_t* getHalfEdge(int startVertIndex, int endVertIndex, halfEdge_t *prev, halfEdge_t *next);
vertex_t getEdgeMidPoint(dcel_t *dcel, int edgeIndex);
vertex_t getVerticesMidPoint(vertex_t vertex1, vertex_t vertex2);
int halfEdgeCheck(dcel_t* dcel, halfEdge_t *halfEdge, vertex_t midPoint);
void initialiseDcel(dcel_t *dcel, int initArraySize);
void printDcel(dcel_t *dcel);
void printFace(dcel_t* dcel, int faceIndex);
void printEdge(dcel_t *dcel, int edgeIndex);
void printHalfEdge(dcel_t *dcel, halfEdge_t *halfEdge);
void printVertex(dcel_t *dcel, int vertexIndex);
void printVisual(dcel_t *dcel);
void freeFace(halfEdge_t *halfEdge);
void freeDcel(dcel_t *dcel);
void setFaceIndex(dcel_t *dcel, halfEdge_t *halfEdge, int faceIndex);


