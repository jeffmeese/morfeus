#ifndef TRIANGLE_H
#define TRIANGLE_H

/*****************************************************************************/
/*                                                                           */
/*      888888888        ,o,                          / 888                  */
/*         888    88o88o  "    o8888o  88o8888o o88888o 888  o88888o         */
/*         888    888    888       88b 888  888 888 888 888 d888  88b        */
/*         888    888    888  o88^o888 888  888 "88888" 888 8888oo888        */
/*         888    888    888 C888  888 888  888  /      888 q888             */
/*         888    888    888  "88o^888 888  888 Cb      888  "88oooo"        */
/*                                              "8oo8D                       */
/*                                                                           */
/*  A Two-Dimensional Quality Mesh Generator and Delaunay Triangulator.      */
/*  (triangle.c)                                                             */
/*                                                                           */
/*  Version 1.6                                                              */
/*  July 28, 2005                                                            */
/*                                                                           */
/*  Copyright 1993, 1995, 1997, 1998, 2002, 2005                             */
/*  Jonathan Richard Shewchuk                                                */
/*  2360 Woolsey #H                                                          */
/*  Berkeley, California  94705-1927                                         */
/*  jrs@cs.berkeley.edu                                                      */
/*                                                                           */
/*  This program may be freely redistributed under the condition that the    */
/*    copyright notices (including this entire header and the copyright      */
/*    notice printed when the `-h' switch is selected) are not removed, and  */
/*    no compensation is received.  Private, research, and institutional     */
/*    use is free.  You may distribute modified versions of this code UNDER  */
/*    THE CONDITION THAT THIS CODE AND ANY MODIFICATIONS MADE TO IT IN THE   */
/*    SAME FILE REMAIN UNDER COPYRIGHT OF THE ORIGINAL AUTHOR, BOTH SOURCE   */
/*    AND OBJECT CODE ARE MADE FREELY AVAILABLE WITHOUT CHARGE, AND CLEAR    */
/*    NOTICE IS GIVEN OF THE MODIFICATIONS.  Distribution of this code as    */
/*    part of a commercial system is permissible ONLY BY DIRECT ARRANGEMENT  */
/*    WITH THE AUTHOR.  (If you are not directly supplying this code to a    */
/*    customer, and you are instead telling them how they can obtain it for  */
/*    free, then you are not required to make any arrangement with me.)      */
/*                                                                           */
/*  Hypertext instructions for Triangle are available on the Web at          */
/*                                                                           */
/*      http://www.cs.cmu.edu/~quake/triangle.html                           */
/*                                                                           */
/*  Disclaimer:  Neither I nor Carnegie Mellon warrant this code in any way  */
/*    whatsoever.  This code is provided "as-is".  Use at your own risk.     */
/*                                                                           */
/*  Some of the references listed below are marked with an asterisk.  [*]    */
/*    These references are available for downloading from the Web page       */
/*                                                                           */
/*      http://www.cs.cmu.edu/~quake/triangle.research.html                  */
/*                                                                           */
/*  Three papers discussing aspects of Triangle are available.  A short      */
/*    overview appears in "Triangle:  Engineering a 2D Quality Mesh          */
/*    Generator and Delaunay Triangulator," in Applied Computational         */
/*    Geometry:  Towards Geometric Engineering, Ming C. Lin and Dinesh       */
/*    Manocha, editors, Lecture Notes in Computer Science volume 1148,       */
/*    pages 203-222, Springer-Verlag, Berlin, May 1996 (from the First ACM   */
/*    Workshop on Applied Computational Geometry).  [*]                      */
/*                                                                           */
/*    The algorithms are discussed in the greatest detail in "Delaunay       */
/*    Refinement Algorithms for Triangular Mesh Generation," Computational   */
/*    Geometry:  Theory and Applications 22(1-3):21-74, May 2002.  [*]       */
/*                                                                           */
/*    More detail about the data structures may be found in my dissertation: */
/*    "Delaunay Refinement Mesh Generation," Ph.D. thesis, Technical Report  */
/*    CMU-CS-97-137, School of Computer Science, Carnegie Mellon University, */
/*    Pittsburgh, Pennsylvania, 18 May 1997.  [*]                            */
/*                                                                           */
/*  Triangle was created as part of the Quake Project in the School of       */
/*    Computer Science at Carnegie Mellon University.  For further           */
/*    information, see Hesheng Bao, Jacobo Bielak, Omar Ghattas, Loukas F.   */
/*    Kallivokas, David R. O'Hallaron, Jonathan R. Shewchuk, and Jifeng Xu,  */
/*    "Large-scale Simulation of Elastic Wave Propagation in Heterogeneous   */
/*    Media on Parallel Computers," Computer Methods in Applied Mechanics    */
/*    and Engineering 152(1-2):85-102, 22 January 1998.                      */
/*                                                                           */
/*  Triangle's Delaunay refinement algorithm for quality mesh generation is  */
/*    a hybrid of one due to Jim Ruppert, "A Delaunay Refinement Algorithm   */
/*    for Quality 2-Dimensional Mesh Generation," Journal of Algorithms      */
/*    18(3):548-585, May 1995 [*], and one due to L. Paul Chew, "Guaranteed- */
/*    Quality Mesh Generation for Curved Surfaces," Proceedings of the Ninth */
/*    Annual Symposium on Computational Geometry (San Diego, California),    */
/*    pages 274-280, Association for Computing Machinery, May 1993,          */
/*    http://portal.acm.org/citation.cfm?id=161150 .                         */
/*                                                                           */
/*  The Delaunay refinement algorithm has been modified so that it meshes    */
/*    domains with small input angles well, as described in Gary L. Miller,  */
/*    Steven E. Pav, and Noel J. Walkington, "When and Why Ruppert's         */
/*    Algorithm Works," Twelfth International Meshing Roundtable, pages      */
/*    91-102, Sandia National Laboratories, September 2003.  [*]             */
/*                                                                           */
/*  My implementation of the divide-and-conquer and incremental Delaunay     */
/*    triangulation algorithms follows closely the presentation of Guibas    */
/*    and Stolfi, even though I use a triangle-based data structure instead  */
/*    of their quad-edge data structure.  (In fact, I originally implemented */
/*    Triangle using the quad-edge data structure, but the switch to a       */
/*    triangle-based data structure sped Triangle by a factor of two.)  The  */
/*    mesh manipulation primitives and the two aforementioned Delaunay       */
/*    triangulation algorithms are described by Leonidas J. Guibas and Jorge */
/*    Stolfi, "Primitives for the Manipulation of General Subdivisions and   */
/*    the Computation of Voronoi Diagrams," ACM Transactions on Graphics     */
/*    4(2):74-123, April 1985, http://portal.acm.org/citation.cfm?id=282923 .*/
/*                                                                           */
/*  Their O(n log n) divide-and-conquer algorithm is adapted from Der-Tsai   */
/*    Lee and Bruce J. Schachter, "Two Algorithms for Constructing the       */
/*    Delaunay Triangulation," International Journal of Computer and         */
/*    Information Science 9(3):219-242, 1980.  Triangle's improvement of the */
/*    divide-and-conquer algorithm by alternating between vertical and       */
/*    horizontal cuts was introduced by Rex A. Dwyer, "A Faster Divide-and-  */
/*    Conquer Algorithm for Constructing Delaunay Triangulations,"           */
/*    Algorithmica 2(2):137-151, 1987.                                       */
/*                                                                           */
/*  The incremental insertion algorithm was first proposed by C. L. Lawson,  */
/*    "Software for C1 Surface Interpolation," in Mathematical Software III, */
/*    John R. Rice, editor, Academic Press, New York, pp. 161-194, 1977.     */
/*    For point location, I use the algorithm of Ernst P. Mucke, Isaac       */
/*    Saias, and Binhai Zhu, "Fast Randomized Point Location Without         */
/*    Preprocessing in Two- and Three-Dimensional Delaunay Triangulations,"  */
/*    Proceedings of the Twelfth Annual Symposium on Computational Geometry, */
/*    ACM, May 1996.  [*]  If I were to randomize the order of vertex        */
/*    insertion (I currently don't bother), their result combined with the   */
/*    result of Kenneth L. Clarkson and Peter W. Shor, "Applications of      */
/*    Random Sampling in Computational Geometry II," Discrete &              */
/*    Computational Geometry 4(1):387-421, 1989, would yield an expected     */
/*    O(n^{4/3}) bound on running time.                                      */
/*                                                                           */
/*  The O(n log n) sweepline Delaunay triangulation algorithm is taken from  */
/*    Steven Fortune, "A Sweepline Algorithm for Voronoi Diagrams",          */
/*    Algorithmica 2(2):153-174, 1987.  A random sample of edges on the      */
/*    boundary of the triangulation are maintained in a splay tree for the   */
/*    purpose of point location.  Splay trees are described by Daniel        */
/*    Dominic Sleator and Robert Endre Tarjan, "Self-Adjusting Binary Search */
/*    Trees," Journal of the ACM 32(3):652-686, July 1985,                   */
/*    http://portal.acm.org/citation.cfm?id=3835 .                           */
/*                                                                           */
/*  The algorithms for exact computation of the signs of determinants are    */
/*    described in Jonathan Richard Shewchuk, "Adaptive Precision Floating-  */
/*    Point Arithmetic and Fast Robust Geometric Predicates," Discrete &     */
/*    Computational Geometry 18(3):305-363, October 1997.  (Also available   */
/*    as Technical Report CMU-CS-96-140, School of Computer Science,         */
/*    Carnegie Mellon University, Pittsburgh, Pennsylvania, May 1996.)  [*]  */
/*    An abbreviated version appears as Jonathan Richard Shewchuk, "Robust   */
/*    Adaptive Floating-Point Geometric Predicates," Proceedings of the      */
/*    Twelfth Annual Symposium on Computational Geometry, ACM, May 1996. [*] */
/*    Many of the ideas for my exact arithmetic routines originate with      */
/*    Douglas M. Priest, "Algorithms for Arbitrary Precision Floating Point  */
/*    Arithmetic," Tenth Symposium on Computer Arithmetic, pp. 132-143, IEEE */
/*    Computer Society Press, 1991.  [*]  Many of the ideas for the correct  */
/*    evaluation of the signs of determinants are taken from Steven Fortune  */
/*    and Christopher J. Van Wyk, "Efficient Exact Arithmetic for Computa-   */
/*    tional Geometry," Proceedings of the Ninth Annual Symposium on         */
/*    Computational Geometry, ACM, pp. 163-172, May 1993, and from Steven    */
/*    Fortune, "Numerical Stability of Algorithms for 2D Delaunay Triangu-   */
/*    lations," International Journal of Computational Geometry & Applica-   */
/*    tions 5(1-2):193-213, March-June 1995.                                 */
/*                                                                           */
/*  The method of inserting new vertices off-center (not precisely at the    */
/*    circumcenter of every poor-quality triangle) is from Alper Ungor,      */
/*    "Off-centers:  A New Type of Steiner Points for Computing Size-Optimal */
/*    Quality-Guaranteed Delaunay Triangulations," Proceedings of LATIN      */
/*    2004 (Buenos Aires, Argentina), April 2004.                            */
/*                                                                           */
/*  For definitions of and results involving Delaunay triangulations,        */
/*    constrained and conforming versions thereof, and other aspects of      */
/*    triangular mesh generation, see the excellent survey by Marshall Bern  */
/*    and David Eppstein, "Mesh Generation and Optimal Triangulation," in    */
/*    Computing and Euclidean Geometry, Ding-Zhu Du and Frank Hwang,         */
/*    editors, World Scientific, Singapore, pp. 23-90, 1992.  [*]            */
/*                                                                           */
/*  The time for incrementally adding PSLG (planar straight line graph)      */
/*    segments to create a constrained Delaunay triangulation is probably    */
/*    O(t^2) per segment in the worst case and O(t) per segment in the       */
/*    common case, where t is the number of triangles that intersect the     */
/*    segment before it is inserted.  This doesn't count point location,     */
/*    which can be much more expensive.  I could improve this to O(d log d)  */
/*    time, but d is usually quite small, so it's not worth the bother.      */
/*    (This note does not apply when the -s switch is used, invoking a       */
/*    different method is used to insert segments.)                          */
/*                                                                           */
/*  The time for deleting a vertex from a Delaunay triangulation is O(d^2)   */
/*    in the worst case and O(d) in the common case, where d is the degree   */
/*    of the vertex being deleted.  I could improve this to O(d log d) time, */
/*    but d is usually quite small, so it's not worth the bother.            */
/*                                                                           */
/*  Ruppert's Delaunay refinement algorithm typically generates triangles    */
/*    at a linear rate (constant time per triangle) after the initial        */
/*    triangulation is formed.  There may be pathological cases where        */
/*    quadratic time is required, but these never arise in practice.         */
/*                                                                           */
/*  The geometric predicates (circumcenter calculations, segment             */
/*    intersection formulae, etc.) appear in my "Lecture Notes on Geometric  */
/*    Robustness" at http://www.cs.berkeley.edu/~jrs/mesh .                  */
/*                                                                           */
/*  If you make any improvements to this code, please please please let me   */
/*    know, so that I may obtain the improvements.  Even if you don't change */
/*    the code, I'd still love to hear what it's being used for.             */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>

