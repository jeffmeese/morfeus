#ifndef PRIMITIVES_H
#define PRIMITIVES_H

/*****************************************************************************/
/*                                                                           */
/*  Mesh manipulation primitives.  Each triangle contains three pointers to  */
/*  other triangles, with orientations.  Each pointer points not to the      */
/*  first byte of a triangle, but to one of the first three bytes of a       */
/*  triangle.  It is necessary to extract both the triangle itself and the   */
/*  orientation.  To save memory, I keep both pieces of information in one   */
/*  pointer.  To make this possible, I assume that all triangles are aligned */
/*  to four-byte boundaries.  The decode() routine below decodes a pointer,  */
/*  extracting an orientation (in the range 0 to 2) and a pointer to the     */
/*  beginning of a triangle.  The encode() routine compresses a pointer to a */
/*  triangle and an orientation into a single pointer.  My assumptions that  */
/*  triangles are four-byte-aligned and that the `unsigned ULONGLONG' type is     */
/*  ULONGLONG enough to hold a pointer are two of the few kludges in this program.*/
/*                                                                           */
/*  Subsegments are manipulated similarly.  A pointer to a subsegment        */
/*  carries both an address and an orientation in the range 0 to 1.          */
/*                                                                           */
/*  The other primitives take an oriented triangle or oriented subsegment,   */
/*  and return an oriented triangle or oriented subsegment or vertex; or     */
/*  they change the connections in the data structure.                       */
/*                                                                           */
/*  Below, triangles and subsegments are denoted by their vertices.  The     */
/*  triangle abc has origin (org) a, destination (dest) b, and apex (apex)   */
/*  c.  These vertices occur in counterclockwise order about the triangle.   */
/*  The handle abc may simultaneously denote vertex a, edge ab, and triangle */
/*  abc.                                                                     */
/*                                                                           */
/*  Similarly, the subsegment ab has origin (sorg) a and destination (sdest) */
/*  b.  If ab is thought to be directed upward (with b directly above a),    */
/*  then the handle ab is thought to grasp the right side of ab, and may     */
/*  simultaneously denote vertex a and edge ab.                              */
/*                                                                           */
/*  An asterisk (*) denotes a vertex whose identity is unknown.              */
/*                                                                           */
/*  Given this notation, a partial list of mesh manipulation primitives      */
/*  follows.                                                                 */
/*                                                                           */
/*                                                                           */
/*  For triangles:                                                           */
/*                                                                           */
/*  sym:  Find the abutting triangle; same edge.                             */
/*  sym(abc) -> ba*                                                          */
/*                                                                           */
/*  lnext:  Find the next edge (counterclockwise) of a triangle.             */
/*  lnext(abc) -> bca                                                        */
/*                                                                           */
/*  lprev:  Find the previous edge (clockwise) of a triangle.                */
/*  lprev(abc) -> cab                                                        */
/*                                                                           */
/*  onext:  Find the next edge counterclockwise with the same origin.        */
/*  onext(abc) -> ac*                                                        */
/*                                                                           */
/*  oprev:  Find the next edge clockwise with the same origin.               */
/*  oprev(abc) -> a*b                                                        */
/*                                                                           */
/*  dnext:  Find the next edge counterclockwise with the same destination.   */
/*  dnext(abc) -> *ba                                                        */
/*                                                                           */
/*  dprev:  Find the next edge clockwise with the same destination.          */
/*  dprev(abc) -> cb*                                                        */
/*                                                                           */
/*  rnext:  Find the next edge (counterclockwise) of the adjacent triangle.  */
/*  rnext(abc) -> *a*                                                        */
/*                                                                           */
/*  rprev:  Find the previous edge (clockwise) of the adjacent triangle.     */
/*  rprev(abc) -> b**                                                        */
/*                                                                           */
/*  org:  Origin          dest:  Destination          apex:  Apex            */
/*  org(abc) -> a         dest(abc) -> b              apex(abc) -> c         */
/*                                                                           */
/*  bond:  Bond two triangles together at the resepective handles.           */
/*  bond(abc, bad)                                                           */
/*                                                                           */
/*                                                                           */
/*  For subsegments:                                                         */
/*                                                                           */
/*  ssym:  Reverse the orientation of a subsegment.                          */
/*  ssym(ab) -> ba                                                           */
/*                                                                           */
/*  spivot:  Find adjoining subsegment with the same origin.                 */
/*  spivot(ab) -> a*                                                         */
/*                                                                           */
/*  snext:  Find next subsegment in sequence.                                */
/*  snext(ab) -> b*                                                          */
/*                                                                           */
/*  sorg:  Origin                      sdest:  Destination                   */
/*  sorg(ab) -> a                      sdest(ab) -> b                        */
/*                                                                           */
/*  sbond:  Bond two subsegments together at the respective origins.         */
/*  sbond(ab, ac)                                                            */
/*                                                                           */
/*                                                                           */
/*  For interacting tetrahedra and subfacets:                                */
/*                                                                           */
/*  tspivot:  Find a subsegment abutting a triangle.                         */
/*  tspivot(abc) -> ba                                                       */
/*                                                                           */
/*  stpivot:  Find a triangle abutting a subsegment.                         */
/*  stpivot(ab) -> ba*                                                       */
/*                                                                           */
/*  tsbond:  Bond a triangle to a subsegment.                                */
/*  tsbond(abc, ba)                                                          */
/*                                                                           */
/*****************************************************************************/

