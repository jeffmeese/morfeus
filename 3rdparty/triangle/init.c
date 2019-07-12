#include "init.h"

#include "pool.h"
#include "geom.h"
#include "random.h"

/*****************************************************************************/
/*                                                                           */
/*  triangleinit()   Initialize some variables.                              */
/*                                                                           */
/*****************************************************************************/

void triangleinit(struct mesh *m)
{
  poolzero(&m->vertices);
  poolzero(&m->triangles);
  poolzero(&m->subsegs);
  poolzero(&m->viri);
  poolzero(&m->badsubsegs);
  poolzero(&m->badtriangles);
  poolzero(&m->flipstackers);
  poolzero(&m->splaynodes);

  m->recenttri.tri = (triangle *) NULL; /* No triangle has been visited yet. */
  m->undeads = 0;                       /* No eliminated input vertices yet. */
  m->samples = 1;         /* Point location should take at least one sample. */
  m->checksegments = 0;   /* There are no segments in the triangulation yet. */
  m->checkquality = 0;     /* The quality triangulation stage has not begun. */
  m->incirclecount = m->counterclockcount = m->orient3dcount = 0;
  m->hyperbolacount = m->circletopcount = m->circumcentercount = 0;
  randomseed = 1;

  exactinit();                     /* Initialize exact arithmetic constants. */
}