#define REAL double

#ifdef _UNIX
#define ULONGLONG uint64_t
#else
#define ULONGLONG __int64
#endif

#define INEXACT /* Nothing */
/* #define INEXACT volatile */

/* To insert lots of self-checks for internal errors, define the SELF_CHECK  */
/*   symbol.  This will slow down the program significantly.  It is best to  */
/*   define the symbol using the -DSELF_CHECK compiler switch, but you could */
/*   write "#define SELF_CHECK" below.  If you are modifying this code, I    */
/*   recommend you turn self-checks on until your work is debugged.          */

/* #define SELF_CHECK */

/* Maximum number of characters in a file name (including the null).         */
#define FILENAMESIZE 2048

/* Maximum number of characters in a line read from a file (including the null). */
#define INPUTLINESIZE 1024

/* For efficiency, a variety of data structures are allocated in bulk.  The  */
/*   following constants determine how many of each structure is allocated   */
/*   at once.                                                                */
#define TRIPERBLOCK 4092        /* Number of triangles allocated at once. */
#define SUBSEGPERBLOCK 508      /* Number of subsegments allocated at once. */
#define VERTEXPERBLOCK 4092     /* Number of vertices allocated at once. */
#define VIRUSPERBLOCK 1020      /* Number of virus triangles allocated at once. */
#define BADSUBSEGPERBLOCK 252   /* Number of encroached subsegments allocated at once. */
#define BADTRIPERBLOCK 4092     /* Number of skinny triangles allocated at once. */
#define FLIPSTACKERPERBLOCK 252 /* Number of flipped triangles allocated at once. */
#define SPLAYNODEPERBLOCK 508   /* Number of splay tree nodes allocated at once. */

