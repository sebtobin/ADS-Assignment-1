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

