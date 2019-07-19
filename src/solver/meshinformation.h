#ifndef MESHINFORMATION_H
#define MESHINFORMATION_H

#include "morfeus.h"

#include <vector>

class Face;
class Mesh;

class MeshInformation
{
public:
  MORFEUS_LIB_DECL MeshInformation();

public:
  MORFEUS_LIB_DECL const std::vector<const Face*> & boundaryFaces() const;
  MORFEUS_LIB_DECL std::size_t totalBoundaryUnknowns() const;
  MORFEUS_LIB_DECL std::size_t totalUnknowns() const;

public:
  MORFEUS_LIB_DECL void analyzeMesh(const Mesh * mesh);

private:
  void identifyBoundaryFaces(const Mesh * mesh);
  void init();
  void markMeshUnknowns(const Mesh * mesh);

private:
  std::size_t mTotalBoundaryUnknowns;
  std::size_t mTotalUnknowns;
  std::vector<const Face*> mBoundaryFaces;
};

inline const std::vector<const Face*> & MeshInformation::boundaryFaces() const
{
  return mBoundaryFaces;
}

inline std::size_t MeshInformation::totalBoundaryUnknowns() const
{
  return mTotalBoundaryUnknowns;
}
inline std::size_t MeshInformation::totalUnknowns() const
{
  return mTotalUnknowns;
}

#endif // MESHINFORMATION_H
