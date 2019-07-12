#include "construct.h"

#include "geom.h"
#include "mem.h"

/********* Constructors begin here                                   *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  maketriangle()   Create a new triangle with orientation zero.            */
/*                                                                           */
/*****************************************************************************/

void maketriangle(struct mesh *m, struct behavior *b, struct otri *newotri)
{
  int i;

  newotri->tri = (triangle *) poolalloc(&m->triangles);
  /* Initialize the three adjoining triangles to be "outer space". */
  newotri->tri[0] = (triangle) m->dummytri;
  newotri->tri[1] = (triangle) m->dummytri;
  newotri->tri[2] = (triangle) m->dummytri;
  /* Three NULL vertices. */
  newotri->tri[3] = (triangle) NULL;
  newotri->tri[4] = (triangle) NULL;
  newotri->tri[5] = (triangle) NULL;
  if (b->usesegments) {
    /* Initialize the three adjoining subsegments to be the omnipresent */
    /*   subsegment.                                                    */
    newotri->tri[6] = (triangle) m->dummysub;
    newotri->tri[7] = (triangle) m->dummysub;
    newotri->tri[8] = (triangle) m->dummysub;
  }
  for (i = 0; i < m->eextras; i++) {
    setelemattribute(*newotri, i, 0.0);
  }
  if (b->vararea) {
    setareabound(*newotri, -1.0);
  }

  newotri->orient = 0;
}

/*****************************************************************************/
/*                                                                           */
/*  makesubseg()   Create a new subsegment with orientation zero.            */
/*                                                                           */
/*****************************************************************************/

void makesubseg(struct mesh *m, struct osub *newsubseg)
{
  newsubseg->ss = (subseg *) poolalloc(&m->subsegs);
  /* Initialize the two adjoining subsegments to be the omnipresent */
  /*   subsegment.                                                  */
  newsubseg->ss[0] = (subseg) m->dummysub;
  newsubseg->ss[1] = (subseg) m->dummysub;
  /* Four NULL vertices. */
  newsubseg->ss[2] = (subseg) NULL;
  newsubseg->ss[3] = (subseg) NULL;
  newsubseg->ss[4] = (subseg) NULL;
  newsubseg->ss[5] = (subseg) NULL;
  /* Initialize the two adjoining triangles to be "outer space." */
  newsubseg->ss[6] = (subseg) m->dummytri;
  newsubseg->ss[7] = (subseg) m->dummytri;
  /* Set the boundary marker to zero. */
  setmark(*newsubseg, 0);

  newsubseg->ssorient = 0;
}

/**                                                                         **/
/**                                                                         **/
/********* Constructors end here                                     *********/
