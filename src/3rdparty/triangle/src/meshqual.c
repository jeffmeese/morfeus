#include "meshqual.h"

#include "debug.h"
#include "geom.h"
#include "mem.h"
#include "meshtransform.h"
#include "user.h"

#include <math.h>
#include <stdio.h>

void tallyencs(struct mesh *m, struct behavior *b)
{
  struct osub subsegloop;
  int dummy;

  traversalinit(&m->subsegs);
  subsegloop.ssorient = 0;
  subsegloop.ss = subsegtraverse(m);
  while (subsegloop.ss != (subseg *) NULL) {
    /* If the segment is encroached, add it to the list. */
    dummy = checkseg4encroach(m, b, &subsegloop);
    subsegloop.ss = subsegtraverse(m);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  precisionerror()  Print an error message for precision problems.         */
/*                                                                           */
/*****************************************************************************/


void precisionerror()
{
  printf("Try increasing the area criterion and/or reducing the minimum\n");
  printf("  allowable angle so that tiny triangles are not created.\n");
#ifdef SINGLE
  printf("Alternatively, try recompiling me with double precision\n");
  printf("  arithmetic (by removing \"#define SINGLE\" from the\n");
  printf("  source file or \"-DSINGLE\" from the makefile).\n");
#endif /* SINGLE */
}

/*****************************************************************************/
/*                                                                           */
/*  splitencsegs()   Split all the encroached subsegments.                   */
/*                                                                           */
/*  Each encroached subsegment is repaired by splitting it - inserting a     */
/*  vertex at or near its midpoint.  Newly inserted vertices may encroach    */
/*  upon other subsegments; these are also repaired.                         */
/*                                                                           */
/*  `triflaws' is a flag that specifies whether one should take note of new  */
/*  bad triangles that result from inserting vertices to repair encroached   */
/*  subsegments.                                                             */
/*                                                                           */
/*****************************************************************************/

void splitencsegs(struct mesh *m, struct behavior *b, int triflaws)
{
  struct otri enctri;
  struct otri testtri;
  struct osub testsh;
  struct osub currentenc;
  struct badsubseg *encloop;
  vertex eorg, edest, eapex;
  vertex newvertex;
  enum insertvertexresult success;
  REAL segmentlength, nearestpoweroftwo;
  REAL split;
  REAL multiplier, divisor;
  int acuteorg, acuteorg2, acutedest, acutedest2;
  int dummy;
  int i;
  triangle ptr;                     /* Temporary variable used by stpivot(). */
  subseg sptr;                        /* Temporary variable used by snext(). */

  /* Note that steinerleft == -1 if an unlimited number */
  /*   of Steiner points is allowed.                    */
  while ((m->badsubsegs.items > 0) && (m->steinerleft != 0)) {
    traversalinit(&m->badsubsegs);
    encloop = badsubsegtraverse(m);
    while ((encloop != (struct badsubseg *) NULL) && (m->steinerleft != 0)) {
      sdecode(encloop->encsubseg, currentenc);
      sorg(currentenc, eorg);
      sdest(currentenc, edest);
      /* Make sure that this segment is still the same segment it was   */
      /*   when it was determined to be encroached.  If the segment was */
      /*   enqueued multiple times (because several newly inserted      */
      /*   vertices encroached it), it may have already been split.     */
      if (!deadsubseg(currentenc.ss) &&
          (eorg == encloop->subsegorg) && (edest == encloop->subsegdest)) {
        /* To decide where to split a segment, we need to know if the   */
        /*   segment shares an endpoint with an adjacent segment.       */
        /*   The concern is that, if we simply split every encroached   */
        /*   segment in its center, two adjacent segments with a small  */
        /*   angle between them might lead to an infinite loop; each    */
        /*   vertex added to split one segment will encroach upon the   */
        /*   other segment, which must then be split with a vertex that */
        /*   will encroach upon the first segment, and so on forever.   */
        /* To avoid this, imagine a set of concentric circles, whose    */
        /*   radii are powers of two, about each segment endpoint.      */
        /*   These concentric circles determine where the segment is    */
        /*   split.  (If both endpoints are shared with adjacent        */
        /*   segments, split the segment in the middle, and apply the   */
        /*   concentric circles for later splittings.)                  */

        /* Is the origin shared with another segment? */
        stpivot(currentenc, enctri);
        lnext(enctri, testtri);
        tspivot(testtri, testsh);
        acuteorg = testsh.ss != m->dummysub;
        /* Is the destination shared with another segment? */
        lnextself(testtri);
        tspivot(testtri, testsh);
        acutedest = testsh.ss != m->dummysub;

        /* If we're using Chew's algorithm (rather than Ruppert's) */
        /*   to define encroachment, delete free vertices from the */
        /*   subsegment's diametral circle.                        */
        if (!b->conformdel && !acuteorg && !acutedest) {
          apex(enctri, eapex);
          while ((vertextype(eapex) == FREEVERTEX) &&
                 ((eorg[0] - eapex[0]) * (edest[0] - eapex[0]) +
                  (eorg[1] - eapex[1]) * (edest[1] - eapex[1]) < 0.0)) {
            deletevertex(m, b, &testtri);
            stpivot(currentenc, enctri);
            apex(enctri, eapex);
            lprev(enctri, testtri);
          }
        }

        /* Now, check the other side of the segment, if there's a triangle */
        /*   there.                                                        */
        sym(enctri, testtri);
        if (testtri.tri != m->dummytri) {
          /* Is the destination shared with another segment? */
          lnextself(testtri);
          tspivot(testtri, testsh);
          acutedest2 = testsh.ss != m->dummysub;
          acutedest = acutedest || acutedest2;
          /* Is the origin shared with another segment? */
          lnextself(testtri);
          tspivot(testtri, testsh);
          acuteorg2 = testsh.ss != m->dummysub;
          acuteorg = acuteorg || acuteorg2;

          /* Delete free vertices from the subsegment's diametral circle. */
          if (!b->conformdel && !acuteorg2 && !acutedest2) {
            org(testtri, eapex);
            while ((vertextype(eapex) == FREEVERTEX) &&
                   ((eorg[0] - eapex[0]) * (edest[0] - eapex[0]) +
                    (eorg[1] - eapex[1]) * (edest[1] - eapex[1]) < 0.0)) {
              deletevertex(m, b, &testtri);
              sym(enctri, testtri);
              apex(testtri, eapex);
              lprevself(testtri);
            }
          }
        }

        /* Use the concentric circles if exactly one endpoint is shared */
        /*   with another adjacent segment.                             */
        if (acuteorg || acutedest) {
          segmentlength = sqrt((edest[0] - eorg[0]) * (edest[0] - eorg[0]) +
                               (edest[1] - eorg[1]) * (edest[1] - eorg[1]));
          /* Find the power of two that most evenly splits the segment.  */
          /*   The worst case is a 2:1 ratio between subsegment lengths. */
          nearestpoweroftwo = 1.0;
          while (segmentlength > 3.0 * nearestpoweroftwo) {
            nearestpoweroftwo *= 2.0;
          }
          while (segmentlength < 1.5 * nearestpoweroftwo) {
            nearestpoweroftwo *= 0.5;
          }
          /* Where do we split the segment? */
          split = nearestpoweroftwo / segmentlength;
          if (acutedest) {
            split = 1.0 - split;
          }
        } else {
          /* If we're not worried about adjacent segments, split */
          /*   this segment in the middle.                       */
          split = 0.5;
        }

        /* Create the new vertex. */
        newvertex = (vertex) poolalloc(&m->vertices);
        /* Interpolate its coordinate and attributes. */
        for (i = 0; i < 2 + m->nextras; i++) {
          newvertex[i] = eorg[i] + split * (edest[i] - eorg[i]);
        }

        if (!b->noexact) {
          /* Roundoff in the above calculation may yield a `newvertex'   */
          /*   that is not precisely collinear with `eorg' and `edest'.  */
          /*   Improve collinearity by one step of iterative refinement. */
          multiplier = counterclockwise(m, b, eorg, edest, newvertex);
          divisor = ((eorg[0] - edest[0]) * (eorg[0] - edest[0]) +
                     (eorg[1] - edest[1]) * (eorg[1] - edest[1]));
          if ((multiplier != 0.0) && (divisor != 0.0)) {
            multiplier = multiplier / divisor;
            /* Watch out for NANs. */
            if (multiplier == multiplier) {
              newvertex[0] += multiplier * (edest[1] - eorg[1]);
              newvertex[1] += multiplier * (eorg[0] - edest[0]);
            }
          }
        }

        setvertexmark(newvertex, mark(currentenc));
        setvertextype(newvertex, SEGMENTVERTEX);
        if (b->verbose > 1) {
          printf(
  "  Splitting subsegment (%.12g, %.12g) (%.12g, %.12g) at (%.12g, %.12g).\n",
                 eorg[0], eorg[1], edest[0], edest[1],
                 newvertex[0], newvertex[1]);
        }
        /* Check whether the new vertex lies on an endpoint. */
        if (((newvertex[0] == eorg[0]) && (newvertex[1] == eorg[1])) ||
            ((newvertex[0] == edest[0]) && (newvertex[1] == edest[1]))) {
          printf("Error:  Ran out of precision at (%.12g, %.12g).\n",
                 newvertex[0], newvertex[1]);
          printf("I attempted to split a segment to a smaller size than\n");
          printf("  can be accommodated by the finite precision of\n");
          printf("  floating point arithmetic.\n");
          precisionerror();
          triexit(1);
        }
        /* Insert the splitting vertex.  This should always succeed. */
        success = insertvertex(m, b, newvertex, &enctri, &currentenc,
                               1, triflaws);
        if ((success != SUCCESSFULVERTEX) && (success != ENCROACHINGVERTEX)) {
          printf("Internal error in splitencsegs():\n");
          printf("  Failure to split a segment.\n");
          internalerror();
        }
        if (m->steinerleft > 0) {
          m->steinerleft--;
        }
        /* Check the two new subsegments to see if they're encroached. */
        dummy = checkseg4encroach(m, b, &currentenc);
        snextself(currentenc);
        dummy = checkseg4encroach(m, b, &currentenc);
      }

      badsubsegdealloc(m, encloop);
      encloop = badsubsegtraverse(m);
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  tallyfaces()   Test every triangle in the mesh for quality measures.     */
/*                                                                           */
/*****************************************************************************/

void tallyfaces(struct mesh *m, struct behavior *b)
{
  struct otri triangleloop;

  if (b->verbose) {
    printf("  Making a list of bad triangles.\n");
  }
  traversalinit(&m->triangles);
  triangleloop.orient = 0;
  triangleloop.tri = triangletraverse(m);
  while (triangleloop.tri != (triangle *) NULL) {
    /* If the triangle is bad, enqueue it. */
    testtriangle(m, b, &triangleloop);
    triangleloop.tri = triangletraverse(m);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  splittriangle()   Inserts a vertex at the circumcenter of a triangle.    */
/*                    Deletes the newly inserted vertex if it encroaches     */
/*                    upon a segment.                                        */
/*                                                                           */
/*****************************************************************************/

void splittriangle(struct mesh *m, struct behavior *b,
                   struct badtriang *badtri)
{
  struct otri badotri;
  vertex borg, bdest, bapex;
  vertex newvertex;
  REAL xi, eta;
  enum insertvertexresult success;
  int errorflag;
  int i;

  decode(badtri->poortri, badotri);
  org(badotri, borg);
  dest(badotri, bdest);
  apex(badotri, bapex);
  /* Make sure that this triangle is still the same triangle it was      */
  /*   when it was tested and determined to be of bad quality.           */
  /*   Subsequent transformations may have made it a different triangle. */
  if (!deadtri(badotri.tri) && (borg == badtri->triangorg) &&
      (bdest == badtri->triangdest) && (bapex == badtri->triangapex)) {
    if (b->verbose > 1) {
      printf("  Splitting this triangle at its circumcenter:\n");
      printf("    (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n", borg[0],
             borg[1], bdest[0], bdest[1], bapex[0], bapex[1]);
    }

    errorflag = 0;
    /* Create a new vertex at the triangle's circumcenter. */
    newvertex = (vertex) poolalloc(&m->vertices);
    findcircumcenter(m, b, borg, bdest, bapex, newvertex, &xi, &eta, 1);

    /* Check whether the new vertex lies on a triangle vertex. */
    if (((newvertex[0] == borg[0]) && (newvertex[1] == borg[1])) ||
        ((newvertex[0] == bdest[0]) && (newvertex[1] == bdest[1])) ||
        ((newvertex[0] == bapex[0]) && (newvertex[1] == bapex[1]))) {
      if (!b->quiet) {
        printf(
             "Warning:  New vertex (%.12g, %.12g) falls on existing vertex.\n",
               newvertex[0], newvertex[1]);
        errorflag = 1;
      }
      vertexdealloc(m, newvertex);
    } else {
      for (i = 2; i < 2 + m->nextras; i++) {
        /* Interpolate the vertex attributes at the circumcenter. */
        newvertex[i] = borg[i] + xi * (bdest[i] - borg[i])
                              + eta * (bapex[i] - borg[i]);
      }
      /* The new vertex must be in the interior, and therefore is a */
      /*   free vertex with a marker of zero.                       */
      setvertexmark(newvertex, 0);
      setvertextype(newvertex, FREEVERTEX);

      /* Ensure that the handle `badotri' does not represent the uint64_test  */
      /*   edge of the triangle.  This ensures that the circumcenter must */
      /*   fall to the left of this edge, so point location will work.    */
      /*   (If the angle org-apex-dest exceeds 90 degrees, then the       */
      /*   circumcenter lies outside the org-dest edge, and eta is        */
      /*   negative.  Roundoff error might prevent eta from being         */
      /*   negative when it should be, so I test eta against xi.)         */
      if (eta < xi) {
        lprevself(badotri);
      }

      /* Insert the circumcenter, searching from the edge of the triangle, */
      /*   and maintain the Delaunay property of the triangulation.        */
      success = insertvertex(m, b, newvertex, &badotri, (struct osub *) NULL,
                             1, 1);
      if (success == SUCCESSFULVERTEX) {
        if (m->steinerleft > 0) {
          m->steinerleft--;
        }
      } else if (success == ENCROACHINGVERTEX) {
        /* If the newly inserted vertex encroaches upon a subsegment, */
        /*   delete the new vertex.                                   */
        undovertex(m, b);
        if (b->verbose > 1) {
          printf("  Rejecting (%.12g, %.12g).\n", newvertex[0], newvertex[1]);
        }
        vertexdealloc(m, newvertex);
      } else if (success == VIOLATINGVERTEX) {
        /* Failed to insert the new vertex, but some subsegment was */
        /*   marked as being encroached.                            */
        vertexdealloc(m, newvertex);
      } else {                                 /* success == DUPLICATEVERTEX */
        /* Couldn't insert the new vertex because a vertex is already there. */
        if (!b->quiet) {
          printf(
            "Warning:  New vertex (%.12g, %.12g) falls on existing vertex.\n",
                 newvertex[0], newvertex[1]);
          errorflag = 1;
        }
        vertexdealloc(m, newvertex);
      }
    }
    if (errorflag) {
      if (b->verbose) {
        printf("  The new vertex is at the circumcenter of triangle\n");
        printf("    (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
               borg[0], borg[1], bdest[0], bdest[1], bapex[0], bapex[1]);
      }
      printf("This probably means that I am trying to refine triangles\n");
      printf("  to a smaller size than can be accommodated by the finite\n");
      printf("  precision of floating point arithmetic.  (You can be\n");
      printf("  sure of this if I fail to terminate.)\n");
      precisionerror();
    }
  }
}

/*****************************************************************************/
/*                                                                           */
/*  enforcequality()   Remove all the encroached subsegments and bad         */
/*                     triangles from the triangulation.                     */
/*                                                                           */
/*****************************************************************************/

void enforcequality(struct mesh *m, struct behavior *b)
{
  struct badtriang *badtri;
  int i;

  if (!b->quiet) {
    printf("Adding Steiner points to enforce quality.\n");
  }
  /* Initialize the pool of encroached subsegments. */
  poolinit(&m->badsubsegs, sizeof(struct badsubseg), BADSUBSEGPERBLOCK,
           BADSUBSEGPERBLOCK, 0);
  if (b->verbose) {
    printf("  Looking for encroached subsegments.\n");
  }
  /* Test all segments to see if they're encroached. */
  tallyencs(m, b);
  if (b->verbose && (m->badsubsegs.items > 0)) {
    printf("  Splitting encroached subsegments.\n");
  }
  /* Fix encroached subsegments without noting bad triangles. */
  splitencsegs(m, b, 0);
  /* At this point, if we haven't run out of Steiner points, the */
  /*   triangulation should be (conforming) Delaunay.            */

  /* Next, we worry about enforcing triangle quality. */
  if ((b->minangle > 0.0) || b->vararea || b->fixedarea || b->usertest) {
    /* Initialize the pool of bad triangles. */
    poolinit(&m->badtriangles, sizeof(struct badtriang), BADTRIPERBLOCK,
             BADTRIPERBLOCK, 0);
    /* Initialize the queues of bad triangles. */
    for (i = 0; i < 4096; i++) {
      m->queuefront[i] = (struct badtriang *) NULL;
    }
    m->firstnonemptyq = -1;
    /* Test all triangles to see if they're bad. */
    tallyfaces(m, b);
    /* Initialize the pool of recently flipped triangles. */
    poolinit(&m->flipstackers, sizeof(struct flipstacker), FLIPSTACKERPERBLOCK,
             FLIPSTACKERPERBLOCK, 0);
    m->checkquality = 1;
    if (b->verbose) {
      printf("  Splitting bad triangles.\n");
    }
    while ((m->badtriangles.items > 0) && (m->steinerleft != 0)) {
      /* Fix one bad triangle by inserting a vertex at its circumcenter. */
      badtri = dequeuebadtriang(m);
      splittriangle(m, b, badtri);
      if (m->badsubsegs.items > 0) {
        /* Put bad triangle back in queue for another try later. */
        enqueuebadtriang(m, b, badtri);
        /* Fix any encroached subsegments that resulted. */
        /*   Record any new bad triangles that result.   */
        splitencsegs(m, b, 1);
      } else {
        /* Return the bad triangle to the pool. */
        pooldealloc(&m->badtriangles, (void *) badtri);
      }
    }
  }
  /* At this point, if the "-D" switch was selected and we haven't run out  */
  /*   of Steiner points, the triangulation should be (conforming) Delaunay */
  /*   and have no low-quality triangles.                                   */

  /* Might we have run out of Steiner points too soon? */
  if (!b->quiet && b->conformdel && (m->badsubsegs.items > 0) &&
      (m->steinerleft == 0)) {
    printf("\nWarning:  I ran out of Steiner points, but the mesh has\n");
    if (m->badsubsegs.items == 1) {
      printf("  one encroached subsegment, and therefore might not be truly\n"
             );
    } else {
      printf("  %ld encroached subsegments, and therefore might not be truly\n"
             , m->badsubsegs.items);
    }
    printf("  Delaunay.  If the Delaunay property is important to you,\n");
    printf("  try increasing the number of Steiner points (controlled by\n");
    printf("  the -S switch) slightly and try again.\n\n");
  }
}

/*****************************************************************************/
/*                                                                           */
/*  checkseg4encroach()   Check a subsegment to see if it is encroached; add */
/*                        it to the list if it is.                           */
/*                                                                           */
/*  A subsegment is encroached if there is a vertex in its diametral lens.   */
/*  For Ruppert's algorithm (-D switch), the "diametral lens" is the         */
/*  diametral circle.  For Chew's algorithm (default), the diametral lens is */
/*  just big enough to enclose two isosceles triangles whose bases are the   */
/*  subsegment.  Each of the two isosceles triangles has two angles equal    */
/*  to `b->minangle'.                                                        */
/*                                                                           */
/*  Chew's algorithm does not require diametral lenses at all--but they save */
/*  time.  Any vertex inside a subsegment's diametral lens implies that the  */
/*  triangle adjoining the subsegment will be too skinny, so it's only a     */
/*  matter of time before the encroaching vertex is deleted by Chew's        */
/*  algorithm.  It's faster to simply not insert the doomed vertex in the    */
/*  first place, which is why I use diametral lenses with Chew's algorithm.  */
/*                                                                           */
/*  Returns a nonzero value if the subsegment is encroached.                 */
/*                                                                           */
/*****************************************************************************/

int checkseg4encroach(struct mesh *m, struct behavior *b,
                      struct osub *testsubseg)
{
  struct otri neighbortri;
  struct osub testsym;
  struct badsubseg *encroachedseg;
  REAL dotproduct;
  int encroached;
  int sides;
  vertex eorg, edest, eapex;
  triangle ptr;                     /* Temporary variable used by stpivot(). */

  encroached = 0;
  sides = 0;

  sorg(*testsubseg, eorg);
  sdest(*testsubseg, edest);
  /* Check one neighbor of the subsegment. */
  stpivot(*testsubseg, neighbortri);
  /* Does the neighbor exist, or is this a boundary edge? */
  if (neighbortri.tri != m->dummytri) {
    sides++;
    /* Find a vertex opposite this subsegment. */
    apex(neighbortri, eapex);
    /* Check whether the apex is in the diametral lens of the subsegment */
    /*   (the diametral circle if `conformdel' is set).  A dot product   */
    /*   of two sides of the triangle is used to check whether the angle */
    /*   at the apex is greater than (180 - 2 `minangle') degrees (for   */
    /*   lenses; 90 degrees for diametral circles).                      */
    dotproduct = (eorg[0] - eapex[0]) * (edest[0] - eapex[0]) +
                 (eorg[1] - eapex[1]) * (edest[1] - eapex[1]);
    if (dotproduct < 0.0) {
      if (b->conformdel ||
          (dotproduct * dotproduct >=
           (2.0 * b->goodangle - 1.0) * (2.0 * b->goodangle - 1.0) *
           ((eorg[0] - eapex[0]) * (eorg[0] - eapex[0]) +
            (eorg[1] - eapex[1]) * (eorg[1] - eapex[1])) *
           ((edest[0] - eapex[0]) * (edest[0] - eapex[0]) +
            (edest[1] - eapex[1]) * (edest[1] - eapex[1])))) {
        encroached = 1;
      }
    }
  }
  /* Check the other neighbor of the subsegment. */
  ssym(*testsubseg, testsym);
  stpivot(testsym, neighbortri);
  /* Does the neighbor exist, or is this a boundary edge? */
  if (neighbortri.tri != m->dummytri) {
    sides++;
    /* Find the other vertex opposite this subsegment. */
    apex(neighbortri, eapex);
    /* Check whether the apex is in the diametral lens of the subsegment */
    /*   (or the diametral circle, if `conformdel' is set).              */
    dotproduct = (eorg[0] - eapex[0]) * (edest[0] - eapex[0]) +
                 (eorg[1] - eapex[1]) * (edest[1] - eapex[1]);
    if (dotproduct < 0.0) {
      if (b->conformdel ||
          (dotproduct * dotproduct >=
           (2.0 * b->goodangle - 1.0) * (2.0 * b->goodangle - 1.0) *
           ((eorg[0] - eapex[0]) * (eorg[0] - eapex[0]) +
            (eorg[1] - eapex[1]) * (eorg[1] - eapex[1])) *
           ((edest[0] - eapex[0]) * (edest[0] - eapex[0]) +
            (edest[1] - eapex[1]) * (edest[1] - eapex[1])))) {
        encroached += 2;
      }
    }
  }

  if (encroached && (!b->nobisect || ((b->nobisect == 1) && (sides == 2)))) {
    if (b->verbose > 2) {
      printf(
        "  Queueing encroached subsegment (%.12g, %.12g) (%.12g, %.12g).\n",
        eorg[0], eorg[1], edest[0], edest[1]);
    }
    /* Add the subsegment to the list of encroached subsegments. */
    /*   Be sure to get the orientation right.                   */
    encroachedseg = (struct badsubseg *) poolalloc(&m->badsubsegs);
    if (encroached == 1) {
      encroachedseg->encsubseg = sencode(*testsubseg);
      encroachedseg->subsegorg = eorg;
      encroachedseg->subsegdest = edest;
    } else {
      encroachedseg->encsubseg = sencode(testsym);
      encroachedseg->subsegorg = edest;
      encroachedseg->subsegdest = eorg;
    }
  }

  return encroached;
}

void checkmesh(struct mesh *m, struct behavior *b)
{
  struct otri triangleloop;
  struct otri oppotri, oppooppotri;
  vertex triorg, tridest, triapex;
  vertex oppoorg, oppodest;
  int horrors;
  int saveexact;
  triangle ptr;                         /* Temporary variable used by sym(). */

  /* Temporarily turn on exact arithmetic if it's off. */
  saveexact = b->noexact;
  b->noexact = 0;
  if (!b->quiet) {
    printf("  Checking consistency of mesh...\n");
  }
  horrors = 0;
  /* Run through the list of triangles, checking each one. */
  traversalinit(&m->triangles);
  triangleloop.tri = triangletraverse(m);
  while (triangleloop.tri != (triangle *) NULL) {
    /* Check all three edges of the triangle. */
    for (triangleloop.orient = 0; triangleloop.orient < 3;
         triangleloop.orient++) {
      org(triangleloop, triorg);
      dest(triangleloop, tridest);
      if (triangleloop.orient == 0) {       /* Only test for inversion once. */
        /* Test if the triangle is flat or inverted. */
        apex(triangleloop, triapex);
        if (counterclockwise(m, b, triorg, tridest, triapex) <= 0.0) {
          printf("  !! !! Inverted ");
          printtriangle(m, b, &triangleloop);
          horrors++;
        }
      }
      /* Find the neighboring triangle on this edge. */
      sym(triangleloop, oppotri);
      if (oppotri.tri != m->dummytri) {
        /* Check that the triangle's neighbor knows it's a neighbor. */
        sym(oppotri, oppooppotri);
        if ((triangleloop.tri != oppooppotri.tri)
            || (triangleloop.orient != oppooppotri.orient)) {
          printf("  !! !! Asymmetric triangle-triangle bond:\n");
          if (triangleloop.tri == oppooppotri.tri) {
            printf("   (Right triangle, wrong orientation)\n");
          }
          printf("    First ");
          printtriangle(m, b, &triangleloop);
          printf("    Second (nonreciprocating) ");
          printtriangle(m, b, &oppotri);
          horrors++;
        }
        /* Check that both triangles agree on the identities */
        /*   of their shared vertices.                       */
        org(oppotri, oppoorg);
        dest(oppotri, oppodest);
        if ((triorg != oppodest) || (tridest != oppoorg)) {
          printf("  !! !! Mismatched edge coordinates between two triangles:\n"
                 );
          printf("    First mismatched ");
          printtriangle(m, b, &triangleloop);
          printf("    Second mismatched ");
          printtriangle(m, b, &oppotri);
          horrors++;
        }
      }
    }
    triangleloop.tri = triangletraverse(m);
  }
  if (horrors == 0) {
    if (!b->quiet) {
      printf("  In my studied opinion, the mesh appears to be consistent.\n");
    }
  } else if (horrors == 1) {
    printf("  !! !! !! !! Precisely one festering wound discovered.\n");
  } else {
    printf("  !! !! !! !! %d abominations witnessed.\n", horrors);
  }
  /* Restore the status of exact arithmetic. */
  b->noexact = saveexact;
}

/*****************************************************************************/
/*                                                                           */
/*  checkdelaunay()   Ensure that the mesh is (constrained) Delaunay.        */
/*                                                                           */
/*****************************************************************************/

void checkdelaunay(struct mesh *m, struct behavior *b)
{
  struct otri triangleloop;
  struct otri oppotri;
  struct osub opposubseg;
  vertex triorg, tridest, triapex;
  vertex oppoapex;
  int shouldbedelaunay;
  int horrors;
  int saveexact;
  triangle ptr;                         /* Temporary variable used by sym(). */
  subseg sptr;                      /* Temporary variable used by tspivot(). */

  /* Temporarily turn on exact arithmetic if it's off. */
  saveexact = b->noexact;
  b->noexact = 0;
  if (!b->quiet) {
    printf("  Checking Delaunay property of mesh...\n");
  }
  horrors = 0;
  /* Run through the list of triangles, checking each one. */
  traversalinit(&m->triangles);
  triangleloop.tri = triangletraverse(m);
  while (triangleloop.tri != (triangle *) NULL) {
    /* Check all three edges of the triangle. */
    for (triangleloop.orient = 0; triangleloop.orient < 3;
         triangleloop.orient++) {
      org(triangleloop, triorg);
      dest(triangleloop, tridest);
      apex(triangleloop, triapex);
      sym(triangleloop, oppotri);
      apex(oppotri, oppoapex);
      /* Only test that the edge is locally Delaunay if there is an   */
      /*   adjoining triangle whose pointer is larger (to ensure that */
      /*   each pair isn't tested twice).                             */
      shouldbedelaunay = (oppotri.tri != m->dummytri) &&
            !deadtri(oppotri.tri) && (triangleloop.tri < oppotri.tri) &&
            (triorg != m->infvertex1) && (triorg != m->infvertex2) &&
            (triorg != m->infvertex3) &&
            (tridest != m->infvertex1) && (tridest != m->infvertex2) &&
            (tridest != m->infvertex3) &&
            (triapex != m->infvertex1) && (triapex != m->infvertex2) &&
            (triapex != m->infvertex3) &&
            (oppoapex != m->infvertex1) && (oppoapex != m->infvertex2) &&
            (oppoapex != m->infvertex3);
      if (m->checksegments && shouldbedelaunay) {
        /* If a subsegment separates the triangles, then the edge is */
        /*   constrained, so no local Delaunay test should be done.  */
        tspivot(triangleloop, opposubseg);
        if (opposubseg.ss != m->dummysub){
          shouldbedelaunay = 0;
        }
      }
      if (shouldbedelaunay) {
        if (nonregular(m, b, triorg, tridest, triapex, oppoapex) > 0.0) {
          if (!b->weighted) {
            printf("  !! !! Non-Delaunay pair of triangles:\n");
            printf("    First non-Delaunay ");
            printtriangle(m, b, &triangleloop);
            printf("    Second non-Delaunay ");
          } else {
            printf("  !! !! Non-regular pair of triangles:\n");
            printf("    First non-regular ");
            printtriangle(m, b, &triangleloop);
            printf("    Second non-regular ");
          }
          printtriangle(m, b, &oppotri);
          horrors++;
        }
      }
    }
    triangleloop.tri = triangletraverse(m);
  }
  if (horrors == 0) {
    if (!b->quiet) {
      printf(
  "  By virtue of my perceptive intelligence, I declare the mesh Delaunay.\n");
    }
  } else if (horrors == 1) {
    printf(
         "  !! !! !! !! Precisely one terrifying transgression identified.\n");
  } else {
    printf("  !! !! !! !! %d obscenities viewed with horror.\n", horrors);
  }
  /* Restore the status of exact arithmetic. */
  b->noexact = saveexact;
}

/*****************************************************************************/
/*                                                                           */
/*  dequeuebadtriang()   Remove a triangle from the front of the queue.      */
/*                                                                           */
/*****************************************************************************/

struct badtriang *dequeuebadtriang(struct mesh *m)
{
  struct badtriang *result;

  /* If no queues are nonempty, return NULL. */
  if (m->firstnonemptyq < 0) {
    return (struct badtriang *) NULL;
  }
  /* Find the first triangle of the highest-priority queue. */
  result = m->queuefront[m->firstnonemptyq];
  /* Remove the triangle from the queue. */
  m->queuefront[m->firstnonemptyq] = result->nexttriang;
  /* If this queue is now empty, note the new highest-priority */
  /*   nonempty queue.                                         */
  if (result == m->queuetail[m->firstnonemptyq]) {
    m->firstnonemptyq = m->nextnonemptyq[m->firstnonemptyq];
  }
  return result;
}

/*****************************************************************************/
/*                                                                           */
/*  enqueuebadtri()   Add a bad triangle to the end of a queue.              */
/*                                                                           */
/*  Allocates a badtriang data structure for the triangle, then passes it to */
/*  enqueuebadtriang().                                                      */
/*                                                                           */
/*****************************************************************************/

void enqueuebadtri(struct mesh *m, struct behavior *b, struct otri *enqtri,
                   REAL minedge, vertex enqapex, vertex enqorg, vertex enqdest)
{
  struct badtriang *newbad;

  /* Allocate space for the bad triangle. */
  newbad = (struct badtriang *) poolalloc(&m->badtriangles);
  newbad->poortri = encode(*enqtri);
  newbad->key = minedge;
  newbad->triangapex = enqapex;
  newbad->triangorg = enqorg;
  newbad->triangdest = enqdest;
  enqueuebadtriang(m, b, newbad);
}

/*****************************************************************************/
/*                                                                           */
/*  enqueuebadtriang()   Add a bad triangle data structure to the end of a   */
/*                       queue.                                              */
/*                                                                           */
/*  The queue is actually a set of 4096 queues.  I use multiple queues to    */
/*  give priority to smaller angles.  I originally implemented a heap, but   */
/*  the queues are faster by a larger margin than I'd suspected.             */
/*                                                                           */
/*****************************************************************************/

void enqueuebadtriang(struct mesh *m, struct behavior *b, struct badtriang *badtri)
{
  REAL length, multiplier;
  int exponent, expincrement;
  int queuenumber;
  int posexponent;
  int i;

  if (b->verbose > 2) {
    printf("  Queueing bad triangle:\n");
    printf("    (%.12g, %.12g) (%.12g, %.12g) (%.12g, %.12g)\n",
           badtri->triangorg[0], badtri->triangorg[1],
           badtri->triangdest[0], badtri->triangdest[1],
           badtri->triangapex[0], badtri->triangapex[1]);
  }

  /* Determine the appropriate queue to put the bad triangle into.    */
  /*   Recall that the key is the square of its shortest edge length. */
  if (badtri->key >= 1.0) {
    length = badtri->key;
    posexponent = 1;
  } else {
    /* `badtri->key' is 2.0 to a negative exponent, so we'll record that */
    /*   fact and use the reciprocal of `badtri->key', which is > 1.0.   */
    length = 1.0 / badtri->key;
    posexponent = 0;
  }
  /* `length' is approximately 2.0 to what exponent?  The following code */
  /*   determines the answer in time logarithmic in the exponent.        */
  exponent = 0;
  while (length > 2.0) {
    /* Find an approximation by repeated squaring of two. */
    expincrement = 1;
    multiplier = 0.5;
    while (length * multiplier * multiplier > 1.0) {
      expincrement *= 2;
      multiplier *= multiplier;
    }
    /* Reduce the value of `length', then iterate if necessary. */
    exponent += expincrement;
    length *= multiplier;
  }
  /* `length' is approximately squareroot(2.0) to what exponent? */
  exponent = 2.0 * exponent + (length > SQUAREROOTTWO);
  /* `exponent' is now in the range 0...2047 for IEEE double precision.   */
  /*   Choose a queue in the range 0...4095.  The shortest edges have the */
  /*   highest priority (queue 4095).                                     */
  if (posexponent) {
    queuenumber = 2047 - exponent;
  } else {
    queuenumber = 2048 + exponent;
  }

  /* Are we inserting into an empty queue? */
  if (m->queuefront[queuenumber] == (struct badtriang *) NULL) {
    /* Yes, we are inserting into an empty queue.     */
    /*   Will this become the highest-priority queue? */
    if (queuenumber > m->firstnonemptyq) {
      /* Yes, this is the highest-priority queue. */
      m->nextnonemptyq[queuenumber] = m->firstnonemptyq;
      m->firstnonemptyq = queuenumber;
    } else {
      /* No, this is not the highest-priority queue. */
      /*   Find the queue with next higher priority. */
      i = queuenumber + 1;
      while (m->queuefront[i] == (struct badtriang *) NULL) {
        i++;
      }
      /* Mark the newly nonempty queue as following a higher-priority queue. */
      m->nextnonemptyq[queuenumber] = m->nextnonemptyq[i];
      m->nextnonemptyq[i] = queuenumber;
    }
    /* Put the bad triangle at the beginning of the (empty) queue. */
    m->queuefront[queuenumber] = badtri;
  } else {
    /* Add the bad triangle to the end of an already nonempty queue. */
    m->queuetail[queuenumber]->nexttriang = badtri;
  }
  /* Maintain a pointer to the last triangle of the queue. */
  m->queuetail[queuenumber] = badtri;
  /* Newly enqueued bad triangle has no successor in the queue. */
  badtri->nexttriang = (struct badtriang *) NULL;
}

/*****************************************************************************/
/*                                                                           */
/*  testtriangle()   Test a triangle for quality and size.                   */
/*                                                                           */
/*  Tests a triangle to see if it satisfies the minimum angle condition and  */
/*  the maximum area condition.  Triangles that aren't up to spec are added  */
/*  to the bad triangle queue.                                               */
/*                                                                           */
/*****************************************************************************/

void testtriangle(struct mesh *m, struct behavior *b, struct otri *testtri)
{
  struct otri tri1, tri2;
  struct osub testsub;
  vertex torg, tdest, tapex;
  vertex base1, base2;
  vertex org1, dest1, org2, dest2;
  vertex joinvertex;
  REAL dxod, dyod, dxda, dyda, dxao, dyao;
  REAL dxod2, dyod2, dxda2, dyda2, dxao2, dyao2;
  REAL apexlen, orglen, destlen, minedge;
  REAL angle;
  REAL area;
  REAL dist1, dist2;
  subseg sptr;                      /* Temporary variable used by tspivot(). */
  triangle ptr;           /* Temporary variable used by oprev() and dnext(). */

  org(*testtri, torg);
  dest(*testtri, tdest);
  apex(*testtri, tapex);
  dxod = torg[0] - tdest[0];
  dyod = torg[1] - tdest[1];
  dxda = tdest[0] - tapex[0];
  dyda = tdest[1] - tapex[1];
  dxao = tapex[0] - torg[0];
  dyao = tapex[1] - torg[1];
  dxod2 = dxod * dxod;
  dyod2 = dyod * dyod;
  dxda2 = dxda * dxda;
  dyda2 = dyda * dyda;
  dxao2 = dxao * dxao;
  dyao2 = dyao * dyao;
  /* Find the lengths of the triangle's three edges. */
  apexlen = dxod2 + dyod2;
  orglen = dxda2 + dyda2;
  destlen = dxao2 + dyao2;

  if ((apexlen < orglen) && (apexlen < destlen)) {
    /* The edge opposite the apex is shortest. */
    minedge = apexlen;
    /* Find the square of the cosine of the angle at the apex. */
    angle = dxda * dxao + dyda * dyao;
    angle = angle * angle / (orglen * destlen);
    base1 = torg;
    base2 = tdest;
    otricopy(*testtri, tri1);
  } else if (orglen < destlen) {
    /* The edge opposite the origin is shortest. */
    minedge = orglen;
    /* Find the square of the cosine of the angle at the origin. */
    angle = dxod * dxao + dyod * dyao;
    angle = angle * angle / (apexlen * destlen);
    base1 = tdest;
    base2 = tapex;
    lnext(*testtri, tri1);
  } else {
    /* The edge opposite the destination is shortest. */
    minedge = destlen;
    /* Find the square of the cosine of the angle at the destination. */
    angle = dxod * dxda + dyod * dyda;
    angle = angle * angle / (apexlen * orglen);
    base1 = tapex;
    base2 = torg;
    lprev(*testtri, tri1);
  }

  if (b->vararea || b->fixedarea || b->usertest) {
    /* Check whether the area is larger than permitted. */
    area = 0.5 * (dxod * dyda - dyod * dxda);
    if (b->fixedarea && (area > b->maxarea)) {
      /* Add this triangle to the list of bad triangles. */
      enqueuebadtri(m, b, testtri, minedge, tapex, torg, tdest);
      return;
    }

    /* Nonpositive area constraints are treated as unconstrained. */
    if ((b->vararea) && (area > areabound(*testtri)) &&
        (areabound(*testtri) > 0.0)) {
      /* Add this triangle to the list of bad triangles. */
      enqueuebadtri(m, b, testtri, minedge, tapex, torg, tdest);
      return;
    }

    if (b->usertest) {
      /* Check whether the user thinks this triangle is too large. */
      if (triunsuitable(torg, tdest, tapex, area)) {
        enqueuebadtri(m, b, testtri, minedge, tapex, torg, tdest);
        return;
      }
    }
  }

  /* Check whether the angle is smaller than permitted. */
  if (angle > b->goodangle) {
    /* Use the rules of Miller, Pav, and Walkington to decide that certain */
    /*   triangles should not be split, even if they have bad angles.      */
    /*   A skinny triangle is not split if its shortest edge subtends a    */
    /*   small input angle, and both endpoints of the edge lie on a        */
    /*   concentric circular shell.  For convenience, I make a small       */
    /*   adjustment to that rule:  I check if the endpoints of the edge    */
    /*   both lie in segment interiors, equidistant from the apex where    */
    /*   the two segments meet.                                            */
    /* First, check if both points lie in segment interiors.               */
    if ((vertextype(base1) == SEGMENTVERTEX) &&
        (vertextype(base2) == SEGMENTVERTEX)) {
      /* Check if both points lie in a common segment.  If they do, the */
      /*   skinny triangle is enqueued to be split as usual.            */
      tspivot(tri1, testsub);
      if (testsub.ss == m->dummysub) {
        /* No common segment.  Find a subsegment that contains `torg'. */
        otricopy(tri1, tri2);
        do {
          oprevself(tri1);
          tspivot(tri1, testsub);
        } while (testsub.ss == m->dummysub);
        /* Find the endpoints of the containing segment. */
        segorg(testsub, org1);
        segdest(testsub, dest1);
        /* Find a subsegment that contains `tdest'. */
        do {
          dnextself(tri2);
          tspivot(tri2, testsub);
        } while (testsub.ss == m->dummysub);
        /* Find the endpoints of the containing segment. */
        segorg(testsub, org2);
        segdest(testsub, dest2);
        /* Check if the two containing segments have an endpoint in common. */
        joinvertex = (vertex) NULL;
        if ((dest1[0] == org2[0]) && (dest1[1] == org2[1])) {
          joinvertex = dest1;
        } else if ((org1[0] == dest2[0]) && (org1[1] == dest2[1])) {
          joinvertex = org1;
        }
        if (joinvertex != (vertex) NULL) {
          /* Compute the distance from the common endpoint (of the two  */
          /*   segments) to each of the endpoints of the shortest edge. */
          dist1 = ((base1[0] - joinvertex[0]) * (base1[0] - joinvertex[0]) +
                   (base1[1] - joinvertex[1]) * (base1[1] - joinvertex[1]));
          dist2 = ((base2[0] - joinvertex[0]) * (base2[0] - joinvertex[0]) +
                   (base2[1] - joinvertex[1]) * (base2[1] - joinvertex[1]));
          /* If the two distances are equal, don't split the triangle. */
          if ((dist1 < 1.001 * dist2) && (dist1 > 0.999 * dist2)) {
            /* Return now to avoid enqueueing the bad triangle. */
            return;
          }
        }
      }
    }

    /* Add this triangle to the list of bad triangles. */
    enqueuebadtri(m, b, testtri, minedge, tapex, torg, tdest);
  }
}
