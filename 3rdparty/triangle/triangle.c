#include "trimesh.h"
#include "pool.h"

#include <stdlib.h>
#include <stdio.h>

char *readline(char *string, FILE *infile, char *infilename);
void syntax(void);
void readnodes(struct mesh *m, struct behavior *b, char *nodefilename, char *polyfilename, FILE **polyfile);
char *findfield(char *string);

int main(int argc, char **argv)
{
  char * triswitches = NULL;
  struct triangulateio in;
  struct triangulateio out;
  struct triangulateio vorout;
  //FILE *polyfile;

  //readnodes(&m, &b, b.innodefilename, b.inpolyfilename, &polyfile);
  //m.hullsize = reconstruct(&m, &b, b.inelefilename, b.areafilename, b.inpolyfilename, polyfile);
  //formskeleton(&m, &b, polyfile, b.inpolyfilename);
  //readholes(&m, &b, polyfile, b.inpolyfilename, &holearray, &m.holes, &regionarray, &m.regions);

  triangulate(triswitches, &in, &out, &vorout);

  //writenodes(&m, &b, b.outnodefilename, argc, argv);
  //writeelements(&m, &b, b.outelefilename, argc, argv);
  //writepoly(&m, &b, b.outpolyfilename, holearray, m.holes, regionarray, m.regions, argc, argv);
  //writeedges(&m, &b, b.edgefilename, argc, argv);
  //writevoronoi(&m, &b, b.vnodefilename, b.vedgefilename, argc, argv);
  //writeneighbors(&m, &b, b.neighborfilename, argc, argv);

  //if (m.regions > 0) {
  //  trifree((void *) regionarray);
  //}
  //if (m.holes > 0) {
  //  trifree((void *) holearray);
  //}
  //if (b.geomview) {
  //  writeoff(&m, &b, b.offfilename, argc, argv);
  //}
}

/*****************************************************************************/
/*                                                                           */
/*  readline()   Read a nonempty line from a file.                           */
/*                                                                           */
/*  A line is considered "nonempty" if it contains something that looks like */
/*  a number.  Comments (prefaced by `#') are ignored.                       */
/*                                                                           */
/*****************************************************************************/

char *readline(char *string, FILE *infile, char *infilename)
{
  char *result;

  /* Search for something that looks like a number. */
  do {
    result = fgets(string, INPUTLINESIZE, infile);
    if (result == (char *) NULL) {
      printf("  Error:  Unexpected end of file in %s.\n", infilename);
      triexit(1);
    }
    /* Skip anything that doesn't look like a number, a comment, */
    /*   or the end of a line.                                   */
    while ((*result != '\0') && (*result != '#')
           && (*result != '.') && (*result != '+') && (*result != '-')
           && ((*result < '0') || (*result > '9'))) {
      result++;
    }
  /* If it's a comment or end of line, read another line and try again. */
  } while ((*result == '#') || (*result == '\0'));
  return result;
}

/*****************************************************************************/
/*                                                                           */
/*  findfield()   Find the next field of a string.                           */
/*                                                                           */
/*  Jumps past the current field by searching for whitespace, then jumps     */
/*  past the whitespace to find the next field.                              */
/*                                                                           */
/*****************************************************************************/

char *findfield(char *string)
{
  char *result;

  result = string;
  /* Skip the current field.  Stop upon reaching whitespace. */
  while ((*result != '\0') && (*result != '#')
         && (*result != ' ') && (*result != '\t')) {
    result++;
  }
  /* Now skip the whitespace and anything else that doesn't look like a */
  /*   number, a comment, or the end of a line.                         */
  while ((*result != '\0') && (*result != '#')
         && (*result != '.') && (*result != '+') && (*result != '-')
         && ((*result < '0') || (*result > '9'))) {
    result++;
  }
  /* Check for a comment (prefixed with `#'). */
  if (*result == '#') {
    *result = '\0';
  }
  return result;
}

/*****************************************************************************/
/*                                                                           */
/*  readnodes()   Read the vertices from a file, which may be a .node or     */
/*                .poly file.                                                */
/*                                                                           */
/*****************************************************************************/
void readnodes(struct mesh *m, struct behavior *b, char *nodefilename, char *polyfilename, FILE **polyfile)
{
  FILE *infile;
  vertex vertexloop;
  char inputline[INPUTLINESIZE];
  char *stringptr;
  char *infilename;
  REAL x, y;
  int firstnode;
  int nodemarkers;
  int currentmarker;
  int i, j;

  if (b->poly) {
    /* Read the vertices from a .poly file. */
    if (!b->quiet) {
      printf("Opening %s.\n", polyfilename);
    }
    *polyfile = fopen(polyfilename, "r");
    if (*polyfile == (FILE *) NULL) {
      printf("  Error:  Cannot access file %s.\n", polyfilename);
      triexit(1);
    }
    /* Read number of vertices, number of dimensions, number of vertex */
    /*   attributes, and number of boundary markers.                   */
    stringptr = readline(inputline, *polyfile, polyfilename);
    m->invertices = (int) strtol(stringptr, &stringptr, 0);
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      m->mesh_dim = 2;
    } else {
      m->mesh_dim = (int) strtol(stringptr, &stringptr, 0);
    }
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      m->nextras = 0;
    } else {
      m->nextras = (int) strtol(stringptr, &stringptr, 0);
    }
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      nodemarkers = 0;
    } else {
      nodemarkers = (int) strtol(stringptr, &stringptr, 0);
    }
    if (m->invertices > 0) {
      infile = *polyfile;
      infilename = polyfilename;
      m->readnodefile = 0;
    } else {
      /* If the .poly file claims there are zero vertices, that means that */
      /*   the vertices should be read from a separate .node file.         */
      m->readnodefile = 1;
      infilename = nodefilename;
    }
  } else {
    m->readnodefile = 1;
    infilename = nodefilename;
    *polyfile = (FILE *) NULL;
  }

  if (m->readnodefile) {
    /* Read the vertices from a .node file. */
    if (!b->quiet) {
      printf("Opening %s.\n", nodefilename);
    }
    infile = fopen(nodefilename, "r");
    if (infile == (FILE *) NULL) {
      printf("  Error:  Cannot access file %s.\n", nodefilename);
      triexit(1);
    }
    /* Read number of vertices, number of dimensions, number of vertex */
    /*   attributes, and number of boundary markers.                   */
    stringptr = readline(inputline, infile, nodefilename);
    m->invertices = (int) strtol(stringptr, &stringptr, 0);
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      m->mesh_dim = 2;
    } else {
      m->mesh_dim = (int) strtol(stringptr, &stringptr, 0);
    }
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      m->nextras = 0;
    } else {
      m->nextras = (int) strtol(stringptr, &stringptr, 0);
    }
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      nodemarkers = 0;
    } else {
      nodemarkers = (int) strtol(stringptr, &stringptr, 0);
    }
  }

  if (m->invertices < 3) {
    printf("Error:  Input must have at least three input vertices.\n");
    triexit(1);
  }
  if (m->mesh_dim != 2) {
    printf("Error:  Triangle only works with two-dimensional meshes.\n");
    triexit(1);
  }
  if (m->nextras == 0) {
    b->weighted = 0;
  }

  initializevertexpool(m, b);

  /* Read the vertices. */
  for (i = 0; i < m->invertices; i++) {
    vertexloop = (vertex) poolalloc(&m->vertices);
    stringptr = readline(inputline, infile, infilename);
    if (i == 0) {
      firstnode = (int) strtol(stringptr, &stringptr, 0);
      if ((firstnode == 0) || (firstnode == 1)) {
        b->firstnumber = firstnode;
      }
    }
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      printf("Error:  Vertex %d has no x coordinate.\n", b->firstnumber + i);
      triexit(1);
    }
    x = (REAL) strtod(stringptr, &stringptr);
    stringptr = findfield(stringptr);
    if (*stringptr == '\0') {
      printf("Error:  Vertex %d has no y coordinate.\n", b->firstnumber + i);
      triexit(1);
    }
    y = (REAL) strtod(stringptr, &stringptr);
    vertexloop[0] = x;
    vertexloop[1] = y;
    /* Read the vertex attributes. */
    for (j = 2; j < 2 + m->nextras; j++) {
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        vertexloop[j] = 0.0;
      } else {
        vertexloop[j] = (REAL) strtod(stringptr, &stringptr);
      }
    }
    if (nodemarkers) {
      /* Read a vertex marker. */
      stringptr = findfield(stringptr);
      if (*stringptr == '\0') {
        setvertexmark(vertexloop, 0);
      } else {
        currentmarker = (int) strtol(stringptr, &stringptr, 0);
        setvertexmark(vertexloop, currentmarker);
      }
    } else {
      /* If no markers are specified in the file, they default to zero. */
      setvertexmark(vertexloop, 0);
    }
    setvertextype(vertexloop, INPUTVERTEX);
    /* Determine the smallest and largest x and y coordinates. */
    if (i == 0) {
      m->xmin = m->xmax = x;
      m->ymin = m->ymax = y;
    } else {
      m->xmin = (x < m->xmin) ? x : m->xmin;
      m->xmax = (x > m->xmax) ? x : m->xmax;
      m->ymin = (y < m->ymin) ? y : m->ymin;
      m->ymax = (y > m->ymax) ? y : m->ymax;
    }
  }
  if (m->readnodefile) {
    fclose(infile);
  }

  /* Nonexistent x value used as a flag to mark circle events in sweepline */
  /*   Delaunay algorithm.                                                 */
  m->xminextreme = 10 * m->xmin - 9 * m->xmax;
}

/*****************************************************************************/
/*                                                                           */
/*  syntax()   Print list of command line switches.                          */
/*                                                                           */
/*****************************************************************************/

