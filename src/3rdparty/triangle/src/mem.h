#ifndef MEM_H
#define MEM_H

#include "triangle.h"

void badsubsegdealloc(struct mesh *m, struct badsubseg *dyingseg);
struct badsubseg *badsubsegtraverse(struct mesh *m);
void dummyinit(struct mesh *m, struct behavior *b, int trianglebytes, int subsegbytes);
vertex getvertex(struct mesh *m, struct behavior *b, int number);
void initializetrisubpools(struct mesh *m, struct behavior *b);
void initializevertexpool(struct mesh *m, struct behavior *b);
void poolinit(struct memorypool *pool, int bytecount, int itemcount, int firstitemcount, int alignment);
void poolrestart(struct memorypool *pool);
void poolinit(struct memorypool *pool, int bytecount, int itemcount, int firstitemcount, int alignment);
void poolzero(struct memorypool *pool);
void pooldeinit(struct memorypool *pool);
void *poolalloc(struct memorypool *pool);
void pooldealloc(struct memorypool *pool, void *dyingitem);
void subsegdealloc(struct mesh *m, subseg *dyingsubseg);
subseg *subsegtraverse(struct mesh *m);
void traversalinit(struct memorypool *pool);
void *traverse(struct memorypool *pool);
void trifree(void *memptr);
void *trimalloc(int size);
void triangledealloc(struct mesh *m, triangle *dyingtriangle);
void triangledeinit(struct mesh *m, struct behavior *b);
triangle *triangletraverse(struct mesh *m);
void vertexdealloc(struct mesh *m, vertex dyingvertex);
vertex vertextraverse(struct mesh *m);

#endif // MEM_H
