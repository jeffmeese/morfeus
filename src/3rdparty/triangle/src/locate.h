#ifndef LOCATE_H
#define LOCATE_H

#include "triangle.h"

enum locateresult locate(struct mesh *m, struct behavior *b, vertex searchpoint, struct otri *searchtri);
void makevertexmap(struct mesh *m, struct behavior *b);
enum locateresult preciselocate(struct mesh *m, struct behavior *b, vertex searchpoint, struct otri *searchtri, int stopatsubsegment);

#endif // LOCATE_H
