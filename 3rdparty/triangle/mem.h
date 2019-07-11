#ifndef MEM_H
#define MEM_H

#include "trimesh.h"

void triangledealloc(struct mesh *m, triangle *dyingtriangle);
triangle *triangletraverse(struct mesh *m);
void subsegdealloc(struct mesh *m, subseg *dyingsubseg);
subseg *subsegtraverse(struct mesh *m);
void vertexdealloc(struct mesh *m, vertex dyingvertex);
vertex vertextraverse(struct mesh *m);
void badsubsegdealloc(struct mesh *m, struct badsubseg *dyingseg);
struct badsubseg *badsubsegtraverse(struct mesh *m);
void *trimalloc(int size);
void trifree(void *memptr);

#endif // MEM_H
