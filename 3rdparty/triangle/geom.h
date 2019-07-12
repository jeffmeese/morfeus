#ifndef GEOM_H
#define GEOM_H

#include "triangle.h"

extern REAL splitter;       /* Used to split REAL factors for exact multiplication. */
extern REAL epsilon;                             /* Floating-point machine epsilon. */
extern REAL resulterrbound;
extern REAL ccwerrboundA, ccwerrboundB, ccwerrboundC;
extern REAL iccerrboundA, iccerrboundB, iccerrboundC;
extern REAL o3derrboundA, o3derrboundB, o3derrboundC;

REAL counterclockwise(struct mesh *m, struct behavior *b, vertex pa, vertex pb, vertex pc);
REAL counterclockwiseadapt(vertex pa, vertex pb, vertex pc, REAL detsum);
REAL estimate(int elen, REAL *e);
void exactinit();
int fast_expansion_sum_zeroelim(int elen, REAL *e, int flen, REAL *f, REAL *h);
void findcircumcenter(struct mesh *m, struct behavior *b,
                      vertex torg, vertex tdest, vertex tapex,
                      vertex circumcenter, REAL *xi, REAL *eta, int offcenter);
REAL incircle(struct mesh *m, struct behavior *b,
              vertex pa, vertex pb, vertex pc, vertex pd);
REAL incircleadapt(vertex pa, vertex pb, vertex pc, vertex pd, REAL permanent);
REAL nonregular(struct mesh *m, struct behavior *b,
                vertex pa, vertex pb, vertex pc, vertex pd);
REAL orient3d(struct mesh *m, struct behavior *b,
              vertex pa, vertex pb, vertex pc, vertex pd,
              REAL aheight, REAL bheight, REAL cheight, REAL dheight);
REAL orient3dadapt(vertex pa, vertex pb, vertex pc, vertex pd,
                   REAL aheight, REAL bheight, REAL cheight, REAL dheight,
                   REAL permanent);
int scale_expansion_zeroelim(int elen, REAL *e, REAL b, REAL *h);
#endif // GEOM_H
