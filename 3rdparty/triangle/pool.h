#ifndef POOL_H
#define POOL_H

#include "triangle.h"

void poolinit(struct memorypool *pool, int bytecount, int itemcount, int firstitemcount, int alignment);
void poolrestart(struct memorypool *pool);
void poolinit(struct memorypool *pool, int bytecount, int itemcount, int firstitemcount, int alignment);
void poolzero(struct memorypool *pool);
void pooldeinit(struct memorypool *pool);
void *poolalloc(struct memorypool *pool);
void pooldealloc(struct memorypool *pool, void *dyingitem);
void traversalinit(struct memorypool *pool);
void *traverse(struct memorypool *pool);

#endif // POOL_H