/********* Geometric primitives begin here                           *********/
/**                                                                         **/
/**                                                                         **/

/* The adaptive exact arithmetic geometric predicates implemented herein are */
/*   described in detail in my paper, "Adaptive Precision Floating-Point     */
/*   Arithmetic and Fast Robust Geometric Predicates."  See the header for a */
/*   full citation.                                                          */

/* Which of the following two methods of finding the absolute values is      */
/*   fastest is compiler-dependent.  A few compilers can inline and optimize */
/*   the fabs() call; but most will incur the overhead of a function call,   */
/*   which is disastrously slow.  A faster way on IEEE machines might be to  */
/*   mask the appropriate bit, but that's difficult to do in C without       */
/*   forcing the value to be stored to memory (rather than be kept in the    */
/*   register to which the optimizer assigned it).                           */

#define Absolute(a)  ((a) >= 0.0 ? (a) : -(a))
/* #define Absolute(a)  fabs(a) */

/* Many of the operations are broken up into two pieces, a main part that    */
/*   performs an approximate operation, and a "tail" that computes the       */
/*   roundoff error of that operation.                                       */
/*                                                                           */
/* The operations Fast_Two_Sum(), Fast_Two_Diff(), Two_Sum(), Two_Diff(),    */
/*   Split(), and Two_Product() are all implemented as described in the      */
/*   reference.  Each of these macros requires certain variables to be       */
/*   defined in the calling routine.  The variables `bvirt', `c', `abig',    */
/*   `_i', `_j', `_k', `_l', `_m', and `_n' are declared `INEXACT' because   */
/*   they store the result of an operation that may incur roundoff error.    */
/*   The input parameter `x' (or the highest numbered `x_' parameter) must   */
/*   also be declared `INEXACT'.                                             */

#define Fast_Two_Sum_Tail(a, b, x, y) \
  bvirt = x - a; \
  y = b - bvirt

#define Fast_Two_Sum(a, b, x, y) \
  x = (REAL) (a + b); \
  Fast_Two_Sum_Tail(a, b, x, y)

#define Two_Sum_Tail(a, b, x, y) \
  bvirt = (REAL) (x - a); \
  avirt = x - bvirt; \
  bround = b - bvirt; \
  around = a - avirt; \
  y = around + bround

#define Two_Sum(a, b, x, y) \
  x = (REAL) (a + b); \
  Two_Sum_Tail(a, b, x, y)

#define Two_Diff_Tail(a, b, x, y) \
  bvirt = (REAL) (a - x); \
  avirt = x + bvirt; \
  bround = bvirt - b; \
  around = a - avirt; \
  y = around + bround

#define Two_Diff(a, b, x, y) \
  x = (REAL) (a - b); \
  Two_Diff_Tail(a, b, x, y)

#define Split(a, ahi, alo) \
  c = (REAL) (splitter * a); \
  abig = (REAL) (c - a); \
  ahi = c - abig; \
  alo = a - ahi

