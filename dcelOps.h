/*
 *
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
    size_t maxVertices;
    size_t maxEdges;
    size_t maxFaces;
} dcel_t;

dcel_t *buildInitPolygon(char **initPolygonStringArray, dcel_t *dcel);
dcel_t* executeSplits(dcel_t *dcel, char **splitStringArray);
dcel_t* edgeSplit(dcel_t *dcel, int startEdgeIndex, int endEdgeIndex);
halfEdge_t* getHalfEdge(int startVertIndex, int endVertIndex, int edgeIndex);
vertex_t getEdgeMidPoint(dcel_t *dcel, int edgeIndex);
void initialiseDcel(dcel_t *dcel, int initArraySize);
void printDcel(dcel_t *dcel);
void printFace(dcel_t* dcel, int faceIndex);
void printEdge(dcel_t *dcel, int edgeIndex);
void printHalfEdge(dcel_t *dcel, halfEdge_t *halfEdge);
void printVertex(dcel_t *dcel, int vertexIndex);


