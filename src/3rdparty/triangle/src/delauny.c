#include "delauny.h"

#include "construct.h"
#include "debug.h"
#include "delauny.h"
#include "geom.h"
#include "io.h"
#include "locate.h"
#include "mem.h"
#include "meshqual.h"
#include "meshtransform.h"
#include "random.h"
#include "user.h"

#include <math.h>
#include <string.h>

/********* Divide-and-conquer Delaunay triangulation begins here     *********/
/**                                                                         **/
/**                                                                         **/

void boundingbox(struct mesh *m, struct behavior *b)
{
  struct otri inftri;          /* Handle for the triangular bounding box. */
  REAL width;

  if (b->verbose) {
    printf("  Creating triangular bounding box.\n");
  }
  /* Find the width (or height, whichever is larger) of the triangulation. */
  width = m->xmax - m->xmin;
  if (m->ymax - m->ymin > width) {
    width = m->ymax - m->ymin;
  }
  if (width == 0.0) {
    width = 1.0;
  }
  /* Create the vertices of the bounding box. */
  m->infvertex1 = (vertex) trimalloc(m->vertices.itembytes);
  m->infvertex2 = (vertex) trimalloc(m->vertices.itembytes);
  m->infvertex3 = (vertex) trimalloc(m->vertices.itembytes);
  m->infvertex1[0] = m->xmin - 50.0 * width;
  m->infvertex1[1] = m->ymin - 40.0 * width;
  m->infvertex2[0] = m->xmax + 50.0 * width;
  m->infvertex2[1] = m->ymin - 40.0 * width;
  m->infvertex3[0] = 0.5 * (m->xmin + m->xmax);
  m->infvertex3[1] = m->ymax + 60.0 * width;

  /* Create the bounding box. */
  maketriangle(m, b, &inftri);
  setorg(inftri, m->infvertex1);
  setdest(inftri, m->infvertex2);
  setapex(inftri, m->infvertex3);
  /* Link dummytri to the bounding box so we can always find an */
  /*   edge to begin searching (point location) from.           */
  m->dummytri[0] = (triangle) inftri.tri;
  if (b->verbose > 2) {
    printf("  Creating ");
    printtriangle(m, b, &inftri);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  The divide-and-conquer bounding box                                      */
/*                                                                           */
/*  I originally implemented the divide-and-conquer and incremental Delaunay */
/*  triangulations using the edge-based data structure presented by Guibas   */
/*  and Stolfi.  Switching to a triangle-based data structure doubled the    */
/*  speed.  However, I had to think of a few extra tricks to maintain the    */
/*  elegance of the original algorithms.                                     */
/*                                                                           */
/*  The "bounding box" used by my variant of the divide-and-conquer          */
/*  algorithm uses one triangle for each edge of the convex hull of the      */
/*  triangulation.  These bounding triangles all share a common apical       */
/*  vertex, which is represented by NULL and which represents nothing.       */
/*  The bounding triangles are linked in a circular fan about this NULL      */
/*  vertex, and the edges on the convex hull of the triangulation appear     */
/*  opposite the NULL vertex.  You might find it easiest to imagine that     */
/*  the NULL vertex is a point in 3D space behind the center of the          */
/*  triangulation, and that the bounding triangles form a sort of cone.      */
/*                                                                           */
/*  This bounding box makes it easy to represent degenerate cases.  For      */
/*  instance, the triangulation of two vertices is a single edge.  This edge */
/*  is represented by two bounding box triangles, one on each "side" of the  */
/*  edge.  These triangles are also linked together in a fan about the NULL  */
/*  vertex.                                                                  */
/*                                                                           */
/*  The bounding box also makes it easy to traverse the convex hull, as the  */
/*  divide-and-conquer algorithm needs to do.                                */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  vertexsort()   Sort an array of vertices by x-coordinate, using the      */
/*                 y-coordinate as a secondary key.                          */
/*                                                                           */
/*  Uses quicksort.  Randomized O(n log n) time.  No, I did not make any of  */
/*  the usual quicksort mistakes.                                            */
/*                                                                           */
/*****************************************************************************/

void vertexsort(vertex *sortarray, int arraysize)
{
  int left, right;
  int pivot;
  REAL pivotx, pivoty;
  vertex temp;

  if (arraysize == 2) {
    /* Recursive base case. */
    if ((sortarray[0][0] > sortarray[1][0]) ||
        ((sortarray[0][0] == sortarray[1][0]) &&
         (sortarray[0][1] > sortarray[1][1]))) {
      temp = sortarray[1];
      sortarray[1] = sortarray[0];
      sortarray[0] = temp;
    }
    return;
  }
  /* Choose a random pivot to split the array. */
  pivot = (int) randomnation((unsigned int) arraysize);
  pivotx = sortarray[pivot][0];
  pivoty = sortarray[pivot][1];
  /* Split the array. */
  left = -1;
  right = arraysize;
  while (left < right) {
    /* Search for a vertex whose x-coordinate is too large for the left. */
    do {
      left++;
    } while ((left <= right) && ((sortarray[left][0] < pivotx) ||
                                 ((sortarray[left][0] == pivotx) &&
                                  (sortarray[left][1] < pivoty))));
    /* Search for a vertex whose x-coordinate is too small for the right. */
    do {
      right--;
    } while ((left <= right) && ((sortarray[right][0] > pivotx) ||
                                 ((sortarray[right][0] == pivotx) &&
                                  (sortarray[right][1] > pivoty))));
    if (left < right) {
      /* Swap the left and right vertices. */
      temp = sortarray[left];
      sortarray[left] = sortarray[right];
      sortarray[right] = temp;
    }
  }
  if (left > 1) {
    /* Recursively sort the left subset. */
    vertexsort(sortarray, left);
  }
  if (right < arraysize - 2) {
    /* Recursively sort the right subset. */
    vertexsort(&sortarray[right + 1], arraysize - right - 1);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  vertexmedian()   An order statistic algorithm, almost.  Shuffles an      */
/*                   array of vertices so that the first `median' vertices   */
/*                   occur lexicographically before the remaining vertices.  */
/*                                                                           */
/*  Uses the x-coordinate as the primary key if axis == 0; the y-coordinate  */
/*  if axis == 1.  Very similar to the vertexsort() procedure, but runs in   */
/*  randomized linear time.                                                  */
/*                                                                           */
/*****************************************************************************/

void vertexmedian(vertex *sortarray, int arraysize, int median, int axis)
{
  int left, right;
  int pivot;
  REAL pivot1, pivot2;
  vertex temp;

  if (arraysize == 2) {
    /* Recursive base case. */
    if ((sortarray[0][axis] > sortarray[1][axis]) ||
        ((sortarray[0][axis] == sortarray[1][axis]) &&
         (sortarray[0][1 - axis] > sortarray[1][1 - axis]))) {
      temp = sortarray[1];
      sortarray[1] = sortarray[0];
      sortarray[0] = temp;
    }
    return;
  }
  /* Choose a random pivot to split the array. */
  pivot = (int) randomnation((unsigned int) arraysize);
  pivot1 = sortarray[pivot][axis];
  pivot2 = sortarray[pivot][1 - axis];
  /* Split the array. */
  left = -1;
  right = arraysize;
  while (left < right) {
    /* Search for a vertex whose x-coordinate is too large for the left. */
    do {
      left++;
    } while ((left <= right) && ((sortarray[left][axis] < pivot1) ||
                                 ((sortarray[left][axis] == pivot1) &&
                                  (sortarray[left][1 - axis] < pivot2))));
    /* Search for a vertex whose x-coordinate is too small for the right. */
    do {
      right--;
    } while ((left <= right) && ((sortarray[right][axis] > pivot1) ||
                                 ((sortarray[right][axis] == pivot1) &&
                                  (sortarray[right][1 - axis] > pivot2))));
    if (left < right) {
      /* Swap the left and right vertices. */
      temp = sortarray[left];
      sortarray[left] = sortarray[right];
      sortarray[right] = temp;
    }
  }
  /* Unlike in vertexsort(), at most one of the following */
  /*   conditionals is true.                             */
  if (left > median) {
    /* Recursively shuffle the left subset. */
    vertexmedian(sortarray, left, median, axis);
  }
  if (right < median - 1) {
    /* Recursively shuffle the right subset. */
    vertexmedian(&sortarray[right + 1], arraysize - right - 1,
                 median - right - 1, axis);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  alternateaxes()   Sorts the vertices as appropriate for the divide-and-  */
/*                    conquer algorithm with alternating cuts.               */
/*                                                                           */
/*  Partitions by x-coordinate if axis == 0; by y-coordinate if axis == 1.   */
/*  For the base case, subsets containing only two or three vertices are     */
/*  always sorted by x-coordinate.                                           */
/*                                                                           */
/*****************************************************************************/

void alternateaxes(vertex *sortarray, int arraysize, int axis)
{
  int divider;

  divider = arraysize >> 1;
  if (arraysize <= 3) {
    /* Recursive base case:  subsets of two or three vertices will be    */
    /*   handled specially, and should always be sorted by x-coordinate. */
    axis = 0;
  }
  /* Partition with a horizontal or vertical cut. */
  vertexmedian(sortarray, arraysize, divider, axis);
  /* Recursively partition the subsets with a cross cut. */
  if (arraysize - divider >= 2) {
    if (divider >= 2) {
      alternateaxes(sortarray, divider, 1 - axis);
    }
    alternateaxes(&sortarray[divider], arraysize - divider, 1 - axis);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  mergehulls()   Merge two adjacent Delaunay triangulations into a         */
/*                 single Delaunay triangulation.                            */
/*                                                                           */
/*  This is similar to the algorithm given by Guibas and Stolfi, but uses    */
/*  a triangle-based, rather than edge-based, data structure.                */
/*                                                                           */
/*  The algorithm walks up the gap between the two triangulations, knitting  */
/*  them together.  As they are merged, some of their bounding triangles     */
/*  are converted into real triangles of the triangulation.  The procedure   */
/*  pulls each hull's bounding triangles apart, then knits them together     */
/*  like the teeth of two gears.  The Delaunay property determines, at each  */
/*  step, whether the next "tooth" is a bounding triangle of the left hull   */
/*  or the right.  When a bounding triangle becomes real, its apex is        */
/*  changed from NULL to a real vertex.                                      */
/*                                                                           */
/*  Only two new triangles need to be allocated.  These become new bounding  */
/*  triangles at the top and bottom of the seam.  They are used to connect   */
/*  the remaining bounding triangles (those that have not been converted     */
/*  into real triangles) into a single fan.                                  */
/*                                                                           */
/*  On entry, `farleft' and `innerleft' are bounding triangles of the left   */
/*  triangulation.  The origin of `farleft' is the leftmost vertex, and      */
/*  the destination of `innerleft' is the rightmost vertex of the            */
/*  triangulation.  Similarly, `innerright' and `farright' are bounding      */
/*  triangles of the right triangulation.  The origin of `innerright' and    */
/*  destination of `farright' are the leftmost and rightmost vertices.       */
/*                                                                           */
/*  On completion, the origin of `farleft' is the leftmost vertex of the     */
/*  merged triangulation, and the destination of `farright' is the rightmost */
/*  vertex.                                                                  */
/*                                                                           */
/*****************************************************************************/

void mergehulls(struct mesh *m, struct behavior *b, struct otri *farleft,
                struct otri *innerleft, struct otri *innerright,
                struct otri *farright, int axis)
{
  struct otri leftcand, rightcand;
  struct otri baseedge;
  struct otri nextedge;
  struct otri sidecasing, topcasing, outercasing;
  struct otri checkedge;
  vertex innerleftdest;
  vertex innerrightorg;
  vertex innerleftapex, innerrightapex;
  vertex farleftpt, farrightpt;
  vertex farleftapex, farrightapex;
  vertex lowerleft, lowerright;
  vertex upperleft, upperright;
  vertex nextapex;
  vertex checkvertex;
  int changemade;
  int badedge;
  int leftfinished, rightfinished;
  triangle ptr;                         /* Temporary variable used by sym(). */

  dest(*innerleft, innerleftdest);
  apex(*innerleft, innerleftapex);
  org(*innerright, innerrightorg);
  apex(*innerright, innerrightapex);
  /* Special treatment for horizontal cuts. */
  if (b->dwyer && (axis == 1)) {
    org(*farleft, farleftpt);
    apex(*farleft, farleftapex);
    dest(*farright, farrightpt);
    apex(*farright, farrightapex);
    /* The pointers to the extremal vertices are shifted to point to the */
    /*   topmost and bottommost vertex of each hull, rather than the     */
    /*   leftmost and rightmost vertices.                                */
    while (farleftapex[1] < farleftpt[1]) {
      lnextself(*farleft);
      symself(*farleft);
      farleftpt = farleftapex;
      apex(*farleft, farleftapex);
    }
    sym(*innerleft, checkedge);
    apex(checkedge, checkvertex);
    while (checkvertex[1] > innerleftdest[1]) {
      lnext(checkedge, *innerleft);
      innerleftapex = innerleftdest;
      innerleftdest = checkvertex;
      sym(*innerleft, checkedge);
      apex(checkedge, checkvertex);
    }
    while (innerrightapex[1] < innerrightorg[1]) {
      lnextself(*innerright);
      symself(*innerright);
      innerrightorg = innerrightapex;
      apex(*innerright, innerrightapex);
    }
    sym(*farright, checkedge);
    apex(checkedge, checkvertex);
    while (checkvertex[1] > farrightpt[1]) {
      lnext(checkedge, *farright);
      farrightapex = farrightpt;
      farrightpt = checkvertex;
      sym(*farright, checkedge);
      apex(checkedge, checkvertex);
    }
  }
  /* Find a line tangent to and below both hulls. */
  do {
    changemade = 0;
    /* Make innerleftdest the "bottommost" vertex of the left hull. */
    if (counterclockwise(m, b, innerleftdest, innerleftapex, innerrightorg) >
        0.0) {
      lprevself(*innerleft);
      symself(*innerleft);
      innerleftdest = innerleftapex;
      apex(*innerleft, innerleftapex);
      changemade = 1;
    }
    /* Make innerrightorg the "bottommost" vertex of the right hull. */
    if (counterclockwise(m, b, innerrightapex, innerrightorg, innerleftdest) >
        0.0) {
      lnextself(*innerright);
      symself(*innerright);
      innerrightorg = innerrightapex;
      apex(*innerright, innerrightapex);
      changemade = 1;
    }
  } while (changemade);
  /* Find the two candidates to be the next "gear tooth." */
  sym(*innerleft, leftcand);
  sym(*innerright, rightcand);
  /* Create the bottom new bounding triangle. */
  maketriangle(m, b, &baseedge);
  /* Connect it to the bounding boxes of the left and right triangulations. */
  bond(baseedge, *innerleft);
  lnextself(baseedge);
  bond(baseedge, *innerright);
  lnextself(baseedge);
  setorg(baseedge, innerrightorg);
  setdest(baseedge, innerleftdest);
  /* Apex is intentionally left NULL. */
  if (b->verbose > 2) {
    printf("  Creating base bounding ");
    printtriangle(m, b, &baseedge);
  }
  /* Fix the extreme triangles if necessary. */
  org(*farleft, farleftpt);
  if (innerleftdest == farleftpt) {
    lnext(baseedge, *farleft);
  }
  dest(*farright, farrightpt);
  if (innerrightorg == farrightpt) {
    lprev(baseedge, *farright);
  }
  /* The vertices of the current knitting edge. */
  lowerleft = innerleftdest;
  lowerright = innerrightorg;
  /* The candidate vertices for knitting. */
  apex(leftcand, upperleft);
  apex(rightcand, upperright);
  /* Walk up the gap between the two triangulations, knitting them together. */
  while (1) {
    /* Have we reached the top?  (This isn't quite the right question,       */
    /*   because even though the left triangulation might seem finished now, */
    /*   moving up on the right triangulation might reveal a new vertex of   */
    /*   the left triangulation.  And vice-versa.)                           */
    leftfinished = counterclockwise(m, b, upperleft, lowerleft, lowerright) <=
                   0.0;
    rightfinished = counterclockwise(m, b, upperright, lowerleft, lowerright)
                 <= 0.0;
    if (leftfinished && rightfinished) {
      /* Create the top new bounding triangle. */
      maketriangle(m, b, &nextedge);
      setorg(nextedge, lowerleft);
      setdest(nextedge, lowerright);
      /* Apex is intentionally left NULL. */
      /* Connect it to the bounding boxes of the two triangulations. */
      bond(nextedge, baseedge);
      lnextself(nextedge);
      bond(nextedge, rightcand);
      lnextself(nextedge);
      bond(nextedge, leftcand);
      if (b->verbose > 2) {
        printf("  Creating top bounding ");
        printtriangle(m, b, &nextedge);
      }
      /* Special treatment for horizontal cuts. */
      if (b->dwyer && (axis == 1)) {
        org(*farleft, farleftpt);
        apex(*farleft, farleftapex);
        dest(*farright, farrightpt);
        apex(*farright, farrightapex);
        sym(*farleft, checkedge);
        apex(checkedge, checkvertex);
        /* The pointers to the extremal vertices are restored to the  */
        /*   leftmost and rightmost vertices (rather than topmost and */
        /*   bottommost).                                             */
        while (checkvertex[0] < farleftpt[0]) {
          lprev(checkedge, *farleft);
          farleftapex = farleftpt;
          farleftpt = checkvertex;
          sym(*farleft, checkedge);
          apex(checkedge, checkvertex);
        }
        while (farrightapex[0] > farrightpt[0]) {
          lprevself(*farright);
          symself(*farright);
          farrightpt = farrightapex;
          apex(*farright, farrightapex);
        }
      }
      return;
    }
    /* Consider eliminating edges from the left triangulation. */
    if (!leftfinished) {
      /* What vertex would be exposed if an edge were deleted? */
      lprev(leftcand, nextedge);
      symself(nextedge);
      apex(nextedge, nextapex);
      /* If nextapex is NULL, then no vertex would be exposed; the */
      /*   triangulation would have been eaten right through.      */
      if (nextapex != (vertex) NULL) {
        /* Check whether the edge is Delaunay. */
        badedge = incircle(m, b, lowerleft, lowerright, upperleft, nextapex) >
                  0.0;
        while (badedge) {
          /* Eliminate the edge with an edge flip.  As a result, the    */
          /*   left triangulation will have one more boundary triangle. */
          lnextself(nextedge);
          sym(nextedge, topcasing);
          lnextself(nextedge);
          sym(nextedge, sidecasing);
          bond(nextedge, topcasing);
          bond(leftcand, sidecasing);
          lnextself(leftcand);
          sym(leftcand, outercasing);
          lprevself(nextedge);
          bond(nextedge, outercasing);
          /* Correct the vertices to reflect the edge flip. */
          setorg(leftcand, lowerleft);
          setdest(leftcand, NULL);
          setapex(leftcand, nextapex);
          setorg(nextedge, NULL);
          setdest(nextedge, upperleft);
          setapex(nextedge, nextapex);
          /* Consider the newly exposed vertex. */
          upperleft = nextapex;
          /* What vertex would be exposed if another edge were deleted? */
          otricopy(sidecasing, nextedge);
          apex(nextedge, nextapex);
          if (nextapex != (vertex) NULL) {
            /* Check whether the edge is Delaunay. */
            badedge = incircle(m, b, lowerleft, lowerright, upperleft,
                               nextapex) > 0.0;
          } else {
            /* Avoid eating right through the triangulation. */
            badedge = 0;
          }
        }
      }
    }
    /* Consider eliminating edges from the right triangulation. */
    if (!rightfinished) {
      /* What vertex would be exposed if an edge were deleted? */
      lnext(rightcand, nextedge);
      symself(nextedge);
      apex(nextedge, nextapex);
      /* If nextapex is NULL, then no vertex would be exposed; the */
      /*   triangulation would have been eaten right through.      */
      if (nextapex != (vertex) NULL) {
        /* Check whether the edge is Delaunay. */
        badedge = incircle(m, b, lowerleft, lowerright, upperright, nextapex) >
                  0.0;
        while (badedge) {
          /* Eliminate the edge with an edge flip.  As a result, the     */
          /*   right triangulation will have one more boundary triangle. */
          lprevself(nextedge);
          sym(nextedge, topcasing);
          lprevself(nextedge);
          sym(nextedge, sidecasing);
          bond(nextedge, topcasing);
          bond(rightcand, sidecasing);
          lprevself(rightcand);
          sym(rightcand, outercasing);
          lnextself(nextedge);
          bond(nextedge, outercasing);
          /* Correct the vertices to reflect the edge flip. */
          setorg(rightcand, NULL);
          setdest(rightcand, lowerright);
          setapex(rightcand, nextapex);
          setorg(nextedge, upperright);
          setdest(nextedge, NULL);
          setapex(nextedge, nextapex);
          /* Consider the newly exposed vertex. */
          upperright = nextapex;
          /* What vertex would be exposed if another edge were deleted? */
          otricopy(sidecasing, nextedge);
          apex(nextedge, nextapex);
          if (nextapex != (vertex) NULL) {
            /* Check whether the edge is Delaunay. */
            badedge = incircle(m, b, lowerleft, lowerright, upperright,
                               nextapex) > 0.0;
          } else {
            /* Avoid eating right through the triangulation. */
            badedge = 0;
          }
        }
      }
    }
    if (leftfinished || (!rightfinished &&
           (incircle(m, b, upperleft, lowerleft, lowerright, upperright) >
            0.0))) {
      /* Knit the triangulations, adding an edge from `lowerleft' */
      /*   to `upperright'.                                       */
      bond(baseedge, rightcand);
      lprev(rightcand, baseedge);
      setdest(baseedge, lowerleft);
      lowerright = upperright;
      sym(baseedge, rightcand);
      apex(rightcand, upperright);
    } else {
      /* Knit the triangulations, adding an edge from `upperleft' */
      /*   to `lowerright'.                                       */
      bond(baseedge, leftcand);
      lnext(leftcand, baseedge);
      setorg(baseedge, lowerright);
      lowerleft = upperleft;
      sym(baseedge, leftcand);
      apex(leftcand, upperleft);
    }
    if (b->verbose > 2) {
      printf("  Connecting ");
      printtriangle(m, b, &baseedge);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  divconqrecurse()   Recursively form a Delaunay triangulation by the      */
/*                     divide-and-conquer method.                            */
/*                                                                           */
/*  Recursively breaks down the problem into smaller pieces, which are       */
/*  knitted together by mergehulls().  The base cases (problems of two or    */
/*  three vertices) are handled specially here.                              */
/*                                                                           */
/*  On completion, `farleft' and `farright' are bounding triangles such that */
/*  the origin of `farleft' is the leftmost vertex (breaking ties by         */
/*  choosing the highest leftmost vertex), and the destination of            */
/*  `farright' is the rightmost vertex (breaking ties by choosing the        */
/*  lowest rightmost vertex).                                                */
/*                                                                           */
/*****************************************************************************/

void divconqrecurse(struct mesh *m, struct behavior *b, vertex *sortarray,
                    int vertices, int axis,
                    struct otri *farleft, struct otri *farright)
{
  struct otri midtri, tri1, tri2, tri3;
  struct otri innerleft, innerright;
  REAL area;
  int divider;

  if (b->verbose > 2) {
    printf("  Triangulating %d vertices.\n", vertices);
  }
  if (vertices == 2) {
    /* The triangulation of two vertices is an edge.  An edge is */
    /*   represented by two bounding triangles.                  */
    maketriangle(m, b, farleft);
    setorg(*farleft, sortarray[0]);
    setdest(*farleft, sortarray[1]);
    /* The apex is intentionally left NULL. */
    maketriangle(m, b, farright);
    setorg(*farright, sortarray[1]);
    setdest(*farright, sortarray[0]);
    /* The apex is intentionally left NULL. */
    bond(*farleft, *farright);
    lprevself(*farleft);
    lnextself(*farright);
    bond(*farleft, *farright);
    lprevself(*farleft);
    lnextself(*farright);
    bond(*farleft, *farright);
    if (b->verbose > 2) {
      printf("  Creating ");
      printtriangle(m, b, farleft);
      printf("  Creating ");
      printtriangle(m, b, farright);
    }
    /* Ensure that the origin of `farleft' is sortarray[0]. */
    lprev(*farright, *farleft);
    return;
  } else if (vertices == 3) {
    /* The triangulation of three vertices is either a triangle (with */
    /*   three bounding triangles) or two edges (with four bounding   */
    /*   triangles).  In either case, four triangles are created.     */
    maketriangle(m, b, &midtri);
    maketriangle(m, b, &tri1);
    maketriangle(m, b, &tri2);
    maketriangle(m, b, &tri3);
    area = counterclockwise(m, b, sortarray[0], sortarray[1], sortarray[2]);
    if (area == 0.0) {
      /* Three collinear vertices; the triangulation is two edges. */
      setorg(midtri, sortarray[0]);
      setdest(midtri, sortarray[1]);
      setorg(tri1, sortarray[1]);
      setdest(tri1, sortarray[0]);
      setorg(tri2, sortarray[2]);
      setdest(tri2, sortarray[1]);
      setorg(tri3, sortarray[1]);
      setdest(tri3, sortarray[2]);
      /* All apices are intentionally left NULL. */
      bond(midtri, tri1);
      bond(tri2, tri3);
      lnextself(midtri);
      lprevself(tri1);
      lnextself(tri2);
      lprevself(tri3);
      bond(midtri, tri3);
      bond(tri1, tri2);
      lnextself(midtri);
      lprevself(tri1);
      lnextself(tri2);
      lprevself(tri3);
      bond(midtri, tri1);
      bond(tri2, tri3);
      /* Ensure that the origin of `farleft' is sortarray[0]. */
      otricopy(tri1, *farleft);
      /* Ensure that the destination of `farright' is sortarray[2]. */
      otricopy(tri2, *farright);
    } else {
      /* The three vertices are not collinear; the triangulation is one */
      /*   triangle, namely `midtri'.                                   */
      setorg(midtri, sortarray[0]);
      setdest(tri1, sortarray[0]);
      setorg(tri3, sortarray[0]);
      /* Apices of tri1, tri2, and tri3 are left NULL. */
      if (area > 0.0) {
        /* The vertices are in counterclockwise order. */
        setdest(midtri, sortarray[1]);
        setorg(tri1, sortarray[1]);
        setdest(tri2, sortarray[1]);
        setapex(midtri, sortarray[2]);
        setorg(tri2, sortarray[2]);
        setdest(tri3, sortarray[2]);
      } else {
        /* The vertices are in clockwise order. */
        setdest(midtri, sortarray[2]);
        setorg(tri1, sortarray[2]);
        setdest(tri2, sortarray[2]);
        setapex(midtri, sortarray[1]);
        setorg(tri2, sortarray[1]);
        setdest(tri3, sortarray[1]);
      }
      /* The topology does not depend on how the vertices are ordered. */
      bond(midtri, tri1);
      lnextself(midtri);
      bond(midtri, tri2);
      lnextself(midtri);
      bond(midtri, tri3);
      lprevself(tri1);
      lnextself(tri2);
      bond(tri1, tri2);
      lprevself(tri1);
      lprevself(tri3);
      bond(tri1, tri3);
      lnextself(tri2);
      lprevself(tri3);
      bond(tri2, tri3);
      /* Ensure that the origin of `farleft' is sortarray[0]. */
      otricopy(tri1, *farleft);
      /* Ensure that the destination of `farright' is sortarray[2]. */
      if (area > 0.0) {
        otricopy(tri2, *farright);
      } else {
        lnext(*farleft, *farright);
      }
    }
    if (b->verbose > 2) {
      printf("  Creating ");
      printtriangle(m, b, &midtri);
      printf("  Creating ");
      printtriangle(m, b, &tri1);
      printf("  Creating ");
      printtriangle(m, b, &tri2);
      printf("  Creating ");
      printtriangle(m, b, &tri3);
    }
    return;
  } else {
    /* Split the vertices in half. */
    divider = vertices >> 1;
    /* Recursively triangulate each half. */
    divconqrecurse(m, b, sortarray, divider, 1 - axis, farleft, &innerleft);
    divconqrecurse(m, b, &sortarray[divider], vertices - divider, 1 - axis,
                   &innerright, farright);
    if (b->verbose > 1) {
      printf("  Joining triangulations with %d and %d vertices.\n", divider,
             vertices - divider);
    }
    /* Merge the two triangulations into one. */
    mergehulls(m, b, farleft, &innerleft, &innerright, farright, axis);
  }
}

ULONGLONG removeghosts(struct mesh *m, struct behavior *b, struct otri *startghost)
{
  struct otri searchedge;
  struct otri dissolveedge;
  struct otri deadtriangle;
  vertex markorg;
  ULONGLONG hullsize;
  triangle ptr;                         /* Temporary variable used by sym(). */

  if (b->verbose) {
    printf("  Removing ghost triangles.\n");
  }
  /* Find an edge on the convex hull to start point location from. */
  lprev(*startghost, searchedge);
  symself(searchedge);
  m->dummytri[0] = encode(searchedge);
  /* Remove the bounding box and count the convex hull edges. */
  otricopy(*startghost, dissolveedge);
  hullsize = 0;
  do {
    hullsize++;
    lnext(dissolveedge, deadtriangle);
    lprevself(dissolveedge);
    symself(dissolveedge);
    /* If no PSLG is involved, set the boundary markers of all the vertices */
    /*   on the convex hull.  If a PSLG is used, this step is done later.   */
    if (!b->poly) {
      /* Watch out for the case where all the input vertices are collinear. */
      if (dissolveedge.tri != m->dummytri) {
        org(dissolveedge, markorg);
        if (vertexmark(markorg) == 0) {
          setvertexmark(markorg, 1);
        }
      }
    }
    /* Remove a bounding triangle from a convex hull triangle. */
    dissolve(dissolveedge);
    /* Find the next bounding triangle. */
    sym(deadtriangle, dissolveedge);
    /* Delete the bounding triangle. */
    triangledealloc(m, deadtriangle.tri);
  } while (!otriequal(dissolveedge, *startghost));
  return hullsize;
}

/*****************************************************************************/
/*                                                                           */
/*  divconqdelaunay()   Form a Delaunay triangulation by the divide-and-     */
/*                      conquer method.                                      */
/*                                                                           */
/*  Sorts the vertices, calls a recursive procedure to triangulate them, and */
/*  removes the bounding box, setting boundary markers as appropriate.       */
/*                                                                           */
/*****************************************************************************/

ULONGLONG divconqdelaunay(struct mesh *m, struct behavior *b)
{
  vertex *sortarray;
  struct otri hullleft, hullright;
  int divider;
  int i, j;

  if (b->verbose) {
    printf("  Sorting vertices.\n");
  }

  /* Allocate an array of pointers to vertices for sorting. */
  sortarray = (vertex *) trimalloc(m->invertices * (int) sizeof(vertex));
  traversalinit(&m->vertices);
  for (i = 0; i < m->invertices; i++) {
    sortarray[i] = vertextraverse(m);
  }
  /* Sort the vertices. */
  vertexsort(sortarray, m->invertices);
  /* Discard duplicate vertices, which can really mess up the algorithm. */
  i = 0;
  for (j = 1; j < m->invertices; j++) {
    if ((sortarray[i][0] == sortarray[j][0])
        && (sortarray[i][1] == sortarray[j][1])) {
      if (!b->quiet) {
        printf(
"Warning:  A duplicate vertex at (%.12g, %.12g) appeared and was ignored.\n",
               sortarray[j][0], sortarray[j][1]);
      }
      setvertextype(sortarray[j], UNDEADVERTEX);
      m->undeads++;
    } else {
      i++;
      sortarray[i] = sortarray[j];
    }
  }
  i++;
  if (b->dwyer) {
    /* Re-sort the array of vertices to accommodate alternating cuts. */
    divider = i >> 1;
    if (i - divider >= 2) {
      if (divider >= 2) {
        alternateaxes(sortarray, divider, 1);
      }
      alternateaxes(&sortarray[divider], i - divider, 1);
    }
  }

  if (b->verbose) {
    printf("  Forming triangulation.\n");
  }

  /* Form the Delaunay triangulation. */
  divconqrecurse(m, b, sortarray, i, 0, &hullleft, &hullright);
  trifree((void *) sortarray);

  return removeghosts(m, b, &hullleft);
}

/**                                                                         **/
/**                                                                         **/
/********* Divide-and-conquer Delaunay triangulation ends here       *********/

/********* Incremental Delaunay triangulation begins here            *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  boundingbox()   Form an "infinite" bounding triangle to insert vertices  */
/*                  into.                                                    */
/*                                                                           */
/*  The vertices at "infinity" are assigned finite coordinates, which are    */
/*  used by the point location routines, but (mostly) ignored by the         */
/*  Delaunay edge flip routines.                                             */
/*                                                                           */
/*****************************************************************************/


/**                                                                         **/
/**                                                                         **/
/********* Sweepline Delaunay triangulation ends here                *********/

/********* General mesh construction routines begin here             *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  delaunay()   Form a Delaunay triangulation.                              */
/*                                                                           */
/*****************************************************************************/

ULONGLONG delaunay(struct mesh *m, struct behavior *b)
{
  ULONGLONG hulledges;

  m->eextras = 0;
  initializetrisubpools(m, b);

  if (!b->quiet) {
    printf("Constructing Delaunay triangulation ");
    if (b->incremental) {
      printf("by incremental method.\n");
    } else if (b->sweepline) {
      printf("by sweepline method.\n");
    } else {
      printf("by divide-and-conquer method.\n");
    }
  }
  if (b->incremental) {
    hulledges = incrementaldelaunay(m, b);
  } else if (b->sweepline) {
    hulledges = sweeplinedelaunay(m, b);
  } else {
    hulledges = divconqdelaunay(m, b);
  }

  if (m->triangles.items == 0) {
    /* The input vertices were all collinear, so there are no triangles. */
    return 0l;
  } else {
    return hulledges;
  }
}

/*****************************************************************************/
/*                                                                           */
/*  reconstruct()   Reconstruct a triangulation from its .ele (and possibly  */
/*                  .poly) file.  Used when the -r switch is used.           */
/*                                                                           */
/*  Reads an .ele file and reconstructs the original mesh.  If the -p switch */
/*  is used, this procedure will also read a .poly file and reconstruct the  */
/*  subsegments of the original mesh.  If the -a switch is used, this        */
/*  procedure will also read an .area file and set a maximum area constraint */
/*  on each triangle.                                                        */
/*                                                                           */
/*  Vertices that are not corners of triangles, such as nodes on edges of    */
/*  subparametric elements, are discarded.                                   */
/*                                                                           */
/*  This routine finds the adjacencies between triangles (and subsegments)   */
/*  by forming one stack of triangles for each vertex.  Each triangle is on  */
/*  three different stacks simultaneously.  Each triangle's subsegment       */
/*  pointers are used to link the items in each stack.  This memory-saving   */
/*  feature makes the code harder to read.  The most important thing to keep */
/*  in mind is that each triangle is removed from a stack precisely when     */
/*  the corresponding pointer is adjusted to refer to a subsegment rather    */
/*  than the next triangle of the stack.                                     */
/*                                                                           */
/*****************************************************************************/
int reconstruct(struct mesh *m, struct behavior *b, int *trianglelist,
                REAL *triangleattriblist, REAL *trianglearealist,
                int elements, int corners, int attribs,
                int *segmentlist,int *segmentmarkerlist, int numberofsegments)
{
  int vertexindex;
  int attribindex;
  struct otri triangleloop;
  struct otri triangleleft;
  struct otri checktri;
  struct otri checkleft;
  struct otri checkneighbor;
  struct osub subsegloop;
  triangle *vertexarray;
  triangle *prevlink;
  triangle nexttri;
  vertex tdest, tapex;
  vertex checkdest, checkapex;
  vertex shorg;
  vertex killvertex;
  vertex segmentorg, segmentdest;
  REAL area;
  int corner[3];
  int end[2];
  int killvertexindex;
  int incorners;
  int segmentmarkers;
  int boundmarker;
  int aroundvertex;
  ULONGLONG hullsize;
  int notfound;
  ULONGLONG elementnumber, segmentnumber;
  int i, j;
  triangle ptr;                         /* Temporary variable used by sym(). */

  m->inelements = elements;
  incorners = corners;
  if (incorners < 3) {
    printf("Error:  Triangles must have at least 3 vertices.\n");
    triexit(1);
  }
  m->eextras = attribs;

  initializetrisubpools(m, b);

  /* Create the triangles. */
  for (elementnumber = 1; elementnumber <= m->inelements; elementnumber++) {
    maketriangle(m, b, &triangleloop);
    /* Mark the triangle as living. */
    triangleloop.tri[3] = (triangle) triangleloop.tri;
  }

  segmentmarkers = 0;
  if (b->poly) {
    m->insegments = numberofsegments;
    segmentmarkers = segmentmarkerlist != (int *) NULL;

    /* Create the subsegments. */
    for (segmentnumber = 1; segmentnumber <= m->insegments; segmentnumber++) {
      makesubseg(m, &subsegloop);
      /* Mark the subsegment as living. */
      subsegloop.ss[2] = (subseg) subsegloop.ss;
    }
  }

  vertexindex = 0;
  attribindex = 0;

  if (!b->quiet) {
    printf("Reconstructing mesh.\n");
  }
  /* Allocate a temporary array that maps each vertex to some adjacent */
  /*   triangle.  I took care to allocate all the permanent memory for */
  /*   triangles and subsegments first.                                */
  vertexarray = (triangle *) trimalloc(m->vertices.items * (int) sizeof(triangle));
  /* Each vertex is initially unrepresented. */
  for (i = 0; i < m->vertices.items; i++) {
    vertexarray[i] = (triangle) m->dummytri;
  }

  if (b->verbose) {
    printf("  Assembling triangles.\n");
  }
  /* Read the triangles from the .ele file, and link */
  /*   together those that share an edge.            */
  traversalinit(&m->triangles);
  triangleloop.tri = triangletraverse(m);
  elementnumber = b->firstnumber;
  while (triangleloop.tri != (triangle *) NULL) {
    /* Copy the triangle's three corners. */
    for (j = 0; j < 3; j++) {
      corner[j] = trianglelist[vertexindex++];
      if ((corner[j] < b->firstnumber) ||
          (corner[j] >= b->firstnumber + m->invertices)) {
        printf("Error:  Triangle %ld has an invalid vertex index.\n", elementnumber);
        triexit(1);
      }
    }

    /* Find out about (and throw away) extra nodes. */
    for (j = 3; j < incorners; j++) {
      killvertexindex = trianglelist[vertexindex++];
        if ((killvertexindex >= b->firstnumber) &&
            (killvertexindex < b->firstnumber + m->invertices)) {
          /* Delete the non-corner vertex if it's not already deleted. */
          killvertex = getvertex(m, b, killvertexindex);
          if (vertextype(killvertex) != DEADVERTEX) {
            vertexdealloc(m, killvertex);
          }
        }
    }

    /* Read the triangle's attributes. */
    for (j = 0; j < m->eextras; j++) {
      setelemattribute(triangleloop, j, triangleattriblist[attribindex++]);
    }

    if (b->vararea) {
      area = trianglearealist[elementnumber - b->firstnumber];
      setareabound(triangleloop, area);
    }

    /* Set the triangle's vertices. */
    triangleloop.orient = 0;
    setorg(triangleloop, getvertex(m, b, corner[0]));
    setdest(triangleloop, getvertex(m, b, corner[1]));
    setapex(triangleloop, getvertex(m, b, corner[2]));
    /* Try linking the triangle to others that share these vertices. */
    for (triangleloop.orient = 0; triangleloop.orient < 3;
         triangleloop.orient++) {
      /* Take the number for the origin of triangleloop. */
      aroundvertex = corner[triangleloop.orient];
      /* Look for other triangles having this vertex. */
      nexttri = vertexarray[aroundvertex - b->firstnumber];
      /* Link the current triangle to the next one in the stack. */
      triangleloop.tri[6 + triangleloop.orient] = nexttri;
      /* Push the current triangle onto the stack. */
      vertexarray[aroundvertex - b->firstnumber] = encode(triangleloop);
      decode(nexttri, checktri);
      if (checktri.tri != m->dummytri) {
        dest(triangleloop, tdest);
        apex(triangleloop, tapex);
        /* Look for other triangles that share an edge. */
        do {
          dest(checktri, checkdest);
          apex(checktri, checkapex);
          if (tapex == checkdest) {
            /* The two triangles share an edge; bond them together. */
            lprev(triangleloop, triangleleft);
            bond(triangleleft, checktri);
          }
          if (tdest == checkapex) {
            /* The two triangles share an edge; bond them together. */
            lprev(checktri, checkleft);
            bond(triangleloop, checkleft);
          }
          /* Find the next triangle in the stack. */
          nexttri = checktri.tri[6 + checktri.orient];
          decode(nexttri, checktri);
        } while (checktri.tri != m->dummytri);
      }
    }
    triangleloop.tri = triangletraverse(m);
    elementnumber++;
  }

  vertexindex = 0;
  hullsize = 0;                      /* Prepare to count the boundary edges. */
  if (b->poly) {
    if (b->verbose) {
      printf("  Marking segments in triangulation.\n");
    }
    /* Read the segments from the .poly file, and link them */
    /*   to their neighboring triangles.                    */
    boundmarker = 0;
    traversalinit(&m->subsegs);
    subsegloop.ss = subsegtraverse(m);
    segmentnumber = b->firstnumber;
    while (subsegloop.ss != (subseg *) NULL) {
      end[0] = segmentlist[vertexindex++];
      end[1] = segmentlist[vertexindex++];
      if (segmentmarkers) {
        boundmarker = segmentmarkerlist[segmentnumber - b->firstnumber];
      }
      for (j = 0; j < 2; j++) {
        if ((end[j] < b->firstnumber) ||
            (end[j] >= b->firstnumber + m->invertices)) {
          printf("Error:  Segment %ld has an invalid vertex index.\n",
                 segmentnumber);
          triexit(1);
        }
      }

      /* set the subsegment's vertices. */
      subsegloop.ssorient = 0;
      segmentorg = getvertex(m, b, end[0]);
      segmentdest = getvertex(m, b, end[1]);
      setsorg(subsegloop, segmentorg);
      setsdest(subsegloop, segmentdest);
      setsegorg(subsegloop, segmentorg);
      setsegdest(subsegloop, segmentdest);
      setmark(subsegloop, boundmarker);
      /* Try linking the subsegment to triangles that share these vertices. */
      for (subsegloop.ssorient = 0; subsegloop.ssorient < 2;
           subsegloop.ssorient++) {
        /* Take the number for the destination of subsegloop. */
        aroundvertex = end[1 - subsegloop.ssorient];
        /* Look for triangles having this vertex. */
        prevlink = &vertexarray[aroundvertex - b->firstnumber];
        nexttri = vertexarray[aroundvertex - b->firstnumber];
        decode(nexttri, checktri);
        sorg(subsegloop, shorg);
        notfound = 1;
        /* Look for triangles having this edge.  Note that I'm only       */
        /*   comparing each triangle's destination with the subsegment;   */
        /*   each triangle's apex is handled through a different vertex.  */
        /*   Because each triangle appears on three vertices' lists, each */
        /*   occurrence of a triangle on a list can (and does) represent  */
        /*   an edge.  In this way, most edges are represented twice, and */
        /*   every triangle-subsegment bond is represented once.          */
        while (notfound && (checktri.tri != m->dummytri)) {
          dest(checktri, checkdest);
          if (shorg == checkdest) {
            /* We have a match.  Remove this triangle from the list. */
            *prevlink = checktri.tri[6 + checktri.orient];
            /* Bond the subsegment to the triangle. */
            tsbond(checktri, subsegloop);
            /* Check if this is a boundary edge. */
            sym(checktri, checkneighbor);
            if (checkneighbor.tri == m->dummytri) {
              /* The next line doesn't insert a subsegment (because there's */
              /*   already one there), but it sets the boundary markers of  */
              /*   the existing subsegment and its vertices.                */
              insertsubseg(m, b, &checktri, 1);
              hullsize++;
            }
            notfound = 0;
          }
          /* Find the next triangle in the stack. */
          prevlink = &checktri.tri[6 + checktri.orient];
          nexttri = checktri.tri[6 + checktri.orient];
          decode(nexttri, checktri);
        }
      }
      subsegloop.ss = subsegtraverse(m);
      segmentnumber++;
    }
  }

  /* Mark the remaining edges as not being attached to any subsegment. */
  /* Also, count the (yet uncounted) boundary edges.                   */
  for (i = 0; i < m->vertices.items; i++) {
    /* Search the stack of triangles adjacent to a vertex. */
    nexttri = vertexarray[i];
    decode(nexttri, checktri);
    while (checktri.tri != m->dummytri) {
      /* Find the next triangle in the stack before this */
      /*   information gets overwritten.                 */
      nexttri = checktri.tri[6 + checktri.orient];
      /* No adjacent subsegment.  (This overwrites the stack info.) */
      tsdissolve(checktri);
      sym(checktri, checkneighbor);
      if (checkneighbor.tri == m->dummytri) {
        insertsubseg(m, b, &checktri, 1);
        hullsize++;
      }
      decode(nexttri, checktri);
    }
  }

  trifree((void *) vertexarray);
  return hullsize;
}

/**                                                                         **/
/**                                                                         **/
/********* General mesh construction routines end here               *********/

/********* Segment insertion begins here                             *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  finddirection()   Find the first triangle on the path from one point     */
/*                    to another.                                            */
/*                                                                           */
/*  Finds the triangle that intersects a line segment drawn from the         */
/*  origin of `searchtri' to the point `searchpoint', and returns the result */
/*  in `searchtri'.  The origin of `searchtri' does not change, even though  */
/*  the triangle returned may differ from the one passed in.  This routine   */
/*  is used to find the direction to move in to get from one point to        */
/*  another.                                                                 */
/*                                                                           */
/*  The return value notes whether the destination or apex of the found      */
/*  triangle is collinear with the two points in question.                   */
/*                                                                           */
/*****************************************************************************/

enum finddirectionresult finddirection(struct mesh *m, struct behavior *b,
                                       struct otri *searchtri,
                                       vertex searchpoint)
{
  struct otri checktri;
  vertex startvertex;
  vertex leftvertex, rightvertex;
  REAL leftccw, rightccw;
  int leftflag, rightflag;
  triangle ptr;           /* Temporary variable used by onext() and oprev(). */

  org(*searchtri, startvertex);
  dest(*searchtri, rightvertex);
  apex(*searchtri, leftvertex);
  /* Is `searchpoint' to the left? */
  leftccw = counterclockwise(m, b, searchpoint, startvertex, leftvertex);
  leftflag = leftccw > 0.0;
  /* Is `searchpoint' to the right? */
  rightccw = counterclockwise(m, b, startvertex, searchpoint, rightvertex);
  rightflag = rightccw > 0.0;
  if (leftflag && rightflag) {
    /* `searchtri' faces directly away from `searchpoint'.  We could go left */
    /*   or right.  Ask whether it's a triangle or a boundary on the left.   */
    onext(*searchtri, checktri);
    if (checktri.tri == m->dummytri) {
      leftflag = 0;
    } else {
      rightflag = 0;
    }
  }
  while (leftflag) {
    /* Turn left until satisfied. */
    onextself(*searchtri);
    if (searchtri->tri == m->dummytri) {
      printf("Internal error in finddirection():  Unable to find a\n");
      printf("  triangle leading from (%.12g, %.12g) to", startvertex[0],
             startvertex[1]);
      printf("  (%.12g, %.12g).\n", searchpoint[0], searchpoint[1]);
      internalerror();
    }
    apex(*searchtri, leftvertex);
    rightccw = leftccw;
    leftccw = counterclockwise(m, b, searchpoint, startvertex, leftvertex);
    leftflag = leftccw > 0.0;
  }
  while (rightflag) {
    /* Turn right until satisfied. */
    oprevself(*searchtri);
    if (searchtri->tri == m->dummytri) {
      printf("Internal error in finddirection():  Unable to find a\n");
      printf("  triangle leading from (%.12g, %.12g) to", startvertex[0],
             startvertex[1]);
      printf("  (%.12g, %.12g).\n", searchpoint[0], searchpoint[1]);
      internalerror();
    }
    dest(*searchtri, rightvertex);
    leftccw = rightccw;
    rightccw = counterclockwise(m, b, startvertex, searchpoint, rightvertex);
    rightflag = rightccw > 0.0;
  }
  if (leftccw == 0.0) {
    return LEFTCOLLINEAR;
  } else if (rightccw == 0.0) {
    return RIGHTCOLLINEAR;
  } else {
    return WITHIN;
  }
}

/*****************************************************************************/
/*                                                                           */
/*  segmentintersection()   Find the intersection of an existing segment     */
/*                          and a segment that is being inserted.  Insert    */
/*                          a vertex at the intersection, splitting an       */
/*                          existing subsegment.                             */
/*                                                                           */
/*  The segment being inserted connects the apex of splittri to endpoint2.   */
/*  splitsubseg is the subsegment being split, and MUST adjoin splittri.     */
/*  Hence, endpoints of the subsegment being split are the origin and        */
/*  destination of splittri.                                                 */
/*                                                                           */
/*  On completion, splittri is a handle having the newly inserted            */
/*  intersection point as its origin, and endpoint1 as its destination.      */
/*                                                                           */
/*****************************************************************************/

void segmentintersection(struct mesh *m, struct behavior *b,
                         struct otri *splittri, struct osub *splitsubseg,
                         vertex endpoint2)
{
  struct osub opposubseg;
  vertex endpoint1;
  vertex torg, tdest;
  vertex leftvertex, rightvertex;
  vertex newvertex;
  enum insertvertexresult success;
  enum finddirectionresult collinear;
  REAL ex, ey;
  REAL tx, ty;
  REAL etx, ety;
  REAL split, denom;
  int i;
  triangle ptr;                       /* Temporary variable used by onext(). */
  subseg sptr;                        /* Temporary variable used by snext(). */

  /* Find the other three segment endpoints. */
  apex(*splittri, endpoint1);
  org(*splittri, torg);
  dest(*splittri, tdest);
  /* Segment intersection formulae; see the Antonio reference. */
  tx = tdest[0] - torg[0];
  ty = tdest[1] - torg[1];
  ex = endpoint2[0] - endpoint1[0];
  ey = endpoint2[1] - endpoint1[1];
  etx = torg[0] - endpoint2[0];
  ety = torg[1] - endpoint2[1];
  denom = ty * ex - tx * ey;
  if (denom == 0.0) {
    printf("Internal error in segmentintersection():");
    printf("  Attempt to find intersection of parallel segments.\n");
    internalerror();
  }
  split = (ey * etx - ex * ety) / denom;
  /* Create the new vertex. */
  newvertex = (vertex) poolalloc(&m->vertices);
  /* Interpolate its coordinate and attributes. */
  for (i = 0; i < 2 + m->nextras; i++) {
    newvertex[i] = torg[i] + split * (tdest[i] - torg[i]);
  }
  setvertexmark(newvertex, mark(*splitsubseg));
  setvertextype(newvertex, INPUTVERTEX);
  if (b->verbose > 1) {
    printf(
  "  Splitting subsegment (%.12g, %.12g) (%.12g, %.12g) at (%.12g, %.12g).\n",
           torg[0], torg[1], tdest[0], tdest[1], newvertex[0], newvertex[1]);
  }
  /* Insert the intersection vertex.  This should always succeed. */
  success = insertvertex(m, b, newvertex, splittri, splitsubseg, 0, 0);
  if (success != SUCCESSFULVERTEX) {
    printf("Internal error in segmentintersection():\n");
    printf("  Failure to split a segment.\n");
    internalerror();
  }
  /* Record a triangle whose origin is the new vertex. */
  setvertex2tri(newvertex, encode(*splittri));
  if (m->steinerleft > 0) {
    m->steinerleft--;
  }

  /* Divide the segment into two, and correct the segment endpoints. */
  ssymself(*splitsubseg);
  spivot(*splitsubseg, opposubseg);
  sdissolve(*splitsubseg);
  sdissolve(opposubseg);
  do {
    setsegorg(*splitsubseg, newvertex);
    snextself(*splitsubseg);
  } while (splitsubseg->ss != m->dummysub);
  do {
    setsegorg(opposubseg, newvertex);
    snextself(opposubseg);
  } while (opposubseg.ss != m->dummysub);

  /* Inserting the vertex may have caused edge flips.  We wish to rediscover */
  /*   the edge connecting endpoint1 to the new intersection vertex.         */
  collinear = finddirection(m, b, splittri, endpoint1);
  dest(*splittri, rightvertex);
  apex(*splittri, leftvertex);
  if ((leftvertex[0] == endpoint1[0]) && (leftvertex[1] == endpoint1[1])) {
    onextself(*splittri);
  } else if ((rightvertex[0] != endpoint1[0]) ||
             (rightvertex[1] != endpoint1[1])) {
    printf("Internal error in segmentintersection():\n");
    printf("  Topological inconsistency after splitting a segment.\n");
    internalerror();
  }
  /* `splittri' should have destination endpoint1. */
}

/*****************************************************************************/
/*                                                                           */
/*  scoutsegment()   Scout the first triangle on the path from one endpoint  */
/*                   to another, and check for completion (reaching the      */
/*                   second endpoint), a collinear vertex, or the            */
/*                   intersection of two segments.                           */
/*                                                                           */
/*  Returns one if the entire segment is successfully inserted, and zero if  */
/*  the job must be finished by conformingedge() or constrainededge().       */
/*                                                                           */
/*  If the first triangle on the path has the second endpoint as its         */
/*  destination or apex, a subsegment is inserted and the job is done.       */
/*                                                                           */
/*  If the first triangle on the path has a destination or apex that lies on */
/*  the segment, a subsegment is inserted connecting the first endpoint to   */
/*  the collinear vertex, and the search is continued from the collinear     */
/*  vertex.                                                                  */
/*                                                                           */
/*  If the first triangle on the path has a subsegment opposite its origin,  */
/*  then there is a segment that intersects the segment being inserted.      */
/*  Their intersection vertex is inserted, splitting the subsegment.         */
/*                                                                           */
/*****************************************************************************/

int scoutsegment(struct mesh *m, struct behavior *b, struct otri *searchtri,
                 vertex endpoint2, int newmark)
{
  struct otri crosstri;
  struct osub crosssubseg;
  vertex leftvertex, rightvertex;
  enum finddirectionresult collinear;
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  collinear = finddirection(m, b, searchtri, endpoint2);
  dest(*searchtri, rightvertex);
  apex(*searchtri, leftvertex);
  if (((leftvertex[0] == endpoint2[0]) && (leftvertex[1] == endpoint2[1])) ||
      ((rightvertex[0] == endpoint2[0]) && (rightvertex[1] == endpoint2[1]))) {
    /* The segment is already an edge in the mesh. */
    if ((leftvertex[0] == endpoint2[0]) && (leftvertex[1] == endpoint2[1])) {
      lprevself(*searchtri);
    }
    /* Insert a subsegment, if there isn't already one there. */
    insertsubseg(m, b, searchtri, newmark);
    return 1;
  } else if (collinear == LEFTCOLLINEAR) {
    /* We've collided with a vertex between the segment's endpoints. */
    /* Make the collinear vertex be the triangle's origin. */
    lprevself(*searchtri);
    insertsubseg(m, b, searchtri, newmark);
    /* Insert the remainder of the segment. */
    return scoutsegment(m, b, searchtri, endpoint2, newmark);
  } else if (collinear == RIGHTCOLLINEAR) {
    /* We've collided with a vertex between the segment's endpoints. */
    insertsubseg(m, b, searchtri, newmark);
    /* Make the collinear vertex be the triangle's origin. */
    lnextself(*searchtri);
    /* Insert the remainder of the segment. */
    return scoutsegment(m, b, searchtri, endpoint2, newmark);
  } else {
    lnext(*searchtri, crosstri);
    tspivot(crosstri, crosssubseg);
    /* Check for a crossing segment. */
    if (crosssubseg.ss == m->dummysub) {
      return 0;
    } else {
      /* Insert a vertex at the intersection. */
      segmentintersection(m, b, &crosstri, &crosssubseg, endpoint2);
      otricopy(crosstri, *searchtri);
      insertsubseg(m, b, searchtri, newmark);
      /* Insert the remainder of the segment. */
      return scoutsegment(m, b, searchtri, endpoint2, newmark);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  conformingedge()   Force a segment into a conforming Delaunay            */
/*                     triangulation by inserting a vertex at its midpoint,  */
/*                     and recursively forcing in the two half-segments if   */
/*                     necessary.                                            */
/*                                                                           */
/*  Generates a sequence of subsegments connecting `endpoint1' to            */
/*  `endpoint2'.  `newmark' is the boundary marker of the segment, assigned  */
/*  to each new splitting vertex and subsegment.                             */
/*                                                                           */
/*  Note that conformingedge() does not always maintain the conforming       */
/*  Delaunay property.  Once inserted, segments are locked into place;       */
/*  vertices inserted later (to force other segments in) may render these    */
/*  fixed segments non-Delaunay.  The conforming Delaunay property will be   */
/*  restored by enforcequality() by splitting encroached subsegments.        */
/*                                                                           */
/*****************************************************************************/

void conformingedge(struct mesh *m, struct behavior *b,
                    vertex endpoint1, vertex endpoint2, int newmark)
{
  struct otri searchtri1, searchtri2;
  struct osub brokensubseg;
  vertex newvertex;
  vertex midvertex1, midvertex2;
  enum insertvertexresult success;
  int i;
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  if (b->verbose > 2) {
    printf("Forcing segment into triangulation by recursive splitting:\n");
    printf("  (%.12g, %.12g) (%.12g, %.12g)\n", endpoint1[0], endpoint1[1],
           endpoint2[0], endpoint2[1]);
  }
  /* Create a new vertex to insert in the middle of the segment. */
  newvertex = (vertex) poolalloc(&m->vertices);
  /* Interpolate coordinates and attributes. */
  for (i = 0; i < 2 + m->nextras; i++) {
    newvertex[i] = 0.5 * (endpoint1[i] + endpoint2[i]);
  }
  setvertexmark(newvertex, newmark);
  setvertextype(newvertex, SEGMENTVERTEX);
  /* No known triangle to search from. */
  searchtri1.tri = m->dummytri;
  /* Attempt to insert the new vertex. */
  success = insertvertex(m, b, newvertex, &searchtri1, (struct osub *) NULL,
                         0, 0);
  if (success == DUPLICATEVERTEX) {
    if (b->verbose > 2) {
      printf("  Segment intersects existing vertex (%.12g, %.12g).\n",
             newvertex[0], newvertex[1]);
    }
    /* Use the vertex that's already there. */
    vertexdealloc(m, newvertex);
    org(searchtri1, newvertex);
  } else {
    if (success == VIOLATINGVERTEX) {
      if (b->verbose > 2) {
        printf("  Two segments intersect at (%.12g, %.12g).\n",
               newvertex[0], newvertex[1]);
      }
      /* By fluke, we've landed right on another segment.  Split it. */
      tspivot(searchtri1, brokensubseg);
      success = insertvertex(m, b, newvertex, &searchtri1, &brokensubseg,
                             0, 0);
      if (success != SUCCESSFULVERTEX) {
        printf("Internal error in conformingedge():\n");
        printf("  Failure to split a segment.\n");
        internalerror();
      }
    }
    /* The vertex has been inserted successfully. */
    if (m->steinerleft > 0) {
      m->steinerleft--;
    }
  }
  otricopy(searchtri1, searchtri2);
  /* `searchtri1' and `searchtri2' are fastened at their origins to         */
  /*   `newvertex', and will be directed toward `endpoint1' and `endpoint2' */
  /*   respectively.  First, we must get `searchtri2' out of the way so it  */
  /*   won't be invalidated during the insertion of the first half of the   */
  /*   segment.                                                             */
  finddirection(m, b, &searchtri2, endpoint2);
  if (!scoutsegment(m, b, &searchtri1, endpoint1, newmark)) {
    /* The origin of searchtri1 may have changed if a collision with an */
    /*   intervening vertex on the segment occurred.                    */
    org(searchtri1, midvertex1);
    conformingedge(m, b, midvertex1, endpoint1, newmark);
  }
  if (!scoutsegment(m, b, &searchtri2, endpoint2, newmark)) {
    /* The origin of searchtri2 may have changed if a collision with an */
    /*   intervening vertex on the segment occurred.                    */
    org(searchtri2, midvertex2);
    conformingedge(m, b, midvertex2, endpoint2, newmark);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  delaunayfixup()   Enforce the Delaunay condition at an edge, fanning out */
/*                    recursively from an existing vertex.  Pay special      */
/*                    attention to stacking inverted triangles.              */
/*                                                                           */
/*  This is a support routine for inserting segments into a constrained      */
/*  Delaunay triangulation.                                                  */
/*                                                                           */
/*  The origin of fixuptri is treated as if it has just been inserted, and   */
/*  the local Delaunay condition needs to be enforced.  It is only enforced  */
/*  in one sector, however, that being the angular range defined by          */
/*  fixuptri.                                                                */
/*                                                                           */
/*  This routine also needs to make decisions regarding the "stacking" of    */
/*  triangles.  (Read the description of constrainededge() below before      */
/*  reading on here, so you understand the algorithm.)  If the position of   */
/*  the new vertex (the origin of fixuptri) indicates that the vertex before */
/*  it on the polygon is a reflex vertex, then "stack" the triangle by       */
/*  doing nothing.  (fixuptri is an inverted triangle, which is how stacked  */
/*  triangles are identified.)                                               */
/*                                                                           */
/*  Otherwise, check whether the vertex before that was a reflex vertex.     */
/*  If so, perform an edge flip, thereby eliminating an inverted triangle    */
/*  (popping it off the stack).  The edge flip may result in the creation    */
/*  of a new inverted triangle, depending on whether or not the new vertex   */
/*  is visible to the vertex three edges behind on the polygon.              */
/*                                                                           */
/*  If neither of the two vertices behind the new vertex are reflex          */
/*  vertices, fixuptri and fartri, the triangle opposite it, are not         */
/*  inverted; hence, ensure that the edge between them is locally Delaunay.  */
/*                                                                           */
/*  `leftside' indicates whether or not fixuptri is to the left of the       */
/*  segment being inserted.  (Imagine that the segment is pointing up from   */
/*  endpoint1 to endpoint2.)                                                 */
/*                                                                           */
/*****************************************************************************/

void delaunayfixup(struct mesh *m, struct behavior *b,
                   struct otri *fixuptri, int leftside)
{
  struct otri neartri;
  struct otri fartri;
  struct osub faredge;
  vertex nearvertex, leftvertex, rightvertex, farvertex;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  lnext(*fixuptri, neartri);
  sym(neartri, fartri);
  /* Check if the edge opposite the origin of fixuptri can be flipped. */
  if (fartri.tri == m->dummytri) {
    return;
  }
  tspivot(neartri, faredge);
  if (faredge.ss != m->dummysub) {
    return;
  }
  /* Find all the relevant vertices. */
  apex(neartri, nearvertex);
  org(neartri, leftvertex);
  dest(neartri, rightvertex);
  apex(fartri, farvertex);
  /* Check whether the previous polygon vertex is a reflex vertex. */
  if (leftside) {
    if (counterclockwise(m, b, nearvertex, leftvertex, farvertex) <= 0.0) {
      /* leftvertex is a reflex vertex too.  Nothing can */
      /*   be done until a convex section is found.      */
      return;
    }
  } else {
    if (counterclockwise(m, b, farvertex, rightvertex, nearvertex) <= 0.0) {
      /* rightvertex is a reflex vertex too.  Nothing can */
      /*   be done until a convex section is found.       */
      return;
    }
  }
  if (counterclockwise(m, b, rightvertex, leftvertex, farvertex) > 0.0) {
    /* fartri is not an inverted triangle, and farvertex is not a reflex */
    /*   vertex.  As there are no reflex vertices, fixuptri isn't an     */
    /*   inverted triangle, either.  Hence, test the edge between the    */
    /*   triangles to ensure it is locally Delaunay.                     */
    if (incircle(m, b, leftvertex, farvertex, rightvertex, nearvertex) <=
        0.0) {
      return;
    }
    /* Not locally Delaunay; go on to an edge flip. */
  }        /* else fartri is inverted; remove it from the stack by flipping. */
  flip(m, b, &neartri);
  lprevself(*fixuptri);    /* Restore the origin of fixuptri after the flip. */
  /* Recursively process the two triangles that result from the flip. */
  delaunayfixup(m, b, fixuptri, leftside);
  delaunayfixup(m, b, &fartri, leftside);
}

/*****************************************************************************/
/*                                                                           */
/*  constrainededge()   Force a segment into a constrained Delaunay          */
/*                      triangulation by deleting the triangles it           */
/*                      intersects, and triangulating the polygons that      */
/*                      form on each side of it.                             */
/*                                                                           */
/*  Generates a single subsegment connecting `endpoint1' to `endpoint2'.     */
/*  The triangle `starttri' has `endpoint1' as its origin.  `newmark' is the */
/*  boundary marker of the segment.                                          */
/*                                                                           */
/*  To insert a segment, every triangle whose interior intersects the        */
/*  segment is deleted.  The union of these deleted triangles is a polygon   */
/*  (which is not necessarily monotone, but is close enough), which is       */
/*  divided into two polygons by the new segment.  This routine's task is    */
/*  to generate the Delaunay triangulation of these two polygons.            */
/*                                                                           */
/*  You might think of this routine's behavior as a two-step process.  The   */
/*  first step is to walk from endpoint1 to endpoint2, flipping each edge    */
/*  encountered.  This step creates a fan of edges connected to endpoint1,   */
/*  including the desired edge to endpoint2.  The second step enforces the   */
/*  Delaunay condition on each side of the segment in an incremental manner: */
/*  proceeding aULONGLONG the polygon from endpoint1 to endpoint2 (this is done   */
/*  independently on each side of the segment), each vertex is "enforced"    */
/*  as if it had just been inserted, but affecting only the previous         */
/*  vertices.  The result is the same as if the vertices had been inserted   */
/*  in the order they appear on the polygon, so the result is Delaunay.      */
/*                                                                           */
/*  In truth, constrainededge() interleaves these two steps.  The procedure  */
/*  walks from endpoint1 to endpoint2, and each time an edge is encountered  */
/*  and flipped, the newly exposed vertex (at the far end of the flipped     */
/*  edge) is "enforced" upon the previously flipped edges, usually affecting */
/*  only one side of the polygon (depending upon which side of the segment   */
/*  the vertex falls on).                                                    */
/*                                                                           */
/*  The algorithm is complicated by the need to handle polygons that are not */
/*  convex.  Although the polygon is not necessarily monotone, it can be     */
/*  triangulated in a manner similar to the stack-based algorithms for       */
/*  monotone polygons.  For each reflex vertex (local concavity) of the      */
/*  polygon, there will be an inverted triangle formed by one of the edge    */
/*  flips.  (An inverted triangle is one with negative area - that is, its   */
/*  vertices are arranged in clockwise order - and is best thought of as a   */
/*  wrinkle in the fabric of the mesh.)  Each inverted triangle can be       */
/*  thought of as a reflex vertex pushed on the stack, waiting to be fixed   */
/*  later.                                                                   */
/*                                                                           */
/*  A reflex vertex is popped from the stack when a vertex is inserted that  */
/*  is visible to the reflex vertex.  (However, if the vertex behind the     */
/*  reflex vertex is not visible to the reflex vertex, a new inverted        */
/*  triangle will take its place on the stack.)  These details are handled   */
/*  by the delaunayfixup() routine above.                                    */
/*                                                                           */
/*****************************************************************************/

void constrainededge(struct mesh *m, struct behavior *b,
                     struct otri *starttri, vertex endpoint2, int newmark)
{
  struct otri fixuptri, fixuptri2;
  struct osub crosssubseg;
  vertex endpoint1;
  vertex farvertex;
  REAL area;
  int collision;
  int done;
  triangle ptr;             /* Temporary variable used by sym() and oprev(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  org(*starttri, endpoint1);
  lnext(*starttri, fixuptri);
  flip(m, b, &fixuptri);
  /* `collision' indicates whether we have found a vertex directly */
  /*   between endpoint1 and endpoint2.                            */
  collision = 0;
  done = 0;
  do {
    org(fixuptri, farvertex);
    /* `farvertex' is the extreme point of the polygon we are "digging" */
    /*   to get from endpoint1 to endpoint2.                           */
    if ((farvertex[0] == endpoint2[0]) && (farvertex[1] == endpoint2[1])) {
      oprev(fixuptri, fixuptri2);
      /* Enforce the Delaunay condition around endpoint2. */
      delaunayfixup(m, b, &fixuptri, 0);
      delaunayfixup(m, b, &fixuptri2, 1);
      done = 1;
    } else {
      /* Check whether farvertex is to the left or right of the segment */
      /*   being inserted, to decide which edge of fixuptri to dig      */
      /*   through next.                                                */
      area = counterclockwise(m, b, endpoint1, endpoint2, farvertex);
      if (area == 0.0) {
        /* We've collided with a vertex between endpoint1 and endpoint2. */
        collision = 1;
        oprev(fixuptri, fixuptri2);
        /* Enforce the Delaunay condition around farvertex. */
        delaunayfixup(m, b, &fixuptri, 0);
        delaunayfixup(m, b, &fixuptri2, 1);
        done = 1;
      } else {
        if (area > 0.0) {        /* farvertex is to the left of the segment. */
          oprev(fixuptri, fixuptri2);
          /* Enforce the Delaunay condition around farvertex, on the */
          /*   left side of the segment only.                        */
          delaunayfixup(m, b, &fixuptri2, 1);
          /* Flip the edge that crosses the segment.  After the edge is */
          /*   flipped, one of its endpoints is the fan vertex, and the */
          /*   destination of fixuptri is the fan vertex.               */
          lprevself(fixuptri);
        } else {                /* farvertex is to the right of the segment. */
          delaunayfixup(m, b, &fixuptri, 0);
          /* Flip the edge that crosses the segment.  After the edge is */
          /*   flipped, one of its endpoints is the fan vertex, and the */
          /*   destination of fixuptri is the fan vertex.               */
          oprevself(fixuptri);
        }
        /* Check for two intersecting segments. */
        tspivot(fixuptri, crosssubseg);
        if (crosssubseg.ss == m->dummysub) {
          flip(m, b, &fixuptri);    /* May create inverted triangle at left. */
        } else {
          /* We've collided with a segment between endpoint1 and endpoint2. */
          collision = 1;
          /* Insert a vertex at the intersection. */
          segmentintersection(m, b, &fixuptri, &crosssubseg, endpoint2);
          done = 1;
        }
      }
    }
  } while (!done);
  /* Insert a subsegment to make the segment permanent. */
  insertsubseg(m, b, &fixuptri, newmark);
  /* If there was a collision with an interceding vertex, install another */
  /*   segment connecting that vertex with endpoint2.                     */
  if (collision) {
    /* Insert the remainder of the segment. */
    if (!scoutsegment(m, b, &fixuptri, endpoint2, newmark)) {
      constrainededge(m, b, &fixuptri, endpoint2, newmark);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  insertsegment()   Insert a PSLG segment into a triangulation.            */
/*                                                                           */
/*****************************************************************************/

void insertsegment(struct mesh *m, struct behavior *b,
                   vertex endpoint1, vertex endpoint2, int newmark)
{
  struct otri searchtri1, searchtri2;
  triangle encodedtri;
  vertex checkvertex;
  triangle ptr;                         /* Temporary variable used by sym(). */

  if (b->verbose > 1) {
    printf("  Connecting (%.12g, %.12g) to (%.12g, %.12g).\n",
           endpoint1[0], endpoint1[1], endpoint2[0], endpoint2[1]);
  }

  /* Find a triangle whose origin is the segment's first endpoint. */
  checkvertex = (vertex) NULL;
  encodedtri = vertex2tri(endpoint1);
  if (encodedtri != (triangle) NULL) {
    decode(encodedtri, searchtri1);
    org(searchtri1, checkvertex);
  }
  if (checkvertex != endpoint1) {
    /* Find a boundary triangle to search from. */
    searchtri1.tri = m->dummytri;
    searchtri1.orient = 0;
    symself(searchtri1);
    /* Search for the segment's first endpoint by point location. */
    if (locate(m, b, endpoint1, &searchtri1) != ONVERTEX) {
      printf(
        "Internal error in insertsegment():  Unable to locate PSLG vertex\n");
      printf("  (%.12g, %.12g) in triangulation.\n",
             endpoint1[0], endpoint1[1]);
      internalerror();
    }
  }
  /* Remember this triangle to improve subsequent point location. */
  otricopy(searchtri1, m->recenttri);
  /* Scout the beginnings of a path from the first endpoint */
  /*   toward the second.                                   */
  if (scoutsegment(m, b, &searchtri1, endpoint2, newmark)) {
    /* The segment was easily inserted. */
    return;
  }
  /* The first endpoint may have changed if a collision with an intervening */
  /*   vertex on the segment occurred.                                      */
  org(searchtri1, endpoint1);

  /* Find a triangle whose origin is the segment's second endpoint. */
  checkvertex = (vertex) NULL;
  encodedtri = vertex2tri(endpoint2);
  if (encodedtri != (triangle) NULL) {
    decode(encodedtri, searchtri2);
    org(searchtri2, checkvertex);
  }
  if (checkvertex != endpoint2) {
    /* Find a boundary triangle to search from. */
    searchtri2.tri = m->dummytri;
    searchtri2.orient = 0;
    symself(searchtri2);
    /* Search for the segment's second endpoint by point location. */
    if (locate(m, b, endpoint2, &searchtri2) != ONVERTEX) {
      printf(
        "Internal error in insertsegment():  Unable to locate PSLG vertex\n");
      printf("  (%.12g, %.12g) in triangulation.\n",
             endpoint2[0], endpoint2[1]);
      internalerror();
    }
  }
  /* Remember this triangle to improve subsequent point location. */
  otricopy(searchtri2, m->recenttri);
  /* Scout the beginnings of a path from the second endpoint */
  /*   toward the first.                                     */
  if (scoutsegment(m, b, &searchtri2, endpoint1, newmark)) {
    /* The segment was easily inserted. */
    return;
  }
  /* The second endpoint may have changed if a collision with an intervening */
  /*   vertex on the segment occurred.                                       */
  org(searchtri2, endpoint2);

  if (b->splitseg) {
    /* Insert vertices to force the segment into the triangulation. */
    conformingedge(m, b, endpoint1, endpoint2, newmark);
  } else {
    /* Insert the segment directly into the triangulation. */
    constrainededge(m, b, &searchtri1, endpoint2, newmark);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  markhull()   Cover the convex hull of a triangulation with subsegments.  */
/*                                                                           */
/*****************************************************************************/

void markhull(struct mesh *m, struct behavior *b)
{
  struct otri hulltri;
  struct otri nexttri;
  struct otri starttri;
  triangle ptr;             /* Temporary variable used by sym() and oprev(). */

  /* Find a triangle handle on the hull. */
  hulltri.tri = m->dummytri;
  hulltri.orient = 0;
  symself(hulltri);
  /* Remember where we started so we know when to stop. */
  otricopy(hulltri, starttri);
  /* Go once counterclockwise around the convex hull. */
  do {
    /* Create a subsegment if there isn't already one here. */
    insertsubseg(m, b, &hulltri, 1);
    /* To find the next hull edge, go clockwise around the next vertex. */
    lnextself(hulltri);
    oprev(hulltri, nexttri);
    while (nexttri.tri != m->dummytri) {
      otricopy(nexttri, hulltri);
      oprev(hulltri, nexttri);
    }
  } while (!otriequal(hulltri, starttri));
}

/*****************************************************************************/
/*                                                                           */
/*  formskeleton()   Create the segments of a triangulation, including PSLG  */
/*                   segments and edges on the convex hull.                  */
/*                                                                           */
/*  The PSLG segments are read from a .poly file.  The return value is the   */
/*  number of segments in the file.                                          */
/*                                                                           */
/*****************************************************************************/

void formskeleton(struct mesh *m, struct behavior *b, int *segmentlist, int *segmentmarkerlist, int numberofsegments)
{
  char polyfilename[6];
  int index;
  vertex endpoint1, endpoint2;
  int segmentmarkers;
  int end1, end2;
  int boundmarker;
  int i;

  if (b->poly) {
    if (!b->quiet) {
      printf("Recovering segments in Delaunay triangulation.\n");
    }
    strcpy(polyfilename, "input");
    m->insegments = numberofsegments;
    segmentmarkers = segmentmarkerlist != (int *) NULL;
    index = 0;
    /* If the input vertices are collinear, there is no triangulation, */
    /*   so don't try to insert segments.                              */
    if (m->triangles.items == 0) {
      return;
    }

    /* If segments are to be inserted, compute a mapping */
    /*   from vertices to triangles.                     */
    if (m->insegments > 0) {
      makevertexmap(m, b);
      if (b->verbose) {
        printf("  Recovering PSLG segments.\n");
      }
    }

    boundmarker = 0;
    /* Read and insert the segments. */
    for (i = 0; i < m->insegments; i++) {
      end1 = segmentlist[index++];
      end2 = segmentlist[index++];
      if (segmentmarkers) {
        boundmarker = segmentmarkerlist[i];
      }
      if ((end1 < b->firstnumber) ||
          (end1 >= b->firstnumber + m->invertices)) {
        if (!b->quiet) {
          printf("Warning:  Invalid first endpoint of segment %d in %s.\n",
                 b->firstnumber + i, polyfilename);
        }
      } else if ((end2 < b->firstnumber) ||
                 (end2 >= b->firstnumber + m->invertices)) {
        if (!b->quiet) {
          printf("Warning:  Invalid second endpoint of segment %d in %s.\n",
                 b->firstnumber + i, polyfilename);
        }
      } else {
        /* Find the vertices numbered `end1' and `end2'. */
        endpoint1 = getvertex(m, b, end1);
        endpoint2 = getvertex(m, b, end2);
        if ((endpoint1[0] == endpoint2[0]) && (endpoint1[1] == endpoint2[1])) {
          if (!b->quiet) {
            printf("Warning:  Endpoints of segment %d are coincident in %s.\n",
                   b->firstnumber + i, polyfilename);
          }
        } else {
          insertsegment(m, b, endpoint1, endpoint2, boundmarker);
        }
      }
    }
  } else {
    m->insegments = 0;
  }
  if (b->convex || !b->poly) {
    /* Enclose the convex hull with subsegments. */
    if (b->verbose) {
      printf("  Enclosing convex hull with segments.\n");
    }
    markhull(m, b);
  }
}

/**                                                                         **/
/**                                                                         **/
/********* Segment insertion ends here                               *********/

/********* Carving out holes and concavities begins here             *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  infecthull()   Virally infect all of the triangles of the convex hull    */
/*                 that are not protected by subsegments.  Where there are   */
/*                 subsegments, set boundary markers as appropriate.         */
/*                                                                           */
/*****************************************************************************/

void infecthull(struct mesh *m, struct behavior *b)
{
  struct otri hulltri;
  struct otri nexttri;
  struct otri starttri;
  struct osub hullsubseg;
  triangle **deadtriangle;
  vertex horg, hdest;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  if (b->verbose) {
    printf("  Marking concavities (external triangles) for elimination.\n");
  }
  /* Find a triangle handle on the hull. */
  hulltri.tri = m->dummytri;
  hulltri.orient = 0;
  symself(hulltri);
  /* Remember where we started so we know when to stop. */
  otricopy(hulltri, starttri);
  /* Go once counterclockwise around the convex hull. */
  do {
    /* Ignore triangles that are already infected. */
    if (!infected(hulltri)) {
      /* Is the triangle protected by a subsegment? */
      tspivot(hulltri, hullsubseg);
      if (hullsubseg.ss == m->dummysub) {
        /* The triangle is not protected; infect it. */
        if (!infected(hulltri)) {
          infect(hulltri);
          deadtriangle = (triangle **) poolalloc(&m->viri);
          *deadtriangle = hulltri.tri;
        }
      } else {
        /* The triangle is protected; set boundary markers if appropriate. */
        if (mark(hullsubseg) == 0) {
          setmark(hullsubseg, 1);
          org(hulltri, horg);
          dest(hulltri, hdest);
          if (vertexmark(horg) == 0) {
            setvertexmark(horg, 1);
          }
          if (vertexmark(hdest) == 0) {
            setvertexmark(hdest, 1);
          }
        }
      }
    }
    /* To find the next hull edge, go clockwise around the next vertex. */
    lnextself(hulltri);
    oprev(hulltri, nexttri);
    while (nexttri.tri != m->dummytri) {
      otricopy(nexttri, hulltri);
      oprev(hulltri, nexttri);
    }
  } while (!otriequal(hulltri, starttri));
}

/*****************************************************************************/
/*                                                                           */
/*  plague()   Spread the virus from all infected triangles to any neighbors */
/*             not protected by subsegments.  Delete all infected triangles. */
/*                                                                           */
/*  This is the procedure that actually creates holes and concavities.       */
/*                                                                           */
/*  This procedure operates in two phases.  The first phase identifies all   */
/*  the triangles that will die, and marks them as infected.  They are       */
/*  marked to ensure that each triangle is added to the virus pool only      */
/*  once, so the procedure will terminate.                                   */
/*                                                                           */
/*  The second phase actually eliminates the infected triangles.  It also    */
/*  eliminates orphaned vertices.                                            */
/*                                                                           */
/*****************************************************************************/

void plague(struct mesh *m, struct behavior *b)
{
  struct otri testtri;
  struct otri neighbor;
  triangle **virusloop;
  triangle **deadtriangle;
  struct osub neighborsubseg;
  vertex testvertex;
  vertex norg, ndest;
  vertex deadorg, deaddest, deadapex;
  int killorg;
  triangle ptr;             /* Temporary variable used by sym() and onext(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  if (b->verbose) {
    printf("  Marking neighbors of marked triangles.\n");
  }
  /* Loop through all the infected triangles, spreading the virus to */
  /*   their neighbors, then to their neighbors' neighbors.          */
  traversalinit(&m->viri);
  virusloop = (triangle **) traverse(&m->viri);
  while (virusloop != (triangle **) NULL) {
    testtri.tri = *virusloop;
    /* A triangle is marked as infected by messing with one of its pointers */
    /*   to subsegments, setting it to an illegal value.  Hence, we have to */
    /*   temporarily uninfect this triangle so that we can examine its      */
    /*   adjacent subsegments.                                              */
    uninfect(testtri);
    if (b->verbose > 2) {
      /* Assign the triangle an orientation for convenience in */
      /*   checking its vertices.                              */
      testtri.orient = 0;
      org(testtri, deadorg);
      dest(testtri, deaddest);
      apex(testtri, deadapex);
      printf("    Checking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
             deadorg[0], deadorg[1], deaddest[0], deaddest[1],
             deadapex[0], deadapex[1]);
    }
    /* Check each of the triangle's three neighbors. */
    for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
      /* Find the neighbor. */
      sym(testtri, neighbor);
      /* Check for a subsegment between the triangle and its neighbor. */
      tspivot(testtri, neighborsubseg);
      /* Check if the neighbor is nonexistent or already infected. */
      if ((neighbor.tri == m->dummytri) || infected(neighbor)) {
        if (neighborsubseg.ss != m->dummysub) {
          /* There is a subsegment separating the triangle from its      */
          /*   neighbor, but both triangles are dying, so the subsegment */
          /*   dies too.                                                 */
          subsegdealloc(m, neighborsubseg.ss);
          if (neighbor.tri != m->dummytri) {
            /* Make sure the subsegment doesn't get deallocated again */
            /*   later when the infected neighbor is visited.         */
            uninfect(neighbor);
            tsdissolve(neighbor);
            infect(neighbor);
          }
        }
      } else {                   /* The neighbor exists and is not infected. */
        if (neighborsubseg.ss == m->dummysub) {
          /* There is no subsegment protecting the neighbor, so */
          /*   the neighbor becomes infected.                   */
          if (b->verbose > 2) {
            org(neighbor, deadorg);
            dest(neighbor, deaddest);
            apex(neighbor, deadapex);
            printf(
              "    Marking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
                   deadorg[0], deadorg[1], deaddest[0], deaddest[1],
                   deadapex[0], deadapex[1]);
          }
          infect(neighbor);
          /* Ensure that the neighbor's neighbors will be infected. */
          deadtriangle = (triangle **) poolalloc(&m->viri);
          *deadtriangle = neighbor.tri;
        } else {               /* The neighbor is protected by a subsegment. */
          /* Remove this triangle from the subsegment. */
          stdissolve(neighborsubseg);
          /* The subsegment becomes a boundary.  Set markers accordingly. */
          if (mark(neighborsubseg) == 0) {
            setmark(neighborsubseg, 1);
          }
          org(neighbor, norg);
          dest(neighbor, ndest);
          if (vertexmark(norg) == 0) {
            setvertexmark(norg, 1);
          }
          if (vertexmark(ndest) == 0) {
            setvertexmark(ndest, 1);
          }
        }
      }
    }
    /* Remark the triangle as infected, so it doesn't get added to the */
    /*   virus pool again.                                             */
    infect(testtri);
    virusloop = (triangle **) traverse(&m->viri);
  }

  if (b->verbose) {
    printf("  Deleting marked triangles.\n");
  }

  traversalinit(&m->viri);
  virusloop = (triangle **) traverse(&m->viri);
  while (virusloop != (triangle **) NULL) {
    testtri.tri = *virusloop;

    /* Check each of the three corners of the triangle for elimination. */
    /*   This is done by walking around each vertex, checking if it is  */
    /*   still connected to at least one live triangle.                 */
    for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
      org(testtri, testvertex);
      /* Check if the vertex has already been tested. */
      if (testvertex != (vertex) NULL) {
        killorg = 1;
        /* Mark the corner of the triangle as having been tested. */
        setorg(testtri, NULL);
        /* Walk counterclockwise about the vertex. */
        onext(testtri, neighbor);
        /* Stop upon reaching a boundary or the starting triangle. */
        while ((neighbor.tri != m->dummytri) &&
               (!otriequal(neighbor, testtri))) {
          if (infected(neighbor)) {
            /* Mark the corner of this triangle as having been tested. */
            setorg(neighbor, NULL);
          } else {
            /* A live triangle.  The vertex survives. */
            killorg = 0;
          }
          /* Walk counterclockwise about the vertex. */
          onextself(neighbor);
        }
        /* If we reached a boundary, we must walk clockwise as well. */
        if (neighbor.tri == m->dummytri) {
          /* Walk clockwise about the vertex. */
          oprev(testtri, neighbor);
          /* Stop upon reaching a boundary. */
          while (neighbor.tri != m->dummytri) {
            if (infected(neighbor)) {
            /* Mark the corner of this triangle as having been tested. */
              setorg(neighbor, NULL);
            } else {
              /* A live triangle.  The vertex survives. */
              killorg = 0;
            }
            /* Walk clockwise about the vertex. */
            oprevself(neighbor);
          }
        }
        if (killorg) {
          if (b->verbose > 1) {
            printf("    Deleting vertex (%.12g, %.12g)\n",
                   testvertex[0], testvertex[1]);
          }
          setvertextype(testvertex, UNDEADVERTEX);
          m->undeads++;
        }
      }
    }

    /* Record changes in the number of boundary edges, and disconnect */
    /*   dead triangles from their neighbors.                         */
    for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
      sym(testtri, neighbor);
      if (neighbor.tri == m->dummytri) {
        /* There is no neighboring triangle on this edge, so this edge    */
        /*   is a boundary edge.  This triangle is being deleted, so this */
        /*   boundary edge is deleted.                                    */
        m->hullsize--;
      } else {
        /* Disconnect the triangle from its neighbor. */
        dissolve(neighbor);
        /* There is a neighboring triangle on this edge, so this edge */
        /*   becomes a boundary edge when this triangle is deleted.   */
        m->hullsize++;
      }
    }
    /* Return the dead triangle to the pool of triangles. */
    triangledealloc(m, testtri.tri);
    virusloop = (triangle **) traverse(&m->viri);
  }
  /* Empty the virus pool. */
  poolrestart(&m->viri);
}

/*****************************************************************************/
/*                                                                           */
/*  regionplague()   Spread regional attributes and/or area constraints      */
/*                   (from a .poly file) throughout the mesh.                */
/*                                                                           */
/*  This procedure operates in two phases.  The first phase spreads an       */
/*  attribute and/or an area constraint through a (segment-bounded) region.  */
/*  The triangles are marked to ensure that each triangle is added to the    */
/*  virus pool only once, so the procedure will terminate.                   */
/*                                                                           */
/*  The second phase uninfects all infected triangles, returning them to     */
/*  normal.                                                                  */
/*                                                                           */
/*****************************************************************************/

void regionplague(struct mesh *m, struct behavior *b,
                  REAL attribute, REAL area)
{
  struct otri testtri;
  struct otri neighbor;
  triangle **virusloop;
  triangle **regiontri;
  struct osub neighborsubseg;
  vertex regionorg, regiondest, regionapex;
  triangle ptr;             /* Temporary variable used by sym() and onext(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  if (b->verbose > 1) {
    printf("  Marking neighbors of marked triangles.\n");
  }
  /* Loop through all the infected triangles, spreading the attribute      */
  /*   and/or area constraint to their neighbors, then to their neighbors' */
  /*   neighbors.                                                          */
  traversalinit(&m->viri);
  virusloop = (triangle **) traverse(&m->viri);
  while (virusloop != (triangle **) NULL) {
    testtri.tri = *virusloop;
    /* A triangle is marked as infected by messing with one of its pointers */
    /*   to subsegments, setting it to an illegal value.  Hence, we have to */
    /*   temporarily uninfect this triangle so that we can examine its      */
    /*   adjacent subsegments.                                              */
    uninfect(testtri);
    if (b->regionattrib) {
      /* Set an attribute. */
      setelemattribute(testtri, m->eextras, attribute);
    }
    if (b->vararea) {
      /* Set an area constraint. */
      setareabound(testtri, area);
    }
    if (b->verbose > 2) {
      /* Assign the triangle an orientation for convenience in */
      /*   checking its vertices.                              */
      testtri.orient = 0;
      org(testtri, regionorg);
      dest(testtri, regiondest);
      apex(testtri, regionapex);
      printf("    Checking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
             regionorg[0], regionorg[1], regiondest[0], regiondest[1],
             regionapex[0], regionapex[1]);
    }
    /* Check each of the triangle's three neighbors. */
    for (testtri.orient = 0; testtri.orient < 3; testtri.orient++) {
      /* Find the neighbor. */
      sym(testtri, neighbor);
      /* Check for a subsegment between the triangle and its neighbor. */
      tspivot(testtri, neighborsubseg);
      /* Make sure the neighbor exists, is not already infected, and */
      /*   isn't protected by a subsegment.                          */
      if ((neighbor.tri != m->dummytri) && !infected(neighbor)
          && (neighborsubseg.ss == m->dummysub)) {
        if (b->verbose > 2) {
          org(neighbor, regionorg);
          dest(neighbor, regiondest);
          apex(neighbor, regionapex);
          printf("    Marking (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
                 regionorg[0], regionorg[1], regiondest[0], regiondest[1],
                 regionapex[0], regionapex[1]);
        }
        /* Infect the neighbor. */
        infect(neighbor);
        /* Ensure that the neighbor's neighbors will be infected. */
        regiontri = (triangle **) poolalloc(&m->viri);
        *regiontri = neighbor.tri;
      }
    }
    /* Remark the triangle as infected, so it doesn't get added to the */
    /*   virus pool again.                                             */
    infect(testtri);
    virusloop = (triangle **) traverse(&m->viri);
  }

  /* Uninfect all triangles. */
  if (b->verbose > 1) {
    printf("  Unmarking marked triangles.\n");
  }
  traversalinit(&m->viri);
  virusloop = (triangle **) traverse(&m->viri);
  while (virusloop != (triangle **) NULL) {
    testtri.tri = *virusloop;
    uninfect(testtri);
    virusloop = (triangle **) traverse(&m->viri);
  }
  /* Empty the virus pool. */
  poolrestart(&m->viri);
}

/*****************************************************************************/
/*                                                                           */
/*  carveholes()   Find the holes and infect them.  Find the area            */
/*                 constraints and infect them.  Infect the convex hull.     */
/*                 Spread the infection and kill triangles.  Spread the      */
/*                 area constraints.                                         */
/*                                                                           */
/*  This routine mainly calls other routines to carry out all these          */
/*  functions.                                                               */
/*                                                                           */
/*****************************************************************************/

void carveholes(struct mesh *m, struct behavior *b, REAL *holelist, int holes,
                REAL *regionlist, int regions)
{
  struct otri searchtri;
  struct otri triangleloop;
  struct otri *regiontris;
  triangle **holetri;
  triangle **regiontri;
  vertex searchorg, searchdest;
  enum locateresult intersect;
  int i;
  triangle ptr;                         /* Temporary variable used by sym(). */

  if (!(b->quiet || (b->noholes && b->convex))) {
    printf("Removing unwanted triangles.\n");
    if (b->verbose && (holes > 0)) {
      printf("  Marking holes for elimination.\n");
    }
  }

  if (regions > 0) {
    /* Allocate storage for the triangles in which region points fall. */
    regiontris = (struct otri *) trimalloc(regions *
                                           (int) sizeof(struct otri));
  } else {
    regiontris = (struct otri *) NULL;
  }

  if (((holes > 0) && !b->noholes) || !b->convex || (regions > 0)) {
    /* Initialize a pool of viri to be used for holes, concavities, */
    /*   regional attributes, and/or regional area constraints.     */
    poolinit(&m->viri, sizeof(triangle *), VIRUSPERBLOCK, VIRUSPERBLOCK, 0);
  }

  if (!b->convex) {
    /* Mark as infected any unprotected triangles on the boundary. */
    /*   This is one way by which concavities are created.         */
    infecthull(m, b);
  }

  if ((holes > 0) && !b->noholes) {
    /* Infect each triangle in which a hole lies. */
    for (i = 0; i < 2 * holes; i += 2) {
      /* Ignore holes that aren't within the bounds of the mesh. */
      if ((holelist[i] >= m->xmin) && (holelist[i] <= m->xmax)
          && (holelist[i + 1] >= m->ymin) && (holelist[i + 1] <= m->ymax)) {
        /* Start searching from some triangle on the outer boundary. */
        searchtri.tri = m->dummytri;
        searchtri.orient = 0;
        symself(searchtri);
        /* Ensure that the hole is to the left of this boundary edge; */
        /*   otherwise, locate() will falsely report that the hole    */
        /*   falls within the starting triangle.                      */
        org(searchtri, searchorg);
        dest(searchtri, searchdest);
        if (counterclockwise(m, b, searchorg, searchdest, &holelist[i]) >
            0.0) {
          /* Find a triangle that contains the hole. */
          intersect = locate(m, b, &holelist[i], &searchtri);
          if ((intersect != OUTSIDE) && (!infected(searchtri))) {
            /* Infect the triangle.  This is done by marking the triangle  */
            /*   as infected and including the triangle in the virus pool. */
            infect(searchtri);
            holetri = (triangle **) poolalloc(&m->viri);
            *holetri = searchtri.tri;
          }
        }
      }
    }
  }

  /* Now, we have to find all the regions BEFORE we carve the holes, because */
  /*   locate() won't work when the triangulation is no ULONGLONGer convex.       */
  /*   (Incidentally, this is the reason why regional attributes and area    */
  /*   constraints can't be used when refining a preexisting mesh, which     */
  /*   might not be convex; they can only be used with a freshly             */
  /*   triangulated PSLG.)                                                   */
  if (regions > 0) {
    /* Find the starting triangle for each region. */
    for (i = 0; i < regions; i++) {
      regiontris[i].tri = m->dummytri;
      /* Ignore region points that aren't within the bounds of the mesh. */
      if ((regionlist[4 * i] >= m->xmin) && (regionlist[4 * i] <= m->xmax) &&
          (regionlist[4 * i + 1] >= m->ymin) &&
          (regionlist[4 * i + 1] <= m->ymax)) {
        /* Start searching from some triangle on the outer boundary. */
        searchtri.tri = m->dummytri;
        searchtri.orient = 0;
        symself(searchtri);
        /* Ensure that the region point is to the left of this boundary */
        /*   edge; otherwise, locate() will falsely report that the     */
        /*   region point falls within the starting triangle.           */
        org(searchtri, searchorg);
        dest(searchtri, searchdest);
        if (counterclockwise(m, b, searchorg, searchdest, &regionlist[4 * i]) >
            0.0) {
          /* Find a triangle that contains the region point. */
          intersect = locate(m, b, &regionlist[4 * i], &searchtri);
          if ((intersect != OUTSIDE) && (!infected(searchtri))) {
            /* Record the triangle for processing after the */
            /*   holes have been carved.                    */
            otricopy(searchtri, regiontris[i]);
          }
        }
      }
    }
  }

  if (m->viri.items > 0) {
    /* Carve the holes and concavities. */
    plague(m, b);
  }
  /* The virus pool should be empty now. */

  if (regions > 0) {
    if (!b->quiet) {
      if (b->regionattrib) {
        if (b->vararea) {
          printf("Spreading regional attributes and area constraints.\n");
        } else {
          printf("Spreading regional attributes.\n");
        }
      } else {
        printf("Spreading regional area constraints.\n");
      }
    }
    if (b->regionattrib && !b->refine) {
      /* Assign every triangle a regional attribute of zero. */
      traversalinit(&m->triangles);
      triangleloop.orient = 0;
      triangleloop.tri = triangletraverse(m);
      while (triangleloop.tri != (triangle *) NULL) {
        setelemattribute(triangleloop, m->eextras, 0.0);
        triangleloop.tri = triangletraverse(m);
      }
    }
    for (i = 0; i < regions; i++) {
      if (regiontris[i].tri != m->dummytri) {
        /* Make sure the triangle under consideration still exists. */
        /*   It may have been eaten by the virus.                   */
        if (!deadtri(regiontris[i].tri)) {
          /* Put one triangle in the virus pool. */
          infect(regiontris[i]);
          regiontri = (triangle **) poolalloc(&m->viri);
          *regiontri = regiontris[i].tri;
          /* Apply one region's attribute and/or area constraint. */
          regionplague(m, b, regionlist[4 * i + 2], regionlist[4 * i + 3]);
          /* The virus pool should be empty now. */
        }
      }
    }
    if (b->regionattrib && !b->refine) {
      /* Note the fact that each triangle has an additional attribute. */
      m->eextras++;
    }
  }

  /* Free up memory. */
  if (((holes > 0) && !b->noholes) || !b->convex || (regions > 0)) {
    pooldeinit(&m->viri);
  }
  if (regions > 0) {
    trifree((void *) regiontris);
  }
}

/**                                                                         **/
/**                                                                         **/
/********* Carving out holes and concavities ends here               *********/

/********* Mesh quality maintenance begins here                      *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  tallyencs()   Traverse the entire list of subsegments, and check each    */
/*                to see if it is encroached.  If so, add it to the list.    */
/*                                                                           */
/*****************************************************************************/



/**                                                                         **/
/**                                                                         **/
/********* Mesh quality maintenance ends here                        *********/

/*****************************************************************************/
/*                                                                           */
/*  highorder()   Create extra nodes for quadratic subparametric elements.   */
/*                                                                           */
/*****************************************************************************/

void highorder(struct mesh *m, struct behavior *b)
{
  struct otri triangleloop, trisym;
  struct osub checkmark;
  vertex newvertex;
  vertex torg, tdest;
  int i;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  if (!b->quiet) {
    printf("Adding vertices for second-order triangles.\n");
  }
  /* The following line ensures that dead items in the pool of nodes    */
  /*   cannot be allocated for the extra nodes associated with high     */
  /*   order elements.  This ensures that the primary nodes (at the     */
  /*   corners of elements) will occur earlier in the output files, and */
  /*   have lower indices, than the extra nodes.                        */
  m->vertices.deaditemstack = (void *) NULL;

  traversalinit(&m->triangles);
  triangleloop.tri = triangletraverse(m);
  /* To loop over the set of edges, loop over all triangles, and look at   */
  /*   the three edges of each triangle.  If there isn't another triangle  */
  /*   adjacent to the edge, operate on the edge.  If there is another     */
  /*   adjacent triangle, operate on the edge only if the current triangle */
  /*   has a smaller pointer than its neighbor.  This way, each edge is    */
  /*   considered only once.                                               */
  while (triangleloop.tri != (triangle *) NULL) {
    for (triangleloop.orient = 0; triangleloop.orient < 3;
         triangleloop.orient++) {
      sym(triangleloop, trisym);
      if ((triangleloop.tri < trisym.tri) || (trisym.tri == m->dummytri)) {
        org(triangleloop, torg);
        dest(triangleloop, tdest);
        /* Create a new node in the middle of the edge.  Interpolate */
        /*   its attributes.                                         */
        newvertex = (vertex) poolalloc(&m->vertices);
        for (i = 0; i < 2 + m->nextras; i++) {
          newvertex[i] = 0.5 * (torg[i] + tdest[i]);
        }
        /* Set the new node's marker to zero or one, depending on */
        /*   whether it lies on a boundary.                       */
        setvertexmark(newvertex, trisym.tri == m->dummytri);
        setvertextype(newvertex,
                      trisym.tri == m->dummytri ? FREEVERTEX : SEGMENTVERTEX);
        if (b->usesegments) {
          tspivot(triangleloop, checkmark);
          /* If this edge is a segment, transfer the marker to the new node. */
          if (checkmark.ss != m->dummysub) {
            setvertexmark(newvertex, mark(checkmark));
            setvertextype(newvertex, SEGMENTVERTEX);
          }
        }
        if (b->verbose > 1) {
          printf("  Creating (%.12g, %.12g).\n", newvertex[0], newvertex[1]);
        }
        /* Record the new node in the (one or two) adjacent elements. */
        triangleloop.tri[m->highorderindex + triangleloop.orient] =
                (triangle) newvertex;
        if (trisym.tri != m->dummytri) {
          trisym.tri[m->highorderindex + trisym.orient] = (triangle) newvertex;
        }
      }
    }
    triangleloop.tri = triangletraverse(m);
  }
}



/*****************************************************************************/
/*                                                                           */
/*  removebox()   Remove the "infinite" bounding triangle, setting boundary  */
/*                markers as appropriate.                                    */
/*                                                                           */
/*  The triangular bounding box has three boundary triangles (one for each   */
/*  side of the bounding box), and a bunch of triangles fanning out from     */
/*  the three bounding box vertices (one triangle for each edge of the       */
/*  convex hull of the inner mesh).  This routine removes these triangles.   */
/*                                                                           */
/*  Returns the number of edges on the convex hull of the triangulation.     */
/*                                                                           */
/*****************************************************************************/

ULONGLONG removebox(struct mesh *m, struct behavior *b)
{
  struct otri deadtriangle;
  struct otri searchedge;
  struct otri checkedge;
  struct otri nextedge, finaledge, dissolveedge;
  vertex markorg;
  ULONGLONG hullsize;
  triangle ptr;                         /* Temporary variable used by sym(). */

  if (b->verbose) {
    printf("  Removing triangular bounding box.\n");
  }
  /* Find a boundary triangle. */
  nextedge.tri = m->dummytri;
  nextedge.orient = 0;
  symself(nextedge);
  /* Mark a place to stop. */
  lprev(nextedge, finaledge);
  lnextself(nextedge);
  symself(nextedge);
  /* Find a triangle (on the boundary of the vertex set) that isn't */
  /*   a bounding box triangle.                                     */
  lprev(nextedge, searchedge);
  symself(searchedge);
  /* Check whether nextedge is another boundary triangle */
  /*   adjacent to the first one.                        */
  lnext(nextedge, checkedge);
  symself(checkedge);
  if (checkedge.tri == m->dummytri) {
    /* Go on to the next triangle.  There are only three boundary   */
    /*   triangles, and this next triangle cannot be the third one, */
    /*   so it's safe to stop here.                                 */
    lprevself(searchedge);
    symself(searchedge);
  }
  /* Find a new boundary edge to search from, as the current search */
  /*   edge lies on a bounding box triangle and will be deleted.    */
  m->dummytri[0] = encode(searchedge);
  hullsize = -2l;
  while (!otriequal(nextedge, finaledge)) {
    hullsize++;
    lprev(nextedge, dissolveedge);
    symself(dissolveedge);
    /* If not using a PSLG, the vertices should be marked now. */
    /*   (If using a PSLG, markhull() will do the job.)        */
    if (!b->poly) {
      /* Be careful!  One must check for the case where all the input     */
      /*   vertices are collinear, and thus all the triangles are part of */
      /*   the bounding box.  Otherwise, the setvertexmark() call below   */
      /*   will cause a bad pointer reference.                            */
      if (dissolveedge.tri != m->dummytri) {
        org(dissolveedge, markorg);
        if (vertexmark(markorg) == 0) {
          setvertexmark(markorg, 1);
        }
      }
    }
    /* Disconnect the bounding box triangle from the mesh triangle. */
    dissolve(dissolveedge);
    lnext(nextedge, deadtriangle);
    sym(deadtriangle, nextedge);
    /* Get rid of the bounding box triangle. */
    triangledealloc(m, deadtriangle.tri);
    /* Do we need to turn the corner? */
    if (nextedge.tri == m->dummytri) {
      /* Turn the corner. */
      otricopy(dissolveedge, nextedge);
    }
  }
  triangledealloc(m, finaledge.tri);

  trifree((void *) m->infvertex1);  /* Deallocate the bounding box vertices. */
  trifree((void *) m->infvertex2);
  trifree((void *) m->infvertex3);

  return hullsize;
}

/*****************************************************************************/
/*                                                                           */
/*  incrementaldelaunay()   Form a Delaunay triangulation by incrementally   */
/*                          inserting vertices.                              */
/*                                                                           */
/*  Returns the number of edges on the convex hull of the triangulation.     */
/*                                                                           */
/*****************************************************************************/

ULONGLONG incrementaldelaunay(struct mesh *m, struct behavior *b)
{
  struct otri starttri;
  vertex vertexloop;

  /* Create a triangular bounding box. */
  boundingbox(m, b);
  if (b->verbose) {
    printf("  Incrementally inserting vertices.\n");
  }
  traversalinit(&m->vertices);
  vertexloop = vertextraverse(m);
  while (vertexloop != (vertex) NULL) {
    starttri.tri = m->dummytri;
    if (insertvertex(m, b, vertexloop, &starttri, (struct osub *) NULL, 0, 0)
        == DUPLICATEVERTEX) {
      if (!b->quiet) {
        printf(
"Warning:  A duplicate vertex at (%.12g, %.12g) appeared and was ignored.\n",
               vertexloop[0], vertexloop[1]);
      }
      setvertextype(vertexloop, UNDEADVERTEX);
      m->undeads++;
    }
    vertexloop = vertextraverse(m);
  }
  /* Remove the bounding box. */
  return removebox(m, b);
}

ULONGLONG sweeplinedelaunay(struct mesh *m, struct behavior *b)
{
  struct event **eventheap;
  struct event *events;
  struct event *freeevents;
  struct event *nextevent;
  struct event *newevent;
  struct splaynode *splayroot;
  struct otri bottommost;
  struct otri searchtri;
  struct otri fliptri;
  struct otri lefttri, righttri, farlefttri, farrighttri;
  struct otri inserttri;
  vertex firstvertex, secondvertex;
  vertex nextvertex, lastvertex;
  vertex connectvertex;
  vertex leftvertex, midvertex, rightvertex;
  REAL lefttest, righttest;
  int heapsize;
  int check4events, farrightflag;
  triangle ptr;   /* Temporary variable used by sym(), onext(), and oprev(). */

  poolinit(&m->splaynodes, sizeof(struct splaynode), SPLAYNODEPERBLOCK,
           SPLAYNODEPERBLOCK, 0);
  splayroot = (struct splaynode *) NULL;

  if (b->verbose) {
    printf("  Placing vertices in event heap.\n");
  }
  createeventheap(m, &eventheap, &events, &freeevents);
  heapsize = m->invertices;

  if (b->verbose) {
    printf("  Forming triangulation.\n");
  }
  maketriangle(m, b, &lefttri);
  maketriangle(m, b, &righttri);
  bond(lefttri, righttri);
  lnextself(lefttri);
  lprevself(righttri);
  bond(lefttri, righttri);
  lnextself(lefttri);
  lprevself(righttri);
  bond(lefttri, righttri);
  firstvertex = (vertex) eventheap[0]->eventptr;
  eventheap[0]->eventptr = (void *) freeevents;
  freeevents = eventheap[0];
  eventheapdelete(eventheap, heapsize, 0);
  heapsize--;
  do {
    if (heapsize == 0) {
      printf("Error:  Input vertices are all identical.\n");
      triexit(1);
    }
    secondvertex = (vertex) eventheap[0]->eventptr;
    eventheap[0]->eventptr = (void *) freeevents;
    freeevents = eventheap[0];
    eventheapdelete(eventheap, heapsize, 0);
    heapsize--;
    if ((firstvertex[0] == secondvertex[0]) &&
        (firstvertex[1] == secondvertex[1])) {
      if (!b->quiet) {
        printf(
"Warning:  A duplicate vertex at (%.12g, %.12g) appeared and was ignored.\n",
               secondvertex[0], secondvertex[1]);
      }
      setvertextype(secondvertex, UNDEADVERTEX);
      m->undeads++;
    }
  } while ((firstvertex[0] == secondvertex[0]) &&
           (firstvertex[1] == secondvertex[1]));
  setorg(lefttri, firstvertex);
  setdest(lefttri, secondvertex);
  setorg(righttri, secondvertex);
  setdest(righttri, firstvertex);
  lprev(lefttri, bottommost);
  lastvertex = secondvertex;
  while (heapsize > 0) {
    nextevent = eventheap[0];
    eventheapdelete(eventheap, heapsize, 0);
    heapsize--;
    check4events = 1;
    if (nextevent->xkey < m->xmin) {
      decode(nextevent->eventptr, fliptri);
      oprev(fliptri, farlefttri);
      check4deadevent(&farlefttri, &freeevents, eventheap, &heapsize);
      onext(fliptri, farrighttri);
      check4deadevent(&farrighttri, &freeevents, eventheap, &heapsize);

      if (otriequal(farlefttri, bottommost)) {
        lprev(fliptri, bottommost);
      }
      flip(m, b, &fliptri);
      setapex(fliptri, NULL);
      lprev(fliptri, lefttri);
      lnext(fliptri, righttri);
      sym(lefttri, farlefttri);

      if (randomnation(SAMPLERATE) == 0) {
        symself(fliptri);
        dest(fliptri, leftvertex);
        apex(fliptri, midvertex);
        org(fliptri, rightvertex);
        splayroot = circletopinsert(m, b, splayroot, &lefttri, leftvertex,
                                    midvertex, rightvertex, nextevent->ykey);
      }
    } else {
      nextvertex = (vertex) nextevent->eventptr;
      if ((nextvertex[0] == lastvertex[0]) &&
          (nextvertex[1] == lastvertex[1])) {
        if (!b->quiet) {
          printf(
"Warning:  A duplicate vertex at (%.12g, %.12g) appeared and was ignored.\n",
                 nextvertex[0], nextvertex[1]);
        }
        setvertextype(nextvertex, UNDEADVERTEX);
        m->undeads++;
        check4events = 0;
      } else {
        lastvertex = nextvertex;

        splayroot = frontlocate(m, splayroot, &bottommost, nextvertex,
                                &searchtri, &farrightflag);
/*
        otricopy(bottommost, searchtri);
        farrightflag = 0;
        while (!farrightflag && rightofhyperbola(m, &searchtri, nextvertex)) {
          onextself(searchtri);
          farrightflag = otriequal(searchtri, bottommost);
        }
*/

        check4deadevent(&searchtri, &freeevents, eventheap, &heapsize);

        otricopy(searchtri, farrighttri);
        sym(searchtri, farlefttri);
        maketriangle(m, b, &lefttri);
        maketriangle(m, b, &righttri);
        dest(farrighttri, connectvertex);
        setorg(lefttri, connectvertex);
        setdest(lefttri, nextvertex);
        setorg(righttri, nextvertex);
        setdest(righttri, connectvertex);
        bond(lefttri, righttri);
        lnextself(lefttri);
        lprevself(righttri);
        bond(lefttri, righttri);
        lnextself(lefttri);
        lprevself(righttri);
        bond(lefttri, farlefttri);
        bond(righttri, farrighttri);
        if (!farrightflag && otriequal(farrighttri, bottommost)) {
          otricopy(lefttri, bottommost);
        }

        if (randomnation(SAMPLERATE) == 0) {
          splayroot = splayinsert(m, splayroot, &lefttri, nextvertex);
        } else if (randomnation(SAMPLERATE) == 0) {
          lnext(righttri, inserttri);
          splayroot = splayinsert(m, splayroot, &inserttri, nextvertex);
        }
      }
    }
    nextevent->eventptr = (void *) freeevents;
    freeevents = nextevent;

    if (check4events) {
      apex(farlefttri, leftvertex);
      dest(lefttri, midvertex);
      apex(lefttri, rightvertex);
      lefttest = counterclockwise(m, b, leftvertex, midvertex, rightvertex);
      if (lefttest > 0.0) {
        newevent = freeevents;
        freeevents = (struct event *) freeevents->eventptr;
        newevent->xkey = m->xminextreme;
        newevent->ykey = circletop(m, leftvertex, midvertex, rightvertex,
                                   lefttest);
        newevent->eventptr = (void *) encode(lefttri);
        eventheapinsert(eventheap, heapsize, newevent);
        heapsize++;
        setorg(lefttri, newevent);
      }
      apex(righttri, leftvertex);
      org(righttri, midvertex);
      apex(farrighttri, rightvertex);
      righttest = counterclockwise(m, b, leftvertex, midvertex, rightvertex);
      if (righttest > 0.0) {
        newevent = freeevents;
        freeevents = (struct event *) freeevents->eventptr;
        newevent->xkey = m->xminextreme;
        newevent->ykey = circletop(m, leftvertex, midvertex, rightvertex,
                                   righttest);
        newevent->eventptr = (void *) encode(farrighttri);
        eventheapinsert(eventheap, heapsize, newevent);
        heapsize++;
        setorg(farrighttri, newevent);
      }
    }
  }

  pooldeinit(&m->splaynodes);
  lprevself(bottommost);
  return removeghosts(m, b, &bottommost);
}

struct splaynode *circletopinsert(struct mesh *m, struct behavior *b,
                                  struct splaynode *splayroot,
                                  struct otri *newkey,
                                  vertex pa, vertex pb, vertex pc, REAL topy)
{
  REAL ccwabc;
  REAL xac, yac, xbc, ybc;
  REAL aclen2, bclen2;
  REAL searchpoint[2];
  struct otri dummytri;

  ccwabc = counterclockwise(m, b, pa, pb, pc);
  xac = pa[0] - pc[0];
  yac = pa[1] - pc[1];
  xbc = pb[0] - pc[0];
  ybc = pb[1] - pc[1];
  aclen2 = xac * xac + yac * yac;
  bclen2 = xbc * xbc + ybc * ybc;
  searchpoint[0] = pc[0] - (yac * bclen2 - ybc * aclen2) / (2.0 * ccwabc);
  searchpoint[1] = topy;
  return splayinsert(m, splay(m, splayroot, (vertex) searchpoint, &dummytri),
                     newkey, (vertex) searchpoint);
}

void eventheapinsert(struct event **heap, int heapsize, struct event *newevent)
{
  REAL eventx, eventy;
  int eventnum;
  int parent;
  int notdone;

  eventx = newevent->xkey;
  eventy = newevent->ykey;
  eventnum = heapsize;
  notdone = eventnum > 0;
  while (notdone) {
    parent = (eventnum - 1) >> 1;
    if ((heap[parent]->ykey < eventy) ||
        ((heap[parent]->ykey == eventy)
         && (heap[parent]->xkey <= eventx))) {
      notdone = 0;
    } else {
      heap[eventnum] = heap[parent];
      heap[eventnum]->heapposition = eventnum;

      eventnum = parent;
      notdone = eventnum > 0;
    }
  }
  heap[eventnum] = newevent;
  newevent->heapposition = eventnum;
}

void eventheapify(struct event **heap, int heapsize, int eventnum)
{
  struct event *thisevent;
  REAL eventx, eventy;
  int leftchild, rightchild;
  int smallest;
  int notdone;

  thisevent = heap[eventnum];
  eventx = thisevent->xkey;
  eventy = thisevent->ykey;
  leftchild = 2 * eventnum + 1;
  notdone = leftchild < heapsize;
  while (notdone) {
    if ((heap[leftchild]->ykey < eventy) ||
        ((heap[leftchild]->ykey == eventy)
         && (heap[leftchild]->xkey < eventx))) {
      smallest = leftchild;
    } else {
      smallest = eventnum;
    }
    rightchild = leftchild + 1;
    if (rightchild < heapsize) {
      if ((heap[rightchild]->ykey < heap[smallest]->ykey) ||
          ((heap[rightchild]->ykey == heap[smallest]->ykey)
           && (heap[rightchild]->xkey < heap[smallest]->xkey))) {
        smallest = rightchild;
      }
    }
    if (smallest == eventnum) {
      notdone = 0;
    } else {
      heap[eventnum] = heap[smallest];
      heap[eventnum]->heapposition = eventnum;
      heap[smallest] = thisevent;
      thisevent->heapposition = smallest;

      eventnum = smallest;
      leftchild = 2 * eventnum + 1;
      notdone = leftchild < heapsize;
    }
  }
}

void eventheapdelete(struct event **heap, int heapsize, int eventnum)
{
  struct event *moveevent;
  REAL eventx, eventy;
  int parent;
  int notdone;

  moveevent = heap[heapsize - 1];
  if (eventnum > 0) {
    eventx = moveevent->xkey;
    eventy = moveevent->ykey;
    do {
      parent = (eventnum - 1) >> 1;
      if ((heap[parent]->ykey < eventy) ||
          ((heap[parent]->ykey == eventy)
           && (heap[parent]->xkey <= eventx))) {
        notdone = 0;
      } else {
        heap[eventnum] = heap[parent];
        heap[eventnum]->heapposition = eventnum;

        eventnum = parent;
        notdone = eventnum > 0;
      }
    } while (notdone);
  }
  heap[eventnum] = moveevent;
  moveevent->heapposition = eventnum;
  eventheapify(heap, heapsize - 1, eventnum);
}

void createeventheap(struct mesh *m, struct event ***eventheap,
                     struct event **events, struct event **freeevents)
{
  vertex thisvertex;
  int maxevents;
  int i;

  maxevents = (3 * m->invertices) / 2;
  *eventheap = (struct event **) trimalloc(maxevents *
                                           (int) sizeof(struct event *));
  *events = (struct event *) trimalloc(maxevents * (int) sizeof(struct event));
  traversalinit(&m->vertices);
  for (i = 0; i < m->invertices; i++) {
    thisvertex = vertextraverse(m);
    (*events)[i].eventptr = (void *) thisvertex;
    (*events)[i].xkey = thisvertex[0];
    (*events)[i].ykey = thisvertex[1];
    eventheapinsert(*eventheap, i, *events + i);
  }
  *freeevents = (struct event *) NULL;
  for (i = maxevents - 1; i >= m->invertices; i--) {
    (*events)[i].eventptr = (void *) *freeevents;
    *freeevents = *events + i;
  }
}

int rightofhyperbola(struct mesh *m, struct otri *fronttri, vertex newsite)
{
  vertex leftvertex, rightvertex;
  REAL dxa, dya, dxb, dyb;

  m->hyperbolacount++;

  dest(*fronttri, leftvertex);
  apex(*fronttri, rightvertex);
  if ((leftvertex[1] < rightvertex[1]) ||
      ((leftvertex[1] == rightvertex[1]) &&
       (leftvertex[0] < rightvertex[0]))) {
    if (newsite[0] >= rightvertex[0]) {
      return 1;
    }
  } else {
    if (newsite[0] <= leftvertex[0]) {
      return 0;
    }
  }
  dxa = leftvertex[0] - newsite[0];
  dya = leftvertex[1] - newsite[1];
  dxb = rightvertex[0] - newsite[0];
  dyb = rightvertex[1] - newsite[1];
  return dya * (dxb * dxb + dyb * dyb) > dyb * (dxa * dxa + dya * dya);
}

REAL circletop(struct mesh *m, vertex pa, vertex pb, vertex pc, REAL ccwabc)
{
  REAL xac, yac, xbc, ybc, xab, yab;
  REAL aclen2, bclen2, ablen2;

  m->circletopcount++;

  xac = pa[0] - pc[0];
  yac = pa[1] - pc[1];
  xbc = pb[0] - pc[0];
  ybc = pb[1] - pc[1];
  xab = pa[0] - pb[0];
  yab = pa[1] - pb[1];
  aclen2 = xac * xac + yac * yac;
  bclen2 = xbc * xbc + ybc * ybc;
  ablen2 = xab * xab + yab * yab;
  return pc[1] + (xac * bclen2 - xbc * aclen2 + sqrt(aclen2 * bclen2 * ablen2))
               / (2.0 * ccwabc);
}

void check4deadevent(struct otri *checktri, struct event **freeevents,
                     struct event **eventheap, int *heapsize)
{
  struct event *deadevent;
  vertex eventvertex;
  int eventnum;

  org(*checktri, eventvertex);
  if (eventvertex != (vertex) NULL) {
    deadevent = (struct event *) eventvertex;
    eventnum = deadevent->heapposition;
    deadevent->eventptr = (void *) *freeevents;
    *freeevents = deadevent;
    eventheapdelete(eventheap, *heapsize, eventnum);
    (*heapsize)--;
    setorg(*checktri, NULL);
  }
}

struct splaynode *splay(struct mesh *m, struct splaynode *splaytree,
                        vertex searchpoint, struct otri *searchtri)
{
  struct splaynode *child, *grandchild;
  struct splaynode *lefttree, *righttree;
  struct splaynode *leftright;
  vertex checkvertex;
  int rightofroot, rightofchild;

  if (splaytree == (struct splaynode *) NULL) {
    return (struct splaynode *) NULL;
  }
  dest(splaytree->keyedge, checkvertex);
  if (checkvertex == splaytree->keydest) {
    rightofroot = rightofhyperbola(m, &splaytree->keyedge, searchpoint);
    if (rightofroot) {
      otricopy(splaytree->keyedge, *searchtri);
      child = splaytree->rchild;
    } else {
      child = splaytree->lchild;
    }
    if (child == (struct splaynode *) NULL) {
      return splaytree;
    }
    dest(child->keyedge, checkvertex);
    if (checkvertex != child->keydest) {
      child = splay(m, child, searchpoint, searchtri);
      if (child == (struct splaynode *) NULL) {
        if (rightofroot) {
          splaytree->rchild = (struct splaynode *) NULL;
        } else {
          splaytree->lchild = (struct splaynode *) NULL;
        }
        return splaytree;
      }
    }
    rightofchild = rightofhyperbola(m, &child->keyedge, searchpoint);
    if (rightofchild) {
      otricopy(child->keyedge, *searchtri);
      grandchild = splay(m, child->rchild, searchpoint, searchtri);
      child->rchild = grandchild;
    } else {
      grandchild = splay(m, child->lchild, searchpoint, searchtri);
      child->lchild = grandchild;
    }
    if (grandchild == (struct splaynode *) NULL) {
      if (rightofroot) {
        splaytree->rchild = child->lchild;
        child->lchild = splaytree;
      } else {
        splaytree->lchild = child->rchild;
        child->rchild = splaytree;
      }
      return child;
    }
    if (rightofchild) {
      if (rightofroot) {
        splaytree->rchild = child->lchild;
        child->lchild = splaytree;
      } else {
        splaytree->lchild = grandchild->rchild;
        grandchild->rchild = splaytree;
      }
      child->rchild = grandchild->lchild;
      grandchild->lchild = child;
    } else {
      if (rightofroot) {
        splaytree->rchild = grandchild->lchild;
        grandchild->lchild = splaytree;
      } else {
        splaytree->lchild = child->rchild;
        child->rchild = splaytree;
      }
      child->lchild = grandchild->rchild;
      grandchild->rchild = child;
    }
    return grandchild;
  } else {
    lefttree = splay(m, splaytree->lchild, searchpoint, searchtri);
    righttree = splay(m, splaytree->rchild, searchpoint, searchtri);

    pooldealloc(&m->splaynodes, (void *) splaytree);
    if (lefttree == (struct splaynode *) NULL) {
      return righttree;
    } else if (righttree == (struct splaynode *) NULL) {
      return lefttree;
    } else if (lefttree->rchild == (struct splaynode *) NULL) {
      lefttree->rchild = righttree->lchild;
      righttree->lchild = lefttree;
      return righttree;
    } else if (righttree->lchild == (struct splaynode *) NULL) {
      righttree->lchild = lefttree->rchild;
      lefttree->rchild = righttree;
      return lefttree;
    } else {
/*      printf("Holy Toledo!!!\n"); */
      leftright = lefttree->rchild;
      while (leftright->rchild != (struct splaynode *) NULL) {
        leftright = leftright->rchild;
      }
      leftright->rchild = righttree;
      return lefttree;
    }
  }
}

struct splaynode *splayinsert(struct mesh *m, struct splaynode *splayroot,
                              struct otri *newkey, vertex searchpoint)
{
  struct splaynode *newsplaynode;

  newsplaynode = (struct splaynode *) poolalloc(&m->splaynodes);
  otricopy(*newkey, newsplaynode->keyedge);
  dest(*newkey, newsplaynode->keydest);
  if (splayroot == (struct splaynode *) NULL) {
    newsplaynode->lchild = (struct splaynode *) NULL;
    newsplaynode->rchild = (struct splaynode *) NULL;
  } else if (rightofhyperbola(m, &splayroot->keyedge, searchpoint)) {
    newsplaynode->lchild = splayroot;
    newsplaynode->rchild = splayroot->rchild;
    splayroot->rchild = (struct splaynode *) NULL;
  } else {
    newsplaynode->lchild = splayroot->lchild;
    newsplaynode->rchild = splayroot;
    splayroot->lchild = (struct splaynode *) NULL;
  }
  return newsplaynode;
}

struct splaynode *frontlocate(struct mesh *m, struct splaynode *splayroot,
                              struct otri *bottommost, vertex searchvertex,
                              struct otri *searchtri, int *farright)
{
  int farrightflag;
  triangle ptr;                       /* Temporary variable used by onext(). */

  otricopy(*bottommost, *searchtri);
  splayroot = splay(m, splayroot, searchvertex, searchtri);

  farrightflag = 0;
  while (!farrightflag && rightofhyperbola(m, searchtri, searchvertex)) {
    onextself(*searchtri);
    farrightflag = otriequal(*searchtri, *bottommost);
  }
  *farright = farrightflag;
  return splayroot;
}