#define Two_Product_Tail(a, b, x, y) \
  Split(a, ahi, alo); \
  Split(b, bhi, blo); \
  err1 = x - (ahi * bhi); \
  err2 = err1 - (alo * bhi); \
  err3 = err2 - (ahi * blo); \
  y = (alo * blo) - err3

#define Two_Product(a, b, x, y) \
  x = (REAL) (a * b); \
  Two_Product_Tail(a, b, x, y)

/* Two_Product_Presplit() is Two_Product() where one of the inputs has       */
/*   already been split.  Avoids redundant splitting.                        */

#define Two_Product_Presplit(a, b, bhi, blo, x, y) \
  x = (REAL) (a * b); \
  Split(a, ahi, alo); \
  err1 = x - (ahi * bhi); \
  err2 = err1 - (alo * bhi); \
  err3 = err2 - (ahi * blo); \
  y = (alo * blo) - err3

/* Square() can be done more quickly than Two_Product().                     */

#define Square_Tail(a, x, y) \
  Split(a, ahi, alo); \
  err1 = x - (ahi * ahi); \
  err3 = err1 - ((ahi + ahi) * alo); \
  y = (alo * alo) - err3

#define Square(a, x, y) \
  x = (REAL) (a * a); \
  Square_Tail(a, x, y)

/* Macros for summing expansions of various fixed lengths.  These are all    */
/*   unrolled versions of Expansion_Sum().                                   */

#define Two_One_Sum(a1, a0, b, x2, x1, x0) \
  Two_Sum(a0, b , _i, x0); \
  Two_Sum(a1, _i, x2, x1)

#define Two_One_Diff(a1, a0, b, x2, x1, x0) \
  Two_Diff(a0, b , _i, x0); \
  Two_Sum( a1, _i, x2, x1)

#define Two_Two_Sum(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Sum(a1, a0, b0, _j, _0, x0); \
  Two_One_Sum(_j, _0, b1, x3, x2, x1)

#define Two_Two_Diff(a1, a0, b1, b0, x3, x2, x1, x0) \
  Two_One_Diff(a1, a0, b0, _j, _0, x0); \
  Two_One_Diff(_j, _0, b1, x3, x2, x1)

/* Macro for multiplying a two-component expansion by a single component.    */

#define Two_One_Product(a1, a0, b, x3, x2, x1, x0) \
  Split(b, bhi, blo); \
  Two_Product_Presplit(a0, b, bhi, blo, _i, x0); \
  Two_Product_Presplit(a1, b, bhi, blo, _j, _0); \
  Two_Sum(_i, _0, _k, x1); \
  Fast_Two_Sum(_j, _k, x3, x2)

/* decode() converts a pointer to an oriented triangle.  The orientation is  */
/*   extracted from the two least significant bits of the pointer.           */

#define decode(ptr, otri)                                                     \
  (otri).orient = (int) ((unsigned ULONGLONG) (ptr) & (unsigned ULONGLONG) 3l);         \
  (otri).tri = (triangle *)                                                   \
                  ((unsigned ULONGLONG) (ptr) ^ (unsigned ULONGLONG) (otri).orient)

/* encode() compresses an oriented triangle into a single pointer.  It       */
/*   relies on the assumption that all triangles are aligned to four-byte    */
/*   boundaries, so the two least significant bits of (otri).tri are zero.   */

#define encode(otri)                                                          \
  (triangle) ((unsigned ULONGLONG) (otri).tri | (unsigned ULONGLONG) (otri).orient)

/* The following handle manipulation primitives are all described by Guibas  */
/*   and Stolfi.  However, Guibas and Stolfi use an edge-based data          */
/*   structure, whereas I use a triangle-based data structure.               */

/* sym() finds the abutting triangle, on the same edge.  Note that the edge  */
/*   direction is necessarily reversed, because the handle specified by an   */
/*   oriented triangle is directed counterclockwise around the triangle.     */

#define sym(otri1, otri2)                                                     \
  ptr = (otri1).tri[(otri1).orient];                                          \
  decode(ptr, otri2);

#define symself(otri)                                                         \
  ptr = (otri).tri[(otri).orient];                                            \
  decode(ptr, otri);

/* lnext() finds the next edge (counterclockwise) of a triangle.             */

#define lnext(otri1, otri2)                                                   \
  (otri2).tri = (otri1).tri;                                                  \
  (otri2).orient = plus1mod3[(otri1).orient]

