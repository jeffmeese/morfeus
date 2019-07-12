#include "meshtransform.h"

#include "geom.h"
#include "primitives.h"
#include "construct.h"
#include "debug.h"
#include "locate.h"
#include "pool.h"
#include "cdt.h"
#include "mem.h"

#include <stdio.h>

/********* Mesh transformation routines begin here                   *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  insertsubseg()   Create a new subsegment and insert it between two       */
/*                   triangles.                                              */
/*                                                                           */
/*  The new subsegment is inserted at the edge described by the handle       */
/*  `tri'.  Its vertices are properly initialized.  The marker `subsegmark'  */
/*  is applied to the subsegment and, if appropriate, its vertices.          */
/*                                                                           */
/*****************************************************************************/

void insertsubseg(struct mesh *m, struct behavior *b, struct otri *tri, int subsegmark)
{
  struct otri oppotri;
  struct osub newsubseg;
  vertex triorg, tridest;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  org(*tri, triorg);
  dest(*tri, tridest);
  /* Mark vertices if possible. */
  if (vertexmark(triorg) == 0) {
    setvertexmark(triorg, subsegmark);
  }
  if (vertexmark(tridest) == 0) {
    setvertexmark(tridest, subsegmark);
  }
  /* Check if there's already a subsegment here. */
  tspivot(*tri, newsubseg);
  if (newsubseg.ss == m->dummysub) {
    /* Make new subsegment and initialize its vertices. */
    makesubseg(m, &newsubseg);
    setsorg(newsubseg, tridest);
    setsdest(newsubseg, triorg);
    setsegorg(newsubseg, tridest);
    setsegdest(newsubseg, triorg);
    /* Bond new subsegment to the two triangles it is sandwiched between. */
    /*   Note that the facing triangle `oppotri' might be equal to        */
    /*   `dummytri' (outer space), but the new subsegment is bonded to it */
    /*   all the same.                                                    */
    tsbond(*tri, newsubseg);
    sym(*tri, oppotri);
    ssymself(newsubseg);
    tsbond(oppotri, newsubseg);
    setmark(newsubseg, subsegmark);
    if (b->verbose > 2) {
      printf("  Inserting new ");
      printsubseg(m, b, &newsubseg);
    }
  } else {
    if (mark(newsubseg) == 0) {
      setmark(newsubseg, subsegmark);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  Terminology                                                              */
/*                                                                           */
/*  A "local transformation" replaces a small set of triangles with another  */
/*  set of triangles.  This may or may not involve inserting or deleting a   */
/*  vertex.                                                                  */
/*                                                                           */
/*  The term "casing" is used to describe the set of triangles that are      */
/*  attached to the triangles being transformed, but are not transformed     */
/*  themselves.  Think of the casing as a fixed hollow structure inside      */
/*  which all the action happens.  A "casing" is only defined relative to    */
/*  a single transformation; each occurrence of a transformation will        */
/*  involve a different casing.                                              */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  flip()   Transform two triangles to two different triangles by flipping  */
/*           an edge counterclockwise within a quadrilateral.                */
/*                                                                           */
/*  Imagine the original triangles, abc and bad, oriented so that the        */
/*  shared edge ab lies in a horizontal plane, with the vertex b on the left */
/*  and the vertex a on the right.  The vertex c lies below the edge, and    */
/*  the vertex d lies above the edge.  The `flipedge' handle holds the edge  */
/*  ab of triangle abc, and is directed left, from vertex a to vertex b.     */
/*                                                                           */
/*  The triangles abc and bad are deleted and replaced by the triangles cdb  */
/*  and dca.  The triangles that represent abc and bad are NOT deallocated;  */
/*  they are reused for dca and cdb, respectively.  Hence, any handles that  */
/*  may have held the original triangles are still valid, although not       */
/*  directed as they were before.                                            */
/*                                                                           */
/*  Upon completion of this routine, the `flipedge' handle holds the edge    */
/*  dc of triangle dca, and is directed down, from vertex d to vertex c.     */
/*  (Hence, the two triangles have rotated counterclockwise.)                */
/*                                                                           */
/*  WARNING:  This transformation is geometrically valid only if the         */
/*  quadrilateral adbc is convex.  Furthermore, this transformation is       */
/*  valid only if there is not a subsegment between the triangles abc and    */
/*  bad.  This routine does not check either of these preconditions, and     */
/*  it is the responsibility of the calling routine to ensure that they are  */
/*  met.  If they are not, the streets shall be filled with wailing and      */
/*  gnashing of teeth.                                                       */
/*                                                                           */
/*****************************************************************************/

void flip(struct mesh *m, struct behavior *b, struct otri *flipedge)
{
  struct otri botleft, botright;
  struct otri topleft, topright;
  struct otri top;
  struct otri botlcasing, botrcasing;
  struct otri toplcasing, toprcasing;
  struct osub botlsubseg, botrsubseg;
  struct osub toplsubseg, toprsubseg;
  vertex leftvertex, rightvertex, botvertex;
  vertex farvertex;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  /* Identify the vertices of the quadrilateral. */
  org(*flipedge, rightvertex);
  dest(*flipedge, leftvertex);
  apex(*flipedge, botvertex);
  sym(*flipedge, top);
#ifdef SELF_CHECK
  if (top.tri == m->dummytri) {
    printf("Internal error in flip():  Attempt to flip on boundary.\n");
    lnextself(*flipedge);
    return;
  }
  if (m->checksegments) {
    tspivot(*flipedge, toplsubseg);
    if (toplsubseg.ss != m->dummysub) {
      printf("Internal error in flip():  Attempt to flip a segment.\n");
      lnextself(*flipedge);
      return;
    }
  }
#endif /* SELF_CHECK */
  apex(top, farvertex);

  /* Identify the casing of the quadrilateral. */
  lprev(top, topleft);
  sym(topleft, toplcasing);
  lnext(top, topright);
  sym(topright, toprcasing);
  lnext(*flipedge, botleft);
  sym(botleft, botlcasing);
  lprev(*flipedge, botright);
  sym(botright, botrcasing);
  /* Rotate the quadrilateral one-quarter turn counterclockwise. */
  bond(topleft, botlcasing);
  bond(botleft, botrcasing);
  bond(botright, toprcasing);
  bond(topright, toplcasing);

  if (m->checksegments) {
    /* Check for subsegments and rebond them to the quadrilateral. */
    tspivot(topleft, toplsubseg);
    tspivot(botleft, botlsubseg);
    tspivot(botright, botrsubseg);
    tspivot(topright, toprsubseg);
    if (toplsubseg.ss == m->dummysub) {
      tsdissolve(topright);
    } else {
      tsbond(topright, toplsubseg);
    }
    if (botlsubseg.ss == m->dummysub) {
      tsdissolve(topleft);
    } else {
      tsbond(topleft, botlsubseg);
    }
    if (botrsubseg.ss == m->dummysub) {
      tsdissolve(botleft);
    } else {
      tsbond(botleft, botrsubseg);
    }
    if (toprsubseg.ss == m->dummysub) {
      tsdissolve(botright);
    } else {
      tsbond(botright, toprsubseg);
    }
  }

  /* New vertex assignments for the rotated quadrilateral. */
  setorg(*flipedge, farvertex);
  setdest(*flipedge, botvertex);
  setapex(*flipedge, rightvertex);
  setorg(top, botvertex);
  setdest(top, farvertex);
  setapex(top, leftvertex);
  if (b->verbose > 2) {
    printf("  Edge flip results in left ");
    printtriangle(m, b, &top);
    printf("  and right ");
    printtriangle(m, b, flipedge);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  unflip()   Transform two triangles to two different triangles by         */
/*             flipping an edge clockwise within a quadrilateral.  Reverses  */
/*             the flip() operation so that the data structures representing */
/*             the triangles are back where they were before the flip().     */
/*                                                                           */
/*  Imagine the original triangles, abc and bad, oriented so that the        */
/*  shared edge ab lies in a horizontal plane, with the vertex b on the left */
/*  and the vertex a on the right.  The vertex c lies below the edge, and    */
/*  the vertex d lies above the edge.  The `flipedge' handle holds the edge  */
/*  ab of triangle abc, and is directed left, from vertex a to vertex b.     */
/*                                                                           */
/*  The triangles abc and bad are deleted and replaced by the triangles cdb  */
/*  and dca.  The triangles that represent abc and bad are NOT deallocated;  */
/*  they are reused for cdb and dca, respectively.  Hence, any handles that  */
/*  may have held the original triangles are still valid, although not       */
/*  directed as they were before.                                            */
/*                                                                           */
/*  Upon completion of this routine, the `flipedge' handle holds the edge    */
/*  cd of triangle cdb, and is directed up, from vertex c to vertex d.       */
/*  (Hence, the two triangles have rotated clockwise.)                       */
/*                                                                           */
/*  WARNING:  This transformation is geometrically valid only if the         */
/*  quadrilateral adbc is convex.  Furthermore, this transformation is       */
/*  valid only if there is not a subsegment between the triangles abc and    */
/*  bad.  This routine does not check either of these preconditions, and     */
/*  it is the responsibility of the calling routine to ensure that they are  */
/*  met.  If they are not, the streets shall be filled with wailing and      */
/*  gnashing of teeth.                                                       */
/*                                                                           */
/*****************************************************************************/

void unflip(struct mesh *m, struct behavior *b, struct otri *flipedge)
{
  struct otri botleft, botright;
  struct otri topleft, topright;
  struct otri top;
  struct otri botlcasing, botrcasing;
  struct otri toplcasing, toprcasing;
  struct osub botlsubseg, botrsubseg;
  struct osub toplsubseg, toprsubseg;
  vertex leftvertex, rightvertex, botvertex;
  vertex farvertex;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  /* Identify the vertices of the quadrilateral. */
  org(*flipedge, rightvertex);
  dest(*flipedge, leftvertex);
  apex(*flipedge, botvertex);
  sym(*flipedge, top);
#ifdef SELF_CHECK
  if (top.tri == m->dummytri) {
    printf("Internal error in unflip():  Attempt to flip on boundary.\n");
    lnextself(*flipedge);
    return;
  }
  if (m->checksegments) {
    tspivot(*flipedge, toplsubseg);
    if (toplsubseg.ss != m->dummysub) {
      printf("Internal error in unflip():  Attempt to flip a subsegment.\n");
      lnextself(*flipedge);
      return;
    }
  }
#endif /* SELF_CHECK */
  apex(top, farvertex);

  /* Identify the casing of the quadrilateral. */
  lprev(top, topleft);
  sym(topleft, toplcasing);
  lnext(top, topright);
  sym(topright, toprcasing);
  lnext(*flipedge, botleft);
  sym(botleft, botlcasing);
  lprev(*flipedge, botright);
  sym(botright, botrcasing);
  /* Rotate the quadrilateral one-quarter turn clockwise. */
  bond(topleft, toprcasing);
  bond(botleft, toplcasing);
  bond(botright, botlcasing);
  bond(topright, botrcasing);

  if (m->checksegments) {
    /* Check for subsegments and rebond them to the quadrilateral. */
    tspivot(topleft, toplsubseg);
    tspivot(botleft, botlsubseg);
    tspivot(botright, botrsubseg);
    tspivot(topright, toprsubseg);
    if (toplsubseg.ss == m->dummysub) {
      tsdissolve(botleft);
    } else {
      tsbond(botleft, toplsubseg);
    }
    if (botlsubseg.ss == m->dummysub) {
      tsdissolve(botright);
    } else {
      tsbond(botright, botlsubseg);
    }
    if (botrsubseg.ss == m->dummysub) {
      tsdissolve(topright);
    } else {
      tsbond(topright, botrsubseg);
    }
    if (toprsubseg.ss == m->dummysub) {
      tsdissolve(topleft);
    } else {
      tsbond(topleft, toprsubseg);
    }
  }

  /* New vertex assignments for the rotated quadrilateral. */
  setorg(*flipedge, botvertex);
  setdest(*flipedge, farvertex);
  setapex(*flipedge, leftvertex);
  setorg(top, farvertex);
  setdest(top, botvertex);
  setapex(top, rightvertex);
  if (b->verbose > 2) {
    printf("  Edge unflip results in left ");
    printtriangle(m, b, flipedge);
    printf("  and right ");
    printtriangle(m, b, &top);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  insertvertex()   Insert a vertex into a Delaunay triangulation,          */
/*                   performing flips as necessary to maintain the Delaunay  */
/*                   property.                                               */
/*                                                                           */
/*  The point `insertvertex' is located.  If `searchtri.tri' is not NULL,    */
/*  the search for the containing triangle begins from `searchtri'.  If      */
/*  `searchtri.tri' is NULL, a full point location procedure is called.      */
/*  If `insertvertex' is found inside a triangle, the triangle is split into */
/*  three; if `insertvertex' lies on an edge, the edge is split in two,      */
/*  thereby splitting the two adjacent triangles into four.  Edge flips are  */
/*  used to restore the Delaunay property.  If `insertvertex' lies on an     */
/*  existing vertex, no action is taken, and the value DUPLICATEVERTEX is    */
/*  returned.  On return, `searchtri' is set to a handle whose origin is the */
/*  existing vertex.                                                         */
/*                                                                           */
/*  Normally, the parameter `splitseg' is set to NULL, implying that no      */
/*  subsegment should be split.  In this case, if `insertvertex' is found to */
/*  lie on a segment, no action is taken, and the value VIOLATINGVERTEX is   */
/*  returned.  On return, `searchtri' is set to a handle whose primary edge  */
/*  is the violated subsegment.                                              */
/*                                                                           */
/*  If the calling routine wishes to split a subsegment by inserting a       */
/*  vertex in it, the parameter `splitseg' should be that subsegment.  In    */
/*  this case, `searchtri' MUST be the triangle handle reached by pivoting   */
/*  from that subsegment; no point location is done.                         */
/*                                                                           */
/*  `segmentflaws' and `triflaws' are flags that indicate whether or not     */
/*  there should be checks for the creation of encroached subsegments or bad */
/*  quality triangles.  If a newly inserted vertex encroaches upon           */
/*  subsegments, these subsegments are added to the list of subsegments to   */
/*  be split if `segmentflaws' is set.  If bad triangles are created, these  */
/*  are added to the queue if `triflaws' is set.                             */
/*                                                                           */
/*  If a duplicate vertex or violated segment does not prevent the vertex    */
/*  from being inserted, the return value will be ENCROACHINGVERTEX if the   */
/*  vertex encroaches upon a subsegment (and checking is enabled), or        */
/*  SUCCESSFULVERTEX otherwise.  In either case, `searchtri' is set to a     */
/*  handle whose origin is the newly inserted vertex.                        */
/*                                                                           */
/*  insertvertex() does not use flip() for reasons of speed; some            */
/*  information can be reused from edge flip to edge flip, like the          */
/*  locations of subsegments.                                                */
/*                                                                           */
/*****************************************************************************/

enum insertvertexresult insertvertex(struct mesh *m, struct behavior *b,
                                     vertex newvertex, struct otri *searchtri,
                                     struct osub *splitseg,
                                     int segmentflaws, int triflaws)
{
  struct otri horiz;
  struct otri top;
  struct otri botleft, botright;
  struct otri topleft, topright;
  struct otri newbotleft, newbotright;
  struct otri newtopright;
  struct otri botlcasing, botrcasing;
  struct otri toplcasing, toprcasing;
  struct otri testtri;
  struct osub botlsubseg, botrsubseg;
  struct osub toplsubseg, toprsubseg;
  struct osub brokensubseg;
  struct osub checksubseg;
  struct osub rightsubseg;
  struct osub newsubseg;
  struct badsubseg *encroached;
  struct flipstacker *newflip;
  vertex first;
  vertex leftvertex, rightvertex, botvertex, topvertex, farvertex;
  vertex segmentorg, segmentdest;
  REAL attrib;
  REAL area;
  enum insertvertexresult success;
  enum locateresult intersect;
  int doflip;
  int mirrorflag;
  int enq;
  int i;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;         /* Temporary variable used by spivot() and tspivot(). */

  if (b->verbose > 1) {
    printf("  Inserting (%.12g, %.12g).\n", newvertex[0], newvertex[1]);
  }

  if (splitseg == (struct osub *) NULL) {
    /* Find the location of the vertex to be inserted.  Check if a good */
    /*   starting triangle has already been provided by the caller.     */
    if (searchtri->tri == m->dummytri) {
      /* Find a boundary triangle. */
      horiz.tri = m->dummytri;
      horiz.orient = 0;
      symself(horiz);
      /* Search for a triangle containing `newvertex'. */
      intersect = locate(m, b, newvertex, &horiz);
    } else {
      /* Start searching from the triangle provided by the caller. */
      otricopy(*searchtri, horiz);
      intersect = preciselocate(m, b, newvertex, &horiz, 1);
    }
  } else {
    /* The calling routine provides the subsegment in which */
    /*   the vertex is inserted.                             */
    otricopy(*searchtri, horiz);
    intersect = ONEDGE;
  }

  if (intersect == ONVERTEX) {
    /* There's already a vertex there.  Return in `searchtri' a triangle */
    /*   whose origin is the existing vertex.                            */
    otricopy(horiz, *searchtri);
    otricopy(horiz, m->recenttri);
    return DUPLICATEVERTEX;
  }
  if ((intersect == ONEDGE) || (intersect == OUTSIDE)) {
    /* The vertex falls on an edge or boundary. */
    if (m->checksegments && (splitseg == (struct osub *) NULL)) {
      /* Check whether the vertex falls on a subsegment. */
      tspivot(horiz, brokensubseg);
      if (brokensubseg.ss != m->dummysub) {
        /* The vertex falls on a subsegment, and hence will not be inserted. */
        if (segmentflaws) {
          enq = b->nobisect != 2;
          if (enq && (b->nobisect == 1)) {
            /* This subsegment may be split only if it is an */
            /*   internal boundary.                          */
            sym(horiz, testtri);
            enq = testtri.tri != m->dummytri;
          }
          if (enq) {
            /* Add the subsegment to the list of encroached subsegments. */
            encroached = (struct badsubseg *) poolalloc(&m->badsubsegs);
            encroached->encsubseg = sencode(brokensubseg);
            sorg(brokensubseg, encroached->subsegorg);
            sdest(brokensubseg, encroached->subsegdest);
            if (b->verbose > 2) {
              printf(
          "  Queueing encroached subsegment (%.12g, %.12g) (%.12g, %.12g).\n",
                     encroached->subsegorg[0], encroached->subsegorg[1],
                     encroached->subsegdest[0], encroached->subsegdest[1]);
            }
          }
        }
        /* Return a handle whose primary edge contains the vertex, */
        /*   which has not been inserted.                          */
        otricopy(horiz, *searchtri);
        otricopy(horiz, m->recenttri);
        return VIOLATINGVERTEX;
      }
    }

    /* Insert the vertex on an edge, dividing one triangle into two (if */
    /*   the edge lies on a boundary) or two triangles into four.       */
    lprev(horiz, botright);
    sym(botright, botrcasing);
    sym(horiz, topright);
    /* Is there a second triangle?  (Or does this edge lie on a boundary?) */
    mirrorflag = topright.tri != m->dummytri;
    if (mirrorflag) {
      lnextself(topright);
      sym(topright, toprcasing);
      maketriangle(m, b, &newtopright);
    } else {
      /* Splitting a boundary edge increases the number of boundary edges. */
      m->hullsize++;
    }
    maketriangle(m, b, &newbotright);

    /* Set the vertices of changed and new triangles. */
    org(horiz, rightvertex);
    dest(horiz, leftvertex);
    apex(horiz, botvertex);
    setorg(newbotright, botvertex);
    setdest(newbotright, rightvertex);
    setapex(newbotright, newvertex);
    setorg(horiz, newvertex);
    for (i = 0; i < m->eextras; i++) {
      /* Set the element attributes of a new triangle. */
      setelemattribute(newbotright, i, elemattribute(botright, i));
    }
    if (b->vararea) {
      /* Set the area constraint of a new triangle. */
      setareabound(newbotright, areabound(botright));
    }
    if (mirrorflag) {
      dest(topright, topvertex);
      setorg(newtopright, rightvertex);
      setdest(newtopright, topvertex);
      setapex(newtopright, newvertex);
      setorg(topright, newvertex);
      for (i = 0; i < m->eextras; i++) {
        /* Set the element attributes of another new triangle. */
        setelemattribute(newtopright, i, elemattribute(topright, i));
      }
      if (b->vararea) {
        /* Set the area constraint of another new triangle. */
        setareabound(newtopright, areabound(topright));
      }
    }

    /* There may be subsegments that need to be bonded */
    /*   to the new triangle(s).                       */
    if (m->checksegments) {
      tspivot(botright, botrsubseg);
      if (botrsubseg.ss != m->dummysub) {
        tsdissolve(botright);
        tsbond(newbotright, botrsubseg);
      }
      if (mirrorflag) {
        tspivot(topright, toprsubseg);
        if (toprsubseg.ss != m->dummysub) {
          tsdissolve(topright);
          tsbond(newtopright, toprsubseg);
        }
      }
    }

    /* Bond the new triangle(s) to the surrounding triangles. */
    bond(newbotright, botrcasing);
    lprevself(newbotright);
    bond(newbotright, botright);
    lprevself(newbotright);
    if (mirrorflag) {
      bond(newtopright, toprcasing);
      lnextself(newtopright);
      bond(newtopright, topright);
      lnextself(newtopright);
      bond(newtopright, newbotright);
    }

    if (splitseg != (struct osub *) NULL) {
      /* Split the subsegment into two. */
      setsdest(*splitseg, newvertex);
      segorg(*splitseg, segmentorg);
      segdest(*splitseg, segmentdest);
      ssymself(*splitseg);
      spivot(*splitseg, rightsubseg);
      insertsubseg(m, b, &newbotright, mark(*splitseg));
      tspivot(newbotright, newsubseg);
      setsegorg(newsubseg, segmentorg);
      setsegdest(newsubseg, segmentdest);
      sbond(*splitseg, newsubseg);
      ssymself(newsubseg);
      sbond(newsubseg, rightsubseg);
      ssymself(*splitseg);
      /* Transfer the subsegment's boundary marker to the vertex */
      /*   if required.                                          */
      if (vertexmark(newvertex) == 0) {
        setvertexmark(newvertex, mark(*splitseg));
      }
    }

    if (m->checkquality) {
      poolrestart(&m->flipstackers);
      m->lastflip = (struct flipstacker *) poolalloc(&m->flipstackers);
      m->lastflip->flippedtri = encode(horiz);
      m->lastflip->prevflip = (struct flipstacker *) &insertvertex;
    }

#ifdef SELF_CHECK
    if (counterclockwise(m, b, rightvertex, leftvertex, botvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf(
            "  Clockwise triangle prior to edge vertex insertion (bottom).\n");
    }
    if (mirrorflag) {
      if (counterclockwise(m, b, leftvertex, rightvertex, topvertex) < 0.0) {
        printf("Internal error in insertvertex():\n");
        printf("  Clockwise triangle prior to edge vertex insertion (top).\n");
      }
      if (counterclockwise(m, b, rightvertex, topvertex, newvertex) < 0.0) {
        printf("Internal error in insertvertex():\n");
        printf(
            "  Clockwise triangle after edge vertex insertion (top right).\n");
      }
      if (counterclockwise(m, b, topvertex, leftvertex, newvertex) < 0.0) {
        printf("Internal error in insertvertex():\n");
        printf(
            "  Clockwise triangle after edge vertex insertion (top left).\n");
      }
    }
    if (counterclockwise(m, b, leftvertex, botvertex, newvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf(
          "  Clockwise triangle after edge vertex insertion (bottom left).\n");
    }
    if (counterclockwise(m, b, botvertex, rightvertex, newvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf(
        "  Clockwise triangle after edge vertex insertion (bottom right).\n");
    }
#endif /* SELF_CHECK */
    if (b->verbose > 2) {
      printf("  Updating bottom left ");
      printtriangle(m, b, &botright);
      if (mirrorflag) {
        printf("  Updating top left ");
        printtriangle(m, b, &topright);
        printf("  Creating top right ");
        printtriangle(m, b, &newtopright);
      }
      printf("  Creating bottom right ");
      printtriangle(m, b, &newbotright);
    }

    /* Position `horiz' on the first edge to check for */
    /*   the Delaunay property.                        */
    lnextself(horiz);
  } else {
    /* Insert the vertex in a triangle, splitting it into three. */
    lnext(horiz, botleft);
    lprev(horiz, botright);
    sym(botleft, botlcasing);
    sym(botright, botrcasing);
    maketriangle(m, b, &newbotleft);
    maketriangle(m, b, &newbotright);

    /* Set the vertices of changed and new triangles. */
    org(horiz, rightvertex);
    dest(horiz, leftvertex);
    apex(horiz, botvertex);
    setorg(newbotleft, leftvertex);
    setdest(newbotleft, botvertex);
    setapex(newbotleft, newvertex);
    setorg(newbotright, botvertex);
    setdest(newbotright, rightvertex);
    setapex(newbotright, newvertex);
    setapex(horiz, newvertex);
    for (i = 0; i < m->eextras; i++) {
      /* Set the element attributes of the new triangles. */
      attrib = elemattribute(horiz, i);
      setelemattribute(newbotleft, i, attrib);
      setelemattribute(newbotright, i, attrib);
    }
    if (b->vararea) {
      /* Set the area constraint of the new triangles. */
      area = areabound(horiz);
      setareabound(newbotleft, area);
      setareabound(newbotright, area);
    }

    /* There may be subsegments that need to be bonded */
    /*   to the new triangles.                         */
    if (m->checksegments) {
      tspivot(botleft, botlsubseg);
      if (botlsubseg.ss != m->dummysub) {
        tsdissolve(botleft);
        tsbond(newbotleft, botlsubseg);
      }
      tspivot(botright, botrsubseg);
      if (botrsubseg.ss != m->dummysub) {
        tsdissolve(botright);
        tsbond(newbotright, botrsubseg);
      }
    }

    /* Bond the new triangles to the surrounding triangles. */
    bond(newbotleft, botlcasing);
    bond(newbotright, botrcasing);
    lnextself(newbotleft);
    lprevself(newbotright);
    bond(newbotleft, newbotright);
    lnextself(newbotleft);
    bond(botleft, newbotleft);
    lprevself(newbotright);
    bond(botright, newbotright);

    if (m->checkquality) {
      poolrestart(&m->flipstackers);
      m->lastflip = (struct flipstacker *) poolalloc(&m->flipstackers);
      m->lastflip->flippedtri = encode(horiz);
      m->lastflip->prevflip = (struct flipstacker *) NULL;
    }

#ifdef SELF_CHECK
    if (counterclockwise(m, b, rightvertex, leftvertex, botvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf("  Clockwise triangle prior to vertex insertion.\n");
    }
    if (counterclockwise(m, b, rightvertex, leftvertex, newvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf("  Clockwise triangle after vertex insertion (top).\n");
    }
    if (counterclockwise(m, b, leftvertex, botvertex, newvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf("  Clockwise triangle after vertex insertion (left).\n");
    }
    if (counterclockwise(m, b, botvertex, rightvertex, newvertex) < 0.0) {
      printf("Internal error in insertvertex():\n");
      printf("  Clockwise triangle after vertex insertion (right).\n");
    }
#endif /* SELF_CHECK */
    if (b->verbose > 2) {
      printf("  Updating top ");
      printtriangle(m, b, &horiz);
      printf("  Creating left ");
      printtriangle(m, b, &newbotleft);
      printf("  Creating right ");
      printtriangle(m, b, &newbotright);
    }
  }

  /* The insertion is successful by default, unless an encroached */
  /*   subsegment is found.                                       */
  success = SUCCESSFULVERTEX;
  /* Circle around the newly inserted vertex, checking each edge opposite */
  /*   it for the Delaunay property.  Non-Delaunay edges are flipped.     */
  /*   `horiz' is always the edge being checked.  `first' marks where to  */
  /*   stop circling.                                                     */
  org(horiz, first);
  rightvertex = first;
  dest(horiz, leftvertex);
  /* Circle until finished. */
  while (1) {
    /* By default, the edge will be flipped. */
    doflip = 1;

    if (m->checksegments) {
      /* Check for a subsegment, which cannot be flipped. */
      tspivot(horiz, checksubseg);
      if (checksubseg.ss != m->dummysub) {
        /* The edge is a subsegment and cannot be flipped. */
        doflip = 0;
        if (segmentflaws) {
          /* Does the new vertex encroach upon this subsegment? */
          if (checkseg4encroach(m, b, &checksubseg)) {
            success = ENCROACHINGVERTEX;
          }
        }
      }
    }

    if (doflip) {
      /* Check if the edge is a boundary edge. */
      sym(horiz, top);
      if (top.tri == m->dummytri) {
        /* The edge is a boundary edge and cannot be flipped. */
        doflip = 0;
      } else {
        /* Find the vertex on the other side of the edge. */
        apex(top, farvertex);
        /* In the incremental Delaunay triangulation algorithm, any of      */
        /*   `leftvertex', `rightvertex', and `farvertex' could be vertices */
        /*   of the triangular bounding box.  These vertices must be        */
        /*   treated as if they are infinitely distant, even though their   */
        /*   "coordinates" are not.                                         */
        if ((leftvertex == m->infvertex1) || (leftvertex == m->infvertex2) ||
            (leftvertex == m->infvertex3)) {
          /* `leftvertex' is infinitely distant.  Check the convexity of  */
          /*   the boundary of the triangulation.  'farvertex' might be   */
          /*   infinite as well, but trust me, this same condition should */
          /*   be applied.                                                */
          doflip = counterclockwise(m, b, newvertex, rightvertex, farvertex)
                   > 0.0;
        } else if ((rightvertex == m->infvertex1) ||
                   (rightvertex == m->infvertex2) ||
                   (rightvertex == m->infvertex3)) {
          /* `rightvertex' is infinitely distant.  Check the convexity of */
          /*   the boundary of the triangulation.  'farvertex' might be   */
          /*   infinite as well, but trust me, this same condition should */
          /*   be applied.                                                */
          doflip = counterclockwise(m, b, farvertex, leftvertex, newvertex)
                   > 0.0;
        } else if ((farvertex == m->infvertex1) ||
                   (farvertex == m->infvertex2) ||
                   (farvertex == m->infvertex3)) {
          /* `farvertex' is infinitely distant and cannot be inside */
          /*   the circumcircle of the triangle `horiz'.            */
          doflip = 0;
        } else {
          /* Test whether the edge is locally Delaunay. */
          doflip = incircle(m, b, leftvertex, newvertex, rightvertex,
                            farvertex) > 0.0;
        }
        if (doflip) {
          /* We made it!  Flip the edge `horiz' by rotating its containing */
          /*   quadrilateral (the two triangles adjacent to `horiz').      */
          /* Identify the casing of the quadrilateral. */
          lprev(top, topleft);
          sym(topleft, toplcasing);
          lnext(top, topright);
          sym(topright, toprcasing);
          lnext(horiz, botleft);
          sym(botleft, botlcasing);
          lprev(horiz, botright);
          sym(botright, botrcasing);
          /* Rotate the quadrilateral one-quarter turn counterclockwise. */
          bond(topleft, botlcasing);
          bond(botleft, botrcasing);
          bond(botright, toprcasing);
          bond(topright, toplcasing);
          if (m->checksegments) {
            /* Check for subsegments and rebond them to the quadrilateral. */
            tspivot(topleft, toplsubseg);
            tspivot(botleft, botlsubseg);
            tspivot(botright, botrsubseg);
            tspivot(topright, toprsubseg);
            if (toplsubseg.ss == m->dummysub) {
              tsdissolve(topright);
            } else {
              tsbond(topright, toplsubseg);
            }
            if (botlsubseg.ss == m->dummysub) {
              tsdissolve(topleft);
            } else {
              tsbond(topleft, botlsubseg);
            }
            if (botrsubseg.ss == m->dummysub) {
              tsdissolve(botleft);
            } else {
              tsbond(botleft, botrsubseg);
            }
            if (toprsubseg.ss == m->dummysub) {
              tsdissolve(botright);
            } else {
              tsbond(botright, toprsubseg);
            }
          }
          /* New vertex assignments for the rotated quadrilateral. */
          setorg(horiz, farvertex);
          setdest(horiz, newvertex);
          setapex(horiz, rightvertex);
          setorg(top, newvertex);
          setdest(top, farvertex);
          setapex(top, leftvertex);
          for (i = 0; i < m->eextras; i++) {
            /* Take the average of the two triangles' attributes. */
            attrib = 0.5 * (elemattribute(top, i) + elemattribute(horiz, i));
            setelemattribute(top, i, attrib);
            setelemattribute(horiz, i, attrib);
          }
          if (b->vararea) {
            if ((areabound(top) <= 0.0) || (areabound(horiz) <= 0.0)) {
              area = -1.0;
            } else {
              /* Take the average of the two triangles' area constraints.    */
              /*   This prevents small area constraints from migrating a     */
              /*   ULONGLONG, ULONGLONG way from their original location due to flips. */
              area = 0.5 * (areabound(top) + areabound(horiz));
            }
            setareabound(top, area);
            setareabound(horiz, area);
          }

          if (m->checkquality) {
            newflip = (struct flipstacker *) poolalloc(&m->flipstackers);
            newflip->flippedtri = encode(horiz);
            newflip->prevflip = m->lastflip;
            m->lastflip = newflip;
          }

#ifdef SELF_CHECK
          if (newvertex != (vertex) NULL) {
            if (counterclockwise(m, b, leftvertex, newvertex, rightvertex) <
                0.0) {
              printf("Internal error in insertvertex():\n");
              printf("  Clockwise triangle prior to edge flip (bottom).\n");
            }
            /* The following test has been removed because constrainededge() */
            /*   sometimes generates inverted triangles that insertvertex()  */
            /*   removes.                                                    */
/*
            if (counterclockwise(m, b, rightvertex, farvertex, leftvertex) <
                0.0) {
              printf("Internal error in insertvertex():\n");
              printf("  Clockwise triangle prior to edge flip (top).\n");
            }
*/
            if (counterclockwise(m, b, farvertex, leftvertex, newvertex) <
                0.0) {
              printf("Internal error in insertvertex():\n");
              printf("  Clockwise triangle after edge flip (left).\n");
            }
            if (counterclockwise(m, b, newvertex, rightvertex, farvertex) <
                0.0) {
              printf("Internal error in insertvertex():\n");
              printf("  Clockwise triangle after edge flip (right).\n");
            }
          }
#endif /* SELF_CHECK */
          if (b->verbose > 2) {
            printf("  Edge flip results in left ");
            lnextself(topleft);
            printtriangle(m, b, &topleft);
            printf("  and right ");
            printtriangle(m, b, &horiz);
          }
          /* On the next iterations, consider the two edges that were  */
          /*   exposed (this is, are now visible to the newly inserted */
          /*   vertex) by the edge flip.                               */
          lprevself(horiz);
          leftvertex = farvertex;
        }
      }
    }
    if (!doflip) {
      /* The handle `horiz' is accepted as locally Delaunay. */
      if (triflaws) {
        /* Check the triangle `horiz' for quality. */
        testtriangle(m, b, &horiz);
      }
      /* Look for the next edge around the newly inserted vertex. */
      lnextself(horiz);
      sym(horiz, testtri);
      /* Check for finishing a complete revolution about the new vertex, or */
      /*   falling outside  of the triangulation.  The latter will happen   */
      /*   when a vertex is inserted at a boundary.                         */
      if ((leftvertex == first) || (testtri.tri == m->dummytri)) {
        /* We're done.  Return a triangle whose origin is the new vertex. */
        lnext(horiz, *searchtri);
        lnext(horiz, m->recenttri);
        return success;
      }
      /* Finish finding the next edge around the newly inserted vertex. */
      lnext(testtri, horiz);
      rightvertex = leftvertex;
      dest(horiz, leftvertex);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  triangulatepolygon()   Find the Delaunay triangulation of a polygon that */
/*                         has a certain "nice" shape.  This includes the    */
/*                         polygons that result from deletion of a vertex or */
/*                         insertion of a segment.                           */
/*                                                                           */
/*  This is a conceptually difficult routine.  The starting assumption is    */
/*  that we have a polygon with n sides.  n - 1 of these sides are currently */
/*  represented as edges in the mesh.  One side, called the "base", need not */
/*  be.                                                                      */
/*                                                                           */
/*  Inside the polygon is a structure I call a "fan", consisting of n - 1    */
/*  triangles that share a common origin.  For each of these triangles, the  */
/*  edge opposite the origin is one of the sides of the polygon.  The        */
/*  primary edge of each triangle is the edge directed from the origin to    */
/*  the destination; note that this is not the same edge that is a side of   */
/*  the polygon.  `firstedge' is the primary edge of the first triangle.     */
/*  From there, the triangles follow in counterclockwise order about the     */
/*  polygon, until `lastedge', the primary edge of the last triangle.        */
/*  `firstedge' and `lastedge' are probably connected to other triangles     */
/*  beyond the extremes of the fan, but their identity is not important, as  */
/*  ULONGLONG as the fan remains connected to them.                               */
/*                                                                           */
/*  Imagine the polygon oriented so that its base is at the bottom.  This    */
/*  puts `firstedge' on the far right, and `lastedge' on the far left.       */
/*  The right vertex of the base is the destination of `firstedge', and the  */
/*  left vertex of the base is the apex of `lastedge'.                       */
/*                                                                           */
/*  The challenge now is to find the right sequence of edge flips to         */
/*  transform the fan into a Delaunay triangulation of the polygon.  Each    */
/*  edge flip effectively removes one triangle from the fan, committing it   */
/*  to the polygon.  The resulting polygon has one fewer edge.  If `doflip'  */
/*  is set, the final flip will be performed, resulting in a fan of one      */
/*  (useless?) triangle.  If `doflip' is not set, the final flip is not      */
/*  performed, resulting in a fan of two triangles, and an unfinished        */
/*  triangular polygon that is not yet filled out with a single triangle.    */
/*  On completion of the routine, `lastedge' is the last remaining triangle, */
/*  or the leftmost of the last two.                                         */
/*                                                                           */
/*  Although the flips are performed in the order described above, the       */
/*  decisions about what flips to perform are made in precisely the reverse  */
/*  order.  The recursive triangulatepolygon() procedure makes a decision,   */
/*  uses up to two recursive calls to triangulate the "subproblems"          */
/*  (polygons with fewer edges), and then performs an edge flip.             */
/*                                                                           */
/*  The "decision" it makes is which vertex of the polygon should be         */
/*  connected to the base.  This decision is made by testing every possible  */
/*  vertex.  Once the best vertex is found, the two edges that connect this  */
/*  vertex to the base become the bases for two smaller polygons.  These     */
/*  are triangulated recursively.  Unfortunately, this approach can take     */
/*  O(n^2) time not only in the worst case, but in many common cases.  It's  */
/*  rarely a big deal for vertex deletion, where n is rarely larger than     */
/*  ten, but it could be a big deal for segment insertion, especially if     */
/*  there's a lot of ULONGLONG segments that each cut many triangles.  I ought to */
/*  code a faster algorithm some day.                                        */
/*                                                                           */
/*  The `edgecount' parameter is the number of sides of the polygon,         */
/*  including its base.  `triflaws' is a flag that determines whether the    */
/*  new triangles should be tested for quality, and enqueued if they are     */
/*  bad.                                                                     */
/*                                                                           */
/*****************************************************************************/

void triangulatepolygon(struct mesh *m, struct behavior *b,
                        struct otri *firstedge, struct otri *lastedge,
                        int edgecount, int doflip, int triflaws)
{
  struct otri testtri;
  struct otri besttri;
  struct otri tempedge;
  vertex leftbasevertex, rightbasevertex;
  vertex testvertex;
  vertex bestvertex;
  int bestnumber;
  int i;
  triangle ptr;   /* Temporary variable used by sym(), onext(), and oprev(). */

  /* Identify the base vertices. */
  apex(*lastedge, leftbasevertex);
  dest(*firstedge, rightbasevertex);
  if (b->verbose > 2) {
    printf("  Triangulating interior polygon at edge\n");
    printf("    (%.12g, %.12g) (%.12g, %.12g)\n", leftbasevertex[0],
           leftbasevertex[1], rightbasevertex[0], rightbasevertex[1]);
  }
  /* Find the best vertex to connect the base to. */
  onext(*firstedge, besttri);
  dest(besttri, bestvertex);
  otricopy(besttri, testtri);
  bestnumber = 1;
  for (i = 2; i <= edgecount - 2; i++) {
    onextself(testtri);
    dest(testtri, testvertex);
    /* Is this a better vertex? */
    if (incircle(m, b, leftbasevertex, rightbasevertex, bestvertex,
                 testvertex) > 0.0) {
      otricopy(testtri, besttri);
      bestvertex = testvertex;
      bestnumber = i;
    }
  }
  if (b->verbose > 2) {
    printf("    Connecting edge to (%.12g, %.12g)\n", bestvertex[0],
           bestvertex[1]);
  }
  if (bestnumber > 1) {
    /* Recursively triangulate the smaller polygon on the right. */
    oprev(besttri, tempedge);
    triangulatepolygon(m, b, firstedge, &tempedge, bestnumber + 1, 1,
                       triflaws);
  }
  if (bestnumber < edgecount - 2) {
    /* Recursively triangulate the smaller polygon on the left. */
    sym(besttri, tempedge);
    triangulatepolygon(m, b, &besttri, lastedge, edgecount - bestnumber, 1,
                       triflaws);
    /* Find `besttri' again; it may have been lost to edge flips. */
    sym(tempedge, besttri);
  }
  if (doflip) {
    /* Do one final edge flip. */
    flip(m, b, &besttri);
    if (triflaws) {
      /* Check the quality of the newly committed triangle. */
      sym(besttri, testtri);
      testtriangle(m, b, &testtri);
    }
  }
  /* Return the base triangle. */
  otricopy(besttri, *lastedge);
}

/*****************************************************************************/
/*                                                                           */
/*  deletevertex()   Delete a vertex from a Delaunay triangulation, ensuring */
/*                   that the triangulation remains Delaunay.                */
/*                                                                           */
/*  The origin of `deltri' is deleted.  The union of the triangles adjacent  */
/*  to this vertex is a polygon, for which the Delaunay triangulation is     */
/*  found.  Two triangles are removed from the mesh.                         */
/*                                                                           */
/*  Only interior vertices that do not lie on segments or boundaries may be  */
/*  deleted.                                                                 */
/*                                                                           */
/*****************************************************************************/

void deletevertex(struct mesh *m, struct behavior *b, struct otri *deltri)
{
  struct otri countingtri;
  struct otri firstedge, lastedge;
  struct otri deltriright;
  struct otri lefttri, righttri;
  struct otri leftcasing, rightcasing;
  struct osub leftsubseg, rightsubseg;
  vertex delvertex;
  vertex neworg;
  int edgecount;
  triangle ptr;   /* Temporary variable used by sym(), onext(), and oprev(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  org(*deltri, delvertex);
  if (b->verbose > 1) {
    printf("  Deleting (%.12g, %.12g).\n", delvertex[0], delvertex[1]);
  }
  vertexdealloc(m, delvertex);

  /* Count the degree of the vertex being deleted. */
  onext(*deltri, countingtri);
  edgecount = 1;
  while (!otriequal(*deltri, countingtri)) {
#ifdef SELF_CHECK
    if (countingtri.tri == m->dummytri) {
      printf("Internal error in deletevertex():\n");
      printf("  Attempt to delete boundary vertex.\n");
      internalerror();
    }
#endif /* SELF_CHECK */
    edgecount++;
    onextself(countingtri);
  }

#ifdef SELF_CHECK
  if (edgecount < 3) {
    printf("Internal error in deletevertex():\n  Vertex has degree %d.\n",
           edgecount);
    internalerror();
  }
#endif /* SELF_CHECK */
  if (edgecount > 3) {
    /* Triangulate the polygon defined by the union of all triangles */
    /*   adjacent to the vertex being deleted.  Check the quality of */
    /*   the resulting triangles.                                    */
    onext(*deltri, firstedge);
    oprev(*deltri, lastedge);
    triangulatepolygon(m, b, &firstedge, &lastedge, edgecount, 0,
                       !b->nobisect);
  }
  /* Splice out two triangles. */
  lprev(*deltri, deltriright);
  dnext(*deltri, lefttri);
  sym(lefttri, leftcasing);
  oprev(deltriright, righttri);
  sym(righttri, rightcasing);
  bond(*deltri, leftcasing);
  bond(deltriright, rightcasing);
  tspivot(lefttri, leftsubseg);
  if (leftsubseg.ss != m->dummysub) {
    tsbond(*deltri, leftsubseg);
  }
  tspivot(righttri, rightsubseg);
  if (rightsubseg.ss != m->dummysub) {
    tsbond(deltriright, rightsubseg);
  }

  /* Set the new origin of `deltri' and check its quality. */
  org(lefttri, neworg);
  setorg(*deltri, neworg);
  if (!b->nobisect) {
    testtriangle(m, b, deltri);
  }

  /* Delete the two spliced-out triangles. */
  triangledealloc(m, lefttri.tri);
  triangledealloc(m, righttri.tri);
}

/*****************************************************************************/
/*                                                                           */
/*  undovertex()   Undo the most recent vertex insertion.                    */
/*                                                                           */
/*  Walks through the list of transformations (flips and a vertex insertion) */
/*  in the reverse of the order in which they were done, and undoes them.    */
/*  The inserted vertex is removed from the triangulation and deallocated.   */
/*  Two triangles (possibly just one) are also deallocated.                  */
/*                                                                           */
/*****************************************************************************/

void undovertex(struct mesh *m, struct behavior *b)
{
  struct otri fliptri;
  struct otri botleft, botright, topright;
  struct otri botlcasing, botrcasing, toprcasing;
  struct otri gluetri;
  struct osub botlsubseg, botrsubseg, toprsubseg;
  vertex botvertex, rightvertex;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  /* Walk through the list of transformations (flips and a vertex insertion) */
  /*   in the reverse of the order in which they were done, and undo them.   */
  while (m->lastflip != (struct flipstacker *) NULL) {
    /* Find a triangle involved in the last unreversed transformation. */
    decode(m->lastflip->flippedtri, fliptri);

    /* We are reversing one of three transformations:  a trisection of one */
    /*   triangle into three (by inserting a vertex in the triangle), a    */
    /*   bisection of two triangles into four (by inserting a vertex in an */
    /*   edge), or an edge flip.                                           */
    if (m->lastflip->prevflip == (struct flipstacker *) NULL) {
      /* Restore a triangle that was split into three triangles, */
      /*   so it is again one triangle.                          */
      dprev(fliptri, botleft);
      lnextself(botleft);
      onext(fliptri, botright);
      lprevself(botright);
      sym(botleft, botlcasing);
      sym(botright, botrcasing);
      dest(botleft, botvertex);

      setapex(fliptri, botvertex);
      lnextself(fliptri);
      bond(fliptri, botlcasing);
      tspivot(botleft, botlsubseg);
      tsbond(fliptri, botlsubseg);
      lnextself(fliptri);
      bond(fliptri, botrcasing);
      tspivot(botright, botrsubseg);
      tsbond(fliptri, botrsubseg);

      /* Delete the two spliced-out triangles. */
      triangledealloc(m, botleft.tri);
      triangledealloc(m, botright.tri);
    } else if (m->lastflip->prevflip == (struct flipstacker *) &insertvertex) {
      /* Restore two triangles that were split into four triangles, */
      /*   so they are again two triangles.                         */
      lprev(fliptri, gluetri);
      sym(gluetri, botright);
      lnextself(botright);
      sym(botright, botrcasing);
      dest(botright, rightvertex);

      setorg(fliptri, rightvertex);
      bond(gluetri, botrcasing);
      tspivot(botright, botrsubseg);
      tsbond(gluetri, botrsubseg);

      /* Delete the spliced-out triangle. */
      triangledealloc(m, botright.tri);

      sym(fliptri, gluetri);
      if (gluetri.tri != m->dummytri) {
        lnextself(gluetri);
        dnext(gluetri, topright);
        sym(topright, toprcasing);

        setorg(gluetri, rightvertex);
        bond(gluetri, toprcasing);
        tspivot(topright, toprsubseg);
        tsbond(gluetri, toprsubseg);

        /* Delete the spliced-out triangle. */
        triangledealloc(m, topright.tri);
      }

      /* This is the end of the list, sneakily encoded. */
      m->lastflip->prevflip = (struct flipstacker *) NULL;
    } else {
      /* Undo an edge flip. */
      unflip(m, b, &fliptri);
    }

    /* Go on and process the next transformation. */
    m->lastflip = m->lastflip->prevflip;
  }
}

/**                                                                         **/
/**                                                                         **/
/********* Mesh transformation routines end here                     *********/
