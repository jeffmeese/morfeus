#ifndef REDUCED_H
#define REDUCED_H

#include "trimesh.h"

void boundingbox(struct mesh *m, struct behavior *b);
void check4deadevent(struct otri *checktri, struct event **freeevents,
                     struct event **eventheap, int *heapsize);
void checkdelaunay(struct mesh *m, struct behavior *b);
void checkmesh(struct mesh *m, struct behavior *b);
REAL circletop(struct mesh *m, vertex pa, vertex pb, vertex pc, REAL ccwabc);
void createeventheap(struct mesh *m, struct event ***eventheap,
                     struct event **events, struct event **freeevents);
void eventheapdelete(struct event **heap, int heapsize, int eventnum);
void eventheapify(struct event **heap, int heapsize, int eventnum);
void eventheapinsert(struct event **heap, int heapsize, struct event *newevent);
struct splaynode *frontlocate(struct mesh *m, struct splaynode *splayroot,
                              struct otri *bottommost, vertex searchvertex,
                              struct otri *searchtri, int *farright);
int rightofhyperbola(struct mesh *m, struct otri *fronttri, vertex newsite);
struct splaynode *splay(struct mesh *m, struct splaynode *splaytree,
                        vertex searchpoint, struct otri *searchtri);
struct splaynode *splayinsert(struct mesh *m, struct splaynode *splayroot,
                              struct otri *newkey, vertex searchpoint);
ULONGLONG incrementaldelaunay(struct mesh *m, struct behavior *b);
ULONGLONG sweeplinedelaunay(struct mesh *m, struct behavior *b);

#endif // REDUCED_H