#define lnextself(otri)                                                       \
  (otri).orient = plus1mod3[(otri).orient]

/* lprev() finds the previous edge (clockwise) of a triangle.                */

#define lprev(otri1, otri2)                                                   \
  (otri2).tri = (otri1).tri;                                                  \
  (otri2).orient = minus1mod3[(otri1).orient]

#define lprevself(otri)                                                       \
  (otri).orient = minus1mod3[(otri).orient]

/* onext() spins counterclockwise around a vertex; that is, it finds the     */
/*   next edge with the same origin in the counterclockwise direction.  This */
/*   edge is part of a different triangle.                                   */

#define onext(otri1, otri2)                                                   \
  lprev(otri1, otri2);                                                        \
  symself(otri2);

#define onextself(otri)                                                       \
  lprevself(otri);                                                            \
  symself(otri);

/* oprev() spins clockwise around a vertex; that is, it finds the next edge  */
/*   with the same origin in the clockwise direction.  This edge is part of  */
/*   a different triangle.                                                   */

#define oprev(otri1, otri2)                                                   \
  sym(otri1, otri2);                                                          \
  lnextself(otri2);

#define oprevself(otri)                                                       \
  symself(otri);                                                              \
  lnextself(otri);

/* dnext() spins counterclockwise around a vertex; that is, it finds the     */
/*   next edge with the same destination in the counterclockwise direction.  */
/*   This edge is part of a different triangle.                              */

#define dnext(otri1, otri2)                                                   \
  sym(otri1, otri2);                                                          \
  lprevself(otri2);

#define dnextself(otri)                                                       \
  symself(otri);                                                              \
  lprevself(otri);

/* dprev() spins clockwise around a vertex; that is, it finds the next edge  */
/*   with the same destination in the clockwise direction.  This edge is     */
/*   part of a different triangle.                                           */

#define dprev(otri1, otri2)                                                   \
  lnext(otri1, otri2);                                                        \
  symself(otri2);

#define dprevself(otri)                                                       \
  lnextself(otri);                                                            \
  symself(otri);

/* rnext() moves one edge counterclockwise about the adjacent triangle.      */
/*   (It's best understood by reading Guibas and Stolfi.  It involves        */
/*   changing triangles twice.)                                              */

#define rnext(otri1, otri2)                                                   \
  sym(otri1, otri2);                                                          \
  lnextself(otri2);                                                           \
  symself(otri2);

#define rnextself(otri)                                                       \
  symself(otri);                                                              \
  lnextself(otri);                                                            \
  symself(otri);

/* rprev() moves one edge clockwise about the adjacent triangle.             */
/*   (It's best understood by reading Guibas and Stolfi.  It involves        */
/*   changing triangles twice.)                                              */

#define rprev(otri1, otri2)                                                   \
  sym(otri1, otri2);                                                          \
  lprevself(otri2);                                                           \
  symself(otri2);

#define rprevself(otri)                                                       \
  symself(otri);                                                              \
  lprevself(otri);                                                            \
  symself(otri);

/* These primitives determine or set the origin, destination, or apex of a   */
/* triangle.                                                                 */

#define org(otri, vertexptr)                                                  \
  vertexptr = (vertex) (otri).tri[plus1mod3[(otri).orient] + 3]

#define dest(otri, vertexptr)                                                 \
  vertexptr = (vertex) (otri).tri[minus1mod3[(otri).orient] + 3]

#define apex(otri, vertexptr)                                                 \
  vertexptr = (vertex) (otri).tri[(otri).orient + 3]

#define setorg(otri, vertexptr)                                               \
  (otri).tri[plus1mod3[(otri).orient] + 3] = (triangle) vertexptr

#define setdest(otri, vertexptr)                                              \
  (otri).tri[minus1mod3[(otri).orient] + 3] = (triangle) vertexptr

#define setapex(otri, vertexptr)                                              \
  (otri).tri[(otri).orient + 3] = (triangle) vertexptr

/* Bond two triangles together.                                              */

#define bond(otri1, otri2)                                                    \
  (otri1).tri[(otri1).orient] = encode(otri2);                                \
  (otri2).tri[(otri2).orient] = encode(otri1)

