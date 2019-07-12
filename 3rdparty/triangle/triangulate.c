#include "triangulate.h"

#include "init.h"
#include "user.h"
#include "io.h"
#include "meshtransform.h"
#include "delauny.h"
#include "meshqual.h"
#include "mem.h"
#include "meshqual.h"
#include "cdt.h"
#include "pool.h"
#include "geom.h"

/*****************************************************************************/
/*                                                                           */
/*  triangulate()   Gosh, do everything.                           */
/*                                                                           */
/*  The sequence is roughly as follows.  Many of these steps can be skipped, */
/*  depending on the command line switches.                                  */
/*                                                                           */
/*  - Initialize constants and parse the command line.                       */
/*  - Read the vertices from a file and either                               */
/*    - triangulate them (no -r), or                                         */
/*    - read an old mesh from files and reconstruct it (-r).                 */
/*  - Insert the PSLG segments (-p), and possibly segments on the convex     */
/*      hull (-c).                                                           */
/*  - Read the holes (-p), regional attributes (-pA), and regional area      */
/*      constraints (-pa).  Carve the holes and concavities, and spread the  */
/*      regional attributes and area constraints.                            */
/*  - Enforce the constraints on minimum angle (-q) and maximum area (-a).   */
/*      Also enforce the conforming Delaunay property (-q and -a).           */
/*  - Compute the number of edges in the resulting mesh.                     */
/*  - Promote the mesh's linear triangles to higher order elements (-o).     */
/*  - Write the output files and print the statistics.                       */
/*  - Check the consistency and Delaunay property of the mesh (-C).          */
/*                                                                           */
/*****************************************************************************/

void triangulate(char *triswitches, struct triangulateio *in,
                 struct triangulateio *out, struct triangulateio *vorout)
{
  struct mesh m;
  struct behavior b;
  REAL *holearray;                                        /* Array of holes. */
  REAL *regionarray;   /* Array of regional attributes and area constraints. */

  triangleinit(&m);
  parsecommandline(1, &triswitches, &b);
  m.steinerleft = b.steiner;

  transfernodes(&m, &b, in->pointlist, in->pointattributelist,
                in->pointmarkerlist, in->numberofpoints,
                in->numberofpointattributes);

  if (b.refine) {
    /* Read and reconstruct a mesh. */
    m.hullsize = reconstruct(&m, &b, in->trianglelist,
                             in->triangleattributelist, in->trianglearealist,
                             in->numberoftriangles, in->numberofcorners,
                             in->numberoftriangleattributes,
                             in->segmentlist, in->segmentmarkerlist,
                             in->numberofsegments);
  } else {
    m.hullsize = delaunay(&m, &b);              /* Triangulate the vertices. */
  }

  /* Ensure that no vertex can be mistaken for a triangular bounding */
  /*   box vertex in insertvertex().                                 */
  m.infvertex1 = (vertex) NULL;
  m.infvertex2 = (vertex) NULL;
  m.infvertex3 = (vertex) NULL;

  if (b.usesegments) {
    m.checksegments = 1;                /* Segments will be introduced next. */
    if (!b.refine) {
      /* Insert PSLG segments and/or convex hull segments. */
      formskeleton(&m, &b, in->segmentlist,
                   in->segmentmarkerlist, in->numberofsegments);
    }
  }

  if (b.poly && (m.triangles.items > 0)) {
    holearray = in->holelist;
    m.holes = in->numberofholes;
    regionarray = in->regionlist;
    m.regions = in->numberofregions;
    if (!b.refine) {
      /* Carve out holes and concavities. */
      carveholes(&m, &b, holearray, m.holes, regionarray, m.regions);
    }
  } else {
    /* Without a PSLG, there can be no holes or regional attributes   */
    /*   or area constraints.  The following are set to zero to avoid */
    /*   an accidental free() later.                                  */
    m.holes = 0;
    m.regions = 0;
  }

  if (b.quality && (m.triangles.items > 0)) {
    enforcequality(&m, &b);           /* Enforce angle and area constraints. */
  }

  /* Calculate the number of edges. */
  m.edges = (3l * m.triangles.items + m.hullsize) / 2l;

  if (b.order > 1) {
    highorder(&m, &b);       /* Promote elements to higher polynomial order. */
  }
  if (!b.quiet) {
    printf("\n");
  }

  if (b.jettison) {
    out->numberofpoints = m.vertices.items - m.undeads;
  } else {
    out->numberofpoints = m.vertices.items;
  }
  out->numberofpointattributes = m.nextras;
  out->numberoftriangles = m.triangles.items;
  out->numberofcorners = (b.order + 1) * (b.order + 2) / 2;
  out->numberoftriangleattributes = m.eextras;
  out->numberofedges = m.edges;
  if (b.usesegments) {
    out->numberofsegments = m.subsegs.items;
  } else {
    out->numberofsegments = m.hullsize;
  }
  if (vorout != (struct triangulateio *) NULL) {
    vorout->numberofpoints = m.triangles.items;
    vorout->numberofpointattributes = m.nextras;
    vorout->numberofedges = m.edges;
  }
  /* If not using iteration numbers, don't write a .node file if one was */
  /*   read, because the original one would be overwritten!              */
  if (b.nonodewritten || (b.noiterationnum && m.readnodefile)) {
    if (!b.quiet) {
      printf("NOT writing vertices.\n");
    }
    numbernodes(&m, &b);         /* We must remember to number the vertices. */
  } else {
    /* writenodes() numbers the vertices too. */
    writenodes(&m, &b, &out->pointlist, &out->pointattributelist,
               &out->pointmarkerlist);
  }
  if (b.noelewritten) {
    if (!b.quiet) {
      printf("NOT writing triangles.\n");
    }
  } else {
    writeelements(&m, &b, &out->trianglelist, &out->triangleattributelist);
  }
  /* The -c switch (convex switch) causes a PSLG to be written */
  /*   even if none was read.                                  */
  if (b.poly || b.convex) {
    /* If not using iteration numbers, don't overwrite the .poly file. */
    if (b.nopolywritten || b.noiterationnum) {
      if (!b.quiet) {
        printf("NOT writing segments.\n");
      }
    } else {
      writepoly(&m, &b, &out->segmentlist, &out->segmentmarkerlist);
      out->numberofholes = m.holes;
      out->numberofregions = m.regions;
      if (b.poly) {
        out->holelist = in->holelist;
        out->regionlist = in->regionlist;
      } else {
        out->holelist = (REAL *) NULL;
        out->regionlist = (REAL *) NULL;
      }
    }
  }
  if (b.edgesout) {
    writeedges(&m, &b, &out->edgelist, &out->edgemarkerlist);
  }
  if (b.voronoi) {
    writevoronoi(&m, &b, &vorout->pointlist, &vorout->pointattributelist,
                 &vorout->pointmarkerlist, &vorout->edgelist,
                 &vorout->edgemarkerlist, &vorout->normlist);
  }
  if (b.neighbors) {
    writeneighbors(&m, &b, &out->neighborlist);
  }

  if (!b.quiet) {
    statistics(&m, &b);
  }

  if (b.docheck) {
    checkmesh(&m, &b);
    checkdelaunay(&m, &b);
  }

  triangledeinit(&m, &b);
}