/* The vertex types. A DEADVERTEX has been deleted entirely.  An             */
/*   UNDEADVERTEX is not part of the mesh, but is written to the output      */
/*   .node file and affects the node indexing in the other output files.     */
#define INPUTVERTEX 0
#define SEGMENTVERTEX 1
#define FREEVERTEX 2
#define DEADVERTEX -32768
#define UNDEADVERTEX -32767

/* The next line is used to outsmart some very stupid compilers.  If your    */
/*   compiler is smarter, feel free to replace the "int" with "void".        */
/*   Not that it matters.                                                    */
#define VOID void

/* Two constants for algorithms based on random sampling.  Both constants    */
/*   have been chosen empirically to optimize their respective algorithms.   */

/* Used for the point location scheme of Mucke, Saias, and Zhu, to decide    */
/*   how large a random sample of triangles to inspect.                      */
#define SAMPLEFACTOR 11

/* Used in Fortune's sweepline Delaunay algorithm to determine what fraction */
/*   of boundary edges should be maintained in the splay tree for point      */
/*   location on the front.                                                  */
#define SAMPLERATE 10

/* A number that speaks for itself, every kissable digit.                    */
#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

/* Another fave.                                                             */
#define SQUAREROOTTWO 1.4142135623730950488016887242096980785696718753769480732

/* And here's one for those of you who are intimidated by math.              */
#define ONETHIRD 0.333333333333333333333333333333333333333333333333333333333333