/* Dissolve a bond (from one side).  Note that the other triangle will still */
/*   think it's connected to this triangle.  Usually, however, the other     */
/*   triangle is being deleted entirely, or bonded to another triangle, so   */
/*   it doesn't matter.                                                      */

#define dissolve(otri)                                                        \
  (otri).tri[(otri).orient] = (triangle) m->dummytri

/* Copy an oriented triangle.                                                */

#define otricopy(otri1, otri2)                                                \
  (otri2).tri = (otri1).tri;                                                  \
  (otri2).orient = (otri1).orient

/* Test for equality of oriented triangles.                                  */

#define otriequal(otri1, otri2)                                               \
  (((otri1).tri == (otri2).tri) &&                                            \
   ((otri1).orient == (otri2).orient))

/* Primitives to infect or cure a triangle with the virus.  These rely on    */
/*   the assumption that all subsegments are aligned to four-byte boundaries.*/

#define infect(otri)                                                          \
  (otri).tri[6] = (triangle)                                                  \
                    ((unsigned ULONGLONG) (otri).tri[6] | (unsigned ULONGLONG) 2l)

#define uninfect(otri)                                                        \
  (otri).tri[6] = (triangle)                                                  \
                    ((unsigned ULONGLONG) (otri).tri[6] & ~ (unsigned ULONGLONG) 2l)

/* Test a triangle for viral infection.                                      */

#define infected(otri)                                                        \
  (((unsigned ULONGLONG) (otri).tri[6] & (unsigned ULONGLONG) 2l) != 0l)

/* Check or set a triangle's attributes.                                     */

#define elemattribute(otri, attnum)                                           \
  ((REAL *) (otri).tri)[m->elemattribindex + (attnum)]

#define setelemattribute(otri, attnum, value)                                 \
  ((REAL *) (otri).tri)[m->elemattribindex + (attnum)] = value

/* Check or set a triangle's maximum area bound.                             */

#define areabound(otri)  ((REAL *) (otri).tri)[m->areaboundindex]

#define setareabound(otri, value)                                             \
  ((REAL *) (otri).tri)[m->areaboundindex] = value

/* Check or set a triangle's deallocation.  Its second pointer is set to     */
/*   NULL to indicate that it is not allocated.  (Its first pointer is used  */
/*   for the stack of dead items.)  Its fourth pointer (its first vertex)    */
/*   is set to NULL in case a `badtriang' structure points to it.            */

#define deadtri(tria)  ((tria)[1] == (triangle) NULL)

#define killtri(tria)                                                         \
  (tria)[1] = (triangle) NULL;                                                \
  (tria)[3] = (triangle) NULL

/********* Primitives for subsegments                                *********/
/*                                                                           */
/*                                                                           */

/* sdecode() converts a pointer to an oriented subsegment.  The orientation  */
/*   is extracted from the least significant bit of the pointer.  The two    */
/*   least significant bits (one for orientation, one for viral infection)   */
/*   are masked out to produce the real pointer.                             */

#define sdecode(sptr, osub)                                                   \
  (osub).ssorient = (int) ((unsigned ULONGLONG) (sptr) & (unsigned ULONGLONG) 1l);      \
  (osub).ss = (subseg *)                                                      \
              ((unsigned ULONGLONG) (sptr) & ~ (unsigned ULONGLONG) 3l)

/* sencode() compresses an oriented subsegment into a single pointer.  It    */
/*   relies on the assumption that all subsegments are aligned to two-byte   */
/*   boundaries, so the least significant bit of (osub).ss is zero.          */

#define sencode(osub)                                                         \
  (subseg) ((unsigned ULONGLONG) (osub).ss | (unsigned ULONGLONG) (osub).ssorient)

/* ssym() toggles the orientation of a subsegment.                           */

#define ssym(osub1, osub2)                                                    \
  (osub2).ss = (osub1).ss;                                                    \
  (osub2).ssorient = 1 - (osub1).ssorient

#define ssymself(osub)                                                        \
  (osub).ssorient = 1 - (osub).ssorient

/* spivot() finds the other subsegment (from the same segment) that shares   */
/*   the same origin.                                                        */

#define spivot(osub1, osub2)                                                  \
  sptr = (osub1).ss[(osub1).ssorient];                                        \
  sdecode(sptr, osub2)

#define spivotself(osub)                                                      \
  sptr = (osub).ss[(osub).ssorient];                                          \
  sdecode(sptr, osub)

