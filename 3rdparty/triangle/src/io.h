#ifndef IO_H
#define IO_H

#include "triangle.h"
#include <stdio.h>

void numbernodes(struct mesh *m, struct behavior *b);
void readholes(struct mesh *m, struct behavior *b, FILE *polyfile, char *polyfilename, REAL **hlist, int *holes, REAL **rlist, int *regions);
void transfernodes(struct mesh *m, struct behavior *b, REAL *pointlist, REAL *pointattriblist, int *pointmarkerlist, int numberofpoints, int numberofpointattribs);
void writeedges(struct mesh *m, struct behavior *b, int **edgelist, int **edgemarkerlist);
void writeelements(struct mesh *m, struct behavior *b, int **trianglelist, REAL **triangleattriblist);
void writeneighbors(struct mesh *m, struct behavior *b, int **neighborlist);
void writenodes(struct mesh *m, struct behavior *b, REAL **pointlist, REAL **pointattriblist, int **pointmarkerlist);
void writepoly(struct mesh *m, struct behavior *b, int **segmentlist, int **segmentmarkerlist);
void writevoronoi(struct mesh *m, struct behavior *b, REAL **vpointlist, REAL **vpointattriblist, int **vpointmarkerlist, int **vedgelist, int **vedgemarkerlist, REAL **vnormlist);
#endif // IO_H
