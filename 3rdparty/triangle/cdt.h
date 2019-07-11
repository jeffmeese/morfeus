#ifndef CDT_H
#define CDT_H

#include "trimesh.h"

void enforcequality(struct mesh *m, struct behavior *b);
void tallyencs(struct mesh *m, struct behavior *b);
void precisionerror();
void splitencsegs(struct mesh *m, struct behavior *b, int triflaws);
void tallyfaces(struct mesh *m, struct behavior *b);
void splittriangle(struct mesh *m, struct behavior *b, struct badtriang *badtri);
void testtriangle(struct mesh *m, struct behavior *b, struct otri *testtri);

#endif // CDT_H