/* snext() finds the next subsegment (from the same segment) in sequence;    */
/*   one whose origin is the input subsegment's destination.                 */

#define snext(osub1, osub2)                                                   \
  sptr = (osub1).ss[1 - (osub1).ssorient];                                    \
  sdecode(sptr, osub2)

#define snextself(osub)                                                       \
  sptr = (osub).ss[1 - (osub).ssorient];                                      \
  sdecode(sptr, osub)

/* These primitives determine or set the origin or destination of a          */
/*   subsegment or the segment that includes it.                             */

#define sorg(osub, vertexptr)                                                 \
  vertexptr = (vertex) (osub).ss[2 + (osub).ssorient]

#define sdest(osub, vertexptr)                                                \
  vertexptr = (vertex) (osub).ss[3 - (osub).ssorient]

#define setsorg(osub, vertexptr)                                              \
  (osub).ss[2 + (osub).ssorient] = (subseg) vertexptr

#define setsdest(osub, vertexptr)                                             \
  (osub).ss[3 - (osub).ssorient] = (subseg) vertexptr

#define segorg(osub, vertexptr)                                               \
  vertexptr = (vertex) (osub).ss[4 + (osub).ssorient]

#define segdest(osub, vertexptr)                                              \
  vertexptr = (vertex) (osub).ss[5 - (osub).ssorient]

#define setsegorg(osub, vertexptr)                                            \
  (osub).ss[4 + (osub).ssorient] = (subseg) vertexptr

#define setsegdest(osub, vertexptr)                                           \
  (osub).ss[5 - (osub).ssorient] = (subseg) vertexptr

/* These primitives read or set a boundary marker.  Boundary markers are     */
/*   used to hold user-defined tags for setting boundary conditions in       */
/*   finite element solvers.                                                 */

#define mark(osub)  (* (int *) ((osub).ss + 8))

#define setmark(osub, value)                                                  \
  * (int *) ((osub).ss + 8) = value

/* Bond two subsegments together.                                            */

#define sbond(osub1, osub2)                                                   \
  (osub1).ss[(osub1).ssorient] = sencode(osub2);                              \
  (osub2).ss[(osub2).ssorient] = sencode(osub1)

/* Dissolve a subsegment bond (from one side).  Note that the other          */
/*   subsegment will still think it's connected to this subsegment.          */

#define sdissolve(osub)                                                       \
  (osub).ss[(osub).ssorient] = (subseg) m->dummysub

/* Copy a subsegment.                                                        */

#define subsegcopy(osub1, osub2)                                              \
  (osub2).ss = (osub1).ss;                                                    \
  (osub2).ssorient = (osub1).ssorient

/* Test for equality of subsegments.                                         */

#define subsegequal(osub1, osub2)                                             \
  (((osub1).ss == (osub2).ss) &&                                              \
   ((osub1).ssorient == (osub2).ssorient))

/* Check or set a subsegment's deallocation.  Its second pointer is set to   */
/*   NULL to indicate that it is not allocated.  (Its first pointer is used  */
/*   for the stack of dead items.)  Its third pointer (its first vertex)     */
/*   is set to NULL in case a `badsubseg' structure points to it.            */

#define deadsubseg(sub)  ((sub)[1] == (subseg) NULL)

#define killsubseg(sub)                                                       \
  (sub)[1] = (subseg) NULL;                                                   \
  (sub)[2] = (subseg) NULL

/********* Primitives for interacting triangles and subsegments      *********/
/*                                                                           */
/*                                                                           */

/* tspivot() finds a subsegment abutting a triangle.                         */

#define tspivot(otri, osub)                                                   \
  sptr = (subseg) (otri).tri[6 + (otri).orient];                              \
  sdecode(sptr, osub)

/* stpivot() finds a triangle abutting a subsegment.  It requires that the   */
/*   variable `ptr' of type `triangle' be defined.                           */

#define stpivot(osub, otri)                                                   \
  ptr = (triangle) (osub).ss[6 + (osub).ssorient];                            \
  decode(ptr, otri)

/* Bond a triangle to a subsegment.                                          */

#define tsbond(otri, osub)                                                    \
  (otri).tri[6 + (otri).orient] = (triangle) sencode(osub);                   \
  (osub).ss[6 + (osub).ssorient] = (subseg) encode(otri)

