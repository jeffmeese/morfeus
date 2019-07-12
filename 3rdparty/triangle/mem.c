#include "mem.h"
#include "pool.h"
#include "primitives.h"
#include "user.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*****************************************************************************/
/*                                                                           */
/*  triangledealloc()   Deallocate space for a triangle, marking it dead.    */
/*                                                                           */
/*****************************************************************************/

void triangledealloc(struct mesh *m, triangle *dyingtriangle)
{
  /* Mark the triangle as dead.  This makes it possible to detect dead */
  /*   triangles when traversing the list of all triangles.            */
  killtri(dyingtriangle);
  pooldealloc(&m->triangles, (void *) dyingtriangle);
}

/*****************************************************************************/
/*                                                                           */
/*  triangletraverse()   Traverse the triangles, skipping dead ones.         */
/*                                                                           */
/*****************************************************************************/

triangle *triangletraverse(struct mesh *m)
{
  triangle *newtriangle;

  do {
    newtriangle = (triangle *) traverse(&m->triangles);
    if (newtriangle == (triangle *) NULL) {
      return (triangle *) NULL;
    }
  } while (deadtri(newtriangle));                         /* Skip dead ones. */
  return newtriangle;
}

/*****************************************************************************/
/*                                                                           */
/*  subsegdealloc()   Deallocate space for a subsegment, marking it dead.    */
/*                                                                           */
/*****************************************************************************/

void subsegdealloc(struct mesh *m, subseg *dyingsubseg)
{
  /* Mark the subsegment as dead.  This makes it possible to detect dead */
  /*   subsegments when traversing the list of all subsegments.          */
  killsubseg(dyingsubseg);
  pooldealloc(&m->subsegs, (void *) dyingsubseg);
}

/*****************************************************************************/
/*                                                                           */
/*  subsegtraverse()   Traverse the subsegments, skipping dead ones.         */
/*                                                                           */
/*****************************************************************************/

subseg *subsegtraverse(struct mesh *m)
{
  subseg *newsubseg;

  do {
    newsubseg = (subseg *) traverse(&m->subsegs);
    if (newsubseg == (subseg *) NULL) {
      return (subseg *) NULL;
    }
  } while (deadsubseg(newsubseg));                        /* Skip dead ones. */
  return newsubseg;
}

/*****************************************************************************/
/*                                                                           */
/*  vertexdealloc()   Deallocate space for a vertex, marking it dead.        */
/*                                                                           */
/*****************************************************************************/

void vertexdealloc(struct mesh *m, vertex dyingvertex)
{
  /* Mark the vertex as dead.  This makes it possible to detect dead */
  /*   vertices when traversing the list of all vertices.            */
  setvertextype(dyingvertex, DEADVERTEX);
  pooldealloc(&m->vertices, (void *) dyingvertex);
}

/*****************************************************************************/
/*                                                                           */
/*  vertextraverse()   Traverse the vertices, skipping dead ones.            */
/*                                                                           */
/*****************************************************************************/

vertex vertextraverse(struct mesh *m)
{
  vertex newvertex;

  do {
    newvertex = (vertex) traverse(&m->vertices);
    if (newvertex == (vertex) NULL) {
      return (vertex) NULL;
    }
  } while (vertextype(newvertex) == DEADVERTEX);          /* Skip dead ones. */
  return newvertex;
}

/*****************************************************************************/
/*                                                                           */
/*  badsubsegdealloc()   Deallocate space for a bad subsegment, marking it   */
/*                       dead.                                               */
/*                                                                           */
/*****************************************************************************/


void badsubsegdealloc(struct mesh *m, struct badsubseg *dyingseg)
{
  /* Set subsegment's origin to NULL.  This makes it possible to detect dead */
  /*   badsubsegs when traversing the list of all badsubsegs             .   */
  dyingseg->subsegorg = (vertex) NULL;
  pooldealloc(&m->badsubsegs, (void *) dyingseg);
}

/*****************************************************************************/
/*                                                                           */
/*  badsubsegtraverse()   Traverse the bad subsegments, skipping dead ones.  */
/*                                                                           */
/*****************************************************************************/

struct badsubseg *badsubsegtraverse(struct mesh *m)
{
  struct badsubseg *newseg;

