#include "locate.h"

#include "geom.h"
#include "mem.h"
#include "random.h"

#include <stdio.h>

/********* Point location routines begin here                        *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  makevertexmap()   Construct a mapping from vertices to triangles to      */
/*                    improve the speed of point location for segment        */
/*                    insertion.                                             */
/*                                                                           */
/*  Traverses all the triangles, and provides each corner of each triangle   */
/*  with a pointer to that triangle.  Of course, pointers will be            */
/*  overwritten by other pointers because (almost) each vertex is a corner   */
/*  of several triangles, but in the end every vertex will point to some     */
/*  triangle that contains it.                                               */
/*                                                                           */
/*****************************************************************************/

void makevertexmap(struct mesh *m, struct behavior *b)
{
  struct otri triangleloop;
  vertex triorg;

  if (b->verbose) {
    printf("    Constructing mapping from vertices to triangles.\n");
  }
  traversalinit(&m->triangles);
  triangleloop.tri = triangletraverse(m);
  while (triangleloop.tri != (triangle *) NULL) {
    /* Check all three vertices of the triangle. */
    for (triangleloop.orient = 0; triangleloop.orient < 3;
         triangleloop.orient++) {
      org(triangleloop, triorg);
      setvertex2tri(triorg, encode(triangleloop));
    }
    triangleloop.tri = triangletraverse(m);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  preciselocate()   Find a triangle or edge containing a given point.      */
/*                                                                           */
/*  Begins its search from `searchtri'.  It is important that `searchtri'    */
/*  be a handle with the property that `searchpoint' is strictly to the left */
/*  of the edge denoted by `searchtri', or is collinear with that edge and   */
/*  does not intersect that edge.  (In particular, `searchpoint' should not  */
/*  be the origin or destination of that edge.)                              */
/*                                                                           */
/*  These conditions are imposed because preciselocate() is normally used in */
/*  one of two situations:                                                   */
/*                                                                           */
/*  (1)  To try to find the location to insert a new point.  Normally, we    */
/*       know an edge that the point is strictly to the left of.  In the     */
/*       incremental Delaunay algorithm, that edge is a bounding box edge.   */
/*       In Ruppert's Delaunay refinement algorithm for quality meshing,     */
/*       that edge is the shortest edge of the triangle whose circumcenter   */
/*       is being inserted.                                                  */
/*                                                                           */
/*  (2)  To try to find an existing point.  In this case, any edge on the    */
/*       convex hull is a good starting edge.  You must screen out the       */
/*       possibility that the vertex sought is an endpoint of the starting   */
/*       edge before you call preciselocate().                               */
/*                                                                           */
/*  On completion, `searchtri' is a triangle that contains `searchpoint'.    */
/*                                                                           */
/*  This implementation differs from that given by Guibas and Stolfi.  It    */
/*  walks from triangle to triangle, crossing an edge only if `searchpoint'  */
/*  is on the other side of the line containing that edge.  After entering   */
/*  a triangle, there are two edges by which one can leave that triangle.    */
/*  If both edges are valid (`searchpoint' is on the other side of both      */
/*  edges), one of the two is chosen by drawing a line perpendicular to      */
/*  the entry edge (whose endpoints are `forg' and `fdest') passing through  */
/*  `fapex'.  Depending on which side of this perpendicular `searchpoint'    */
/*  falls on, an exit edge is chosen.                                        */
/*                                                                           */
/*  This implementation is empirically faster than the Guibas and Stolfi     */
/*  point location routine (which I originally used), which tends to spiral  */
/*  in toward its target.                                                    */
/*                                                                           */
/*  Returns ONVERTEX if the point lies on an existing vertex.  `searchtri'   */
/*  is a handle whose origin is the existing vertex.                         */
/*                                                                           */
/*  Returns ONEDGE if the point lies on a mesh edge.  `searchtri' is a       */
/*  handle whose primary edge is the edge on which the point lies.           */
/*                                                                           */
/*  Returns INTRIANGLE if the point lies strictly within a triangle.         */
/*  `searchtri' is a handle on the triangle that contains the point.         */
/*                                                                           */
/*  Returns OUTSIDE if the point lies outside the mesh.  `searchtri' is a    */
/*  handle whose primary edge the point is to the right of.  This might      */
/*  occur when the circumcenter of a triangle falls just slightly outside    */
/*  the mesh due to floating-point roundoff error.  It also occurs when      */
/*  seeking a hole or region point that a foolish user has placed outside    */
/*  the mesh.                                                                */
/*                                                                           */
/*  If `stopatsubsegment' is nonzero, the search will stop if it tries to    */
/*  walk through a subsegment, and will return OUTSIDE.                      */
/*                                                                           */
/*  WARNING:  This routine is designed for convex triangulations, and will   */
/*  not generally work after the holes and concavities have been carved.     */
/*  However, it can still be used to find the circumcenter of a triangle, as */
/*  ULONGLONG as the search is begun from the triangle in question.               */
/*                                                                           */
/*****************************************************************************/

enum locateresult preciselocate(struct mesh *m, struct behavior *b,
                                vertex searchpoint, struct otri *searchtri,
                                int stopatsubsegment)
{
  struct otri backtracktri;
  struct osub checkedge;
  vertex forg, fdest, fapex;
  REAL orgorient, destorient;
  int moveleft;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  if (b->verbose > 2) {
    printf("  Searching for point (%.12g, %.12g).\n",
           searchpoint[0], searchpoint[1]);
  }
  /* Where are we? */
  org(*searchtri, forg);
  dest(*searchtri, fdest);
  apex(*searchtri, fapex);
  while (1) {
    if (b->verbose > 2) {
      printf("    At (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
             forg[0], forg[1], fdest[0], fdest[1], fapex[0], fapex[1]);
    }
    /* Check whether the apex is the point we seek. */
    if ((fapex[0] == searchpoint[0]) && (fapex[1] == searchpoint[1])) {
      lprevself(*searchtri);
      return ONVERTEX;
    }
    /* Does the point lie on the other side of the line defined by the */
    /*   triangle edge opposite the triangle's destination?            */
destorient = counterclockwise(m, b, forg, fapex, searchpoint);
    /* Does the point lie on the other side of the line defined by the */
    /*   triangle edge opposite the triangle's origin?                 */
    orgorient = counterclockwise(m, b, fapex, fdest, searchpoint);
    if (destorient > 0.0) {
      if (orgorient > 0.0) {
        /* Move left if the inner product of (fapex - searchpoint) and  */
        /*   (fdest - forg) is positive.  This is equivalent to drawing */
        /*   a line perpendicular to the line (forg, fdest) and passing */
        /*   through `fapex', and determining which side of this line   */
        /*   `searchpoint' falls on.                                    */
        moveleft = (fapex[0] - searchpoint[0]) * (fdest[0] - forg[0]) +
                   (fapex[1] - searchpoint[1]) * (fdest[1] - forg[1]) > 0.0;
      } else {
        moveleft = 1;
      }
    } else {
      if (orgorient > 0.0) {
        moveleft = 0;
      } else {
        /* The point we seek must be on the boundary of or inside this */
        /*   triangle.                                                 */
        if (destorient == 0.0) {
          lprevself(*searchtri);
          return ONEDGE;
        }
        if (orgorient == 0.0) {
          lnextself(*searchtri);
          return ONEDGE;
        }
        return INTRIANGLE;
      }
    }

    /* Move to another triangle.  Leave a trace `backtracktri' in case */
    /*   floating-point roundoff or some such bogey causes us to walk  */
    /*   off a boundary of the triangulation.                          */
    if (moveleft) {
      lprev(*searchtri, backtracktri);
      fdest = fapex;
    } else {
      lnext(*searchtri, backtracktri);
      forg = fapex;
    }
    sym(backtracktri, *searchtri);

    if (m->checksegments && stopatsubsegment) {
      /* Check for walking through a subsegment. */
      tspivot(backtracktri, checkedge);
      if (checkedge.ss != m->dummysub) {
        /* Go back to the last triangle. */
        otricopy(backtracktri, *searchtri);
        return OUTSIDE;
      }
    }
    /* Check for walking right out of the triangulation. */
    if (searchtri->tri == m->dummytri) {
      /* Go back to the last triangle. */
      otricopy(backtracktri, *searchtri);
      return OUTSIDE;
    }

    apex(*searchtri, fapex);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  locate()   Find a triangle or edge containing a given point.             */
/*                                                                           */
/*  Searching begins from one of:  the input `searchtri', a recently         */
/*  encountered triangle `recenttri', or from a triangle chosen from a       */
/*  random sample.  The choice is made by determining which triangle's       */
/*  origin is closest to the point we are searching for.  Normally,          */
/*  `searchtri' should be a handle on the convex hull of the triangulation.  */
/*                                                                           */
/*  Details on the random sampling method can be found in the Mucke, Saias,  */
/*  and Zhu paper cited in the header of this code.                          */
/*                                                                           */
/*  On completion, `searchtri' is a triangle that contains `searchpoint'.    */
/*                                                                           */
/*  Returns ONVERTEX if the point lies on an existing vertex.  `searchtri'   */
/*  is a handle whose origin is the existing vertex.                         */
/*                                                                           */
/*  Returns ONEDGE if the point lies on a mesh edge.  `searchtri' is a       */
/*  handle whose primary edge is the edge on which the point lies.           */
/*                                                                           */
/*  Returns INTRIANGLE if the point lies strictly within a triangle.         */
/*  `searchtri' is a handle on the triangle that contains the point.         */
/*                                                                           */
/*  Returns OUTSIDE if the point lies outside the mesh.  `searchtri' is a    */
/*  handle whose primary edge the point is to the right of.  This might      */
/*  occur when the circumcenter of a triangle falls just slightly outside    */
/*  the mesh due to floating-point roundoff error.  It also occurs when      */
/*  seeking a hole or region point that a foolish user has placed outside    */
/*  the mesh.                                                                */
/*                                                                           */
/*  WARNING:  This routine is designed for convex triangulations, and will   */
/*  not generally work after the holes and concavities have been carved.     */
/*                                                                           */
/*****************************************************************************/

enum locateresult locate(struct mesh *m, struct behavior *b,
                         vertex searchpoint, struct otri *searchtri)
{
  void **sampleblock;
  char *firsttri;
  struct otri sampletri;
  vertex torg, tdest;
  unsigned ULONGLONG alignptr;
  REAL searchdist, dist;
  REAL ahead;
  ULONGLONG samplesperblock, totalsamplesleft, samplesleft;
  ULONGLONG population, totalpopulation;
  triangle ptr;                         /* Temporary variable used by sym(). */

  if (b->verbose > 2) {
    printf("  Randomly sampling for a triangle near point (%.12g, %.12g).\n",
           searchpoint[0], searchpoint[1]);
  }
  /* Record the distance from the suggested starting triangle to the */
  /*   point we seek.                                                */
  org(*searchtri, torg);
  searchdist = (searchpoint[0] - torg[0]) * (searchpoint[0] - torg[0]) +
               (searchpoint[1] - torg[1]) * (searchpoint[1] - torg[1]);
  if (b->verbose > 2) {
    printf("    Boundary triangle has origin (%.12g, %.12g).\n",
           torg[0], torg[1]);
  }

  /* If a recently encountered triangle has been recorded and has not been */
  /*   deallocated, test it as a good starting point.                      */
  if (m->recenttri.tri != (triangle *) NULL) {
    if (!deadtri(m->recenttri.tri)) {
      org(m->recenttri, torg);
      if ((torg[0] == searchpoint[0]) && (torg[1] == searchpoint[1])) {
        otricopy(m->recenttri, *searchtri);
        return ONVERTEX;
      }
      dist = (searchpoint[0] - torg[0]) * (searchpoint[0] - torg[0]) +
             (searchpoint[1] - torg[1]) * (searchpoint[1] - torg[1]);
      if (dist < searchdist) {
        otricopy(m->recenttri, *searchtri);
        searchdist = dist;
        if (b->verbose > 2) {
          printf("    Choosing recent triangle with origin (%.12g, %.12g).\n",
                 torg[0], torg[1]);
        }
      }
    }
  }

  /* The number of random samples taken is proportional to the cube root of */
  /*   the number of triangles in the mesh.  The next bit of code assumes   */
  /*   that the number of triangles increases monotonically (or at least    */
  /*   doesn't decrease enough to matter).                                  */
  while (SAMPLEFACTOR * m->samples * m->samples * m->samples <
         m->triangles.items) {
    m->samples++;
  }

  /* We'll draw ceiling(samples * TRIPERBLOCK / maxitems) random samples  */
  /*   from each block of triangles (except the first)--until we meet the */
  /*   sample quota.  The ceiling means that blocks at the end might be   */
  /*   neglected, but I don't care.                                       */
  samplesperblock = (m->samples * TRIPERBLOCK - 1) / m->triangles.maxitems + 1;
  /* We'll draw ceiling(samples * itemsfirstblock / maxitems) random samples */
  /*   from the first block of triangles.                                    */
  samplesleft = (m->samples * m->triangles.itemsfirstblock - 1) /
                m->triangles.maxitems + 1;
  totalsamplesleft = m->samples;
  population = m->triangles.itemsfirstblock;
  totalpopulation = m->triangles.maxitems;
  sampleblock = m->triangles.firstblock;
  sampletri.orient = 0;
  while (totalsamplesleft > 0) {
    /* If we're in the last block, `population' needs to be corrected. */
    if (population > totalpopulation) {
      population = totalpopulation;
    }
    /* Find a pointer to the first triangle in the block. */
    alignptr = (unsigned ULONGLONG) (sampleblock + 1);
    firsttri = (char *) (alignptr +
                         (unsigned ULONGLONG) m->triangles.alignbytes -
                         (alignptr %
                          (unsigned ULONGLONG) m->triangles.alignbytes));

    /* Choose `samplesleft' randomly sampled triangles in this block. */
    do {
      sampletri.tri = (triangle *) (firsttri +
                                    (randomnation((unsigned int) population) *
                                     m->triangles.itembytes));
      if (!deadtri(sampletri.tri)) {
        org(sampletri, torg);
        dist = (searchpoint[0] - torg[0]) * (searchpoint[0] - torg[0]) +
               (searchpoint[1] - torg[1]) * (searchpoint[1] - torg[1]);
        if (dist < searchdist) {
          otricopy(sampletri, *searchtri);
          searchdist = dist;
          if (b->verbose > 2) {
            printf("    Choosing triangle with origin (%.12g, %.12g).\n",
                   torg[0], torg[1]);
          }
        }
      }

      samplesleft--;
      totalsamplesleft--;
    } while ((samplesleft > 0) && (totalsamplesleft > 0));

    if (totalsamplesleft > 0) {
      sampleblock = (void **) *sampleblock;
      samplesleft = samplesperblock;
      totalpopulation -= population;
      population = TRIPERBLOCK;
    }
  }

  /* Where are we? */
  org(*searchtri, torg);
  dest(*searchtri, tdest);
  /* Check the starting triangle's vertices. */
  if ((torg[0] == searchpoint[0]) && (torg[1] == searchpoint[1])) {
    return ONVERTEX;
  }
  if ((tdest[0] == searchpoint[0]) && (tdest[1] == searchpoint[1])) {
    lnextself(*searchtri);
    return ONVERTEX;
  }
  /* Orient `searchtri' to fit the preconditions of calling preciselocate(). */
  ahead = counterclockwise(m, b, torg, tdest, searchpoint);
  if (ahead < 0.0) {
    /* Turn around so that `searchpoint' is to the left of the */
    /*   edge specified by `searchtri'.                        */
    symself(*searchtri);
  } else if (ahead == 0.0) {
    /* Check if `searchpoint' is between `torg' and `tdest'. */
    if (((torg[0] < searchpoint[0]) == (searchpoint[0] < tdest[0])) &&
        ((torg[1] < searchpoint[1]) == (searchpoint[1] < tdest[1]))) {
      return ONEDGE;
    }
  }
  return preciselocate(m, b, searchpoint, searchtri, 0);
}

/**                                                                         **/
/**                                                                         **/
/********* Point location routines end here                          *********/