/*****************************************************************************/
/*                                                                           */
/*  The `triangulateio' structure.                                           */
/*                                                                           */
/*  Used to pass data into and out of the triangulate() procedure.           */
/*                                                                           */
/*                                                                           */
/*  Arrays are used to store points, triangles, markers, and so forth.  In   */
/*  all cases, the first item in any array is stored starting at index [0].  */
/*  However, that item is item number `1' unless the `z' switch is used, in  */
/*  which case it is item number `0'.  Hence, you may find it easier to      */
/*  index points (and triangles in the neighbor list) if you use the `z'     */
/*  switch.  Unless, of course, you're calling Triangle from a Fortran       */
/*  program.                                                                 */
/*                                                                           */
/*  Description of fields (except the `numberof' fields, which are obvious): */
/*                                                                           */
/*  `pointlist':  An array of point coordinates.  The first point's x        */
/*    coordinate is at index [0] and its y coordinate at index [1], followed */
/*    by the coordinates of the remaining points.  Each point occupies two   */
/*    REALs.                                                                 */
/*  `pointattributelist':  An array of point attributes.  Each point's       */
/*    attributes occupy `numberofpointattributes' REALs.                     */
/*  `pointmarkerlist':  An array of point markers; one int per point.        */
/*                                                                           */
/*  `trianglelist':  An array of triangle corners.  The first triangle's     */
/*    first corner is at index [0], followed by its other two corners in     */
/*    counterclockwise order, followed by any other nodes if the triangle    */
/*    represents a nonlinear element.  Each triangle occupies                */
/*    `numberofcorners' ints.                                                */
/*  `triangleattributelist':  An array of triangle attributes.  Each         */
/*    triangle's attributes occupy `numberoftriangleattributes' REALs.       */
/*  `trianglearealist':  An array of triangle area constraints; one REAL per */
/*    triangle.  Input only.                                                 */
/*  `neighborlist':  An array of triangle neighbors; three ints per          */
/*    triangle.  Output only.                                                */
/*                                                                           */
/*  `segmentlist':  An array of segment endpoints.  The first segment's      */
/*    endpoints are at indices [0] and [1], followed by the remaining        */
/*    segments.  Two ints per segment.                                       */
/*  `segmentmarkerlist':  An array of segment markers; one int per segment.  */
/*                                                                           */
/*  `holelist':  An array of holes.  The first hole's x and y coordinates    */
/*    are at indices [0] and [1], followed by the remaining holes.  Two      */
/*    REALs per hole.  Input only, although the pointer is copied to the     */
/*    output structure for your convenience.                                 */
/*                                                                           */
/*  `regionlist':  An array of regional attributes and area constraints.     */
/*    The first constraint's x and y coordinates are at indices [0] and [1], */
/*    followed by the regional attribute at index [2], followed by the       */
/*    maximum area at index [3], followed by the remaining area constraints. */
/*    Four REALs per area constraint.  Note that each regional attribute is  */
/*    used only if you select the `A' switch, and each area constraint is    */
/*    used only if you select the `a' switch (with no number following), but */
/*    omitting one of these switches does not change the memory layout.      */
/*    Input only, although the pointer is copied to the output structure for */
/*    your convenience.                                                      */
/*                                                                           */
/*  `edgelist':  An array of edge endpoints.  The first edge's endpoints are */
/*    at indices [0] and [1], followed by the remaining edges.  Two ints per */
/*    edge.  Output only.                                                    */
/*  `edgemarkerlist':  An array of edge markers; one int per edge.  Output   */
/*    only.                                                                  */
/*  `normlist':  An array of normal vectors, used for infinite rays in       */
/*    Voronoi diagrams.  The first normal vector's x and y magnitudes are    */
/*    at indices [0] and [1], followed by the remaining vectors.  For each   */
/*    finite edge in a Voronoi diagram, the normal vector written is the     */
/*    zero vector.  Two REALs per edge.  Output only.                        */
/*                                                                           */
/*                                                                           */
/*  Any input fields that Triangle will examine must be initialized.         */
/*  Furthermore, for each output array that Triangle will write to, you      */
/*  must either provide space by setting the appropriate pointer to point    */
/*  to the space you want the data written to, or you must initialize the    */
/*  pointer to NULL, which tells Triangle to allocate space for the results. */
/*  The latter option is preferable, because Triangle always knows exactly   */
/*  how much space to allocate.  The former option is provided mainly for    */
/*  people who need to call Triangle from Fortran code, though it also makes */
/*  possible some nasty space-saving tricks, like writing the output to the  */
/*  same arrays as the input.                                                */
/*                                                                           */
/*  Triangle will not free() any input or output arrays, including those it  */
/*  allocates itself; that's up to you.  You should free arrays allocated by */
/*  Triangle by calling the trifree() procedure defined below.  (By default, */
/*  trifree() just calls the standard free() library procedure, but          */
/*  applications that call triangulate() may replace trimalloc() and         */
/*  trifree() in triangle.c to use specialized memory allocators.)           */
/*                                                                           */
/*  Here's a guide to help you decide which fields you must initialize       */
/*  before you call triangulate().                                           */
/*                                                                           */
/*  `in':                                                                    */
/*                                                                           */
/*    - `pointlist' must always point to a list of points; `numberofpoints'  */
/*      and `numberofpointattributes' must be properly set.                  */
/*      `pointmarkerlist' must either be set to NULL (in which case all      */
/*      markers default to zero), or must point to a list of markers.  If    */
/*      `numberofpointattributes' is not zero, `pointattributelist' must     */
/*      point to a list of point attributes.                                 */
/*    - If the `r' switch is used, `trianglelist' must point to a list of    */
/*      triangles, and `numberoftriangles', `numberofcorners', and           */
/*      `numberoftriangleattributes' must be properly set.  If               */
/*      `numberoftriangleattributes' is not zero, `triangleattributelist'    */
/*      must point to a list of triangle attributes.  If the `a' switch is   */
/*      used (with no number following), `trianglearealist' must point to a  */
/*      list of triangle area constraints.  `neighborlist' may be ignored.   */
/*    - If the `p' switch is used, `segmentlist' must point to a list of     */
/*      segments, `numberofsegments' must be properly set, and               */
/*      `segmentmarkerlist' must either be set to NULL (in which case all    */
/*      markers default to zero), or must point to a list of markers.        */
/*    - If the `p' switch is used without the `r' switch, then               */
/*      `numberofholes' and `numberofregions' must be properly set.  If      */
/*      `numberofholes' is not zero, `holelist' must point to a list of      */
/*      holes.  If `numberofregions' is not zero, `regionlist' must point to */
/*      a list of region constraints.                                        */
/*    - If the `p' switch is used, `holelist', `numberofholes',              */
/*      `regionlist', and `numberofregions' is copied to `out'.  (You can    */
/*      nonetheless get away with not initializing them if the `r' switch is */
/*      used.)                                                               */
/*    - `edgelist', `edgemarkerlist', `normlist', and `numberofedges' may be */
/*      ignored.                                                             */
/*                                                                           */
/*  `out':                                                                   */
/*                                                                           */
/*    - `pointlist' must be initialized (NULL or pointing to memory) unless  */
/*      the `N' switch is used.  `pointmarkerlist' must be initialized       */
/*      unless the `N' or `B' switch is used.  If `N' is not used and        */
/*      `in->numberofpointattributes' is not zero, `pointattributelist' must */
/*      be initialized.                                                      */
/*    - `trianglelist' must be initialized unless the `E' switch is used.    */
/*      `neighborlist' must be initialized if the `n' switch is used.  If    */
/*      the `E' switch is not used and (`in->numberofelementattributes' is   */
/*      not zero or the `A' switch is used), `elementattributelist' must be  */
/*      initialized.  `trianglearealist' may be ignored.                     */
/*    - `segmentlist' must be initialized if the `p' or `c' switch is used,  */
/*      and the `P' switch is not used.  `segmentmarkerlist' must also be    */
/*      initialized under these circumstances unless the `B' switch is used. */
/*    - `edgelist' must be initialized if the `e' switch is used.            */
/*      `edgemarkerlist' must be initialized if the `e' switch is used and   */
/*      the `B' switch is not.                                               */
/*    - `holelist', `regionlist', `normlist', and all scalars may be ignored.*/
/*                                                                           */
/*  `vorout' (only needed if `v' switch is used):                            */
/*                                                                           */
/*    - `pointlist' must be initialized.  If `in->numberofpointattributes'   */
/*      is not zero, `pointattributelist' must be initialized.               */
/*      `pointmarkerlist' may be ignored.                                    */
/*    - `edgelist' and `normlist' must both be initialized.                  */
/*      `edgemarkerlist' may be ignored.                                     */
/*    - Everything else may be ignored.                                      */
/*                                                                           */
/*  After a call to triangulate(), the valid fields of `out' and `vorout'    */
/*  will depend, in an obvious way, on the choice of switches used.  Note    */
/*  that when the `p' switch is used, the pointers `holelist' and            */
/*  `regionlist' are copied from `in' to `out', but no new space is          */
/*  allocated; be careful that you don't free() the same array twice.  On    */
/*  the other hand, Triangle will never copy the `pointlist' pointer (or any */
/*  others); new space is allocated for `out->pointlist', or if the `N'      */
/*  switch is used, `out->pointlist' remains uninitialized.                  */
/*                                                                           */
/*  All of the meaningful `numberof' fields will be properly set; for        */
/*  instance, `numberofedges' will represent the number of edges in the      */
/*  triangulation whether or not the edges were written.  If segments are    */
/*  not used, `numberofsegments' will indicate the number of boundary edges. */
/*                                                                           */
/*****************************************************************************/