  do {
    newseg = (struct badsubseg *) traverse(&m->badsubsegs);
    if (newseg == (struct badsubseg *) NULL) {
      return (struct badsubseg *) NULL;
    }
  } while (newseg->subsegorg == (vertex) NULL);           /* Skip dead ones. */
  return newseg;
}

void *trimalloc(int size)
{
  void *memptr;

  memptr = (void *) malloc((unsigned int) size);
  if (memptr == (void *) NULL) {
    printf("Error:  Out of memory.\n");
    triexit(1);
  }
  return(memptr);
}

void trifree(void *memptr)
{
  free(memptr);
}

/********* Memory management routines begin here                     *********/
/**                                                                         **/
/**                                                                         **/



/*****************************************************************************/
/*                                                                           */
/*  dummyinit()   Initialize the triangle that fills "outer space" and the   */
/*                omnipresent subsegment.                                    */
/*                                                                           */
/*  The triangle that fills "outer space," called `dummytri', is pointed to  */
/*  by every triangle and subsegment on a boundary (be it outer or inner) of */
/*  the triangulation.  Also, `dummytri' points to one of the triangles on   */
/*  the convex hull (until the holes and concavities are carved), making it  */
/*  possible to find a starting triangle for point location.                 */
/*                                                                           */
/*  The omnipresent subsegment, `dummysub', is pointed to by every triangle  */
/*  or subsegment that doesn't have a full complement of real subsegments    */
/*  to point to.                                                             */
/*                                                                           */
/*  `dummytri' and `dummysub' are generally required to fulfill only a few   */
/*  invariants:  their vertices must remain NULL and `dummytri' must always  */
/*  be bonded (at offset zero) to some triangle on the convex hull of the    */
/*  mesh, via a boundary edge.  Otherwise, the connections of `dummytri' and */
/*  `dummysub' may change willy-nilly.  This makes it possible to avoid      */
/*  writing a good deal of special-case code (in the edge flip, for example) */
/*  for dealing with the boundary of the mesh, places where no subsegment is */
/*  present, and so forth.  Other entities are frequently bonded to          */
/*  `dummytri' and `dummysub' as if they were real mesh entities, with no    */
/*  harm done.                                                               */
/*                                                                           */
/*****************************************************************************/

void dummyinit(struct mesh *m, struct behavior *b, int trianglebytes, int subsegbytes)
{
  unsigned ULONGLONG alignptr;

  /* Set up `dummytri', the `triangle' that occupies "outer space." */
  m->dummytribase = (triangle *) trimalloc(trianglebytes +
                                           m->triangles.alignbytes);
  /* Align `dummytri' on a `triangles.alignbytes'-byte boundary. */
  alignptr = (unsigned ULONGLONG) m->dummytribase;
  m->dummytri = (triangle *)
    (alignptr + (unsigned ULONGLONG) m->triangles.alignbytes -
     (alignptr % (unsigned ULONGLONG) m->triangles.alignbytes));
  /* Initialize the three adjoining triangles to be "outer space."  These  */
  /*   will eventually be changed by various bonding operations, but their */
  /*   values don't really matter, as ULONGLONG as they can legally be          */
  /*   dereferenced.                                                       */
  m->dummytri[0] = (triangle) m->dummytri;
  m->dummytri[1] = (triangle) m->dummytri;
  m->dummytri[2] = (triangle) m->dummytri;
  /* Three NULL vertices. */
  m->dummytri[3] = (triangle) NULL;
  m->dummytri[4] = (triangle) NULL;
  m->dummytri[5] = (triangle) NULL;

  if (b->usesegments) {
    /* Set up `dummysub', the omnipresent subsegment pointed to by any */
    /*   triangle side or subsegment end that isn't attached to a real */
    /*   subsegment.                                                   */
    m->dummysubbase = (subseg *) trimalloc(subsegbytes +
                                           m->subsegs.alignbytes);
    /* Align `dummysub' on a `subsegs.alignbytes'-byte boundary. */
    alignptr = (unsigned ULONGLONG) m->dummysubbase;
    m->dummysub = (subseg *)
      (alignptr + (unsigned ULONGLONG) m->subsegs.alignbytes -
       (alignptr % (unsigned ULONGLONG) m->subsegs.alignbytes));
    /* Initialize the two adjoining subsegments to be the omnipresent      */
    /*   subsegment.  These will eventually be changed by various bonding  */
    /*   operations, but their values don't really matter, as ULONGLONG as they */
    /*   can legally be dereferenced.                                      */
    m->dummysub[0] = (subseg) m->dummysub;
    m->dummysub[1] = (subseg) m->dummysub;
    /* Four NULL vertices. */
    m->dummysub[2] = (subseg) NULL;
    m->dummysub[3] = (subseg) NULL;
    m->dummysub[4] = (subseg) NULL;
    m->dummysub[5] = (subseg) NULL;
    /* Initialize the two adjoining triangles to be "outer space." */
    m->dummysub[6] = (subseg) m->dummytri;
    m->dummysub[7] = (subseg) m->dummytri;
    /* Set the boundary marker to zero. */
    * (int *) (m->dummysub + 8) = 0;

    /* Initialize the three adjoining subsegments of `dummytri' to be */
    /*   the omnipresent subsegment.                                  */
    m->dummytri[6] = (triangle) m->dummysub;
    m->dummytri[7] = (triangle) m->dummysub;
    m->dummytri[8] = (triangle) m->dummysub;
  }
}

