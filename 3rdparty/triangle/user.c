#include "user.h"

#include "primitives.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/********* User-defined triangle evaluation routine begins here      *********/
/**                                                                         **/
/**                                                                         **/
/*****************************************************************************/
/*                                                                           */
/*  triunsuitable()   Determine if a triangle is unsuitable, and thus must   */
/*                    be further refined.                                    */
/*                                                                           */
/*  You may write your own procedure that decides whether or not a selected  */
/*  triangle is too big (and needs to be refined).  There are two ways to do */
/*  this.                                                                    */
/*                                                                           */
/*  (1)  Modify the procedure `triunsuitable' below, then recompile          */
/*  Triangle.                                                                */
/*                                                                           */
/*  (2)  Define the symbol EXTERNAL_TEST (either by adding the definition    */
/*  to this file, or by using the appropriate compiler switch).  This way,   */
/*  you can compile triangle.c separately from your test.  Write your own    */
/*  `triunsuitable' procedure in a separate C file (using the same prototype */
/*  as below).  Compile it and link the object code with triangle.o.         */
/*                                                                           */
/*  This procedure returns 1 if the triangle is too large and should be      */
/*  refined; 0 otherwise.                                                    */
/*                                                                           */
/*****************************************************************************/

#ifdef EXTERNAL_TEST
int triunsuitable();
#else /* not EXTERNAL_TEST */
int triunsuitable(vertex triorg, vertex tridest, vertex triapex, REAL area)
{
  (void)area;
  REAL dxoa, dxda, dxod;
  REAL dyoa, dyda, dyod;
  REAL oalen, dalen, odlen;
  REAL maxlen;

  dxoa = triorg[0] - triapex[0];
  dyoa = triorg[1] - triapex[1];
  dxda = tridest[0] - triapex[0];
  dyda = tridest[1] - triapex[1];
  dxod = triorg[0] - tridest[0];
  dyod = triorg[1] - tridest[1];
  /* Find the squares of the lengths of the triangle's three edges. */
  oalen = dxoa * dxoa + dyoa * dyoa;
  dalen = dxda * dxda + dyda * dyda;
  odlen = dxod * dxod + dyod * dyod;
  /* Find the square of the length of the ULONGLONGest edge. */
  maxlen = (dalen > oalen) ? dalen : oalen;
  maxlen = (odlen > maxlen) ? odlen : maxlen;

  if (maxlen > 0.05 * (triorg[0] * triorg[0] + triorg[1] * triorg[1]) + 0.02) {
    return 1;
  } else {
    return 0;
  }
}

void triexit(int status)
{
  exit(status);
}

void internalerror()
{
  printf("  Please report this bug to jrs@cs.berkeley.edu\n");
  printf("  Include the message above, your input data set, and the exact\n");
  printf("    command line you used to run Triangle.\n");
  triexit(1);
}

/*****************************************************************************/
/*                                                                           */
/*  parsecommandline()   Read the command line, identify switches, and set   */
/*                       up options and file names.                          */
/*                                                                           */
/*****************************************************************************/