/* For single precision (which will save some memory and reduce paging),     */
/*   define the symbol SINGLE by using the -DSINGLE compiler switch or by    */
/*   writing "#define SINGLE" below.                                         */
/*                                                                           */
/* For double precision (which will allow you to refine meshes to a smaller  */
/*   edge length), leave SINGLE undefined.                                   */
/*                                                                           */
/* Double precision uses more memory, but improves the resolution of the     */
/*   meshes you can generate with Triangle.  It also reduces the likelihood  */
/*   of a floating exception due to overflow.  Finally, it is much faster    */
/*   than single precision on 64-bit architectures like the DEC Alpha.  I    */
/*   recommend double precision unless you want to generate a mesh for which */
/*   you do not have enough memory.                                          */

struct triangulateio {
  REAL *pointlist;                                               /* In / out */
  REAL *pointattributelist;                                      /* In / out */
  int *pointmarkerlist;                                          /* In / out */
  int numberofpoints;                                            /* In / out */
  int numberofpointattributes;                                   /* In / out */

  int *trianglelist;                                             /* In / out */
  REAL *triangleattributelist;                                   /* In / out */
  REAL *trianglearealist;                                         /* In only */
  int *neighborlist;                                             /* Out only */
  int numberoftriangles;                                         /* In / out */
  int numberofcorners;                                           /* In / out */
  int numberoftriangleattributes;                                /* In / out */

  int *segmentlist;                                              /* In / out */
  int *segmentmarkerlist;                                        /* In / out */
  int numberofsegments;                                          /* In / out */

  REAL *holelist;                        /* In / pointer to array copied out */
  int numberofholes;                                      /* In / copied out */

  REAL *regionlist;                      /* In / pointer to array copied out */
  int numberofregions;                                    /* In / copied out */

  int *edgelist;                                                 /* Out only */
  int *edgemarkerlist;            /* Not used with Voronoi diagram; out only */
  REAL *normlist;                /* Used only with Voronoi diagram; out only */
  int numberofedges;                                             /* Out only */
};

/* Labels that signify the result of point location.  The result of a        */
/*   search indicates that the point falls in the interior of a triangle, on */
/*   an edge, on a vertex, or outside the mesh.                              */

enum locateresult {
  INTRIANGLE,
  ONEDGE,
  ONVERTEX,
  OUTSIDE
};

/* Labels that signify the result of vertex insertion.  The result indicates */
/*   that the vertex was inserted with complete success, was inserted but    */
/*   encroaches upon a subsegment, was not inserted because it lies on a     */
/*   segment, or was not inserted because another vertex occupies the same   */
/*   location.                                                               */

enum insertvertexresult {
  SUCCESSFULVERTEX,
  ENCROACHINGVERTEX,
  VIOLATINGVERTEX,
  DUPLICATEVERTEX
};

/* Labels that signify the result of direction finding.  The result          */
/*   indicates that a segment connecting the two query points falls within   */
/*   the direction triangle, aULONGLONG the left edge of the direction triangle,  */
/*   or aULONGLONG the right edge of the direction triangle.                      */

enum finddirectionresult {
  WITHIN,
  LEFTCOLLINEAR,
  RIGHTCOLLINEAR
};