/*****************************************************************************/
/*                                                                           */
/*  initializevertexpool()   Calculate the size of the vertex data structure */
/*                           and initialize its memory pool.                 */
/*                                                                           */
/*  This routine also computes the `vertexmarkindex' and `vertex2triindex'   */
/*  indices used to find values within each vertex.                          */
/*                                                                           */
/*****************************************************************************/

void initializevertexpool(struct mesh *m, struct behavior *b)
{
  int vertexsize;

  /* The index within each vertex at which the boundary marker is found,    */
  /*   followed by the vertex type.  Ensure the vertex marker is aligned to */
  /*   a sizeof(int)-byte address.                                          */
  m->vertexmarkindex = ((m->mesh_dim + m->nextras) * sizeof(REAL) +
                        sizeof(int) - 1) /
                       sizeof(int);
  vertexsize = (m->vertexmarkindex + 2) * sizeof(int);
  if (b->poly) {
    /* The index within each vertex at which a triangle pointer is found.  */
    /*   Ensure the pointer is aligned to a sizeof(triangle)-byte address. */
    m->vertex2triindex = (vertexsize + sizeof(triangle) - 1) /
                         sizeof(triangle);
    vertexsize = (m->vertex2triindex + 1) * sizeof(triangle);
  }

  /* Initialize the pool of vertices. */
  poolinit(&m->vertices, vertexsize, VERTEXPERBLOCK,
           m->invertices > VERTEXPERBLOCK ? m->invertices : VERTEXPERBLOCK,
           sizeof(REAL));
}

/*****************************************************************************/
/*                                                                           */
/*  initializetrisubpools()   Calculate the sizes of the triangle and        */
/*                            subsegment data structures and initialize      */
/*                            their memory pools.                            */
/*                                                                           */
/*  This routine also computes the `highorderindex', `elemattribindex', and  */
/*  `areaboundindex' indices used to find values within each triangle.       */
/*                                                                           */
/*****************************************************************************/

