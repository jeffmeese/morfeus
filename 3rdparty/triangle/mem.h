#ifndef MEM_H
#define MEM_H

#include "triangle.h"

void badsubsegdealloc(struct mesh *m, struct badsubseg *dyingseg);
struct badsubseg *badsubsegtraverse(struct mesh *m);
void dummyinit(struct mesh *m, struct behavior *b, int trianglebytes, int subsegbytes);
vertex getvertex(struct mesh *m, struct behavior *b, int number);
void initializetrisubpools(struct mesh *m, struct behavior *b);
void initializevertexpool(struct mesh *m, struct behavior *b);
void subsegdealloc(struct mesh *m, subseg *dyingsubseg);
subseg *subsegtraverse(struct mesh *m);
void trifree(void *memptr);
void *trimalloc(int size);
void triangledealloc(struct mesh *m, triangle *dyingtriangle);
void triangledeinit(struct mesh *m, struct behavior *b);
triangle *triangletraverse(struct mesh *m);
void vertexdealloc(struct mesh *m, vertex dyingvertex);
vertex vertextraverse(struct mesh *m);

#endif // MEM_H
