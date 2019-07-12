#ifndef USER_H
#define USER_H

#include "triangle.h"

void parsecommandline(int argc, char **argv, struct behavior *b);
void internalerror();
void triexit(int status);

#ifdef EXTERNAL_TEST
int triunsuitable();
#else /* not EXTERNAL_TEST */
int triunsuitable(vertex triorg, vertex tridest, vertex triapex, REAL area);
#endif

#endif // USER_H