void initializetrisubpools(struct mesh *m, struct behavior *b)
{
  int trisize;

  /* The index within each triangle at which the extra nodes (above three)  */
  /*   associated with high order elements are found.  There are three      */
  /*   pointers to other triangles, three pointers to corners, and possibly */
  /*   three pointers to subsegments before the extra nodes.                */
  m->highorderindex = 6 + (b->usesegments * 3);
  /* The number of bytes occupied by a triangle. */
  trisize = ((b->order + 1) * (b->order + 2) / 2 + (m->highorderindex - 3)) *
            sizeof(triangle);
  /* The index within each triangle at which its attributes are found, */
  /*   where the index is measured in REALs.                           */
  m->elemattribindex = (trisize + sizeof(REAL) - 1) / sizeof(REAL);
  /* The index within each triangle at which the maximum area constraint  */
  /*   is found, where the index is measured in REALs.  Note that if the  */
  /*   `regionattrib' flag is set, an additional attribute will be added. */
  m->areaboundindex = m->elemattribindex + m->eextras + b->regionattrib;
  /* If triangle attributes or an area bound are needed, increase the number */
  /*   of bytes occupied by a triangle.                                      */
  if (b->vararea) {
    trisize = (m->areaboundindex + 1) * sizeof(REAL);
  } else if (m->eextras + b->regionattrib > 0) {
    trisize = m->areaboundindex * sizeof(REAL);
  }
  /* If a Voronoi diagram or triangle neighbor graph is requested, make    */
  /*   sure there's room to store an integer index in each triangle.  This */
  /*   integer index can occupy the same space as the subsegment pointers  */
  /*   or attributes or area constraint or extra nodes.                    */
  if ((b->voronoi || b->neighbors) &&
      (trisize < 6 * sizeof(triangle) + sizeof(int))) {
    trisize = 6 * sizeof(triangle) + sizeof(int);
  }

  /* Having determined the memory size of a triangle, initialize the pool. */
  poolinit(&m->triangles, trisize, TRIPERBLOCK,
           (2 * m->invertices - 2) > TRIPERBLOCK ? (2 * m->invertices - 2) :
           TRIPERBLOCK, 4);

  if (b->usesegments) {
    /* Initialize the pool of subsegments.  Take into account all eight */
    /*   pointers and one boundary marker.                              */
    poolinit(&m->subsegs, 8 * sizeof(triangle) + sizeof(int),
             SUBSEGPERBLOCK, SUBSEGPERBLOCK, 4);

    /* Initialize the "outer space" triangle and omnipresent subsegment. */
    dummyinit(m, b, m->triangles.itembytes, m->subsegs.itembytes);
  } else {
    /* Initialize the "outer space" triangle. */
    dummyinit(m, b, m->triangles.itembytes, 0);
  }
}

/*****************************************************************************/
/*                                                                           */
/*  getvertex()   Get a specific vertex, by number, from the list.           */
/*                                                                           */
/*  The first vertex is number 'firstnumber'.                                */
/*                                                                           */
/*  Note that this takes O(n) time (with a small constant, if VERTEXPERBLOCK */
/*  is large).  I don't care to take the trouble to make it work in constant */
/*  time.                                                                    */
/*                                                                           */
/*****************************************************************************/

vertex getvertex(struct mesh *m, struct behavior *b, int number)
{
  void **getblock;
  char *foundvertex;
  unsigned ULONGLONG alignptr;
  int current;

  getblock = m->vertices.firstblock;
  current = b->firstnumber;

  /* Find the right block. */
  if (current + m->vertices.itemsfirstblock <= number) {
    getblock = (void **) *getblock;
    current += m->vertices.itemsfirstblock;
    while (current + m->vertices.itemsperblock <= number) {
      getblock = (void **) *getblock;
      current += m->vertices.itemsperblock;
    }
  }

  /* Now find the right vertex. */
  alignptr = (unsigned ULONGLONG) (getblock + 1);
  foundvertex = (char *) (alignptr + (unsigned ULONGLONG) m->vertices.alignbytes -
                          (alignptr % (unsigned ULONGLONG) m->vertices.alignbytes));
  return (vertex) (foundvertex + m->vertices.itembytes * (number - current));
}

/*****************************************************************************/
/*                                                                           */
/*  triangledeinit()   Free all remaining allocated memory.                  */
/*                                                                           */
/*****************************************************************************/

void triangledeinit(struct mesh *m, struct behavior *b)
{
  pooldeinit(&m->triangles);
  trifree((void *) m->dummytribase);
  if (b->usesegments) {
    pooldeinit(&m->subsegs);
    trifree((void *) m->dummysubbase);
  }
  pooldeinit(&m->vertices);
  if (b->quality) {
    pooldeinit(&m->badsubsegs);
    if ((b->minangle > 0.0) || b->vararea || b->fixedarea || b->usertest) {
      pooldeinit(&m->badtriangles);
      pooldeinit(&m->flipstackers);
    }
  }
}

/**                                                                         **/
/**                                                                         **/
/********* Memory management routines end here                       *********/