void syntax()
{
#ifdef CDT_ONLY
#ifdef REDUCED
  printf("triangle [-pAcjevngBPNEIOXzo_lQVh] input_file\n");
#else /* not REDUCED */
  printf("triangle [-pAcjevngBPNEIOXzo_iFlCQVh] input_file\n");
#endif /* not REDUCED */
#else /* not CDT_ONLY */
#ifdef REDUCED
  printf("triangle [-prq__a__uAcDjevngBPNEIOXzo_YS__lQVh] input_file\n");
#else /* not REDUCED */
  printf("triangle [-prq__a__uAcDjevngBPNEIOXzo_YS__iFlsCQVh] input_file\n");
#endif /* not REDUCED */
#endif /* not CDT_ONLY */

  printf("    -p  Triangulates a Planar Straight Line Graph (.poly file).\n");
#ifndef CDT_ONLY
  printf("    -r  Refines a previously generated mesh.\n");
  printf(
    "    -q  Quality mesh generation.  A minimum angle may be specified.\n");
  printf("    -a  Applies a maximum triangle area constraint.\n");
  printf("    -u  Applies a user-defined triangle constraint.\n");
#endif /* not CDT_ONLY */
  printf(
    "    -A  Applies attributes to identify triangles in certain regions.\n");
  printf("    -c  Encloses the convex hull with segments.\n");
#ifndef CDT_ONLY
  printf("    -D  Conforming Delaunay:  all triangles are truly Delaunay.\n");
#endif /* not CDT_ONLY */
/*
  printf("    -w  Weighted Delaunay triangulation.\n");
  printf("    -W  Regular triangulation (lower hull of a height field).\n");
*/
  printf("    -j  Jettison unused vertices from output .node file.\n");
  printf("    -e  Generates an edge list.\n");
  printf("    -v  Generates a Voronoi diagram.\n");
  printf("    -n  Generates a list of triangle neighbors.\n");
  printf("    -g  Generates an .off file for Geomview.\n");
  printf("    -B  Suppresses output of boundary information.\n");
  printf("    -P  Suppresses output of .poly file.\n");
  printf("    -N  Suppresses output of .node file.\n");
  printf("    -E  Suppresses output of .ele file.\n");
  printf("    -I  Suppresses mesh iteration numbers.\n");
  printf("    -O  Ignores holes in .poly file.\n");
  printf("    -X  Suppresses use of exact arithmetic.\n");
  printf("    -z  Numbers all items starting from zero (rather than one).\n");
  printf("    -o2 Generates second-order subparametric elements.\n");
#ifndef CDT_ONLY
  printf("    -Y  Suppresses boundary segment splitting.\n");
  printf("    -S  Specifies maximum number of added Steiner points.\n");
#endif /* not CDT_ONLY */
#ifndef REDUCED
  printf("    -i  Uses incremental method, rather than divide-and-conquer.\n");
  printf("    -F  Uses Fortune's sweepline algorithm, rather than d-and-c.\n");
#endif /* not REDUCED */
  printf("    -l  Uses vertical cuts only, rather than alternating cuts.\n");
#ifndef REDUCED
#ifndef CDT_ONLY
  printf(
    "    -s  Force segments into mesh by splitting (instead of using CDT).\n");
#endif /* not CDT_ONLY */
  printf("    -C  Check consistency of final mesh.\n");
#endif /* not REDUCED */
  printf("    -Q  Quiet:  No terminal output except errors.\n");
  printf("    -V  Verbose:  Detailed information on what I'm doing.\n");
  printf("    -h  Help:  Detailed instructions for Triangle.\n");
  triexit(0);
}

/*****************************************************************************/
/*                                                                           */
/*  info()   Print out complete instructions.                                */
/*                                                                           */
/*****************************************************************************/

