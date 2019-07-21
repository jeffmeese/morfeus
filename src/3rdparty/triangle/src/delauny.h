#ifndef DELAUNY_H
#define DELAUNY_H

#include "triangle.h"

void alternateaxes(vertex *sortarray, int arraysize, int axis);
void boundingbox(struct mesh *m, struct behavior *b);
void carveholes(struct mesh *m, struct behavior *b, REAL *holelist, int holes, REAL *regionlist, int regions);
void check4deadevent(struct otri *checktri, struct event **freeevents, struct event **eventheap, int *heapsize);
REAL circletop(struct mesh *m, vertex pa, vertex pb, vertex pc, REAL ccwabc);
struct splaynode *circletopinsert(struct mesh *m, struct behavior *b, struct splaynode *splayroot, struct otri *newkey, vertex pa, vertex pb, vertex pc, REAL topy);
void conformingedge(struct mesh *m, struct behavior *b, vertex endpoint1, vertex endpoint2, int newmark);
void constrainededge(struct mesh *m, struct behavior *b, struct otri *starttri, vertex endpoint2, int newmark);
void createeventheap(struct mesh *m, struct event ***eventheap, struct event **events, struct event **freeevents);
ULONGLONG delaunay(struct mesh *m, struct behavior *b);
void delaunayfixup(struct mesh *m, struct behavior *b, struct otri *fixuptri, int leftside);
ULONGLONG divconqdelaunay(struct mesh *m, struct behavior *b);
void divconqrecurse(struct mesh *m, struct behavior *b, vertex *sortarray, int vertices, int axis, struct otri *farleft, struct otri *farright);
void eventheapdelete(struct event **heap, int heapsize, int eventnum);
void eventheapify(struct event **heap, int heapsize, int eventnum);
void eventheapinsert(struct event **heap, int heapsize, struct event *newevent);
enum finddirectionresult finddirection(struct mesh *m, struct behavior *b, struct otri *searchtri, vertex searchpoint);
void formskeleton(struct mesh *m, struct behavior *b, int *segmentlist, int *segmentmarkerlist, int numberofsegments);
struct splaynode *frontlocate(struct mesh *m, struct splaynode *splayroot, struct otri *bottommost, vertex searchvertex, struct otri *searchtri, int *farright);
ULONGLONG incrementaldelaunay(struct mesh *m, struct behavior *b);
void infecthull(struct mesh *m, struct behavior *b);
void insertsegment(struct mesh *m, struct behavior *b, vertex endpoint1, vertex endpoint2, int newmark);
void markhull(struct mesh *m, struct behavior *b);
void mergehulls(struct mesh *m, struct behavior *b, struct otri *farleft, struct otri *innerleft, struct otri *innerright, struct otri *farright, int axis);
void plague(struct mesh *m, struct behavior *b);
int reconstruct(struct mesh *m, struct behavior *b, int *trianglelist, REAL *triangleattriblist, REAL *trianglearealist, int elements, int corners, int attribs, int *segmentlist,int *segmentmarkerlist, int numberofsegments);
void regionplague(struct mesh *m, struct behavior *b, REAL attribute, REAL area);
ULONGLONG removebox(struct mesh *m, struct behavior *b);
ULONGLONG removeghosts(struct mesh *m, struct behavior *b, struct otri *startghost);
int rightofhyperbola(struct mesh *m, struct otri *fronttri, vertex newsite);
int scoutsegment(struct mesh *m, struct behavior *b, struct otri *searchtri, vertex endpoint2, int newmark);
void segmentintersection(struct mesh *m, struct behavior *b, struct otri *splittri, struct osub *splitsubseg, vertex endpoint2);
struct splaynode *splay(struct mesh *m, struct splaynode *splaytree, vertex searchpoint, struct otri *searchtri);
struct splaynode *splayinsert(struct mesh *m, struct splaynode *splayroot, struct otri *newkey, vertex searchpoint);
ULONGLONG sweeplinedelaunay(struct mesh *m, struct behavior *b);
void vertexmedian(vertex *sortarray, int arraysize, int median, int axis);
void vertexsort(vertex *sortarray, int arraysize);

#endif // DELAUNY_H
