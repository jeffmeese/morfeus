#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include "triangle.h"

void makesubseg(struct mesh *m, struct osub *newsubseg);
void maketriangle(struct mesh *m, struct behavior *b, struct otri *newotri);

#endif // CONSTRUCT_H