/*****************************************************************************/
/*                                                                           */
/*  The basic mesh data structures                                           */
/*                                                                           */
/*  There are three:  vertices, triangles, and subsegments (abbreviated      */
/*  `subseg').  These three data structures, linked by pointers, comprise    */
/*  the mesh.  A vertex simply represents a mesh vertex and its properties.  */
/*  A triangle is a triangle.  A subsegment is a special data structure used */
/*  to represent an impenetrable edge of the mesh (perhaps on the outer      */
/*  boundary, on the boundary of a hole, or part of an internal boundary     */
/*  separating two triangulated regions).  Subsegments represent boundaries, */
/*  defined by the user, that triangles may not lie across.                  */
/*                                                                           */
/*  A triangle consists of a list of three vertices, a list of three         */
/*  adjoining triangles, a list of three adjoining subsegments (when         */
/*  segments exist), an arbitrary number of optional user-defined            */
/*  floating-point attributes, and an optional area constraint.  The latter  */
/*  is an upper bound on the permissible area of each triangle in a region,  */
/*  used for mesh refinement.                                                */
/*                                                                           */
/*  For a triangle on a boundary of the mesh, some or all of the neighboring */
/*  triangles may not be present.  For a triangle in the interior of the     */
/*  mesh, often no neighboring subsegments are present.  Such absent         */
/*  triangles and subsegments are never represented by NULL pointers; they   */
/*  are represented by two special records:  `dummytri', the triangle that   */
/*  fills "outer space", and `dummysub', the omnipresent subsegment.         */
/*  `dummytri' and `dummysub' are used for several reasons; for instance,    */
/*  they can be dereferenced and their contents examined without violating   */
/*  protected memory.                                                        */
/*                                                                           */
/*  However, it is important to understand that a triangle includes other    */
/*  information as well.  The pointers to adjoining vertices, triangles, and */
/*  subsegments are ordered in a way that indicates their geometric relation */
/*  to each other.  Furthermore, each of these pointers contains orientation */
/*  information.  Each pointer to an adjoining triangle indicates which face */
/*  of that triangle is contacted.  Similarly, each pointer to an adjoining  */
/*  subsegment indicates which side of that subsegment is contacted, and how */
/*  the subsegment is oriented relative to the triangle.                     */
/*                                                                           */
/*  The data structure representing a subsegment may be thought to be        */
/*  abutting the edge of one or two triangle data structures:  either        */
/*  sandwiched between two triangles, or resting against one triangle on an  */
/*  exterior boundary or hole boundary.                                      */
/*                                                                           */
/*  A subsegment consists of a list of four vertices--the vertices of the    */
/*  subsegment, and the vertices of the segment it is a part of--a list of   */
/*  two adjoining subsegments, and a list of two adjoining triangles.  One   */
/*  of the two adjoining triangles may not be present (though there should   */
/*  always be one), and neighboring subsegments might not be present.        */
/*  Subsegments also store a user-defined integer "boundary marker".         */
/*  Typically, this integer is used to indicate what boundary conditions are */
/*  to be applied at that location in a finite element simulation.           */
/*                                                                           */
/*  Like triangles, subsegments maintain information about the relative      */
/*  orientation of neighboring objects.                                      */
/*                                                                           */
/*  Vertices are relatively simple.  A vertex is a list of floating-point    */
/*  numbers, starting with the x, and y coordinates, followed by an          */
/*  arbitrary number of optional user-defined floating-point attributes,     */
/*  followed by an integer boundary marker.  During the segment insertion    */
/*  phase, there is also a pointer from each vertex to a triangle that may   */
/*  contain it.  Each pointer is not always correct, but when one is, it     */
/*  speeds up segment insertion.  These pointers are assigned values once    */
/*  at the beginning of the segment insertion phase, and are not used or     */
/*  updated except during this phase.  Edge flipping during segment          */
/*  insertion will render some of them incorrect.  Hence, don't rely upon    */
/*  them for anything.                                                       */
/*                                                                           */
/*  Other than the exception mentioned above, vertices have no information   */
/*  about what triangles, subfacets, or subsegments they are linked to.      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  Handles                                                                  */
/*                                                                           */
/*  The oriented triangle (`otri') and oriented subsegment (`osub') data     */
/*  structures defined below do not themselves store any part of the mesh.   */
/*  The mesh itself is made of `triangle's, `subseg's, and `vertex's.        */
/*                                                                           */
/*  Oriented triangles and oriented subsegments will usually be referred to  */
/*  as "handles."  A handle is essentially a pointer into the mesh; it       */
/*  allows you to "hold" one particular part of the mesh.  Handles are used  */
/*  to specify the regions in which one is traversing and modifying the mesh.*/
/*  A single `triangle' may be held by many handles, or none at all.  (The   */
/*  latter case is not a memory leak, because the triangle is still          */
/*  connected to other triangles in the mesh.)                               */
/*                                                                           */
/*  An `otri' is a handle that holds a triangle.  It holds a specific edge   */
/*  of the triangle.  An `osub' is a handle that holds a subsegment.  It     */
/*  holds either the left or right side of the subsegment.                   */
/*                                                                           */
/*  Navigation about the mesh is accomplished through a set of mesh          */
/*  manipulation primitives, further below.  Many of these primitives take   */
/*  a handle and produce a new handle that holds the mesh near the first     */
/*  handle.  Other primitives take two handles and glue the corresponding    */
/*  parts of the mesh together.  The orientation of the handles is           */
/*  important.  For instance, when two triangles are glued together by the   */
/*  bond() primitive, they are glued at the edges on which the handles lie.  */
/*                                                                           */
/*  Because vertices have no information about which triangles they are      */
/*  attached to, I commonly represent a vertex by use of a handle whose      */
/*  origin is the vertex.  A single handle can simultaneously represent a    */
/*  triangle, an edge, and a vertex.                                         */
/*                                                                           */
/*****************************************************************************/

/* The triangle data structure.  Each triangle contains three pointers to    */
/*   adjoining triangles, plus three pointers to vertices, plus three        */
/*   pointers to subsegments (declared below; these pointers are usually     */
/*   `dummysub').  It may or may not also contain user-defined attributes    */
/*   and/or a floating-point "area constraint."  It may also contain extra   */
/*   pointers for nodes, when the user asks for high-order elements.         */
/*   Because the size and structure of a `triangle' is not decided until     */
/*   runtime, I haven't simply declared the type `triangle' as a struct.     */

typedef REAL **triangle;            /* Really:  typedef triangle *triangle   */

/* An oriented triangle:  includes a pointer to a triangle and orientation.  */
/*   The orientation denotes an edge of the triangle.  Hence, there are      */
/*   three possible orientations.  By convention, each edge always points    */
/*   counterclockwise about the corresponding triangle.                      */

struct otri {
  triangle *tri;
  int orient;                                         /* Ranges from 0 to 2. */
};