void info()
{
  printf("Triangle\n");
  printf(
"A Two-Dimensional Quality Mesh Generator and Delaunay Triangulator.\n");
  printf("Version 1.6\n\n");
  printf(
"Copyright 1993, 1995, 1997, 1998, 2002, 2005 Jonathan Richard Shewchuk\n");
  printf("2360 Woolsey #H / Berkeley, California 94705-1927\n");
  printf("Bugs/comments to jrs@cs.berkeley.edu\n");
  printf(
"Created as part of the Quake project (tools for earthquake simulation).\n");
  printf(
"Supported in part by NSF Grant CMS-9318163 and an NSERC 1967 Scholarship.\n");
  printf("There is no warranty whatsoever.  Use at your own risk.\n");
#ifdef SINGLE
  printf("This executable is compiled for single precision arithmetic.\n\n\n");
#else /* not SINGLE */
  printf("This executable is compiled for double precision arithmetic.\n\n\n");
#endif /* not SINGLE */
  printf(
"Triangle generates exact Delaunay triangulations, constrained Delaunay\n");
  printf(
"triangulations, conforming Delaunay triangulations, Voronoi diagrams, and\n");
  printf(
"high-quality triangular meshes.  The latter can be generated with no small\n"
);
  printf(
"or large angles, and are thus suitable for finite element analysis.  If no\n"
);
  printf(
"command line switch is specified, your .node input file is read, and the\n");
  printf(
"Delaunay triangulation is returned in .node and .ele output files.  The\n");
  printf("command syntax is:\n\n");
  printf("triangle [-prq__a__uAcDjevngBPNEIOXzo_YS__iFlsCQVh] input_file\n\n");
  printf(
"Underscores indicate that numbers may optionally follow certain switches.\n");
  printf(
"Do not leave any space between a switch and its numeric parameter.\n");
  printf(
"input_file must be a file with extension .node, or extension .poly if the\n");
  printf(
"-p switch is used.  If -r is used, you must supply .node and .ele files,\n");
  printf(
"and possibly a .poly file and an .area file as well.  The formats of these\n"
);
  printf("files are described below.\n\n");
  printf("Command Line Switches:\n\n");
  printf(
"    -p  Reads a Planar Straight Line Graph (.poly file), which can specify\n"
);
  printf(
"        vertices, segments, holes, regional attributes, and regional area\n");
  printf(
"        constraints.  Generates a constrained Delaunay triangulation (CDT)\n"
);
  printf(
"        fitting the input; or, if -s, -q, -a, or -u is used, a conforming\n");
  printf(
"        constrained Delaunay triangulation (CCDT).  If you want a truly\n");
  printf(
"        Delaunay (not just constrained Delaunay) triangulation, use -D as\n");
  printf(
"        well.  When -p is not used, Triangle reads a .node file by default.\n"
);
  printf(
"    -r  Refines a previously generated mesh.  The mesh is read from a .node\n"
);
  printf(
"        file and an .ele file.  If -p is also used, a .poly file is read\n");
  printf(
"        and used to constrain segments in the mesh.  If -a is also used\n");
  printf(
"        (with no number following), an .area file is read and used to\n");
  printf(
"        impose area constraints on the mesh.  Further details on refinement\n"
);
  printf("        appear below.\n");
  printf(
"    -q  Quality mesh generation by Delaunay refinement (a hybrid of Paul\n");
  printf(
"        Chew's and Jim Ruppert's algorithms).  Adds vertices to the mesh to\n"
);
  printf(
"        ensure that all angles are between 20 and 140 degrees.  An\n");
  printf(
"        alternative bound on the minimum angle, replacing 20 degrees, may\n");
  printf(
"        be specified after the `q'.  The specified angle may include a\n");
  printf(
"        decimal point, but not exponential notation.  Note that a bound of\n"
);
  printf(
"        theta degrees on the smallest angle also implies a bound of\n");
  printf(
"        (180 - 2 theta) on the largest angle.  If the minimum angle is 28.6\n"
);
  printf(
"        degrees or smaller, Triangle is mathematically guaranteed to\n");
  printf(
"        terminate (assuming infinite precision arithmetic--Triangle may\n");
  printf(
"        fail to terminate if you run out of precision).  In practice,\n");
  printf(
"        Triangle often succeeds for minimum angles up to 34 degrees.  For\n");
  printf(
"        some meshes, however, you might need to reduce the minimum angle to\n"
);
  printf(
"        avoid problems associated with insufficient floating-point\n");
  printf("        precision.\n");
  printf(
"    -a  Imposes a maximum triangle area.  If a number follows the `a', no\n");
  printf(
"        triangle is generated whose area is larger than that number.  If no\n"
);
  printf(
"        number is specified, an .area file (if -r is used) or .poly file\n");
  printf(
"        (if -r is not used) specifies a set of maximum area constraints.\n");
  printf(
"        An .area file contains a separate area constraint for each\n");
  printf(
"        triangle, and is useful for refining a finite element mesh based on\n"
);
  printf(
"        a posteriori error estimates.  A .poly file can optionally contain\n"
);
  printf(
"        an area constraint for each segment-bounded region, thereby\n");
  printf(
"        controlling triangle densities in a first triangulation of a PSLG.\n"
);
  printf(
"        You can impose both a fixed area constraint and a varying area\n");
  printf(
"        constraint by invoking the -a switch twice, once with and once\n");
  printf(
"        without a number following.  Each area specified may include a\n");
  printf("        decimal point.\n");
  printf(
"    -u  Imposes a user-defined constraint on triangle size.  There are two\n"
);
  printf(
"        ways to use this feature.  One is to edit the triunsuitable()\n");
  printf(
"        procedure in triangle.c to encode any constraint you like, then\n");
  printf(
"        recompile Triangle.  The other is to compile triangle.c with the\n");
  printf(
"        EXTERNAL_TEST symbol set (compiler switch -DEXTERNAL_TEST), then\n");
  printf(
"        link Triangle with a separate object file that implements\n");
  printf(
"        triunsuitable().  In either case, the -u switch causes the user-\n");
  printf("        defined test to be applied to every triangle.\n");
  printf(
"    -A  Assigns an additional floating-point attribute to each triangle\n");
  printf(
"        that identifies what segment-bounded region each triangle be__int64s\n");
  printf(
"        to.  Attributes are assigned to regions by the .poly file.  If a\n");
  printf(
"        region is not explicitly marked by the .poly file, triangles in\n");
  printf(
"        that region are assigned an attribute of zero.  The -A switch has\n");
  printf(
"        an effect only when the -p switch is used and the -r switch is not.\n"
);
  printf(
"    -c  Creates segments on the convex hull of the triangulation.  If you\n");
  printf(
"        are triangulating a vertex set, this switch causes a .poly file to\n"
);
  printf(
"        be written, containing all edges of the convex hull.  If you are\n");
  printf(
"        triangulating a PSLG, this switch specifies that the whole convex\n");
  printf(
"        hull of the PSLG should be triangulated, regardless of what\n");
  printf(
"        segments the PSLG has.  If you do not use this switch when\n");
  printf(
"        triangulating a PSLG, Triangle assumes that you have identified the\n"
);
  printf(
"        region to be triangulated by surrounding it with segments of the\n");
  printf(
"        input PSLG.  Beware:  if you are not careful, this switch can cause\n"
);
  printf(
"        the introduction of an extremely thin angle between a PSLG segment\n"
);
  printf(
"        and a convex hull segment, which can cause overrefinement (and\n");
  printf(
"        possibly failure if Triangle runs out of precision).  If you are\n");
  printf(
"        refining a mesh, the -c switch works differently:  it causes a\n");
  printf(
"        .poly file to be written containing the boundary edges of the mesh\n"
);
  printf("        (useful if no .poly file was read).\n");
  printf(
"    -D  Conforming Delaunay triangulation:  use this switch if you want to\n"
);
  printf(
"        ensure that all the triangles in the mesh are Delaunay, and not\n");
  printf(
"        merely constrained Delaunay; or if you want to ensure that all the\n"
);
  printf(
"        Voronoi vertices lie within the triangulation.  (Some finite volume\n"
);
  printf(
"        methods have this requirement.)  This switch invokes Ruppert's\n");
  printf(
"        original algorithm, which splits every subsegment whose diametral\n");
  printf(
"        circle is encroached.  It usually increases the number of vertices\n"
);
  printf("        and triangles.\n");
  printf(
"    -j  Jettisons vertices that are not part of the final triangulation\n");
  printf(
"        from the output .node file.  By default, Triangle copies all\n");
  printf(
"        vertices in the input .node file to the output .node file, in the\n");
  printf(
"        same order, so their indices do not change.  The -j switch prevents\n"
);
  printf(
"        duplicated input vertices, or vertices `eaten' by holes, from\n");
  printf(
"        appearing in the output .node file.  Thus, if two input vertices\n");
  printf(
"        have exactly the same coordinates, only the first appears in the\n");
  printf(
"        output.  If any vertices are jettisoned, the vertex numbering in\n");
  printf(
"        the output .node file differs from that of the input .node file.\n");
  printf(
"    -e  Outputs (to an .edge file) a list of edges of the triangulation.\n");
  printf(
"    -v  Outputs the Voronoi diagram associated with the triangulation.\n");
  printf(
"        Does not attempt to detect degeneracies, so some Voronoi vertices\n");
  printf(
"        may be duplicated.  See the discussion of Voronoi diagrams below.\n");
  printf(
"    -n  Outputs (to a .neigh file) a list of triangles neighboring each\n");
  printf("        triangle.\n");
  printf(
"    -g  Outputs the mesh to an Object File Format (.off) file, suitable for\n"
);
  printf("        viewing with the Geometry Center's Geomview package.\n");
  printf(
"    -B  No boundary markers in the output .node, .poly, and .edge output\n");
  printf(
"        files.  See the detailed discussion of boundary markers below.\n");
  printf(
"    -P  No output .poly file.  Saves disk space, but you lose the ability\n");
  printf(
"        to maintain constraining segments on later refinements of the mesh.\n"
);
  printf("    -N  No output .node file.\n");
  printf("    -E  No output .ele file.\n");
  printf(
"    -I  No iteration numbers.  Suppresses the output of .node and .poly\n");
  printf(
"        files, so your input files won't be overwritten.  (If your input is\n"
);
  printf(
"        a .poly file only, a .node file is written.)  Cannot be used with\n");
  printf(
"        the -r switch, because that would overwrite your input .ele file.\n");
  printf(
"        Shouldn't be used with the -q, -a, -u, or -s switch if you are\n");
  printf(
"        using a .node file for input, because no .node file is written, so\n"
);
  printf("        there is no record of any added Steiner points.\n");
  printf("    -O  No holes.  Ignores the holes in the .poly file.\n");
  printf(
"    -X  No exact arithmetic.  Normally, Triangle uses exact floating-point\n"
);
  printf(
"        arithmetic for certain tests if it thinks the inexact tests are not\n"
);
  printf(
"        accurate enough.  Exact arithmetic ensures the robustness of the\n");
  printf(
"        triangulation algorithms, despite floating-point roundoff error.\n");
  printf(
"        Disabling exact arithmetic with the -X switch causes a small\n");
  printf(
"        improvement in speed and creates the possibility that Triangle will\n"
);
  printf("        fail to produce a valid mesh.  Not recommended.\n");
  printf(
"    -z  Numbers all items starting from zero (rather than one).  Note that\n"
);
  printf(
"        this switch is normally overridden by the value used to number the\n"
);
  printf(
"        first vertex of the input .node or .poly file.  However, this\n");
  printf(
"        switch is useful when calling Triangle from another program.\n");
  printf(
"    -o2 Generates second-order subparametric elements with six nodes each.\n"
);
  printf(
"    -Y  No new vertices on the boundary.  This switch is useful when the\n");
  printf(
"        mesh boundary must be preserved so that it conforms to some\n");
  printf(
"        adjacent mesh.  Be forewarned that you will probably sacrifice much\n"
);
  printf(
"        of the quality of the mesh; Triangle will try, but the resulting\n");
  printf(
"        mesh may contain poorly shaped triangles.  Works well if all the\n");
  printf(
"        boundary vertices are closely spaced.  Specify this switch twice\n");
  printf(
"        (`-YY') to prevent all segment splitting, including internal\n");
  printf("        boundaries.\n");
  printf(
"    -S  Specifies the maximum number of Steiner points (vertices that are\n");
  printf(
"        not in the input, but are added to meet the constraints on minimum\n"
);
  printf(
"        angle and maximum area).  The default is to allow an unlimited\n");
  printf(
"        number.  If you specify this switch with no number after it,\n");
  printf(
"        the limit is set to zero.  Triangle always adds vertices at segment\n"
);
  printf(
"        intersections, even if it needs to use more vertices than the limit\n"
);
  printf(
"        you set.  When Triangle inserts segments by splitting (-s), it\n");
  printf(
"        always adds enough vertices to ensure that all the segments of the\n"
);
  printf("        PLSG are recovered, ignoring the limit if necessary.\n");
  printf(
"    -i  Uses an incremental rather than a divide-and-conquer algorithm to\n");
  printf(
"        construct a Delaunay triangulation.  Try it if the divide-and-\n");
  printf("        conquer algorithm fails.\n");
  printf(
"    -F  Uses Steven Fortune's sweepline algorithm to construct a Delaunay\n");
  printf(
"        triangulation.  Warning:  does not use exact arithmetic for all\n");
  printf("        calculations.  An exact result is not guaranteed.\n");
  printf(
"    -l  Uses only vertical cuts in the divide-and-conquer algorithm.  By\n");
  printf(
"        default, Triangle alternates between vertical and horizontal cuts,\n"
);
  printf(
"        which usually improve the speed except with vertex sets that are\n");
  printf(
"        small or short and wide.  This switch is primarily of theoretical\n");
  printf("        interest.\n");
  printf(
"    -s  Specifies that segments should be forced into the triangulation by\n"
);
  printf(
"        recursively splitting them at their midpoints, rather than by\n");
  printf(
"        generating a constrained Delaunay triangulation.  Segment splitting\n"
);
  printf(
"        is true to Ruppert's original algorithm, but can create needlessly\n"
);
  printf(
"        small triangles.  This switch is primarily of theoretical interest.\n"
);
  printf(
"    -C  Check the consistency of the final mesh.  Uses exact arithmetic for\n"
);
  printf(
"        checking, even if the -X switch is used.  Useful if you suspect\n");
  printf("        Triangle is buggy.\n");
  printf(
"    -Q  Quiet:  Suppresses all explanation of what Triangle is doing,\n");
  printf("        unless an error occurs.\n");
  printf(
"    -V  Verbose:  Gives detailed information about what Triangle is doing.\n"
);
  printf(
"        Add more `V's for increasing amount of detail.  `-V' is most\n");
  printf(
"        useful; itgives information on algorithmic progress and much more\n");
  printf(
"        detailed statistics.  `-VV' gives vertex-by-vertex details, and\n");
  printf(
"        prints so much that Triangle runs much more slowly.  `-VVVV' gives\n"
);
  printf("        information only a debugger could love.\n");
  printf("    -h  Help:  Displays these instructions.\n");
  printf("\n");
  printf("Definitions:\n");
  printf("\n");
  printf(
"  A Delaunay triangulation of a vertex set is a triangulation whose\n");
  printf(
"  vertices are the vertex set, that covers the convex hull of the vertex\n");
  printf(
"  set.  A Delaunay triangulation has the property that no vertex lies\n");
  printf(
"  inside the circumscribing circle (circle that passes through all three\n");
  printf("  vertices) of any triangle in the triangulation.\n\n");
  printf(
"  A Voronoi diagram of a vertex set is a subdivision of the plane into\n");
  printf(
"  polygonal cells (some of which may be unbounded, meaning infinitely\n");
  printf(
"  large), where each cell is the set of points in the plane that are closer\n"
);
  printf(
"  to some input vertex than to any other input vertex.  The Voronoi diagram\n"
);
  printf("  is a geometric dual of the Delaunay triangulation.\n\n");
  printf(
"  A Planar Straight Line Graph (PSLG) is a set of vertices and segments.\n");
  printf(
"  Segments are simply edges, whose endpoints are all vertices in the PSLG.\n"
);
  printf(
"  Segments may intersect each other only at their endpoints.  The file\n");
  printf("  format for PSLGs (.poly files) is described below.\n\n");
  printf(
"  A constrained Delaunay triangulation (CDT) of a PSLG is similar to a\n");
  printf(
"  Delaunay triangulation, but each PSLG segment is present as a single edge\n"
);
  printf(
"  of the CDT.  (A constrained Delaunay triangulation is not truly a\n");
  printf(
"  Delaunay triangulation, because some of its triangles might not be\n");
  printf(
"  Delaunay.)  By definition, a CDT does not have any vertices other than\n");
  printf(
"  those specified in the input PSLG.  Depending on context, a CDT might\n");
  printf(
"  cover the convex hull of the PSLG, or it might cover only a segment-\n");
  printf("  bounded region (e.g. a polygon).\n\n");
  printf(
"  A conforming Delaunay triangulation of a PSLG is a triangulation in which\n"
);
  printf(
"  each triangle is truly Delaunay, and each PSLG segment is represented by\n"
);
  printf(
"  a linear contiguous sequence of edges of the triangulation.  New vertices\n"
);
  printf(
"  (not part of the PSLG) may appear, and each input segment may have been\n");
  printf(
"  subdivided into shorter edges (subsegments) by these additional vertices.\n"
);
  printf(
"  The new vertices are frequently necessary to maintain the Delaunay\n");
  printf("  property while ensuring that every segment is represented.\n\n");
  printf(
"  A conforming constrained Delaunay triangulation (CCDT) of a PSLG is a\n");
  printf(
"  triangulation of a PSLG whose triangles are constrained Delaunay.  New\n");
  printf("  vertices may appear, and input segments may be subdivided into\n");
  printf(
"  subsegments, but not to guarantee that segments are respected; rather, to\n"
);
  printf(
"  improve the quality of the triangles.  The high-quality meshes produced\n");
  printf(
"  by the -q switch are usually CCDTs, but can be made conforming Delaunay\n");
  printf("  with the -D switch.\n\n");
  printf("File Formats:\n\n");
  printf(
"  All files may contain comments prefixed by the character '#'.  Vertices,\n"
);
  printf(
"  triangles, edges, holes, and maximum area constraints must be numbered\n");
  printf(
"  consecutively, starting from either 1 or 0.  Whichever you choose, all\n");
  printf(
"  input files must be consistent; if the vertices are numbered from 1, so\n");
  printf(
"  must be all other objects.  Triangle automatically detects your choice\n");
  printf(
"  while reading the .node (or .poly) file.  (When calling Triangle from\n");
  printf(
"  another program, use the -z switch if you wish to number objects from\n");
  printf("  zero.)  Examples of these file formats are given below.\n\n");
  printf("  .node files:\n");
  printf(
"    First line:  <# of vertices> <dimension (must be 2)> <# of attributes>\n"
);
  printf(
"                                           <# of boundary markers (0 or 1)>\n"
);
  printf(
"    Remaining lines:  <vertex #> <x> <y> [attributes] [boundary marker]\n");
  printf("\n");
  printf(
"    The attributes, which are typically floating-point values of physical\n");
  printf(
"    quantities (such as mass or conductivity) associated with the nodes of\n"
);
  printf(
"    a finite element mesh, are copied unchanged to the output mesh.  If -q,\n"
);
  printf(
"    -a, -u, -D, or -s is selected, each new Steiner point added to the mesh\n"
);
  printf("    has attributes assigned to it by linear interpolation.\n\n");
  printf(
"    If the fourth entry of the first line is `1', the last column of the\n");
  printf(
"    remainder of the file is assumed to contain boundary markers.  Boundary\n"
);
  printf(
"    markers are used to identify boundary vertices and vertices resting on\n"
);
  printf(
"    PSLG segments; a complete description appears in a section below.  The\n"
);
  printf(
"    .node file produced by Triangle contains boundary markers in the last\n");
  printf("    column unless they are suppressed by the -B switch.\n\n");
  printf("  .ele files:\n");
  printf(
"    First line:  <# of triangles> <nodes per triangle> <# of attributes>\n");
  printf(
"    Remaining lines:  <triangle #> <node> <node> <node> ... [attributes]\n");
  printf("\n");
  printf(
"    Nodes are indices into the corresponding .node file.  The first three\n");
  printf(
"    nodes are the corner vertices, and are listed in counterclockwise order\n"
);
  printf(
"    around each triangle.  (The remaining nodes, if any, depend on the type\n"
);
  printf("    of finite element used.)\n\n");
  printf(
"    The attributes are just like those of .node files.  Because there is no\n"
);
  printf(
"    simple mapping from input to output triangles, Triangle attempts to\n");
  printf(
"    interpolate attributes, and may cause a lot of diffusion of attributes\n"
);
  printf(
"    among nearby triangles as the triangulation is refined.  Attributes do\n"
);
  printf("    not diffuse across segments, so attributes used to identify\n");
  printf("    segment-bounded regions remain intact.\n\n");
  printf(
"    In .ele files produced by Triangle, each triangular element has three\n");
  printf(
"    nodes (vertices) unless the -o2 switch is used, in which case\n");
  printf(
"    subparametric quadratic elements with six nodes each are generated.\n");
  printf(
"    The first three nodes are the corners in counterclockwise order, and\n");
  printf(
"    the fourth, fifth, and sixth nodes lie on the midpoints of the edges\n");
  printf(
"    opposite the first, second, and third vertices, respectively.\n");
  printf("\n");
  printf("  .poly files:\n");
  printf(
"    First line:  <# of vertices> <dimension (must be 2)> <# of attributes>\n"
);
  printf(
"                                           <# of boundary markers (0 or 1)>\n"
);
  printf(
"    Following lines:  <vertex #> <x> <y> [attributes] [boundary marker]\n");
  printf("    One line:  <# of segments> <# of boundary markers (0 or 1)>\n");
  printf(
"    Following lines:  <segment #> <endpoint> <endpoint> [boundary marker]\n");
  printf("    One line:  <# of holes>\n");
  printf("    Following lines:  <hole #> <x> <y>\n");
  printf(
"    Optional line:  <# of regional attributes and/or area constraints>\n");
  printf(
"    Optional following lines:  <region #> <x> <y> <attribute> <max area>\n");
  printf("\n");
  printf(
"    A .poly file represents a PSLG, as well as some additional information.\n"
);
  printf(
"    The first section lists all the vertices, and is identical to the\n");
  printf(
"    format of .node files.  <# of vertices> may be set to zero to indicate\n"
);
  printf(
"    that the vertices are listed in a separate .node file; .poly files\n");
  printf(
"    produced by Triangle always have this format.  A vertex set represented\n"
);
  printf(
"    this way has the advantage that it may easily be triangulated with or\n");
  printf(
"    without segments (depending on whether the -p switch is invoked).\n");
  printf("\n");
  printf(
"    The second section lists the segments.  Segments are edges whose\n");
  printf(
"    presence in the triangulation is enforced.  (Depending on the choice of\n"
);
  printf(
"    switches, segment might be subdivided into smaller edges).  Each\n");
  printf(
"    segment is specified by listing the indices of its two endpoints.  This\n"
);
  printf(
"    means that you must include its endpoints in the vertex list.  Each\n");
  printf("    segment, like each point, may have a boundary marker.\n\n");
  printf(
"    If -q, -a, -u, and -s are not selected, Triangle produces a constrained\n"
);
  printf(
"    Delaunay triangulation (CDT), in which each segment appears as a single\n"
);
  printf(
"    edge in the triangulation.  If -q, -a, -u, or -s is selected, Triangle\n"
);
  printf(
"    produces a conforming constrained Delaunay triangulation (CCDT), in\n");
  printf(
"    which segments may be subdivided into smaller edges.  If -D is\n");
  printf(
"    selected, Triangle produces a conforming Delaunay triangulation, so\n");
  printf(
"    that every triangle is Delaunay, and not just constrained Delaunay.\n");
  printf("\n");
  printf(
"    The third section lists holes (and concavities, if -c is selected) in\n");
  printf(
"    the triangulation.  Holes are specified by identifying a point inside\n");
  printf(
"    each hole.  After the triangulation is formed, Triangle creates holes\n");
  printf(
"    by eating triangles, spreading out from each hole point until its\n");
  printf(
"    progress is blocked by segments in the PSLG.  You must be careful to\n");
  printf(
"    enclose each hole in segments, or your whole triangulation might be\n");
  printf(
"    eaten away.  If the two triangles abutting a segment are eaten, the\n");
  printf(
"    segment itself is also eaten.  Do not place a hole directly on a\n");
  printf("    segment; if you do, Triangle chooses one side of the segment\n");
  printf("    arbitrarily.\n\n");
  printf(
"    The optional fourth section lists regional attributes (to be assigned\n");
  printf(
"    to all triangles in a region) and regional constraints on the maximum\n");
  printf(
"    triangle area.  Triangle reads this section only if the -A switch is\n");
  printf(
"    used or the -a switch is used without a number following it, and the -r\n"
);
  printf(
"    switch is not used.  Regional attributes and area constraints are\n");
  printf(
"    propagated in the same manner as holes:  you specify a point for each\n");
  printf(
"    attribute and/or constraint, and the attribute and/or constraint\n");
  printf(
"    affects the whole region (bounded by segments) containing the point.\n");
  printf(
"    If two values are written on a line after the x and y coordinate, the\n");
  printf(
"    first such value is assumed to be a regional attribute (but is only\n");
  printf(
"    applied if the -A switch is selected), and the second value is assumed\n"
);
  printf(
"    to be a regional area constraint (but is only applied if the -a switch\n"
);
  printf(
"    is selected).  You may specify just one value after the coordinates,\n");
  printf(
"    which can serve as both an attribute and an area constraint, depending\n"
);
  printf(
"    on the choice of switches.  If you are using the -A and -a switches\n");
  printf(
"    simultaneously and wish to assign an attribute to some region without\n");
  printf("    imposing an area constraint, use a negative maximum area.\n\n");
  printf(
"    When a triangulation is created from a .poly file, you must either\n");
  printf(
"    enclose the entire region to be triangulated in PSLG segments, or\n");
  printf(
"    use the -c switch, which automatically creates extra segments that\n");
  printf(
"    enclose the convex hull of the PSLG.  If you do not use the -c switch,\n"
);
  printf(
"    Triangle eats all triangles that are not enclosed by segments; if you\n");
  printf(
"    are not careful, your whole triangulation may be eaten away.  If you do\n"
);
  printf(
"    use the -c switch, you can still produce concavities by the appropriate\n"
);
  printf(
"    placement of holes just inside the boundary of the convex hull.\n");
  printf("\n");
  printf(
"    An ideal PSLG has no intersecting segments, nor any vertices that lie\n");
  printf(
"    upon segments (except, of course, the endpoints of each segment).  You\n"
);
  printf(
"    aren't required to make your .poly files ideal, but you should be aware\n"
);
  printf(
"    of what can go wrong.  Segment intersections are relatively safe--\n");
  printf(
"    Triangle calculates the intersection points for you and adds them to\n");
  printf(
"    the triangulation--as __int64 as your machine's floating-point precision\n");
  printf(
"    doesn't become a problem.  You are tempting the fates if you have three\n"
);
  printf(
"    segments that cross at the same location, and expect Triangle to figure\n"
);
  printf(
"    out where the intersection point is.  Thanks to floating-point roundoff\n"
);
  printf(
"    error, Triangle will probably decide that the three segments intersect\n"
);
  printf(
"    at three different points, and you will find a minuscule triangle in\n");
  printf(
"    your output--unless Triangle tries to refine the tiny triangle, uses\n");
  printf(
"    up the last bit of machine precision, and fails to terminate at all.\n");
  printf(
"    You're better off putting the intersection point in the input files,\n");
  printf(
"    and manually breaking up each segment into two.  Similarly, if you\n");
  printf(
"    place a vertex at the middle of a segment, and hope that Triangle will\n"
);
  printf(
"    break up the segment at that vertex, you might get lucky.  On the other\n"
);
  printf(
"    hand, Triangle might decide that the vertex doesn't lie precisely on\n");
  printf(
"    the segment, and you'll have a needle-sharp triangle in your output--or\n"
);
  printf("    a lot of tiny triangles if you're generating a quality mesh.\n");
  printf("\n");
  printf(
"    When Triangle reads a .poly file, it also writes a .poly file, which\n");
  printf(
"    includes all the subsegments--the edges that are parts of input\n");
  printf(
"    segments.  If the -c switch is used, the output .poly file also\n");
  printf(
"    includes all of the edges on the convex hull.  Hence, the output .poly\n"
);
  printf(
"    file is useful for finding edges associated with input segments and for\n"
);
  printf(
"    setting boundary conditions in finite element simulations.  Moreover,\n");
  printf(
"    you will need the output .poly file if you plan to refine the output\n");
  printf(
"    mesh, and don't want segments to be missing in later triangulations.\n");
  printf("\n");
  printf("  .area files:\n");
  printf("    First line:  <# of triangles>\n");
  printf("    Following lines:  <triangle #> <maximum area>\n");
  printf("\n");
  printf(
"    An .area file associates with each triangle a maximum area that is used\n"
);
  printf(
"    for mesh refinement.  As with other file formats, every triangle must\n");
  printf(
"    be represented, and the triangles must be numbered consecutively.  A\n");
  printf(
"    triangle may be left unconstrained by assigning it a negative maximum\n");
  printf("    area.\n\n");
  printf("  .edge files:\n");
  printf("    First line:  <# of edges> <# of boundary markers (0 or 1)>\n");
  printf(
"    Following lines:  <edge #> <endpoint> <endpoint> [boundary marker]\n");
  printf("\n");
  printf(
"    Endpoints are indices into the corresponding .node file.  Triangle can\n"
);
  printf(
"    produce .edge files (use the -e switch), but cannot read them.  The\n");
  printf(
"    optional column of boundary markers is suppressed by the -B switch.\n");
  printf("\n");
  printf(
"    In Voronoi diagrams, one also finds a special kind of edge that is an\n");
  printf(
"    infinite ray with only one endpoint.  For these edges, a different\n");
  printf("    format is used:\n\n");
  printf("        <edge #> <endpoint> -1 <direction x> <direction y>\n\n");
  printf(
"    The `direction' is a floating-point vector that indicates the direction\n"
);
  printf("    of the infinite ray.\n\n");
  printf("  .neigh files:\n");
  printf(
"    First line:  <# of triangles> <# of neighbors per triangle (always 3)>\n"
);
  printf(
"    Following lines:  <triangle #> <neighbor> <neighbor> <neighbor>\n");
  printf("\n");
  printf(
"    Neighbors are indices into the corresponding .ele file.  An index of -1\n"
);
  printf(
"    indicates no neighbor (because the triangle is on an exterior\n");
  printf(
"    boundary).  The first neighbor of triangle i is opposite the first\n");
  printf("    corner of triangle i, and so on.\n\n");
  printf(
"    Triangle can produce .neigh files (use the -n switch), but cannot read\n"
);
  printf("    them.\n\n");
  printf("Boundary Markers:\n\n");
  printf(
"  Boundary markers are tags used mainly to identify which output vertices\n");
  printf(
"  and edges are associated with which PSLG segment, and to identify which\n");
  printf(
"  vertices and edges occur on a boundary of the triangulation.  A common\n");
  printf(
"  use is to determine where boundary conditions should be applied to a\n");
  printf(
"  finite element mesh.  You can prevent boundary markers from being written\n"
);
  printf("  into files produced by Triangle by using the -B switch.\n\n");
  printf(
"  The boundary marker associated with each segment in an output .poly file\n"
);
  printf("  and each edge in an output .edge file is chosen as follows:\n");
  printf(
"    - If an output edge is part or all of a PSLG segment with a nonzero\n");
  printf(
"      boundary marker, then the edge is assigned the same marker.\n");
  printf(
"    - Otherwise, if the edge lies on a boundary of the triangulation\n");
  printf(
"      (even the boundary of a hole), then the edge is assigned the marker\n");
  printf("      one (1).\n");
  printf("    - Otherwise, the edge is assigned the marker zero (0).\n");
  printf(
"  The boundary marker associated with each vertex in an output .node file\n");
  printf("  is chosen as follows:\n");
  printf(
"    - If a vertex is assigned a nonzero boundary marker in the input file,\n"
);
  printf(
"      then it is assigned the same marker in the output .node file.\n");
  printf(
"    - Otherwise, if the vertex lies on a PSLG segment (even if it is an\n");
  printf(
"      endpoint of the segment) with a nonzero boundary marker, then the\n");
  printf(
"      vertex is assigned the same marker.  If the vertex lies on several\n");
  printf("      such segments, one of the markers is chosen arbitrarily.\n");
  printf(
"    - Otherwise, if the vertex occurs on a boundary of the triangulation,\n");
  printf("      then the vertex is assigned the marker one (1).\n");
  printf("    - Otherwise, the vertex is assigned the marker zero (0).\n");
  printf("\n");
  printf(
"  If you want Triangle to determine for you which vertices and edges are on\n"
);
  printf(
"  the boundary, assign them the boundary marker zero (or use no markers at\n"
);
  printf(
"  all) in your input files.  In the output files, all boundary vertices,\n");
  printf("  edges, and segments will be assigned the value one.\n\n");
  printf("Triangulation Iteration Numbers:\n\n");
  printf(
"  Because Triangle can read and refine its own triangulations, input\n");
  printf(
"  and output files have iteration numbers.  For instance, Triangle might\n");
  printf(
"  read the files mesh.3.node, mesh.3.ele, and mesh.3.poly, refine the\n");
  printf(
"  triangulation, and output the files mesh.4.node, mesh.4.ele, and\n");
  printf("  mesh.4.poly.  Files with no iteration number are treated as if\n");
  printf(
"  their iteration number is zero; hence, Triangle might read the file\n");
  printf(
"  points.node, triangulate it, and produce the files points.1.node and\n");
  printf("  points.1.ele.\n\n");
  printf(
"  Iteration numbers allow you to create a sequence of successively finer\n");
  printf(
"  meshes suitable for multigrid methods.  They also allow you to produce a\n"
);
  printf(
"  sequence of meshes using error estimate-driven mesh refinement.\n");
  printf("\n");
  printf(
"  If you're not using refinement or quality meshing, and you don't like\n");
  printf(
"  iteration numbers, use the -I switch to disable them.  This switch also\n");
  printf(
"  disables output of .node and .poly files to prevent your input files from\n"
);
  printf(
"  being overwritten.  (If the input is a .poly file that contains its own\n");
  printf(
"  points, a .node file is written.  This can be quite convenient for\n");
  printf("  computing CDTs or quality meshes.)\n\n");
  printf("Examples of How to Use Triangle:\n\n");
  printf(
"  `triangle dots' reads vertices from dots.node, and writes their Delaunay\n"
);
  printf(
"  triangulation to dots.1.node and dots.1.ele.  (dots.1.node is identical\n");
  printf(
"  to dots.node.)  `triangle -I dots' writes the triangulation to dots.ele\n");
  printf(
"  instead.  (No additional .node file is needed, so none is written.)\n");
  printf("\n");
  printf(
"  `triangle -pe object.1' reads a PSLG from object.1.poly (and possibly\n");
  printf(
"  object.1.node, if the vertices are omitted from object.1.poly) and writes\n"
);
  printf(
"  its constrained Delaunay triangulation to object.2.node and object.2.ele.\n"
);
  printf(
"  The segments are copied to object.2.poly, and all edges are written to\n");
  printf("  object.2.edge.\n\n");
  printf(
"  `triangle -pq31.5a.1 object' reads a PSLG from object.poly (and possibly\n"
);
  printf(
"  object.node), generates a mesh whose angles are all between 31.5 and 117\n"
);
  printf(
"  degrees and whose triangles all have areas of 0.1 or less, and writes the\n"
);
  printf(
"  mesh to object.1.node and object.1.ele.  Each segment may be broken up\n");
  printf("  into multiple subsegments; these are written to object.1.poly.\n");
  printf("\n");
  printf(
"  Here is a sample file `box.poly' describing a square with a square hole:\n"
);
  printf("\n");
  printf(
"    # A box with eight vertices in 2D, no attributes, one boundary marker.\n"
);
  printf("    8 2 0 1\n");
  printf("     # Outer box has these vertices:\n");
  printf("     1   0 0   0\n");
  printf("     2   0 3   0\n");
  printf("     3   3 0   0\n");
  printf("     4   3 3   33     # A special marker for this vertex.\n");
  printf("     # Inner square has these vertices:\n");
  printf("     5   1 1   0\n");
  printf("     6   1 2   0\n");
  printf("     7   2 1   0\n");
  printf("     8   2 2   0\n");
  printf("    # Five segments with boundary markers.\n");
  printf("    5 1\n");
  printf("     1   1 2   5      # Left side of outer box.\n");
  printf("     # Square hole has these segments:\n");
  printf("     2   5 7   0\n");
  printf("     3   7 8   0\n");
  printf("     4   8 6   10\n");
  printf("     5   6 5   0\n");
  printf("    # One hole in the middle of the inner square.\n");
  printf("    1\n");
  printf("     1   1.5 1.5\n");
  printf("\n");
  printf(
"  Note that some segments are missing from the outer square, so you must\n");
  printf(
"  use the `-c' switch.  After `triangle -pqc box.poly', here is the output\n"
);
  printf(
"  file `box.1.node', with twelve vertices.  The last four vertices were\n");
  printf(
"  added to meet the angle constraint.  Vertices 1, 2, and 9 have markers\n");
  printf(
"  from segment 1.  Vertices 6 and 8 have markers from segment 4.  All the\n");
  printf(
"  other vertices but 4 have been marked to indicate that they lie on a\n");
  printf("  boundary.\n\n");
  printf("    12  2  0  1\n");
  printf("       1    0   0      5\n");
  printf("       2    0   3      5\n");
  printf("       3    3   0      1\n");
  printf("       4    3   3     33\n");
  printf("       5    1   1      1\n");
  printf("       6    1   2     10\n");
  printf("       7    2   1      1\n");
  printf("       8    2   2     10\n");
  printf("       9    0   1.5    5\n");
  printf("      10    1.5   0    1\n");
  printf("      11    3   1.5    1\n");
  printf("      12    1.5   3    1\n");
  printf("    # Generated by triangle -pqc box.poly\n");
  printf("\n");
  printf("  Here is the output file `box.1.ele', with twelve triangles.\n");
  printf("\n");
  printf("    12  3  0\n");
  printf("       1     5   6   9\n");
  printf("       2    10   3   7\n");
  printf("       3     6   8  12\n");
  printf("       4     9   1   5\n");
  printf("       5     6   2   9\n");
  printf("       6     7   3  11\n");
  printf("       7    11   4   8\n");
  printf("       8     7   5  10\n");
  printf("       9    12   2   6\n");
  printf("      10     8   7  11\n");
  printf("      11     5   1  10\n");
  printf("      12     8   4  12\n");
  printf("    # Generated by triangle -pqc box.poly\n\n");
  printf(
"  Here is the output file `box.1.poly'.  Note that segments have been added\n"
);
  printf(
"  to represent the convex hull, and some segments have been subdivided by\n");
  printf(
"  newly added vertices.  Note also that <# of vertices> is set to zero to\n");
  printf("  indicate that the vertices should be read from the .node file.\n");
  printf("\n");
  printf("    0  2  0  1\n");
  printf("    12  1\n");
  printf("       1     1   9     5\n");
  printf("       2     5   7     1\n");
  printf("       3     8   7     1\n");
  printf("       4     6   8    10\n");
  printf("       5     5   6     1\n");
  printf("       6     3  10     1\n");
  printf("       7     4  11     1\n");
  printf("       8     2  12     1\n");
  printf("       9     9   2     5\n");
  printf("      10    10   1     1\n");
  printf("      11    11   3     1\n");
  printf("      12    12   4     1\n");
  printf("    1\n");
  printf("       1   1.5 1.5\n");
  printf("    # Generated by triangle -pqc box.poly\n");
  printf("\n");
  printf("Refinement and Area Constraints:\n");
  printf("\n");
  printf(
"  The -r switch causes a mesh (.node and .ele files) to be read and\n");
  printf(
"  refined.  If the -p switch is also used, a .poly file is read and used to\n"
);
  printf(
"  specify edges that are constrained and cannot be eliminated (although\n");
  printf(
"  they can be subdivided into smaller edges) by the refinement process.\n");
  printf("\n");
  printf(
"  When you refine a mesh, you generally want to impose tighter constraints.\n"
);
  printf(
"  One way to accomplish this is to use -q with a larger angle, or -a\n");
  printf(
"  followed by a smaller area than you used to generate the mesh you are\n");
  printf(
"  refining.  Another way to do this is to create an .area file, which\n");
  printf(
"  specifies a maximum area for each triangle, and use the -a switch\n");
  printf(
"  (without a number following).  Each triangle's area constraint is applied\n"
);
  printf(
"  to that triangle.  Area constraints tend to diffuse as the mesh is\n");
  printf(
"  refined, so if there are large variations in area constraint between\n");
  printf(
"  adjacent triangles, you may not get the results you want.  In that case,\n"
);
  printf(
"  consider instead using the -u switch and writing a C procedure that\n");
  printf("  determines which triangles are too large.\n\n");
  printf(
"  If you are refining a mesh composed of linear (three-node) elements, the\n"
);
  printf(
"  output mesh contains all the nodes present in the input mesh, in the same\n"
);
  printf(
"  order, with new nodes added at the end of the .node file.  However, the\n");
  printf(
"  refinement is not hierarchical: there is no guarantee that each output\n");
  printf(
"  element is contained in a single input element.  Often, an output element\n"
);
  printf(
"  can overlap two or three input elements, and some input edges are not\n");
  printf(
"  present in the output mesh.  Hence, a sequence of refined meshes forms a\n"
);
  printf(
"  hierarchy of nodes, but not a hierarchy of elements.  If you refine a\n");
  printf(
"  mesh of higher-order elements, the hierarchical property applies only to\n"
);
  printf(
"  the nodes at the corners of an element; the midpoint nodes on each edge\n");
  printf("  are discarded before the mesh is refined.\n\n");
  printf(
"  Maximum area constraints in .poly files operate differently from those in\n"
);
  printf(
"  .area files.  A maximum area in a .poly file applies to the whole\n");
  printf(
"  (segment-bounded) region in which a point falls, whereas a maximum area\n");
  printf(
"  in an .area file applies to only one triangle.  Area constraints in .poly\n"
);
  printf(
"  files are used only when a mesh is first generated, whereas area\n");
  printf(
"  constraints in .area files are used only to refine an existing mesh, and\n"
);
  printf(
"  are typically based on a posteriori error estimates resulting from a\n");
  printf("  finite element simulation on that mesh.\n\n");
  printf(
"  `triangle -rq25 object.1' reads object.1.node and object.1.ele, then\n");
  printf(
"  refines the triangulation to enforce a 25 degree minimum angle, and then\n"
);
  printf(
"  writes the refined triangulation to object.2.node and object.2.ele.\n");
  printf("\n");
  printf(
"  `triangle -rpaa6.2 z.3' reads z.3.node, z.3.ele, z.3.poly, and z.3.area.\n"
);
  printf(
"  After reconstructing the mesh and its subsegments, Triangle refines the\n");
  printf(
"  mesh so that no triangle has area greater than 6.2, and furthermore the\n");
  printf(
"  triangles satisfy the maximum area constraints in z.3.area.  No angle\n");
  printf(
"  bound is imposed at all.  The output is written to z.4.node, z.4.ele, and\n"
);
  printf("  z.4.poly.\n\n");
  printf(
"  The sequence `triangle -qa1 x', `triangle -rqa.3 x.1', `triangle -rqa.1\n");
  printf(
"  x.2' creates a sequence of successively finer meshes x.1, x.2, and x.3,\n");
  printf("  suitable for multigrid.\n\n");
  printf("Convex Hulls and Mesh Boundaries:\n\n");
  printf(
"  If the input is a vertex set (not a PSLG), Triangle produces its convex\n");
  printf(
"  hull as a by-product in the output .poly file if you use the -c switch.\n");
  printf(
"  There are faster algorithms for finding a two-dimensional convex hull\n");
  printf("  than triangulation, of course, but this one comes for free.\n\n");
  printf(
"  If the input is an unconstrained mesh (you are using the -r switch but\n");
  printf(
"  not the -p switch), Triangle produces a list of its boundary edges\n");
  printf(
"  (including hole boundaries) as a by-product when you use the -c switch.\n");
  printf(
"  If you also use the -p switch, the output .poly file contains all the\n");
  printf("  segments from the input .poly file as well.\n\n");
  printf("Voronoi Diagrams:\n\n");
  printf(
"  The -v switch produces a Voronoi diagram, in files suffixed .v.node and\n");
  printf(
"  .v.edge.  For example, `triangle -v points' reads points.node, produces\n");
  printf(
"  its Delaunay triangulation in points.1.node and points.1.ele, and\n");
  printf(
"  produces its Voronoi diagram in points.1.v.node and points.1.v.edge.  The\n"
);
  printf(
"  .v.node file contains a list of all Voronoi vertices, and the .v.edge\n");
  printf(
"  file contains a list of all Voronoi edges, some of which may be infinite\n"
);
  printf(
"  rays.  (The choice of filenames makes it easy to run the set of Voronoi\n");
  printf("  vertices through Triangle, if so desired.)\n\n");
  printf(
"  This implementation does not use exact arithmetic to compute the Voronoi\n"
);
  printf(
"  vertices, and does not check whether neighboring vertices are identical.\n"
);
  printf(
"  Be forewarned that if the Delaunay triangulation is degenerate or\n");
  printf(
"  near-degenerate, the Voronoi diagram may have duplicate vertices or\n");
  printf("  crossing edges.\n\n");
  printf(
"  The result is a valid Voronoi diagram only if Triangle's output is a true\n"
);
  printf(
"  Delaunay triangulation.  The Voronoi output is usually meaningless (and\n");
  printf(
"  may contain crossing edges and other pathology) if the output is a CDT or\n"
);
  printf(
"  CCDT, or if it has holes or concavities.  If the triangulated domain is\n");
  printf(
"  convex and has no holes, you can use -D switch to force Triangle to\n");
  printf(
"  construct a conforming Delaunay triangulation instead of a CCDT, so the\n");
  printf("  Voronoi diagram will be valid.\n\n");
  printf("Mesh Topology:\n\n");
  printf(
"  You may wish to know which triangles are adjacent to a certain Delaunay\n");
  printf(
"  edge in an .edge file, which Voronoi cells are adjacent to a certain\n");
  printf(
"  Voronoi edge in a .v.edge file, or which Voronoi cells are adjacent to\n");
  printf(
"  each other.  All of this information can be found by cross-referencing\n");
  printf(
"  output files with the recollection that the Delaunay triangulation and\n");
  printf("  the Voronoi diagram are planar duals.\n\n");
  printf(
"  Specifically, edge i of an .edge file is the dual of Voronoi edge i of\n");
  printf(
"  the corresponding .v.edge file, and is rotated 90 degrees counterclock-\n");
  printf(
"  wise from the Voronoi edge.  Triangle j of an .ele file is the dual of\n");
  printf(
"  vertex j of the corresponding .v.node file.  Voronoi cell k is the dual\n");
  printf("  of vertex k of the corresponding .node file.\n\n");
  printf(
"  Hence, to find the triangles adjacent to a Delaunay edge, look at the\n");
  printf(
"  vertices of the corresponding Voronoi edge.  If the endpoints of a\n");
  printf(
"  Voronoi edge are Voronoi vertices 2 and 6 respectively, then triangles 2\n"
);
  printf(
"  and 6 adjoin the left and right sides of the corresponding Delaunay edge,\n"
);
  printf(
"  respectively.  To find the Voronoi cells adjacent to a Voronoi edge, look\n"
);
  printf(
"  at the endpoints of the corresponding Delaunay edge.  If the endpoints of\n"
);
  printf(
"  a Delaunay edge are input vertices 7 and 12, then Voronoi cells 7 and 12\n"
);
  printf(
"  adjoin the right and left sides of the corresponding Voronoi edge,\n");
  printf(
"  respectively.  To find which Voronoi cells are adjacent to each other,\n");
  printf("  just read the list of Delaunay edges.\n\n");
  printf(
"  Triangle does not write a list of the edges adjoining each Voronoi cell,\n"
);
  printf(
"  but you can reconstructed it straightforwardly.  For instance, to find\n");
  printf(
"  all the edges of Voronoi cell 1, search the output .edge file for every\n");
  printf(
"  edge that has input vertex 1 as an endpoint.  The corresponding dual\n");
  printf(
"  edges in the output .v.edge file form the boundary of Voronoi cell 1.\n");
  printf("\n");
  printf(
"  For each Voronoi vertex, the .neigh file gives a list of the three\n");
  printf(
"  Voronoi vertices attached to it.  You might find this more convenient\n");
  printf("  than the .v.edge file.\n\n");
  printf("Quadratic Elements:\n\n");
  printf(
"  Triangle generates meshes with subparametric quadratic elements if the\n");
  printf(
"  -o2 switch is specified.  Quadratic elements have six nodes per element,\n"
);
  printf(
"  rather than three.  `Subparametric' means that the edges of the triangles\n"
);
  printf(
"  are always straight, so that subparametric quadratic elements are\n");
  printf(
"  geometrically identical to linear elements, even though they can be used\n"
);
  printf(
"  with quadratic interpolating functions.  The three extra nodes of an\n");
  printf(
"  element fall at the midpoints of the three edges, with the fourth, fifth,\n"
);
  printf(
"  and sixth nodes appearing opposite the first, second, and third corners\n");
  printf("  respectively.\n\n");
  printf("Domains with Small Angles:\n\n");
  printf(
"  If two input segments adjoin each other at a small angle, clearly the -q\n"
);
  printf(
"  switch cannot remove the small angle.  Moreover, Triangle may have no\n");
  printf(
"  choice but to generate additional triangles whose smallest angles are\n");
  printf(
"  smaller than the specified bound.  However, these triangles only appear\n");
  printf(
"  between input segments separated by small angles.  Moreover, if you\n");
  printf(
"  request a minimum angle of theta degrees, Triangle will generally produce\n"
);
  printf(
"  no angle larger than 180 - 2 theta, even if it is forced to compromise on\n"
);
  printf("  the minimum angle.\n\n");
  printf("Statistics:\n\n");
  printf(
"  After generating a mesh, Triangle prints a count of entities in the\n");
  printf(
"  output mesh, including the number of vertices, triangles, edges, exterior\n"
);
  printf(
"  boundary edges (i.e. subsegments on the boundary of the triangulation,\n");
  printf(
"  including hole boundaries), interior boundary edges (i.e. subsegments of\n"
);
  printf(
"  input segments not on the boundary), and total subsegments.  If you've\n");
  printf(
"  forgotten the statistics for an existing mesh, run Triangle on that mesh\n"
);
  printf(
"  with the -rNEP switches to read the mesh and print the statistics without\n"
);
  printf(
"  writing any files.  Use -rpNEP if you've got a .poly file for the mesh.\n");
  printf("\n");
  printf(
"  The -V switch produces extended statistics, including a rough estimate\n");
  printf(
"  of memory use, the number of calls to geometric predicates, and\n");
  printf(
"  histograms of the angles and the aspect ratios of the triangles in the\n");
  printf("  mesh.\n\n");
  printf("Exact Arithmetic:\n\n");
  printf(
"  Triangle uses adaptive exact arithmetic to perform what computational\n");
  printf(
"  geometers call the `orientation' and `incircle' tests.  If the floating-\n"
);
  printf(
"  point arithmetic of your machine conforms to the IEEE 754 standard (as\n");
  printf(
"  most workstations do), and does not use extended precision internal\n");
  printf(
"  floating-point registers, then your output is guaranteed to be an\n");
  printf(
"  absolutely true Delaunay or constrained Delaunay triangulation, roundoff\n"
);
  printf(
"  error notwithstanding.  The word `adaptive' implies that these arithmetic\n"
);
  printf(
"  routines compute the result only to the precision necessary to guarantee\n"
);
  printf(
"  correctness, so they are usually nearly as fast as their approximate\n");
  printf("  counterparts.\n\n");
  printf(
"  May CPUs, including Intel x86 processors, have extended precision\n");
  printf(
"  floating-point registers.  These must be reconfigured so their precision\n"
);
  printf(
"  is reduced to memory precision.  Triangle does this if it is compiled\n");
  printf("  correctly.  See the makefile for details.\n\n");
  printf(
"  The exact tests can be disabled with the -X switch.  On most inputs, this\n"
);
  printf(
"  switch reduces the computation time by about eight percent--it's not\n");
  printf(
"  worth the risk.  There are rare difficult inputs (having many collinear\n");
  printf(
"  and cocircular vertices), however, for which the difference in speed\n");
  printf(
"  could be a factor of two.  Be forewarned that these are precisely the\n");
  printf(
"  inputs most likely to cause errors if you use the -X switch.  Hence, the\n"
);
  printf("  -X switch is not recommended.\n\n");
  printf(
"  Unfortunately, the exact tests don't solve every numerical problem.\n");
  printf(
"  Exact arithmetic is not used to compute the positions of new vertices,\n");
  printf(
"  because the bit complexity of vertex coordinates would grow without\n");
  printf(
"  bound.  Hence, segment intersections aren't computed exactly; in very\n");
  printf(
"  unusual cases, roundoff error in computing an intersection point might\n");
  printf(
"  actually lead to an inverted triangle and an invalid triangulation.\n");
  printf(
"  (This is one reason to specify your own intersection points in your .poly\n"
);
  printf(
"  files.)  Similarly, exact arithmetic is not used to compute the vertices\n"
);
  printf("  of the Voronoi diagram.\n\n");
  printf(
"  Another pair of problems not solved by the exact arithmetic routines is\n");
  printf(
"  underflow and overflow.  If Triangle is compiled for double precision\n");
  printf(
"  arithmetic, I believe that Triangle's geometric predicates work correctly\n"
);
  printf(
"  if the exponent of every input coordinate falls in the range [-148, 201].\n"
);
  printf(
"  Underflow can silently prevent the orientation and incircle tests from\n");
  printf(
"  being performed exactly, while overflow typically causes a floating\n");
  printf("  exception.\n\n");
  printf("Calling Triangle from Another Program:\n\n");
  printf("  Read the file triangle.h for details.\n\n");
  printf("Troubleshooting:\n\n");
  printf("  Please read this section before mailing me bugs.\n\n");
  printf("  `My output mesh has no triangles!'\n\n");
  printf(
"    If you're using a PSLG, you've probably failed to specify a proper set\n"
);
  printf(
"    of bounding segments, or forgotten to use the -c switch.  Or you may\n");
  printf(
"    have placed a hole badly, thereby eating all your triangles.  To test\n");
  printf("    these possibilities, try again with the -c and -O switches.\n");
  printf(
"    Alternatively, all your input vertices may be collinear, in which case\n"
);
  printf("    you can hardly expect to triangulate them.\n\n");
  printf("  `Triangle doesn't terminate, or just crashes.'\n\n");
  printf(
"    Bad things can happen when triangles get so small that the distance\n");
  printf(
"    between their vertices isn't much larger than the precision of your\n");
  printf(
"    machine's arithmetic.  If you've compiled Triangle for single-precision\n"
);
  printf(
"    arithmetic, you might do better by recompiling it for double-precision.\n"
);
  printf(
"    Then again, you might just have to settle for more lenient constraints\n"
);
  printf(
"    on the minimum angle and the maximum area than you had planned.\n");
  printf("\n");
  printf(
"    You can minimize precision problems by ensuring that the origin lies\n");
  printf(
"    inside your vertex set, or even inside the densest part of your\n");
  printf(
"    mesh.  If you're triangulating an object whose x-coordinates all fall\n");
  printf(
"    between 6247133 and 6247134, you're not leaving much floating-point\n");
  printf("    precision for Triangle to work with.\n\n");
  printf(
"    Precision problems can occur covertly if the input PSLG contains two\n");
  printf(
"    segments that meet (or intersect) at an extremely small angle, or if\n");
  printf(
"    such an angle is introduced by the -c switch.  If you don't realize\n");
  printf(
"    that a tiny angle is being formed, you might never discover why\n");
  printf(
"    Triangle is crashing.  To check for this possibility, use the -S switch\n"
);
  printf(
"    (with an appropriate limit on the number of Steiner points, found by\n");
  printf(
"    trial-and-error) to stop Triangle early, and view the output .poly file\n"
);
  printf(
"    with Show Me (described below).  Look carefully for regions where dense\n"
);
  printf(
"    clusters of vertices are forming and for small angles between segments.\n"
);
  printf(
"    Zoom in closely, as such segments might look like a single segment from\n"
);
  printf("    a distance.\n\n");
  printf(
"    If some of the input values are too large, Triangle may suffer a\n");
  printf(
"    floating exception due to overflow when attempting to perform an\n");
  printf(
"    orientation or incircle test.  (Read the section on exact arithmetic\n");
  printf(
"    above.)  Again, I recommend compiling Triangle for double (rather\n");
  printf("    than single) precision arithmetic.\n\n");
  printf(
"    Unexpected problems can arise if you use quality meshing (-q, -a, or\n");
  printf(
"    -u) with an input that is not segment-bounded--that is, if your input\n");
  printf(
"    is a vertex set, or you're using the -c switch.  If the convex hull of\n"
);
  printf(
"    your input vertices has collinear vertices on its boundary, an input\n");
  printf(
"    vertex that you think lies on the convex hull might actually lie just\n");
  printf(
"    inside the convex hull.  If so, the vertex and the nearby convex hull\n");
  printf(
"    edge form an extremely thin triangle.  When Triangle tries to refine\n");
  printf(
"    the mesh to enforce angle and area constraints, Triangle might generate\n"
);
  printf(
"    extremely tiny triangles, or it might fail because of insufficient\n");
  printf("    floating-point precision.\n\n");
  printf(
"  `The numbering of the output vertices doesn't match the input vertices.'\n"
);
  printf("\n");
  printf(
"    You may have had duplicate input vertices, or you may have eaten some\n");
  printf(
"    of your input vertices with a hole, or by placing them outside the area\n"
);
  printf(
"    enclosed by segments.  In any case, you can solve the problem by not\n");
  printf("    using the -j switch.\n\n");
  printf(
"  `Triangle executes without incident, but when I look at the resulting\n");
  printf(
"  mesh, it has overlapping triangles or other geometric inconsistencies.'\n");
  printf("\n");
  printf(
"    If you select the -X switch, Triangle occasionally makes mistakes due\n");
  printf(
"    to floating-point roundoff error.  Although these errors are rare,\n");
  printf(
"    don't use the -X switch.  If you still have problems, please report the\n"
);
  printf("    bug.\n\n");
  printf(
"  `Triangle executes without incident, but when I look at the resulting\n");
  printf("  Voronoi diagram, it has overlapping edges or other geometric\n");
  printf("  inconsistencies.'\n");
  printf("\n");
  printf(
"    If your input is a PSLG (-p), you can only expect a meaningful Voronoi\n"
);
  printf(
"    diagram if the domain you are triangulating is convex and free of\n");
  printf(
"    holes, and you use the -D switch to construct a conforming Delaunay\n");
  printf("    triangulation (instead of a CDT or CCDT).\n\n");
  printf(
"  Strange things can happen if you've taken liberties with your PSLG.  Do\n");
  printf(
"  you have a vertex lying in the middle of a segment?  Triangle sometimes\n");
  printf(
"  copes poorly with that sort of thing.  Do you want to lay out a collinear\n"
);
  printf(
"  row of evenly spaced, segment-connected vertices?  Have you simply\n");
  printf(
"  defined one __int64 segment connecting the leftmost vertex to the rightmost\n"
);
  printf(
"  vertex, and a bunch of vertices lying a__int64 it?  This method occasionally\n"
);
  printf(
"  works, especially with horizontal and vertical lines, but often it\n");
  printf(
"  doesn't, and you'll have to connect each adjacent pair of vertices with a\n"
);
  printf("  separate segment.  If you don't like it, tough.\n\n");
  printf(
"  Furthermore, if you have segments that intersect other than at their\n");
  printf(
"  endpoints, try not to let the intersections fall extremely close to PSLG\n"
);
  printf("  vertices or each other.\n\n");
  printf(
"  If you have problems refining a triangulation not produced by Triangle:\n");
  printf(
"  Are you sure the triangulation is geometrically valid?  Is it formatted\n");
  printf(
"  correctly for Triangle?  Are the triangles all listed so the first three\n"
);
  printf(
"  vertices are their corners in counterclockwise order?  Are all of the\n");
  printf(
"  triangles constrained Delaunay?  Triangle's Delaunay refinement algorithm\n"
);
  printf("  assumes that it starts with a CDT.\n\n");
  printf("Show Me:\n\n");
  printf(
"  Triangle comes with a separate program named `Show Me', whose primary\n");
  printf(
"  purpose is to draw meshes on your screen or in PostScript.  Its secondary\n"
);
  printf(
"  purpose is to check the validity of your input files, and do so more\n");
  printf(
"  thoroughly than Triangle does.  Unlike Triangle, Show Me requires that\n");
  printf(
"  you have the X Windows system.  Sorry, Microsoft Windows users.\n");
  printf("\n");
  printf("Triangle on the Web:\n");
  printf("\n");
  printf("  To see an illustrated version of these instructions, check out\n");
  printf("\n");
  printf("    http://www.cs.cmu.edu/~quake/triangle.html\n");
  printf("\n");
  printf("A Brief Plea:\n");
  printf("\n");
  printf(
"  If you use Triangle, and especially if you use it to accomplish real\n");
  printf(
"  work, I would like very much to hear from you.  A short letter or email\n");
  printf(
"  (to jrs@cs.berkeley.edu) describing how you use Triangle will mean a lot\n"
);
  printf(
"  to me.  The more people I know are using this program, the more easily I\n"
);
  printf(
"  can justify spending time on improvements, which in turn will benefit\n");
  printf(
"  you.  Also, I can put you on a list to receive email whenever a new\n");
  printf("  version of Triangle is available.\n\n");
  printf(
"  If you use a mesh generated by Triangle in a publication, please include\n"
);
  printf(
"  an acknowledgment as well.  And please spell Triangle with a capital `T'!\n"
);
  printf(
"  If you want to include a citation, use `Jonathan Richard Shewchuk,\n");
  printf(
"  ``Triangle: Engineering a 2D Quality Mesh Generator and Delaunay\n");
  printf(
"  Triangulator,'' in Applied Computational Geometry:  Towards Geometric\n");
  printf(
"  Engineering (Ming C. Lin and Dinesh Manocha, editors), volume 1148 of\n");
  printf(
"  Lecture Notes in Computer Science, pages 203-222, Springer-Verlag,\n");
  printf(
"  Berlin, May 1996.  (From the First ACM Workshop on Applied Computational\n"
);
  printf("  Geometry.)'\n\n");
  printf("Research credit:\n\n");
  printf(
"  Of course, I can take credit for only a fraction of the ideas that made\n");
  printf(
"  this mesh generator possible.  Triangle owes its existence to the efforts\n"
);
  printf(
"  of many fine computational geometers and other researchers, including\n");
  printf(
"  Marshall Bern, L. Paul Chew, Kenneth L. Clarkson, Boris Delaunay, Rex A.\n"
);
  printf(
"  Dwyer, David Eppstein, Steven Fortune, Leonidas J. Guibas, Donald E.\n");
  printf(
"  Knuth, Charles L. Lawson, Der-Tsai Lee, Gary L. Miller, Ernst P. Mucke,\n");
  printf(
"  Steven E. Pav, Douglas M. Priest, Jim Ruppert, Isaac Saias, Bruce J.\n");
  printf(
"  Schachter, Micha Sharir, Peter W. Shor, Daniel D. Sleator, Jorge Stolfi,\n"
);
  printf("  Robert E. Tarjan, Alper Ungor, Christopher J. Van Wyk, Noel J.\n");
  printf(
"  Walkington, and Binhai Zhu.  See the comments at the beginning of the\n");
  printf("  source code for references.\n\n");
  triexit(0);
}
