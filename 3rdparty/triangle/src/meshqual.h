#ifndef MESHQUAL_H
#define MESHQUAL_H

#include "triangle.h"

void checkdelaunay(struct mesh *m, struct behavior *b);
void checkmesh(struct mesh *m, struct behavior *b);
int checkseg4encroach(struct mesh *m, struct behavior *b, struct osub *testsubseg);
struct badtriang *dequeuebadtriang(struct mesh *m);
void enforcequality(struct mesh *m, struct behavior *b);
void enqueuebadtri(struct mesh *m, struct behavior *b, struct otri *enqtri, REAL minedge, vertex enqapex, vertex enqorg, vertex enqdest);
void enqueuebadtriang(struct mesh *m, struct behavior *b, struct badtriang *badtri);
void precisionerror();
void splitencsegs(struct mesh *m, struct behavior *b, int triflaws);
void splittriangle(struct mesh *m, struct behavior *b, struct badtriang *badtri);
void tallyencs(struct mesh *m, struct behavior *b);
void tallyfaces(struct mesh *m, struct behavior *b);
void testtriangle(struct mesh *m, struct behavior *b, struct otri *testtri);

#endif // MESHQUAL_H