/* Dissolve a bond (from the triangle side).                                 */

#define tsdissolve(otri)                                                      \
  (otri).tri[6 + (otri).orient] = (triangle) m->dummysub

/* Dissolve a bond (from the subsegment side).                               */

#define stdissolve(osub)                                                      \
  (osub).ss[6 + (osub).ssorient] = (subseg) m->dummytri

/********* Primitives for vertices                                   *********/
/*                                                                           */
/*                                                                           */

#define vertexmark(vx)  ((int *) (vx))[m->vertexmarkindex]

#define setvertexmark(vx, value)                                              \
  ((int *) (vx))[m->vertexmarkindex] = value

#define vertextype(vx)  ((int *) (vx))[m->vertexmarkindex + 1]

#define setvertextype(vx, value)                                              \
  ((int *) (vx))[m->vertexmarkindex + 1] = value

#define vertex2tri(vx)  ((triangle *) (vx))[m->vertex2triindex]

#define setvertex2tri(vx, value)                                              \
  ((triangle *) (vx))[m->vertex2triindex] = value

/* To insert lots of self-checks for internal errors, define the SELF_CHECK  */
/*   symbol.  This will slow down the program significantly.  It is best to  */
/*   define the symbol using the -DSELF_CHECK compiler switch, but you could */
/*   write "#define SELF_CHECK" below.  If you are modifying this code, I    */
/*   recommend you turn self-checks on until your work is debugged.          */

/* #define SELF_CHECK */

/* To compile Triangle as a callable object library (triangle.o), define the */
/*   TRILIBRARY symbol.  Read the file triangle.h for details on how to call */
/*   the procedure triangulate() that results.                               */

/* #define TRILIBRARY */

/* On some machines, my exact arithmetic routines might be defeated by the   */
/*   use of internal extended precision floating-point registers.  The best  */
/*   way to solve this problem is to set the floating-point registers to use */
/*   single or double precision internally.  On 80x86 processors, this may   */
/*   be accomplished by setting the CPU86 symbol for the Microsoft C         */
/*   compiler, or the LINUX symbol for the gcc compiler running on Linux.    */
/*                                                                           */
/* An inferior solution is to declare certain values as `volatile', thus     */
/*   forcing them to be stored to memory and rounded off.  Unfortunately,    */
/*   this solution might slow Triangle down quite a bit.  To use volatile    */
/*   values, write "#define INEXACT volatile" below.  Normally, however,     */
/*   INEXACT should be defined to be nothing.  ("#define INEXACT".)          */
/*                                                                           */
/* For more discussion, see http://www.cs.cmu.edu/~quake/robust.pc.html .    */
/*   For yet more discussion, see Section 5 of my paper, "Adaptive Precision */
/*   Floating-Point Arithmetic and Fast Robust Geometric Predicates" (also   */
/*   available as Section 6.6 of my dissertation).                           */

#define INEXACT /* Nothing */
/* #define INEXACT volatile */

/* Maximum number of characters in a file name (including the null).         */

#define FILENAMESIZE 2048

/* Maximum number of characters in a line read from a file (including the    */
/* null).                                                                    */

#define INPUTLINESIZE 1024

/* For efficiency, a variety of data structures are allocated in bulk.  The  */
/*   following constants determine how many of each structure is allocated   */
/*   at once.                                                                */

#define TRIPERBLOCK 4092           /* Number of triangles allocated at once. */
#define SUBSEGPERBLOCK 508       /* Number of subsegments allocated at once. */
#define VERTEXPERBLOCK 4092         /* Number of vertices allocated at once. */
#define VIRUSPERBLOCK 1020   /* Number of virus triangles allocated at once. */
/* Number of encroached subsegments allocated at once. */
#define BADSUBSEGPERBLOCK 252
/* Number of skinny triangles allocated at once. */
#define BADTRIPERBLOCK 4092
/* Number of flipped triangles allocated at once. */
#define FLIPSTACKERPERBLOCK 252
/* Number of splay tree nodes allocated at once. */
#define SPLAYNODEPERBLOCK 508

/* The vertex types.   A DEADVERTEX has been deleted entirely.  An           */
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

#endif // PRIMITIVES_H
