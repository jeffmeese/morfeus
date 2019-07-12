#ifndef DELAUNY_H
#define DELAUNY_H

#include "triangle.h"

void alternateaxes(vertex *sortarray, int arraysize, int axis);
void carveholes(struct mesh *m, struct behavior *b, REAL *holelist, int holes,
                REAL *regionlist, int regions);
void conformingedge(struct mesh *m, struct behavior *b,
                    vertex endpoint1, vertex endpoint2, int newmark);
void constrainededge(struct mesh *m, struct behavior *b,
                     struct otri *starttri, vertex endpoint2, int newmark);
ULONGLONG delaunay(struct mesh *m, struct behavior *b);
void delaunayfixup(struct mesh *m, struct behavior *b,
                   struct otri *fixuptri, int leftside);
ULONGLONG divconqdelaunay(struct mesh *m, struct behavior *b);
void divconqrecurse(struct mesh *m, struct behavior *b, vertex *sortarray,
                    int vertices, int axis,
                    struct otri *farleft, struct otri *farright);
enum finddirectionresult finddirection(struct mesh *m, struct behavior *b,
                                       struct otri *searchtri,
                                       vertex searchpoint);
void formskeleton(struct mesh *m, struct behavior *b, int *segmentlist, int *segmentmarkerlist, int numberofsegments);
void highorder(struct mesh *m, struct behavior *b);
void infecthull(struct mesh *m, struct behavior *b);
void insertsegment(struct mesh *m, struct behavior *b,
                   vertex endpoint1, vertex endpoint2, int newmark);
void markhull(struct mesh *m, struct behavior *b);
void mergehulls(struct mesh *m, struct behavior *b, struct otri *farleft,
                struct otri *innerleft, struct otri *innerright,
                struct otri *farright, int axis);
void plague(struct mesh *m, struct behavior *b);
void quality_statistics(struct mesh *m, struct behavior *b);
int reconstruct(struct mesh *m, struct behavior *b, int *trianglelist,
                REAL *triangleattriblist, REAL *trianglearealist,
                int elements, int corners, int attribs,
                int *segmentlist,int *segmentmarkerlist, int numberofsegments);
void regionplague(struct mesh *m, struct behavior *b,
                  REAL attribute, REAL area);
ULONGLONG removeghosts(struct mesh *m, struct behavior *b, struct otri *startghost);
int scoutsegment(struct mesh *m, struct behavior *b, struct otri *searchtri,
                 vertex endpoint2, int newmark);
void segmentintersection(struct mesh *m, struct behavior *b,
                         struct otri *splittri, struct osub *splitsubseg,
                         vertex endpoint2);
void statistics(struct mesh *m, struct behavior *b);
void vertexmedian(vertex *sortarray, int arraysize, int median, int axis);
void vertexsort(vertex *sortarray, int arraysize);
#endif // DELAUNY_H
