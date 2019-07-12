#ifndef CDT_H
#define CDT_H

#include "triangle.h"

/* Fast lookup arrays to speed some of the mesh manipulation primitives.     */
extern int plus1mod3[3];
extern int minus1mod3[3];

void enforcequality(struct mesh *m, struct behavior *b);
void tallyencs(struct mesh *m, struct behavior *b);
void precisionerror();
void splitencsegs(struct mesh *m, struct behavior *b, int triflaws);
void tallyfaces(struct mesh *m, struct behavior *b);
void splittriangle(struct mesh *m, struct behavior *b, struct badtriang *badtri);
void testtriangle(struct mesh *m, struct behavior *b, struct otri *testtri);
void enqueuebadtriang(struct mesh *m, struct behavior *b,
                      struct badtriang *badtri);
struct badtriang *dequeuebadtriang(struct mesh *m);
int checkseg4encroach(struct mesh *m, struct behavior *b,
                      struct osub *testsubseg);

#endif // CDT_H