/* The subsegment data structure.  Each subsegment contains two pointers to  */
/*   adjoining subsegments, plus four pointers to vertices, plus two         */
/*   pointers to adjoining triangles, plus one boundary marker, plus one     */
/*   segment number.                                                         */

typedef REAL **subseg;                  /* Really:  typedef subseg *subseg   */

/* An oriented subsegment:  includes a pointer to a subsegment and an        */
/*   orientation.  The orientation denotes a side of the edge.  Hence, there */
/*   are two possible orientations.  By convention, the edge is always       */
/*   directed so that the "side" denoted is the right side of the edge.      */

struct osub {
  subseg *ss;
  int ssorient;                                       /* Ranges from 0 to 1. */
};

/* The vertex data structure.  Each vertex is actually an array of REALs.    */
/*   The number of REALs is unknown until runtime.  An integer boundary      */
/*   marker, and sometimes a pointer to a triangle, is appended after the    */
/*   REALs.                                                                  */

typedef REAL *vertex;

/* A queue used to store encroached subsegments.  Each subsegment's vertices */
/*   are stored so that we can check whether a subsegment is still the same. */

struct badsubseg {
  subseg encsubseg;                             /* An encroached subsegment. */
  vertex subsegorg, subsegdest;                         /* Its two vertices. */
};

/* A queue used to store bad triangles.  The key is the square of the cosine */
/*   of the smallest angle of the triangle.  Each triangle's vertices are    */
/*   stored so that one can check whether a triangle is still the same.      */

struct badtriang {
  triangle poortri;                       /* A skinny or too-large triangle. */
  REAL key;                             /* cos^2 of smallest (apical) angle. */
  vertex triangorg, triangdest, triangapex;           /* Its three vertices. */
  struct badtriang *nexttriang;             /* Pointer to next bad triangle. */
};

/* A stack of triangles flipped during the most recent vertex insertion.     */
/*   The stack is used to undo the vertex insertion if the vertex encroaches */
/*   upon a subsegment.                                                      */

struct flipstacker {
  triangle flippedtri;                       /* A recently flipped triangle. */
  struct flipstacker *prevflip;               /* Previous flip in the stack. */
};

/* A node in a heap used to store events for the sweepline Delaunay          */
/*   algorithm.  Nodes do not point directly to their parents or children in */
/*   the heap.  Instead, each node knows its position in the heap, and can   */
/*   look up its parent and children in a separate array.  The `eventptr'    */
/*   points either to a `vertex' or to a triangle (in encoded format, so     */
/*   that an orientation is included).  In the latter case, the origin of    */
/*   the oriented triangle is the apex of a "circle event" of the sweepline  */
/*   algorithm.  To distinguish site events from circle events, all circle   */
/*   events are given an invalid (smaller than `xmin') x-coordinate `xkey'.  */

struct event {
  REAL xkey, ykey;                              /* Coordinates of the event. */
  void *eventptr;      /* Can be a vertex or the location of a circle event. */
  int heapposition;              /* Marks this event's position in the heap. */
};

/* A node in the splay tree.  Each node holds an oriented ghost triangle     */
/*   that represents a boundary edge of the growing triangulation.  When a   */
/*   circle event covers two boundary edges with a triangle, so that they    */
/*   are no ULONGLONGer boundary edges, those edges are not immediately deleted   */
/*   from the tree; rather, they are lazily deleted when they are next       */
/*   encountered.  (Since only a random sample of boundary edges are kept    */
/*   in the tree, lazy deletion is faster.)  `keydest' is used to verify     */
/*   that a triangle is still the same as when it entered the splay tree; if */
/*   it has been rotated (due to a circle event), it no ULONGLONGer represents a  */
/*   boundary edge and should be deleted.                                    */

struct splaynode {
  struct otri keyedge;                     /* Lprev of an edge on the front. */
  vertex keydest;           /* Used to verify that splay node is still live. */
  struct splaynode *lchild, *rchild;              /* Children in splay tree. */
};

/* A type used to allocate memory.  firstblock is the first block of items.  */
/*   nowblock is the block from which items are currently being allocated.   */
/*   nextitem points to the next slab of free memory for an item.            */
/*   deaditemstack is the head of a linked list (stack) of deallocated items */
/*   that can be recycled.  unallocateditems is the number of items that     */
/*   remain to be allocated from nowblock.                                   */
/*                                                                           */
/* Traversal is the process of walking through the entire list of items, and */
/*   is separate from allocation.  Note that a traversal will visit items on */
/*   the "deaditemstack" stack as well as live items.  pathblock points to   */
/*   the block currently being traversed.  pathitem points to the next item  */
/*   to be traversed.  pathitemsleft is the number of items that remain to   */
/*   be traversed in pathblock.                                              */
/*                                                                           */
/* alignbytes determines how new records should be aligned in memory.        */
/*   itembytes is the length of a record in bytes (after rounding up).       */
/*   itemsperblock is the number of items allocated at once in a single      */
/*   block.  itemsfirstblock is the number of items in the first block,      */
/*   which can vary from the others.  items is the number of currently       */
/*   allocated items.  maxitems is the maximum number of items that have     */
/*   been allocated at once; it is the current number of items plus the      */
/*   number of records kept on deaditemstack.                                */

struct memorypool {
  void **firstblock, **nowblock;
  void *nextitem;
  void *deaditemstack;
  void **pathblock;
  void *pathitem;
  int alignbytes;
  int itembytes;
  int itemsperblock;
  int itemsfirstblock;
  ULONGLONG items, maxitems;
  int unallocateditems;
  int pathitemsleft;
};

/* Mesh data structure.  Triangle operates on only one mesh, but the mesh    */
/*   structure is used (instead of global variables) to allow reentrancy.    */

struct mesh {

/* Variables used to allocate memory for triangles, subsegments, vertices,   */
/*   viri (triangles being eaten), encroached segments, bad (skinny or too   */
/*   large) triangles, and splay tree nodes.                                 */

