#ifndef MESHTRANSFORM_H
#define MESHTRANSFORM_H

#include "triangle.h"

void deletevertex(struct mesh *m, struct behavior *b, struct otri *deltri);
void flip(struct mesh *m, struct behavior *b, struct otri *flipedge);
void insertsubseg(struct mesh *m, struct behavior *b, struct otri *tri, int subsegmark);
enum insertvertexresult insertvertex(struct mesh *m, struct behavior *b,
                                     vertex newvertex, struct otri *searchtri,
                                     struct osub *splitseg,
                                     int segmentflaws, int triflaws);
void triangulatepolygon(struct mesh *m, struct behavior *b,
                        struct otri *firstedge, struct otri *lastedge,
                        int edgecount, int doflip, int triflaws);
void undovertex(struct mesh *m, struct behavior *b);
void unflip(struct mesh *m, struct behavior *b, struct otri *flipedge);
#endif // MESHTRANSFORM_H