void parsecommandline(int argc, char **argv, struct behavior *b)
{
  int i, j, k;
  char workstring[FILENAMESIZE];

  b->poly = b->refine = b->quality = 0;
  b->vararea = b->fixedarea = b->usertest = 0;
  b->regionattrib = b->convex = b->weighted = b->jettison = 0;
  b->firstnumber = 1;
  b->edgesout = b->voronoi = b->neighbors = b->geomview = 0;
  b->nobound = b->nopolywritten = b->nonodewritten = b->noelewritten = 0;
  b->noiterationnum = 0;
  b->noholes = b->noexact = 0;
  b->incremental = b->sweepline = 0;
  b->dwyer = 1;
  b->splitseg = 0;
  b->docheck = 0;
  b->nobisect = 0;
  b->conformdel = 0;
  b->steiner = -1;
  b->order = 1;
  b->minangle = 0.0;
  b->maxarea = -1.0;
  b->quiet = b->verbose = 0;

  for (i = 0; i < argc; i++) {
    for (j = 0; argv[i][j] != '\0'; j++) {
      if (argv[i][j] == 'p') {
        b->poly = 1;
      }
      if (argv[i][j] == 'r') {
        b->refine = 1;
      }
      if (argv[i][j] == 'q') {
        b->quality = 1;
        if (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
          k = 0;
          while (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
            j++;
            workstring[k] = argv[i][j];
            k++;
          }
          workstring[k] = '\0';
          b->minangle = (REAL) strtod(workstring, (char **) NULL);
        } else {
          b->minangle = 20.0;
        }
      }
      if (argv[i][j] == 'a') {
        b->quality = 1;
        if (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
          b->fixedarea = 1;
          k = 0;
          while (((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) || (argv[i][j + 1] == '.')) {
            j++;
            workstring[k] = argv[i][j];
            k++;
          }
          workstring[k] = '\0';
          b->maxarea = (REAL) strtod(workstring, (char **) NULL);
          if (b->maxarea <= 0.0) {
            printf("Error:  Maximum area must be greater than zero.\n");
            triexit(1);
          }
        } else {
          b->vararea = 1;
        }
      }
      if (argv[i][j] == 'u') {
        b->quality = 1;
        b->usertest = 1;
      }
      if (argv[i][j] == 'A') {
        b->regionattrib = 1;
      }
      if (argv[i][j] == 'c') {
        b->convex = 1;
      }
      if (argv[i][j] == 'w') {
        b->weighted = 1;
      }
      if (argv[i][j] == 'W') {
        b->weighted = 2;
      }
      if (argv[i][j] == 'j') {
        b->jettison = 1;
      }
      if (argv[i][j] == 'z') {
        b->firstnumber = 0;
      }
      if (argv[i][j] == 'e') {
        b->edgesout = 1;
      }
      if (argv[i][j] == 'v') {
        b->voronoi = 1;
      }
      if (argv[i][j] == 'n') {
        b->neighbors = 1;
      }
      if (argv[i][j] == 'g') {
        b->geomview = 1;
      }
      if (argv[i][j] == 'B') {
        b->nobound = 1;
      }
      if (argv[i][j] == 'P') {
        b->nopolywritten = 1;
      }
      if (argv[i][j] == 'N') {
        b->nonodewritten = 1;
      }
      if (argv[i][j] == 'E') {
        b->noelewritten = 1;
      }
      if (argv[i][j] == 'O') {
        b->noholes = 1;
      }
      if (argv[i][j] == 'X') {
        b->noexact = 1;
      }
      if (argv[i][j] == 'o') {
        if (argv[i][j + 1] == '2') {
          j++;
          b->order = 2;
        }
      }
      if (argv[i][j] == 'Y') {
        b->nobisect++;
      }
      if (argv[i][j] == 'S') {
        b->steiner = 0;
        while ((argv[i][j + 1] >= '0') && (argv[i][j + 1] <= '9')) {
          j++;
          b->steiner = b->steiner * 10 + (int) (argv[i][j] - '0');
        }
      }
      if (argv[i][j] == 'i') {
        b->incremental = 1;
      }
      if (argv[i][j] == 'F') {
        b->sweepline = 1;
      }
      if (argv[i][j] == 'l') {
        b->dwyer = 0;
      }
      if (argv[i][j] == 's') {
        b->splitseg = 1;
      }
      if ((argv[i][j] == 'D') || (argv[i][j] == 'L')) {
        b->quality = 1;
        b->conformdel = 1;
      }
      if (argv[i][j] == 'C') {
        b->docheck = 1;
      }
      if (argv[i][j] == 'Q') {
        b->quiet = 1;
      }
      if (argv[i][j] == 'V') {
        b->verbose++;
      }
    }
  }
  b->usesegments = b->poly || b->refine || b->quality || b->convex;
  b->goodangle = cos(b->minangle * PI / 180.0);
  if (b->goodangle == 1.0) {
    b->offconstant = 0.0;
  } else {
    b->offconstant = 0.475 * sqrt((1.0 + b->goodangle) / (1.0 - b->goodangle));
  }
  b->goodangle *= b->goodangle;
  if (b->refine && b->noiterationnum) {
    printf(
      "Error:  You cannot use the -I switch when refining a triangulation.\n");
    triexit(1);
  }
  /* Be careful not to allocate space for element area constraints that */
  /*   will never be assigned any value (other than the default -1.0).  */
  if (!b->refine && !b->poly) {
    b->vararea = 0;
  }
  /* Be careful not to add an extra attribute to each element unless the */
  /*   input supports it (PSLG in, but not refining a preexisting mesh). */
  if (b->refine || !b->poly) {
    b->regionattrib = 0;
  }
  /* Regular/weighted triangulations are incompatible with PSLGs */
  /*   and meshing.                                              */
  if (b->weighted && (b->poly || b->quality)) {
    b->weighted = 0;
    if (!b->quiet) {
      printf("Warning:  weighted triangulations (-w, -W) are incompatible\n");
      printf("  with PSLGs (-p) and meshing (-q, -a, -u).  Weights ignored.\n"
             );
    }
  }
  if (b->jettison && b->nonodewritten && !b->quiet) {
    printf("Warning:  -j and -N switches are somewhat incompatible.\n");
    printf("  If any vertices are jettisoned, you will need the output\n");
    printf("  .node file to reconstruct the new node indices.");
  }
}

/**                                                                         **/
/**                                                                         **/
/********* User interaction routines begin here                      *********/

#endif /* not EXTERNAL_TEST */