  struct memorypool triangles;
  struct memorypool subsegs;
  struct memorypool vertices;
  struct memorypool viri;
  struct memorypool badsubsegs;
  struct memorypool badtriangles;
  struct memorypool flipstackers;
  struct memorypool splaynodes;

/* Variables that maintain the bad triangle queues.  The queues are          */
/*   ordered from 4095 (highest priority) to 0 (lowest priority).            */

  struct badtriang *queuefront[4096];
  struct badtriang *queuetail[4096];
  int nextnonemptyq[4096];
  int firstnonemptyq;

/* Variable that maintains the stack of recently flipped triangles.          */

  struct flipstacker *lastflip;

/* Other variables. */

  REAL xmin, xmax, ymin, ymax;                            /* x and y bounds. */
  REAL xminextreme;      /* Nonexistent x value used as a flag in sweepline. */
  int invertices;                               /* Number of input vertices. */
  int inelements;                              /* Number of input triangles. */
  int insegments;                               /* Number of input segments. */
  int holes;                                       /* Number of input holes. */
  int regions;                                   /* Number of input regions. */
  int undeads;    /* Number of input vertices that don't appear in the mesh. */
  ULONGLONG edges;                                     /* Number of output edges. */
  int mesh_dim;                                /* Dimension (ought to be 2). */
  int nextras;                           /* Number of attributes per vertex. */
  int eextras;                         /* Number of attributes per triangle. */
  ULONGLONG hullsize;                          /* Number of edges in convex hull. */
  int steinerleft;                 /* Number of Steiner points not yet used. */
  int vertexmarkindex;         /* Index to find boundary marker of a vertex. */
  int vertex2triindex;     /* Index to find a triangle adjacent to a vertex. */
  int highorderindex;  /* Index to find extra nodes for high-order elements. */
  int elemattribindex;            /* Index to find attributes of a triangle. */
  int areaboundindex;             /* Index to find area bound of a triangle. */
  int checksegments;         /* Are there segments in the triangulation yet? */
  int checkquality;                  /* Has quality triangulation begun yet? */
  int readnodefile;                           /* Has a .node file been read? */
  ULONGLONG samples;              /* Number of random samples for point location. */

  ULONGLONG incirclecount;                 /* Number of incircle tests performed. */
  ULONGLONG counterclockcount;     /* Number of counterclockwise tests performed. */
  ULONGLONG orient3dcount;           /* Number of 3D orientation tests performed. */
  ULONGLONG hyperbolacount;      /* Number of right-of-hyperbola tests performed. */
  ULONGLONG circumcentercount;  /* Number of circumcenter calculations performed. */
  ULONGLONG circletopcount;       /* Number of circle top calculations performed. */

/* Triangular bounding box vertices.                                         */

  vertex infvertex1, infvertex2, infvertex3;

/* Pointer to the `triangle' that occupies all of "outer space."             */

  triangle *dummytri;
  triangle *dummytribase;    /* Keep base address so we can free() it later. */

/* Pointer to the omnipresent subsegment.  Referenced by any triangle or     */
/*   subsegment that isn't really connected to a subsegment at that          */
/*   location.                                                               */

  subseg *dummysub;
  subseg *dummysubbase;      /* Keep base address so we can free() it later. */

/* Pointer to a recently visited triangle.  Improves point location if       */
/*   proximate vertices are inserted sequentially.                           */

  struct otri recenttri;

};                                                  /* End of `struct mesh'. */


/* Data structure for command line switches and file names.  This structure  */
/*   is used (instead of global variables) to allow reentrancy.              */

struct behavior {

/* Switches for the triangulator.                                            */
/*   poly: -p switch.  refine: -r switch.                                    */
/*   quality: -q switch.                                                     */
/*     minangle: minimum angle bound, specified after -q switch.             */
/*     goodangle: cosine squared of minangle.                                */
/*     offconstant: constant used to place off-center Steiner points.        */
/*   vararea: -a switch without number.                                      */
/*   fixedarea: -a switch with number.                                       */
/*     maxarea: maximum area bound, specified after -a switch.               */
/*   usertest: -u switch.                                                    */
/*   regionattrib: -A switch.  convex: -c switch.                            */
/*   weighted: 1 for -w switch, 2 for -W switch.  jettison: -j switch        */
/*   firstnumber: inverse of -z switch.  All items are numbered starting     */
/*     from `firstnumber'.                                                   */
/*   edgesout: -e switch.  voronoi: -v switch.                               */
/*   neighbors: -n switch.  geomview: -g switch.                             */
/*   nobound: -B switch.  nopolywritten: -P switch.                          */
/*   nonodewritten: -N switch.  noelewritten: -E switch.                     */
/*   noiterationnum: -I switch.  noholes: -O switch.                         */
/*   noexact: -X switch.                                                     */
/*   order: element order, specified after -o switch.                        */
/*   nobisect: count of how often -Y switch is selected.                     */
/*   steiner: maximum number of Steiner points, specified after -S switch.   */
/*   incremental: -i switch.  sweepline: -F switch.                          */
/*   dwyer: inverse of -l switch.                                            */
/*   splitseg: -s switch.                                                    */
/*   conformdel: -D switch.  docheck: -C switch.                             */
/*   quiet: -Q switch.  verbose: count of how often -V switch is selected.   */
/*   usesegments: -p, -r, -q, or -c switch; determines whether segments are  */
/*     used at all.                                                          */
/*                                                                           */
/* Read the instructions to find out the meaning of these switches.          */

  int poly, refine, quality, vararea, fixedarea, usertest;
  int regionattrib, convex, weighted, jettison;
  int firstnumber;
  int edgesout, voronoi, neighbors, geomview;
  int nobound, nopolywritten, nonodewritten, noelewritten, noiterationnum;
  int noholes, noexact, conformdel;
  int incremental, sweepline, dwyer;
  int splitseg;
  int docheck;
  int quiet, verbose;
  int usesegments;
  int order;
  int nobisect;
  int steiner;
  REAL minangle, goodangle, offconstant;
  REAL maxarea;
};                                              /* End of `struct behavior'. */

#endif // TRIANGLE_H
