#ifndef MESHER_H
#define MESHER_H

#include "morfeus.h"

class Geometry;
class Mesh;
class MeshRefinement;

class Mesher
{
public:
  MORFEUS_LIB_DECL Mesher();
  MORFEUS_LIB_DECL virtual ~Mesher();

public:
  MORFEUS_LIB_DECL void createMesh(const Geometry & geometry, Mesh & mesh);
  MORFEUS_LIB_DECL void refineMesh(const Mesh & oldMesh, const MeshRefinement & refinement, Mesh & mesh);

private:
  double mMaxArea;
  double mMinArea;
};

#endif // MESHER_H
